// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "GameHUD.h"
#include "Animation/AnimMontage.h"
#include "Gun.h"
#include "AK.h"
#include "FPS_ProjectCharacter.generated.h"

UCLASS(config=Game)
class AFPS_ProjectCharacter : public ACharacter
{
	GENERATED_BODY()
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

public:
	AFPS_ProjectCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);


	//Sprint
	void StartSprint();
	void StopSprint();

	bool IsSprinting = false;

	//Spawning Gun
	UPROPERTY(EditDefaultsOnly, Category = Gun)
	TSubclassOf<class AGun> AKSubclass;

	UPROPERTY(EditDefaultsOnly, Category = Gun)
	TSubclassOf<class AGun> GlockSubclass;

	void SpawnGun();

	AGun* Gun;
	AGun* Primary;
	AGun* Secondary;
	

	void PrimaryGun();
	void SecondaryGun();

	bool IsSecondaryEquiped = false;
	bool IsPrimaryEquiped = false;

	UPROPERTY(BlueprintReadWrite)
	int WeaponSwitch = 0;

	//ADS
	void Adsing();

	void NotAdsing();

	FTimeline CurveTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	UCurveFloat* CurveFloat;

	UPROPERTY(BlueprintReadWrite)
	bool IsAdsing = false;

	UFUNCTION()
	void Zoom(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	FVector Mesh_ADS_Pos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	FRotator Mesh_ADS_Rot;

	//Shoot
	void IsFiring();
	void NotFiring();
	void FireShot();

	FTimerHandle Time_Handle_Manager;

	UPROPERTY(BlueprintReadWrite)
	bool Is_Shooting = false;

	

	UAnimInstance* AnimInstance;

	//Reload
	void Reloading();

	UFUNCTION(BlueprintCallable, Category = "Gun")
	void StopReloading();

	bool IsReloading = false;

	float ReloadDelay = 0;

	int subtractedAmmo = 0;

	void EmptyMagazineReload(float DeltaTime);
	

	//UI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gun)
	AGameHUD* CrossHairHUD;

	UPROPERTY(BlueprintReadWrite)
	int UI_Magazine = 0;

	UPROPERTY(BlueprintReadWrite)
	int UI_MaxMagazine = 0;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	///** Returns CameraBoom subobject **/
	//FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

