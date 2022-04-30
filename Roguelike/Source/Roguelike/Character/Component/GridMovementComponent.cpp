// Fill out your copyright notice in the Description page of Project Settings.


#include "GridMovementComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Algo/Reverse.h"

#include "Roguelike/Map/RoguelikeMap.h"

namespace
{

void Dijkstra(const ARoguelikeMap& Map, const GridPoint& Start, const GridPoint& Goal, TQueue<GridPoint>& OutQueue)
{
	struct Node
	{
		int32 X;
		int32 Y;
		Node* PrevNode;
		int32 Score;
		bool Pass;
		bool Checked;
	};

	const int MapWidth = Map.GetWidth(), MapHeight = Map.GetHeight();

	TArray<Node> NodeMap;
	{
		int32 Index = 0;
		for (const auto& Value : Map.GetCollisionMap())
		{
			Node node;
			node.X = Index % MapHeight;
			node.Y = Index / MapWidth;
			node.PrevNode = nullptr;
			node.Score = 0xFFFF;
			node.Pass = Value == 0;
			node.Checked = false;
			NodeMap.Add(node);
			++Index;
		}
	}

	TArray<Node*> SearchNodeList;

	int X = Start.GetX(), Y = Start.GetY();
	auto* StartNode = &NodeMap[(MapWidth * Y) + X];
	StartNode->Score = 0;
	SearchNodeList.Add(StartNode);

	for (int SearchIndex = 0; SearchIndex < SearchNodeList.Num(); ++SearchIndex)
	{
		auto* Node = SearchNodeList[SearchIndex];
		X = Node->X;
		Y = Node->Y;

		const int32 XOffsets[] = { 0, 1, 0, -1 }, YOffsets[] = { 1, 0, -1, 0 };
		for (int Direction = 0; Direction < 4; ++Direction)
		{
			const int CheckX = X + XOffsets[Direction];
			const int CheckY = Y + YOffsets[Direction];

			if (CheckX < 0 || CheckX >= MapWidth || CheckY < 0 || CheckY >= MapHeight)
			{
				continue;
			}
			const int CheckIndex = (MapWidth * CheckY) + CheckX;
			if (!NodeMap[CheckIndex].Pass || NodeMap[CheckIndex].Checked)
			{
				continue;
			}
			const int NextScore = Node->Score + 1;
			if (NextScore < NodeMap[CheckIndex].Score)
			{
				NodeMap[CheckIndex].Score = Node->Score + 1;
				NodeMap[CheckIndex].PrevNode = Node;
			}
			SearchNodeList.Add(&NodeMap[CheckIndex]);
		}
		Node->Checked = true;
	}

	FString DebugMap;
	for (int y = 0; y < MapHeight; ++y)
	{
		for (int x = 0; x < MapWidth; ++x)
		{
			const int CheckIndex = (MapWidth * y) + x;

			DebugMap.Appendf(TEXT("%d "), NodeMap[CheckIndex].Score);
		}
		DebugMap.Append(TEXT("\n"));
	}
	UE_LOG(LogTemp, Log, TEXT("%s"), *DebugMap);

	OutQueue.Empty();
	{
		auto* GoalNode = &NodeMap[(MapWidth * Goal.GetY()) + Goal.GetX()];
		TArray<Node*> TraceNodes;

		Node* CheckNode = GoalNode;
		while (CheckNode)
		{
			TraceNodes.Add(CheckNode);
			CheckNode = CheckNode->PrevNode;
		}

		Algo::Reverse(TraceNodes);
		for (auto* Node : TraceNodes)
		{
			UE_LOG(LogTemp, Log, TEXT("%d %d"), Node->X, Node->Y);
			OutQueue.Enqueue(GridPoint(Node->X, Node->Y));
		}
	}
}

}

UGridMovementComponent::UGridMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	States.Value = 0;
}

void UGridMovementComponent::SetRoguelikeMap(class ARoguelikeMap* Map)
{
	RoguelikeMap = Map;
}

void UGridMovementComponent::SetOnGrid(int X, int Y)
{
	check(RoguelikeMap != nullptr);
	
	if (APawn* const Pawn = GetPawnOwner())
	{
		CurrentPoint = GridPoint(X, Y);

		const auto NewLocation = RoguelikeMap->GetLocationOnGrid(X, Y) + (FVector::UpVector * MapDefinitions::GridSize);
		CurrentLocation = NewLocation;
		NextLocation = NewLocation;
		Pawn->SetActorLocation(NewLocation);
	}
}

void UGridMovementComponent::MoveTo(const int X, const int Y)
{
	NextPoint = GridPoint(X, Y);
	NextLocation = RoguelikeMap->GetLocationOnGrid(X, Y);
	States.Flags.IsMoving = true;
}

void UGridMovementComponent::TraceTo(const int X, const int Y)
{
	NextPoint = GridPoint(X, Y);
	Dijkstra(*RoguelikeMap, CurrentPoint, NextPoint, TracePoints);
	States.Flags.IsTracing = true;

	// 最初の移動先を設定
	GridPoint Point;
	if (TracePoints.Dequeue(Point))
	{
		if (TracePoints.Dequeue(Point))
		{
			MoveTo(Point.GetX(), Point.GetY());
		}
	}
}

bool UGridMovementComponent::MoveByInput(const bool bLeft, const bool bRight, const bool bUp, const bool bDown)
{
	if (APawn* const Pawn = GetPawnOwner())
	{
		const auto& Location = Pawn->GetActorLocation();

		bool IsControl = false;

		if (States.Flags.IsMoving)
		{
			// 移動中もグリッドの半分以上移動していたら次の入力を受け付ける
			if (FVector::Distance(CurrentLocation, Location) > MapDefinitions::GridSize * 0.5f)
			{
				// 次の移動先が遠すぎる場合は移動させない
				if (FVector::Distance(NextLocation, Location) < MapDefinitions::GridSize * 2.0f)
				{
					IsControl = true;
				}
			}

			// 再移動が許可されてた
			if (IsControl)
			{
				// 移動済みとみなして強制更新
				CurrentPoint = NextPoint;
			}
		}
		else
		{
			IsControl = true;
		}

		if (IsControl && (bLeft || bRight || bUp || bDown))
		{
			// 入力を最後に受け付けた位置
			CurrentLocation = Location;

			int NextX = CurrentPoint.GetX(), NextY = CurrentPoint.GetY();

			if (bLeft)
			{
				--NextX;
			}
			else if (bRight)
			{
				++NextX;
			}
			else if (bUp)
			{
				--NextY;
			}
			else if (bDown)
			{
				++NextY;
			}

			if (RoguelikeMap->CanPass(NextX, NextY))
			{
				MoveTo(NextX, NextY);
				return true;
			}

		}
	}

	return false;
}

FVector UGridMovementComponent::GetNextLocation() const
{
	return NextLocation;
}

void UGridMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGridMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (States.Flags.IsTracing)
	{
		if (APawn* const Pawn = GetPawnOwner())
		{
			const auto& Location = Pawn->GetActorLocation();

			// 移動中もグリッドの半分以上移動していたら次の入力を受け付ける
			if (FVector::Distance(CurrentLocation, Location) > MapDefinitions::GridSize * 0.5f)
			{
				// 次の移動先が遠すぎる場合は移動させない
				if (FVector::Distance(NextLocation, Location) < MapDefinitions::GridSize * 2.0f)
				{
					GridPoint Point;
					if (TracePoints.Dequeue(Point))
					{
						CurrentPoint = NextPoint;
						MoveTo(Point.GetX(), Point.GetY());
					}
					else
					{
						States.Flags.IsTracing = false;
					}
				}
			}
		}
	}

	if (States.Flags.IsMoving)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), NextLocation);

		if (APawn* const Pawn = GetPawnOwner())
		{
			// 移動方向
			FVector WorldDirection = (NextLocation - Pawn->GetActorLocation()).GetSafeNormal();
			Pawn->AddMovementInput(WorldDirection, 1.f, false);

			const auto ActorLocation = Pawn->GetActorLocation();
			// 移動終了チェック
			auto CheckLocation = ActorLocation;
			CheckLocation.Z = NextLocation.Z;
			if (NextLocation.Equals(CheckLocation, MapDefinitions::GridSize * 0.1f))
			{
				CurrentPoint = NextPoint;
				CurrentLocation = ActorLocation;
				States.Flags.IsMoving = false;

				UE_LOG(LogTemp, VeryVerbose, TEXT("%s - [MOVE END]"), *Pawn->GetName());
			}
			else
			{
				UE_LOG(LogTemp, VeryVerbose, TEXT("%s - [MOVING] (%d, %d) : (%f, %f, %f)"), 
					*Pawn->GetName(), NextPoint.GetX(), NextPoint.GetY(), NextLocation.X, NextLocation.Y, NextLocation.Z);
			}
		}
	}

}

