// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPS_ProjectCharacter.h"
#include "GameFramework/Character.h"


AEnemyController::AEnemyController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
	BlackBoardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoard"));

	SetupPerception();

}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree != nullptr)
	{
		RunBehaviorTree(BehaviorTree);
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}
}

void AEnemyController::OnPossess(APawn* const InPawn)
{
	Super::OnPossess(InPawn);

	if (Blackboard != nullptr && BehaviorTree != nullptr)
	{
		Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	}

}

void AEnemyController::Update(TArray<AActor*> const& updateActors)
{

}

void AEnemyController::OnTargetDetected(AActor* Actor, FAIStimulus stimulus)
{
	if (AFPS_ProjectCharacter* character = Cast<AFPS_ProjectCharacter>(Actor))
	{
		Blackboard->SetValueAsBool(FName(TEXT("CanSeePlayer")), stimulus.WasSuccessfullySensed());
	}
}

void AEnemyController::SetupPerception()
{
	sight_config = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	if (sight_config)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		sight_config->SightRadius = 600.0f;
		sight_config->LoseSightRadius = sight_config->SightRadius + 25.0f;
		sight_config->PeripheralVisionAngleDegrees = 90.0f;
		sight_config->SetMaxAge(5.0f);
		sight_config->AutoSuccessRangeFromLastSeenLocation = 520.0f;
		sight_config->DetectionByAffiliation.bDetectEnemies =
			sight_config->DetectionByAffiliation.bDetectFriendlies =
			sight_config->DetectionByAffiliation.bDetectNeutrals = true;

		// add sight configuration component to perception component
		GetPerceptionComponent()->SetDominantSense(*sight_config->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*sight_config);
	}



}


