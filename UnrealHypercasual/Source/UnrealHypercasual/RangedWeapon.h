// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "RangedWeapon.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHYPERCASUAL_API ARangedWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARangedWeapon();

	void Fire();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* ProjectileSpawnPoint;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRange = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 2.0f;
};
