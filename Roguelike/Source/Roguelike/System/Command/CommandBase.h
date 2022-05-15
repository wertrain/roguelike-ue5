// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * ���ׂẴR�}���h�̊��N���X
 * 
 * �R�}���h�Ɋւ�����ۂ̏����̓R���X�g���N�^�ōs��
 * Execute �ł̓A�j���[�V�����Ȃǂ��s�����Ƃ������̊�{�Ƃ���
 * 
 * �܂�A�ړ��R�}���h�ł���΁A�R�}���h�𔭍s�������_�œ����I�Ȑ��l�͈ړ���ɐi�߂Ă���
 * ���̂��� Execute �̌Ăяo���āA�ړ���ւ̈ړ��A�j���[�V�������s���悤�ȃC���[�W�ƂȂ�
 */
class CommandBase
{
public:
	CommandBase();
	virtual ~CommandBase();

	/** ���̃R�}���h�����s�����ۂɏ����R�X�g��Ԃ� */
	virtual int32 GetCost() const = 0;
	/** �R�}���h�̌����ڂ̏��������s���� */
	virtual void Execute() {}
	/** �R�}���h�̌����ڂ̏����������������ǂ�����Ԃ� */
	virtual bool IsFinished() { return true; }
	/** �R�}���h�̌����ڂ̏�������������܂łɎ��̃R�}���h�����s���Ă悢����Ԃ� */
	virtual bool IsSync() { return false; }
	/** �R�}���h�̌����ڂ̏����̍X�V���s�� */
	virtual void Update(float DeltaTime) {}
};
