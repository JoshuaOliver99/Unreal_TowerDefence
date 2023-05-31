// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction_Grab.h"
#include "Interactor.h"


EInteractableStates UInteraction_Grab::PerformInteraction(UInteractor* Interactor, UInteractable* Interactable)
{
	Interactor->HandleGrab();
	
	return Super::PerformInteraction(Interactor, Interactable);
}
