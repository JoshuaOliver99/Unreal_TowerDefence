// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHYPERCASUAL_API AEnemy : public ABasePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void HandleDestruction() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
