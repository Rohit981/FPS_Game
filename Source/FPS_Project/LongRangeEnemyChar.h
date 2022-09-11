// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyGun.h"
#include "EnemyChar.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "LongRangeEnemyChar.generated.h"

UCLASS()
class FPS_PROJECT_API ALongRangeEnemyChar : public AEnemyChar
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALongRangeEnemyChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Gun)
	TSubclassOf<class AEnemyGun> GunSubclass;

	UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	float countingTimerDown;

	UPROPERTY(VisibleAnywhere, Category = Character)
	float StopstrafeTimer = 0;


	FTimerHandle Time_Handle_Manager;

	void FireGun();

	void StartFiringTimer(float DeltaTime);

	void GunSpawner();

	void Dead();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Strafe();

	UPROPERTY(BlueprintReadWrite)
	bool IsAttacked = false;

	UPROPERTY(BlueprintReadWrite)
	bool CanFire = false;

	UPROPERTY(BlueprintReadWrite)
	bool CanStrafe = false;

private:
	AEnemyGun* Gun;


};
