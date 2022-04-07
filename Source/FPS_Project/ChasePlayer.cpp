// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayer.h"
#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


UChasePlayer::UChasePlayer()
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyController* AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	FVector playerLoc = AIController->GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

	Enemy = Cast<AEnemyChar>(AIController->GetPawn());

	if (Enemy->Is_Punching == false)
	{
	   UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, playerLoc);

	}


	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
