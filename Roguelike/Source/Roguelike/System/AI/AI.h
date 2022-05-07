// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class AI
{
public:
	static void CreateCommands(class URoguelikeGameSubsystem* RoguelikeGameSubsystem, class ARoguelikePawn* Pawn, TArray<class CommandBase*>& Commands);

private:
	static void CommandAttack(class URoguelikeGameSubsystem* RoguelikeGameSubsystem, class ARoguelikePawn* Pawn, TArray<class CommandBase*>& Commands);
	static void CommandSearchAdversary(class URoguelikeGameSubsystem* RoguelikeGameSubsystem, class ARoguelikePawn* Pawn, TArray<class CommandBase*>& Commands);

	AI() {};
};
