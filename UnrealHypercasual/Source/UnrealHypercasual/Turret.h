// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Turret.generated.h"

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

	// Delegate bound to Enemy OnDestroyed
	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RotateTurret(const FVector& LookAtTarget) const;

	void Fire();
	
private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* ProjectileSpawnPoint;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class AProjectile> ProjectileClass;

	// ----- Fire Settings
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
