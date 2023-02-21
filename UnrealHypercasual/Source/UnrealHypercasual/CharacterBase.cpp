// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"

#include "GrabberComponent.h"
#include "HealthComponent.h"
#include "WeaponBase.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Default Components
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	
	PerceptionStimuliSourceComponentComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Perception Stimuli Source Component"));

	// NOTE: These are broken and must be manually set (16/02/2023)
	// Source: https://forums.unrealengine.com/t/cant-register-uaisense-sight-with-uaiperceptionstimulisourcecomponent/149852/3
	//PerceptionStimuliSourceComponentComponent->RegisterWithPerceptionSystem();
	//PerceptionStimuliSourceComponentComponent->RegisterForSense(TSubclassOf<UAISense_Sight>());
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

		// Remove gun mesh from model via bone
		//GetMesh()->HideBoneByName(TEXT("weapon_r"), PBO_None);

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

void ACharacterBase::HandleDestruction()
{
	if (HealthComponent)
	{
		HealthComponent->HandleDestruction();
	}
}

void ACharacterBase::UseWeapon()
{
	if (Weapon)
	{
		Weapon->UseWeapon();
	}
}

