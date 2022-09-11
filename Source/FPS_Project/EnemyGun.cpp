// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGun.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyGun::AEnemyGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GunComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	GunComp->AttachTo(RootComponent);

	MuzzleComp = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	MuzzleComp->AttachTo(GunComp);

	MuzzleParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Muzzle_FX"));
	MuzzleParticle->AttachTo(MuzzleComp);
}

// Called when the game starts or when spawned
void AEnemyGun::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyGun::Firing()
{

	MuzzleVFX();

	SpawnBullet();

}



void AEnemyGun::MuzzleVFX()
{
	MuzzleParticle->SetWorldScale3D(FVector(3.f, 3.f, 3.f));

	MuzzleParticle->SetActive(true);
}

void AEnemyGun::SpawnBullet()
{
	if (EnemyBulletClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			FVector Location = MuzzleComp->GetComponentLocation();
			FRotator OldRotation = MuzzleComp->GetComponentRotation() + RotationOffset;

			World->SpawnActor<AEnemyBulletsRounds>(EnemyBulletClass, Location, OldRotation);

		}
	}
}

// Called every frame
void AEnemyGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

