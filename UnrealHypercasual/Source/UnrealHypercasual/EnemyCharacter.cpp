// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "HealthComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemyCharacter::HandleDestruction()
{
	TArray<UHealthComponent*> HealthComponents;
	GetComponents<UHealthComponent>(HealthComponents);

	if (HealthComponents.Num() == 1)
	{
		HealthComponents[0]->HandleDestruction();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%i HealthComponent found on %s!"), HealthComponents.Num(), *GetActorNameOrLabel())
	}
	
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

