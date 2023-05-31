// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// TODO review - added for enum
#include "Interactable.h"

#include "Components/ActorComponent.h"
#include "Interactor.generated.h"

// Interactor handles interacting with Interactable actors
// NOTE: The Interactor is to be added to the player character
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UNREALHYPERCASUAL_API UInteractor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Raycast from the player's camera to detect Interactable objects
	void CheckForInteractable();

	// Handle interactions with the currently targeted Interactable object
	UFUNCTION(BlueprintCallable)
	void Interact();

protected:	
	// The distance from the player's camera to detect Interactable objects
	UPROPERTY(EditAnywhere)
	float InteractionDistance = 200.0f;

	UPROPERTY(EditDefaultsOnly)
	class UPhysicsHandleComponent* PhysicsHandleComponent;
	
	// The timer handle for checking for Interactable objects                      
	FTimerHandle InteractableCheckTimerHandle;                                     
                                                                                   
	// The currently targeted Interactable object                                  
	UInteractable* CurrentInteractable = nullptr;

	FHitResult CurrentInteractableHitResult;
	
public:
	UFUNCTION(BlueprintCallable)
	UInteractable* GetCurrentInteractable() const { return CurrentInteractable; }


// ---------- Interactor Data
protected:
	// Whether the Interactor is currently enabled                                 
	bool bIsEnabled = true;                                                        
                                                                                   
	// Whether the player is currently interacting with an object                  
	bool bIsInteracting = false;

public:
	bool GetIsEnabled() const { return bIsEnabled; }      
	void SetIsEnabled(bool bEnabled) { bIsEnabled = bEnabled; }
	



	
// ----- Grabbing
public:
	void HandleGrab();

protected:
	UPROPERTY(EditAnywhere)
	float GrabDistance = 100.0f;




// ----- TODO reivew - Interaction states
protected:
	// Enum used for checking the current interaction state
	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<EInteractableStates> CurrentInteractionState = EInteractableStates::Idle;

public:
	// Return the current interaction state of this Interactor
	EInteractableStates GetCurrentInteractionState() const { return CurrentInteractionState; }

	// Set the current interaction state of this interactor
	void SetCurrentInteractionState(const EInteractableStates NewInteractionState) { CurrentInteractionState = NewInteractionState; }
	
};
