// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "LongRangeStrafe.generated.h"

/**
 * 
 */
UCLASS()
class FPS_PROJECT_API ULongRangeStrafe : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

private:

	ULongRangeStrafe();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	
};