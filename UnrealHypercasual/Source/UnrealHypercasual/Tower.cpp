// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

// Sets default values
ATower::ATower()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower Mesh"));
	TowerMesh->SetupAttachment(RootComponent);
}

// Called every frame
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATower::HandleDestruction()
{
	// TODO Put death effects here...
	
	Destroy();
}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();
}



