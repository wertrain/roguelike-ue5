// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class CommandBase
{
public:
	CommandBase();
	virtual ~CommandBase();

	virtual int32 GetCost() const = 0;

	virtual void Execute() {}
	virtual bool IsFinished() { return true; }
	virtual void Update(float DeltaTime) {}
};
