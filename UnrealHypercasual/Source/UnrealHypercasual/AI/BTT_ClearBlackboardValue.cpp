// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ClearBlackboardValue.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTT_ClearBlackboardValue::UBTT_ClearBlackboardValue()
{
	NodeName = TEXT("Clear Blackboard Value");
}

EBTNodeResult::Type UBTT_ClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: OwnerComp is nullptr "), *this->GetName())
		return EBTNodeResult::Failed;
	}
	
	OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

	return EBTNodeResult::Succeeded;
}

FString UBTT_ClearBlackboardValue::GetStaticDescription() const
{
	return FString::Printf(TEXT("Clears the value of the %s"), *BlackboardKey.SelectedKeyName.ToString());
}