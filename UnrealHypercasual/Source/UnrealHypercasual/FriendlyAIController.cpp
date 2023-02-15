// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendlyAIController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

AFriendlyAIController::AFriendlyAIController()
{
	// Create Default Components
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
	
}

void AFriendlyAIController::OnPerceptionRegistered(const TArray<AActor*>& UpdatedActors)
{
	// TODO: Debug
	for (int i = 0; i < UpdatedActors.Num(); ++i)
	{
		// Test for sight
		const FAISenseID SightSenseID = UAISense::GetSenseID(UAISense_Sight::StaticClass());
		if (PerceptionComponent->HasActiveStimulus(*UpdatedActors[i], SightSenseID))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s Is seen"), *UpdatedActors[i]->GetActorNameOrLabel());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s Is out of sight"), *UpdatedActors[i]->GetActorNameOrLabel());
		}
	}
}

void AFriendlyAIController::BeginPlay()
{
	Super::BeginPlay();

	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AFriendlyAIController::OnPerceptionRegistered);
}
