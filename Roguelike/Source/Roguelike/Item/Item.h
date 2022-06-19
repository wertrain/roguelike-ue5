// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Roguelike/Item/ItemData.h"
#include "Item.generated.h"

UCLASS(BlueprintType)
class UItem : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FItemData OriginalData;
};
