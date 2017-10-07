// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PatrollingGuard.h"
#include "AIController.h"



EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get control points
	auto AIController = OwnerComp.GetAIOwner();
	auto ControlledPawn = AIController->GetControlledPawn();
	auto PatrollingGuard = Cast<APatrollingGuard>(ControlledPawn);
	auto PatrollingPoints = PatrollingGuard->PatrolPointsCPP;

	// Set next waypoint
	auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComponent->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackboardComponent->SetValueAsObject(WaypointKey.SelectedKeyName, PatrollingPoints[Index]);


	// Cycle the index
	auto NextIndex = (Index + 1) % PatrollingPoints.Num();
	BlackboardComponent->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	UE_LOG(LogTemp, Warning, TEXT("Waypoint Index: %i"), Index)
	return EBTNodeResult::Succeeded;
}
