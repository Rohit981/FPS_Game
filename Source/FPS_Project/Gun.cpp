// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/TimelineComponent.h"
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
	float BulletSpreadRange = FMath::RandRange(EquipedWeapon.BulletSpread * -1.f, EquipedWeapon.BulletSpread);

	Start = FollowCamera->GetComponentLocation() + LineTraceOffset;
	End = ((FollowCamera->GetForwardVector() * 5000) + Start ) + FVector(BulletSpreadRange, BulletSpreadRange, BulletSpreadRange);
	

	FHitResult OutHit;

	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(FollowCamera->GetOwner());
	ActorsToIgnore.Add(Player);

	bool IsHit = false;

	IsHit = UKismetSystemLibrary::LineTraceSingle(Player->GetWorld(), Start, End, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera),
													false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FLinearColor::Red, FLinearColor::Green, 0.f);

	
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

void AGun::Reload()
{

	GunComp->PlayAnimation(ReloadAnim, false);

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

void AGun::SpawnDecal(FHitResult OutHit)
{
	if (DecalSubclass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			FVector Location = OutHit.ImpactPoint;
			FRotator OldRotation = OutHit.ImpactPoint.Rotation();

			FRotator Rotation = UKismetMathLibrary::MakeRotFromX(FVector(OldRotation.Vector().X));

			World->SpawnActor<ABullet_Decal>(DecalSubclass, Location, Rotation);

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletHit_FX, Location, Rotation);

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


		MuzzleParticle->SetWorldScale3D(FVector(0.8, 0.8, 0.8));
	}

	MuzzleParticle->SetActive(true);
}



void AGun::Recoil(float Value)
{
	//float ActorVerticalRot = UGameplayStatics::GetPlayerController(this, 0)->RotationInput.Quaternion().Rotator().Pitch;
	float VerticalRecoil;

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


