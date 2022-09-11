// Fill out your copyright notice in the Description page of Project Settings.


#include "Glock.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGlock::AGlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EquipedWeapon.weapon = Weapon::Secondary;

	RenderAssets();

	EquipedWeapon.GunAmmo = 10;
	EquipedWeapon.MaxAmmo = 180;
	
	EquipedWeapon.ADS_PlayRate = 0.8f;

	EquipedWeapon.LowAmmo = 5;

}



// Called when the game starts or when spawned
void AGlock::BeginPlay()
{
	Super::BeginPlay();

	EquipedWeapon.RateOfFire = Glock_RateOfFire;
	EquipedWeapon.BulletSpread = Glock_BulletSpread;

	EquipedWeapon.Gun_UI_Magazine = EquipedWeapon.GunAmmo;
	EquipedWeapon.Gun_UI_MaxMagazine = EquipedWeapon.MaxAmmo;

	EquipedWeapon.BodyShotDMG = 20;
	EquipedWeapon.HeadShotDMG = 40;
	
}

// Called every frame
void AGlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ReloadUI();

	
}

void AGlock::Shoot(UCameraComponent* FollowCamera, AActor* Player, bool IsAds)
{
	float BulletSpreadRange = FMath::RandRange(EquipedWeapon.BulletSpread * -1.f, EquipedWeapon.BulletSpread);

	Start = FollowCamera->GetComponentLocation() + LineTraceOffset;
	End = ((FollowCamera->GetForwardVector() * 5000) + Start) + FVector(BulletSpreadRange, BulletSpreadRange, BulletSpreadRange);


	TArray<FHitResult> OutHit;

	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(FollowCamera->GetOwner());
	ActorsToIgnore.Add(Player);

	bool IsHit = false;

	IsHit = UKismetSystemLibrary::LineTraceMulti(Player->GetWorld(), Start, End, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera),
		false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FLinearColor::Red, FLinearColor::Green, 0.f);


	/* UKismetSystemLibrary::LineTraceSingle(Player->GetWorld(), MuzzleComp->GetComponentLocation(), OutHit.ImpactPoint, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera),
		false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FLinearColor::Green, FLinearColor::Green, 1.f);*/


	if (IsHit == true)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Got Hit")));

		EnemyHit(OutHit);

		SpawnDecal(OutHit);

	}

	FireAnimAndFX(IsAds);

	IsShooting = true;

	
}

