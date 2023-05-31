// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_GetDistanceTo.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UnrealHypercasual/Characters/CharacterBase.h"

UBTS_GetDistanceTo::UBTS_GetDistanceTo()
{
	NodeName = TEXT("Get Location Distance");

	// Accept only floats
	BlackboardKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTS_GetDistanceTo, BlackboardKey));
}

void UBTS_GetDistanceTo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: OwnerComp is nullptr "), *this->GetName())
		return;
	}

	const ACharacterBase* Character = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: No Pawn is not of type ACharacterBase "), *this->GetName())
		return;
	}

	// Note: this seems to be broken, calling sporadically 
	//if (OwnerComp.GetBlackboardComponent()->GetValueAsObject(VectorToFindDistanceTo.SelectedKeyName))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("%s: VectorToFindDistanceTo is not set "), *this->GetName());
	//	return;
	//}
	
	const FVector SelectedLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(VectorToFindDistanceTo.SelectedKeyName);
	if (SelectedLocation == FVector::Zero())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Invalid Locatiion set to find the distance to "), *this->GetName());
		return;
	}

	const float Distance = FVector::Distance(Character->GetActorLocation(), SelectedLocation);
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(), Distance);
	
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}

FString UBTS_GetDistanceTo::GetStaticDescription() const
{
	return FString::Printf(TEXT("Get the location of this AI to %s into %s"), *VectorToFindDistanceTo.SelectedKeyName.ToString(), *BlackboardKey.SelectedKeyName.ToString());
}
