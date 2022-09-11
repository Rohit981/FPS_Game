// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyLockON.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EnemyChar.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

EnemyLockON::EnemyLockON()
{
}

EnemyLockON::~EnemyLockON()
{
}

//void EnemyLockON::LockON(UCameraComponent* FollowCamera, AActor* Player, float LookRadius, bool IsLockedON)
//{
//	FVector Start = FollowCamera->GetComponentLocation();
//	FVector End = ((FollowCamera->GetForwardVector() * 5000) + Start);
//
//	FHitResult OutHit;
//
//	TArray<AActor*> ActorsToIgnore;
//
//	ActorsToIgnore.Add(FollowCamera->GetOwner());
//	ActorsToIgnore.Add(Player);
//
//	bool IsHit = false;
//
//	IsHit = UKismetSystemLibrary::SphereTraceSingle(Player->GetWorld(), Start, End, LookRadius, ETraceTypeQuery::TraceTypeQuery4, false, ActorsToIgnore,
//		EDrawDebugTrace::None, OutHit, true, FLinearColor::Blue, FLinearColor::Green, 1.f);
//
//	
//
//	if (IsHit == true)
//	{
//		IsLockedON = true;
//		Enemies = Cast<AEnemyChar>(OutHit.GetActor());
//
//		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Ray Got Hit")));
//
//	}
//	else
//	{
//	
//		IsLockedON = false;
//
//		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Ray didn't Hit")));
//
//	}
//	
//	
//}
