// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interactable.generated.h"

class UInteractor;
class UInteractionBase;


UENUM(BlueprintType)
enum EInteractableStates
{
	Idle,
	Interacting,
};


// The Interactable class is a component that allows actors to be interacted with by a player with the Interactor class.
// It provides methods for interacting with the interactable, such as grabbing and deploying it.
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALHYPERCASUAL_API UInteractable : public UActorComponent
{
	GENERATED_BODY()

	
// -------------------- Core actions
public:
	/** @brief The primary action, causing an an interaction to be invoked.
	 * @param Initiator The Interactor interacting with this Interactable
	 * @param InteractionIndex The index of the action to perform from AvailableInteractions */
	UFUNCTION(BlueprintCallable)
	void Interact(UInteractor* Initiator, int InteractionIndex);

	/** @brief Handles when the Interactable becomes scope of an Interactor */
	void OnHoverEnter() const;

	/** @brief Handles when the Interactable is no longer in scope of an Interactor */
	void OnHoverExit() const;


	
// -------------------- Current Interactor
private:
	/** @brief Reference to the Interactor using this Interactable */
	UInteractor* CurrentInteractor = nullptr;

public:
	/** @brief Return the Interactor  currently interacting with this Interactable */
	UInteractor* GetCurrentInteractor() const { return CurrentInteractor; }
	

	
// -------------------- Specific Interactions	
protected:
	/** @brief Interactions (Subclasses of UInteractionBase) available to this Interactable Actor.
	 * TODO: Implement error handing to ensure Interactions aren't duplicated */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly)
	TArray<TObjectPtr<UInteractionBase>> AvailableInteractions;

public:
	UFUNCTION(BlueprintCallable)
	int GetAvailableInteractionNum() const { return AvailableInteractions.Num(); }

	UFUNCTION(BlueprintCallable)
	TArray<UInteractionBase*> GetAvailableInteractions() const;
};
