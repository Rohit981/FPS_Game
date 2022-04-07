// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyController.generated.h"

class UBehaviorTreeComponent;

/**
 *
 */
UCLASS()
class FPS_PROJECT_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyController();

protected:

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* const InPawn) override;

	

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (ALlowPrivateAccess = true))
		UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (ALlowPrivateAccess = true))
		UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (ALlowPrivateAccess = true))
		UBlackboardComponent* BlackBoardComponent;

	class UAISenseConfig_Sight* sight_config;

	UFUNCTION()
	void Update(TArray<AActor*> const& updateActors);

	UFUNCTION()
	void OnTargetDetected(AActor * Actor, FAIStimulus stimulus);

	void SetupPerception();

};
