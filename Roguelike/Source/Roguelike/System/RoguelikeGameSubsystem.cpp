// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeGameSubsystem.h"

#include "Roguelike/Character/RoguelikePawn.h"

void URoguelikeGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    States.Value = 0;
    TurnCount = 0;
}

void URoguelikeGameSubsystem::Deinitialize()
{

}

TStatId URoguelikeGameSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(URoguelikeGameSubsystem, STATGROUP_Tickables)
}

bool URoguelikeGameSubsystem::IsTickable() const
{
    return true;
}

void URoguelikeGameSubsystem::Tick(float DeltaTime)
{
    if (!States.Flags.IsPlayerTurn)
    {
        States.Flags.IsPlayerTurn = true;
        for (auto& Pawn : RoguelikePawns)
        {
            if (!Pawn->IsStepFinished(TurnCount))
            {
                States.Flags.IsPlayerTurn = false;
                break;
            }
        }
    }
}

bool URoguelikeGameSubsystem::IsPlayerTurn()
{
    return States.Flags.IsPlayerTurn;
}

void URoguelikeGameSubsystem::StartStepAllPawns()
{
    States.Flags.IsPlayerTurn = false;

    for (auto& Pawn : RoguelikePawns)
    {
        Pawn->Step(TurnCount);
    }
}

void URoguelikeGameSubsystem::AddPawn(class ARoguelikePawn* Pawn)
{
    RoguelikePawns.AddUnique(Pawn);
}

void URoguelikeGameSubsystem::RemovePawn(class ARoguelikePawn* Pawn)
{
    RoguelikePawns.Remove(Pawn);
}
