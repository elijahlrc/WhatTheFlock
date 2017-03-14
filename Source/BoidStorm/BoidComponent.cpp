// Fill out your copyright notice in the Description page of Project Settings.

#include "BoidStorm.h"
#include "BoidComponent.h"
#include "FlockController.h"


FVector circle(UBoidComponent *b, float DeltaTime) {
	/*if (DeltaTime < 50)
		return Vec3(0.05, 0.0, 0.0);
	else if (50 <= DeltaTime && DeltaTime < 100)
		return Vec3(0.0, 0.0, -0.05);
	else if (100 <= DeltaTime && DeltaTime < 150)
		return Vec3(-0.05, 0.0, 0.0);
	else if (150 <= DeltaTime && DeltaTime < 200)
		return Vec3(0.0, 0.0, 0.05);
	else {
		DeltaTime = 0;
		return Vec3();*/
	return FVector(.1,0,0);
}

// Sets default values for this component's properties
UBoidComponent::UBoidComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	
	flockKey = "flock1";
	pos = Vec3();
	velocity = Vec3();
	forward = Vec3(1.0, 0.0, 0.0); 
	up = Vec3(0.0, 1.0, 0.0);
	side = Vec3(0.0, 0.0, 1.0);
	max_speed = 1.0;
	max_accel = 0.1;
	behaviors = std::vector<FVector(*)(UBoidComponent *, float)>();
	behaviors.push_back(&circle);

}

FVector UBoidComponent::GetPos()
{
	return ComponentToWorld.GetLocation();
}

FVector UBoidComponent::behave(float DeltaTime)
{
	FVector acc = FVector();
	for (FVector(*behavior)(UBoidComponent *, float time) : behaviors) {
		acc = acc + behavior(this, DeltaTime);	
	}
	return acc;
}

//calculate and set position, velocity and orientation
void UBoidComponent::fly()
{
	Vec3 acc = accel.truncate(max_accel * max_speed);
	newVelocity = (velocity + acc).truncate(max_speed);
	newPos = pos + velocity;
	// Reorient
	newForward = velocity.normalized();
	Vec3 aprx_up = up; // Should be a weighted sum of acc, acc due to gravity, and the old up
	newSide = forward.cross(aprx_up);
	newUp = forward.cross(side);
}

void UBoidComponent::applyFly()
{

	pos = newPos;
	velocity = newVelocity;
	forward = newForward;
	up = newUp;
	side = newSide;
	FVector newpos = FVector(pos.x, pos.y, pos.z);
	GetAttachParent()->SetWorldLocation(newpos);
}

// Called when the game starts
void UBoidComponent::BeginPlay()
{
	Super::BeginPlay();

	AFlockController::registerBoid(this, flockKey, GetWorld());
}

// Called every frame
void UBoidComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector a = behave(DeltaTime);
	accel = Vec3(a.X, a.Y, a.Z);
	fly();
}

std::vector<UBoidComponent*> UBoidComponent::get_neighbors(float radius) {
	return AFlockController::get_neighbors(pos, radius, flockKey);
}