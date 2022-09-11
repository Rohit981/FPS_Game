// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyChar.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"




// Sets default values
AEnemyChar::AEnemyChar()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthBar_UI = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar_UI->AttachTo(RootComponent);
	
	
}

// Called when the game starts or when spawned
void AEnemyChar::BeginPlay()
{
	Super::BeginPlay();

}





// Called every frame
void AEnemyChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Player = Cast<AFPS_ProjectCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	AnimInstance = GetMesh()->GetAnimInstance();

	if (Player->Enemy_IsLookOn == false)
	{
		Enemy_IsLookOn = false;

	}


}



void AEnemyChar::DamageTaken(int BulletDMG, FVector BulletHitPos)
{
	EnemyDamageTaken = BulletDMG;

	HitPosition = BulletHitPos;

	hitCounter += 1;

	if (hitCounter == 1)
	{
	   AnimInstance->Montage_Play(HitReaction_Montage, 1.f, EMontagePlayReturnType::Duration, 0.f);

	}

	Bullet_Hit = true;

	EnemyAgro = true;


	if (Damage_HUD != nullptr && Enemy_Health > 0)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			FVector Location = HitPosition;

			World->SpawnActor<AEnemyDamageUI>(Damage_HUD, Location, FRotator(0,0,0));

			

		}
	}

}

void AEnemyChar::BulletHit(float DeltaTime)
{
	if (Bullet_Hit == true)
	{
		HitReactionTime += DeltaTime;

	}

	if (HitReactionTime >= 2.f)
	{
		HitReactionTime = 0;

		hitCounter = 2;

		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_NavWalking);

	}

}



