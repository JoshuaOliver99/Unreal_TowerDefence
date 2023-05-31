// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "UnrealHypercasual/Interactables/Interactable.h"

// Sets default values
AItem::AItem()
{
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	RootComponent = ItemMesh;

	InteractableComponent = CreateDefaultSubobject<UInteractable>(TEXT("Interactable"));
}

