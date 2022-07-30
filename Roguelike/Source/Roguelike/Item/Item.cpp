// Fill out your copyright notice in the Description page of Project Settings.

#include "Roguelike/Item/Item.h"

FItemData* UItem::GetItemData()
{
    if (OriginalData.ItemType == EItemTypes::Equipment)
    {
        return &OriginalEquipmentData;
    }
    return &OriginalData;
}