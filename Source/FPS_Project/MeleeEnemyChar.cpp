// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeEnemyChar.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/CapsuleComponent.h"




// Sets default values
AMeleeEnemyChar::AMeleeEnemyChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PunchHitBox = CreateDefaultSubobject<USphereComponent>(TEXT("PunchCollider"));
	PunchHitBox->AttachTo(RootComponent, TEXT("RightHand"));

	

}

// Called when the game starts or when spawned
void AMeleeEnemyChar::BeginPlay()
{
	Super::BeginPlay();

	PunchHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PunchHitBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeEnemyChar::OnOverlapBegin);
	
}



// Called every frame
void AMeleeEnemyChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Is_Punching == true)
	{
		attackWaitTime += DeltaTime;

		
	}

	if (canMove == false || hitCounter == 1)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	}


	if (Enemy_Health > 0)
	HitCheck();
	

	PlayerHitReact();

	BulletHit(DeltaTime);

	Dead();

	

}

void AMeleeEnemyChar::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		Player->Health -= 10;


	}
}



void AMeleeEnemyChar::Attacking()
{
	PunchHitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	playerHit = true;

}

void AMeleeEnemyChar::StopAttacking()
{

	PunchHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	playerHit = false;

	canMove = true;

	


}

void AMeleeEnemyChar::HitCheck()
{
	FVector Start = GetActorLocation();

	FVector End = (GetActorForwardVector() * DetectionLength) + Start;


	FHitResult OutHit;

	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(this);

	IsHit = false;

	IsHit = UKismetSystemLibrary::SphereTraceSingle(this->GetWorld(), Start, End, 40, ETraceTypeQuery::TraceTypeQuery3, false, ActorsToIgnore,
		EDrawDebugTrace::None, OutHit, true, FLinearColor::Red, FLinearColor::Green, 0.f);

	if (IsHit == true && attackWaitTime == 0)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Got Hit")));

		Is_Punching = true;

		
		canMove = false;

	}
	else
	{
		if (attackWaitTime >= 2.f)
		{
			Is_Punching = false;

			attackWaitTime = 0;

			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_NavWalking);
		}



	}
}


void AMeleeEnemyChar::Dead()
{
	if (Enemy_Health <= 0)
	{

		Is_Dead = true;

		HealthBar_UI->SetVisibility(false);

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);



	}
}

void AMeleeEnemyChar::PlayerHitReact()
{
	FVector back = Player->GetActorForwardVector();

	if (playerHit == true)
	{

		Player->AddMovementInput(-back * 0.1f);

	}
	else
	{
		Player->AddMovementInput(-back * 0);

	}
}
