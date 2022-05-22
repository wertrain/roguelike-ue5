// Fill out your copyright notice in the Description page of Project Settings.


#include "Roguelike/System/Command/ProjectileCommand.h"
#include "Roguelike/Map/ProjectileObject.h"
#include "Roguelike/Map/RoguelikeMap.h"
#include "Roguelike/Character/RoguelikePawn.h"
#include "Roguelike/Character/CharacterAnimInstance.h"
#include "Kismet/GameplayStatics.h"

ProjectileCommand::ProjectileCommand(ARoguelikePawn* Attacker, TSubclassOf<class AProjectileObject> ProjectileClass, FVector TargetLocation)
    : CommandBase()
    , AttackerPawn(Attacker)
    , ProjectileObjectClass(ProjectileClass)
	, TargetLocation(TargetLocation)
{
    check(AttackerPawn != nullptr);
    check(ProjectileClass != nullptr);
}

ProjectileCommand::~ProjectileCommand()
{
}

void ProjectileCommand::Execute()
{
    if (UCharacterAnimInstance* AnimInstance = AttackerPawn->GetAnimInstance())
    {
        AnimInstance->PlayAnimMontage(ECharacterAnimMontageFlag::FireProjectile);
    }

    auto Location = AttackerPawn->GetActorLocation();
    ProjectileObject = Cast<AProjectileObject>(AttackerPawn->GetWorld()->
        SpawnActor(ProjectileObjectClass, &Location));
    ProjectileObject->Fire(Location, TargetLocation);
}

bool ProjectileCommand::IsFinished()
{
    return ProjectileObject->IsActorBeingDestroyed();
}
