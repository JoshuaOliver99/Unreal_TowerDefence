// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionBase.h"
#include "Interaction_Deploy.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHYPERCASUAL_API UInteraction_Deploy : public UInteractionBase
{
	GENERATED_BODY()

	UInteraction_Deploy();

public:
	virtual EInteractableStates PerformInteraction(UInteractor* Interactor, UInteractable* Interactable) override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorToDeploy;
};
