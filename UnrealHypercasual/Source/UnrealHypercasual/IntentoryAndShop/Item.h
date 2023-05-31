// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UInteractable;

UCLASS()
class UNREALHYPERCASUAL_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();
	
protected:
	// The physical representation of the Item
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	UStaticMeshComponent* ItemMesh;

	// Interactable component to allows for Item interactions
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	UInteractable* InteractableComponent;
	
protected:
	// The title given to this Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Title;

	// The value of the Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Value;
	
	// The display icon of the Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* Thumbnail;

public:
	FString GetTitle() const { return Title; }
	void SetTitle(const FString& NewTitle) { Title = NewTitle; }
    
	int32 GetValue() const { return Value; }
	void SetValue(const int32 NewValue) { Value = NewValue; }
    
	UTexture2D* GetThumbnail() const { return Thumbnail; }
	void SetThumbnail(UTexture2D* NewThumbnail) { Thumbnail = NewThumbnail; }
};
