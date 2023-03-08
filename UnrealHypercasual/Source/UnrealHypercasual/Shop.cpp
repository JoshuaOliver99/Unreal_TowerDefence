// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop.h"

#include "Item.h"
#include "UW_Shop.h"
#include "UW_ShopItem.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Game/TowerDefenceGameMode.h"
#include "Game/TowerDefencePlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/Build.h"

// Sets default values
AShop::AShop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ShopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shop Mesh Component"));
	RootComponent = ShopMesh;
	
	ShopTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Shop Trigger Component"));
	ShopTrigger->SetupAttachment(RootComponent);

	ShopCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Shop Camera Component"));
	ShopCamera->SetupAttachment(RootComponent);

	ItemSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Item Spawn Point"));
	ItemSpawnPoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AShop::BeginPlay()
{
	Super::BeginPlay();

	// Delegates
	ShopTrigger->OnComponentBeginOverlap.AddDynamic(this, &AShop::OnShopTriggerBeginOverlap);
	ShopTrigger->OnComponentEndOverlap.AddDynamic(this, &AShop::OnShopTriggerEndOverlap);
}

// Called every frame
void AShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AShop::OnShopTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor); 
	if (Character == nullptr)
	{
		return;
	}
	
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (PlayerController == nullptr)
	{
		return;
	}
	
	PlayerController->SetViewTargetWithBlend(this, 1.0f, VTBlend_Linear);
	
	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;
	PlayerController->SetInputMode(FInputModeGameAndUI());

	// ----- Add the UI
	if (ShopWidgetClass)
	{
		ShopWidget = Cast<UUW_Shop>(CreateWidget(PlayerController, ShopWidgetClass));

		if (ShopWidget)
		{
			ShopWidget->AddToViewport();

			ATowerDefenceGameMode* GameMode = Cast<ATowerDefenceGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			if (GameMode)
			{
				TArray<TSubclassOf<AItem>> ShopItemClasses;
				for (int i = 0; i < GameMode->GetShopItems().Num(); ++i)
				{
					ShopItemClasses.Add(GameMode->GetShopItems()[i]);
				}
				
				ShopWidget->SetShopItemGrid(ShopItemClasses);

				// Add button delegates
				const UUniformGridPanel* ShopItemGrid = ShopWidget->GetShopItemGrid();
				for (int i = 0; i < ShopItemGrid->GetChildrenCount(); ++i)
				{
					Cast<UUW_ShopItem>(ShopItemGrid->GetChildAt(i))->OnShopItemButtonClicked.AddDynamic(this, &AShop::OnShopItemClicked);
				}
			}
		}
	}
}


inline void AShop::OnShopTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor); 
	if (Character == nullptr)
	{
		return;
	}
	
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (PlayerController == nullptr)
	{
		return;
	}
	
	PlayerController->SetViewTargetWithBlend(OtherActor, 0.2f, VTBlend_Linear);

	PlayerController->bShowMouseCursor = false;
	PlayerController->bEnableClickEvents = false;
	PlayerController->bEnableMouseOverEvents = false;
	PlayerController->SetInputMode(FInputModeGameOnly());

	
	// ----- Remove the UI
	if (ShopWidget)
	{
		ShopWidget->RemoveFromParent();
	}
}


void AShop::OnShopItemClicked(TSubclassOf<AItem> Item)
{
	if (Item == nullptr)
	{
		return;
	}

	ATowerDefenceGameMode* GameMode = Cast<ATowerDefenceGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr)
	{
		return;
	}
	
	// Buy the Item
	if (GameMode->GetPlayerGold() - Item.GetDefaultObject()->GetValue() < 0)
	{
		return;
	}
	GameMode->AddPlayerGold(-Item.GetDefaultObject()->GetValue());

	
	// Spawn the Item
	const FVector SpawnLocation = ItemSpawnPoint->GetComponentLocation();
	const FRotator SpawnRotation = ItemSpawnPoint->GetComponentRotation();
	GetWorld()->SpawnActor(Item.Get(), &SpawnLocation, &SpawnRotation);

	// Remove the Item
}