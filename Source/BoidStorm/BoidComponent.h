// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "NathanGeometry.h"
#include <vector>
#include <string>
#include "Components/SceneComponent.h"
#include "BoidComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOIDSTORM_API UBoidComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	std::string flockKey;
	FVector pos;
	FVector velocity;

	FVector forward; // x
	FVector up; // y
	FVector side; // z

	FVector newPos;
	FVector newVelocity;

	FVector newForward; // x
	FVector newUp; // y
	FVector newSide; // z
	UPROPERTY(EditAnywhere)//this can be used on 
	//unreal types and primitive types to expose 
	//them in the edditor
	float max_speed;
	UPROPERTY(EditAnywhere)
	float max_accel; // Some fraction of max_speed
	std::vector<FVector(*)(UBoidComponent *,float time)> behaviors;
	
	UBoidComponent();
	FVector GetPos();
	void fly();//calculate new locations
	void applyFly();//move to new calculated locations
	FVector behave(float DeltaTime);
	std::vector<UBoidComponent*> get_neighbors(float radius);
protected:
	FVector accel;
	//position and rotation can be found using
	//ComponentToWorld.GetLocation() gives mutable position vector (in world space)
	//ComponentToWorld.Getrotation() gives mutable rotation quaternion (in world space)

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;
};



