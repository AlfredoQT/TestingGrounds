// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PatrolRouteComponent.h"
#include "AIController.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
    auto AIPawn = OwnerComp.GetAIOwner()->GetPawn();
    auto PatrolRouteComponent = AIPawn->FindComponentByClass<UPatrolRouteComponent>();
    if (!ensure(PatrolRouteComponent)) return EBTNodeResult::Failed;
    GetPatrolPoints(PatrolRouteComponent);
    if (PatrolPoints.Num() == 0) {
        UE_LOG(LogTemp, Warning, TEXT("%s is missing patrol points"), *AIPawn->GetName());
        return EBTNodeResult::Failed;
    }
    int32 Index = SetNewWaypoint(BlackboardComponent);
    CycleIndex(BlackboardComponent, Index);
    return EBTNodeResult::Succeeded;
}

void UChooseNextWaypoint::GetPatrolPoints(UPatrolRouteComponent* PatrolRouteComponent)
{
    if (!PatrolRouteComponent) return;
    PatrolPoints = PatrolRouteComponent->GetPatrolPoints();
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
