// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrabbableActorComponent.generated.h"


///
/// NOTE: Requirements:
/// The owning Actor has Collision Presets set to custom, with ECC_GameTraceChannel2 "Grabber" set to "block"  
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALHYPERCASUAL_API UGrabbableActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabbableActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// TODO Review this!
	enum EGrabbedState
	{
		Idle,
		Grabbed
	};
	EGrabbedState GrabbedState;
	
};
