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
	if (!this->InputComponent) {
		UE_LOG(LogTemp, Error, TEXT("%s missing input component."), *GetOwner()->GetName())
	}
	else {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
		UE_LOG(LogTemp, Warning, TEXT("%s has Grabber bindings"), *GetOwner()->GetName())
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
	UPrimitiveComponent* componentToGrab = hitResult.GetComponent();
	AActor* ActorHit = hitResult.GetActor();
	
	if (ActorHit) {
	// if linetrace hit valid actor
		this->PhysicsHandle->GrabComponent(
			componentToGrab,
			NAME_None,
			componentToGrab->GetOwner()->GetActorLocation(),
			true
		);
	}

}

void UGrabber::Release()
{
	if (PhysicsHandle->GrabbedComponent) {
		this->PhysicsHandle->ReleaseComponent();
		UE_LOG(LogTemp, Warning, TEXT("%s released grab."), *GetOwner()->GetName())
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	FHitResult Hit;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointLocation, //out
		PlayerViewPointRotation  //out
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * this->reach;

	GetWorld()->LineTraceSingleByObjectType(
		Hit, //out
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
	);

	AActor* HitActor = Hit.GetActor();
	if (HitActor) {
		UE_LOG(LogTemp, Warning, TEXT("Grabber hit: %s"), *HitActor->GetName())
	}

	return Hit;
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent) {
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;

		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			PlayerViewPointLocation, //out
			PlayerViewPointRotation  //out
		);

		FVector playerReach = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * this->reach;


		PhysicsHandle->SetTargetLocation(playerReach);
	}
}

