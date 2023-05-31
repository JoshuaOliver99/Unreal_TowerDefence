// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIControllerBase.h"
#include "AIControllerFriendly.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHYPERCASUAL_API AAIControllerFriendly : public AAIControllerBase
{
	GENERATED_BODY()

public:
	AAIControllerFriendly();

	// Delegate Called by PerceptionComponent->OnPerceptionUpdated 
	UFUNCTION()
	void OnPerceptionRegistered(const TArray<AActor*>& UpdatedActors);

protected:
	virtual void BeginPlay() override;
	
	// TODO: Note all this can likely be generalised and moved to the parent to avoid duplication
	// e.g. UpdateClosestTarget() which uses a TargetTypeClass 
private:
	TArray<class ACharacterEnemy*> EnemyInSight;

	AActor* ClosestEnemy;
	void UpdateClosestEnemy();
	FTimerHandle UpdateClosestEnemyTimerHandle;
	float UpdateClosestEnemyRate = 0.5f;

	// Delegate Called by ClosestEnemy->OnDestroyed for cleanup 
	UFUNCTION()
	void OnClosestEnemyDestroyed(AActor* DestroyedActor);

	void UseWeapon();
	FTimerHandle UseWeaponTimerHandle;
	float UseWeaponRate = 0.5f;
};
