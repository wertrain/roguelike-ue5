// Fill out your copyright notice in the Description page of Project Settings.

#include "Roguelike/Item/ItemEquipments.h"
#include "Utility/EnumBitset.h"

void UItemEquipments::Equip(const EEquipmentTypes Type, UItem* TargetItem)
{
    auto* ItemData = static_cast<FEquipmentData*>(TargetItem->GetItemData());

    UItem* PrevEquipment = nullptr;

    switch (ItemData->EquipmentType)
    {
    case EEquipmentTypes::RightHand:
        PrevEquipment = RightHand;
        RightHand = TargetItem;
        break;
    case EEquipmentTypes::LeftHand:
        PrevEquipment = LeftHand;
        LeftHand = TargetItem;
        break;
    case EEquipmentTypes::Accessory:
        PrevEquipment = Accessory;
        Accessory = TargetItem;
        break;
    }

    if (PrevEquipment)
    {
        EnumBitset<EEquipmentStatus>::Off(PrevEquipment->EquipmentStatus, EEquipmentStatus::Equipped);
    }

    if (TargetItem)
    {
        EnumBitset<EEquipmentStatus>::On(TargetItem->EquipmentStatus, EEquipmentStatus::Equipped);
    }

}

void UItemEquipments::Unequip(const EEquipmentTypes Type)
{
    Equip(Type, nullptr);
}
