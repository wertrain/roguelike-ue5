// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Roguelike/System/Command/CommandQueue.h"
#include "Roguelike/Character/CharacterStatus.h"
#include "RoguelikePawn.generated.h"

UCLASS()
class ROGUELIKE_API ARoguelikePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARoguelikePawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Character")
	int32 PlayerIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Character")
	FIntPoint InitialPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Character")
	FCharacterStatus OriginalStatus;

	/** Returns GridMovementComponent subobject */
	FORCEINLINE class URoguelikeMovementComponent* GetRoguelikeMovementComponent() const { return RoguelikeMovementComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	void Step(uint64 TurnCount);
	bool IsStepFinished(uint64 TurnCount);
	void XTurnAction(uint64 TurnCount);

private:
	/** Support for moving characters on the grid */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class URoguelikeMovementComponent* RoguelikeMovementComponent;

private:
	enum class EBehaviorID
	{
		Nop,
		SearchPlayer,
		TracePlayer
	};

	EBehaviorID BehaviorID;
	CommandQueue RoguelikeCommands;
	TQueue<FIntPoint> TracePoints;
};
