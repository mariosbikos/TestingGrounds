// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIModule/Classes/AIController.h"
#include "PatrolRoute.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//OwnerComp->
	OwnerCompRef = &OwnerComp;

	GetPatrolPoints();
	SetNextWaypoint();
	CycleToNextWaypoint();

	return EBTNodeResult::Succeeded;
}

void UChooseNextWaypoint::GetPatrolPoints()
{
	auto AIController = OwnerCompRef->GetAIOwner();
	UPatrolRoute* PatrolRouteComp = Cast<UPatrolRoute>(AIController->GetPawn()->FindComponentByClass<UPatrolRoute>());
	if (!PatrolRouteComp) { return; }
	PatrolPoints = PatrolRouteComp->GetPatrolPoints();
}

void UChooseNextWaypoint::SetNextWaypoint()
{
	UBlackboardComponent* BlackboardComp = OwnerCompRef->GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackboardComp->SetValueAsObject(Waypoint.SelectedKeyName, PatrolPoints[Index]);

}

void UChooseNextWaypoint::CycleToNextWaypoint()
{
	UBlackboardComponent* BlackboardComp = OwnerCompRef->GetBlackboardComponent();

	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	Index = (Index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, Index);
		
}
