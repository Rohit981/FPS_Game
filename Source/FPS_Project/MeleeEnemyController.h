// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.h"
#include "MeleeEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class FPS_PROJECT_API AMeleeEnemyController : public AEnemyController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* const InPawn) override;

private:
   AMeleeEnemyController();
	
};
