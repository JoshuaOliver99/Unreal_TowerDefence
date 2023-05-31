// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_GetClosestPerceivedEnemy.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHYPERCASUAL_API UBTS_GetClosestPerceivedEnemy : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTS_GetClosestPerceivedEnemy();
	
private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;

private:
    // Tags to compare perceived targets with
	UPROPERTY(EditAnywhere)
	TArray<FString> EnemyTypeTags;

private:
	// Pointer saved for use in delegate
	UBehaviorTreeComponent* BTOwnerComp;
	
public:
	// Delegate called a perception change
	UFUNCTION()
	void OnTargetPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
};