// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyKamikaze.h"

#include "AIController.h"
#include "UnrealHypercasual/Characters/CharacterEnemy_Kamikaze.h"

UBTT_EnemyKamikaze::UBTT_EnemyKamikaze()
{
	NodeName = TEXT("Initiate Enemy Kamikaze");
}

EBTNodeResult::Type UBTT_EnemyKamikaze::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	
	// Get Kamikaze Enemy
	ACharacterEnemy_Kamikaze* KamikazeEnemy = Cast<ACharacterEnemy_Kamikaze>(OwnerComp.GetAIOwner()->GetCharacter());
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

FString UBTT_EnemyKamikaze::GetStaticDescription() const
{
	return FString::Printf(TEXT("Call AEnemyCharacter_Kamikaze::Kamikaze()"));
}
