// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Turret.generated.h"

class UAIPerceptionStimuliSourceComponent;
class UHealthComponent;
class UInteractable;

/**
 * 
 */
UCLASS()
class UNREALHYPERCASUAL_API ATurret : public ABasePawn
{
	GENERATED_BODY()
	
public:
	// Sets default values for this pawn's properties
	ATurret();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void HandleDestruction() override;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RotateTurret(const FVector& LookAtTarget) const;

	void Fire();


// ---------- Components
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UAIPerceptionStimuliSourceComponent* PerceptionStimuliComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UInteractable* InteractableComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	
// ---------- Fire Settings
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere)
	float FireRange = 1000.f;

	UPROPERTY(EditAnywhere)
	float FireRate = 2.0f;

	
	FTimerHandle FireRateTimerHandle;
	
	void CheckFireCondition();

	bool InFireRange() const;

	
// ---------- AI - Enemy Interaction
// TODO: Remove: This is a temporary implementation: should be moved into Controller & BT
protected:
	AActor* Target;

	FTimerHandle UpdateEnemiesArray;
	float UpdateEnemiesRate = 0.5f;
	bool IsCheckingForEnemy = false;
	
	// Delegate bound to Enemy OnDestroyed to identify that a new target is needed
	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);

	void GetClosestEnemy();

	
// ---------- Turret Movement
protected:
	// The max angle in degree which the turret can look up and down
	UPROPERTY(EditAnywhere)
	float TurretPitchLimit = 30.0f;

	// The speed in which the turret takes to look at it's target
	UPROPERTY(EditAnywhere)
	float TurretRotationSpeed = 5.0f;
};
