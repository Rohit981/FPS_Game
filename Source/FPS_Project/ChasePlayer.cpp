// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayer.h"
#include "EnemyController.h"
#include "MeleeEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MeleeEnemyChar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


UChasePlayer::UChasePlayer()
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMeleeEnemyController* AIController = Cast<AMeleeEnemyController>(OwnerComp.GetAIOwner());
	FVector playerLoc = AIController->GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

	AMeleeEnemyChar* MeleeEnemy = Cast<AMeleeEnemyChar>(AIController->GetPawn());

	
	MeleeEnemy->GetCharacterMovement()->MaxWalkSpeed = 800;

	
	
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, playerLoc);



	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
