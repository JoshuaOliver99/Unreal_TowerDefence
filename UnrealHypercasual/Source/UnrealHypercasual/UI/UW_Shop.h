// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIHelpers.h"
#include "Blueprint/UserWidget.h"
#include "UW_Shop.generated.h"

class UTextBlock;
class UUniformGridPanel;
class AItem;

/**
 * 
 */
UCLASS()
class UNREALHYPERCASUAL_API UUW_Shop : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UTextBlock* GetShopTitleText() const { return ShopTitleText; }
	void SetShopTitleText(FString Text);
		
	UUniformGridPanel* GetShopItemGrid() const { return ShopItemGrid; }
	void SetShopItemGrid(TArray<TSubclassOf<AItem>> ItemClasses);
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ShopItemWidgetClass;
	
	UTextBlock* ShopTitleText;
	
	UUniformGridPanel* ShopItemGrid;
};

