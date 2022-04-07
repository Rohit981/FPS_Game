// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EnemyPathFindingTask.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FPS_PROJECT_API UEnemyPathFindingTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UEnemyPathFindingTask();

private:

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = true))
	float SearchRadius = 500;
	
};
