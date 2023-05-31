// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

// TODO: Review - Added as tests to see if these make this class selectable
#include "UObject/ObjectMacros.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/Object.h"
#include "Templates/SubclassOf.h"

#include "InteractionBase.generated.h"

class UInteractor;
class UInteractable;

/**
 * 
 */
UCLASS(Abstract, EditInlineNew, BlueprintType)
class UNREALHYPERCASUAL_API UInteractionBase : public UObject
{
	GENERATED_BODY()

	
// ---------- Initialize
public:
	// Constructor mimicking UAISenseConfig.h and UAISense.cpp to allow for exposed member variables within an array
	UInteractionBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	
public:
	// Do the interaction
	virtual enum EInteractableStates PerformInteraction( UInteractor* Interactor, UInteractable* Interactable);
	
	
// ---------- Properties
protected:
	// The Title to display in UI for this Interaction
	UPROPERTY(EditAnywhere)
	FString Title = "Default Interaction";
	
	// The Thumbnail to display in UI for this Interaction
	UPROPERTY(EditAnywhere)
	UTexture2D* Thumbnail;

public:
	UFUNCTION(BlueprintCallable)
	FString GetTitle() const { return Title; }

	UFUNCTION(BlueprintCallable)
	UTexture2D* GetThumbnail() const { return Thumbnail; }


};


