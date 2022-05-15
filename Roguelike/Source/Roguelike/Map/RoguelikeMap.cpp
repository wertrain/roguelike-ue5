
#include "Roguelike/Map/RoguelikeMap.h"
#include "Roguelike/Map/RoguelikeMapBlock.h"
#include "Roguelike/Map/MapDefinitions.h"
#include "Roguelike/Map/PlacedItem.h"
#include "Roguelike/Character/RoguelikePawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARoguelikeMap::ARoguelikeMap()
	: Blocks()
	, MapWidth(0)
	, MapHeight(0)
	, Map()
	, CollisionMap()
	, PawnMap()
	, PlacedMap()
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
	PlacedMap.AddZeroed(MapElementNum);
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
				-AmountWidth + (BlockWidth * x), 
				-(BlockHeight * 0.5f) + (Maps[y][x] * BlockHeight)) + GetActorLocation();

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

bool ARoguelikeMap::CanPass(const int X, const int Y) const
{
	const auto Index = PointToIndex(X, Y);
	return CollisionMap[Index] == 0 && PawnMap[Index] == nullptr;
}

bool ARoguelikeMap::CanPass(const int Index) const
{
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

bool ARoguelikeMap::RemovePawnPoint(class APawn* Pawn, const FIntPoint Point)
{
	int32 Index = PointToIndex(Point);

	if (PawnMap[Index] == Pawn)
	{
		PawnMap[Index] = nullptr;
		return true;
	}
	return false;
}

const APawn* ARoguelikeMap::GetPawn(const int X, const int Y) const
{
	if (X < 0 || MapWidth <= X || Y < 0 || MapHeight <= Y)
	{
		return nullptr;
	}
	return PawnMap[PointToIndex(X, Y)];
}

ARoguelikePawn* ARoguelikeMap::GetRoguelikePawn(int X, int Y)
{
	if (X < 0 || MapWidth <= X || Y < 0 || MapHeight <= Y)
	{
		return nullptr;
	}
	return Cast<ARoguelikePawn>(PawnMap[PointToIndex(X, Y)]);
}

APlacedObject* ARoguelikeMap::GetPlacedObject(const int X, const int Y) const
{
	if (X < 0 || MapWidth <= X || Y < 0 || MapHeight <= Y)
	{
		return nullptr;
	}
	return PlacedMap[PointToIndex(X, Y)];
}

bool ARoguelikeMap::PutItem(class APlacedItem* Item, const int X, const int Y)
{
	int Index = PointToIndex(FIntPoint(X, Y));

	if (PlacedMap[Index] != nullptr)
	{
		return false;
	}

	if (!CanPass(X, Y))
	{
		return false;
	}

	PlacedMap[Index] = Item;
	Item->SetRoguelikeMap(this);
	Item->SetActorLocation(GetLocationOnGrid(X, Y));
	return true;
}

bool ARoguelikeMap::RemoveItem(class APlacedItem* Item)
{
	int Index = PointToIndex(FIntPoint(Item->GetPoint().X, Item->GetPoint().Y));

	if (PlacedMap[Index] == Item)
	{
		PlacedMap[Index] = nullptr;
		return true;
	}

	return false;
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
