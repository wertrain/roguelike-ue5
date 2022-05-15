// Fill out your copyright notice in the Description page of Project Settings.


#include "Roguelike/Map/RoguelikeMapManager.h"
#include "Roguelike/Map/RoguelikeMap.h"
#include "Roguelike/Map/PlacedItem.h"
#include "Roguelike/Character/RoguelikePawn.h"
#include "Roguelike/Character/Component/RoguelikeMovementComponent.h"
#include "Math/RandomStream.h"
#include "Kismet/GameplayStatics.h"

ARoguelikeMapManager::ARoguelikeMapManager()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<ARoguelikeMap> RoguelikeMapBPClass(TEXT("/Game/Roguelike/Blueprints/BP_RoguelikeMap"));
	if (RoguelikeMapBPClass.Class != nullptr)
	{
		DefaultMapClass = RoguelikeMapBPClass.Class;
	}
	static ConstructorHelpers::FClassFinder<ARoguelikePawn> PawnBPClass(TEXT("/Game/Roguelike/Blueprints/BP_RoguelikePawn"));
	if (PawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PawnBPClass.Class;
	}
	static ConstructorHelpers::FClassFinder<APlacedItem> PlacedItemBPClass(TEXT("/Game/Roguelike/Blueprints/BP_PlacedItem"));
	if (PlacedItemBPClass.Class != nullptr)
	{
		DefaultPlacedItemClass = PlacedItemBPClass.Class;
	}
}

void ARoguelikeMapManager::BeginPlay()
{
	Super::BeginPlay();

	FRoguelikeMapLevelParameter param;
	param.EnemyNum = 2;
	param.ItemNum = 4;
	
	CurrentMap = Cast<ARoguelikeMap>(GetWorld()->SpawnActor(DefaultMapClass));
	CurrentMap->CreateNewMap();
	CurrentMap->SetActorLocation(GetActorLocation());

	int seed = 123456;
	FRandomStream random = FRandomStream(seed);

	// ÉAÉCÉeÉÄÇÃê∂ê¨
	{
		TArray<APlacedItem*> PlacedItems;
		for (int Index = 0; Index < param.ItemNum; ++Index)
		{
			auto* Item = Cast<APlacedItem>(GetWorld()->SpawnActor(DefaultPlacedItemClass));
			Item->Create(static_cast<EItemType>(random.RandRange(0, static_cast<int32>(EItemType::Num) - 1)));
			PlacedItems.Add(Item);
		}

		int32 ItemNum = 0;
		while (ItemNum < param.ItemNum)
		{
			if (CurrentMap->PutItem(
				PlacedItems[ItemNum],
				random.RandRange(0, CurrentMap->GetWidth()),
				random.RandRange(0, CurrentMap->GetHeight())))
			{
				++ItemNum;
			}
		}
	}

	auto* Player = Cast<ARoguelikePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Player->GetRoguelikeMovementComponent()->SetRoguelikeMap(CurrentMap);
	Player->GetRoguelikeMovementComponent()->SetPoint(FIntPoint(5, 5));

	{
		TArray<ARoguelikePawn*> Enemys;
		for (int Index = 0; Index < param.EnemyNum; ++Index)
		{
			auto* Pawn = Cast<ARoguelikePawn>(GetWorld()->SpawnActor(DefaultPawnClass));
			Pawn->GetRoguelikeMovementComponent()->SetRoguelikeMap(CurrentMap);
			Enemys.Add(Pawn);
		}

		int32 EnemyNum = 0;
		while (EnemyNum < param.EnemyNum)
		{
			const FIntPoint Point(
				random.RandRange(0, CurrentMap->GetWidth()), 
				random.RandRange(0, CurrentMap->GetHeight()));
			if (CurrentMap->CanPass(Point.X, Point.Y))
			{
				Enemys[EnemyNum]->GetRoguelikeMovementComponent()->SetPoint(Point);
				++EnemyNum;
			}
		}
	}
}

// Called every frame
void ARoguelikeMapManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

