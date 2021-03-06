// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/TimelineComponent.h"
#include "EnemyChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}


// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentAmmo = EquipedWeapon.GunAmmo;


	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("Recoil"));
		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);

	}

}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurveTimeline.TickTimeline(DeltaTime);

}

void AGun::Shoot(UCameraComponent* FollowCamera, AActor* Player, bool IsAds)
{
}


void AGun::RenderAssets()
{
	GunComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	GunComp->AttachTo(RootComponent);

	MuzzleComp = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	MuzzleComp->AttachTo(GunComp);

	MuzzleParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Muzzle_FX"));
	MuzzleParticle->AttachTo(MuzzleComp);

}

void AGun::SpawnDecal(TArray<FHitResult> OutHit)
{
	for (int i = 0; i < OutHit.Num(); i++)
	{
		if (DecalSubclass != nullptr)
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				FVector Location = OutHit[i].ImpactPoint;
				FRotator OldRotation = OutHit[i].ImpactPoint.Rotation();

				FRotator Rotation = UKismetMathLibrary::MakeRotFromX(FVector(OldRotation.Vector().X));

				World->SpawnActor<ABullet_Decal>(DecalSubclass, Location, Rotation);

				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletHit_FX, Location, Rotation);

			}
		}

	}
}


void AGun::FireAnimAndFX(bool IsAds)
{
	GunComp->PlayAnimation(FireAnim, false);

	if (IsAds == true)
	{

		MuzzleParticle->SetWorldScale3D(FVector(0.5, 0.5, 0.5));

	}
	else
	{


		MuzzleParticle->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
	}

	MuzzleParticle->SetActive(true);
}

void AGun::GunReloading(UAnimInstance* AnimInstance)
{
	if (CurrentAmmo < EquipedWeapon.GunAmmo)
	{
		IsGunReloading = true;
		AnimInstance->Montage_Play(ReloadMontage, 1.f, EMontagePlayReturnType::MontageLength, 0.f);

		Reload();

	}
}

void AGun::Reload()
{
	GunComp->PlayAnimation(ReloadAnim, false);
}



void AGun::ReloadUI()
{
	int GunCurrentMaxAmmo = 0;

	EquipedWeapon.Gun_UI_Magazine = CurrentAmmo;

	if (bReloading == true)
	{
		
		AmmoDifference = EquipedWeapon.GunAmmo - CurrentAmmo;

		/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Gloack MaxAmmo: %i"), AmmoDifference));*/

		EquipedWeapon.Gun_UI_MaxMagazine = EquipedWeapon.Gun_UI_MaxMagazine - AmmoDifference;

		CurrentAmmo = EquipedWeapon.GunAmmo;

		bReloading = false;

	

		
	}
	
	
}

void AGun::EnemyHit(TArray<FHitResult> OutHit)
{
	for (int i = 0; i < OutHit.Num(); i++)
	{
		AEnemyChar* Enemies = Cast<AEnemyChar>(OutHit[i].GetActor());

		if (OutHit[i].Actor == Enemies)
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Enemy Got Hit")));

			if (OutHit[i].BoneName == "Head")
			{
				Enemies->Enemy_Health -= EquipedWeapon.HeadShotDMG;

				Enemies->DamageTaken(EquipedWeapon.HeadShotDMG, OutHit[i].ImpactPoint);

				BulletDMG = EquipedWeapon.HeadShotDMG;

			}
			else
			{
				Enemies->Enemy_Health -= EquipedWeapon.BodyShotDMG;

				Enemies->DamageTaken(EquipedWeapon.BodyShotDMG, OutHit[i].ImpactPoint);

				BulletDMG = EquipedWeapon.BodyShotDMG;


			}

		

			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Enemy Health: %f"), Enemies->Enemy_Health));

		}

		

	}
	
}

void AGun::Recoil(float Value)
{
	//float ActorVerticalRot = UGameplayStatics::GetPlayerController(this, 0)->RotationInput.Quaternion().Rotator().Pitch;
	float VerticalRecoil = 0.f;

	if (IsShooting == true)
	{
		VerticalRecoil = FMath::Lerp(0.f, FMath::RandRange(EquipedWeapon.MinRecoil, EquipedWeapon.MaxRecoil), Value);

	}


	UGameplayStatics::GetPlayerController(this, 0)->AddPitchInput(VerticalRecoil);

	

}

void AGun::NotRecoil()
{
	IsShooting = false;
	CurveTimeline.Reverse();

}


