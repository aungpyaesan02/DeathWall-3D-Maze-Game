//=============================================================================
//
// ファイル	: Button処理 [button.h]
// Author	: 東京　GP 006 AUNG PYAE SAN
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

struct BUTTON
{
	XMFLOAT3		pos;		// ポリゴンの座標
	XMFLOAT4		color;
	float			w, h;		// 幅と高さ
	int				texNo;		// 使用しているテクスチャ番号
	bool			use;		//buttonのuse
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
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitButton(void);
void UninitButton(void);
void UpdateButton(void);
void DrawButton(void);

void GameStartAnimation(void);
void SelectMenu(void);


