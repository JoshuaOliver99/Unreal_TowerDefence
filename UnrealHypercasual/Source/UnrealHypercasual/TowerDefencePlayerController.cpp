// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerDefencePlayerController.h"

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