
#include "RoguelikeMap.h"
#include "RoguelikeMapBlock.h"
#include "MapDefinitions.h"

#include "Roguelike/RoguelikePlayerController.h"
#include "Roguelike/RoguelikeCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARoguelikeMap::ARoguelikeMap()
	: Blocks()
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

	Map.AddUninitialized(Height * Width);
	CollisionMap.AddUninitialized(Height * Width);
	FMemory::Memcpy(Map.GetData(), Maps, Height * Width * sizeof(int32));
	FMemory::Memcpy(CollisionMap.GetData(), Maps, Height * Width * sizeof(int32));

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

FVector ARoguelikeMap::GetLocationOnGrid(int X, int Y) const
{
	const int Index = (MapWidth * Y) + X;
	return Blocks[Index]->GetActorLocation();
}

bool ARoguelikeMap::CanPass(const int X, const int Y)
{
	const int Index = (MapWidth * Y) + X;
	return CollisionMap[Index] == 0;
}

void ARoguelikeMap::SetHighlight(TArray<FIntPoint> Points)
{
	ResetHighlight();

	for (auto& Point : Points)
	{
		const int Index = (MapWidth * Point.Y) + Point.X;
		Blocks[Index]->SetHighlight();
	}
}

void ARoguelikeMap::ResetHighlight()
{
	for (auto& Block : Blocks)
	{
		Block->ResetMaterial();
	}
}