// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_TowerDefenceHUD.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHYPERCASUAL_API UUW_TowerDefenceHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateTitleText(FString Text);
	
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdatePlayerHealthBar(float PlayerHealthPercentage);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateTowerHealthBar(float TowerHealthPercentage);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateGoldText(FString Text);
	
private:
	class UTextBlock* TitleText;
	
	class UProgressBar* PlayerHealthBar;
	
	class UProgressBar* TowerHealthBar;

	class UTextBlock* GoldText;
};
