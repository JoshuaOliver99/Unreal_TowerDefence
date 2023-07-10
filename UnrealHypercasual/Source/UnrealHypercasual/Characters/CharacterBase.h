// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class UNREALHYPERCASUAL_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* PerceptionStimuliSourceComponentComponent;


	
// ----- Health
public:
	// To be called whenever this Character is destroyed
	virtual void HandleDestruction();
	
	UFUNCTION(BlueprintPure)
	bool GetAliveStatus() const;

	UFUNCTION(BlueprintPure)
	float GetHealthValue() const;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;
	

// ----- Weapons
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AWeaponBase> WeaponClass;

	UPROPERTY(BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class AWeaponBase* Weapon;

	// TODO - Implement weapon offset within the character itself as this is individual
	//FVector WeaponPositionOffset;
	//FRotator WeaponRotationOffset;

public:
	UFUNCTION(BlueprintCallable)
	virtual void UseWeapon();

	
	UPROPERTY()
	bool IsAttacking = false;

	UFUNCTION(BlueprintPure)
	bool GetAttackingStatus() const {return IsAttacking;}

	void StopAttacking() {IsAttacking = false;}

	
// ----- TODO REVIEW / Move to Controller?
protected:
    // The range this AI aggregates
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    float AggressionRange = 500.0f;

    // The range this AI attacks
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    float AttackRange = 200.0f;

public:
    UFUNCTION()
    float GetAggressionRange() const { return AggressionRange; }

    UFUNCTION()
    float GetAttackRange() const { return AttackRange; }
};
