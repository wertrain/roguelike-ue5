// Fill out your copyright notice in the Description page of Project Settings.

#include "Roguelike/System/Command/MontagePlayCommand.h"
#include "Roguelike/Character/RoguelikePawn.h"
#include "Components/SkeletalMeshComponent.h"

MontagePlayCommand::MontagePlayCommand(class ARoguelikePawn* Pawn, const ECharacterAnimMontageFlag Flag)
    : CommandBase()
    , AnimPawn(Pawn)
    , MontageFlag(Flag)
{
    check(AnimPawn != nullptr);
}

MontagePlayCommand::~MontagePlayCommand()
{
}

void MontagePlayCommand::Execute()
{
    if (UCharacterAnimInstance* AnimInstance = AnimPawn->GetAnimInstance())
    {
        AnimInstance->PlayAnimMontage(MontageFlag);
        AnimInstance->WaitNotificationAnimMontage(MontageFlag);
    }
}

bool MontagePlayCommand::IsFinished()
{
    if (UCharacterAnimInstance* AnimInstance = AnimPawn->GetAnimInstance())
    {
        if (AnimInstance->IsNotifyAnimMontage(MontageFlag))
        {
            return true;
        }
        return !AnimInstance->IsPlayingAnimMontage(MontageFlag);
    }
    checkNoEntry();

    return false;
}
