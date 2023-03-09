// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

#include "Interactor.h"


// Sets default values for this component's properties
UInteractable::UInteractable()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UInteractable::BeginPlay()
{
	Super::BeginPlay();
}

#pragma region Basic Interactions
void UInteractable::Interact()
{

	switch (CurrentInteractionAction)
	{
	case EInteractionActions::None:
		{
			UE_LOG(LogTemp, Warning, TEXT("BASE ACTION"));
			break;
		}
	case EInteractionActions::MoveAction:
		{
			DoMoveInteraction();
			break;
		}
	default:
		{
			UE_LOG(LogTemp, Warning, TEXT("NO ACTION RECOGNISED"));
			break;
		}
	}
}

void UInteractable::OnHoverEnter()
{
	//UE_LOG(LogTemp, Warning, TEXT("I %s AM BEING hovered over WITH"), *GetOwner()->GetActorNameOrLabel());
}

void UInteractable::OnHoverExit()
{
	//UE_LOG(LogTemp, Warning, TEXT("I %s AM no longer BEING hovered over WITH"), *GetOwner()->GetActorNameOrLabel());
}
#pragma endregion  Basic Interactions


#pragma region Unique Interactions
void UInteractable::DoMoveInteraction()
{
	if (CurrentInteractor)
	{
		CurrentInteractor->CurrentInteractableGrabHandle();
	}
}
#pragma endregion Unique Interactions
