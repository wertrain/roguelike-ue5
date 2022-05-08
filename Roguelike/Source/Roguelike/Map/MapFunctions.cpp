// Fill out your copyright notice in the Description page of Project Settings.


#include "MapFunctions.h"
#include "RoguelikeMap.h"

namespace
{

struct Node
{
	int32 X;
	int32 Y;
	Node* PrevNode;
	int32 Score;
	bool Pass;
	bool Checked;
};

struct LinkedNode
{
	Node* Node;
	LinkedNode* NextLink;
};

static LinkedNode NodePool[1024*1024*16];

}

void MapFunctions::Dijkstra(const ARoguelikeMap& Map, const FIntPoint& Start, const FIntPoint& Goal, TArray<FIntPoint>& OutArray)
{
	TArray<FIntPoint> Goals;
	TArray<TArray<FIntPoint>*> OutArrays;
	Goals.Add(Goal);
	OutArrays.Add(&OutArray);
	Dijkstra(Map, Start, Goals, OutArrays);
}

void MapFunctions::Dijkstra(const class ARoguelikeMap& Map, const FIntPoint& Start, const TArray<FIntPoint>& Goals, TArray<TArray<FIntPoint>*>& OutArrays)
{
	const int MapWidth = Map.GetWidth(), MapHeight = Map.GetHeight();

	TArray<Node> NodeMap;
	{
		int32 Index = 0;
		for (const auto& Value : Map.GetCollisionMap())
		{
			Node node;
			node.X = Index % MapHeight;
			node.Y = Index / MapWidth;
			node.PrevNode = nullptr;
			node.Score = 0xFFFF;
			node.Pass = Value == 0;
			node.Checked = false;
			NodeMap.Add(node);
			++Index;
		}
	}

	//TArray<Node*> SearchNodeList;

	int X = Start.X, Y = Start.Y;
	auto* StartNode = &NodeMap[(MapWidth * Y) + X];
	StartNode->Score = 0;
	//SearchNodeList.Add(StartNode);

	LinkedNode StartLink;
	StartLink.Node = StartNode;
	StartLink.NextLink = nullptr;

	LinkedNode* CurrentLink = &StartLink;
	LinkedNode* TailLink = CurrentLink;
	int LinkIndex = 0;

	while (CurrentLink)
	{
		auto* Node = CurrentLink->Node;
		X = Node->X;
		Y = Node->Y;

		const int32 XOffsets[] = { 0, 1, 0, -1 }, YOffsets[] = { 1, 0, -1, 0 };
		for (int Direction = 0; Direction < 4; ++Direction)
		{
			const int CheckX = X + XOffsets[Direction];
			const int CheckY = Y + YOffsets[Direction];

			if (CheckX < 0 || CheckX >= MapWidth || CheckY < 0 || CheckY >= MapHeight)
			{
				continue;
			}
			const int CheckIndex = (MapWidth * CheckY) + CheckX;
			if (!NodeMap[CheckIndex].Pass || NodeMap[CheckIndex].Checked)
			{
				continue;
			}
			const int NextScore = Node->Score + 1;
			if (NextScore < NodeMap[CheckIndex].Score)
			{
				NodeMap[CheckIndex].Score = Node->Score + 1;
				NodeMap[CheckIndex].PrevNode = Node;
			}

			auto NextLink = &NodePool[LinkIndex++];
			NextLink->Node = &NodeMap[CheckIndex];
			NextLink->NextLink = nullptr;

			TailLink->NextLink = NextLink;
			TailLink = NextLink;
		}
		Node->Checked = true;

		CurrentLink = CurrentLink->NextLink;
	}

#if false
	FString DebugMap;
	for (int y = 0; y < MapHeight; ++y)
	{
		for (int x = 0; x < MapWidth; ++x)
		{
			const int CheckIndex = (MapWidth * y) + x;

			DebugMap.Appendf(TEXT("%d "), NodeMap[CheckIndex].Score);
		}
		DebugMap.Append(TEXT("\n"));
	}
	UE_LOG(LogTemp, Log, TEXT("%s"), *DebugMap);
#endif

	for (int32 Index = 0; Index < Goals.Num(); ++Index)
	{
		OutArrays[Index]->Empty();
		{
			auto* GoalNode = &NodeMap[(MapWidth * Goals[Index].Y) + Goals[Index].X];
			TArray<Node*> TraceNodes;

			Node* CheckNode = GoalNode;
			while (CheckNode)
			{
				TraceNodes.Add(CheckNode);
				CheckNode = CheckNode->PrevNode;
			}

			Algo::Reverse(TraceNodes);
			for (auto* Node : TraceNodes)
			{
				OutArrays[Index]->Add(FIntPoint(Node->X, Node->Y));
			}
		}
	}
}
