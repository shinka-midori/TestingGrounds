// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool.h"


// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActorPool::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UActorPool::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


AActor* UActorPool::Checkout()
{
	if (Pool.Num() == 0)
	{
		return nullptr;
	}
	return Pool.Pop();
}

void UActorPool::Return(AActor* ActorToReturn)
{
	if (ActorToReturn == nullptr)
	{
		return;
	}
	Add(ActorToReturn);
}

void UActorPool::Add(AActor* ActorToAdd)
{
	if (ActorToAdd == nullptr)
	{
		return;
	}
	Pool.Push(ActorToAdd);
}
