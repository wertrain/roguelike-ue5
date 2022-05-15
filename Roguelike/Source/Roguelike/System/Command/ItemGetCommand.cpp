// Fill out your copyright notice in the Description page of Project Settings.


#include "Roguelike/System/Command/ItemGetCommand.h"
#include "Roguelike/Character/RoguelikePawn.h"
#include "Roguelike/Character/Component/RoguelikeMovementComponent.h"
#include "Roguelike/Map/RoguelikeMap.h"
#include "Roguelike/Map/PlacedItem.h"

ItemGetCommand::ItemGetCommand(class ARoguelikePawn* Founder, class APlacedItem* Item)
    : CommandBase()
    , FounderPawn(Founder)
    , PlacedItem(Item)
{
    check(Founder != nullptr);
    check(PlacedItem != nullptr);
}

ItemGetCommand::~ItemGetCommand()
{
}

void ItemGetCommand::Execute()
{
    auto* Map = FounderPawn->GetRoguelikeMovementComponent()->GetRoguelikeMap();
    Map->RemoveItem(PlacedItem);
    PlacedItem->Destroy();
}

bool ItemGetCommand::IsFinished()
{
    return true;
}

void ItemGetCommand::Update(float DeltaTime)
{

}
