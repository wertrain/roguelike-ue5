// Fill out your copyright notice in the Description page of Project Settings.


#include "Roguelike/Map/ProjectileObject.h"
#include "Roguelike/Map/RoguelikeMap.h"

namespace
{

FVector DirectionToVector(const EDirections& Dir)
{
	// マップのブロックが正方形の前提
	FVector Vector = FVector::Zero();
	switch (Dir)
	{
	case EDirections::Up: return FVector::ForwardVector;
	case EDirections::UpRight: return FVector::ForwardVector + FVector::RightVector;
	case EDirections::Right: return  FVector::RightVector;
	case EDirections::DownRight: return FVector::BackwardVector + FVector::RightVector;
	case EDirections::Down: return FVector::BackwardVector;
	case EDirections::UpLeft: return FVector::ForwardVector + FVector::LeftVector;
	case EDirections::Left: return FVector::LeftVector;
	case EDirections::DownLeft: return FVector::BackwardVector + FVector::LeftVector;
	}

	return Vector;
}

}

// Sets default values
AProjectileObject::AProjectileObject()
	: bPenetrating(false)
	, Speed(0)
	, RoguelikeMap(nullptr)
	, StartPoint()
	, FireDirection(EDirections::Down)
	, TargetLocation(FVector::Zero())
	, VectorDirection(FVector::Zero())
{
	PrimaryActorTick.bCanEverTick = true;
}

void AProjectileObject::SetRoguelikeMap(class ARoguelikeMap* Map)
{
	RoguelikeMap = Map;
}

void AProjectileObject::Fire(const FVector Start, const FVector Target)
{
	SetActorLocation(Start);
	TargetLocation = Target;
	VectorDirection = (Target - Start).GetSafeNormal();
}

void AProjectileObject::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileObject::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto Location = GetActorLocation();
	if ((TargetLocation - Location).Length() <= 10.0)
	{
		Destroy();
	}
	else
	{
		SetActorLocation(Location + (VectorDirection * Speed * DeltaTime));
	}
}