// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

#include "InteractionBase.h"
#include "Interactor.h"

void UInteractable::Interact(UInteractor* Initiator, const int InteractionIndex)
{
	if (!IsValid(Initiator))
		return;

	if (AvailableInteractions.IsEmpty())
		return;

	if (InteractionIndex > AvailableInteractions.Num() - 1 || InteractionIndex < 0)
		return;

	// Try perform the Interaction...
	if (UInteractionBase* Interaction = CastChecked<UInteractionBase>(AvailableInteractions[InteractionIndex]))
	{
		CurrentInteractor = Initiator;
		Interaction->PerformInteraction(CurrentInteractor, this);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[%s] InteractionIndex(%d) was not a valid Interaction."), *this->GetName(), InteractionIndex)
}


void UInteractable::OnHoverEnter() const
{
	UE_LOG(LogTemp,	Display, TEXT("%s hovered over"),  *GetOwner()->GetActorNameOrLabel());
}

void UInteractable::OnHoverExit() const
{
	UE_LOG(LogTemp, Display, TEXT("%s no longer hovered over"), *GetOwner()->GetActorNameOrLabel());
}


TArray<UInteractionBase*> UInteractable::GetAvailableInteractions() const
{
	TArray<UInteractionBase*> Interactions;
	for (auto AvailableInteraction : AvailableInteractions)
	{
		if (UInteractionBase* Interaction = Cast<UInteractionBase>(AvailableInteraction))
			Interactions.Add(Interaction);
	}

	return Interactions;
}
