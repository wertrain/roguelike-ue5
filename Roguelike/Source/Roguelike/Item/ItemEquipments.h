// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Roguelike/Item/Item.h"
#include "Roguelike/Item/ItemData.h"
#include "Roguelike/Item/ItemList.h"
#include "ItemEquipments.generated.h"

UCLASS(BlueprintType)
class UItemEquipments : public UObject
{
    GENERATED_BODY()

public:
    void Equip(const EEquipmentTypes Type, UItem* Item);
    void Unequip(const EEquipmentTypes Type);

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UItemList* SourceItemList;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UItem* RightHand;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UItem* LeftHand;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UItem* Accessory;
};
