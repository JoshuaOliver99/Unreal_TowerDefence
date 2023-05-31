// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_TowerDefenceHUD.generated.h"

class UTextBlock;
class UProgressBar;

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
	void UpdateSubtitleText(FString Text);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateBottomText(FString Text);
	
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdatePlayerHealthBar(float PlayerHealthPercentage);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateTowerHealthBar(float TowerHealthPercentage);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateGoldText(int GoldBalance);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateWaveText(int WaveNum);
	
private:
	UTextBlock* TitleText;

	UTextBlock* SubtitleText;
	
	UTextBlock* BottomText;
	
	UProgressBar* PlayerHealthBar;
	
	UProgressBar* TowerHealthBar;

	UTextBlock* GoldText;
	
	UTextBlock* WaveText;
};
