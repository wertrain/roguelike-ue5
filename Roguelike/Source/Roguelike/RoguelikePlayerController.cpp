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
#include "Map/ProjectileObject.h"
#include "Map/MapDefinitions.h"
#include "Roguelike/Map/ProjectileObject.h"
#include "Roguelike/Character/RoguelikePawn.h"
#include "Roguelike/Character/Component/RoguelikeMovementComponent.h"
#include "Roguelike/Character/Skill/SkillFireBall.h"
#include "Roguelike/System/RoguelikeGameSubsystem.h"
#include "Roguelike/System/Command/CommandUtility.h"

ARoguelikePlayerController::ARoguelikePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ARoguelikePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
#if false
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
#else
	UGameInstance* GameInstance = GetGameInstance();
	URoguelikeGameSubsystem* RoguelikeGameSubsystem = GameInstance->GetSubsystem<URoguelikeGameSubsystem>();

	if (RoguelikeGameSubsystem->IsPlayerTurn() && (bMoveLeft || bMoveRight || bMoveUp || bMoveDown || bButtonDown || bButtonRight))
	{
		if (ARoguelikePawn* const MyPawn = Cast<ARoguelikePawn>(GetPawn()))
		{
			FIntPoint CurrentPoint = MyPawn->GetRoguelikeMovementComponent()->GetPoint();
			if (bButtonLeft)
			{
				if (bMoveLeft)
				{
					MyPawn->GetRoguelikeMovementComponent()->SetDirection(EDirections::Left);
				}
				else if (bMoveRight)
				{
					MyPawn->GetRoguelikeMovementComponent()->SetDirection(EDirections::Right);
				}
				else if (bMoveUp)
				{
					MyPawn->GetRoguelikeMovementComponent()->SetDirection(EDirections::Up);
				}
				else if (bMoveDown)
				{
					MyPawn->GetRoguelikeMovementComponent()->SetDirection(EDirections::Down);
				}
			}
			else
			{
				bool PlayerMoved = false;
				FIntPoint NextPoint;

				if (bMoveLeft)
				{
					if (MyPawn->GetRoguelikeMovementComponent()->GetRoguelikeMap()->CanPass(CurrentPoint.X - 1, CurrentPoint.Y))
					{
						PlayerMoved = true;
						NextPoint = FIntPoint(CurrentPoint.X - 1, CurrentPoint.Y);
					}
				}
				else if (bMoveRight)
				{
					if (MyPawn->GetRoguelikeMovementComponent()->GetRoguelikeMap()->CanPass(CurrentPoint.X + 1, CurrentPoint.Y))
					{
						PlayerMoved = true;
						NextPoint = FIntPoint(CurrentPoint.X + 1, CurrentPoint.Y);
					}
				}
				else if (bMoveUp)
				{
					if (MyPawn->GetRoguelikeMovementComponent()->GetRoguelikeMap()->CanPass(CurrentPoint.X, CurrentPoint.Y - 1))
					{
						PlayerMoved = true;
						NextPoint = FIntPoint(CurrentPoint.X, CurrentPoint.Y - 1);
					}
				}
				else if (bMoveDown)
				{
					if (MyPawn->GetRoguelikeMovementComponent()->GetRoguelikeMap()->CanPass(CurrentPoint.X, CurrentPoint.Y + 1))
					{
						PlayerMoved = true;
						NextPoint = FIntPoint(CurrentPoint.X, CurrentPoint.Y + 1);
					}
				}

				if (PlayerMoved)
				{
					TArray<CommandBase*> Commands;
					CommandUtility::CreateMovementCommand(Commands, MyPawn, NextPoint);
					RoguelikeGameSubsystem->ExecuteTurnCommands(Commands);
				}
				else if (bButtonDown)
				{
					TArray<CommandBase*> Commands;
					CommandUtility::CreateAttackCommand(Commands, MyPawn);

					auto* MovementComponent = MyPawn->GetRoguelikeMovementComponent();
					auto* RoguelikeMap = MovementComponent->GetRoguelikeMap();

					const auto& FrontPoint = MovementComponent->GetFrontPoint();
					if (auto* DamagedPawn = RoguelikeMap->GetRoguelikePawn(FrontPoint.X, FrontPoint.Y))
					{
						// Ž©•ª‚ÆˆÙ‚È‚é¨—Í‚Å‚ ‚ê‚Îƒ_ƒ[ƒW‚ð—^‚¦‚é
						if (DamagedPawn->GetCurrentStatus().Faction != MyPawn->GetCurrentStatus().Faction)
						{
							CommandUtility::CreateDamageCommand(Commands, MyPawn, DamagedPawn);
						}
					}
					RoguelikeGameSubsystem->ExecuteTurnCommands(Commands);
				}
				else if (bButtonRight)
				{
					auto* Skill = new SkillFireBall();

					TArray<CommandBase*> Commands;
					CommandUtility::CreateProjectileCommand(Commands, MyPawn, Skill->GetSkillClass());
					RoguelikeGameSubsystem->ExecuteTurnCommands(Commands);

					delete Skill;
				}
			}
		}
	}
#endif
}

void ARoguelikePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("CursorUp", IE_Pressed, this, &ARoguelikePlayerController::OnPressedUp);
	InputComponent->BindAction("CursorUp", IE_Released, this, &ARoguelikePlayerController::OnReleasedUp);
	InputComponent->BindAction("CursorDown", IE_Pressed, this, &ARoguelikePlayerController::OnPressedDown);
	InputComponent->BindAction("CursorDown", IE_Released, this, &ARoguelikePlayerController::OnReleasedDown);
	InputComponent->BindAction("CursorLeft", IE_Pressed, this, &ARoguelikePlayerController::OnPressedLeft);
	InputComponent->BindAction("CursorLeft", IE_Released, this, &ARoguelikePlayerController::OnReleasedLeft);
	InputComponent->BindAction("CursorRight", IE_Pressed, this, &ARoguelikePlayerController::OnPressedRight);
	InputComponent->BindAction("CursorRight", IE_Released, this, &ARoguelikePlayerController::OnReleasedRight);
	InputComponent->BindAction("ButtonLeft", IE_Pressed, this, &ARoguelikePlayerController::OnPressedButtonLeft);
	InputComponent->BindAction("ButtonLeft", IE_Released, this, &ARoguelikePlayerController::OnReleasedButtonLeft);
	InputComponent->BindAction("ButtonRight", IE_Pressed, this, &ARoguelikePlayerController::OnPressedButtonRight);
	InputComponent->BindAction("ButtonRight", IE_Released, this, &ARoguelikePlayerController::OnReleasedButtonRight);
	InputComponent->BindAction("ButtonDown", IE_Pressed, this, &ARoguelikePlayerController::OnPressedButtonDown);
	InputComponent->BindAction("ButtonDown", IE_Released, this, &ARoguelikePlayerController::OnReleasedButtonDown);

	//InputComponent->BindAction("SetDestination", IE_Pressed, this, &ARoguelikePlayerController::OnSetDestinationPressed);
	//InputComponent->BindAction("SetDestination", IE_Released, this, &ARoguelikePlayerController::OnSetDestinationReleased);

	// support touch devices
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ARoguelikePlayerController::OnTouchPressed);
	InputComponent->BindTouch(EInputEvent::IE_Released, this, &ARoguelikePlayerController::OnTouchReleased);
}

void ARoguelikePlayerController::OnPressedUp() { bMoveUp = true; }
void ARoguelikePlayerController::OnPressedDown() { bMoveDown = true; }
void ARoguelikePlayerController::OnPressedLeft() { bMoveLeft = true; }
void ARoguelikePlayerController::OnPressedRight() { bMoveRight = true; }
void ARoguelikePlayerController::OnPressedButtonUp() { bButtonUp = true; }
void ARoguelikePlayerController::OnPressedButtonLeft() { bButtonLeft = true; }
void ARoguelikePlayerController::OnPressedButtonRight() { bButtonRight = true; }
void ARoguelikePlayerController::OnPressedButtonDown() { bButtonDown = true; }
void ARoguelikePlayerController::OnReleasedUp() { bMoveUp = false; }
void ARoguelikePlayerController::OnReleasedDown() { bMoveDown = false; }
void ARoguelikePlayerController::OnReleasedLeft() { bMoveLeft = false; }
void ARoguelikePlayerController::OnReleasedRight() { bMoveRight = false; }
void ARoguelikePlayerController::OnReleasedButtonUp() { bButtonUp = false; }
void ARoguelikePlayerController::OnReleasedButtonLeft() { bButtonLeft = false; }
void ARoguelikePlayerController::OnReleasedButtonRight() { bButtonRight = false; }
void ARoguelikePlayerController::OnReleasedButtonDown() { bButtonDown = false; }

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
