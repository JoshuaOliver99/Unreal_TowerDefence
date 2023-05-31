// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tower.generated.h"

class UHealthComponent;

UCLASS()
class UNREALHYPERCASUAL_API ATower : public AActor
{
	GENERATED_BODY()

protected:	
	// Sets default values for this actor's properties
	ATower();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TowerMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	
// ----- Health
public:
	UHealthComponent* GetHealthComponent() const { return  HealthComponent; }
	
	void HandleDestruction();
};
