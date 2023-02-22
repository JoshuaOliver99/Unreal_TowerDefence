// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindLocationNearTower.generated.h"

/**
 * Find a location nearby to the tower of this level.
 */
UCLASS()
class UNREALHYPERCASUAL_API UBTTask_FindLocationNearTower : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_FindLocationNearTower();
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = true))
	float AcceptanceRadius {500.f};
};
