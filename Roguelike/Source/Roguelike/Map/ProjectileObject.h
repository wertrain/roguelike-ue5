// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Roguelike/Character/CharacterDefinitions.h"
#include "GameFramework/Actor.h"
#include "ProjectileObject.generated.h"

UCLASS()
class ROGUELIKE_API AProjectileObject : public AActor
{
	GENERATED_BODY()	

public:
	AProjectileObject();
	void SetRoguelikeMap(class ARoguelikeMap* RoguelikeMap);
	void Fire(const FIntPoint& Point, const EDirections Direction);

protected:
	class ARoguelikeMap* RoguelikeMap;
	FIntPoint StartPoint;
	EDirections FireDirection;
};
