// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TowerDefenceGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHYPERCASUAL_API ATowerDefenceGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void ActorDied(AActor* DeadActor);

	class ATower* GetTower()
	{
		return Tower;
	}

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	
	void GameOver(bool bWonGame);
	
private:

	UPROPERTY(VisibleAnywhere)
	class ATower* Tower;
	
	class ATowerDefencePlayerController* PlayerController;

	float StartDelay = 3.f;

	void HandleGameStart();

	int32 EnemyCount = 0;
	int32 GetTargetEnemyCount();
};
