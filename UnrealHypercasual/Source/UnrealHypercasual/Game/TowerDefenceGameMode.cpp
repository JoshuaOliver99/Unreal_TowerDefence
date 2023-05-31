// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerDefenceGameMode.h"

#include "EnemySpawn.h"
#include "Tower.h"
#include "TowerDefencePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"
#include "UnrealHypercasual/Characters/CharacterEnemy.h"
#include "UnrealHypercasual/Health/HealthComponent.h"
#include "UnrealHypercasual/IntentoryAndShop/Item.h"
#include "UnrealHypercasual/UI/UW_TowerDefenceHUD.h"


void ATowerDefenceGameMode::BeginPlay()
{
	Super::BeginPlay();
}

#pragma region Gameplay Stages
void ATowerDefenceGameMode::HandleGameStart()
{
	// ----- Get & Setup Player
	PlayerController = Cast<ATowerDefencePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController)
	{
		if (UHealthComponent* PlayerHealthComponent = Cast<UHealthComponent>(PlayerController->GetPawn()->GetComponentByClass(UHealthComponent::StaticClass())))
		{
			PlayerHealthComponent->OnDamageTaken.AddDynamic(this, &ATowerDefenceGameMode::ActorTakenDamage);
		}
	}

		
	// ----- Get & Setup Towers
	TArray<AActor*> FoundTowers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), FoundTowers);
	Towers.Empty();
	for (int i = 0; i < FoundTowers.Num(); ++i)
	{
		Towers.Add(Cast<ATower>(FoundTowers[i]));
		
		if (Towers[i]->GetHealthComponent())
		{
			Towers[i]->GetHealthComponent()->OnDamageTaken.AddDynamic(this, &ATowerDefenceGameMode::ActorTakenDamage);
		}
	}
	if (Towers.Num() < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s no Towers found!"), *GetActorNameOrLabel())
	}
	

		
	// ---- Get & Setup EnemySpawnPoints
	TArray<AActor*> FoundEnemySpawns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawn::StaticClass(), FoundEnemySpawns);
	EnemySpawnPoints.Empty();
	for (int i = 0; i < FoundEnemySpawns.Num(); ++i)
	{
		EnemySpawnPoints.Add(Cast<AEnemySpawn>(FoundEnemySpawns[i]));	
	}
	if (EnemySpawnPoints.Num() < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s no EnemySpawnPoints found!"), *GetActorNameOrLabel())
	}

	
	// ----- Initialize Game Data
	WaveNum = 1;
	GenerateShopStock();

	
	// ----- Initialize UI
	FTimerHandle DelayUIInitializeTimerHandle;
	GetWorldTimerManager().SetTimer(DelayUIInitializeTimerHandle, this, &ATowerDefenceGameMode::UpdateAllUI, 0.01f, false);

	
	// ----- Begin Wave 1
	HandleWave();
}

void ATowerDefenceGameMode::HandleWave()
{
	GameState = EGameState::WaveActive;

	
	// ----- Wave UI
	UpdateTitleText(FString::Printf(TEXT("Wave Incoming!")));
	UpdateWaveText(WaveNum);
	
	FTimerHandle ClearWaveTextTimerHandle;
	GetWorldTimerManager().SetTimer(ClearWaveTextTimerHandle, this, &ATowerDefenceGameMode::ClearTitleText, 3.0f, false);

	UpdateBottomText(FString::Printf(TEXT("")));

	
	// ----- Calculate difficulty budget
	DifficultyBudget = WaveNum * WaveDifficultyMultiplier;

	// TODO
	// ----- Order the array of enemies, cheapest first
	
	// ----- Start Spawning Enemies
	GetWorldTimerManager().SetTimer(EnemySpawnTimerHandle, this, &ATowerDefenceGameMode::TrySpawnEnemy, EnemySpawnSpeed, true);
}

void ATowerDefenceGameMode::HandlePostWave()
{
	GameState = PostWave;

	// Increase the wave number
	WaveNum++;

	// Give the player gold
	AddPlayerGold(10);

	// Refresh the shop stock every 5 waves
	if (WaveNum % 5 == 0)
		GenerateShopStock();

	
	HandlePreWave();
}

void ATowerDefenceGameMode::HandlePreWave()
{
	GameState = PreWave;

	// ----- Pre-Round
	UpdateBottomText(FString::Printf(TEXT("Press ENTER to skip")));
	
	// ----- Wave Start Delay
	TimeUntilWaveStart = WaveStartDelay;
	GetWorldTimerManager().SetTimer(WaveCountdownTimerDelegate, this, &ATowerDefenceGameMode::WaveCountdownDelegate, 1.0f, true);
}

void ATowerDefenceGameMode::WaveCountdownDelegate()
{
	UpdateTitleText(FString::Printf(TEXT("Time Until Next Wave: %d"), static_cast<int>(TimeUntilWaveStart)));
	
	TimeUntilWaveStart --;
	
	if (TimeUntilWaveStart < 0)
	{
		GetWorldTimerManager().ClearTimer(WaveCountdownTimerDelegate);
		HandleWave();
	}
}

void ATowerDefenceGameMode::GameLost()
{
	GameState = GameIsLost;
	
	PlayerController->GameHasEnded(nullptr, false);
}
#pragma endregion  Gameplay Stages


#pragma region Gameplay Helpers
void ATowerDefenceGameMode::ActorDied(AActor* DeadActor)
{
	if (const ATower* TowerKilled = Cast<ATower>(DeadActor))
	{
		for (int i = 0; i < Towers.Num(); ++i)
		{
			if (TowerKilled == Towers[i])
			{
				Towers[i]->HandleDestruction();
				Towers.RemoveAt(i);
				break;
			}
		}

		if (Towers.Num() < 1 && PlayerController)
		{
			GameLost();
		}

		return;
	}
	
	if (ACharacterEnemy* KilledEnemy = Cast<ACharacterEnemy>(DeadActor))
	{
		KilledEnemy->HandleDestruction();

		// Add player gold.
		// TODO: make this value relative to the enemy spawn cost
		AddPlayerGold(10);
		
		EnemyCount--;
		if (EnemyCount <= 0)
		{
			// Ensure this isn't a false win...
			// TODO: add a timer to ensure the level is actually won!
			//if (GameState != InPlay)
			//{
			//	GameOver();
			//	return;
			//}
			
			HandlePostWave();
		}
	}
}

void ATowerDefenceGameMode::ActorTakenDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Cast<ATower>(DamagedActor))            
	{                                                              
		UpdateTowerHealthBar();
		return;
	}

	if (Cast<ACharacter>(DamagedActor))
	{
		if (DamagedActor == PlayerController->GetPawn())
		{
			UpdatePlayerHealthBar();
			return;
		}
		
		if (Cast<ACharacterEnemy>(DamagedActor))
		{
			return;
		}
	}

	
}

int32 ATowerDefenceGameMode::CalculateEnemyCount() const
{
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(this, ACharacterEnemy::StaticClass(), Enemies);

	return Enemies.Num();
}


void ATowerDefenceGameMode::TrySpawnEnemy()
{	
	// ----- Find what's spawnable
	int16 MostExpensiveSpawnableEnemy = -1;
	for (int i = 0; i < SpawnableEnemyTypes.Num(); ++i)
	{
		// Index out of budget?
		if (SpawnableEnemyTypes[0].GetDefaultObject()->GetEnemySpawnCost() > DifficultyBudget)
			break;
		
		// Save most expensive spawnable
		MostExpensiveSpawnableEnemy = i;
	}

	
	// ----- Check if none Spawnable
	if (MostExpensiveSpawnableEnemy < 0)
	{
		GetWorldTimerManager().ClearTimer(EnemySpawnTimerHandle);
		return;
	}


	// Calculate chosen spawn information
	const int16 RandomEnemyIndex = FMath::RandRange(0, MostExpensiveSpawnableEnemy);
	const TSubclassOf<ACharacterEnemy> EnemyClass = SpawnableEnemyTypes[RandomEnemyIndex].Get();

	const int16 RandomSpawnPointIndex = FMath::RandRange(0, EnemySpawnPoints.Num() - 1);
	const FVector* RandomSpawnPointLocation = new FVector(EnemySpawnPoints[RandomSpawnPointIndex]->GetActorLocation()); // NOTE: FVector* because const* required
	
	
	// Spawn Enemy
	if (GetWorld()->SpawnActor(EnemyClass, RandomSpawnPointLocation))
	{
		DifficultyBudget -= SpawnableEnemyTypes[RandomEnemyIndex].GetDefaultObject()->GetEnemySpawnCost();
		EnemyCount++;
	}

	
	// Delete allocated memory... (used "new")
	delete RandomSpawnPointLocation;
}


void ATowerDefenceGameMode::HandlePlayerSelectAction()
{	
	switch (GameState)
	{
	case EGameState::PreGame:
		UE_LOG(LogTemp, Warning, TEXT("PreGame, Lets go!"));
		HandleGameStart();
		break;
		
	case EGameState::PreWave:
		UE_LOG(LogTemp, Warning, TEXT("PreWave!"));
		GetWorldTimerManager().ClearTimer(WaveCountdownTimerDelegate);
		HandleWave();
		break;

	case EGameState::WaveActive:
		UE_LOG(LogTemp, Warning, TEXT("WaveActive!"));
		break;

	case EGameState::PostWave:
		UE_LOG(LogTemp, Warning, TEXT("PostWave!"));
		break;
		
	case EGameState::GameIsLost:
		UE_LOG(LogTemp, Warning, TEXT("GameLost!"));
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);

	case EGameState::GameIsWon:
		UE_LOG(LogTemp, Warning, TEXT("GameWon!"));
		break;
		
	default:
		UE_LOG(LogTemp, Warning, TEXT("%s Unrecognised EGameState"), *this->GetName());
		break;
	}
}
#pragma endregion  Gameplay Helpers


#pragma region Player
void ATowerDefenceGameMode::SetPlayerGold(const int GoldTotal)
{
	PlayerGold = GoldTotal;
	UpdateGoldText(PlayerGold);
}

void ATowerDefenceGameMode::AddPlayerGold(const int GoldAmount)
{
	PlayerGold += GoldAmount;
	UpdateGoldText(PlayerGold);
}
#pragma endregion Player


#pragma region Shop
void ATowerDefenceGameMode::GenerateShopStock()
{
	if (SpawnableShopItems.Num() == 0)
		return;
	
	ShopItems.Empty();

	for (int i = 0; i < 8; ++i)
	{
		const int ItemSpawning  = FMath::RandRange(0, SpawnableShopItems.Num() - 1);
		ShopItems.Add(SpawnableShopItems[ItemSpawning]);
	}
}

void ATowerDefenceGameMode::RemoveShopItem(TSubclassOf<AItem> ItemToRemove)
{
	if (ShopItems.Num() <= 0)
		return;

	for (int i = 0; i < ShopItems.Num(); ++i)
	{
		if (ShopItems[i] == ItemToRemove)
		{
			ShopItems.RemoveAt(i);
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[%s] Unable to RemoveShopItem()."), *this->GetName())
}

#pragma endregion Shop


#pragma region UI
UUW_TowerDefenceHUD* ATowerDefenceGameMode::GetHUD() const
{
	if (PlayerController == nullptr)
		return nullptr;

	return Cast<UUW_TowerDefenceHUD>(PlayerController->GetHud());
}

void ATowerDefenceGameMode::UpdateTitleText(FString Text)
{
	if (UUW_TowerDefenceHUD* TowerDefenceHUD = GetHUD())
		TowerDefenceHUD->UpdateTitleText(Text);
}

void ATowerDefenceGameMode::ClearTitleText()
{
	UpdateTitleText("");
}

void ATowerDefenceGameMode::UpdateSubtitleText(FString Text)
{
	if (UUW_TowerDefenceHUD* TowerDefenceHUD = GetHUD())
		TowerDefenceHUD->UpdateSubtitleText(Text);
}

void ATowerDefenceGameMode::UpdateBottomText(FString Text)
{
	if (UUW_TowerDefenceHUD* TowerDefenceHUD = GetHUD())
		TowerDefenceHUD->UpdateBottomText(Text);
}

void ATowerDefenceGameMode::UpdatePlayerHealthBar()
{
	if (UUW_TowerDefenceHUD* TowerDefenceHUD = GetHUD())
	{
		if (PlayerController)
		{
			if (UHealthComponent* PlayerHealthComponent = Cast<UHealthComponent>(PlayerController->GetPawn()->GetComponentByClass(UHealthComponent::StaticClass())))
			{

				TowerDefenceHUD->UpdatePlayerHealthBar(PlayerHealthComponent->GetHealthPercentage());
			}
		}
	}
}

void ATowerDefenceGameMode::UpdateTowerHealthBar()
{
	if (UUW_TowerDefenceHUD* TowerDefenceHUD = GetHUD())
	{
		float TotalTowerHealthPercentage = 0.0f;
		for (int i = 0; i < Towers.Num(); ++i)
		{
			if (Towers[i]->GetHealthComponent())
			{
				TotalTowerHealthPercentage += Towers[i]->GetHealthComponent()->GetHealthPercentage();
			}
		}
		
		TowerDefenceHUD->UpdateTowerHealthBar(TotalTowerHealthPercentage / Towers.Num());
	}
}

void ATowerDefenceGameMode::UpdateGoldText(int GoldBalance)
{
	if (UUW_TowerDefenceHUD* TowerDefenceHUD = GetHUD())
	{
		TowerDefenceHUD->UpdateGoldText(GoldBalance);
	}
}

void ATowerDefenceGameMode::UpdateWaveText(int CurrentWave)
{
	if (UUW_TowerDefenceHUD* TowerDefenceHUD = GetHUD())
	{
		TowerDefenceHUD->UpdateWaveText(CurrentWave);
	}
}


void ATowerDefenceGameMode::UpdateAllUI()
{
	// TODO Implement ClearTitleText();
	UpdatePlayerHealthBar();
	UpdateTowerHealthBar();
	UpdateGoldText(PlayerGold);
	UpdateWaveText(WaveNum);
}

void ATowerDefenceGameMode::SetUIGameOver()
{
	if (PlayerController)
	{
		
	}
}
#pragma endregion UI




// TODO: Better implement a game start function
// Remove control from the player for X seconds
//if (PlayerController)
//{
//	PlayerController->SetPlayerEnabledState(false);
//
//	// Begin play timer...
//	FTimerHandle PlayerEnabledTimerHandle;
//
//	const FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
//		PlayerController,
//		&ATowerDefencePlayerController::SetPlayerEnabledState,
//		true);
//	
//	GetWorldTimerManager().SetTimer(
//		PlayerEnabledTimerHandle,
//		PlayerEnableTimerDelegate,
//		StartDelay,
//		false);
//}