// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerDefenceGameMode.h"

#include "Enemy.h"
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
	else if (AEnemy* KilledEnemy = Cast<AEnemy>(DeadActor))
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

void ATowerDefenceGameMode::HandleGameStart()
{
	EnemyCount = GetTargetEnemyCount();
	
	//Tower = Cast<ATower>(UGameplayStatics::GetPlayerPawn(this, 0));

	// Get the level Tower
	Tower = Cast<ATower>(UGameplayStatics::GetActorOfClass(GetWorld(), ATower::StaticClass()));

	PlayerController = Cast<ATowerDefencePlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();
	
	if (PlayerController)
	{
		PlayerController->SetPlayerEnabledState(false);

		// Begin play timer...
		FTimerHandle PlayerEnabledTimerHandle;

		const FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
			PlayerController,
			&ATowerDefencePlayerController::SetPlayerEnabledState,
			true);
		
		GetWorldTimerManager().SetTimer(
			PlayerEnabledTimerHandle,
			PlayerEnableTimerDelegate,
			StartDelay,
			false);
	}
}

int32 ATowerDefenceGameMode::GetTargetEnemyCount()
{
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemy::StaticClass(), Enemies);

	return Enemies.Num();
}
