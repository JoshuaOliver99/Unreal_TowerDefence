// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction_Deploy.h"
#include "Interactable.h"

UInteraction_Deploy::UInteraction_Deploy()
{
}

EInteractableStates UInteraction_Deploy::PerformInteraction(UInteractor* Interactor, UInteractable* Interactable)
{
	if (ActorToDeploy == nullptr)
	{
		return EInteractableStates::Idle;

		// TODO : Implement failed condition
		//return EInteractableStates::Failed;
	}


	// ----- Spawn Actor
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	AActor* DeployedActor = GetWorld()->SpawnActor<AActor>(
	ActorToDeploy,
	Interactable->GetOwner()->GetActorLocation(),
	Interactable->GetOwner()->GetActorRotation(),
	SpawnParameters);
	
	
	// ----- Move actor up to prevent it colliding with the floor
	// (likely will cause issues if deployed somewhere with a low roof, but shall do for now)
	FVector DeployedActorLocation = DeployedActor->GetActorLocation();
	FVector ActorBoxVolume;
	
	DeployedActor->GetActorBounds(true, DeployedActorLocation, ActorBoxVolume, true);

	const FVector NewLocation = DeployedActorLocation + FVector(0.0f, 0.0f, ActorBoxVolume.Z / 2);
	DeployedActor->SetActorLocation(NewLocation);

	
	// ----- Destroy Interactable
	Interactable->GetOwner()->Destroy();

	
	return Super::PerformInteraction(Interactor, Interactable);
}
