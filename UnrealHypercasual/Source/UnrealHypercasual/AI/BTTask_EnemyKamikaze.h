// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EnemyKamikaze.generated.h"

/**
 * Calls AEnemyCharacter_Kamikaze::Kamikaze() provided that the owning character is of the correct type.
 */
UCLASS()
class UNREALHYPERCASUAL_API UBTTask_EnemyKamikaze : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_EnemyKamikaze();
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};
