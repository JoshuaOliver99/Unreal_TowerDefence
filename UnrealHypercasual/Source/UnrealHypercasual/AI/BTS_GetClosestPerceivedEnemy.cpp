// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_GetClosestPerceivedEnemy.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

UBTS_GetClosestPerceivedEnemy::UBTS_GetClosestPerceivedEnemy()
{
	NodeName = TEXT("Get Closest Perceived Enemy Actor");

	// Accept only Actors
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_GetClosestPerceivedEnemy, BlackboardKey),	AActor::StaticClass());
}

void UBTS_GetClosestPerceivedEnemy::OnTargetPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	AActor* ClosestEnemy = nullptr;
	float ClosestEnemyDistance = TNumericLimits<float>::Max();

	for (AActor* PerceivedActor : UpdatedActors)
	{
		for (auto Tag : EnemyTypeTags)
		{
			// Check if the perceived actor is an enemy
			if (PerceivedActor->Tags.Contains(*Tag))
			{
				FVector MyLocation = BTOwnerComp->GetOwner()->GetActorLocation();
				FVector EnemyLocation = PerceivedActor->GetActorLocation();

				const float DistanceToEnemy = FVector::Dist(MyLocation, EnemyLocation);

				if (DistanceToEnemy < ClosestEnemyDistance)
				{
					ClosestEnemy = PerceivedActor;
					ClosestEnemyDistance = DistanceToEnemy;
				}
			}
		}
	}
	
	
	if (ClosestEnemy == nullptr)
	{
		BTOwnerComp->GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}

	
	BTOwnerComp->GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), ClosestEnemy);
}

void UBTS_GetClosestPerceivedEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: OwnerComp is nullptr "), *this->GetName())
		return;
	}

	// Save OwnerComp for the delegate call
	BTOwnerComp = &OwnerComp;
	

	if (EnemyTypeTags.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: EnemyTypeTags has no values set! "), *this->GetName());
		return;
	}


	// NOTE: TObjectPtr stumped me for hours!
	const TObjectPtr<UAIPerceptionComponent> OwnerPerceptionComponent = OwnerComp.GetAIOwner()->GetPerceptionComponent();
	if (OwnerPerceptionComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: OwnerPerceptionComponent is nullptr "), *this->GetName());
		return;
	}

	

	// Assign a on OnPerceptionUpdated delegate
	if (OwnerPerceptionComponent->OnPerceptionUpdated.Contains(this, "OnTargetPerceptionUpdated") == false)
	{
		OwnerPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &UBTS_GetClosestPerceivedEnemy::OnTargetPerceptionUpdated);
		UE_LOG(LogTemp, Warning, TEXT("%s: OwnerPerceptionComponent OnPerceptionUpdated delegate bound "), *this->GetName())
	}
}

FString UBTS_GetClosestPerceivedEnemy::GetStaticDescription() const
{
	return FString::Printf(TEXT("Sets the Blackboard Key to the closest Actor in perception"));
}
