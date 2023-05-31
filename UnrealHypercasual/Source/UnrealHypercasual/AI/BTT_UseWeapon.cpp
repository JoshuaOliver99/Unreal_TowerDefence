// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_UseWeapon.h"

#include "AIController.h"
#include "UnrealHypercasual/Characters/CharacterBase.h"

UBTT_UseWeapon::UBTT_UseWeapon()
{
	NodeName = TEXT("Use Weapon");
}

EBTNodeResult::Type UBTT_UseWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: AIController is nullptr "), *this->GetName())
		return EBTNodeResult::Failed;
	}

	ACharacterBase* Character = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Pawn is not of type ACharacterBase "), *this->GetName())
		return EBTNodeResult::Failed;
	}



	
	Character->UseWeapon();
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UBTT_UseWeapon::GetStaticDescription() const
{
	return FString::Printf(TEXT("Call UseWeapon() on ACharacterBase"));
}
