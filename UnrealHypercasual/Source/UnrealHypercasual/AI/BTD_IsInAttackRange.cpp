// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_IsInAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UnrealHypercasual/Characters/CharacterBase.h"

UBTD_IsInAttackRange::UBTD_IsInAttackRange()
{
	NodeName = TEXT("Is In Attack Range?");
	
	// Accept only Actors
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_IsInAttackRange, BlackboardKey), AActor::StaticClass());
	// Accept only Vectors
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_IsInAttackRange, BlackboardKey));
}

bool UBTD_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (OwnerComp.GetAIOwner() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: OwnerComp.GetAIOwner() is nullptr "), *this->GetName())
		return false;
	}

	const ACharacterBase* Character = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Pawn is not of type ACharacterBase "), *this->GetName())
		return false;
	}

	
	FVector TargetLocation;
	if (const AActor* TargetActor =  Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey())))
	{
		// Find the closest point on the target actor's mesh
		FVector ClosestPoint;
		const UStaticMeshComponent* TargetMesh = TargetActor->FindComponentByClass<UStaticMeshComponent>();
		if (TargetMesh != nullptr)
		{
			const FVector StartLocation = Character->GetActorLocation();
			const FVector EndLocation = TargetActor->GetActorLocation();
			FVector Direction = (EndLocation - StartLocation).GetSafeNormal();
			FVector ClosestPointOnMesh;

			const float HitResult = TargetMesh->GetClosestPointOnCollision(StartLocation, ClosestPointOnMesh);
			if (HitResult >= 0.0f)
			{
				ClosestPoint = ClosestPointOnMesh;
			}
			else
			{
				// If no hit, fallback to using the target actor's location
				ClosestPoint = EndLocation;
			}

			UE_LOG(LogTemp, Warning, TEXT("HitResult: %f"), HitResult);
		}

		TargetLocation = ClosestPoint;
	}
	else
	{
		// Is the Blackboard key a FVector?
		TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
	}

	
	
	const float Distance = FVector::Distance(Character->GetActorLocation(), TargetLocation);

	UE_LOG(LogTemp, Warning, TEXT("Distance: %f & Character.GetAttackRange: %f"), Distance, Character->GetAttackRange());

	if (Distance > Character->GetAttackRange())
	{
		// Out of range
		return false;
	}
	
	return true;
}

FString UBTD_IsInAttackRange::GetStaticDescription() const
{
	return FString::Printf(TEXT("Is %s within ACharacterBase AttackRange"), *BlackboardKey.SelectedKeyName.ToString());
}
