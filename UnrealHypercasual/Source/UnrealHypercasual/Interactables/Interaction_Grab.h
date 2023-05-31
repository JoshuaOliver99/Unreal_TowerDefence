// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionBase.h"
#include "Interaction_Grab.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHYPERCASUAL_API UInteraction_Grab : public UInteractionBase
{
	GENERATED_BODY()

public:
	virtual EInteractableStates PerformInteraction(UInteractor* Interactor, UInteractable* Interactable) override;

protected:
	// The state of this grab interaction 
	bool IsGrabbed = false;
};
