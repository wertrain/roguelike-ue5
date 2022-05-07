
#include "RoguelikeMap.h"
#include "RoguelikeMapBlock.h"
#include "MapDefinitions.h"

#include "Roguelike/RoguelikePlayerController.h"
#include "Roguelike/RoguelikeCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARoguelikeMap::ARoguelikeMap()
	: Blocks()
	, MapWidth(0)
	, MapHeight(0)
	, Map()
	, CollisionMap()
	, PawnMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;
}

// Called when the game starts or when spawned
void ARoguelikeMap::BeginPlay()
{
	Super::BeginPlay();

	CreateNewMap();
}

// Called every frame
void ARoguelikeMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARoguelikeMap::CreateNewMap()
{
	const int Height = 20, Width = 20;
	int Maps[Height][Width] =
	{
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	};

	const int32 MapElementNum = Height * Width;
	Map.AddUninitialized(MapElementNum);
	CollisionMap.AddUninitialized(MapElementNum);
	PawnMap.AddZeroed(MapElementNum);
	FMemory::Memcpy(Map.GetData(), Maps, MapElementNum * sizeof(int32));
	FMemory::Memcpy(CollisionMap.GetData(), Maps, MapElementNum * sizeof(int32));

	MapWidth = Width;
	MapHeight = Height;

	const float BlockWidth = MapDefinitions::GridSize, BlockHeight = MapDefinitions::GridSize;
	const int32 AmountWidth = (Width * BlockWidth) * .5f, AmountHeight = (Height * BlockHeight) * .5f;

	for (int32 y = 0; y < Height; ++y)
	{
		for (int32 x = 0; x < Width; ++x)
		{
			const FVector BlockLocation = FVector(
				AmountHeight - (BlockHeight * y), 
				-AmountWidth + (BlockWidth * x), Maps[y][x] * BlockHeight) + GetActorLocation();

			// Spawn a block
			auto* NewBlock = GetWorld()->SpawnActor<ARoguelikeMapBlock>(BlockLocation, FRotator(0, 0, 0));
			Blocks.Add(NewBlock);
		}
	}
}

FVector ARoguelikeMap::GetLocationOnGrid(const int X, const int Y) const
{
	return Blocks[PointToIndex(X, Y)]->GetActorLocation();
}

bool ARoguelikeMap::CanPass(const int X, const int Y)
{
	const auto Index = PointToIndex(X, Y);
	return CollisionMap[Index] == 0 && PawnMap[Index] == nullptr;
}

void ARoguelikeMap::ResetPawnPoint(class APawn* Pawn, const FIntPoint Point)
{
	for (int32 Index = 0; Index < PawnMap.Num(); ++Index)
	{
		if (PawnMap[Index] == Pawn)
		{
			PawnMap[Index] = nullptr;
			break;
		}
	}
	PawnMap[PointToIndex(Point)] = Pawn;
}

void ARoguelikeMap::UpdatePawnPoint(class APawn* Pawn, const FIntPoint OldPoint, const FIntPoint NewPoint)
{
	check(PawnMap[PointToIndex(OldPoint)] == Pawn);
	PawnMap[PointToIndex(OldPoint)] = nullptr;
	PawnMap[PointToIndex(NewPoint)] = Pawn;
}

const APawn* ARoguelikeMap::GetPawn(const int X, const int Y) const
{
	if (X < 0 || MapWidth >= X || Y < 0 || MapHeight >= Y)
	{
		return nullptr;
	}
	return PawnMap[PointToIndex(X, Y)];
}

void ARoguelikeMap::SetHighlight(TArray<FIntPoint> Points)
{
	ResetHighlight();

	for (const auto& Point : Points)
	{
		Blocks[PointToIndex(Point)]->SetHighlight();
	}
}

void ARoguelikeMap::ResetHighlight()
{
	for (auto& Block : Blocks)
	{
		Block->ResetMaterial();
	}
}

uint32 ARoguelikeMap::PointToIndex(const int32 X, const int32 Y) const
{
	return (MapWidth * Y) + X;
}

uint32 ARoguelikeMap::PointToIndex(const FIntPoint Point) const
{
	return (MapWidth * Point.Y) + Point.X;
}

FIntPoint ARoguelikeMap::IndexToPoint(const int32 Index) const
{
	return FIntPoint(Index % MapWidth, Index / MapHeight);
}
