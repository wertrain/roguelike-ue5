// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Roguelike/Character/CharacterDefinitions.h"
#include "GameFramework/Actor.h"
#include "ProjectileObject.generated.h"

UCLASS()
class ROGUELIKE_API AProjectileObject : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Map|Projectile")
	uint8 bPenetrating : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Map|Projectile")
	float Speed;

	AProjectileObject();

	UFUNCTION(BlueprintCallable, Category = "Roguelike|Map|Projectile")
	void SetRoguelikeMap(class ARoguelikeMap* RoguelikeMap);
	UFUNCTION(BlueprintCallable, Category = "Roguelike|Map|Projectile")
	void Fire(const FVector Start, const FVector Target);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	class ARoguelikeMap* RoguelikeMap;
	FIntPoint StartPoint;
	EDirections FireDirection;
	FVector TargetLocation;
	FVector VectorDirection;
};
