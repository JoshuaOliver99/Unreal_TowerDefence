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

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* ProjectileSpawnPoint;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RotateTurret(const FVector& LookAtTarget) const;

	void Fire();
	
private:
	
	class AEnemy* Enemy;
	void GetClosestEnemy();
	
	UPROPERTY(EditInstanceOnly, Category = "Combat")
	float FireRange = 1000.f;

	FTimerHandle UpdateEnemiesArray;
	float UpdateEnemiesRate = 0.5f;
	bool IsCheckingForEnememy = false;
	
	FTimerHandle FireRateTimerHandle;
	float FireRate = 2.0f;

	void CheckFireCondition();

	bool InFireRange() const;
};
