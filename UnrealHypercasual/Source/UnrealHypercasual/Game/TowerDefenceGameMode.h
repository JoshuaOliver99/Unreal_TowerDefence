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
	// Pass the actor who dies to dictate gameplay function
	void ActorDied(AActor* DeadActor);

	// Return the active tower
	class ATower* GetTower() const {return Tower;}
	
protected:
	// Native event for when play begins for this actor
	virtual void BeginPlay() override;
	
	//UFUNCTION(BlueprintImplementableEvent)
	//void StartGame();

	// Called upon all towers becoming destroyed
	void GameOver();
	
private:

	// TODO: Make this an array to allow for multiple towers. Maybe order matters?
	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	class ATower* Tower;

	UPROPERTY()
	class ATowerDefencePlayerController* PlayerController;

	//float StartDelay = 3.f;
	void HandleGameStart();

	int32 GetTargetEnemyCount();
	int32 EnemyCount = 0;



	// TODO: Review these additions...
	
	// Enemy Spawning...

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	TArray<TSubclassOf<class ACharacterEnemy>> SpawnableEnemyTypes;

	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	TArray<class AEnemySpawn*> EnemySpawnPoints;
	
	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	int32 DifficultyBudget = 0;
	
	// Game properties...

	enum EGameState
	{
		Ended,
		InPlay,
		BetweenRounds
	};
	EGameState GameState;
	
	// Wave properties...
	
	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	int32 Wave = 1;
	
	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float WaveDifficultyMultiplier = 110;

	// Wave delay...
	
	float WaveStartDelay = 30.0f;
	float TimeUntilWaveStart;

	FTimerHandle WaveCountdownTimerDelegate;
	
	// Wave functions...

	// Begins the wave
	void BeginWave(int WaveNumber);

	// Called when a Wave is successfully completed 
	void WaveCompleted();

	// ...
	void HandleBeginWave();

	
};
