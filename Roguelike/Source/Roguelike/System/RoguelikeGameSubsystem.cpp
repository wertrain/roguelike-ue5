// Fill out your copyright notice in the Description page of Project Settings.


#include "Roguelike/System/RoguelikeGameSubsystem.h"

#include "Roguelike/System/AI/AI.h"
#include "Roguelike/System/Command/CommandBase.h"
#include "Roguelike/System/Resource/GameResourceManager.h"
#include "Roguelike/Character/RoguelikePawn.h"
#include "Roguelike/Character/CharacterStatus.h"
#include "Kismet/GameplayStatics.h"

void URoguelikeGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    States.Value = 0;
    TurnCount = 0;
}

void URoguelikeGameSubsystem::Deinitialize()
{

}

TStatId URoguelikeGameSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(URoguelikeGameSubsystem, STATGROUP_Tickables)
}

bool URoguelikeGameSubsystem::IsTickable() const
{
    return true;
}

void URoguelikeGameSubsystem::Tick(float DeltaTime)
{
#if false
    if (!States.Flags.IsPlayerTurn)
    {
        States.Flags.IsPlayerTurn = true;
        for (int i = 0; i < static_cast<size_t>(EFactions::Num); ++i)
        {
            for (auto& Pawn : RoguelikePawns[i])
            {
                if (!Pawn->IsStepFinished(TurnCount))
                {
                    States.Flags.IsPlayerTurn = false;
                    break;
                }
            }
        }
    }
#else
    if (!Commands.IsEmpty())
    {
        if (Commands.UpdateCurrentCommand(DeltaTime))
        {
            Commands.ExecuteCommand();
        }
    }
    else
    {
        States.Flags.IsPlayerTurn = true;
    }
#endif
}

bool URoguelikeGameSubsystem::IsPlayerTurn()
{
    return States.Flags.IsPlayerTurn;
}

ARoguelikePawn* URoguelikeGameSubsystem::GetPlayer() const
{
    const auto GameInstance = GetGameInstance();
    return Cast<ARoguelikePawn>(UGameplayStatics::GetPlayerPawn(GameInstance->GetWorld(), 0));
}

void URoguelikeGameSubsystem::ExecuteTurnCommands(const TArray<class CommandBase*> PlayerCommands)
{
    struct PawnParam
    {
        ARoguelikePawn* Pawn;
        PawnParam(ARoguelikePawn* Pawn)
            : Pawn(Pawn) {}
    };

    TArray<PawnParam> PawnParams;
    //PawnParams.Reserve(RoguelikePawns.Num());
    for (int index = 0; index < static_cast<size_t>(EFactions::Num); ++index)
    {
        for (auto& Pawn : RoguelikePawns[index])
        {
            // RoguelikePawns から削除されるタイミングは Pawn::EndPlay のため
            // 既に倒されていても配列内に残っている場合があるので暫定対応
            //check(Pawn->GetCurrentStatus().HealthPoint > 0);

            PawnParam Param(Pawn);
            PawnParams.Add(Param);
        }
    }

    // 素早さでソート
    PawnParams.Sort([](const PawnParam& P1, const PawnParam& P2) {
        return  P1.Pawn->OriginalStatus.Agility < P2.Pawn->OriginalStatus.Agility;
    });

    check(Commands.IsEmpty());

    // プレイヤーコマンドの追加
    for (auto* Command : PlayerCommands)
    {
        Commands.Enqueue(Command);
    }

    // 行動の開始
    for (auto& PawnParam : PawnParams)
    {
        TArray<CommandBase*> PawnCommands;
        AI::CreateCommands(this, PawnParam.Pawn, PawnCommands);

        for (auto* Command : PawnCommands)
        {
            Commands.Enqueue(Command);
        }
    }

    Commands.ExecuteCommand();

    States.Flags.IsPlayerTurn = false;
}

void URoguelikeGameSubsystem::GetPawns(const EFactions Faction, TArray<class ARoguelikePawn*>& OutArray)
{
    for (auto* Pawn : RoguelikePawns[static_cast<size_t>(Faction)])
    {
        OutArray.Add(Pawn);
    }
}

void URoguelikeGameSubsystem::AddPawn(const EFactions Faction, class ARoguelikePawn* Pawn)
{
    RoguelikePawns[static_cast<size_t>(Faction)].AddUnique(Pawn);
}

void URoguelikeGameSubsystem::RemovePawn(class ARoguelikePawn* Pawn)
{
    for (int index = 0; index < static_cast<size_t>(EFactions::Num); ++index)
    {
        if (RoguelikePawns[index].Remove(Pawn) > 0)
        {
            break;
        }
    }
}

void URoguelikeGameSubsystem::RemovePawn(const EFactions Faction, class ARoguelikePawn* Pawn)
{
    RoguelikePawns[static_cast<size_t>(Faction)].Remove(Pawn);
}

void URoguelikeGameSubsystem::SetResourceManager(AGameResourceManager* Manager)
{
    ResourceManager = Manager;
    check(ResourceManager != nullptr);
}

AGameResourceManager* URoguelikeGameSubsystem::GetResourceManager() const
{
    return ResourceManager;
}
