// Fill out your copyright notice in the Description page of Project Settings.


#include "Roguelike/System/Command/AttackCommand.h"
#include "Roguelike/Character/RoguelikePawn.h"
#include "Roguelike/Character/CharacterAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

AttackCommand::AttackCommand(class ARoguelikePawn* Attacker)
    : CommandBase()
    , AttackerPawn(Attacker)
{
    check(AttackerPawn != nullptr);
}

AttackCommand::~AttackCommand()
{
}

void AttackCommand::Execute()
{
    if (UCharacterAnimInstance* AnimInstance = AttackerPawn->GetAnimInstance())
    {
        AnimInstance->PlayAnimMontage(ECharacterAnimMontageFlag::Attack);
    }
}

bool AttackCommand::IsFinished()
{
    if (UCharacterAnimInstance* AnimInstance = AttackerPawn->GetAnimInstance())
    {
        return !AnimInstance->IsPlayingAnimMontage(ECharacterAnimMontageFlag::Attack);
    }
    checkNoEntry();

    return false;
}
