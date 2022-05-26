// Fill out your copyright notice in the Description page of Project Settings.

#include "Roguelike/System/Command/CommandUtility.h"
#include "Roguelike/System/Command/CommandBase.h"
#include "Roguelike/System/Command/MovementCommand.h"
#include "Roguelike/System/Command/DamageCommand.h"
#include "Roguelike/System/Command/DieCommand.h"
#include "Roguelike/System/Command/ItemGetCommand.h"
#include "Roguelike/System/Command/AttackCommand.h"
#include "Roguelike/System/Command/ProjectileCommand.h"
#include "Roguelike/Character/RoguelikePawn.h"
#include "Roguelike/Character/Component/RoguelikeMovementComponent.h"
#include "Roguelike/Map/RoguelikeMap.h"
#include "Roguelike/Map/ProjectileObject.h"
#include "Roguelike/Map/PlacedObject.h"
#include "Roguelike/Map/PlacedItem.h"

// TODO:
// URoguelikeGameSubsystem::ExecuteTurnCommands ターンで構築されたコマンドキューを走査して
// 必要なコマンドを追加するような仕組みにしてこのクラスは必要ないようにしたい
// （そのためには、コマンドがどのクラスであるかの判定が必要）

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

void CommandUtility::CreateProjectileCommand(TArray<class CommandBase*>& Commands, class ARoguelikePawn* Attacker, TSubclassOf<class AProjectileObject> ProjectileObjectClass)
{
	auto* MovementComponent = Attacker->GetRoguelikeMovementComponent();
	auto* RoguelikeMap = MovementComponent->GetRoguelikeMap();
	auto& PawnMap = RoguelikeMap->GetPawnMap();

	int MapWidth = RoguelikeMap->GetWidth();
	int MapHeight = RoguelikeMap->GetHeight();

	TArray<class APawn*> Pawns;
	auto IsHit = [&](int32 X, int32 Y) -> bool
	{
		int32 Index = (MapWidth * Y) + X;

		if (!RoguelikeMap->CanPass(Index))
		{
			return true;
		}

		if (PawnMap[Index] != nullptr)
		{
			Pawns.Add(PawnMap[Index]);

#if 0
			if (!ProjectileObject->bPenetrating)
			{
				return true;
			}
#else
			return true;
#endif
		}
		return false;
	};

	const FIntPoint StartPoint = MovementComponent->GetFrontPoint();
	FIntPoint TargetPoint = StartPoint;
	switch (MovementComponent->Direction)
	{
	case EDirections::Up:
		for (int32 Y = StartPoint.Y; Y >= 0; --Y)
		{
			if (IsHit(StartPoint.X, Y))
			{
				TargetPoint = FIntPoint(StartPoint.X, Y);
				break;
			}
		}
		break;

	case EDirections::Down:
		for (int32 Y = StartPoint.Y; Y < MapHeight; ++Y)
		{
			if (IsHit(StartPoint.X, Y))
			{
				TargetPoint = FIntPoint(StartPoint.X, Y);
				break;
			}
		}
		break;

	case EDirections::Right:
		for (int32 X = StartPoint.X; X < MapWidth; ++X)
		{
			if (IsHit(X, StartPoint.Y))
			{
				TargetPoint = FIntPoint(X, StartPoint.Y);
				break;
			}
		}
		break;

	case EDirections::Left:
		for (int32 X = StartPoint.X; X >= 0; --X)
		{
			if (IsHit(X, StartPoint.Y))
			{
				TargetPoint = FIntPoint(X, StartPoint.Y);
				break;
			}
		}
		break;
	}

	Commands.Add(new ProjectileCommand(Attacker, ProjectileObjectClass, RoguelikeMap->GetLocationOnGrid(TargetPoint.X, TargetPoint.Y)));
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
