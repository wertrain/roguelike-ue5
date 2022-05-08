// Fill out your copyright notice in the Description page of Project Settings.


#include "Roguelike/Character/CharacterAnimInstance.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    if (auto* Animator = Cast<ICharacterAnimator>(TryGetPawnOwner()))
    {
        CharacterAnimator = Animator;
    }
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if (CharacterAnimator)
    {
        for (size_t Index = 0; Index < static_cast<size_t>(ECharacterAnimationFlag::Num); ++Index)
        {
            AnimationFlag[static_cast<size_t>(Index)] = CharacterAnimator->GetAnimationFlag(static_cast<ECharacterAnimationFlag>(Index));
        }
        CharacterAnimator->ResetAnimationFlags();
    }
}

void UCharacterAnimInstance::NativePostEvaluateAnimation()
{
    Super::NativePostEvaluateAnimation();
}

void UCharacterAnimInstance::NativeUninitializeAnimation()
{
    Super::NativeUninitializeAnimation();
}

void UCharacterAnimInstance::NativeBeginPlay()
{
    Super::NativeBeginPlay();
}

bool UCharacterAnimInstance::GetAnimationFlag(const ECharacterAnimationFlag Flag) const
{
    return AnimationFlag[static_cast<size_t>(Flag)];
}

bool UCharacterAnimInstance::PlayAnimMontage(const ECharacterAnimMontageFlag Flag)
{
    Montage_Play(AnimMontageAssets[static_cast<size_t>(Flag)]);
    return true;
}

bool UCharacterAnimInstance::IsPlayingAnimMontage(const ECharacterAnimMontageFlag Flag)
{
    return Montage_IsPlaying(AnimMontageAssets[static_cast<size_t>(Flag)]);
}
