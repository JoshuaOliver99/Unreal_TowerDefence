// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EnemyKamikaze.h"

#include "AIController.h"
#include "EnemyCharacter_Kamikaze.h"
#include "NavigationSystem.h"
#include "Tower.h"
#include "TowerDefenceGameMode.h"
#include "Async/IAsyncTask.h"
#include "Kismet/GameplayStatics.h"

UBTTask_EnemyKamikaze::UBTTask_EnemyKamikaze()
{
	NodeName = TEXT("Initiate Enemy Kamikaze");
}

EBTNodeResult::Type UBTTask_EnemyKamikaze::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	
	// Get Kamikaze Enemy
	AEnemyCharacter_Kamikaze* KamikazeEnemy = Cast<AEnemyCharacter_Kamikaze>(OwnerComp.GetAIOwner()->GetCharacter());
	if (KamikazeEnemy == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	KamikazeEnemy->Kamikaze();
	
	// Signal the BehaviourTreeComponent that the task finished with a success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UBTTask_EnemyKamikaze::GetStaticDescription() const
{
	return FString::Printf(TEXT("Call AEnemyCharacter_Kamikaze::Kamikaze()"));
}
