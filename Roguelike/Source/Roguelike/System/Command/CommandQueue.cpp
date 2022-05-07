// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandQueue.h"
#include "Roguelike/System/Command/CommandBase.h"

CommandQueue::CommandQueue()
    : Commands()
    , CurrentCommand(nullptr)
{
}

CommandQueue::~CommandQueue()
{
}

void CommandQueue::Enqueue(CommandBase* Command)
{
    Commands.Enqueue(Command);
}

CommandBase* CommandQueue::Dequeue()
{
    CommandBase* Command;
    if (Commands.Dequeue(Command))
    {
        return Command;
    }
    return nullptr;
}

bool CommandQueue::IsEmpty()
{
    return Commands.IsEmpty() && CurrentCommand == nullptr;
}

void CommandQueue::ExecuteCommand()
{
    do
    {
        if (Commands.Dequeue(CurrentCommand))
        {
            CurrentCommand->Execute();
        }
        else
        {
            break;
        }
    }
    while (CurrentCommand->IsFinished());
}

void CommandQueue::Update(const float DeltaTime)
{
    if (CurrentCommand)
    {
        CurrentCommand->Update(DeltaTime);
        
        if (CurrentCommand->IsFinished())
        {
            delete CurrentCommand;
            CurrentCommand = nullptr;
        }
    }
}

bool CommandQueue::UpdateCurrentCommand(const float DeltaTime)
{
    if (CurrentCommand)
    {
        CurrentCommand->Update(DeltaTime);

        if (CurrentCommand->IsFinished())
        {
            delete CurrentCommand;
            CurrentCommand = nullptr;
            return true;
        }
    }
    return false;
}
