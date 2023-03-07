// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_Shop.generated.h"

/**
 * 
 */
UCLASS()
class UNREALHYPERCASUAL_API UUW_Shop : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, Category = "Shop")
	void UpdateShopTitleText(FString Text);
		
	UFUNCTION(BlueprintCallable, Category = "Shop")
	void UpdateShopItemGrid();
	
private:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> ShopItemWidgetClass;
	//class UWidget* ShopItemWidget;
	
	class UTextBlock* ShopTitleText;
	
	class UUniformGridPanel* ShopItemGrid;
};

