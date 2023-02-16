// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter_Kamikaze.h"

#include "Kismet/GameplayStatics.h"

void AEnemyCharacter_Kamikaze::Kamikaze()
{
	UClass* DamageTypeClass = UDamageType::StaticClass();
	const TArray<AActor*> IgnoreActors;
	UGameplayStatics::ApplyRadialDamageWithFalloff(
		GetWorld(),
		BaseDamage,
		MinimumDamage,
		GetActorLocation(),
		DamageInnerRadius,
		DamageOuterRadius,
		DamageFalloff,
		DamageTypeClass,
		IgnoreActors);

	
	HandleDestruction();
}
