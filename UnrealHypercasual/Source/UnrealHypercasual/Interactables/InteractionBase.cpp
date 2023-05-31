// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionBase.h"
#include "Interactable.h"

// TODO - Review, is this needed? Note the Super call
UInteractionBase::UInteractionBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

EInteractableStates UInteractionBase::PerformInteraction(UInteractor* Interactor, UInteractable* Interactable)
{
	return EInteractableStates::Idle;
}


