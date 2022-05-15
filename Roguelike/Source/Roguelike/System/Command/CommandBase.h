// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * すべてのコマンドの基底クラス
 * 
 * コマンドに関する実際の処理はコンストラクタで行い
 * Execute ではアニメーションなどを行うことを実装の基本とする
 * 
 * つまり、移動コマンドであれば、コマンドを発行した時点で内部的な数値は移動先に進めておき
 * そのあと Execute の呼び出して、移動先への移動アニメーションを行うようなイメージとなる
 */
class CommandBase
{
public:
	CommandBase();
	virtual ~CommandBase();

	/** このコマンドを実行した際に消費するコストを返す */
	virtual int32 GetCost() const = 0;
	/** コマンドの見た目の処理を実行する */
	virtual void Execute() {}
	/** コマンドの見た目の処理が完了したかどうかを返す */
	virtual bool IsFinished() { return true; }
	/** コマンドの見た目の処理が完了するまでに次のコマンドを実行してよいかを返す */
	virtual bool IsSync() { return false; }
	/** コマンドの見た目の処理の更新を行う */
	virtual void Update(float DeltaTime) {}
};
