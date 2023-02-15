// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FriendlyCharacter.h"
#include "FriendlyCharacter_Turret.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHYPERCASUAL_API AFriendlyCharacter_Turret : public AFriendlyCharacter
{
	GENERATED_BODY()

public:
	AFriendlyCharacter_Turret();
	
	void Fire();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ARangedWeapon> RangedWeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class ARangedWeapon* RangedWeapon;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
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
