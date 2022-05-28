// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Roguelike/System/Command/CommandQueue.h"
#include "Roguelike/Character/CharacterDefinitions.h"
#include "RoguelikeGameSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_API URoguelikeGameSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	// UGameInstanceSubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// FTickableGameObject 
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override;
	virtual void Tick(float DeltaTime) override;

public:
	bool IsPlayerTurn();
	class ARoguelikePawn* GetPlayer() const;
	void ExecuteTurnCommands(const TArray<class CommandBase*> PlayerCommands);
	void GetPawns(const EFactions Faction, TArray<class ARoguelikePawn*>& OutArray);
	void AddPawn(const EFactions Faction, class ARoguelikePawn* Pawn);
	void RemovePawn(class ARoguelikePawn* Pawn);
	void RemovePawn(const EFactions Faction, class ARoguelikePawn* Pawn);
	void SetResourceManager(class AGameResourceManager* Manager);
	class AGameResourceManager* GetResourceManager() const;

private:
	union GameStates
	{
		uint32 Value;
		struct
		{
			uint32 IsPlayerTurn : 1;
			uint32 Reserved : 31;
		}
		Flags;
	} 
	States;
	uint64 TurnCount;
	TArray<class ARoguelikePawn*> RoguelikePawns[static_cast<size_t>(EFactions::Num)];
	CommandQueue Commands;
	TQueue<class ARoguelikePawn*> WaitingForActionQueue;
	class AGameResourceManager* ResourceManager;
};
