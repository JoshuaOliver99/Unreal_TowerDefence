// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterEnemy.h"
#include "CharacterEnemy_Kamikaze.generated.h"

/**
 * Contains Kamikaze() to cause this Actor to ApplyRadialDamageWithFalloff() just before Destroying.
 */
UCLASS()
class UNREALHYPERCASUAL_API ACharacterEnemy_Kamikaze : public ACharacterEnemy
{
	GENERATED_BODY()

public:
	// Causes this Enemy to do radial damage then destroy
	void Kamikaze();

private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	float BaseDamage = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MinimumDamage = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DamageInnerRadius = 500.0f; 

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DamageOuterRadius = 200.0f; 

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DamageFalloff;
};
