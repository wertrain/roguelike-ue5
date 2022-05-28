// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Roguelike/Character/Skill/Skill.h"
#include "Roguelike/System/Resource/GameResources.h"

class SkillFireBall : Skill
{
public:
    GAME_RESOURCES();

public:
    SkillFireBall();

    UClass* GetSkillClass();

private:
    TSubclassOf <class AProjectileObject> ProjectileObjectClass;
};
