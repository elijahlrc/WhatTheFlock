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
	Vec3 pos;
	Vec3 velocity;

	Vec3 forward; // x
	Vec3 up; // y
	Vec3 side; // z

	Vec3 newPos;
	Vec3 newVelocity;

	Vec3 newForward; // x
	Vec3 newUp; // y
	Vec3 newSide; // z

	float max_speed;
	float max_accel; // Some fraction of max_speed

	std::vector<FVector(*)(UBoidComponent *,float time)> behaviors;
	
	UBoidComponent();
	FVector GetPos();
	void fly();//calculate new locations
	void applyFly();//move to new calculated locations
	FVector behave(float DeltaTime);
	std::vector<UBoidComponent*> get_neighbors(float radius);
protected:
	Vec3 accel;
	//position and rotation can be found using
	//ComponentToWorld.GetLocation() gives mutable position vector (in world space)
	//ComponentToWorld.Getrotation() gives mutable rotation quaternion (in world space)

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};


