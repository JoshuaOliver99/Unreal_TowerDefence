// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_UseWeapon.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHYPERCASUAL_API UBTT_UseWeapon : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_UseWeapon();
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

};
