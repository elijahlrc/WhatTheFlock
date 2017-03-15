// Fill out your copyright notice in the Description page of Project Settings.

#include "BoidStorm.h"
#include "BoidComponent.h"
#include "FlockController.h"

std::map<std::string, AFlockController*> AFlockController::controllers = std::map<std::string, AFlockController*>();
AFlockController* AFlockController::getControllerByKey(std::string key)
{
	return controllers[key];
}
void AFlockController::registerBoid(UBoidComponent *b, std::string key, UWorld* world)
{
	UE_LOG(LogTemp, Warning, TEXT("Registering boid with name %s"), &key)
	if (controllers.count(key) == 1) {
		controllers[key]->boids.push_back(b);
		UE_LOG(LogTemp, Warning, TEXT("adding to existing controller"))
		UE_LOG(LogTemp, Warning, TEXT("Controller now has %d boids"), controllers[key]->boids.size())
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Creating new controller"))
		FVector Location(0.0f, 0.0f, 0.0f);
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;
		AFlockController * newFc = world->SpawnActor<AFlockController>(Location, Rotation, SpawnInfo);
		controllers.insert(std::make_pair(key, newFc));
		controllers[key]->boids.push_back(b);
	}
}
// Sets default values
AFlockController::AFlockController()
{
 	// Set this actor to call Tick() every frame.  
	//You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostUpdateWork;
	boids = std::vector<UBoidComponent*>();
}

std::vector<UBoidComponent*> AFlockController::get_neighbors(FVector pos, float radius, std::string key)
{

	std::vector<UBoidComponent*> neighbors;
	for (UBoidComponent* b : controllers[key]->boids) {
		if ((pos-b->pos).Size() < radius) {
			neighbors.push_back(b);
		}
	}
	return neighbors;
}

// Called when the game starts or when spawned
void AFlockController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlockController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (UBoidComponent *b: boids){

		b->applyFly();
	}
}

