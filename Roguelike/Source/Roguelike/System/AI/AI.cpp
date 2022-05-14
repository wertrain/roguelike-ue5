// Fill out your copyright notice in the Description page of Project Settings.


#include "Roguelike/System/AI/AI.h"
#include "Roguelike/System/Command/CommandBase.h"
#include "Roguelike/System/Command/MovementCommand.h"
#include "Roguelike/System/Command/DamageCommand.h"
#include "Roguelike/System/RoguelikeGameSubsystem.h"
#include "Roguelike/Character/Component/RoguelikeMovementComponent.h"
#include "Roguelike/Character/CharacterStatus.h"
#include "Roguelike/Character/RoguelikePawn.h"
#include "Roguelike/Map/MapFunctions.h"
#include "Roguelike/Map/RoguelikeMap.h"

void AI::CreateCommands(URoguelikeGameSubsystem* RoguelikeGameSubsystem, ARoguelikePawn* Pawn, TArray<CommandBase*>& Commands)
{
    // プレイヤーが操作する場合はスキップ
    if (Pawn->PlayerIndex >= 0)
    {
        return;
    }

    int NumOfActions = Pawn->OriginalStatus.NumberOfActions;
    while (NumOfActions > 0)
    {
        bool IsSearch = false;
        {
            URoguelikeMovementComponent* RoguelikeMovementComponent = Pawn->GetRoguelikeMovementComponent();
            FIntPoint Point = RoguelikeMovementComponent->GetPoint();
            ARoguelikeMap* Map = RoguelikeMovementComponent->GetRoguelikeMap();

            IsSearch = Map->GetPawn(Point.X + 1, Point.Y) == nullptr &&
                       Map->GetPawn(Point.X - 1, Point.Y) == nullptr &&
                       Map->GetPawn(Point.X, Point.Y + 1) == nullptr &&
                       Map->GetPawn(Point.X, Point.Y - 1) == nullptr;
        }

        if (IsSearch)
        {
            CommandSearchAdversary(RoguelikeGameSubsystem, Pawn, Commands);
        }
        --NumOfActions;
    }
}

void AI::CommandAttack(URoguelikeGameSubsystem* RoguelikeGameSubsystem, class ARoguelikePawn* Pawn, TArray<class CommandBase*>& Commands)
{
    URoguelikeMovementComponent* RoguelikeMovementComponent = Pawn->GetRoguelikeMovementComponent();
    FIntPoint Point = RoguelikeMovementComponent->GetPoint();
    ARoguelikeMap* Map = RoguelikeMovementComponent->GetRoguelikeMap();

    //Map->GetPawn();
}

void AI::CommandSearchAdversary(URoguelikeGameSubsystem* RoguelikeGameSubsystem, class ARoguelikePawn* Pawn, TArray<class CommandBase*>& Commands)
{
    URoguelikeMovementComponent* RoguelikeMovementComponent = Pawn->GetRoguelikeMovementComponent();
    FIntPoint Point = RoguelikeMovementComponent->GetPoint();
    ARoguelikeMap* Map = RoguelikeMovementComponent->GetRoguelikeMap();

    TArray<FIntPoint> Points;
    MapFunctions::Dijkstra(
        *Map, Point, RoguelikeGameSubsystem->GetPlayer()->GetRoguelikeMovementComponent()->GetNextPoint(), Points);

    Map->SetHighlight(Points);

    if (Points.Num() > 1)
    {
        auto* Command = new MovementCommand(Pawn, Points[1]);
        Commands.Add(Command);
    }
}
