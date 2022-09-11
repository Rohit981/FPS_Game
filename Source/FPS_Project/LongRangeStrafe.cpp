// Fill out your copyright notice in the Description page of Project Settings.


#include "LongRangeStrafe.h"
#include "LongRangeEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LongRangeEnemyChar.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

ULongRangeStrafe::ULongRangeStrafe()
{
	NodeName = TEXT("Long Range Strafe");

}

EBTNodeResult::Type ULongRangeStrafe::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ALongRangeEnemyController* AIController = Cast<ALongRangeEnemyController>(OwnerComp.GetAIOwner());

	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);


	ALongRangeEnemyChar* LongRangeChar = Cast<ALongRangeEnemyChar>(AIController->GetPawn());

	AIController->SetFocus(player, EAIFocusPriority::Gameplay);

	if (LongRangeChar->CanFire == false)
	{
		LongRangeChar->CanStrafe = true;

	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
