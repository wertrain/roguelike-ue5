// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Roguelike/Item/ItemData.h"
#include "Item.generated.h"

UENUM(Blueprintable, meta = (Bitflags))
enum class EEquipmentStatus : uint8
{
	Equipped UMETA(DisplayName = "Equipped"),
	Cursed UMETA(DisplayName = "Cursed"),
	Blessed UMETA(DisplayName = "Blessed"),
	Num UMETA(Hidden)
};

UCLASS(BlueprintType)
class UItem : public UObject
{
	GENERATED_BODY()

public:
	FItemData* GetItemData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EquipmentStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemData OriginalData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FEquipmentData OriginalEquipmentData;

	FItemData TestData;
};
