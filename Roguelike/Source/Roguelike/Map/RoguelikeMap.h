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
};
