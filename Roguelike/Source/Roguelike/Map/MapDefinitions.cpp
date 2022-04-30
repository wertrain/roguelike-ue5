// Fill out your copyright notice in the Description page of Project Settings.


#include "MapDefinitions.h"


GridPoint::GridPoint()
{

}

GridPoint::GridPoint(const int32 InX, const int32 InY)
{
    X = InX;
    Y = InY;
}

void GridPoint::SetX(const int32 InX)
{
    X = InX;
}

void GridPoint::SetY(const int32 InY)
{
    Y = InY;
}
