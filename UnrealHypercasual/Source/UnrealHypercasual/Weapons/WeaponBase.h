// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class UNREALHYPERCASUAL_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	virtual void UseWeapon();


protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WeaponMesh;


// ----- Animation
protected:
	// The animation called when the weapon is used
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UAnimationAsset* UseWeaponAnimation;
	
	// Invokes the use animation for this weapon
	virtual void DoUseAnimation();

	
// ----- Weapon Cooldown
protected:
	bool bIsAttacking = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float CooldownDuration = 3.0f;

	FTimerHandle CooldownTimerHandle;
	
	virtual void EndCooldown();
};
