// Fill out your copyright notice in the Description page of Project Settings.

#include "Roguelike/Item/ItemEquipments.h"
#include "Utility/EnumBitset.h"

bool UItemEquipments::Equip(const EEquipmentTypes Type, UItem* TargetItem)
{
    // ‘•”õ’†‚Ì‚à‚Ì‚ðŠm”F
    for (UItem* Item : SourceItemList->Items)
    {

    }

    auto* ItemData = static_cast<FEquipmentData*>(TargetItem->GetItemData());

    if (ItemData->EquipmentType == EEquipmentTypes::RightHand)
    {
        if (RightHand != nullptr)
        {
            if (EnumBitset<EEquipmentStatus>::IsOn(RightHand->EquipmentStatus, EEquipmentStatus::Equipped))
            {
                EnumBitset<EEquipmentStatus>::Off(RightHand->EquipmentStatus, EEquipmentStatus::Equipped);
            }
        }
    }

    return true;
}
