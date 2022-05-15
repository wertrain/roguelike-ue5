// Fill out your copyright notice in the Description page of Project Settings.


#include "Roguelike/System/AI/AI.h"
#include "Roguelike/System/Command/CommandBase.h"
#include "Roguelike/System/Command/CommandUtility.h"
#include "Roguelike/System/Command/MovementCommand.h"
#include "Roguelike/System/Command/DamageCommand.h"
#include "Roguelike/System/RoguelikeGameSubsystem.h"
#include "Roguelike/Character/Component/RoguelikeMovementComponent.h"
#include "Roguelike/Character/CharacterStatus.h"
#include "Roguelike/Character/RoguelikePawn.h"
#include "Roguelike/Map/MapFunctions.h"
#include "Roguelike/Map/RoguelikeMap.h"

namespace
{

bool CheckNeighbor(FIntPoint Point, FIntPoint TargetPoint)
{
    if (TargetPoint.X == Point.X)
    {
        if (Point.Y + 1 == TargetPoint.Y)
        {
            return true;
        }
        else if (Point.Y - 1 == TargetPoint.Y)
        {
            return true;
        }
    }
    else if (TargetPoint.Y == Point.Y)
    {
        if (Point.X + 1 == TargetPoint.X)
        {
            return true;
        }
        else if (Point.X - 1 == TargetPoint.X)
        {
            return true;
        }
    }
    return false;
}

}

void AI::CreateCommands(URoguelikeGameSubsystem* RoguelikeGameSubsystem, ARoguelikePawn* Pawn, TArray<CommandBase*>& Commands)
{
    // �v���C���[�����삷��ꍇ�̓X�L�b�v
    if (Pawn->PlayerIndex >= 0)
    {
        return;
    }

    // ���łɓ|����Ă�����X�L�b�v
    if (Pawn->GetCurrentStatus().HealthPoint <= 0)
    {
        return;
    }

    int NumOfActions = Pawn->GetCurrentStatus().NumberOfActions;
    while (NumOfActions > 0)
    {
        FIntPoint Point = Pawn->GetRoguelikeMovementComponent()->GetPoint();
        auto* Player = RoguelikeGameSubsystem->GetPlayer();
        FIntPoint TargetPoint = Player->GetRoguelikeMovementComponent()->GetTempNextPoint();

        if (CheckNeighbor(Point, TargetPoint))
        {
            CommandUtility::CreateAttackCommand(Commands, Pawn);
            CommandUtility::CreateDamageCommand(Commands, Pawn, Player);
        }
        else
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
        *Map, Point, 
        RoguelikeGameSubsystem->GetPlayer()->GetRoguelikeMovementComponent()->GetTempNextPoint(), Points);

    //Map->SetHighlight(Points);

    if (Points.Num() > 1)
    {
        auto* Command = new MovementCommand(Pawn, Points[1]);
        Commands.Add(Command);
    }
}
