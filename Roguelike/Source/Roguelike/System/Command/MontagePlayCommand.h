// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommandBase.h"
#include "Roguelike/Character/CharacterAnimInstance.h"

/**
 * 
 */
class MontagePlayCommand : public CommandBase
{
public:
	MontagePlayCommand(class ARoguelikePawn* Pawn, const ECharacterAnimMontageFlag Flag);
	virtual ~MontagePlayCommand();

	virtual int32 GetCost() const override { return 1; }

	virtual void Execute() override;
	virtual bool IsFinished() override;
	virtual bool IsSync() override { return true; }

private:
	class ARoguelikePawn* AnimPawn;
	ECharacterAnimMontageFlag MontageFlag;
};
