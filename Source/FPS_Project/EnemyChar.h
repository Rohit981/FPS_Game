// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "EnemyDamageUI.h"
#include "FPS_ProjectCharacter.h"
#include "EnemyChar.generated.h"

UCLASS()
class FPS_PROJECT_API AEnemyChar : public ACharacter
{
	GENERATED_BODY()

private:



public:
	// Sets default values for this character's properties
	AEnemyChar();

protected:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* HealthBar_UI;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
	void BulletHit(float DeltaTime);

	

	UAnimInstance* AnimInstance;


	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Enemy)
	int DetectionLength = 0;

	float attackWaitTime = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	int EnemyDamageTaken = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	FVector HitPosition;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = Enemy)
	AFPS_ProjectCharacter* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UAnimMontage* HitReaction_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class AEnemyDamageUI> Damage_HUD;

	bool ISDamageTaken = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	bool Is_Dead = false;

	float HitReactionTime = 0;

	bool Bullet_Hit = false;

	int hitCounter = 0;



public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	bool Is_Punching = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	float Enemy_Health = 100;

	UFUNCTION(BlueprintCallable)
	void DamageTaken(int BulletDMG, FVector BulletHitPos);

	UPROPERTY(BlueprintReadWrite)
	bool Enemy_IsLookOn = false;

	bool EnemyAgro = false;

	bool IsHit;
	

};
