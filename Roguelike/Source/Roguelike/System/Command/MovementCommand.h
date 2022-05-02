// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommandBase.h"

/**
 * 
 */
class MovementCommand : public CommandBase
{
public:
	MovementCommand(class ARoguelikePawn* Pawn, const FIntPoint& Point);
	virtual ~MovementCommand();

	virtual int32 GetCost() const override { return 1; }

	virtual void Execute() override;
	virtual bool IsFinished() override;
	virtual void Update(float DeltaTime);

private:
	class ARoguelikePawn* Target;
	FIntPoint NextPoint;
};
