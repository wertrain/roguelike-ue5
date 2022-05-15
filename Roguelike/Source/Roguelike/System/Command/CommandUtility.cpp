// Fill out your copyright notice in the Description page of Project Settings.

#include "Roguelike/System/Command/CommandUtility.h"
#include "Roguelike/System/Command/CommandBase.h"
#include "Roguelike/System/Command/MovementCommand.h"
#include "Roguelike/System/Command/DamageCommand.h"
#include "Roguelike/System/Command/DieCommand.h"
#include "Roguelike/System/Command/ItemGetCommand.h"
#include "Roguelike/System/Command/AttackCommand.h"
#include "Roguelike/Character/RoguelikePawn.h"
#include "Roguelike/Character/Component/RoguelikeMovementComponent.h"
#include "Roguelike/Map/RoguelikeMap.h"
#include "Roguelike/Map/PlacedObject.h"
#include "Roguelike/Map/PlacedItem.h"

CommandUtility::CommandUtility()
{

}

void CommandUtility::CreateMovementCommand(TArray<CommandBase*>& Commands, ARoguelikePawn* Pawn, const FIntPoint& Point)
{
    Commands.Add(new MovementCommand(Pawn, Point));

    auto* MovementComponent = Pawn->GetRoguelikeMovementComponent();
    auto* RoguelikeMap = MovementComponent->GetRoguelikeMap();
    
    // 移動先のマップで発生するコマンドも発行
    if (auto* PlacedObject = RoguelikeMap->GetPlacedObject(Point.X, Point.Y))
    {
        if (PlacedObject->IsA(APlacedItem::StaticClass()))
        {
            Commands.Add(new ItemGetCommand(Pawn, static_cast<APlacedItem*>(PlacedObject)));
        }
    }
}

void CommandUtility::CreateAttackCommand(TArray<class CommandBase*>& Commands, class ARoguelikePawn* Attacker)
{
    Commands.Add(new AttackCommand(Attacker));
}

void CommandUtility::CreateDamageCommand(TArray<class CommandBase*>& Commands, class ARoguelikePawn* Attacker, class ARoguelikePawn* Defender)
{
    auto& AttackerStatus = Attacker->GetCurrentStatus();
    auto& DefenderStatus = Defender->GetCurrentStatus();

    int Damage = AttackerStatus.Strength - DefenderStatus.Defense;
    
    if (Damage <= 0)
    {
        return;
    }

    DefenderStatus.HealthPoint = DefenderStatus.HealthPoint - Damage;

    if (DefenderStatus.HealthPoint <= 0)
    {
        Commands.Add(new DieCommand(Defender));
    }
    else
    {
        Commands.Add(new DamageCommand(Attacker, Defender));
    }
}

void CommandUtility::DestroyCommand(CommandBase* Command)
{
    delete Command;
}
