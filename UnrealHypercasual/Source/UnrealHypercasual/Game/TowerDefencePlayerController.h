// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TowerDefencePlayerController.generated.h"

class ATowerDefenceGameMode;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class UNREALHYPERCASUAL_API ATowerDefencePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void SetPlayerEnabledState(bool bPlayerEnabled);
	
	virtual void GameHasEnded(AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;
	
protected:
	virtual void BeginPlay() override;


// ----- UI
public:
	UUserWidget* GetHud() const {return HUD;}
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HUDClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> LoseScreenClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WinScreenClass;
	
	UPROPERTY()
	UUserWidget* HUD;


// ----- Input
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* TowerDefencePlayerInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveInputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpInputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ShootInputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SelectInputAction;
	
	virtual void SetupInputComponent() override;

protected:
	void MoveAction(const FInputActionValue& Value);
	void LookAction(const FInputActionValue& Value);
	void JumpAction(const FInputActionValue& Value);
	void ShootAction(const FInputActionValue& Value);
	void InteractAction(const FInputActionValue& Value);
	void SelectAction(const FInputActionValue& Value);


// ----- TODO: REVIEW 
// -----
protected:
	ATowerDefenceGameMode* TowerDefenceGameMode;
};
