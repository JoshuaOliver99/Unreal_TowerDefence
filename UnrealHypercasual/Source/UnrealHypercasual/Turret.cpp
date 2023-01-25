// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"

// My Includes
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"


ATurret::ATurret()
{
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ATurret::BeginPlay()
{
    Super::BeginPlay();

	GetClosestEnemy();

	// Set a looping timer
	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATurret::CheckFireCondition, FireRate, true);
}


void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InFireRange())
		RotateTurret(Enemy->GetActorLocation());
	else
	{
		if (IsCheckingForEnememy == false)
		{
			IsCheckingForEnememy = true;
			
			// Set a looping timer                                                                                        	      
			GetWorldTimerManager().SetTimer(UpdateEnemiesArray, this, &ATurret::GetClosestEnemy, UpdateEnemiesRate, false);

			
		}
	}
}





void ATurret::RotateTurret(const FVector& LookAtTarget) const
{
	const FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	const FRotator LookAtRotation = FRotator(0.0f, ToTarget.Rotation().Yaw, 0.0f);

	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(
			TurretMesh->GetComponentRotation(), 
			LookAtRotation,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			5.0f));
}

void ATurret::Fire()
{
	FVector ProjectileSpawnPointLocation = ProjectileSpawnPoint->GetComponentLocation();
	DrawDebugSphere(
		GetWorld(),
		ProjectileSpawnPointLocation,
		25.f,
		12,
		FColor::Red,
		false,
		3.0f);
}


void ATurret::GetClosestEnemy()
{	
	// Find all enemies
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundEnemies);
    
	if (!FoundEnemies.IsEmpty())
	{
		// Find the distance to the closest Enemy
		Enemy = Cast<AEnemy>(FoundEnemies[0]);
		for (int32 i = 0; i < FoundEnemies.Num(); i++)
		{
			UE_LOG(LogTemp, Display, TEXT("Enemy Found!"));

			// Update which is the closest enemy (target)
			if (FVector::Dist(GetActorLocation(), FoundEnemies[i]->GetActorLocation()) <
				FVector::Dist(GetActorLocation(), Enemy->GetActorLocation()))
			{
				UE_LOG(LogTemp, Display, TEXT("New closest enemy %s!"), *FoundEnemies[i]->GetActorNameOrLabel() );
				Enemy = Cast<AEnemy>(FoundEnemies[i]);
			}
		}
	}

	IsCheckingForEnememy = false;
}


void ATurret::CheckFireCondition()
{
	if (InFireRange())
		Fire();
}

bool ATurret::InFireRange() const
{ 
	// Check to see if the Enemy is in range
	if (Enemy)
		if (FVector::Dist(GetActorLocation(), Enemy->GetActorLocation()) <= FireRange)
			return true;

	// TODO
	// start a routine which periodically checks for new enemies within range

	
	
	return false;
}
