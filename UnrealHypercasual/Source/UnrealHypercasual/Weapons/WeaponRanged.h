// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "WeaponRanged.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHYPERCASUAL_API AWeaponRanged : public AWeaponBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponRanged();

	virtual void UseWeapon() override;

	
// ----- Ranged Settings
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* ProjectileSpawnPoint;
	
	// The projectile to be spawned. Note this will dictate damage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class AProjectile> ProjectileClass;


	// TODO: Consider implementing only if ProjectileClass is not set. i.e. use line trace instead.
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	//float FireRange = 1000.f;
	//
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	//float FireRate = 2.0f;
};
