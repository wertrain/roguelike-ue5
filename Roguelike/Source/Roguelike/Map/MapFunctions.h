// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/// <summary>
/// マップ関数定義
/// </summary>
class MapFunctions
{
public:
    static void Dijkstra(const class ARoguelikeMap& Map, const FIntPoint& Start, const FIntPoint& Goal, TArray<FIntPoint>& OutArray);
    static void Dijkstra(const class ARoguelikeMap& Map, const FIntPoint& Start, const TArray<FIntPoint>& Goals, TArray<TArray<FIntPoint>*>& OutArrays);
};
