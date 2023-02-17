// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindLocationNearTower.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "Tower.h"
#include "TowerDefenceGameMode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTTask_FindLocationNearTower::UBTTask_FindLocationNearTower()
{
	NodeName = TEXT("Find Location near GameMode tower");

	// Accept only vectors
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindLocationNearTower, BlackboardKey));
}

EBTNodeResult::Type UBTTask_FindLocationNearTower::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	// Get AI Pawn
	AAIController* AIController{OwnerComp.GetAIOwner()};

	// Get the Tower
	ATowerDefenceGameMode* GameMode = Cast<ATowerDefenceGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATowerDefenceGameMode 'GameMode' was not found!"));
		return EBTNodeResult::Failed;
	}

	// Get Tower location
	const FVector TowerLocation = GameMode->GetTower()->GetTargetLocation();
	
	// Obtain Navigation System and find a random location
	FNavLocation Location{};
	const UNavigationSystemV1* NavSystem{UNavigationSystemV1::GetCurrent(GetWorld())};
	if (IsValid(NavSystem) && NavSystem->GetRandomReachablePointInRadius(TowerLocation, AcceptanceRadius, Location))
	{
		AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location.Location);
	}

	// Signal the BehaviourTreeComponent that the task finished with a success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UBTTask_FindLocationNearTower::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}
