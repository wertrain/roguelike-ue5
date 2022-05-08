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
	: MoveSpeed(20.0f)
	, RotateSpeed(4.0f)
	, Direction(EDirections::Down)
	, RoguelikeMap(nullptr)
	, CurrentPoint()
	, NextPoint()
	, CurrentLocation()
	, NextLocation()
	, States()
	, VelocityTimer(0.0f)
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
		RoguelikeMap->ResetPawnPoint(Pawn, CurrentPoint);

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

FIntPoint URoguelikeMovementComponent::GetNextPoint() const
{
	return NextPoint;
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

	if (APawn* const Pawn = GetPawnOwner())
	{
		Pawn->SetActorRotation(DirectionToRotator(Direction));
	}
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
						// 移動済みとみなして強制更新
						CurrentPoint = NextPoint;
						// 次の移動先へ
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
			FVector WorldDirection = (NextLocation - CurrentLocation).GetSafeNormal();
			Pawn->AddMovementInput(WorldDirection, 1.f, true);

			const auto ActorLocation = Pawn->GetActorLocation();
			// 移動終了チェック
			if (NextLocation.Equals(ActorLocation, MapDefinitions::GridSize * 0.1f))
			{
				CurrentPoint = NextPoint;
				CurrentLocation = NextLocation;
				States.Flags.IsMoving = false;

				const FRotator Rotator = WorldDirection.Rotation();
				Direction = RotatorToDirection(Rotator);

				FTransform Transform = Pawn->GetActorTransform();
				Transform.SetLocation(CurrentLocation);
				Transform.SetRotation(Rotator.Quaternion());
				Pawn->SetActorTransform(Transform);
				
				//Pawn->SetActorLocation(CurrentLocation);

				//TraceArrivedDelegate.ExecuteIfBound();

				VelocityTimer = 1.0f;
				UE_LOG(LogTemp, VeryVerbose, TEXT("%s - [MOVE END]"), *Pawn->GetName());
			}
			else
			{
				UE_LOG(LogTemp, VeryVerbose, TEXT("%s - [MOVING] (%d, %d) : (%f, %f, %f)"),
					*Pawn->GetName(), NextPoint.X, NextPoint.Y, NextLocation.X, NextLocation.Y, NextLocation.Z);
			}

			auto Rotation = Pawn->GetActorRotation();
			UE_LOG(LogTemp, Log, TEXT(" (%f, %f, %f)"), Rotation.Roll, Rotation.Pitch, Rotation.Yaw);
		}
	}
	else
	{
		if (VelocityTimer > 0.0f)
		{
			if ((VelocityTimer -= 8.0f * DeltaTime) < 0.0f)
			{
				VelocityTimer = 0.0f;
			}
			Velocity = FMath::Lerp(Velocity, FVector::ZeroVector, 1.0f - VelocityTimer);
		}
	}
}

EDirections URoguelikeMovementComponent::RotatorToDirection(const FRotator& Rotator) const
{
	constexpr const double UP = 0.0;
	constexpr const double UP_RIGHT = 45.0;
	constexpr const double RIGHT = 90.0;
	constexpr const double DOWN_RIGHT = 135.0;
	constexpr const double DOWN = 180.0;
	constexpr const double RANGE = 15.0;

	if (Rotator.Yaw >= (UP - RANGE) && Rotator.Yaw < (UP + RANGE))
		return EDirections::Up;
	else if (Rotator.Yaw >= (UP_RIGHT - RANGE) && Rotator.Yaw < (UP_RIGHT + RANGE))
		return EDirections::UpRight;
	else if (Rotator.Yaw >= (RIGHT - RANGE) && Rotator.Yaw < (RIGHT + RANGE))
		return EDirections::Right;
	else if (Rotator.Yaw >= (DOWN_RIGHT - RANGE) && Rotator.Yaw < (DOWN_RIGHT + RANGE))
		return EDirections::DownRight;
	else if (Rotator.Yaw >= (DOWN - RANGE) && Rotator.Yaw < (DOWN + RANGE))
		return EDirections::Down;
	else if (Rotator.Yaw <= -(UP - RANGE) && Rotator.Yaw > -(UP + RANGE))
		return EDirections::Up;
	else if (Rotator.Yaw <= -(UP_RIGHT - RANGE) && Rotator.Yaw > -(UP_RIGHT + RANGE))
		return EDirections::UpLeft;
	else if (Rotator.Yaw <= -(RIGHT - RANGE) && Rotator.Yaw > -(RIGHT + RANGE))
		return EDirections::Left;
	else if (Rotator.Yaw <= -(DOWN_RIGHT - RANGE) && Rotator.Yaw > -(DOWN_RIGHT + RANGE))
		return EDirections::DownLeft;
	else if (Rotator.Yaw <= -(DOWN - RANGE) && Rotator.Yaw > -(DOWN + RANGE))
		return EDirections::Down;
	return EDirections::Up;
}

FRotator URoguelikeMovementComponent::DirectionToRotator(const EDirections& Dir) const
{
	constexpr const double UP = 0.0;
	constexpr const double UP_RIGHT = 45.0;
	constexpr const double RIGHT = 90.0;
	constexpr const double DOWN_RIGHT = 135.0;
	constexpr const double DOWN = 180.0;

	switch (Dir)
	{
	case EDirections::Up: return FRotator(0, UP, 0);
	case EDirections::UpRight: return FRotator(0, UP_RIGHT, 0);
	case EDirections::Right: return FRotator(0, RIGHT, 0);
	case EDirections::DownRight: return FRotator(0, DOWN_RIGHT, 0);
	case EDirections::Down: return FRotator(0, DOWN, 0);
	case EDirections::UpLeft: return FRotator(0, -UP_RIGHT, 0);
	case EDirections::Left: return FRotator(0, -RIGHT, 0);
	case EDirections::DownLeft: return FRotator(0, -DOWN_RIGHT, 0);
	}
	return FRotator(0, UP, 0);
}
