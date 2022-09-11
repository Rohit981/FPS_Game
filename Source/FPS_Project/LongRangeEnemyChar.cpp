// Fill out your copyright notice in the Description page of Project Settings.


#include "LongRangeEnemyChar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"



// Sets default values
ALongRangeEnemyChar::ALongRangeEnemyChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



}

// Called when the game starts or when spawned
void ALongRangeEnemyChar::BeginPlay()
{
	Super::BeginPlay();

	GunSpawner();

	
}


void ALongRangeEnemyChar::GunSpawner()
{
	if (GunSubclass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			FVector Location = GetActorLocation();
			FRotator Rotation = GetActorRotation();

			
			Gun = World->SpawnActor<AEnemyGun>(GunSubclass, Location, Rotation);

			if (Gun != nullptr)
			{
				Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Rifle_Socket"));
				
			}
		}
	}
}





// Called every frame
void ALongRangeEnemyChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AnimInstance = GetMesh()->GetAnimInstance();


	if (IsAttacked == true && Enemy_Health > 0 && CanStrafe == false)
	{

		FireGun();

		StartFiringTimer(DeltaTime);

	

	}

	if (CanStrafe == true)
	{
		Strafe();

		StopstrafeTimer += DeltaTime;
		

	}

	if (StopstrafeTimer >= 1.f)
	{
		CanStrafe = false;

		StopstrafeTimer = 0;
	}
	
	Dead();

}

void ALongRangeEnemyChar::FireGun()
{
	if (CanFire == true )
	{
		Gun->Firing();

	}

	

}

void ALongRangeEnemyChar::StartFiringTimer(float DeltaTime)
{

	countingTimerDown -= DeltaTime;

	if (countingTimerDown <= 0)
	{
		CanFire = true;

		countingTimerDown = 3.0f;

	}
	else
	{
		CanFire = false;
	}

}


void ALongRangeEnemyChar::Strafe()
{
	if (GetCharacterMovement()->bOrientRotationToMovement == true)
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	AddMovementInput(GetActorRightVector(),1);


	
}

void ALongRangeEnemyChar::Dead()
{
	if (Enemy_Health <= 0)
	{
		UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		//SetActorEnableCollision(true);
	}
}
