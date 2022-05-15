// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandQueue.h"
#include "Roguelike/System/Command/CommandBase.h"
#include "Roguelike/System/Command/CommandUtility.h"

CommandQueue::CommandQueue()
    : Commands()
    , CurrentCommand(nullptr)
    , CurrentCommands()
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
#if false
    return Commands.IsEmpty() && CurrentCommand == nullptr;
#else
    return Commands.IsEmpty() && CurrentCommands.IsEmpty();
#endif
}

void CommandQueue::ExecuteCommand()
{
#if false
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
#else
    do
    {
        CommandBase* Command;
        if (Commands.Dequeue(Command))
        {
            Command->Execute();
            CurrentCommands.Add(Command);

            // 同期コマンドを実行したら終了待ちさせるために抜ける
            if (Command->IsSync())
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    while (!CurrentCommands.IsEmpty());
#endif
}

void CommandQueue::Update(const float DeltaTime)
{
#if false
    if (CurrentCommand)
    {
        CurrentCommand->Update(DeltaTime);
        
        if (CurrentCommand->IsFinished())
        {
            delete CurrentCommand;
            CurrentCommand = nullptr;
        }
    }
#else
    if (!CurrentCommands.IsEmpty())
    {
        TArray<CommandBase*> FinishedCommands;
        for (auto* Command : CurrentCommands)
        {
            Command->Update(DeltaTime);

            if (Command->IsFinished())
            {
                FinishedCommands.Add(Command);
            }
        }

        for (auto* Command : FinishedCommands)
        {
            CurrentCommands.Remove(Command);
            CommandUtility::DestroyCommand(Command);
        }
    }
#endif
}

bool CommandQueue::UpdateCurrentCommand(const float DeltaTime)
{
#if false
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
#else
    if (!CurrentCommands.IsEmpty())
    {
        TArray<CommandBase*> FinishedCommands;
        for (auto* Command : CurrentCommands)
        {
            Command->Update(DeltaTime);

            if (Command->IsFinished())
            {
                FinishedCommands.Add(Command);
            }
        }

        for (auto* Command : FinishedCommands)
        {
            CurrentCommands.Remove(Command);
            CommandUtility::DestroyCommand(Command);
        }
    }
    return CurrentCommands.IsEmpty();
#endif
}
