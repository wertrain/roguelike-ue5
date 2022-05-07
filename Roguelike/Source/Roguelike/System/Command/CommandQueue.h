// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ROGUELIKE_API CommandQueue
{
public:
	CommandQueue();
	~CommandQueue();

	void Enqueue(class CommandBase* Command);
	class CommandBase* Dequeue();
	bool IsEmpty();
	void ExecuteCommand();
	void Update(float DeltaTime);
	bool UpdateCurrentCommand(const float DeltaTime);

private:
	TQueue<class CommandBase*> Commands;
	class CommandBase* CurrentCommand;
};
