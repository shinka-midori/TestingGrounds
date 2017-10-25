// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Public/DrawDebugHelpers.h"
#include "Engine/StaticMesh.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "ActorPool.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MinExtent = FVector(0, -2000, 200);
	MaxExtent = FVector(4000, 2000, 200);
	NavigationBoundsOffset = FVector(2000, 0, 0);
}

void ATile::SetPool(UActorPool* InPool)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]Setting Pool: %s"), *GetName(), *InPool->GetName())
	Pool = InPool;

	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = Pool->Checkout();
	if (NavMeshBoundsVolume == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Not enough actors in pool."), *GetName());
		return;
	}
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
	GetWorld()->GetNavigationSystem()->Build();
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
	PlaceActors();
	PlaceAIActors();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Pool->Return(NavMeshBoundsVolume);
}

void ATile::PlaceActors()
{
	FBox Bounds(MinExtent, MaxExtent);
	
	for (int i = 0; i < Props.Num(); i++)
	{
		int NumberToSpawn = FMath::RandRange(1, 5);

		for (int j = 0; j < NumberToSpawn; j++)
		{			

			if (GenerateLocationAndRotation(Bounds))
			{
				auto ActorToSpawn = GetWorld()->SpawnActor<AActor>(Props[i], SpawnTransform.Location, SpawnTransform.Rotation);
				ActorToSpawn->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			}			
						
		}
	}	
}

void ATile::PlaceAIActors()
{
	FBox Bounds(MinExtent, MaxExtent);
	for (int i = 0; i < AIActors.Num(); i++)
	{
		int NumberToSpawn = FMath::RandRange(1, 5);

		for (int j = 0; j < NumberToSpawn; j++)
		{

			if (GenerateLocationAndRotation(Bounds))
			{
				auto ActorToSpawn = GetWorld()->SpawnActor<APawn>(AIActors[i], SpawnTransform.Location, SpawnTransform.Rotation);
				if (ActorToSpawn == nullptr) { return; }
				ActorToSpawn->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
				ActorToSpawn->SpawnDefaultController();
				ActorToSpawn->Tags.Add(FName("Enemy"));
			}

		}
	}
}

//void ATile::PlaceGrass()
//{
//	FBox Bounds(MinExtent, MaxExtent);
//
//	for (int i = 0; i < Grass.Num(); i++)
//	{
//		int NumberToSpawn = FMath::RandRange(10, 20);
//		for (int j = 0; j < NumberToSpawn; j++)
//		{
//			SpawnTransform.Location = FMath::RandPointInBox(Bounds);
//			//auto GrassToSpawn = GetWorld()->SpawnActor(UHierarchicalInstancedStaticMeshComponent, SpawnTransform.Location);
//			//GrassToSpawn->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
//		}
//	}
//}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


bool ATile::GenerateLocationAndRotation(FBox Bounds)
{
	for (int i = 0; i < 100; i++)
	{
		SpawnTransform.Location = FMath::RandPointInBox(Bounds);
		SpawnTransform.Rotation.Yaw = FMath::RandRange(-180.f, 180.f);
		SpawnTransform.Rotation = FRotator(0, SpawnTransform.Rotation.Yaw, 0);

		if (CanSpawnAtLocation(SpawnTransform.Location, SpawnTransform.Rotation))
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
	
	return Dimensions;
}

