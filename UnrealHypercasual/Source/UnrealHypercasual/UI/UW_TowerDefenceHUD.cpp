// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_TowerDefenceHUD.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UUW_TowerDefenceHUD::NativeConstruct()
{
	Super::NativeConstruct();

	TitleText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TitleText")));
	SubtitleText = Cast<UTextBlock>(GetWidgetFromName(TEXT("SubtitleText")));
	BottomText = Cast<UTextBlock>(GetWidgetFromName(TEXT("BottomText")));
	PlayerHealthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PlayerHealthBar")));
	TowerHealthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("TowerHealthBar")));
	GoldText = Cast<UTextBlock>(GetWidgetFromName(TEXT("GoldText")));
	WaveText = Cast<UTextBlock>(GetWidgetFromName(TEXT("WaveText")));
}

void UUW_TowerDefenceHUD::UpdateTitleText(FString NewText)
{
	if (TitleText)
		TitleText->SetText(FText::FromString(NewText));
}

void UUW_TowerDefenceHUD::UpdateSubtitleText(FString NewText)
{
	if (SubtitleText)
		SubtitleText->SetText(FText::FromString(NewText));
}

void UUW_TowerDefenceHUD::UpdateBottomText(FString NewText)
{
	if (BottomText)
		BottomText->SetText(FText::FromString(NewText));
}

void UUW_TowerDefenceHUD::UpdatePlayerHealthBar(float PlayerHealthPercentage)
{
	if (PlayerHealthBar)
		PlayerHealthBar->SetPercent(PlayerHealthPercentage / 100);
}

void UUW_TowerDefenceHUD::UpdateTowerHealthBar(float TowerHealthPercentage)
{
	if (TowerHealthBar)
		TowerHealthBar->SetPercent(TowerHealthPercentage / 100);
}

void UUW_TowerDefenceHUD::UpdateGoldText(int GoldBalance)
{
	if (GoldText)
		GoldText->SetText(FText::Format(FText::FromString("Gold: {0}"), FText::AsNumber(GoldBalance)));
}

void UUW_TowerDefenceHUD::UpdateWaveText(int WaveNum)
{
	if (WaveText)
		WaveText->SetText(FText::Format(FText::FromString("Wave: {0}"), FText::AsNumber(WaveNum)));
}


