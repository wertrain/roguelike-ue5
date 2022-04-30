// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/// <summary>
/// �}�b�v���ʒ�`
/// </summary>
class MapDefinitions
{
public:
	static constexpr const float GridSize = 100.0f;
};

/// <summary>
/// �O���b�h��̈ʒu
/// </summary>
class GridPoint
{
public:
	GridPoint();
	GridPoint(int32 InX, int32 InY);

	int32 GetX() const { return X; }
	int32 GetY() const { return Y; }
	void SetX(int32 InX);
	void SetY(int32 InY);

private:
	int32 X;
	int32 Y;
};
