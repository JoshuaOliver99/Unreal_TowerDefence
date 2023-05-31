// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerFriendly.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "UnrealHypercasual/Characters/CharacterEnemy.h"

AAIControllerFriendly::AAIControllerFriendly()
{

}

void AAIControllerFriendly::BeginPlay()
{
	Super::BeginPlay();

	// Delegates
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AAIControllerFriendly::OnPerceptionRegistered);

	// Timers
	GetWorldTimerManager().SetTimer(UpdateClosestEnemyTimerHandle, this, &AAIControllerFriendly::UpdateClosestEnemy, UpdateClosestEnemyRate, true);
	GetWorldTimerManager().SetTimer(UseWeaponTimerHandle, this, &AAIControllerFriendly::UseWeapon, UseWeaponRate, true); // TODO: move this to tick?
}

void AAIControllerFriendly::OnPerceptionRegistered(const TArray<AActor*>& UpdatedActors)
{
	for (int i = 0; i < UpdatedActors.Num(); ++i)
	{
		// Only look for Enemy
		ACharacterEnemy* CurrentEnemy = Cast<ACharacterEnemy>(UpdatedActors[i]);
		if (CurrentEnemy == nullptr)
		{
			continue;
		}
		
		// Get which Enemy are in sight
		const FAISenseID SightSenseID = UAISense::GetSenseID(UAISense_Sight::StaticClass());
		const bool IsInSight = PerceptionComponent->HasActiveStimulus(*CurrentEnemy, SightSenseID);
		
		if (IsInSight && EnemyInSight.Contains(CurrentEnemy) == false)
		{
			EnemyInSight.Add(CurrentEnemy);
		}
		else if (IsInSight == false && EnemyInSight.Contains(CurrentEnemy))
		{
			EnemyInSight.Remove(CurrentEnemy);
		}
	}
}

void AAIControllerFriendly::UpdateClosestEnemy()
{
	if (EnemyInSight.Num() <= 0)
	{
		ClosestEnemy = nullptr;
		return;
	}

	// Compare which is closest Enemy
	ClosestEnemy = EnemyInSight[0];
	for (int i = 0; i < EnemyInSight.Num(); ++i)
	{
		if (FVector::Dist(GetPawn()->GetActorLocation(), EnemyInSight[i]->GetActorLocation()) <=
			FVector::Dist(GetPawn()->GetActorLocation(), ClosestEnemy->GetActorLocation()))
		{
			ClosestEnemy = EnemyInSight[i];
			ClosestEnemy->OnDestroyed.AddDynamic(this, &AAIControllerFriendly::OnClosestEnemyDestroyed);
		}
	}
}

void AAIControllerFriendly::OnClosestEnemyDestroyed(AActor* DestroyedActor)
{
	// ClosestEnemy OnDestroyed cleanup
	if (EnemyInSight.Contains(ClosestEnemy))
	{
		EnemyInSight.Remove(Cast<ACharacterEnemy>(ClosestEnemy));
	}
}

void AAIControllerFriendly::UseWeapon()
{
	ACharacterBase* AICharacter = Cast<ACharacterBase>(GetCharacter());
	if (AICharacter && ClosestEnemy)
	{
		//TODO: Remove this Debug/Testing...
		// Move into CharacterBase, something like AimAt(AActor*)
		const FVector ToTarget = ClosestEnemy->GetActorLocation() - GetPawn()->GetActorLocation();
		const FRotator LookAtRotation = FRotator(0.0f, ToTarget.Rotation().Yaw, 0.0f);

		GetPawn()->FaceRotation(LookAtRotation, 2.0f);
		// End...
		
		
		// UseWeapon
		AICharacter->UseWeapon();
	}
}
