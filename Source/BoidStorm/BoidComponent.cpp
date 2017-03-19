// Fill out your copyright notice in the Description page of Project Settings.
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

#include "BoidStorm.h"
#include "BoidComponent.h"
#include "FlockController.h"
#include <cmath>

//some constant coificents:
float attractRange = 4000;
float repelltRange = 4000;
float matchVelRange = 2700;
float eAttractRange = 1700;

float centerAttractionForce = -5;
float circleCenterForce = 1;


float attractForce = 20;
float repellForce = -37000;
float matchVelForce = 200;
float eAttractForce = 1;

FVector attractToCenter(UBoidComponent *b, float DeltaTime) {
	return DeltaTime * centerAttractionForce *  b->pos;
}
FVector circleCenter(UBoidComponent *b, float DeltaTime) {
	return DeltaTime * circleCenterForce *  FVector::CrossProduct(b->pos, FVector::UpVector);
}


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
	FVector circleReturn = FVector(0, 0, 0)*DeltaTime;
	return circleReturn;
}
FVector attract(UBoidComponent *b, float DeltaTime) {
	FVector force = FVector(0,0,0);
	std::vector<UBoidComponent*> neighbors = b->get_neighbors(attractRange);
	for (UBoidComponent* otherBoid : neighbors) {
		if (otherBoid != b) {//gotta check your not comparing to yourself!
			//FVector dist = (otherBoid->pos - b->pos);
			//float lengthSquared = dist.Size()*dist.Size();//TODO: fix magic numbers
			//force = dist * 1 / lengthSquared;
			force = force + otherBoid->pos;
		}
		else {
		}
	}


	if (neighbors.size() > 1) {
		force = force * (1.0 / (neighbors.size() - 1.0))- b->pos;
	}
	else {
		return FVector(0, 0, 0);
	}

	return attractForce*force*DeltaTime;
}

FVector elijahsAttract(UBoidComponent *b, float DeltaTime) {
	FVector force = FVector(0, 0, 0);
	std::vector<UBoidComponent*> neighbors = b->get_neighbors(eAttractRange);
	for (UBoidComponent* otherBoid : neighbors) {
		if (otherBoid != b) {//gotta check your not comparing to yourself!
			FVector dist = (otherBoid->pos - b->pos);
			float lengthToPower = std::pow(dist.Size(),1.5);//TODO: fix magic numbers
			force = dist / lengthToPower;
		}
	}
	if (neighbors.size() > 1) {
		force = force * (1.0 / (neighbors.size() - 1.0)) - b->pos;
	}
	else {
		return FVector(0, 0, 0);
	}
	return eAttractForce*force*DeltaTime;
}

FVector repell(UBoidComponent *b, float DeltaTime) {
	FVector force = FVector(0, 0, 0);
	std::vector<UBoidComponent*> neighbors = b->get_neighbors(repelltRange);
	for (UBoidComponent* otherBoid : neighbors) {
		if (otherBoid != b) {//gotta check your not comparing to yourself!
			FVector dist = (otherBoid->pos - b->pos);
			float lengthCubed = std::pow(dist.Size()/100,2);
			force = dist / (lengthCubed);
		}
		else {
		}
	}
	return repellForce * force * DeltaTime;
}

FVector matchVel(UBoidComponent *b, float DeltaTime) {
	FVector force = FVector(0, 0, 0);
	std::vector<UBoidComponent*> neighbors = b->get_neighbors(1000);
	for (UBoidComponent* otherBoid : neighbors) {
		if (otherBoid != b) {//gotta check your not comparing to yourself!
			//FVector dist = (otherBoid->pos - b->pos);
			//float distSquared = dist.Size()*dist.Size();//TODO: fix magic numbers
			force = otherBoid->velocity;
		}
		else {
		}
	}
	if (neighbors.size() > 1) {
		force = force * (1.0 / (neighbors.size() - 1.0)) - b->velocity;
	}
	else {
		return FVector(0, 0, 0);
	}
	return matchVelForce * force * DeltaTime;
}

// Sets default values for this component's properties
UBoidComponent::UBoidComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	
	flockKey = "flock1";
	velocity = FVector(0,0,0);
	forward = FVector(1.0, 0.0, 0.0);
	up = FVector(0.0, 0, 1.0);
	side = FVector(0.0, 1.0, 0);
	accel = FVector(1, 0, 0);
	newForward = 1 * forward;//troll cloning of forward
	newSide = 1 * side;
	newUp = 1 * up;
	averagedVel = FVector(0, 0, 0);
	max_speed = 1.0;
	max_accel = 0.1;
	behaviors = std::vector<FVector(*)(UBoidComponent *, float)>();

	pastVels = std::queue<FVector>();
	for (int i = 0; i < 20; i++) {
		pastVels.push(FVector(0, 0, 0));
	}
	behaviors.push_back(&attract);
	behaviors.push_back(&matchVel);
	behaviors.push_back(&repell);
	behaviors.push_back(&attractToCenter);
	behaviors.push_back(&circleCenter);
	//behaviors.push_back(&elijahsAttract);

}
FVector UBoidComponent::GetPos()
{
	return ComponentToWorld.GetLocation();
}

FVector UBoidComponent::behave(float DeltaTime)
{
	FVector acc = FVector(0,0,0);
	for (FVector(*behavior)(UBoidComponent *, float time) : behaviors) {
		acc = acc + behavior(this, DeltaTime);	
	}
	return acc;
}

//calculate and set position, velocity and orientation
void UBoidComponent::fly()
{
	pastVels.push(accel);
	averagedVel = averagedVel - pastVels.front() + accel;
	pastVels.pop();
	FVector acc = ((1.0/20000.0) * averagedVel).GetClampedToMaxSize(max_accel * max_speed);
	newVelocity = (velocity + acc).GetClampedToMaxSize(max_speed);

	// Reorient
	newForward = newVelocity.GetSafeNormal();
	if (newForward.Size() < .0001) {
		newForward = forward;
	}
	FVector aprx_up = up.GetSafeNormal(); // Should be a weighted sum of acc, acc due to gravity, and the old up
	newSide = FVector::CrossProduct(forward, aprx_up).GetSafeNormal();
	//newSide = side;
	newUp = FVector::CrossProduct(forward, side).GetSafeNormal();
}

void UBoidComponent::applyFly()
{
	//pos = newPos;
	velocity = newVelocity;
	forward = newForward;//TODO: does this help?
	up = newUp;

	side = newSide;
	FRotator rot = FMatrix(side, up, -forward, FVector::ZeroVector).Rotator();
	//GetAttachParent()->SetWorldLocationAndRotation(pos, rot);
	GetAttachParent()->MoveComponent(velocity, forward.Rotation(), false);
	pos = ComponentToWorld.GetLocation();
}

// Called when the game starts
void UBoidComponent::BeginPlay()
{
	Super::BeginPlay();

	pos = ComponentToWorld.GetLocation();
	AFlockController::registerBoid(this, flockKey, GetWorld());
}

// Called every frame
void UBoidComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	accel = behave(DeltaTime);
	fly();
}

std::vector<UBoidComponent*> UBoidComponent::get_neighbors(float radius) {
	return AFlockController::get_neighbors(pos, radius, flockKey);
}


//			GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, FString::SanitizeFloat(lengthSquared));
