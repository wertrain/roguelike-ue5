// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class CommandUtility
{
public:
	static void CreateMovementCommand(TArray<class CommandBase*>& Commands, class ARoguelikePawn* Pawn, const FIntPoint& Point);
	static void CreateAttackCommand(TArray<class CommandBase*>& Commands, class ARoguelikePawn* Attacker);
	static void CreateProjectileCommand(TArray<class CommandBase*>& Commands, class ARoguelikePawn* Attacker, TSubclassOf<class AProjectileObject> ProjectileObjectClass);
	static void CreateDamageCommand(TArray<class CommandBase*>& Commands, class ARoguelikePawn* Attacker, class ARoguelikePawn* Defender);
	static void DestroyCommand(class CommandBase* Command);

private:
	CommandUtility();
};
