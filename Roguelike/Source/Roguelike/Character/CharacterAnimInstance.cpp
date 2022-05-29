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
    for (size_t Index = 0; Index < static_cast<size_t>(ECharacterAnimMontageFlag::Num); ++Index)
    {
        WatchAnimMontageFlags[Index] = EWatchStateFlag::Nop;
    }
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if (CharacterAnimator)
    {
        for (size_t Index = 0; Index < static_cast<size_t>(ECharacterAnimationFlag::Num); ++Index)
        {
            AnimationFlags[static_cast<size_t>(Index)] = CharacterAnimator->GetAnimationFlag(static_cast<ECharacterAnimationFlag>(Index));
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
    return AnimationFlags[static_cast<size_t>(Flag)];
}

void UCharacterAnimInstance::NotifyAnimMontage(const ECharacterAnimMontageFlag Flag)
{
    // 別にチェックしなくても良いが
    if (WatchAnimMontageFlags[static_cast<size_t>(Flag)] == EWatchStateFlag::Watching)
    {
        WatchAnimMontageFlags[static_cast<size_t>(Flag)] = EWatchStateFlag::WatchStateHasEnded;
    }
}

bool UCharacterAnimInstance::PlayAnimMontage(const ECharacterAnimMontageFlag Flag)
{
    Montage_Play(AnimMontageAssets[static_cast<size_t>(Flag)]);
    return true;
}

bool UCharacterAnimInstance::IsPlayingAnimMontage(const ECharacterAnimMontageFlag Flag) const
{
    return Montage_IsPlaying(AnimMontageAssets[static_cast<size_t>(Flag)]);
}


bool UCharacterAnimInstance::WaitNotificationAnimMontage(const ECharacterAnimMontageFlag Flag)
{
    const auto CurrentFlag = WatchAnimMontageFlags[static_cast<size_t>(Flag)];

    // 指定されたフラグはウォッチ中になっている？
    if (CurrentFlag == EWatchStateFlag::WaitWatch)
    {
        if (IsPlayingAnimMontage(Flag))
        {
            return false;
        }
        // 再生していないなら（何かのミスで設定がおかしいので）ウォッチフラグをリセット
        WatchAnimMontageFlags[static_cast<size_t>(Flag)] = EWatchStateFlag::Nop;
        UE_LOG(LogTemp, Warning, TEXT("WaitNotificationAnimMontage(%d) : reset flag."), Flag);
    }
    // そうでない場合、ウォッチするからには事前に再生してほしい
    else if (!IsPlayingAnimMontage(Flag))
    {
        // WaitWatch にしてモンタージュの開始を監視する仕様としてもよいが...
        //WatchAnimMontageFlags[static_cast<size_t>(Flag)] = EWatchStateFlag::WaitWatch;
        return false;
    }

    if (WatchAnimMontageFlags[static_cast<size_t>(Flag)] == EWatchStateFlag::Nop ||
        WatchAnimMontageFlags[static_cast<size_t>(Flag)] == EWatchStateFlag::WatchStateHasEnded)
    {
        WatchAnimMontageFlags[static_cast<size_t>(Flag)] = EWatchStateFlag::Watching;
        return true;
    }

    return false;
}

bool UCharacterAnimInstance::IsNotifyAnimMontage(const ECharacterAnimMontageFlag Flag)
{
    // 再生停止しているならもう通知はこない
    if (!IsPlayingAnimMontage(Flag))
    {
        // フラグをリセットして通知が来たとして扱う（処理ロック対策）
        WatchAnimMontageFlags[static_cast<size_t>(Flag)] = EWatchStateFlag::Nop;
        UE_LOG(LogTemp, Warning, TEXT("IsNotifyAnimMontage(%d) : montage has already been played."), Flag);
        return true;
    }

    return WatchAnimMontageFlags[static_cast<size_t>(Flag)] == EWatchStateFlag::WatchStateHasEnded;
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
