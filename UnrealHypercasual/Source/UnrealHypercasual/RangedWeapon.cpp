// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedWeapon.h"

#include "Projectile.h"

ARangedWeapon::ARangedWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Default Components
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(WeaponMesh);
}

void ARangedWeapon::Fire()
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
