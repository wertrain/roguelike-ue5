// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Roguelike/Character/CharacterDefinitions.h"
#include "Engine/DataTable.h"
#include "CharacterStatus.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct ROGUELIKE_API FCharacterStatus
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Character")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Character")
	int32 HealthPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Character")
	int32 MagicPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Character")
	int32 Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Character")
	int32 Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Character")
	int32 Agility;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Character")
	int32 EXP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Character")
	int32 NumberOfActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Character")
	EFactions Faction;
};

USTRUCT(BlueprintType)
struct FCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCharacterStatus Status;
};
