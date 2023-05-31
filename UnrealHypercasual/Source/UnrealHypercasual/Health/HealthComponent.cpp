// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "Kismet/GameplayStatics.h"
#include "UnrealHypercasual/Game/TowerDefenceGameMode.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
		
	Health = MaxHealth;
	
	TowerDefenceGameMode = Cast<ATowerDefenceGameMode>(UGameplayStatics::GetGameMode(this));
	
	// Bind delegates
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);
}

void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser)
{
	if (Damage <= 0.f)
		return;

	Health -= Damage;
	
	if (OnDamageTaken.IsBound())
		OnDamageTaken.Broadcast(DamagedActor, Damage, DamageType, Instigator, DamageCauser);
	
	if (Health <= 0.f && TowerDefenceGameMode)
		TowerDefenceGameMode->ActorDied(DamagedActor);
}


void UHealthComponent::HandleDestruction()
{
	if (DeathParticles)
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());

	if (DeathSound)
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	
	if (DeathCameraShakeClass)
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);

	
	if (GetOwner())
		GetOwner()->Destroy();
}
