// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_GetClosestTower.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHYPERCASUAL_API UBTS_GetClosestTower : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTS_GetClosestTower();
	
private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;
};
