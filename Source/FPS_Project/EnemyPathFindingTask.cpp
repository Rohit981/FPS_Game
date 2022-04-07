// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPathFindingTask.h"
#include "AIController.h"
#include "EnemyController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"



UEnemyPathFindingTask::UEnemyPathFindingTask()
{
    NodeName = TEXT("Find Random Location");
  
}

EBTNodeResult::Type UEnemyPathFindingTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    FNavLocation Location;

    AEnemyController* AIController = Cast<AEnemyController>( OwnerComp.GetAIOwner());
    APawn* AIPawn = AIController->GetPawn();

    FVector Origin = AIPawn->GetActorLocation();

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

    if (NavSystem != nullptr && NavSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, Location))
    {
        AIController->GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Location.Location);

    }

    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

    return EBTNodeResult::Succeeded;
}

FString UEnemyPathFindingTask::GetStaticDescription() const
{
    return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}
