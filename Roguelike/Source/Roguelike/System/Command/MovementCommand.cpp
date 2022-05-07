// Fill out your copyright notice in the Description page of Project Settings.


#include "Roguelike/System/Command/MovementCommand.h"
#include "Roguelike/Character/RoguelikePawn.h"
#include "Roguelike/Character/Component/RoguelikeMovementComponent.h"
#include "Roguelike/Map/RoguelikeMap.h"

MovementCommand::MovementCommand(class ARoguelikePawn* Pawn, const FIntPoint& Point)
    : CommandBase()
    , Target(Pawn)
    , NextPoint(Point)
{
    check(Target != nullptr);

    if (auto* RoguelikeMovementComponent = Target->GetRoguelikeMovementComponent())
    {
        RoguelikeMovementComponent->GetRoguelikeMap()->UpdatePawnPoint(
            Pawn, RoguelikeMovementComponent->GetPoint(), NextPoint);
    }
}

MovementCommand::~MovementCommand()
{
}

void MovementCommand::Execute()
{
    if (auto* RoguelikeMovementComponent = Target->GetRoguelikeMovementComponent())
    {
        RoguelikeMovementComponent->MoveToPoint(NextPoint);
    }
}

bool MovementCommand::IsFinished()
{
    if (auto* RoguelikeMovementComponent = Target->GetRoguelikeMovementComponent())
    {
        return !RoguelikeMovementComponent->IsMoving();
    }
    return true;
}

void MovementCommand::Update(float DeltaTime)
{

}
