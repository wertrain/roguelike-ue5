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
#include "Roguelike/Character/Component/GridMovementComponent.h"

ARoguelikePlayerController::ARoguelikePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	IsMoving = false;
}

void ARoguelikePlayerController::SetRoguelikeMap(class ARoguelikeMap* Map)
{
	RoguelikeMap = Map;
}

void ARoguelikePlayerController::SetOnGrid(int X, int Y)
{
	check(RoguelikeMap != nullptr);

	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		MapX = X;
		MapY = Y;

		MyPawn->SetActorLocation(RoguelikeMap->GetLocationOnGrid(X, Y) + (FVector::UpVector * MapDefinitions::GridSize));
		LastLocation = MyPawn->GetActorLocation();
		NextLocation = LastLocation;
		//MyPawn->SetActorLocation(RoguelikeMap->GetLocationOnGrid(X, Y) + (FVector::UpVector * 30.0f));
	}
}

void ARoguelikePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (auto* RoguelikeCharacter = Cast<ARoguelikeCharacter>(GetCharacter()))
	{
		if (auto* GridMovementComponent = RoguelikeCharacter->GetGridMovementComponent())
		{
			if (GridMovementComponent->MoveByInput(bMoveLeft, bMoveRight, bMoveUp, bMoveDown))
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor,
					GridMovementComponent->GetNextLocation() + (FVector::UpVector * (MapDefinitions::GridSize * 0.5f)),
					FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
			}
		}
	}

#if false
	// 操作可能かどうか
	bool IsControl = false;
	// プレイヤーの位置
	const auto& CurrentLocation = GetPawn()->GetActorLocation();

	if (IsMoving)
	{
		// 移動中もグリッドの半分以上移動していたら次の入力を受け付ける
		if (FVector::Distance(LastLocation, CurrentLocation) > MapDefinitions::GridSize * 0.5f)
		{
			// 次の移動先が遠すぎる場合は移動させない
			if (FVector::Distance(NextLocation, CurrentLocation) < MapDefinitions::GridSize * 2.0f)
			{
				IsControl = true;
			}
		}
	}
	else
	{
		IsControl = true;
	}

	if (IsControl)
	{
		if (bMoveLeft || bMoveRight || bMoveUp || bMoveDown)
		{
			// 入力を最後に受け付けた位置
			LastLocation = CurrentLocation;

			if (bMoveLeft) --MapX;
			else if (bMoveRight) ++MapX;
			else if (bMoveUp) --MapY;
			else if (bMoveDown) ++MapY;

			NextLocation = RoguelikeMap->GetLocationOnGrid(MapX, MapY);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, 
				NextLocation + (FVector::UpVector * (MapDefinitions::GridSize * 0.5f)), FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
			IsMoving = true;
		}
	}

	if (IsMoving)
	{
		FollowTime += DeltaTime;

		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation);

		if (APawn* const MyPawn = GetPawn())
		{
			FVector WorldDirection = (NextLocation - MyPawn->GetActorLocation()).GetSafeNormal();
			MyPawn->AddMovementInput(WorldDirection, 1.f, false);

			auto ActorLocation = MyPawn->GetActorLocation();
			ActorLocation.Z = NextLocation.Z;
			if (NextLocation.Equals(ActorLocation, MapDefinitions::GridSize * 0.1f))
			{
				UE_LOG(LogTemp, Log, TEXT("MoveEnd"));
				IsMoving = false;
			}

			UE_LOG(LogTemp, Log, TEXT("bMoveLeft (%d, %d) : (%f, %f, %f)"), MapX, MapY, MyPawn->GetActorLocation().X, MyPawn->GetActorLocation().Y, MyPawn->GetActorLocation().Z);
		}
	}
	else
	{
		FollowTime = 0.f;
	}

#else


#endif

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
void ARoguelikePlayerController::OnReleasedUp() { bMoveUp = false; }
void ARoguelikePlayerController::OnReleasedDown() { bMoveDown = false; }
void ARoguelikePlayerController::OnReleasedLeft() { bMoveLeft = false; }
void ARoguelikePlayerController::OnReleasedRight() { bMoveRight = false; }

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
