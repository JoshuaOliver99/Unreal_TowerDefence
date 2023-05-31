// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabberComponent.h"

#include "GrabbableActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabberComponent::UGrabberComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
	// Create Default Components...
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Physics Handle Component"));
}


// Called when the game starts
void UGrabberComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

void UGrabberComponent::HandleGrabInput()
{
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	if (PhysicsHandle->GetGrabbedComponent())
	{
		Release();
		return;
	}

	Grab();
}

void UGrabberComponent::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab()"));
	
	// Trace...
	// TODO: Move Trace into its own function.
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);
	FHitResult HitResult;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	GetWorld()->SweepSingleByChannel(
			HitResult,
			Start, End,
			FQuat::Identity,
			ECC_GameTraceChannel2,
			Sphere);


	if (HitResult.bBlockingHit == false || HitResult.GetActor() == nullptr)
	{
		return;
	}


	// TODO: Implement further functionality with UGrabbableActorComponent
	UGrabbableActorComponent* GrabbableActorComponent = Cast<UGrabbableActorComponent>(HitResult.GetActor()->GetComponentByClass(UGrabbableActorComponent::StaticClass()));
	if (GrabbableActorComponent == nullptr)
	{
		return;
	}

	
	// Do the Grab...
	UPrimitiveComponent* HitComponent = HitResult.GetComponent();
	HitComponent->SetSimulatePhysics(true);
	HitComponent->WakeAllRigidBodies();
	HitResult.GetActor()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);


	// TODO: Make the grabbed object upright always

	PhysicsHandle->GrabComponentAtLocationWithRotation(
		HitComponent,
		NAME_None,
		HitResult.ImpactPoint,
		GetComponentRotation());

	//PhysicsHandle->GrabComponentAtLocation(
	//	HitComponent,
	//	NAME_None,
	//	HitResult.ImpactPoint);		
	
	//PhysicsHandle->GrabComponentAtLocationWithRotation(
	//	HitComponent,
	//	NAME_None,
	//	HitResult.ImpactPoint,
	//	FRotator::ZeroRotator);

}

void UGrabberComponent::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("RELEASE"));
	
	AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
	PhysicsHandle->ReleaseComponent();
	
}

bool UGrabberComponent::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start, End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere);
}
