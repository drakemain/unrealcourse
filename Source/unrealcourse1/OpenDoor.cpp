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

	this->close = GetOwner()->GetActorRotation();
	this->open = close + FRotator(0.f, 90.f, 0.f);
}

void UOpenDoor::openDoor() 
{
	GetOwner()->SetActorRotation(this->open);

}

void UOpenDoor::closeDoor()
{
	GetOwner()->SetActorRotation(this->close);
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

	if (this->GetTotalPressurePlateMass() > this->triggerWeight) {
		openDoor();
		this->doorOpenedTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - this->doorOpenedTime >= this->closeDelay) {
		closeDoor();
	}

	// ...
}
