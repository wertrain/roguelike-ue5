#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoguelikeMap.generated.h"

UCLASS()
class ROGUELIKE_API ARoguelikeMap : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

public:	
	// Sets default values for this actor's properties
	ARoguelikeMap();

	UFUNCTION(BlueprintCallable, Category = "Roguelike|Map")
	void CreateNewMap();

	FVector GetLocationOnGrid(int X, int Y) const;
	bool CanPass(int X, int Y);

	void SetHighlight(TArray<FIntPoint> Points);
	void ResetHighlight();

	const TArray<uint32>& GetMap() const { return Map; }
	const TArray<uint32>& GetCollisionMap() const { return CollisionMap; }
	int32 GetWidth() const { return MapWidth; }
	int32 GetHeight() const { return MapHeight; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TArray<class ARoguelikeMapBlock*> Blocks;
	int32 MapWidth;
	int32 MapHeight;
	TArray<uint32> Map;
	TArray<uint32> CollisionMap;
};
