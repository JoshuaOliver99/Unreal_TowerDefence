// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "UnrealHypercasual/Health/HealthComponent.h"
#include "UnrealHypercasual/Weapons/WeaponBase.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
	// Create Default Components
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	
	PerceptionStimuliSourceComponentComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Perception Stimuli Source Component"));
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// Spawn Weapon
	if (WeaponClass)
	{
		// Spawn Weapon
		Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);

		// Hide any already existing weapon mesh from model via bone
		//GetMesh()->HideBoneByName(TEXT("weapon_r"), PBO_None);

		// Attach the Weapon to the skeleton socket
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));

		// Set owner of the gun (Important for damage and multiplayer)
		Weapon->SetOwner(this);
	}
}

void ACharacterBase::HandleDestruction()
{
	if (HealthComponent)
	{
		if (Weapon)
		{
			Weapon->Destroy();
		}
		
		HealthComponent->HandleDestruction();
	}
}

bool ACharacterBase::GetAliveStatus() const
{
	if (HealthComponent)
	{
		if (HealthComponent->GetHealth() > 0)
		{
			return true;
		}
	}

	return false;
}

float ACharacterBase::GetHealthValue() const
{
	if (HealthComponent)
	{
		return HealthComponent->GetHealth();
	}

	return -1.0f;
}


void ACharacterBase::UseWeapon()
{
	if (Weapon)
	{
		Weapon->UseWeapon();
	}
}
