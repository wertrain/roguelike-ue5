// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//namespace Character
//{

UENUM(Blueprintable)
enum class EFactions : uint8
{
    Players UMETA(DisplayName = "Players"),
    Enemys UMETA(DisplayName = "Enemys"),
    Num UMETA(Hidden)
};

UENUM(Blueprintable)
enum class EDirections : uint8
{
    Up UMETA(DisplayName = "Up"),
    UpRight UMETA(DisplayName = "UpRight"),
    Right UMETA(DisplayName = "Right"),
    DownRight UMETA(DisplayName = "DownRight"),
    Down UMETA(DisplayName = "Down"),
    DownLeft UMETA(DisplayName = "DownLeft"),
    Left UMETA(DisplayName = "Left"),
    UpLeft UMETA(DisplayName = "UpLeft"),
    Num UMETA(Hidden)
};

//} // namespace Character
