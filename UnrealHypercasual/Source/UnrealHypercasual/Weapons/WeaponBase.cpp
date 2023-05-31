// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Default Components
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetupAttachment(Root);
}


void AWeaponBase::UseWeapon()
{
	if (bIsAttacking)
	{
		return;
	}
	
	bIsAttacking = true;
	DoUseAnimation();
	GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &AWeaponBase::EndCooldown, CooldownDuration, false);
}

void AWeaponBase::DoUseAnimation()
{
	if (UseWeaponAnimation == nullptr)
	{
		return;
	}
	
	if (GetOwner() == nullptr)
	{
		return;
	}

	USkeletalMeshComponent* SkeletalMeshComponent = Cast<USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	if (SkeletalMeshComponent == nullptr)
	{
		return;
	}
	
	SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkeletalMeshComponent->PlayAnimation(UseWeaponAnimation, false);
}

void AWeaponBase::EndCooldown()
{
	bIsAttacking = false;
}

