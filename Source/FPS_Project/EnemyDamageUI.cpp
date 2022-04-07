// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDamageUI.h"

// Sets default values
AEnemyDamageUI::AEnemyDamageUI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage_UI = CreateDefaultSubobject<UWidgetComponent>(TEXT("Damage_UI"));
	Damage_UI->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AEnemyDamageUI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyDamageUI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

