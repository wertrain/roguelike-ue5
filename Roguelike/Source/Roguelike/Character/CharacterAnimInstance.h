// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Roguelike/Character/CharacterAnimator.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

UENUM(Blueprintable)
enum class ECharacterAnimMontageFlag : uint8
{
	Damage UMETA(DisplayName = "Damage"),
	Attack UMETA(DisplayName = "Attack"),
	AttackSlash UMETA(DisplayName = "Attack Slash"),
	FireProjectile  UMETA(DisplayName = "Fire Projectile"),
	Num UMETA(Hidden)
};

/**
 * 
 */
UCLASS()
class ROGUELIKE_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	virtual void NativePostEvaluateAnimation() override;
	virtual void NativeUninitializeAnimation() override;
	virtual void NativeBeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = "Roguelike|Character")
	class UAnimMontage* AnimMontageAssets[static_cast<size_t>(ECharacterAnimMontageFlag::Num)];

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Roguelike|Character")
	bool GetAnimationFlag(const ECharacterAnimationFlag Flag) const;

	// アニメーションモンタージュ用の通知を伝える
	// これを呼び出さないと WaitNotificationAnimMontage / IsNotifyAnimMontage が動作しないので注意
	UFUNCTION(BlueprintCallable, Category = "Roguelike|Character")
	void NotifyAnimMontage(const ECharacterAnimMontageFlag Flag);

	bool PlayAnimMontage(const ECharacterAnimMontageFlag Flag);
	bool IsPlayingAnimMontage(const ECharacterAnimMontageFlag Flag) const;

	// アニメーションモンタージュからの通知を待つ
	bool WaitNotificationAnimMontage(const ECharacterAnimMontageFlag Flag);
	// アニメーションモンタージュからの通知が来たかをチェックする
	bool IsNotifyAnimMontage(const ECharacterAnimMontageFlag Flag);

	// 次のステートを監視する
	bool WatchNextState();
	// 監視していた次のステートの終了をチェックする
	bool IsWatchStateHasEnded() const;

	void OnEntryState(const struct FAnimNode_StateMachine& Machine, int32 PrevStateIndex, int32 NextStateIndex);
	void OnExitState(const struct FAnimNode_StateMachine& Machine, int32 PrevStateIndex, int32 NextStateIndex);

private:
	enum class EWatchStateFlag
	{
		Nop,
		WaitWatch,
		Watching,
		WatchStateHasEnded
	};

	class ICharacterAnimator* CharacterAnimator;
	bool AnimationFlags[static_cast<size_t>(ECharacterAnimationFlag::Num)];
	EWatchStateFlag WatchStateFlag;
	EWatchStateFlag WatchAnimMontageFlags[static_cast<size_t>(ECharacterAnimMontageFlag::Num)];
};
