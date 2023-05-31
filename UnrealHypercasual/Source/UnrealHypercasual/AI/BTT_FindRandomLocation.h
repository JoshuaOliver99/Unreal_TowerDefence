// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_FindRandomLocation.generated.h"

/**
 * BTTask for finding a random location on a NavMesh
 */
UCLASS()
class UNREALHYPERCASUAL_API UBTT_FindRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_FindRandomLocation();
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

private:
	UPROPERTY(EditAnywhere)
	float SearchRadius = 500.f;
};
