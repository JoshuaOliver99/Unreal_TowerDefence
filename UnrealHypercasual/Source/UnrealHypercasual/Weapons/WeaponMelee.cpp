// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponMelee.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AWeaponMelee::AWeaponMelee()
{
	// Create Default Components
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Melee Collision Component"));
	CollisionComponent->SetupAttachment(WeaponMesh);
}

void AWeaponMelee::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind Delegate
	if (CollisionComponent)
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeaponMelee::OnComponentBeginOverlap);

}

void AWeaponMelee::UseWeapon()
{
	Super::UseWeapon();
}

void AWeaponMelee::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check we have an owner
	if (!GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] OnComponentBeginOverlap() failed GetOwner."), *this->GetName())
		return;
	}
	
	// Check that we're attacking
	if (!bIsAttacking)
	{
		return;
	}
	
	// Check the attack is not on self
	if (!GetOwner() || GetOwner()  == OtherActor)
	{
		return;
	}
	
	// Check if the overlapped component is valid
	if (!OverlappedComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] OnComponentBeginOverlap() no OverlappedComponent."), *this->GetName())
		return;
	}

	// Check if the other actor is valid
	if (!OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] OnComponentBeginOverlap() Invalid OtherActor."), *this->GetName())
		return;
	}

	// Check if the other component is valid
	if (!OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] OnComponentBeginOverlap() Invalid OtherComp."), *this->GetName())
		return;
	}
	
	UGameplayStatics::ApplyDamage(OtherActor, MeleeDamage, GetInstigatorController(), this, UDamageType::StaticClass());
}