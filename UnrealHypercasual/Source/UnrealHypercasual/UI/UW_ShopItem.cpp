// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_ShopItem.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UnrealHypercasual/IntentoryAndShop/Item.h"

void UUW_ShopItem::NativeConstruct()
{
	Super::NativeConstruct();

	
	ShopItemTitleText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ShopItemTitleText")));
	ShopItemValueText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ShopItemCostText")));
	ShopItemImage = Cast<UImage>(GetWidgetFromName(TEXT("ShopItemImage")));
	ShopItemButton = Cast<UButton>(GetWidgetFromName(TEXT("ShopItemButton")));

	
	if (ShopItemButton)
	{
		ShopItemButton->OnClicked.AddDynamic(this, &UUW_ShopItem::HandleShopItemButtonClick);
	}
}

void UUW_ShopItem::SetShopItemTitleText(FString Text)
{
	if (ShopItemTitleText)
	{
		ShopItemTitleText->SetText(FText::FromString(Text));
	}
}

void UUW_ShopItem::SetShopItemValueText(FString Text)
{
	if (ShopItemValueText)
	{
		ShopItemValueText->SetText(FText::FromString(Text));
	}
}

void UUW_ShopItem::SetShopItemImage(UTexture2D* Image)
{
	if (ShopItemImage && Image)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(Image);
		ShopItemImage->SetBrushFromTexture(Image);
	}
}

void UUW_ShopItem::SetShopItem(TSubclassOf<AItem> ItemClass)
{
	ShopItemClass = ItemClass;
	
	const AItem* ShopItem = ShopItemClass.GetDefaultObject(); 
	
	SetShopItemTitleText(ShopItem->GetTitle());
	SetShopItemValueText(FString::FromInt(ShopItem->GetValue()));
	SetShopItemImage(ShopItem->GetThumbnail());
}


void UUW_ShopItem::HandleShopItemButtonClick()
{
	OnShopItemButtonClicked.Broadcast(ShopItemClass);
}
