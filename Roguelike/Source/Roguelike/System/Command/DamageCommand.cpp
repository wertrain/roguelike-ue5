// Fill out your copyright notice in the Description page of Project Settings.


#include "Roguelike/System/Command/DamageCommand.h"
#include "Roguelike/Character/RoguelikePawn.h"
#include "Roguelike/Character/CharacterAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

DamageCommand::DamageCommand(class ARoguelikePawn* Attack, class ARoguelikePawn* Damaged)
    : CommandBase()
    , AttackPawn(Attack)
    , DamagedPawn(Damaged)
{
    check(AttackPawn != nullptr);
    check(DamagedPawn != nullptr);
}

DamageCommand::~DamageCommand()
{
}

void DamageCommand::Execute()
{
    if (UCharacterAnimInstance* AnimInstance = DamagedPawn->GetAnimInstance())
    {
        AnimInstance->PlayAnimMontage(ECharacterAnimMontageFlag::Damage);
    }
}

bool DamageCommand::IsFinished()
{
    if (UCharacterAnimInstance* AnimInstance = DamagedPawn->GetAnimInstance())
    {
        return !AnimInstance->IsPlayingAnimMontage(ECharacterAnimMontageFlag::Damage);
    }
    checkNoEntry();

    return false;
}
