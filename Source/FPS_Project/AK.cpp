// Fill out your copyright notice in the Description page of Project Settings.


#include "AK.h"

// Sets default values
AAK::AAK()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EquipedWeapon.weapon = Weapon::Primary;

	RenderAssets();

	EquipedWeapon.GunAmmo = AK_GunAmmo;

	EquipedWeapon.MinRecoil = 0.f;
	EquipedWeapon.MaxRecoil = -0.2f;
	
	
}

// Called when the game starts or when spawned
void AAK::BeginPlay()
{
	Super::BeginPlay();
	
	EquipedWeapon.MaxAmmo = AK_MaxAmmo;
	EquipedWeapon.RateOfFire = AK_RateOfFire;
	EquipedWeapon.BulletSpread = AK_BulletSpread;
}

// Called every frame
void AAK::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

