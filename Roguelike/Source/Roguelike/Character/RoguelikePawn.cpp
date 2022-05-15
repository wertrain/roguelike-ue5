// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikePawn.h"

#include "Roguelike/RoguelikeCharacter.h"
#include "Roguelike/Character/Component/RoguelikeMovementComponent.h"
#include "Roguelike/Character/CharacterAnimInstance.h"
#include "Roguelike/Map/MapFunctions.h"
#include "Roguelike/Map/RoguelikeMap.h"
#include "Roguelike/System/Command/MovementCommand.h"
#include "Roguelike/System/RoguelikeGameSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ARoguelikePawn::ARoguelikePawn()
	: PlayerIndex(-1)
	, InitialPoint(0, 0)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Create a movement...
	RoguelikeMovementComponent = CreateDefaultSubobject<URoguelikeMovementComponent>(TEXT("RoguelikeMovementComponent"));
	//GridMovementComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

// Called when the game starts or when spawned
void ARoguelikePawn::BeginPlay()
{
	Super::BeginPlay();
	//RoguelikeMovementComponent->SetPoint(InitialPoint);

	CurrentStatus = OriginalStatus;

	UGameInstance* GameInstance = GetGameInstance();
	URoguelikeGameSubsystem* RoguelikeGameSubsystem = GameInstance->GetSubsystem<URoguelikeGameSubsystem>();
	RoguelikeGameSubsystem->AddPawn(CurrentStatus.Faction, this);
}

void ARoguelikePawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UGameInstance* GameInstance = GetGameInstance();
	URoguelikeGameSubsystem* RoguelikeGameSubsystem = GameInstance->GetSubsystem<URoguelikeGameSubsystem>();
	RoguelikeGameSubsystem->RemovePawn(this);
}

// Called every frame
void ARoguelikePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARoguelikePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UCharacterAnimInstance* ARoguelikePawn::GetAnimInstance() const
{
	USkeletalMeshComponent* SkeletalMeshComponent = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	return Cast<UCharacterAnimInstance>(SkeletalMeshComponent->GetAnimInstance());
}

FCharacterStatus& ARoguelikePawn::GetCurrentStatus()
{
	return CurrentStatus;
}
