// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Misc/AssetRegistryInterface.h"
#include "Interactable.generated.h"

class UInteractor;

UENUM(BlueprintType)
enum EInteractionStates
{
	Idle,
	Interacting
};

UENUM(BlueprintType)
enum EInteractionActions
{
	None,
	MoveAction
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALHYPERCASUAL_API UInteractable : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractable();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	// Do the functionality of this Interactable
	void Interact();
	
	void OnHoverEnter();
	void OnHoverExit();

	UInteractor* GetCurrentInteractor() const { return CurrentInteractor; }
	void SetCurrentInteractor(UInteractor* Interactor) { CurrentInteractor = Interactor; }

private:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EInteractionStates> CurrentInteractionState = EInteractionStates::Idle;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EInteractionActions> CurrentInteractionAction = EInteractionActions::None;

	UInteractor* CurrentInteractor = nullptr;
	
	void DoMoveInteraction();
};
