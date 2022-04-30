// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Roguelike/Map/MapDefinitions.h"
#include "GridMovementComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKE_API UGridMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:	
	typedef union
	{
		uint32 Value;
		struct 
		{
			uint32 IsMoving : 1;
			uint32 IsTracing : 1;
			uint32 Reserved : 30;
		}
		Flags;
	}
	MoveStates;

public:
	UGridMovementComponent();
	void SetRoguelikeMap(class ARoguelikeMap* RoguelikeMap);
	void SetOnGrid(int X, int Y);
	void MoveTo(int X, int Y);
	void TraceTo(int X, int Y);
	bool MoveByInput(bool bLeft, bool bRight, bool bUp, bool bDown);
	FVector GetNextLocation() const;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	class ARoguelikeMap* RoguelikeMap;
	GridPoint CurrentPoint;
	GridPoint NextPoint;
	FVector CurrentLocation;
	FVector NextLocation;
	MoveStates States;
	TQueue<GridPoint> TracePoints;
};
