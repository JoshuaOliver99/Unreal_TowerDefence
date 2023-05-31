// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_EnemyKamikaze.generated.h"

/**
 * Calls AEnemyCharacter_Kamikaze::Kamikaze() provided that the owning character is of the correct type.
 *
 * TODO
 * NOTE: potentially deprecated as the UseWeapon task could be used instead. Not yet implemented
 */
UCLASS()
class UNREALHYPERCASUAL_API UBTT_EnemyKamikaze : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_EnemyKamikaze();
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};
