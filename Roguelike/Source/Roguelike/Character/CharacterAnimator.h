// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterAnimator.generated.h"

UENUM(Blueprintable)
enum class ECharacterAnimationFlag : uint8
{
	AttackPunch UMETA(DisplayName = "Attack Punch"),
	Damage UMETA(DisplayName = "Damage"),
	Die UMETA(DisplayName = "Die"),
    Num UMETA(Hidden)
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterAnimator : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROGUELIKE_API ICharacterAnimator
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	void ResetAnimationFlags();
	void SetAnimationFlag(const ECharacterAnimationFlag Flag, const bool IsOn);
	bool GetAnimationFlag(const ECharacterAnimationFlag Flag) const;

private:
	bool AnimationFlag[static_cast<size_t>(ECharacterAnimationFlag::Num)];
};
