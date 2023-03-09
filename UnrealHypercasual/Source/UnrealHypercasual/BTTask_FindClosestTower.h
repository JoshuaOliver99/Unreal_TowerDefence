// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindClosestTower.generated.h"

/**
 * NOTE: Currently unused
 * To replace FindLocationNearTower which currently finds a location near a tower
 * FindLocationNearTower should become "FindAccessibleLocationNearby" with "float AcceptanceRadius"
 */
UCLASS()
class UNREALHYPERCASUAL_API UBTTask_FindClosestTower : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
};
