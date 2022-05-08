// Fill out your copyright notice in the Description page of Project Settings.


#include "Roguelike/Character/CharacterAnimator.h"

// Add default functionality here for any IRoguelikeCharacterAnimator functions that are not pure virtual.

void ICharacterAnimator::ResetAnimationFlags()
{
    for (size_t Index = 0; Index < static_cast<size_t>(ECharacterAnimationFlag::Num); ++Index)
    {
        AnimationFlag[Index] = false;
    }
}

void ICharacterAnimator::SetAnimationFlag(const ECharacterAnimationFlag Flag, const bool IsOn)
{
    AnimationFlag[static_cast<size_t>(Flag)] = IsOn;
}

bool ICharacterAnimator::GetAnimationFlag(const ECharacterAnimationFlag Flag) const
{
    return AnimationFlag[static_cast<size_t>(Flag)];
}
