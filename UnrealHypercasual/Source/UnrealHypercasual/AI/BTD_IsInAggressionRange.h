// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTD_IsInAggressionRange.generated.h"

/**
 * Intended for use on ACharacterBase type only
 */
UCLASS()
class UNREALHYPERCASUAL_API UBTD_IsInAggressionRange : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTD_IsInAggressionRange();

private:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;
};

