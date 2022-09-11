// Fill out your copyright notice in the Description page of Project Settings.


#include "LongRangeAttackTask.h"
#include "LongRangeEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LongRangeEnemyChar.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

ULongRangeAttackTask::ULongRangeAttackTask()
{
	NodeName = TEXT("Long Range Attack");
}

EBTNodeResult::Type ULongRangeAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ALongRangeEnemyController* AIController = Cast<ALongRangeEnemyController>(OwnerComp.GetAIOwner());

	ALongRangeEnemyChar* LongRangeChar = Cast<ALongRangeEnemyChar>(AIController->GetPawn());

	LongRangeChar->IsAttacked = true;

	

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);


	return EBTNodeResult::Succeeded;
}
