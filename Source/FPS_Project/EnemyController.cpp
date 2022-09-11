// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
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



AEnemyController::AEnemyController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
	BlackBoardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoard"));

	SetupPerception();

}

//void AEnemyController::Update(TArray<AActor*> const& updateActors)
//{
//	//Super::Tick(DeltaTime);
//
//	
//}

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

void AEnemyController::InitializeBehaviourTree()
{
	if (BehaviorTree != nullptr)
	{
		RunBehaviorTree(BehaviorTree);
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}
}

void AEnemyController::AIPossess(APawn* const InPawn)
{

	if (Blackboard != nullptr && BehaviorTree != nullptr)
	{
		Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);


		Enemy = Cast<AEnemyChar>(InPawn);

	}
}

void AEnemyController::OnHit()
{
	if (Enemy->EnemyAgro == true)
	{

		Blackboard->SetValueAsBool(FName(TEXT("CanSeePlayer")), true);


	}
}

void AEnemyController::Dead()
{
	if (Enemy->Enemy_Health <= 0)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);

	}

}



