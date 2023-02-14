// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"

// My Includes
#include "Enemy.h"
#include "Projectile.h"
#include "GenericPlatform/GenericPlatformCrashContext.h"
#include "Kismet/GameplayStatics.h"


ATurret::ATurret()
{
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InFireRange())
	{
		RotateTurret(Enemy->GetActorLocation());
	}
	else if (IsCheckingForEnemy == false)
	{
		IsCheckingForEnemy = true;

		// Begin finding the closest enemy timer
		GetWorldTimerManager().SetTimer(UpdateEnemiesArray, this, &ATurret::GetClosestEnemy, UpdateEnemiesRate, false);
	}
}

void ATurret::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}

void ATurret::OnEnemyDestroyed(AActor* DestroyedActor)
{
	UE_LOG(LogTemp, Warning, TEXT("OnEnemyDestroyed"));

	Enemy = nullptr;
	GetClosestEnemy();
}

void ATurret::BeginPlay()
{
    Super::BeginPlay();
	
	GetClosestEnemy();

	// Begin Fire timer
	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATurret::CheckFireCondition, FireRate, true);
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
	if (ProjectileClass == nullptr)
	{
		return;
	}
	
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
	ProjectileClass,
	ProjectileSpawnPoint->GetComponentLocation(),
	ProjectileSpawnPoint->GetComponentRotation());

	Projectile->SetOwner(this);
}


void ATurret::GetClosestEnemy()
{
	// Find all enemies
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundEnemies);
	
	if (FoundEnemies.IsEmpty() == false)
	{
		// Remove any invalid Enemies
		for (auto FoundEnemy : FoundEnemies)
		{
			if (FoundEnemy->IsActorBeingDestroyed())
			{
				FoundEnemies.Remove(FoundEnemy);
			}
		}

		// Return if now there are NOW no possible targets
		if (FoundEnemies.IsEmpty())
		{
			return;
		}
		
		// Find the closest Enemy
		Enemy = Cast<AEnemy>(FoundEnemies[0]);
		for (int32 i = 0; i < FoundEnemies.Num(); i++)
		{
			if (FVector::Dist(GetActorLocation(), FoundEnemies[i]->GetActorLocation()) < FVector::Dist(GetActorLocation(), Enemy->GetActorLocation()))
			{
				Enemy = Cast<AEnemy>(FoundEnemies[i]);
			}
		}

		// Bind Enemy Delegates
		if (Enemy)
		{
			Enemy->OnDestroyed.AddDynamic(this, &ATurret::OnEnemyDestroyed);
		}
	}
	
	IsCheckingForEnemy = false;
}



void ATurret::CheckFireCondition()
{
	if (Enemy == nullptr)
	{
		return;
	}

	
	if (InFireRange())
	{
		Fire();
	}
}


bool ATurret::InFireRange() const
{ 
	// Check to see if the Enemy is in range
	if (Enemy)
	{
		if (FVector::Dist(GetActorLocation(), Enemy->GetActorLocation()) <= FireRange)
		{
			return true;
		}
	}

	return false;
}
