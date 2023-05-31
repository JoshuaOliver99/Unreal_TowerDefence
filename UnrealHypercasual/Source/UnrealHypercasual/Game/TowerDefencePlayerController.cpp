// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerDefencePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TowerDefenceGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"


void ATowerDefencePlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
	if (bPlayerEnabled)
	{
		GetPawn()->EnableInput(this);
	}
	else
	{
		GetPawn()->DisableInput(this);
	}
	bShowMouseCursor = bPlayerEnabled;
}

void ATowerDefencePlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	HUD->RemoveFromParent();
	
	SetPlayerEnabledState(false);

	if (bIsWinner)
	{		
		UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
		if (WinScreen)
		{
			WinScreen->AddToViewport();
		}
	}
	else
	{
		UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
		if (LoseScreen)
		{
			LoseScreen->AddToViewport();
		}
	}
}

void ATowerDefencePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ----- Initialize HUD
	HUD = CreateWidget(this, HUDClass);
	if (HUD)
	{
		HUD->AddToViewport();
	}

	// ----- Get References
	TowerDefenceGameMode = Cast<ATowerDefenceGameMode>(UGameplayStatics::GetGameMode(this));
}

void ATowerDefencePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ATowerDefencePlayerController::MoveAction);
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ATowerDefencePlayerController::LookAction);
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &ATowerDefencePlayerController::JumpAction);
		EnhancedInputComponent->BindAction(ShootInputAction, ETriggerEvent::Triggered, this, &ATowerDefencePlayerController::ShootAction);
		EnhancedInputComponent->BindAction(InteractInputAction, ETriggerEvent::Triggered, this, &ATowerDefencePlayerController::InteractAction);
		EnhancedInputComponent->BindAction(SelectInputAction, ETriggerEvent::Triggered, this, &ATowerDefencePlayerController::SelectAction);
	}
}

void ATowerDefencePlayerController::MoveAction(const FInputActionValue& Value)
{
}

void ATowerDefencePlayerController::LookAction(const FInputActionValue& Value)
{
}

void ATowerDefencePlayerController::JumpAction(const FInputActionValue& Value)
{
}

void ATowerDefencePlayerController::ShootAction(const FInputActionValue& Value)
{
}

void ATowerDefencePlayerController::InteractAction(const FInputActionValue& Value)
{
}

void ATowerDefencePlayerController::SelectAction(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("SelectAction"));

	if (TowerDefenceGameMode == nullptr)
	{
		return;
	}

	TowerDefenceGameMode->HandlePlayerSelectAction();
	
}
