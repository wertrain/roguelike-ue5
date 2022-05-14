// Fill out your copyright notice in the Description page of Project Settings.


#include "Roguelike/Map/PlacedItem.h"

// Sets default values
APlacedItem::APlacedItem()
	: ItemType(EItemType::Num)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APlacedItem::Create_Implementation(const EItemType Type)
{
	ItemType = Type;
}

// Called when the game starts or when spawned
void APlacedItem::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APlacedItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
