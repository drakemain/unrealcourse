// Fill out your copyright notice in the Description page of Project Settings.

#include "unrealcourse1.h"
#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	getPhysicsHandle();
	getInputComponents();
}

void UGrabber::getInputComponents()
{
	this->InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (this->InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::getPhysicsHandle()
{
	this->PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!this->PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component."), *GetOwner()->GetName())
	}
}

void UGrabber::Grab() 
{
	UE_LOG(LogTemp, Warning, TEXT("%s used grab."), *GetOwner()->GetName())

	const FHitResult hitResult = this->GetFirstPhysicsBodyInReach(); //linetrace hit result
	const AActor* actorHit = hitResult.GetActor();
	
	// only attempt to grab if an actor was hit
	if (actorHit) {
		UPrimitiveComponent* componentToGrab = hitResult.GetComponent();

		this->PhysicsHandle->GrabComponent(
			componentToGrab,
			NAME_None, // no bones
			componentToGrab->GetOwner()->GetActorLocation(),
			true
		);
	}

}

void UGrabber::Release()
{
	if (PhysicsHandle->GrabbedComponent) {
		this->PhysicsHandle->ReleaseComponent();
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FHitResult Hit;
	Pick(Hit); // out parameter
	AActor* HitActor = Hit.GetActor();

	if (HitActor) {
		UE_LOG(LogTemp, Warning, TEXT("Grabber hit: %s"), *HitActor->GetName())
	}

	return Hit;
}

void UGrabber::Pick(FHitResult &Hit)
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	FVector thisReachLength = this->GetReachEnd(PlayerViewPointLocation, PlayerViewPointRotation);

	GetWorld()->LineTraceSingleByObjectType(
		Hit, //out
		PlayerViewPointLocation,
		thisReachLength,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
	);
}

FVector UGrabber::GetReachEnd(FVector actorLocation, FRotator actorDirection)
{
	FVector ReachEndPosition;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		actorLocation, //out
		actorDirection  //out
	);
	
	return actorLocation + actorDirection.Vector() * this->reach;;
}

FVector UGrabber::GetReachStart()
{
	FVector reachStartPosition;
	FRotator actorDirection;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		reachStartPosition, //out
		actorDirection  //out
	);

	return reachStartPosition;
}

void UGrabber::moveGrabbedObject()
{
	if (PhysicsHandle->GrabbedComponent) {
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;

		FVector playerReach = this->GetReachEnd(PlayerViewPointLocation, PlayerViewPointRotation);

		PhysicsHandle->SetTargetLocation(playerReach);
	}
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	moveGrabbedObject();
}