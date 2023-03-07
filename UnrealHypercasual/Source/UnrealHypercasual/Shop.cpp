// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Game/TowerDefencePlayerCharacter.h"

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
		ShopWidget = CreateWidget(PlayerController, ShopWidgetClass);

		if (ShopWidget)
		{
			ShopWidget->AddToViewport();
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