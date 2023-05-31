// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_IsInAggressionRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UnrealHypercasual/Characters/CharacterBase.h"

UBTD_IsInAggressionRange::UBTD_IsInAggressionRange()
{
	NodeName = TEXT("Is In Agression Range?");

	// Accept only Actors
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_IsInAggressionRange, BlackboardKey), AActor::StaticClass());
	// Accept only Vectors
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_IsInAggressionRange, BlackboardKey));
}

bool UBTD_IsInAggressionRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: OwnerComp.GetAIOwner() is nullptr "), *this->GetName())
		return false;
	}

	const ACharacterBase* Character = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: No Pawn is not of type ACharacterBase "), *this->GetName())
		return false;
	}

	
	FVector TargetLocation;
	if (const AActor* TowerActor =  Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey())))
	{
		// TODO: instead raycast to the actors model?
		// This implementation doesnt take into account target size.
		
		// Is the Blackboard key an AActor?
		TargetLocation = TowerActor->GetActorLocation();
	}
	else
	{
		// Is the Blackboard key a FVector?
		TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
	}

	
	const float Distance = FVector::Distance(Character->GetActorLocation(), TargetLocation);

	if (Distance > Character->GetAggressionRange())
	{
		// Out of range
		return false;
	}
	
	return true;
}

FString UBTD_IsInAggressionRange::GetStaticDescription() const
{
	return FString::Printf(TEXT("Is %s within ACharacterBase AgressionRange"), *BlackboardKey.SelectedKeyName.ToString());
}
