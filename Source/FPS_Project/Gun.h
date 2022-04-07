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
	int LowAmmo;
	float RateOfFire;
	float BulletSpread;
	float MaxRecoil;
	float MinRecoil;
	int Gun_UI_Magazine;
	int Gun_UI_MaxMagazine;
	float ADS_PlayRate;
	int HeadShotDMG;
	int BodyShotDMG;
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

	virtual void Shoot(UCameraComponent* FollowCamera, AActor* Player, bool IsAds);
	void FireAnimAndFX(bool IsAds);
	void GunReloading(UAnimInstance* AnimInstance);
	void Reload();
	void NotRecoil();

	int CurrentAmmo;
	
	int BulletDMG = 0;
	

	FTransform FinalRecoilTransform;

	bool bReloading = false;
	bool IsGunReloading = false;

	FEquipedWeapon EquipedWeapon;

	bool IsShooting = false;

	int Gun2_UI_Magazine = 0;
	int Gun2_UI_MaxMagazine = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	UAnimMontage* ADS_FireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	UAnimMontage* ReloadMontage;

private:

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RenderAssets();
	void SpawnDecal(TArray<FHitResult> OutHit);
	void ReloadUI();
	void EnemyHit(TArray<FHitResult> OutHit);

	UFUNCTION()
	void Recoil(float Value);

	FVector Start;
	FVector End;

	int AmmoDifference = 0;

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
