// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"

// My Includes
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "UnrealHypercasual/Characters/CharacterEnemy.h"
#include "UnrealHypercasual/Health/HealthComponent.h"
#include "UnrealHypercasual/Interactables/Interactable.h"
#include "UnrealHypercasual/Weapons/Projectile.h"


ATurret::ATurret()
{
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	PerceptionStimuliComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Perception Stimuli Component"));
	
	InteractableComponent = CreateDefaultSubobject<UInteractable>(TEXT("Interactable Component"));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InFireRange())
	{
		RotateTurret(Target->GetActorLocation());
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
	Target = nullptr;
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

	// Limit Pitch
	FMath::Clamp(ToTarget.Rotation().Pitch, -TurretPitchLimit, TurretPitchLimit);
	
	const FRotator LookAtRotation = FRotator(ToTarget.Rotation().Pitch, ToTarget.Rotation().Yaw, 0.0f);

	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(
			TurretMesh->GetComponentRotation(), 
			LookAtRotation,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			TurretRotationSpeed));
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
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterEnemy::StaticClass(), FoundEnemies);
	
	if (FoundEnemies.IsEmpty() == false)
	{
		// Remove any invalid Enemies 
		for (int32 i = 0; i < FoundEnemies.Num(); i++)
		{
			if (FoundEnemies[i]->IsActorBeingDestroyed())
			{
				FoundEnemies.RemoveAt(i);
			}
		}

		// Return if now there are NOW no possible targets
		if (FoundEnemies.IsEmpty())
		{
			return;
		}
		
		// Find the closest Enemy
		Target = FoundEnemies[0];
		for (int32 i = 0; i < FoundEnemies.Num(); i++)
		{
			if (FVector::Dist(GetActorLocation(), FoundEnemies[i]->GetActorLocation()) < FVector::Dist(GetActorLocation(), Target->GetActorLocation()))
			{
				Target = FoundEnemies[i];
			}
		}

		// Bind Enemy Delegates
		if (Target)
		{
			Target->OnDestroyed.AddDynamic(this, &ATurret::OnEnemyDestroyed);
		}
	}
	
	IsCheckingForEnemy = false;
}



void ATurret::CheckFireCondition()
{
	if (Target == nullptr)
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
	if (Target)
	{
		if (FVector::Dist(GetActorLocation(), Target->GetActorLocation()) <= FireRange)
		{
			return true;
		}
	}

	return false;
}
