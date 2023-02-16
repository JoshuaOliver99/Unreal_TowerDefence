// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterFriendly.h"
#include "CharacterFriendly_Turret.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHYPERCASUAL_API ACharacterFriendly_Turret : public ACharacterFriendly
{
	GENERATED_BODY()

public:
	ACharacterFriendly_Turret();



	
private:
	// TODO
	// Check what's needed here. I copy/pasted
	UPROPERTY(EditInstanceOnly, Category = "Combat")
	float FireRange = 1000.f;

	UPROPERTY(EditInstanceOnly, Category = "Combat")
	float FireRate = 2.0f;
	
	FTimerHandle FireRateTimerHandle;
	
	void CheckFireCondition();

	bool InFireRange() const;

	// ----- Enemy Interaction
	class AActor* Target;
	void GetClosestEnemy();
	
	FTimerHandle UpdateEnemiesArray;
	float UpdateEnemiesRate = 0.5f;
	bool IsCheckingForEnemy = false;
};
