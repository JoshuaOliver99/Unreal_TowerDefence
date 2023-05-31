// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_GetDistanceTo.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHYPERCASUAL_API UBTS_GetDistanceTo : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTS_GetDistanceTo();
	
private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;

private:
	// Blackboard Location were finding the distance to
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowedClasses = "Vector", BaseClassFilter = "Vector"))
	FBlackboardKeySelector VectorToFindDistanceTo;
};