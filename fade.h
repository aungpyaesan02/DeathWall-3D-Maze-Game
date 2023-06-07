//=============================================================================
//
// ファイル	: フェード処理 [fade.h]
// Author	: 東京　GP 006 AUNG PYAE SAN
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************

// フェードの状態
enum
{
	FADE_NONE = 0,		// 何もない状態
	FADE_IN,			// フェードイン処理
	FADE_OUT,			// フェードアウト処理
	FADE_MAX
};

struct FADE
{
	XMFLOAT3		pos;		// ポリゴンの座標
	float			w, h;		// 幅と高さ
	int				texNo;		// 使用しているテクスチャ番号

	int				flag;		// FADE_IN / FADE_OUT
	int				nextMode;	// 遷移する先のモード
	XMFLOAT4		color;		// フェードのカラー（α値）

};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitFade(void);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);

void SetFade(int fadeFlg, int nextMode);

int GetFade(void);



