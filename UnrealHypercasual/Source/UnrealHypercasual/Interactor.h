// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interactor.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UNREALHYPERCASUAL_API UInteractor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractor();

	// The distance from the player's camera to detect Interactable objects
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractionDistance = 200.0f;

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
	UPROPERTY(EditDefaultsOnly)
	class UPhysicsHandleComponent* PhysicsHandleComponent;
	
	// The timer handle for checking for Interactable objects                      
	FTimerHandle InteractableCheckTimerHandle;                                     
                                                                                   
	// The currently targeted Interactable object                                  
	class UInteractable* CurrentInteractable = nullptr;
	FHitResult CurrentInteractableHitResult;
                                                                                   
	// Whether the Interactor is currently enabled                                 
	bool bIsEnabled = true;                                                        
                                                                                   
	// Whether the player is currently interacting with an object                  
	bool bIsInteracting = false;
	
public:
	class UInteractable* GetCurrentInteractable() const { return CurrentInteractable; }
	
	bool GetIsEnabled() const { return bIsEnabled; }      
	void SetIsEnabled(bool bEnabled) { bIsEnabled = bEnabled; }


	// ----- Grabbing
public:
	void CurrentInteractableGrabHandle();
private:
	float HoldDistance = 100.0f;
};
