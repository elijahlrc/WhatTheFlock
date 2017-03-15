// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <vector>
#include <map>
#include <string>
#include "BoidComponent.h"
#include "GameFramework/Actor.h"
#include "FlockController.generated.h"

//class UBoidComponent;

UCLASS()
class BOIDSTORM_API AFlockController : public AActor
{
	GENERATED_BODY()
	
public:	
	
	// Sets default values for this actor's properties
	AFlockController();

	std::vector<UBoidComponent*> boids;
	static AFlockController* getControllerByKey(std::string key);
	static void registerBoid(UBoidComponent * b, std::string key, UWorld* world);
	static std::vector<UBoidComponent*> get_neighbors(FVector pos, float radius, std::string key);
	static std::map<std::string, AFlockController*> controllers;
protected:
	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
