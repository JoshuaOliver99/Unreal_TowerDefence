// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TowerDefenceGameMode.generated.h"

class UUW_TowerDefenceHUD;
class ATower;
class ATowerDefencePlayerController;
class ACharacterEnemy;
class AEnemySpawn;
class AItem;

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
	
protected:
	// Native event for when play begins for this actor
	virtual void BeginPlay() override;

	// Called upon all towers becoming destroyed
	void GameOver();

	// Initialises all starting values
	void HandleGameStart();
	
	// TODO: Implement
	UFUNCTION()
	void ActorTakenDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

private:
	UPROPERTY()
	ATowerDefencePlayerController* PlayerController;

	enum EGameState
	{
		Ended,
		InPlay,
		BetweenRounds
	};
	EGameState GameState;

	// TODO: Implement - A cutscene style pause
	//UPROPERTY()
	//float StartDelay = 3.f;

	
// ----- Player Inventory
public:
	int GetPlayerGold() const { return PlayerGold; }
	void SetPlayerGold(const int GoldTotal);
	void AddPlayerGold(const int GoldAmount);
	
private:
	int PlayerGold;
	
// ----- Towers
public:
	TArray<ATower*> GetTowers() const {return Towers;}
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	TArray<ATower*> Towers;


// ----- Enemy
private:
	UPROPERTY(EditAnywhere, Category = "Gameplay")
	TArray<TSubclassOf<ACharacterEnemy>> SpawnableEnemyTypes;

	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	TArray<AEnemySpawn*> EnemySpawnPoints;
	
	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	int32 DifficultyBudget = 0;

	int32 EnemyCount = 0;
	
	int32 CalculateEnemyCount() const;


// ----- Wave properties
private:	
	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	int32 Wave = 1;
	
	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float WaveDifficultyMultiplier = 110;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float WaveStartDelay = 30.0f;
	
	float TimeUntilWaveStart;

	FTimerHandle WaveCountdownTimerDelegate;
	
	// Begins the wave of Wave
	void BeginWave();

	// Called when a wave is successfully completed 
	void WaveCompleted();

	// A Countdown to handle events in-between waves
	void WaveCountdownDelegate();
	

// ----- Shop
public:
	TArray<TSubclassOf<AItem>> GetShopItems() const { return ShopItems; }

private:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AItem>> SpawnableShopItems;
	
	TArray<TSubclassOf<AItem>>  ShopItems;
	
	void GenerateShopStock();
	

// ----- UI
private:
	UUW_TowerDefenceHUD* GetHUD() const;
	
	void UpdateTitleText(FString Text);
	void ClearTitleText();
	
	void UpdatePlayerHealthBar();

	void UpdateTowerHealthBar();

	void UpdateGoldText(int GoldBalance);
	
	void UpdateWaveText(int CurrentWave);

	void UpdateAllUI();
};

