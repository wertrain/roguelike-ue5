// Fill out your copyright notice in the Description page of Project Settings.


#include "Roguelike/Character/Skill/SkillFireBall.h"
#include "Roguelike/Map/ProjectileObject.h"
#include "Kismet/GameplayStatics.h"

SkillFireBall::SkillFireBall()
{
	static ConstructorHelpers::FClassFinder<AProjectileObject> ProjectileBPClass(TEXT("/Game/Roguelike/Blueprints/BP_ProjectileFire"));
	if (ProjectileBPClass.Class != nullptr)
	{
		ProjectileObjectClass = ProjectileBPClass.Class;
	}
}

UClass* SkillFireBall::GetSkillClass()
{
	return ProjectileObjectClass;
}
