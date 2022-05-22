// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommandBase.h"

/**
 * 
 */
class ProjectileCommand : public CommandBase
{
public:
	ProjectileCommand(class ARoguelikePawn* Attacker, TSubclassOf<class AProjectileObject> ProjectileObjectClass, FVector TargetLocation);
	virtual ~ProjectileCommand();

	virtual int32 GetCost() const override { return 1; }

	virtual void Execute() override;
	virtual bool IsFinished() override;
	virtual bool IsSync() override { return true; }

private:
	class ARoguelikePawn* AttackerPawn;
	TSubclassOf<class AProjectileObject> ProjectileObjectClass;
	class AProjectileObject* ProjectileObject;
	FVector TargetLocation;
};
