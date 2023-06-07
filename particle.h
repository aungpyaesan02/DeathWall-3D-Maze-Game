//=============================================================================
//
// ファイル	: パーティクル処理 [particle.h]
// Author	: 東京　GP 006 AUNG PYAE SAN
//
//=============================================================================
#pragma once

#define	MAX_PARTICLE		(10)	// パーティクル最大数

typedef struct
{
	XMFLOAT3		pos;			// 位置
	XMFLOAT3		playerpos;
	XMFLOAT3		rot;			// 回転
	XMFLOAT3		scale;			// スケール
	XMFLOAT3		move;			// 移動量
	MATERIAL		material;		// マテリアル
	float			fSizeX;			// 幅
	float			fSizeY;			// 高さ
	float			spd;
	int				nIdxShadow;		// 影ID
	int				nLife;			// 寿命

	BOOL			bUse;			// 使用しているかどうか

} PARTICLE;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

int SetParticle(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife);
void SetColorParticle(int nIdxParticle, XMFLOAT4 col);
PARTICLE *GetParticle(void);

