// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendlyCharacter_Turret.h"

#include "RangedWeapon.h"

AFriendlyCharacter_Turret::AFriendlyCharacter_Turret()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AFriendlyCharacter_Turret::Fire()
{
	RangedWeapon->Fire();
}

void AFriendlyCharacter_Turret::BeginPlay()
{
	Super::BeginPlay();

	// Spawn gun
	RangedWeapon = GetWorld()->SpawnActor<ARangedWeapon>(RangedWeaponClass);
	// Remove gun mesh from model via bone
	GetMesh()->HideBoneByName(TEXT("weapon_r"), PBO_None);
	// Attach the gun to the skeleton socket
	//RangedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	RangedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	// Set owner of the gun (Important for damage and multiplayer)
	RangedWeapon->SetOwner(this);
}
