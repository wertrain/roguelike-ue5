// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "ItemData.generated.h"

UENUM(Blueprintable)
enum class EItemTypes : uint8
{
	Consumable UMETA(DisplayName = "Consumables"),
	Permanent UMETA(DisplayName = "Permanent"),
	Special UMETA(DisplayName = "Special"),
	Equipment UMETA(DisplayName = "Equipment"),
	Num UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemTypes ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Summary;
};

UENUM(Blueprintable)
enum class EEquipmentTypes : uint8
{
	RightHand UMETA(DisplayName = "Right Hand"),
	LeftHand UMETA(DisplayName = "Left Hand"),
	Accessory UMETA(DisplayName = "Accessory"),
	Num UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FEquipmentData : public FItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipmentTypes EquipmentType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HealthPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MagicPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Agility;
};
