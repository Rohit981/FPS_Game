// Fill out your copyright notice in the Description page of Project Settings.


#include "LongRangeEnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPS_ProjectCharacter.h"
#include "GameFramework/Character.h"


ALongRangeEnemyController::ALongRangeEnemyController()
{

}

void ALongRangeEnemyController::BeginPlay()
{
	Super::BeginPlay();

	InitializeBehaviourTree();
}


void ALongRangeEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OnHit();

	Dead();
}

void ALongRangeEnemyController::OnPossess(APawn* const InPawn)
{
	Super::OnPossess(InPawn);

	AIPossess(InPawn);
}


