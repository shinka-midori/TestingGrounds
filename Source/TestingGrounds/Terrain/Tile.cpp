// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "Public/DrawDebugHelpers.h"
#include "Engine/StaticMesh.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	PlaceActors();
}

void ATile::PlaceActors()
{
	FVector Min(0, -2000, 200);
	FVector Max(4000, 2000, 200);
	FBox Bounds(Min, Max);
	
	for (int i = 0; i < Props.Num(); i++)
	{
		int NumberToSpawn = FMath::RandRange(1, 5);

		for (int j = 0; j < NumberToSpawn; j++)
		{			

			if (GenerateLocationAndRotation(Bounds))
			{
				auto ActorToSpawn = GetWorld()->SpawnActor<AActor>(Props[i], SpawnPoint, SpawnRotator);
				ActorToSpawn->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			}			
						
		}
	}	
}

void ATile::PlaceGrass()
{
	FVector Min(0, -2000, 200);
	FVector Max(4000, 2000, 200);
	FBox Bounds(Min, Max);

	for (int i = 0; i < Grass.Num(); i++)
	{
		int NumberToSpawn = FMath::RandRange(10, 20);
		for (int j = 0; j < NumberToSpawn; j++)
		{
			auto SpawnPoint = FMath::RandPointInBox(Bounds);
			//auto GrassToSpawn = GetWorld()->SpawnActor(UHierarchicalInstancedStaticMeshComponent, SpawnPoint);
			//GrassToSpawn->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ATile::GenerateLocationAndRotation(FBox Bounds)
{
	for (int i = 0; i < 100; i++)
	{
		SpawnPoint = FMath::RandPointInBox(Bounds);
		SpawnRotator.Yaw = FMath::RandRange(-180.f, 180.f);
		SpawnRotator = FRotator(0, SpawnRotator.Yaw, 0);

		if (CanSpawnAtLocation(SpawnPoint, SpawnRotator))
		{
			return true;
		}
	}	
	
	return false;
}

bool ATile::CanSpawnAtLocation(FVector Location, FRotator Rotation)
{
	FHitResult HitResult;
	auto GlobalLocation = ActorToWorld().TransformPosition(Location);
	
	auto HasHit = GetWorld()->SweepSingleByChannel(HitResult, GlobalLocation, GlobalLocation, Rotation.Quaternion(), ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(300));
		
	FColor ResultColor = HasHit ? FColor::Red : FColor::Green;
	//DrawDebugBox(GetWorld(), GlobalLocation, FVector(200, 300, 150), Rotation.Quaternion(), ResultColor, true, -1.f, 0, 5.f);
	//DrawDebugSphere(GetWorld(), GlobalLocation, 300, 12, ResultColor, true, -1.0, 0, 2);	
	
	return !HasHit;
}

FVector ATile::GetActorDimensions(AActor* ActorToSpawn)
{
	UStaticMesh* StaticMesh;
	TArray<UStaticMeshComponent*> Components;
	ActorToSpawn->GetComponents<UStaticMeshComponent>(Components);

	for (int32 k = 0; k < Components.Num(); k++)
	{
		UStaticMeshComponent* StaticMeshComponent = Components[k];
	}
	StaticMesh = Components[0]->GetStaticMesh();
	auto Dimensions = StaticMesh->GetBoundingBox().GetSize();
	//UE_LOG(LogTemp, Warning, TEXT("Dimensions: %s"), *Dimensions.ToString())

	return Dimensions;
}

