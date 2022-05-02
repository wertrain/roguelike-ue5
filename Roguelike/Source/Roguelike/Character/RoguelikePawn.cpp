// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikePawn.h"

#include "Roguelike/RoguelikeCharacter.h"
#include "Roguelike/Character/Component/RoguelikeMovementComponent.h"
#include "Roguelike/Map/MapFunctions.h"
#include "Roguelike/Map/RoguelikeMap.h"
#include "Roguelike/System/Command/MovementCommand.h"
#include "Roguelike/System/RoguelikeGameSubsystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARoguelikePawn::ARoguelikePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Create a movement...
	RoguelikeMovementComponent = CreateDefaultSubobject<URoguelikeMovementComponent>(TEXT("RoguelikeMovementComponent"));
	//GridMovementComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	BehaviorID = EBehaviorID::Nop;
}

// Called when the game starts or when spawned
void ARoguelikePawn::BeginPlay()
{
	Super::BeginPlay();
	RoguelikeMovementComponent->SetPoint(FIntPoint(14, 14));

	BehaviorID = EBehaviorID::SearchPlayer;

	UGameInstance* GameInstance = GetGameInstance();
	URoguelikeGameSubsystem* RoguelikeGameSubsystem = GameInstance->GetSubsystem<URoguelikeGameSubsystem>();
	RoguelikeGameSubsystem->AddPawn(this);
}

// Called every frame
void ARoguelikePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RoguelikeCommands.Update(DeltaTime);
}

// Called to bind functionality to input
void ARoguelikePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ARoguelikePawn::Step(const uint64 TurnCount)
{
	ARoguelikeCharacter* Player = Cast<ARoguelikeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (BehaviorID == EBehaviorID::SearchPlayer)
	{
		BehaviorID = EBehaviorID::TracePlayer;
		
		TArray<FIntPoint> Points;
		MapFunctions::Dijkstra(
			*RoguelikeMovementComponent->GetRoguelikeMap(), RoguelikeMovementComponent->GetPoint(),
			Player->GetRoguelikeMovementComponent()->GetPoint(), Points);
		for (auto& Point : Points)
		{
			TracePoints.Enqueue(FIntPoint(Point.X, Point.Y));
		}

		RoguelikeMovementComponent->GetRoguelikeMap()->SetHighlight(Points);

		// ç≈èâÇÃà⁄ìÆêÊÇê›íË
		FIntPoint Point;
		if (TracePoints.Dequeue(Point))
		{
			if (TracePoints.Dequeue(Point))
			{
				auto* Command = new MovementCommand(this, Point);
				RoguelikeCommands.Enqueue(Command);
			}
		}
	}
	else if (BehaviorID == EBehaviorID::TracePlayer)
	{
		FIntPoint Point;
		if (TracePoints.Dequeue(Point))
		{
			auto* Command = new MovementCommand(this, Point);
			RoguelikeCommands.Enqueue(Command);
		}
		else
		{
			BehaviorID = EBehaviorID::SearchPlayer;
		}
	}

	RoguelikeCommands.ExecuteCommand();
}

bool ARoguelikePawn::IsStepFinished(uint64 TurnCount)
{
	return RoguelikeCommands.IsEmpty();
}
