// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALCOURSE1_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;

	void openDoor();
	void closeDoor();
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	FRotator open;
	FRotator close;

	float doorOpenedTime;

	UPROPERTY(EditAnywhere)
	float triggerWeight = 50.f;

	UPROPERTY(EditAnywhere)
	float closeDelay = 1.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	float GetTotalPressurePlateMass();
};
