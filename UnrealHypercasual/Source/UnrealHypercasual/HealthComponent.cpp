// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "TowerDefenceGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}



// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	
	// Bind delegates
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);

	TowerDefenceGameMode = Cast<ATowerDefenceGameMode>(UGameplayStatics::GetGameMode(this));
}

void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser)
{
	if (Damage <= 0.f)
		return;

	Health -= Damage;

	if (Health <= 0.f && TowerDefenceGameMode)
	{
		TowerDefenceGameMode->ActorDied(DamagedActor);
		
	}
}