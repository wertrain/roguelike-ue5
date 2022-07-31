// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Roguelike/System/Command/CommandQueue.h"
#include "Roguelike/Character/CharacterStatus.h"
#include "Roguelike/Character/CharacterAnimator.h"
#include "Roguelike/Item/ItemList.h"
#include "Roguelike/Item/ItemEquipments.h"
#include "RoguelikePawn.generated.h"

UCLASS()
class ROGUELIKE_API ARoguelikePawn : public APawn, public ICharacterAnimator
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Character")
	UItemList* ItemList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Character")
	UItemEquipments* Equipments;

	/** Returns GridMovementComponent subobject */
	FORCEINLINE class URoguelikeMovementComponent* GetRoguelikeMovementComponent() const { return RoguelikeMovementComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	class UCharacterAnimInstance* GetAnimInstance() const;
	FCharacterStatus& GetCurrentStatus();

private:
	/** Support for moving characters on the grid */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class URoguelikeMovementComponent* RoguelikeMovementComponent;

	FCharacterStatus CurrentStatus;
};
