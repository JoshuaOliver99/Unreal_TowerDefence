// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterFriendly_Turret.h"

#include "GrabbableActorComponent.h"
#include "WeaponRanged.h"

ACharacterFriendly_Turret::ACharacterFriendly_Turret()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Create Default Components
	GrabbableActorComponent = CreateDefaultSubobject<UGrabbableActorComponent>(TEXT("Grabbable Actor Component"));
}
