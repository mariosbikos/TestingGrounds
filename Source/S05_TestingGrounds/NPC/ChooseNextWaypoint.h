// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ChooseNextWaypoint.generated.h"

/**
 * 
 */
UCLASS()
class S05_TESTINGGROUNDS_API UChooseNextWaypoint : public UBTTaskNode
{
	GENERATED_BODY()
protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	TArray < AActor*> PatrolPoints;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector IndexKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector Waypoint;

	void GetPatrolPoints();
	void SetNextWaypoint();
	void CycleToNextWaypoint();

	class UBehaviorTreeComponent* OwnerCompRef = nullptr;
	
};
