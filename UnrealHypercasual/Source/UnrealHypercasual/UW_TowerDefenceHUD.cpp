// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_TowerDefenceHUD.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UUW_TowerDefenceHUD::NativeConstruct()
{
	Super::NativeConstruct();

	TitleText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TitleText")));
	PlayerHealthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PlayerHealthBar")));
	TowerHealthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("TowerHealthBar")));
	GoldText = Cast<UTextBlock>(GetWidgetFromName(TEXT("GoldText")));
}

void UUW_TowerDefenceHUD::UpdateTitleText(FString NewText)
{
	if (TitleText)
	{
		TitleText->SetText(FText::FromString(NewText));
	}
}

void UUW_TowerDefenceHUD::UpdatePlayerHealthBar(float PlayerHealthPercentage)
{
	if (PlayerHealthBar)
	{
		TowerHealthBar->SetPercent(PlayerHealthPercentage);
	}
}

void UUW_TowerDefenceHUD::UpdateTowerHealthBar(float TowerHealthPercentage)
{
	if (TowerHealthBar)
	{
		TowerHealthBar->SetPercent(TowerHealthPercentage);
	}
}

void UUW_TowerDefenceHUD::UpdateGoldText(FString Text)
{
	if (GoldText)
	{
		GoldText->SetText(FText::FromString(Text));
	}
}



