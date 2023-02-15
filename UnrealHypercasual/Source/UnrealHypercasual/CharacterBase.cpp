// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "HealthComponent.h"
#include "WeaponBase.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Default Components
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

void ACharacterBase::HandleDestruction()
{
	if (HealthComponent)
	{
		HealthComponent->HandleDestruction();
	}
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponClass)
	{
		// Spawn Weapon
		Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);
		// Remove gun mesh from model via bone
		GetMesh()->HideBoneByName(TEXT("weapon_r"), PBO_None);
		// Attach the gun to the skeleton socket
		//RangedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
		// Set owner of the gun (Important for damage and multiplayer)
		Weapon->SetOwner(this);
	}
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterBase::UseWeapon()
{
	if (Weapon)
	{
		Weapon->UseWeapon();
	}
}

