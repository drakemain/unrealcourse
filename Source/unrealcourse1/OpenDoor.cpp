// Fill out your copyright notice in the Description page of Project Settings.

#include "unrealcourse1.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
}

float UOpenDoor::GetTotalPressurePlateMass()
{
	float totalMass = 0.f;
	TArray<AActor*> OverlappingActors;

	this->PressurePlate->GetOverlappingActors(OverlappingActors); // out parameter

	for (const AActor* Actor : OverlappingActors) {
		UE_LOG(LogTemp, Warning, TEXT("Found an actor: %s"), *Actor->GetName());
		totalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return totalMass;
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (!this->PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s is missing pressure plate!"), *GetOwner()->GetName())
	}
	else if (this->GetTotalPressurePlateMass() > this->triggerMass) {
		this->OnOpen.Broadcast();
	}
	else {
		this->OnClose.Broadcast();
	}
}
