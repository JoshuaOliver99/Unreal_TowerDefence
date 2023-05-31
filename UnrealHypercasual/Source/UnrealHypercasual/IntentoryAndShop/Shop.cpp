// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop.h"

#include "Item.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/UniformGridPanel.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/Build.h"
#include "UnrealHypercasual/Game/TowerDefenceGameMode.h"
#include "UnrealHypercasual/UI/UW_Shop.h"
#include "UnrealHypercasual/UI/UW_ShopItem.h"

// Sets default values
AShop::AShop()
{
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


void AShop::OnShopTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Add the UI
	CreateAndUpdateShopUI(true, OtherActor);
}


inline void AShop::OnShopTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Remove the UI
	CreateAndUpdateShopUI(false, OtherActor);
}

void AShop::CreateAndUpdateShopUI(bool bIsCreatingUI, AActor* PlayerActor)
{
	ACharacter* Character = Cast<ACharacter>(PlayerActor); 
	if (Character == nullptr)
		return;
	
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (PlayerController == nullptr)
		return;

	
	PlayerController->bShowMouseCursor = bIsCreatingUI;
	PlayerController->bEnableClickEvents = bIsCreatingUI;
	PlayerController->bEnableMouseOverEvents = bIsCreatingUI;

	
	// ----- Remove the UI
	if (bIsCreatingUI == false)
	{
		PlayerController->SetViewTargetWithBlend(PlayerActor, 0.2f, VTBlend_Linear);
		PlayerController->SetInputMode(FInputModeGameOnly());

		if (ShopWidget)
			ShopWidget->RemoveFromParent();

		return;
	}
	
	
	// ----- Create the UI
	PlayerController->SetViewTargetWithBlend(this, 1.0f, VTBlend_Linear);
	PlayerController->SetInputMode(FInputModeGameAndUI());

	if (ShopWidgetClass == nullptr)
		return;
		
	ShopWidget = Cast<UUW_Shop>(CreateWidget(PlayerController, ShopWidgetClass));
	if (ShopWidget == nullptr)
		return;
		
	ShopWidget->AddToViewport();

	ATowerDefenceGameMode* GameMode = Cast<ATowerDefenceGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr)
		return;
		
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


void AShop::OnShopItemClicked(TSubclassOf<AItem> Item)
{
	if (Item == nullptr)
		return;

	ATowerDefenceGameMode* GameMode = Cast<ATowerDefenceGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr)
		return;

	
	if (GameMode->GetPlayerGold() - Item.GetDefaultObject()->GetValue() < 0)
		return;
	
	// Deduct cost
	GameMode->AddPlayerGold(-Item.GetDefaultObject()->GetValue());

	
	// Spawn the Item
	const FVector SpawnLocation = ItemSpawnPoint->GetComponentLocation();
	const FRotator SpawnRotation = ItemSpawnPoint->GetComponentRotation();
	GetWorld()->SpawnActor(Item.Get(), &SpawnLocation, &SpawnRotation);



	
	// Remove the Item
	GameMode->RemoveShopItem(Item);

	
	// Refresh the UI
	if (AActor* PlayerActor = GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		CreateAndUpdateShopUI(false, PlayerActor);
		CreateAndUpdateShopUI(true, PlayerActor);
	}
}
