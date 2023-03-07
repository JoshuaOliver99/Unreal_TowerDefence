// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_Shop.h"

#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"

void UUW_Shop::NativeConstruct()
{
	Super::NativeConstruct();

	
	ShopTitleText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ShopTitleText")));
	ShopItemGrid = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("ShopItemGrid")));




	UpdateShopItemGrid();
}


void UUW_Shop::UpdateShopTitleText(FString Text)
{
	if (ShopTitleText)
	{
		ShopTitleText->SetText(FText::FromString(Text));
	}
}

void UUW_Shop::UpdateShopItemGrid()
{
	if (ShopItemGrid)
	{
		if (ShopItemWidgetClass)
		{
			if (UUserWidget* ShopItemWidget = CreateWidget<UUserWidget>(this, ShopItemWidgetClass))
			{
				ShopItemGrid->AddChildToUniformGrid(ShopItemWidget, 0, 0);
			}
		}
	}
}
