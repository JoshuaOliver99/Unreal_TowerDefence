// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "CharacterEnemy.generated.h"

UCLASS()
class UNREALHYPERCASUAL_API ACharacterEnemy : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// TODO: Review - Enemy costs
private:
	UPROPERTY(EditDefaultsOnly)
	int32 EnemySpawnCost = 20;

public:
	UFUNCTION(BlueprintCallable)
	int32 GetEnemySpawnCost() {return EnemySpawnCost;}
};
