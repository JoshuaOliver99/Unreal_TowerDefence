// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterFriendly.h"
#include "CharacterFriendly_Turret.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHYPERCASUAL_API ACharacterFriendly_Turret : public ACharacterFriendly
{
	GENERATED_BODY()

public:
	ACharacterFriendly_Turret();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UInteractable* InteractableComponent;
};
