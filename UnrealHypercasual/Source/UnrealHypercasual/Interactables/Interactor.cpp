#include "Interactor.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Interactable.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

UInteractor::UInteractor()
{
	PrimaryComponentTick.bCanEverTick = true;

	PhysicsHandleComponent = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Physics Handle Component"));
	
}

void UInteractor::BeginPlay()
{
	Super::BeginPlay();
	
	// Start the timer to check for Interactable objects
	GetWorld()->GetTimerManager().SetTimer(InteractableCheckTimerHandle, this, &UInteractor::CheckForInteractable, 0.1f, true);
}

void UInteractor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	// ----- Update grabbed object location
	if (PhysicsHandleComponent && PhysicsHandleComponent->GetGrabbedComponent())
	{
		const FVector TargetLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * GrabDistance;
		PhysicsHandleComponent->SetTargetLocationAndRotation(TargetLocation, GetOwner()->GetActorRotation());
	}
}

void UInteractor::CheckForInteractable()
{
	if (bIsEnabled == false)
		return;

	// Get the PlayerController
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
	if (PlayerController == nullptr)
		return;
	
	// Get the gaze vector
	const FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
	const FVector CameraDirection = PlayerController->PlayerCameraManager->GetCameraRotation().Vector();
	const FVector EndTrace = CameraLocation + CameraDirection * InteractionDistance;

	// Raycast
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, EndTrace, ECC_Visibility) == false
		|| HitResult.GetActor() == nullptr)
	{
		if (CurrentInteractable)
		{
			CurrentInteractable->OnHoverExit();
			CurrentInteractable = nullptr;
		}
		return;
	}
	
	// Check if the hit object has an interactable component 
	UInteractable* Interactable = Cast<UInteractable>(HitResult.GetActor()->GetComponentByClass(UInteractable::StaticClass()));
	if (Interactable)
	{
		if (Interactable != CurrentInteractable)
		{
			if (CurrentInteractable)
			{
				CurrentInteractable->OnHoverExit();
			}

			CurrentInteractable = Interactable;
			CurrentInteractableHitResult = HitResult;
			CurrentInteractable->OnHoverEnter();
		}
	}
	else
	{
		if (CurrentInteractable)
		{
			CurrentInteractable->OnHoverExit();
			CurrentInteractable = nullptr;
		}
	}
}

void UInteractor::Interact()
{
	// Release anything Grabbed...
	if (PhysicsHandleComponent->GetGrabbedComponent())
	{
		PhysicsHandleComponent->ReleaseComponent();
		return;
	}
	
	if (CurrentInteractable)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Interact()"));
		//CurrentInteractable->SetCurrentInteractor(this);
		
		
		////CurrentInteractable->Interact(this);
	}
}


void UInteractor::HandleGrab()
{
	if (CurrentInteractable == nullptr || PhysicsHandleComponent == nullptr)
		return;

	
	// Grab
	if (CurrentInteractableHitResult.IsValidBlockingHit())
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentInteractableHitResult: %s"), *CurrentInteractableHitResult.ToString());
	
		if (UPrimitiveComponent* HitComponent = CurrentInteractableHitResult.GetComponent())
		{
			HitComponent->SetSimulatePhysics(true);
			HitComponent->WakeAllRigidBodies();
			CurrentInteractableHitResult.GetActor()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
			PhysicsHandleComponent->GrabComponentAtLocationWithRotation(
				HitComponent,
				NAME_None,
				CurrentInteractableHitResult.ImpactPoint,
				GetOwner()->GetActorRotation());
		}
	}	
}

                                                                                                                 
	


