// Fill out your copyright notice in the Description page of Project Settings.


#include "Roguelike/Character/CharacterAnimInstance.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    if (auto* Animator = Cast<ICharacterAnimator>(TryGetPawnOwner()))
    {
        CharacterAnimator = Animator;
    }

    if (const FBakedAnimationStateMachine* AnimationStateMachine = GetStateMachineInstanceDesc(TEXT("New State Machine")))
    {
        for (auto& State : AnimationStateMachine->States)
        {
            // 開始/離脱イベントをバインド
            AddNativeStateEntryBinding(AnimationStateMachine->MachineName, State.StateName, FOnGraphStateChanged::CreateUObject(this, &UCharacterAnimInstance::OnEntryState));
            AddNativeStateExitBinding(AnimationStateMachine->MachineName, State.StateName, FOnGraphStateChanged::CreateUObject(this, &UCharacterAnimInstance::OnExitState));
        }
    }

    WatchStateFlag = EWatchStateFlag::Nop;
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

bool UCharacterAnimInstance::WatchNextState()
{
    // 次のステートの終了を判定する簡易的な仕組みを開始
    if (WatchStateFlag == EWatchStateFlag::Nop ||
        WatchStateFlag == EWatchStateFlag::WatchStateHasEnded)
    {
        WatchStateFlag = EWatchStateFlag::WaitWatch;

        return true;
    }
    return false;
}

bool UCharacterAnimInstance::IsWatchStateHasEnded() const
{
    return WatchStateFlag == EWatchStateFlag::WatchStateHasEnded;
}

void UCharacterAnimInstance::OnEntryState(const struct FAnimNode_StateMachine& Machine, int32 PrevStateIndex, int32 NextStateIndex)
{
    if (WatchStateFlag == EWatchStateFlag::WaitWatch)
    {
        WatchStateFlag = EWatchStateFlag::Watching;
    }
}

void UCharacterAnimInstance::OnExitState(const struct FAnimNode_StateMachine& Machine, int32 PrevStateIndex, int32 NextStateIndex)
{
    if (WatchStateFlag == EWatchStateFlag::Watching)
    {
        WatchStateFlag = EWatchStateFlag::WatchStateHasEnded;
    }
}
