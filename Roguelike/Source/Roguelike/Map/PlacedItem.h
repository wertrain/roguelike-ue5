// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Roguelike/Map/PlacedObject.h"
#include "PlacedItem.generated.h"

UENUM(Blueprintable)
enum class EItemType : uint8
{
	Sword UMETA(DisplayName = "Sword"),
	Shield UMETA(DisplayName = "Shield"),
	Num UMETA(Hidden)
};

/**
 * 
 */
UCLASS()
class ROGUELIKE_API APlacedItem : public APlacedObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlacedItem();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Roguelike|Item")
	void Create(const EItemType Type);
	virtual void Create_Implementation(const EItemType Type);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	EItemType ItemType;
};
