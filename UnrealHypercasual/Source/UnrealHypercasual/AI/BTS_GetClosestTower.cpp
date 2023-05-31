// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_GetClosestTower.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealHypercasual/Game/Tower.h"
#include "UnrealHypercasual/Game/TowerDefenceGameMode.h"

UBTS_GetClosestTower::UBTS_GetClosestTower()
{
	NodeName = TEXT("Get Closest Accessible Tower");

	// Accept only Actors
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_GetClosestTower, BlackboardKey),	AActor::StaticClass());
}

void UBTS_GetClosestTower::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: OwnerComp is nullptr "), *this->GetName())
		return;
	}
	
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: AIController is nullptr"), *this->GetName())
		return;
	}

	const ATowerDefenceGameMode* TowerDefenceGameMode = Cast<ATowerDefenceGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (TowerDefenceGameMode == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: GameMode is not of type ATowerDefenceGameMode"), *this->GetName());
		return;
	}

	
	// Find set the blackboard key to the closest NavMesh accessible tower...
	TArray<ATower*> Towers = TowerDefenceGameMode->GetTowers();
	
	float ClosestDistance = MAX_FLT;
	AActor* ClosestTower = nullptr;

	for (ATower* Tower : Towers)
	{
		// TODO REVIEW is this functional?
		// Return if the Tower is inaccessible
		if (FNavigationSystem::IsValidLocation(Tower->GetActorLocation()) == false)
		{
			continue;
		}

		const float Distance = FVector::Dist(AIController->GetPawn()->GetActorLocation(), Tower->GetActorLocation());
		
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestTower = Cast<AActor>(Tower);
		}
	}

	if (ClosestTower == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: found no accessible Tower"), *this->GetName());
		
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
		return;
	}
	
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), ClosestTower);
}

FString UBTS_GetClosestTower::GetStaticDescription() const
{
	return FString::Printf(TEXT(""));
}
