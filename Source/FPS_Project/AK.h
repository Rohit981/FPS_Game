// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.h"
#include "AK.generated.h"

UCLASS()
class FPS_PROJECT_API AAK : public AGun
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAK();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	int AK_GunAmmo = 30;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	int AK_MaxAmmo = 600;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	float AK_RateOfFire = 0.15;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	float AK_BulletSpread = 50.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
