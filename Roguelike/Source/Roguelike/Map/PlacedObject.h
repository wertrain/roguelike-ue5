// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlacedObject.generated.h"

UCLASS()
class ROGUELIKE_API APlacedObject : public AActor
{
	GENERATED_BODY()

public:
	APlacedObject();
	void SetRoguelikeMap(class ARoguelikeMap* RoguelikeMap);
	const FIntPoint& GetPoint();

protected:
	class ARoguelikeMap* RoguelikeMap;
	FIntPoint CurrentPoint;
};
