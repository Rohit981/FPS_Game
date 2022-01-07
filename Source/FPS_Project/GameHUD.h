// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class FPS_PROJECT_API AGameHUD : public AHUD
{
	GENERATED_BODY()

public:

	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> CrossHair_HUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> AmmoAndHealth_HUD;

	// Variable to hold the widget After Creating it.
	UUserWidget* CrossHair_Ref;

	UUserWidget* AmmoAndHealth_Ref;

	// Override BeginPlay()
	virtual void BeginPlay() override;

	
};
