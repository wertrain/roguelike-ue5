// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Roguelike/Item/Item.h"
#include "ItemList.generated.h"

UCLASS(BlueprintType)
class UItemList : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UItem*> Items;
};
