// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Enum_Weapon.generated.h"

UENUM()
enum class Weapon : uint8
{
	None     UMETA(DisplayName = "None"),
	Primary     UMETA(DisplayName = "Primary"),
	Secondary      UMETA(DisplayName = "Secondary"),
	Heavy   UMETA(DisplayName = "Heavy"),
};

