// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

#include "UnrealHypercasual/Health/HealthComponent.h"

// Sets default values
ATower::ATower()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create Default Components
	TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower Mesh"));
	TowerMesh->SetupAttachment(RootComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Tower Health Component"));
}

// Called every frame
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATower::HandleDestruction()
{
	if (HealthComponent)
	{
		HealthComponent->HandleDestruction();
	}
}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();
}



