// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponRanged.h"

#include "Projectile.h"

AWeaponRanged::AWeaponRanged()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Default Components
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(WeaponMesh);
}

void AWeaponRanged::UseWeapon()
{
	Super::UseWeapon();

	if (ProjectileClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s ARangedWeapon::UseWeapon] ProjectileClass is null!"), *GetActorNameOrLabel())
		return;
	}
	
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileClass,
		ProjectileSpawnPoint->GetComponentLocation(),
		ProjectileSpawnPoint->GetComponentRotation());

	Projectile->SetOwner(this);
}
