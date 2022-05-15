// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoguelikeMapManager.generated.h"

USTRUCT(Blueprintable)
struct FRoguelikeMapLevelParameter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Map")
	int32 EnemyNum {0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Map")
	int32 ItemNum {0};
};

UCLASS(Blueprintable)
class ROGUELIKE_API ARoguelikeMapManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoguelikeMapManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Map")
	TSubclassOf <class ARoguelikeMap> DefaultMapClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Map")
	TSubclassOf <class ARoguelikePawn> DefaultPawnClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Map")
	TSubclassOf <class APlacedItem> DefaultPlacedItemClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	class ARoguelikeMap* CurrentMap;
};
