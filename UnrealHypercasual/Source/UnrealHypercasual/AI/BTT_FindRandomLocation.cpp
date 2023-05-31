// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FindRandomLocation.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_FindRandomLocation::UBTT_FindRandomLocation()
{
	NodeName = TEXT("Find Random Reachable Location");

	// Accept only vectors
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_FindRandomLocation, BlackboardKey));
}

EBTNodeResult::Type UBTT_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FNavLocation Location{};

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: AIController is nullptr"), *this->GetName())
		return EBTNodeResult::Failed;
	}
	
	const APawn* AIPawn = AIController->GetPawn();
	if (AIPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: AIPawn is nullprt"), *this->GetName())
		return EBTNodeResult::Failed;
	}

	
	// Find a random location
	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (IsValid(NavSystem) && NavSystem->GetRandomReachablePointInRadius(AIPawn->GetActorLocation(), SearchRadius, Location))
	{
		AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location.Location);
	}

	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return  EBTNodeResult::Succeeded;
}

FString UBTT_FindRandomLocation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Set %s to a random reachable point"), *BlackboardKey.SelectedKeyName.ToString());
}
