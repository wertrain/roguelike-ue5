// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommandBase.h"

/**
 * 
 */
class AttackCommand : public CommandBase
{
public:
	AttackCommand(class ARoguelikePawn* Attacker);
	virtual ~AttackCommand();

	virtual int32 GetCost() const override { return 1; }

	virtual void Execute() override;
	virtual bool IsFinished() override;
	virtual bool IsSync() override { return true; }

private:
	class ARoguelikePawn* AttackerPawn;
};
