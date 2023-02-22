// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALHYPERCASUAL_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetHealth() const {return Health;}

	virtual void HandleDestruction();

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.f;
	
	UPROPERTY(VisibleInstanceOnly, Category = "Health")
	float Health = 0.f;

	UPROPERTY(EditAnywhere, Category = "Health")
	class UParticleSystem* DeathParticles;

	UPROPERTY(EditAnywhere, Category = "Health")
	class USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = "Health")
	TSubclassOf<class UCameraShakeBase> DeathCameraShakeClass;
	
	// Bound to a delegate
	UFUNCTION()
	void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* Instigator, AActor* DamageCauser );

	class ATowerDefenceGameMode* TowerDefenceGameMode;
		
};
