// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AEnemyChar::AEnemyChar()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PunchHitBox = CreateDefaultSubobject<USphereComponent>(TEXT("PunchCollider"));
	PunchHitBox->AttachTo(RootComponent, TEXT("RightHand"));

	HealthBar_UI = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar_UI->AttachTo(RootComponent);
	
	
	
}

// Called when the game starts or when spawned
void AEnemyChar::BeginPlay()
{
	Super::BeginPlay();

	PunchHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PunchHitBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyChar::OnOverlapBegin);

	
}





// Called every frame
void AEnemyChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HitCheck();

	Player = Cast<AFPS_ProjectCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (Is_Punching == true)
		attackWaitTime += DeltaTime;

	if (Player->Enemy_IsLookOn == true)
	{
		HealthBar_UI->SetVisibility(true);
	}
	else
	{
		HealthBar_UI->SetVisibility(false);

	}

	


}

void AEnemyChar::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		Player->Health -= 10;

		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Player Health: %f"), Player->Health));

	}

}

void AEnemyChar::DamageTaken(int BulletDMG, FVector BulletHitPos)
{
	EnemyDamageTaken = BulletDMG;

	HitPosition = BulletHitPos;

	if (Damage_HUD != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			FVector Location = HitPosition;

			World->SpawnActor<AEnemyDamageUI>(Damage_HUD, Location, FRotator(0,0,0));

			

		}
	}

}






void AEnemyChar::HitCheck()
{
	FVector Start = GetActorLocation();

	FVector End = (GetActorForwardVector() * DetectionLength) + Start;


	FHitResult OutHit;

	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(this);

	bool IsHit = false;

	IsHit = UKismetSystemLibrary::SphereTraceSingle(this->GetWorld(), Start, End, 20, ETraceTypeQuery::TraceTypeQuery3, false, ActorsToIgnore,
		                                            EDrawDebugTrace::None, OutHit, true, FLinearColor::Red, FLinearColor::Green, 0.f);

	if (IsHit == true)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Got Hit")));

		Is_Punching = true;

	}
	else
	{
		if (attackWaitTime >= 3.f)
		{
			Is_Punching = false;

			attackWaitTime = 0;

		}

	}
}


void AEnemyChar::Attacking()
{
	PunchHitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

void AEnemyChar::StopAttacking()
{

	PunchHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}
