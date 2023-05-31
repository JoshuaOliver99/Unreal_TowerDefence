// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerDefencePlayerCharacter.h"

#include "UnrealHypercasual/Deprecated/GrabberComponent.h"

// Sets default values
ATowerDefencePlayerCharacter::ATowerDefencePlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Default Components...
	GrabberComponent = CreateDefaultSubobject<UGrabberComponent>(TEXT("Grabber Component"));
}

// Called when the game starts or when spawned
void ATowerDefencePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATowerDefencePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATowerDefencePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

