//=============================================================================
//
// �t�@�C��	: Button���� [button.h]
// Author	: �����@GP 006 AUNG PYAE SAN
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

struct BUTTON
{
	XMFLOAT3		pos;		// �|���S���̍��W
	XMFLOAT4		color;
	float			w, h;		// ���ƍ���
	int				texNo;		// �g�p���Ă���e�N�X�`���ԍ�
	bool			use;		//button��use
};

enum
{
	START_BUTTON,
	EXIT_BUTTON,
	PLAYER_BUTTON,
	TITLE_LOGO,
	BUTTON_MAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitButton(void);
void UninitButton(void);
void UpdateButton(void);
void DrawButton(void);

void GameStartAnimation(void);
void SelectMenu(void);


