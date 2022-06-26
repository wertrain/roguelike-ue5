// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "ItemData.generated.h"

UENUM(Blueprintable)
enum class EItemTypes : uint8
{
	Consumable UMETA(DisplayName = "Consumables"),
	Permanent UMETA(DisplayName = "Permanent"),
	Special UMETA(DisplayName = "Special"),
	Equipment UMETA(DisplayName = "Equipment"),
	Num UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemTypes ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Summary;
};
