// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerFriendly.h"

#include "CharacterBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

AAIControllerFriendly::AAIControllerFriendly()
{
	// Create Default Components
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
	
}

void AAIControllerFriendly::OnPerceptionRegistered(const TArray<AActor*>& UpdatedActors)
{
	// TODO: Debug
	for (int i = 0; i < UpdatedActors.Num(); ++i)
	{
		// Test for sight
		const FAISenseID SightSenseID = UAISense::GetSenseID(UAISense_Sight::StaticClass());
		if (PerceptionComponent->HasActiveStimulus(*UpdatedActors[i], SightSenseID))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s Is seen"), *UpdatedActors[i]->GetActorNameOrLabel());

			if (SeenActors.Contains(UpdatedActors[i]) == false)
			{
				SeenActors.Add(UpdatedActors[i]);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s Is out of sight"), *UpdatedActors[i]->GetActorNameOrLabel());

			if (SeenActors.Contains(UpdatedActors[i]))
			{
				SeenActors.Remove(UpdatedActors[i]);
			}
		}
	}
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

void AAIControllerFriendly::UpdateClosestEnemy()
{
	if (SeenActors.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AFriendlyAIController::UpdateClosestEnemy] SeenActors is empty"));
		ClosestEnemy = nullptr;
		return;
	}
	
	ClosestEnemy = SeenActors[0];
	for (int i = 0; i < SeenActors.Num(); ++i)
	{
		if (FVector::Dist(GetPawn()->GetActorLocation(), SeenActors[i]->GetActorLocation()) <= FVector::Dist(GetPawn()->GetActorLocation(), ClosestEnemy->GetActorLocation()))
		{
			ClosestEnemy = SeenActors[i];
		}
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
