// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_Shop.h"

#include "UW_ShopItem.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"

void UUW_Shop::NativeConstruct()
{
	Super::NativeConstruct();

	
	ShopTitleText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ShopTitleText")));
	ShopItemGrid = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("ShopItemGrid")));
}


void UUW_Shop::SetShopTitleText(FString Text)
{
	if (ShopTitleText)
	{
		ShopTitleText->SetText(FText::FromString(Text));
	}
}

void UUW_Shop::SetShopItemGrid(TArray<TSubclassOf<AItem>> ItemClasses)
{
	if (ShopItemGrid == nullptr || ShopItemWidgetClass == nullptr)
	{
		return;
	}
	
	ShopItemGrid->ClearChildren();
	
	// Add new items to the grid
	for (int i = 0; i < ItemClasses.Num(); ++i)
	{
		if (UUW_ShopItem* ShopItemWidget = CreateWidget<UUW_ShopItem>(this, ShopItemWidgetClass))
		{
			constexpr int RowSize = 4;
			const int RowNumber = i / RowSize;
			const int ColumnNumber = i % RowSize;

			ShopItemGrid->AddChildToUniformGrid(ShopItemWidget, RowNumber, ColumnNumber);
			ShopItemWidget->SetShopItem(ItemClasses[i]);

		}
	}
		
	
}
