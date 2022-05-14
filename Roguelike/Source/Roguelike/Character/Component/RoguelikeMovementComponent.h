// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Roguelike/Map/MapDefinitions.h"
#include "Roguelike/Character/CharacterDefinitions.h"
#include "RoguelikeMovementComponent.generated.h"

// @see https://forums.unrealengine.com/t/delegates-as-parameter/299175/15

DECLARE_DYNAMIC_DELEGATE(FOnArrivedDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROGUELIKE_API URoguelikeMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	typedef union
	{
		uint32 Value;
		struct
		{
			uint32 IsMoving : 1;
			uint32 IsTracing : 1;
			uint32 Reserved : 30;
		}
		Flags;
	}
	MoveStates;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Character", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Character", meta = (AllowPrivateAccess = "true"))
	float RotateSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Character", meta = (AllowPrivateAccess = "true"))
	EDirections Direction;

	UFUNCTION(BlueprintCallable, Category = "Roguelike|Character")
	void SetRoguelikeMap(class ARoguelikeMap* RoguelikeMap);
	UFUNCTION(BlueprintCallable, Category = "Roguelike|Character")
	void SetOnGrid(int32 X, int32 Y);
	UFUNCTION(BlueprintCallable, Category = "Roguelike|Character")
	void SetPoint(const FIntPoint Point);
	UFUNCTION(BlueprintCallable, Category = "Roguelike|Character")
	void SetDirection(const EDirections NewDirection);
	UFUNCTION(BlueprintCallable, Category = "Roguelike|Character")
	FIntPoint GetPoint() const;
	UFUNCTION(BlueprintCallable, Category = "Roguelike|Character")
	FIntPoint GetNextPoint() const;
	UFUNCTION(BlueprintCallable, Category = "Roguelike|Character")
	void MoveTo(int32 X, int32 Y);
	UFUNCTION(BlueprintCallable, Category = "Roguelike|Character")
	void MoveToPoint(const FIntPoint Point);
	UFUNCTION(BlueprintCallable, Category = "Roguelike|Character")
	void TraceTo(int32 X, int32 Y);
	UFUNCTION(BlueprintCallable, Category = "Roguelike|Character")
	void TraceToPoint(const FIntPoint Point);
	UFUNCTION(BlueprintCallable, Category = "Roguelike|Character")
	void TraceToPointEx(const FIntPoint Point, const FOnArrivedDelegate& Callback);
	UFUNCTION(BlueprintCallable, Category = "Roguelike|Character")
	bool IsMoving() const;

public:
	/** コンストラクタ */
	URoguelikeMovementComponent();
	/** 入力に応じた移動を行う */
	bool MoveByInput(bool bLeft, bool bRight, bool bUp, bool bDown);
	/** 次の目標位置を取得する */
	FVector GetNextLocation() const;
	/** マップを取得する */
	class ARoguelikeMap* GetRoguelikeMap() const;

protected:
	virtual void InitializeComponent() override;
	virtual void OnComponentCreated() override;
	virtual void AddInputVector(FVector WorldVector, bool bForce) override;
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	EDirections RotatorToDirection(const FRotator& Rotator) const;
	FRotator DirectionToRotator(const EDirections& Direction) const;

private:
	class ARoguelikeMap* RoguelikeMap;
	FIntPoint CurrentPoint;
	FIntPoint NextPoint;
	FVector CurrentLocation;
	FVector NextLocation;
	MoveStates States;
	float VelocityTimer;
	TQueue<FIntPoint> TracePoints;
	FOnArrivedDelegate TraceArrivedDelegate;
};
