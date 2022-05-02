// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Algo/Reverse.h"

#include "Roguelike/Map/RoguelikeMap.h"

namespace
{

void Dijkstra(const ARoguelikeMap& Map, const FIntPoint& Start, const FIntPoint& Goal, TQueue<FIntPoint>& OutQueue)
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

	int X = Start.X, Y = Start.Y;
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

#if false
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
#endif

	OutQueue.Empty();
	{
		auto* GoalNode = &NodeMap[(MapWidth * Goal.Y) + Goal.X];
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
			OutQueue.Enqueue(FIntPoint(Node->X, Node->Y));
		}
	}
}

}

URoguelikeMovementComponent::URoguelikeMovementComponent()
	: MoveSpeed(10.0f)
	, RotateSpeed(5.0f)
	, RoguelikeMap(nullptr)
	, CurrentPoint()
	, NextPoint()
	, CurrentLocation()
	, NextLocation()
	, States()
	, TracePoints()
{
	PrimaryComponentTick.bCanEverTick = true;
	States.Value = 0;
}

void URoguelikeMovementComponent::SetRoguelikeMap(class ARoguelikeMap* Map)
{
	RoguelikeMap = Map;
}

void URoguelikeMovementComponent::SetOnGrid(int32 X, int32 Y)
{
	SetPoint(FIntPoint(X, Y));
}

void URoguelikeMovementComponent::SetPoint(const FIntPoint Point)
{
	check(RoguelikeMap != nullptr);

	if (APawn* const Pawn = GetPawnOwner())
	{
		CurrentPoint = Point;

		auto NewLocation = RoguelikeMap->GetLocationOnGrid(Point.X, Point.Y);
		NewLocation.Z = GetActorLocation().Z;
		CurrentLocation = NewLocation;
		NextLocation = NewLocation;
		Pawn->SetActorLocation(NewLocation);
	}
}

FIntPoint URoguelikeMovementComponent::GetPoint() const
{
	return CurrentPoint;
}

void URoguelikeMovementComponent::MoveTo(const int32 X, const int32 Y)
{
	NextPoint = FIntPoint(X, Y);
	NextLocation = RoguelikeMap->GetLocationOnGrid(X, Y);
	NextLocation.Z = GetActorLocation().Z;
	States.Flags.IsMoving = true;
}

void URoguelikeMovementComponent::MoveToPoint(const FIntPoint Point)
{
	MoveTo(Point.X, Point.Y);
}

void URoguelikeMovementComponent::TraceTo(const int32 X, const int32 Y)
{
	NextPoint = FIntPoint(X, Y);
	Dijkstra(*RoguelikeMap, CurrentPoint, NextPoint, TracePoints);
	States.Flags.IsTracing = true;

	// 最初の移動先を設定
	FIntPoint Point;
	if (TracePoints.Dequeue(Point))
	{
		if (TracePoints.Dequeue(Point))
		{
			MoveTo(Point.X, Point.Y);
		}
	}
}

void URoguelikeMovementComponent::TraceToPoint(const FIntPoint Point)
{
	TraceTo(Point.X, Point.Y);
}

void URoguelikeMovementComponent::TraceToPointEx(const FIntPoint Point, const FOnArrivedDelegate& Callback)
{
	TraceToPoint(Point);
	TraceArrivedDelegate = Callback;
}

bool URoguelikeMovementComponent::IsMoving() const
{
	return States.Flags.IsMoving;
}

bool URoguelikeMovementComponent::MoveByInput(const bool bLeft, const bool bRight, const bool bUp, const bool bDown)
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

			int NextX = CurrentPoint.X, NextY = CurrentPoint.Y;

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

FVector URoguelikeMovementComponent::GetNextLocation() const
{
	return NextLocation;
}

ARoguelikeMap* URoguelikeMovementComponent::GetRoguelikeMap() const
{
	return RoguelikeMap;
}

void URoguelikeMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if ((RoguelikeMap = Cast<ARoguelikeMap>(UGameplayStatics::GetActorOfClass(GetWorld(), ARoguelikeMap::StaticClass()))) == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s - RoguelikeMap is not found."), *GetOwner()->GetName());
	}
}

void URoguelikeMovementComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	if ((RoguelikeMap = Cast<ARoguelikeMap>(UGameplayStatics::GetActorOfClass(GetWorld(), ARoguelikeMap::StaticClass()))) == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s - RoguelikeMap is not found."), *GetOwner()->GetName());
	}
	//check(RoguelikeMap != nullptr);
}

void URoguelikeMovementComponent::AddInputVector(FVector WorldVector, bool bForce)
{
	if (APawn* const Pawn = GetPawnOwner())
	{
		if (bForce || Pawn->IsMoveInputIgnored())
		{
			auto Location = Pawn->GetActorLocation();
			Velocity = (WorldVector * MoveSpeed);
			Location = Location + Velocity;
			Pawn->SetActorLocation(Location);
			FRotator MovementRotation = WorldVector.Rotation();
			Pawn->SetActorRotation(FMath::RInterpTo(Pawn->GetActorRotation(), MovementRotation, GetWorld()->GetDeltaSeconds(), RotateSpeed));
		}
	}
}

void URoguelikeMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	SetOnGrid(5, 5);
}

void URoguelikeMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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
					FIntPoint Point;
					if (TracePoints.Dequeue(Point))
					{
						CurrentPoint = NextPoint;
						MoveTo(Point.X, Point.Y);
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
		if (APawn* const Pawn = GetPawnOwner())
		{
			// 移動方向
			FVector WorldDirection = (NextLocation - Pawn->GetActorLocation()).GetSafeNormal();
			Pawn->AddMovementInput(WorldDirection, 1.f, true);

			const auto ActorLocation = Pawn->GetActorLocation();
			// 移動終了チェック
			if (NextLocation.Equals(ActorLocation, MapDefinitions::GridSize * 0.1f))
			{
				CurrentPoint = NextPoint;
				CurrentLocation = ActorLocation;
				States.Flags.IsMoving = false;

				//TraceArrivedDelegate.ExecuteIfBound();
				UE_LOG(LogTemp, VeryVerbose, TEXT("%s - [MOVE END]"), *Pawn->GetName());
			}
			else
			{
				UE_LOG(LogTemp, VeryVerbose, TEXT("%s - [MOVING] (%d, %d) : (%f, %f, %f)"),
					*Pawn->GetName(), NextPoint.X, NextPoint.Y, NextLocation.X, NextLocation.Y, NextLocation.Z);
			}
		}
	}
}
