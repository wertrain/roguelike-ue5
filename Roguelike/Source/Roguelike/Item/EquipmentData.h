// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "EquipmentData.generated.h"

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
