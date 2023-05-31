// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GrabberComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALHYPERCASUAL_API UGrabberComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabberComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



	// TODO: Review
public:
	// Decide either to Grab() or Release()
	UFUNCTION(BlueprintCallable)
	void HandleGrabInput();

	UFUNCTION(BlueprintCallable)
	void Grab();

	UFUNCTION(BlueprintCallable)
	void Release();


	
private:
	UPROPERTY(EditAnywhere)
	float MaxGrabDistance = 400;

	UPROPERTY(EditAnywhere)
	float GrabRadius = 100;		

	UPROPERTY(EditAnywhere)
	float HoldDistance = 200;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UPhysicsHandleComponent* PhysicsHandle;
	
	// Deprecated
	bool GetGrabbableInReach(FHitResult& OutHitResult) const;

	
	// TODO: Review these mods...

	// Grabbed
	class UGrabbableActorComponent GetGrabbableActorInReach();
};
