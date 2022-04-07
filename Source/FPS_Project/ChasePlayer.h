// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EnemyChar.h"
#include "ChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class FPS_PROJECT_API UChasePlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UChasePlayer();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ALlowPrivateAccess = true))
	AEnemyChar* Enemy;
	
};
