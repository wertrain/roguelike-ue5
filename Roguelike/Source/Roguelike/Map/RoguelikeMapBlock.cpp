// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeMapBlock.h"
#include "RoguelikeMap.h"

// Sets default values
ARoguelikeMapBlock::ARoguelikeMapBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> BlockMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> MaterialFloor;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> MaterialGrid;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> MaterialHighlight;
		FConstructorStatics()
			: BlockMesh(TEXT("/Game/Meshes/SM_Cube.SM_Cube"))
			, MaterialFloor(TEXT("/Game/Materials/MI_BlockFloor.MI_BlockFloor"))
			, MaterialGrid(TEXT("/Game/LevelPrototyping/Materials/MI_PrototypeGrid_Gray.MI_PrototypeGrid_Gray"))
			, MaterialHighlight(TEXT("/Game/Materials/MI_BlockHighlight.MI_BlockHighlight"))
			//, MaterialGrid(TEXT("/Game/LevelPrototyping/Materials/MI_PrototypeGrid_Gray.MI_PrototypeGrid_Gray"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.BlockMesh.Get());
	//BlockMesh->SetRelativeScale3D(FVector(1.f * scale, 1.f * scale, 1.f * scale));
	BlockMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	//BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	//BlockMesh->SetMaterial(0, ConstructorStatics.MaterialFloor.Get());
	BlockMesh->SetupAttachment(DummyRoot);

	Materials.Add(ConstructorStatics.MaterialFloor.Get());
	Materials.Add(ConstructorStatics.MaterialGrid.Get());
	Materials.Add(ConstructorStatics.MaterialHighlight.Get());
}

void ARoguelikeMapBlock::SetHighlight()
{
	BlockMesh->SetMaterial(0, Materials[2]);
}

void ARoguelikeMapBlock::ResetMaterial()
{
	BlockMesh->SetMaterial(0, Materials[1]);
}

// Called when the game starts or when spawned
void ARoguelikeMapBlock::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARoguelikeMapBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

