// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_GetActorLocation.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHYPERCASUAL_API UBTS_GetActorLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTS_GetActorLocation();
	
private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;

private:
	// Blackboard AActor who's location we're finding
	// NOTE: TODO: this still allows for vectors to be selected. Shelved for now.
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowedClasses = "AActor", BaseClassFilter = "AActor"))
	FBlackboardKeySelector ActorToFindLocationOf;
};
