// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Roguelike/Item/EquipmentData.h"
#include "Equipment.generated.h"

UCLASS(BlueprintType)
class UEquipment : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FEquipmentData OriginalData;
};
