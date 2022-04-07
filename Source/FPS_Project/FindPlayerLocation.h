// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FindPlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class FPS_PROJECT_API UFindPlayerLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UFindPlayerLocation();

private:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ALlowPrivateAccess = true))
	bool IsSearching = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ALlowPrivateAccess = true))
	float Search_Radius = 150.f;


	
};
