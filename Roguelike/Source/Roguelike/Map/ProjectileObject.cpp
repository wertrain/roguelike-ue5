// Fill out your copyright notice in the Description page of Project Settings.


#include "Roguelike/Map/ProjectileObject.h"

// Sets default values
AProjectileObject::AProjectileObject()
    : RoguelikeMap(nullptr)
    , StartPoint()
    , FireDirection(EDirections::Down)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AProjectileObject::SetRoguelikeMap(class ARoguelikeMap* Map)
{
    RoguelikeMap = Map;
}

void AProjectileObject::Fire(const FIntPoint& Point, const EDirections Direction)
{
    StartPoint = Point;
    FireDirection = Direction;
}
