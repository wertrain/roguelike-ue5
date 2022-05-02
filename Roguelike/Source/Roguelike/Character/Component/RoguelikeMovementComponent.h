// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Roguelike/Map/MapDefinitions.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Map", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roguelike|Map", meta = (AllowPrivateAccess = "true"))
	float RotateSpeed;

	UFUNCTION(BlueprintCallable, Category = "Roguelike|Map")
	void SetRoguelikeMap(class ARoguelikeMap* RoguelikeMap);
	UFUNCTION(BlueprintCallable, Category = "Roguelike|Map")
	void SetOnGrid(int32 X, int32 Y);
	UFUNCTION(BlueprintCallable, Category = "Roguelike|Map")
	void SetPoint(const FIntPoint Point);
	UFUNCTION(BlueprintCallable, Category = "Roguelike|Map")
	FIntPoint GetPoint() const;
	UFUNCTION(BlueprintCallable, Category = "Roguelike|Map")
	void MoveTo(int32 X, int32 Y);
	UFUNCTION(BlueprintCallable, Category = "Roguelike|Map")
	void MoveToPoint(const FIntPoint Point);
	UFUNCTION(BlueprintCallable, Category = "Roguelike|Map")
	void TraceTo(int32 X, int32 Y);
	UFUNCTION(BlueprintCallable, Category = "Roguelike|Map")
	void TraceToPoint(const FIntPoint Point);
	UFUNCTION(BlueprintCallable, Category = "Roguelike|Map")
	void TraceToPointEx(const FIntPoint Point, const FOnArrivedDelegate& Callback);
	UFUNCTION(BlueprintCallable, Category = "Roguelike|Map")
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
	class ARoguelikeMap* RoguelikeMap;
	FIntPoint CurrentPoint;
	FIntPoint NextPoint;
	FVector CurrentLocation;
	FVector NextLocation;
	MoveStates States;
	TQueue<FIntPoint> TracePoints;
	FOnArrivedDelegate TraceArrivedDelegate;
};
