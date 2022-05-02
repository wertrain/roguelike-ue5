// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementCommand.h"
#include "Roguelike/Character/RoguelikePawn.h"
#include "Roguelike/Character/Component/RoguelikeMovementComponent.h"

MovementCommand::MovementCommand(class ARoguelikePawn* Pawn, const FIntPoint& Point)
    : CommandBase()
    , Target(Pawn)
    , NextPoint(Point)
{
    check(Target != nullptr);
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
