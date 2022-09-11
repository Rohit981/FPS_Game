// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "EnemyBulletsRounds.h"
#include "Animation/AnimMontage.h"
#include "EnemyGun.generated.h"

UCLASS()
class FPS_PROJECT_API AEnemyGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyGun();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* GunComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		USceneComponent* MuzzleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* MuzzleParticle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MuzzleVFX();
	void SpawnBullet();

	UPROPERTY(EditAnyWhere, Category = Gun)
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	FRotator RotationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	TSubclassOf<class AEnemyBulletsRounds> EnemyBulletClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Firing();
};
