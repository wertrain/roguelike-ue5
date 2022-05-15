// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommandBase.h"

/**
 * 
 */
class DamageCommand : public CommandBase
{
public:
	DamageCommand(class ARoguelikePawn* Attack, class ARoguelikePawn* Damaged);
	virtual ~DamageCommand();

	virtual int32 GetCost() const override { return 0; }

	virtual void Execute() override;
	virtual bool IsFinished() override;
	virtual bool IsSync() override { return true; }

private:
	class ARoguelikePawn* AttackPawn;
	class ARoguelikePawn* DamagedPawn;
};
