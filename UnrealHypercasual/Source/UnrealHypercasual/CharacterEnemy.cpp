// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterEnemy.h"

// Sets default values
ACharacterEnemy::ACharacterEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Initial Error Check
	if (EnemySpawnCost < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s EnemySpawnCost is below 0!"), *GetActorNameOrLabel());
	}
	
}

// Called every frame
void ACharacterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

