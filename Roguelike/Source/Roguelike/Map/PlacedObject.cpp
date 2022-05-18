// Fill out your copyright notice in the Description page of Project Settings.


#include "Roguelike/Map/PlacedObject.h"

APlacedObject::APlacedObject()
    : RoguelikeMap(nullptr)
    , CurrentPoint()
{
    PrimaryActorTick.bCanEverTick = false;
}

void APlacedObject::SetRoguelikeMap(class ARoguelikeMap* Map)
{
    RoguelikeMap = Map;
}

const FIntPoint& APlacedObject::GetPoint()
{
    return CurrentPoint;
}
