// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyChar.h"
#include "MeleeEnemyChar.generated.h"

UCLASS()
class FPS_PROJECT_API AMeleeEnemyChar : public AEnemyChar
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Enemy, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* PunchHitBox;

	

public:
	// Sets default values for this character's properties
	AMeleeEnemyChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool canMove = true;

	void Dead();

	void PlayerHitReact();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void Attacking();

	UFUNCTION(BlueprintCallable)
		void StopAttacking();

	

private:
	void HitCheck();

	bool playerHit = false;

	
	

};
