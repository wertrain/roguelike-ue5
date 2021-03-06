// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "RoguelikePlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class ARoguelikePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARoguelikePlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;
	uint32 bMoveUp : 1;
	uint32 bMoveDown : 1;
	uint32 bMoveLeft : 1;
	uint32 bMoveRight : 1;
	uint32 bButtonUp : 1;
	uint32 bButtonDown : 1;
	uint32 bButtonLeft : 1;
	uint32 bButtonRight : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Input handlers for SetDestination action. */
	void OnPressedUp();
	void OnPressedDown();
	void OnPressedLeft();
	void OnPressedRight();
	void OnPressedButtonUp();
	void OnPressedButtonLeft();
	void OnPressedButtonRight();
	void OnPressedButtonDown();
	void OnReleasedUp();
	void OnReleasedDown();
	void OnReleasedLeft();
	void OnReleasedRight();
	void OnReleasedButtonUp();
	void OnReleasedButtonLeft();
	void OnReleasedButtonRight();
	void OnReleasedButtonDown();

	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
	void OnTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location);
	void OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location);

private:
	bool bInputPressed; // Input is bring pressed
	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
};


