// Fill out your copyright notice in the Description page of Project Settings.


#include "AK.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAK::AAK()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EquipedWeapon.weapon = Weapon::Primary;

	RenderAssets();

	EquipedWeapon.GunAmmo = AK_GunAmmo;

	EquipedWeapon.MinRecoil = 0.f;
	EquipedWeapon.MaxRecoil = -0.2f;
	
	EquipedWeapon.ADS_PlayRate = 1.f;

	EquipedWeapon.LowAmmo = 10;

	
}



// Called when the game starts or when spawned
void AAK::BeginPlay()
{
	Super::BeginPlay();
	
	EquipedWeapon.MaxAmmo = AK_MaxAmmo;
	EquipedWeapon.RateOfFire = AK_RateOfFire;
	EquipedWeapon.BulletSpread = AK_BulletSpread;

	EquipedWeapon.Gun_UI_Magazine = EquipedWeapon.GunAmmo;
	EquipedWeapon.Gun_UI_MaxMagazine = EquipedWeapon.MaxAmmo;
}

// Called every frame
void AAK::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ReloadUI();

}

void AAK::Shoot(UCameraComponent* FollowCamera, AActor* Player, bool IsAds)
{
	float BulletSpreadRange = FMath::RandRange(EquipedWeapon.BulletSpread * -1.f, EquipedWeapon.BulletSpread);

	Start = FollowCamera->GetComponentLocation() + LineTraceOffset;
	End = ((FollowCamera->GetForwardVector() * 5000) + Start) + FVector(BulletSpreadRange, BulletSpreadRange, BulletSpreadRange);


	FHitResult OutHit;

	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(FollowCamera->GetOwner());
	ActorsToIgnore.Add(Player);

	bool IsHit = false;

	IsHit = UKismetSystemLibrary::LineTraceSingle(Player->GetWorld(), Start, End, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera),
		false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true, FLinearColor::Red, FLinearColor::Green, 0.f);


	/* UKismetSystemLibrary::LineTraceSingle(Player->GetWorld(), MuzzleComp->GetComponentLocation(), OutHit.ImpactPoint, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera),
		false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FLinearColor::Green, FLinearColor::Green, 1.f);*/


	if (IsHit == true)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Got Hit")));

		SpawnDecal(OutHit);

	}

	FireAnimAndFX(IsAds);

	IsShooting = true;

	CurveTimeline.PlayFromStart();
}



