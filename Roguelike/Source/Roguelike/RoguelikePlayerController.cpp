// Copyright Epic Games, Inc. All Rights Reserved.

#include "RoguelikePlayerController.h"
#include "RoguelikeCharacter.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "RoguelikeCharacter.h"
#include "Engine/World.h"

#include "Map/RoguelikeMap.h"
#include "Map/MapDefinitions.h"
#include "Roguelike/Character/Component/RoguelikeMovementComponent.h"
#include "Roguelike/System/RoguelikeGameSubsystem.h"

ARoguelikePlayerController::ARoguelikePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ARoguelikePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	UGameInstance* GameInstance = GetGameInstance();
	URoguelikeGameSubsystem* RoguelikeGameSubsystem = GameInstance->GetSubsystem<URoguelikeGameSubsystem>();

	if (RoguelikeGameSubsystem->IsPlayerTurn())
	{
		if (auto* RoguelikeCharacter = Cast<ARoguelikeCharacter>(GetCharacter()))
		{
			if (auto* RoguelikeMovementComponent = RoguelikeCharacter->GetRoguelikeMovementComponent())
			{
				if (RoguelikeMovementComponent->MoveByInput(bMoveLeft, bMoveRight, bMoveUp, bMoveDown))
				{
					RoguelikeGameSubsystem->StartStepAllPawns();

					UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor,
						RoguelikeMovementComponent->GetNextLocation() - (FVector::UpVector * (MapDefinitions::GridSize * 0.5f)),
						FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
				}
			}
		}
	}

	if (bInputPressed)
	{
		FollowTime += DeltaTime;

		// Look for the touch location
		FVector HitLocation = FVector::ZeroVector;
		FHitResult Hit;
		if (bIsTouch)
		{
			GetHitResultUnderFinger(ETouchIndex::Touch1, ECC_Visibility, true, Hit);
		}
		else
		{
			GetHitResultUnderCursor(ECC_Visibility, true, Hit);
		}
		HitLocation = Hit.Location;

		// Direct the Pawn towards that location
		APawn* const MyPawn = GetPawn();
		if(MyPawn)
		{
			FVector WorldDirection = (HitLocation - MyPawn->GetActorLocation()).GetSafeNormal();
			MyPawn->AddMovementInput(WorldDirection, 1.f, false);
		}
	}
	else
	{
		FollowTime = 0.f;
	}
}

void ARoguelikePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("Up", IE_Pressed, this, &ARoguelikePlayerController::OnPressedUp);
	InputComponent->BindAction("Up", IE_Released, this, &ARoguelikePlayerController::OnReleasedUp);
	InputComponent->BindAction("Down", IE_Pressed, this, &ARoguelikePlayerController::OnPressedDown);
	InputComponent->BindAction("Down", IE_Released, this, &ARoguelikePlayerController::OnReleasedDown);
	InputComponent->BindAction("Left", IE_Pressed, this, &ARoguelikePlayerController::OnPressedLeft);
	InputComponent->BindAction("Left", IE_Released, this, &ARoguelikePlayerController::OnReleasedLeft);
	InputComponent->BindAction("Right", IE_Pressed, this, &ARoguelikePlayerController::OnPressedRight);
	InputComponent->BindAction("Right", IE_Released, this, &ARoguelikePlayerController::OnReleasedRight);
	InputComponent->BindAction("Enter", IE_Pressed, this, &ARoguelikePlayerController::OnPressedEnter);
	InputComponent->BindAction("Enter", IE_Released, this, &ARoguelikePlayerController::OnReleasedEnter);

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ARoguelikePlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ARoguelikePlayerController::OnSetDestinationReleased);

	// support touch devices
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ARoguelikePlayerController::OnTouchPressed);
	InputComponent->BindTouch(EInputEvent::IE_Released, this, &ARoguelikePlayerController::OnTouchReleased);
}

void ARoguelikePlayerController::OnPressedUp() { bMoveUp = true; }
void ARoguelikePlayerController::OnPressedDown() { bMoveDown = true; }
void ARoguelikePlayerController::OnPressedLeft() { bMoveLeft = true; }
void ARoguelikePlayerController::OnPressedRight() { bMoveRight = true; }
void ARoguelikePlayerController::OnPressedEnter() { bEnter = true; }
void ARoguelikePlayerController::OnReleasedUp() { bMoveUp = false; }
void ARoguelikePlayerController::OnReleasedDown() { bMoveDown = false; }
void ARoguelikePlayerController::OnReleasedLeft() { bMoveLeft = false; }
void ARoguelikePlayerController::OnReleasedRight() { bMoveRight = false; }
void ARoguelikePlayerController::OnReleasedEnter() { bEnter = false; }

void ARoguelikePlayerController::OnSetDestinationPressed()
{
	// We flag that the input is being pressed
	bInputPressed = true;
	// Just in case the character was moving because of a previous short press we stop it
	StopMovement();
}

void ARoguelikePlayerController::OnSetDestinationReleased()
{
	// Player is no longer pressing the input
	bInputPressed = false;

	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We look for the location in the world where the player has pressed the input
		FVector HitLocation = FVector::ZeroVector;
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, true, Hit);
		HitLocation = Hit.Location;

		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, HitLocation);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, HitLocation, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}
}

void ARoguelikePlayerController::OnTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	bIsTouch = true;
	OnSetDestinationPressed();
}

void ARoguelikePlayerController::OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	bIsTouch = false;
	OnSetDestinationReleased();
}
