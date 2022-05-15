// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommandBase.h"

/**
 * 
 */
class ItemGetCommand : public CommandBase
{
public:
	ItemGetCommand(class ARoguelikePawn* Founder, class APlacedItem* Item);
	virtual ~ItemGetCommand();

	virtual int32 GetCost() const override { return 0; }

	virtual void Execute() override;
	virtual bool IsFinished() override;
	virtual void Update(float DeltaTime);

private:
	class ARoguelikePawn* FounderPawn;
	class APlacedItem* PlacedItem;
};
