// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnDamageTaken, AActor*, DamagedActor, float, Damage, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALHYPERCASUAL_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	
	float GetHealth() const {return Health;}
	float GetMaxHealth() const {return MaxHealth;}
	float GetHealthPercentage() const {return (Health / MaxHealth) * 100;}

	// Initiate distruction
	virtual void HandleDestruction();

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	
// ----- Settings
protected:
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.f;
	
	UPROPERTY(VisibleInstanceOnly, Category = "Health")
	float Health = 0.f;

	UPROPERTY(EditAnywhere, Category = "Health")
	UParticleSystem* DeathParticles;

	UPROPERTY(EditAnywhere, Category = "Health")
	USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = "Health")
	TSubclassOf<UCameraShakeBase> DeathCameraShakeClass;

// ----- References
protected:
	UPROPERTY()
	class ATowerDefenceGameMode* TowerDefenceGameMode;

	AActor* Owner;

private:
	// Function delegated to the owner taking damage
	UFUNCTION()
	void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* Instigator, AActor* DamageCauser );

public:
	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnDamageTaken OnDamageTaken;
};
