// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerDefenceGameMode.h"

#include "CharacterEnemy.h"
#include "Tower.h"
#include "TowerDefencePlayerController.h"
#include "Kismet/GameplayStatics.h"

void ATowerDefenceGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Tower)
	{
		
		Tower->HandleDestruction();

		if (PlayerController)
		{
			PlayerController->SetPlayerEnabledState(false);
		}
		
		GameOver(false);
	}
	else if (ACharacterEnemy* KilledEnemy = Cast<ACharacterEnemy>(DeadActor))
	{
		KilledEnemy->HandleDestruction();
		
		EnemyCount--;
		if (EnemyCount <= 0)
			GameOver(true);
	}
}

void ATowerDefenceGameMode::BeginPlay()
{
	Super::BeginPlay();
	HandleGameStart();
}

void ATowerDefenceGameMode::GameOver(bool bWonGame)
{
	if (bWonGame)
	{
		UE_LOG(LogTemp, Warning, TEXT("Round Cleared!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Tower Destroyed! Game Over..."));
	}
}

void ATowerDefenceGameMode::HandleGameStart()
{
	EnemyCount = GetTargetEnemyCount();
	
	// Get the level Tower
	Tower = Cast<ATower>(UGameplayStatics::GetActorOfClass(GetWorld(), ATower::StaticClass()));

	PlayerController = Cast<ATowerDefencePlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();

	// TODO: Better implement a game start function
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
}

int32 ATowerDefenceGameMode::GetTargetEnemyCount()
{
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(this, ACharacterEnemy::StaticClass(), Enemies);

	return Enemies.Num();
}
