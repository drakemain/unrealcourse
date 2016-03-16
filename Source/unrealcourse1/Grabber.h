// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALCOURSE1_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	float reach = 150.f;
		
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	void getInputComponents();
	void getPhysicsHandle();

	void Grab();
	void Release();
	
	const FHitResult GetFirstPhysicsBodyInReach();
	void Pick(FHitResult &Hit); // takes out parameter
	FVector GetReachEnd(FVector, FRotator); // takes out parameters
	FVector GetReachStart();
	void moveGrabbedObject();
};
