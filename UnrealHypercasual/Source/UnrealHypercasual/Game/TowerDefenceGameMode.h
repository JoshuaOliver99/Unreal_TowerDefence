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



// ----- Gameplay
protected:
	// Native event for when play begins for this actor
	virtual void BeginPlay() override;

	// Initialises all starting values
	void HandleGameStart();
	
	// Called upon all towers becoming destroyed
	void GameLost();
	
	// Delegate bound to all Towers and the Players so we are aware of a game ending
	UFUNCTION()
	void ActorTakenDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

public:
	// Pass the actor who dies to dictate gameplay function
	void ActorDied(AActor* DeadActor);
	
protected:
	UPROPERTY()
	ATowerDefencePlayerController* PlayerController;

	enum EGameState
	{
		PreGame,		// The moment before the actual game has started.
		GameIsLost,		// When the game is lost because the tower or player die.
		GameIsWon,		// (UNIMPLEMENTED) When the game is won.

		PreWave,		// The moment before a wave is about to start.
		WaveActive,		// The moment when a wave is active and in play.
		PostWave		// The moment after a wave has been cleared.
	};
	EGameState GameState = PreGame;



	
// ----- Player Input Action
public:
	// Handle "Select" input and decide an appropriate action (e.g. start the game, skip the countdown)
	void HandlePlayerSelectAction();


	
// ----- Player Inventory
public:
	int GetPlayerGold() const { return PlayerGold; }
	void SetPlayerGold(const int GoldTotal);
	void AddPlayerGold(const int GoldAmount);
	
protected:
	int PlayerGold;



	
	
// ----- Towers
protected:
	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	TArray<ATower*> Towers;

public:
	TArray<ATower*> GetTowers() const {return Towers;}

	
	
// ----- Enemy
protected:
	UPROPERTY(EditAnywhere, Category = "Gameplay")
	TArray<TSubclassOf<ACharacterEnemy>> SpawnableEnemyTypes;

	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	TArray<AEnemySpawn*> EnemySpawnPoints;
	
	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	int32 DifficultyBudget = 0;

	int32 EnemyCount = 0;
	
	int32 CalculateEnemyCount() const;

	FTimerHandle EnemySpawnTimerHandle;

	float EnemySpawnSpeed = 0.5f;
	
	void TrySpawnEnemy();

// ----- Wave properties
protected:	
	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	int32 WaveNum = 1;
	
	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float WaveDifficultyMultiplier = 110;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float WaveStartDelay = 30.0f;
	
	float TimeUntilWaveStart;

	FTimerHandle WaveCountdownTimerDelegate;

	// 1. The time as a wave is waiting to start
	void HandlePreWave();

	// 1.1. A Countdown to handle events in-between waves
	void WaveCountdownDelegate();
	
	// 2. The controller of the current wave WaveNum 
	void HandleWave();
	
	// 3. Called when a wave is successfully completed 
	void HandlePostWave();


	

// ----- Shop
// TODO: NOTE: maybe this global shop should instead be independent to the shop instance
	
protected:
	// The type of items which the shop can spawn
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AItem>> SpawnableShopItems;

	// The current shop stock
	TArray<TSubclassOf<AItem>>  ShopItems;
	
	void GenerateShopStock();

public:
	// Returns all ShopItems, the current stock in the stop
	TArray<TSubclassOf<AItem>> GetShopItems() const { return ShopItems; }

	// Remove a shop item (e.g. when sold)
	void RemoveShopItem(TSubclassOf<AItem> ItemToRemove);

	
// ----- UI
protected:
	// Attempts to get the Players UUW_TowerDefenceHUD
	UUW_TowerDefenceHUD* GetHUD() const;
	
	void UpdateTitleText(FString Text);
	void ClearTitleText();

	void UpdateSubtitleText(FString Text);

	void UpdateBottomText(FString Text);
	
	void UpdatePlayerHealthBar();

	void UpdateTowerHealthBar();

	void UpdateGoldText(int GoldBalance);
	
	void UpdateWaveText(int CurrentWave);

	void UpdateAllUI();

	void SetUIGameOver();
};

