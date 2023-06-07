//=============================================================================
//
// �t�@�C��	: �t�F�[�h���� [fade.h]
// Author	: �����@GP 006 AUNG PYAE SAN
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************

// �t�F�[�h�̏��
enum
{
	FADE_NONE = 0,		// �����Ȃ����
	FADE_IN,			// �t�F�[�h�C������
	FADE_OUT,			// �t�F�[�h�A�E�g����
	FADE_MAX
};

struct FADE
{
	XMFLOAT3		pos;		// �|���S���̍��W
	float			w, h;		// ���ƍ���
	int				texNo;		// �g�p���Ă���e�N�X�`���ԍ�

	int				flag;		// FADE_IN / FADE_OUT
	int				nextMode;	// �J�ڂ����̃��[�h
	XMFLOAT4		color;		// �t�F�[�h�̃J���[�i���l�j

};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitFade(void);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);

void SetFade(int fadeFlg, int nextMode);

int GetFade(void);



