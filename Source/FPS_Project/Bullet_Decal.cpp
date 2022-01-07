// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet_Decal.h"

// Sets default values
ABullet_Decal::ABullet_Decal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	DecalComp->AttachTo(RootComponent);

}

// Called when the game starts or when spawned
void ABullet_Decal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet_Decal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

