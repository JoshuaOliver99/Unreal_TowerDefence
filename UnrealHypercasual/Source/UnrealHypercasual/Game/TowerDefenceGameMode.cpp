// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerDefenceGameMode.h"

#include "EnemySpawn.h"
#include "Tower.h"
#include "TowerDefencePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"
#include "UnrealHypercasual/UW_TowerDefenceHUD.h"
#include "UnrealHypercasual/Characters/CharacterEnemy.h"
#include "UnrealHypercasual/Health/HealthComponent.h"


void ATowerDefenceGameMode::BeginPlay()
{
	Super::BeginPlay();
	HandleGameStart();

}

#pragma region Gameplay Stages
void ATowerDefenceGameMode::HandleGameStart()
{
	// ----- Get PlayerController*
	PlayerController = Cast<ATowerDefencePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	
	// ----- Get Towers*
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
	
	
	// ---- Get EnemySpawnPoints*
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
	Wave = 1;
	GenerateShopStock();

	
	// ----- Initialize UI
	FTimerHandle DelayUIInitializeTimerHandle;
	GetWorldTimerManager().SetTimer(DelayUIInitializeTimerHandle, this, &ATowerDefenceGameMode::UpdateAllUI, 0.01f, false);

	
	// ----- Begin Wave 1
	BeginWave();
}

void ATowerDefenceGameMode::BeginWave()
{
	GameState = EGameState::InPlay;

	
	// ----- Wave UI
	UpdateTitleText(FString::Printf(TEXT("Wave Incoming!")));
	UpdateWaveText(Wave);
	
	FTimerHandle ClearWaveTextTimerHandle;
	GetWorldTimerManager().SetTimer(ClearWaveTextTimerHandle, this, &ATowerDefenceGameMode::ClearTitleText, 3.0f, false);

	
	// Calculate difficulty budget
	DifficultyBudget = Wave * WaveDifficultyMultiplier;

	// Calculate chosen spawn point 
	const int32 SpawnPointIndex = FMath::RandRange(0, EnemySpawnPoints.Num() - 1);
	const FVector* SpawnPointLocation = new FVector(EnemySpawnPoints[SpawnPointIndex]->GetActorLocation());

	// Spawn all enemies...
	// TODO: make this on a timer instead of all instantly
	const int16 SpawnLimit = 100;
	for (int SpawningEnemy = 0; SpawningEnemy < SpawnLimit; ++SpawningEnemy)
	{
		// TODO: make sure that SpawnableEnemyTypes array is ordered by cost!
		
		// Spawn most expensive Enemy...
		for (int i = SpawnableEnemyTypes.Num() - 1; i >= 0; --i)
		{
			TSubclassOf<ACharacterEnemy> EnemyClass = SpawnableEnemyTypes[i].Get();
			ACharacterEnemy* EnemyInstance = EnemyClass.GetDefaultObject();
			
			// Check if affordable && spawnable
			if (DifficultyBudget >= EnemyInstance->GetEnemySpawnCost() && GetWorld()->SpawnActor(EnemyClass, SpawnPointLocation))
			{
				DifficultyBudget -= EnemyInstance->GetEnemySpawnCost();
				break;
			}
		}

		// Repeat if sufficient budget remaining...
		TSubclassOf<ACharacterEnemy> MostExpensiveEnemyClass = SpawnableEnemyTypes[0];
		ACharacterEnemy* MostExpensiveEnemyInstance = MostExpensiveEnemyClass.GetDefaultObject();
		if (DifficultyBudget >= MostExpensiveEnemyInstance->GetEnemySpawnCost())
		{
			continue;
		}
		
		EnemyCount = SpawningEnemy + 1;
		break;
	}                         
	
	// Delete allocated memory... (used "new")
	delete SpawnPointLocation;
}

// TODO: Implement method of beginning the next wave manually (e.g. "press Enter")...																	
void ATowerDefenceGameMode::WaveCompleted()
{
	Wave++;
	
	GameState = EGameState::BetweenRounds;

	AddPlayerGold(10);

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
		BeginWave();
	}
}

void ATowerDefenceGameMode::GameOver()
{
	
	UE_LOG(LogTemp, Warning, TEXT("Game Over"));
	
	GameState = EGameState::Ended;

	PlayerController->SetPlayerEnabledState(false);

	// TODO: Replace this with the GameOver Widget Screen
	//UpdateTitleText(FString::Printf(TEXT("Game Over!")));
	
	// TODO: Add an option for reloading the level
}
#pragma endregion  Gameplay Stages


#pragma region Gameplay Helpers
void ATowerDefenceGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor->GetClass() == ATower::StaticClass())
	{
		for (int i = 0; i < Towers.Num(); ++i)
		{
			if (DeadActor == Towers[i])
			{
				Towers[i]->HandleDestruction();
				Towers.RemoveAt(i);
			}
		}

		if (Towers.Num() < 1 && PlayerController)
		{
			GameOver();
		}


	}
	else if (ACharacterEnemy* KilledEnemy = Cast<ACharacterEnemy>(DeadActor))
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
			
			WaveCompleted();
		}
	}
}

int32 ATowerDefenceGameMode::CalculateEnemyCount() const
{
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(this, ACharacterEnemy::StaticClass(), Enemies);

	return Enemies.Num();
}

void ATowerDefenceGameMode::ActorTakenDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Cast<ATower>(DamagedActor))            
	{                                                              
		UpdateTowerHealthBar();
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
	{
		return;
	}
	
	ShopItems.Empty();

	for (int i = 0; i < 8; ++i)
	{
		const int ItemSpawning  = FMath::RandRange(0, SpawnableShopItems.Num() - 1);
		ShopItems.Add(SpawnableShopItems[ItemSpawning]);
	}
}
#pragma endregion Shop


#pragma region UI
UUW_TowerDefenceHUD* ATowerDefenceGameMode::GetHUD() const
{
	if (PlayerController == nullptr)
	{
		return nullptr;
	}

	return Cast<UUW_TowerDefenceHUD>(PlayerController->GetHud());
}

void ATowerDefenceGameMode::UpdateTitleText(FString Text)
{
	if (UUW_TowerDefenceHUD* TowerDefenceHUD = GetHUD())
	{
		TowerDefenceHUD->UpdateTitleText(Text);
	}
}

void ATowerDefenceGameMode::ClearTitleText()
{
	UpdateTitleText("");
}

void ATowerDefenceGameMode::UpdatePlayerHealthBar()
{
	if (UUW_TowerDefenceHUD* TowerDefenceHUD = GetHUD())
	{
		TowerDefenceHUD->UpdatePlayerHealthBar(0);
	}
}

void ATowerDefenceGameMode::UpdateTowerHealthBar()
{
	if (UUW_TowerDefenceHUD* TowerDefenceHUD = GetHUD())
	{
		float TotalTowerHealthPercentage = 0.0f;
		for (int i = 0; i < Towers.Num(); ++i)
		{
			TotalTowerHealthPercentage += Towers[i]->GetHealthComponent()->GetHealthPercentage();
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
	UpdateWaveText(Wave);
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