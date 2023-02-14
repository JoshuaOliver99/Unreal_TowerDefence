// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

AEnemy::AEnemy()
{
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}
