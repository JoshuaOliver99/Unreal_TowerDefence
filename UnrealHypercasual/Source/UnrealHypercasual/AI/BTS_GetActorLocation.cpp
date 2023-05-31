// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_GetActorLocation.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTS_GetActorLocation::UBTS_GetActorLocation()
{
	NodeName = TEXT("Get Actor Location");

	// Accept only Vectors
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_GetActorLocation, BlackboardKey));
}

void UBTS_GetActorLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: OwnerComp is nullptr "), *this->GetName())
		return;
	}

	// Note: this seems to be broken.
	//if (OwnerComp.GetBlackboardComponent()->GetValueAsObject(ActorToFindLocationOf.SelectedKeyName))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("%s: ActorToFindLocationOf is not set "), *this->GetName());
	//	return;
	//}

	const AActor* SelectedActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ActorToFindLocationOf.SelectedKeyName));
	if (SelectedActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Invalid actor set to find the location of "), *this->GetName());
		return;
	}
	
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), SelectedActor->GetActorLocation());
}

FString UBTS_GetActorLocation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Get the location of %s into %s"), *ActorToFindLocationOf.SelectedKeyName.ToString(), *BlackboardKey.SelectedKeyName.ToString());
}
