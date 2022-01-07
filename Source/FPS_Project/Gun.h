// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/SceneComponent.h"
#include "Enum_Weapon.h"
#include "Bullet_Decal.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

USTRUCT(BlueprintType)
struct FEquipedWeapon
{
	GENERATED_USTRUCT_BODY();

	Weapon weapon;
	int GunAmmo;
	int MaxAmmo;
	float RateOfFire;
	float BulletSpread;
	float MaxRecoil;
	float MinRecoil;
};

UCLASS()
class FPS_PROJECT_API AGun : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* GunComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USceneComponent* MuzzleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* MuzzleParticle;

public:	
	// Sets default values for this actor's properties
	AGun();

	void Shoot(UCameraComponent* FollowCamera, AActor* Player, bool IsAds);
	void Reload();
	void FireAnimAndFX(bool IsAds);

	

	void NotRecoil();
	

	int CurrentAmmo;

	FTransform FinalRecoilTransform;

	bool IsReloading = false;

	FEquipedWeapon EquipedWeapon;

	bool IsShooting = false;

private:

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RenderAssets();
	void SpawnDecal(FHitResult OutHit);

	UFUNCTION()
	void Recoil(float Value);

	FVector Start;
	FVector End;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	UAnimSequence* FireAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	UAnimSequence* ReloadAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	FVector LineTraceOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	TSubclassOf<class ABullet_Decal> DecalSubclass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	UParticleSystem* BulletHit_FX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	UParticleSystem* Muzzle_Particle;

	FTimeline CurveTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	UCurveFloat* CurveFloat;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
