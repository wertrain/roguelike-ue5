// Fill out your copyright notice in the Description page of Project Settings.


#include "Roguelike/System/Command/DieCommand.h"
#include "Roguelike/Character/RoguelikePawn.h"
#include "Roguelike/Character/CharacterAnimInstance.h"
#include "Roguelike/System/RoguelikeGameSubsystem.h"
#include "Components/SkeletalMeshComponent.h"

DieCommand::DieCommand(class ARoguelikePawn* Pawn)
    : CommandBase()
    , RoguelikePawn(Pawn)
    , CheckAnimationEnded(false)
{
    check(RoguelikePawn != nullptr);
}

DieCommand::~DieCommand()
{
}

void DieCommand::Execute()
{
    RoguelikePawn->SetAnimationFlag(ECharacterAnimationFlag::Die, true);
    if (UCharacterAnimInstance* AnimInstance = RoguelikePawn->GetAnimInstance())
    {
        CheckAnimationEnded = AnimInstance->WatchNextState();
    }
}

bool DieCommand::IsFinished()
{
    if (CheckAnimationEnded)
    {
        if (UCharacterAnimInstance* AnimInstance = RoguelikePawn->GetAnimInstance())
        {
            if (AnimInstance->IsWatchStateHasEnded())
            {
                RoguelikePawn->Destroy();

                return true;
            }
            return false;
        }
        checkNoEntry();
    }
    return true;
}
