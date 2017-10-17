// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PatrollingGuard.h"
#include "AIController.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
    auto AIPawn = OwnerComp.GetAIOwner()->GetPawn();
    GetPatrolPoints(AIPawn);
    int32 Index = SetNewWaypoint(BlackboardComponent);
    CycleIndex(BlackboardComponent, Index);
    return EBTNodeResult::Succeeded;
}

void UChooseNextWaypoint::GetPatrolPoints(APawn* AIPawn)
{
    if (!AIPawn) return;
    auto PatrollingGuard = Cast<APatrollingGuard>(AIPawn);
    PatrolPoints = PatrollingGuard->GetPatrolPoints();
}

int32 UChooseNextWaypoint::SetNewWaypoint(UBlackboardComponent* BlackboardComponent)
{
    if (!BlackboardComponent) return 0;
    auto Index = BlackboardComponent->GetValueAsInt(IndexKey.SelectedKeyName);
    BlackboardComponent->SetValueAsObject(NextWaypoint.SelectedKeyName, PatrolPoints[Index]);
    return Index;
}

void UChooseNextWaypoint::CycleIndex(UBlackboardComponent* BlackboardComponent, int32 IndexToIncrement)
{
    IndexToIncrement++;
    BlackboardComponent->SetValueAsInt(IndexKey.SelectedKeyName, IndexToIncrement % PatrolPoints.Num());
}
