// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerDefenceGameMode.h"

#include "EnemySpawn.h"
#include "Tower.h"
#include "TowerDefencePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "UnrealHypercasual/UW_TowerDefenceHUD.h"
#include "UnrealHypercasual/Characters/CharacterEnemy.h"


void ATowerDefenceGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Tower)
	{
		
		Tower->HandleDestruction();

		if (PlayerController)
		{
			PlayerController->SetPlayerEnabledState(false);
		}

		// Lose
		GameOver();
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
			if (GameState != InPlay)
			{
				GameOver();
				return;
			}
			
			WaveCompleted();
		}
	}
}

void ATowerDefenceGameMode::BeginPlay()
{
	Super::BeginPlay();
	HandleGameStart();
}

void ATowerDefenceGameMode::GameOver()
{
	GameState = EGameState::Ended;

	// TODO: Replace this with the GameOver Widget Screen
	//UpdateTitleText(FString::Printf(TEXT("Game Over!")));
	
	// TODO: Add an option for reloading the level
}

void ATowerDefenceGameMode::HandleGameStart()
{
	// Get PlayerController*
	PlayerController = Cast<ATowerDefencePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	
	// Get Tower*
	Tower = Cast<ATower>(UGameplayStatics::GetActorOfClass(GetWorld(), ATower::StaticClass()));

	// Get EnemySpawnPoints*
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawn::StaticClass(), SpawnPoints);
	EnemySpawnPoints.Empty();
	for (int i = 0; i < SpawnPoints.Num(); ++i)
	{
		EnemySpawnPoints.Add(Cast<AEnemySpawn>(SpawnPoints[i]));	
	}
	if (SpawnPoints.Num() < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s No EnemySpawnPoints found!"), *GetActorNameOrLabel())
	}


	GenerateShopStock();
	
	Wave = 1;

	// Ensure other Actors are initialised before beginning (5s initial start delay)
	FTimerHandle DelayBeginWaveTimerHandle;
	GetWorldTimerManager().SetTimer(DelayBeginWaveTimerHandle, this, &ATowerDefenceGameMode::BeginWave, 5.0f, false);
}


int32 ATowerDefenceGameMode::GetTargetEnemyCount() const
{
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(this, ACharacterEnemy::StaticClass(), Enemies);

	return Enemies.Num();
}


void ATowerDefenceGameMode::BeginWave()
{
	GameState = EGameState::InPlay;

	
	UpdateTitleText(FString::Printf(TEXT("Wave Incoming!")));
	UpdateWaveText(Wave);

	// Clear Title Text after 3s
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
			
			// Check if affordable
			if (DifficultyBudget >= EnemyInstance->GetEnemySpawnCost())
			{
				// Spawn
				if (GetWorld()->SpawnActor(EnemyClass, SpawnPointLocation))
				{
					// Deduct cost
					DifficultyBudget -= EnemyInstance->GetEnemySpawnCost();
					
					break;
				}
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

void ATowerDefenceGameMode::UpdatePlayerHealthBar(float PlayerHealthPercentage)
{
	if (UUW_TowerDefenceHUD* TowerDefenceHUD = GetHUD())
	{
		TowerDefenceHUD->UpdatePlayerHealthBar(PlayerHealthPercentage);
	}
}

void ATowerDefenceGameMode::UpdateTowerHealthBar(float TowerHealthPercentage)
{
	if (UUW_TowerDefenceHUD* TowerDefenceHUD = GetHUD())
	{
		TowerDefenceHUD->UpdateTowerHealthBar(TowerHealthPercentage);
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
#pragma endregion UI



#pragma region Player
void ATowerDefenceGameMode::SetPlayerGold(const int GoldTotal)
{
	PlayerGold = GoldTotal;
}

void ATowerDefenceGameMode::AddPlayerGold(const int GoldAmount)
{
	PlayerGold += GoldAmount;
}
#pragma endregion Player


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