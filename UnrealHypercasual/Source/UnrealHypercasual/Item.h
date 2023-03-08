// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EUseType : uint8
{
	None,
	DeployItem
};

UCLASS()
class UNREALHYPERCASUAL_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	// TODO Review
public:
	FString GetTitle() const { return Title; }
	//void SetTitle(const FString& NewTitle) { Title = NewTitle; }
    
	int32 GetValue() const { return Value; }
	//void SetValue(int32 NewValue) { Value = NewValue; }
    
	UTexture2D* GetThumbnail() const { return Thumbnail; }
	//void SetThumbnail(UTexture2D* NewThumbnail) { Thumbnail = NewThumbnail; }

	EUseType GetUseType() const { return SelectedUseType; }
	
	void UseItem();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	UStaticMeshComponent* ItemMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Value;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EUseType SelectedUseType = EUseType::None;

private:
	void DeployItem();

};
