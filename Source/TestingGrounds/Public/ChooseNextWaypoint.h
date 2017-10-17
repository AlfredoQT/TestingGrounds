// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ChooseNextWaypoint.generated.h"

/**
 * 
 */

class UBlackboardComponent;
class UPatrolRouteComponent;

UCLASS()
class TESTINGGROUNDS_API UChooseNextWaypoint : public UBTTaskNode
{
	GENERATED_BODY()
public:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    void GetPatrolPoints(UPatrolRouteComponent* PatrolRouteComponent);
    int32 SetNewWaypoint(UBlackboardComponent* BlackboardComponent);
    void CycleIndex(UBlackboardComponent* BlackboardComponent, int32 IndexToIncrement);
private:
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector IndexKey;
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector NextWaypoint;
    
    TArray<AActor*> PatrolPoints;
	
};
