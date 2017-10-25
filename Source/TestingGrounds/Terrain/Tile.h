// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Tile.generated.h"

USTRUCT()
struct FSpawnTransform
{
	GENERATED_BODY()

	FVector Location;
	FRotator Rotation;
	float Scale;
};

class UActorPool;

UCLASS()
class TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void PlaceActors();

	void PlaceAIActors();

	/*UFUNCTION(BlueprintCallable, Category = "Setup")
	void PlaceGrass();*/

	bool CanSpawnAtLocation(FVector, FRotator);

	FVector GetActorDimensions(AActor*);

	bool GenerateLocationAndRotation(FBox);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	FVector MinExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	FVector MaxExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	FVector NavigationBoundsOffset;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Props")
	TArray<TSubclassOf<AActor>> Props;	

	UPROPERTY(EditDefaultsOnly, Category = "AI Actors")
	TArray<TSubclassOf<APawn>> AIActors;

	/*UPROPERTY(EditDefaultsOnly, Category = "Props")
	TArray<TSubclassOf<UHierarchicalInstancedStaticMeshComponent>> Grass;*/

	UFUNCTION(BlueprintCallable, Category = "Nav Mesh Pool")
	void SetPool(UActorPool* InPool);

	void PositionNavMeshBoundsVolume();	
	
private:
	UActorPool* Pool;
	AActor* NavMeshBoundsVolume;
	FSpawnTransform SpawnTransform;
};
