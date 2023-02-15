// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIControllerBase.h"
#include "FriendlyAIController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHYPERCASUAL_API AFriendlyAIController : public AAIControllerBase
{
	GENERATED_BODY()

public:
	AFriendlyAIController();

	UFUNCTION()
	void OnPerceptionRegistered(const TArray<AActor*>& UpdatedActors);

protected:
	virtual void BeginPlay() override;


	
	// TODO: Remove Debug
private:
	TArray<AActor*> SeenActors;

	AActor* ClosestEnemy;
	void UpdateClosestEnemy();
	FTimerHandle UpdateClosestEnemyTimerHandle;
	float UpdateClosestEnemyRate = 0.5f;

	void UseWeapon();
	FTimerHandle UseWeaponTimerHandle;
	float UseWeaponRate = 0.5f;
};
