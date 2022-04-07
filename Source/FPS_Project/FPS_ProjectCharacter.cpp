// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPS_ProjectCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Controller.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AFPS_ProjectCharacter

AFPS_ProjectCharacter::AFPS_ProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	//// Create a camera boom (pulls in towards the player if there is a collision)
	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	//CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(RootComponent, FName(TEXT("Head"))); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh1P->SetupAttachment(FollowCamera);


	Health = 100;
	

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AFPS_ProjectCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpawnGun();

	CrossHairHUD = Cast<AGameHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());


	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("Zoom"));
		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);

	}

}

void AFPS_ProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurveTimeline.TickTimeline(DeltaTime);

	AnimInstance = Mesh1P->GetAnimInstance();

	EmptyMagazineReload(DeltaTime);
	
	UI_Magazine = Gun->EquipedWeapon.Gun_UI_Magazine;
	UI_MaxMagazine = Gun->EquipedWeapon.Gun_UI_MaxMagazine;
		
	LowAmmoUI = Gun->EquipedWeapon.LowAmmo;

	EnemyDMG = Gun->BulletDMG;

	EnemyLookOn();
}

#pragma region Gun Spawner
void AFPS_ProjectCharacter::SpawnGun()
{
	if (AKSubclass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			FVector Location = GetActorLocation();
			FRotator Rotation = GetActorRotation();

			Primary = World->SpawnActor<AGun>(AKSubclass, Location, Rotation);
			Secondary = World->SpawnActor<AGun>(GlockSubclass, Location, Rotation);

			Gun = Primary;

			Secondary->SetActorHiddenInGame(true);
			Secondary->SetActorTickEnabled(false);

			IsPrimaryEquiped = true;

			WeaponSwitch = 0;

			if (Gun != nullptr)
			{
				Primary->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Palm_R"));
				Secondary->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Palm_R"));
			}
		}
	}
}

void AFPS_ProjectCharacter::PrimaryGun()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("1 Pressed")));

	Primary->SetActorHiddenInGame(false);
	Primary->SetActorTickEnabled(true);

	Secondary->SetActorHiddenInGame(true);
	Secondary->SetActorTickEnabled(false);

	WeaponSwitch = 0;


	Gun = Primary;

	IsPrimaryEquiped = true;
	IsSecondaryEquiped = false;

}

void AFPS_ProjectCharacter::SecondaryGun()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("2 Pressed")));

	Primary->SetActorHiddenInGame(true);
	Primary->SetActorTickEnabled(false);

	Secondary->SetActorHiddenInGame(false);
	Secondary->SetActorTickEnabled(true);

	WeaponSwitch = 1;


	Gun = Secondary;

	IsSecondaryEquiped = true;
	IsPrimaryEquiped = false;

}

#pragma endregion


void AFPS_ProjectCharacter::SetupStimuls()
{
	stimulSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimul"));

	stimulSource->RegisterForSense(TSubclassOf<UAISense>());
	stimulSource->RegisterWithPerceptionSystem();

}

void AFPS_ProjectCharacter::EnemyLookOn()
{
	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = ((FollowCamera->GetForwardVector() * 5000) + Start);

	FHitResult OutHit;

	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(FollowCamera->GetOwner());
	ActorsToIgnore.Add(this);

	bool IsHit = false;

	IsHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, LookRadius, ETraceTypeQuery::TraceTypeQuery4 , false, ActorsToIgnore,
		                                            EDrawDebugTrace::None, OutHit, true, FLinearColor::Blue, FLinearColor::Green, 1.f);

	if (IsHit == true)
	{
		Enemy_IsLookOn = true;

		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Ray Got Hit")));

	}
	else
	{
		Enemy_IsLookOn = false;

	}
}

//////////////////////////////////////////////////////////////////////////
// Input
void AFPS_ProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFPS_ProjectCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFPS_ProjectCharacter::StopSprint);

	PlayerInputComponent->BindAction("ADS", IE_Pressed, this, &AFPS_ProjectCharacter::Adsing);
	PlayerInputComponent->BindAction("ADS", IE_Released, this, &AFPS_ProjectCharacter::NotAdsing);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPS_ProjectCharacter::IsFiring);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFPS_ProjectCharacter::NotFiring);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFPS_ProjectCharacter::Reloading);

	PlayerInputComponent->BindAction("Primary", IE_Pressed, this, &AFPS_ProjectCharacter::PrimaryGun);
	PlayerInputComponent->BindAction("Secondary", IE_Pressed, this, &AFPS_ProjectCharacter::SecondaryGun);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPS_ProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPS_ProjectCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFPS_ProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFPS_ProjectCharacter::LookUpAtRate);

	
}

#pragma region Movement
void AFPS_ProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFPS_ProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFPS_ProjectCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward

		if (GetCharacterMovement()->bOrientRotationToMovement == true)
		{
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}



		if (!IsSprinting)
		{
			Value *= 0.3;

		}

		AddMovementInput(GetActorForwardVector(), Value);

	}
}

void AFPS_ProjectCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{

		if (GetCharacterMovement()->bOrientRotationToMovement == true)
		{
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}

		if (!IsSprinting)
		{
			Value *= 0.3;
		}

		AddMovementInput(GetActorRightVector(), Value);

	}
}

void AFPS_ProjectCharacter::StartSprint()
{
	if (GetInputAxisValue("MoveForward") > 0)
	{
		IsSprinting = true;

	}
}

void AFPS_ProjectCharacter::StopSprint()
{
	IsSprinting = false;

}
#pragma endregion

#pragma region ADS
void AFPS_ProjectCharacter::Adsing()
{
	if (Gun->IsGunReloading == false)
	{
		IsAdsing = true;

		

		CurveTimeline.PlayFromStart();

		CrossHairHUD->CrossHair_Ref->SetVisibility(ESlateVisibility::Hidden);
	}
	

}

void AFPS_ProjectCharacter::NotAdsing()
{

	IsAdsing = false;

	CurveTimeline.Reverse();

	CrossHairHUD->CrossHair_Ref->SetVisibility(ESlateVisibility::Visible);

}

void AFPS_ProjectCharacter::Zoom(float Value)
{
	FVector OldMeshPos = FVector(-7.3, 6.1, -14.3);
	FVector NewMeshPos = Mesh_ADS_Pos;

	FRotator OldMeshRot = FRotator(0, 0, 6.4);
	FRotator NewMeshRot = Mesh_ADS_Rot;

	float StartFOV = 105, EndFOV = 50;

	if (!IsAdsing)
	{
		StartFOV = 105;
		EndFOV = 50;

		OldMeshPos = FVector(-7.3, 6.1, -14.3);
		NewMeshPos = Mesh_ADS_Pos;

		OldMeshRot = FRotator(0, 0, 6.4);
		NewMeshRot = Mesh_ADS_Rot;

	}

	float ADSCamFOV = FMath::Lerp(StartFOV, EndFOV, Value);

	FVector ADSMeshPos = FMath::Lerp(OldMeshPos, NewMeshPos, Value);

	FRotator ADSMeshRot = FMath::Lerp(OldMeshRot, NewMeshRot, Value);

	Mesh1P->SetRelativeLocation(ADSMeshPos);

	Mesh1P->SetRelativeRotation(ADSMeshRot);

	FollowCamera->FieldOfView = ADSCamFOV;



}
#pragma endregion

#pragma region Firing
void AFPS_ProjectCharacter::IsFiring()
{
	if (Gun->CurrentAmmo >= 0)
	{
		if (Gun->IsGunReloading == false)
		{
			FireShot();

			GetWorldTimerManager().SetTimer(Time_Handle_Manager, this, &AFPS_ProjectCharacter::FireShot, Gun->EquipedWeapon.RateOfFire, true);

		}



	}
}

void AFPS_ProjectCharacter::NotFiring()
{

	GetWorldTimerManager().ClearTimer(Time_Handle_Manager);


}

void AFPS_ProjectCharacter::FireShot()
{
	if (IsAdsing == true)
	{
		AnimInstance->Montage_Play(Gun->ADS_FireMontage, Gun->EquipedWeapon.ADS_PlayRate, EMontagePlayReturnType::MontageLength, 0.f);


	}
	else
	{
		AnimInstance->Montage_Play(Gun->FireMontage, 0.8f, EMontagePlayReturnType::MontageLength, 0.f);

	}

	Gun->Shoot(FollowCamera, this, IsAdsing);

	if (Gun->CurrentAmmo >= 0)
	{
		Gun->CurrentAmmo -= 1;

		UI_Magazine -= 1;

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Magazine: %i"), Gun->CurrentAmmo));

	}

}


#pragma endregion

#pragma region Reload
void AFPS_ProjectCharacter::Reloading()
{
	Gun->GunReloading(AnimInstance);

}


void AFPS_ProjectCharacter::StopReloading()
{

	Gun->IsGunReloading = false;

	Gun->bReloading = true;


	UI_Magazine = Gun->EquipedWeapon.GunAmmo;

	ReloadCount = 0;

}
void AFPS_ProjectCharacter::EmptyMagazineReload(float DeltaTime)
{

	if (Gun->CurrentAmmo <= 0)
	{

		if (ReloadCount == 0)
		{
			Gun->Reload();

			AnimInstance->Montage_Play(Gun->ReloadMontage, 1.f, EMontagePlayReturnType::MontageLength, 0.f);

			Gun->IsGunReloading = true;

			ReloadCount += 1;

		}
		
		
	}

	if (Gun->IsGunReloading == true)
	{
		NotAdsing();

		NotFiring();
	}
	

}
#pragma endregion






