// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Delegates/Delegate.h"
#include "UW_ShopItem.generated.h"

class AItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShopItemButtonClickedEvent, TSubclassOf<AItem>, Item);

/**
 * 
 */
UCLASS()
class UNREALHYPERCASUAL_API UUW_ShopItem : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	void SetShopItem(TSubclassOf<AItem> ItemClass);

	class UTextBlock* GetShopItemTitle() const { return ShopItemTitleText; }
	void SetShopItemTitleText(FString Text);
	
	class UTextBlock* GetShopItemValueText() const { return ShopItemValueText; }
	void SetShopItemValueText(FString Text);

	class UImage* GetShopItemImage() const { return ShopItemImage; }
	void SetShopItemImage(UTexture2D* Image);

	class UButton* GetShopItemButton() const { return  ShopItemButton; }

public:
	/** Called when the button is clicked */
	UPROPERTY(BlueprintAssignable, Category="Button|Event")
	FOnShopItemButtonClickedEvent OnShopItemButtonClicked;
	
private:
	TSubclassOf<AItem> ShopItemClass;
	
	UTextBlock* ShopItemTitleText;
	
	UTextBlock* ShopItemValueText;

	UImage* ShopItemImage;

	UButton* ShopItemButton;

private:
	UFUNCTION()
	void HandleShopItemButtonClick();
};

