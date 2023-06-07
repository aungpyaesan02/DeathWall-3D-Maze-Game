//=============================================================================
//
// ファイル	: パーティクル処理 [ebullet.cpp]
// Author	: 東京　GP 006 AUNG PYAE SAN
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "shadow.h"
#include "ebullet.h"
#include "player.h"
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(1)			// テクスチャの数

#define	PARTICLE_SIZE_X		(30.0f)		// 頂点サイズ
#define	PARTICLE_SIZE_Y		(30.0f)		// 頂点サイズ
#define	VALUE_MOVE_PARTICLE	(5.0f)		// 移動速度
#define BULLER_DISTANCE		(100)

#define	DISP_SHADOW						// 影の表示
//#undef DISP_SHADOW

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEBullet(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;				// 頂点バッファ

static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static int							g_TexNo;							// テクスチャ番号

static EBULLET					g_EBullet[MAX_EBULLET];					// パーティクルワーク
static XMFLOAT3					g_posBase;								// ビルボード発生位置
static float					g_fWidthBase = 5.0f;					// 基準の幅
static float					g_fHeightBase = 10.0f;					// 基準の高さ
static float					g_roty = 0.0f;							// 移動方向
static float					g_spd = 0.0f;							// 移動スピード

static char *g_TextureName[TEXTURE_MAX] =
{
	"data/TEXTURE/power.jpg",
};

static BOOL						g_Load = FALSE;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEBullet(void)
{
	// 頂点情報の作成
	MakeVertexEBullet();

	// テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}

	g_TexNo = 0;

	// パーティクルワークの初期化
	for(int nEBullet = 0; nEBullet < MAX_EBULLET; nEBullet++)
	{

		g_EBullet[nEBullet].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_EBullet[nEBullet].playerpos = XMFLOAT3(0.0f, 0.0f, 0.0f);

		g_EBullet[nEBullet].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_EBullet[nEBullet].scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_EBullet[nEBullet].move = XMFLOAT3(1.0f, 1.0f, 1.0f);

		ZeroMemory(&g_EBullet[nEBullet].material, sizeof(g_EBullet[nEBullet].material));
		g_EBullet[nEBullet].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_EBullet[nEBullet].fSizeX = PARTICLE_SIZE_X;
		g_EBullet[nEBullet].fSizeY = PARTICLE_SIZE_Y;
		g_EBullet[nEBullet].nIdxShadow = -1;
		g_EBullet[nEBullet].nLife = 0;
		g_EBullet[nEBullet].bUse = FALSE;
	}

	g_posBase = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_roty = 0.0f;
	g_spd = 0.0f;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEBullet(void)
{
	if (g_Load == FALSE) return;

	//テクスチャの解放
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (g_Texture[nCntTex] != NULL)
		{
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	// 頂点バッファの解放
	if (g_VertexBuffer != NULL)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEBullet(void)
{

	{
		for(int nEBullet = 0; nEBullet < MAX_EBULLET; nEBullet++)
		{
			if(g_EBullet[nEBullet].bUse)
			{
				// 弾の移動処理
				g_EBullet[nEBullet].pos.x -= sinf(g_EBullet[nEBullet].rot.y) * g_EBullet[nEBullet].spd;
				g_EBullet[nEBullet].pos.z -= cosf(g_EBullet[nEBullet].rot.y) * g_EBullet[nEBullet].spd;

				// 影の位置設定
				SetPositionShadow(g_EBullet[nEBullet].nIdxShadow, XMFLOAT3(g_EBullet[nEBullet].pos.x, 0.1f, g_EBullet[nEBullet].pos.z));


				// フィールドの外に出たら弾を消す処理
				if (g_EBullet[nEBullet].pos.x < MAP_LEFT 
					|| g_EBullet[nEBullet].pos.x > MAP_RIGHT 
					|| g_EBullet[nEBullet].pos.z < MAP_DOWN 
					|| g_EBullet[nEBullet].pos.z > MAP_TOP )
				{
					g_EBullet[nEBullet].bUse = FALSE;
					ReleaseShadow(g_EBullet[nEBullet].nIdxShadow);
				}

				if (g_EBullet[nEBullet].pos.x < g_EBullet[nEBullet].playerpos.x - BULLER_DISTANCE
					|| g_EBullet[nEBullet].pos.x > g_EBullet[nEBullet].playerpos.x + BULLER_DISTANCE
					|| g_EBullet[nEBullet].pos.z < g_EBullet[nEBullet].playerpos.z - BULLER_DISTANCE
					|| g_EBullet[nEBullet].pos.z > g_EBullet[nEBullet].playerpos.z + BULLER_DISTANCE)
				{
					g_EBullet[nEBullet].bUse = FALSE;
					ReleaseShadow(g_EBullet[nEBullet].nIdxShadow);
				}

			}
		}

	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEBullet(void)
{
	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	CAMERA *cam = GetCamera();

	// ライティングを無効に
	SetLightEnable(FALSE);

	// 加算合成に設定
	SetBlendState(BLEND_MODE_ADD);

	// Z比較無し
	SetDepthEnable(FALSE);

	// フォグ無効
	SetFogEnable(FALSE);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	for(int nEBullet = 0; nEBullet < MAX_EBULLET; nEBullet++)
	{
		if(g_EBullet[nEBullet].bUse)
		{
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// ビューマトリックスを取得
			mtxView = XMLoadFloat4x4(&cam->mtxView);


			// 処理が速いしお勧め
			mtxWorld.r[0].m128_f32[0] = mtxView.r[0].m128_f32[0];
			mtxWorld.r[0].m128_f32[1] = mtxView.r[1].m128_f32[0];
			mtxWorld.r[0].m128_f32[2] = mtxView.r[2].m128_f32[0];

			mtxWorld.r[1].m128_f32[0] = mtxView.r[0].m128_f32[1];
			mtxWorld.r[1].m128_f32[1] = mtxView.r[1].m128_f32[1];
			mtxWorld.r[1].m128_f32[2] = mtxView.r[2].m128_f32[1];

			mtxWorld.r[2].m128_f32[0] = mtxView.r[0].m128_f32[2];
			mtxWorld.r[2].m128_f32[1] = mtxView.r[1].m128_f32[2];
			mtxWorld.r[2].m128_f32[2] = mtxView.r[2].m128_f32[2];

			// スケールを反映
			mtxScl = XMMatrixScaling(g_EBullet[nEBullet].scale.x, g_EBullet[nEBullet].scale.y, g_EBullet[nEBullet].scale.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_EBullet[nEBullet].pos.x, g_EBullet[nEBullet].pos.y, g_EBullet[nEBullet].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			// マテリアル設定
			SetMaterial(g_EBullet[nEBullet].material);

			// ポリゴンの描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

	// ライティングを有効に
	SetLightEnable(TRUE);

	// 通常ブレンドに戻す
	SetBlendState(BLEND_MODE_ALPHABLEND);

	// Z比較有効
	SetDepthEnable(TRUE);

	// フォグ有効
	SetFogEnable(TRUE);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexEBullet(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	{//頂点バッファの中身を埋める
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// 頂点座標の設定
		vertex[0].Position = XMFLOAT3(-PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[1].Position = XMFLOAT3(PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[2].Position = XMFLOAT3(-PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[3].Position = XMFLOAT3(PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);

		// 法線の設定
		vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

		// 反射光の設定
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		vertex[0].TexCoord = { 0.0f, 0.0f };
		vertex[1].TexCoord = { 1.0f, 0.0f };
		vertex[2].TexCoord = { 0.0f, 1.0f };
		vertex[3].TexCoord = { 1.0f, 1.0f };

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	return S_OK;
}

//=============================================================================
// マテリアルカラーの設定
//=============================================================================
void SetColorEBullet(int nIdxParticle, XMFLOAT4 col)
{
	g_EBullet[nIdxParticle].material.Diffuse = col;
}

//=============================================================================
// パーティクルの発生処理
//=============================================================================
int SetEBullet(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife)
{
	int nIdxParticle = -1;

	for(int nEBullet = 0; nEBullet < MAX_EBULLET; nEBullet++)
	{
		if(!g_EBullet[nEBullet].bUse)
		{
			g_EBullet[nEBullet].pos = pos;
			g_EBullet[nEBullet].playerpos = pos;

			g_EBullet[nEBullet].rot = rot;
			g_EBullet[nEBullet].scale = { 1.0f, 1.0f, 1.0f };
			g_EBullet[nEBullet].spd = 4.0f;
			g_EBullet[nEBullet].material.Diffuse = col;
			g_EBullet[nEBullet].fSizeX = fSizeX;
			g_EBullet[nEBullet].fSizeY = fSizeY;
			g_EBullet[nEBullet].nLife = nLife;
			g_EBullet[nEBullet].bUse = TRUE;

			nIdxParticle = nEBullet;

#ifdef DISP_SHADOW
			// 影の設定
			g_EBullet[nEBullet].nIdxShadow = CreateShadow(XMFLOAT3(pos.x, 0.1f, pos.z), 0.8f, 0.8f);		// 影の設定
			if(g_EBullet[nEBullet].nIdxShadow != -1)
			{
				SetColorShadow(g_EBullet[nEBullet].nIdxShadow, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
			}
#endif

			break;
		}
	}

	return nIdxParticle;
}


EBULLET *GetEBullet(void)
{
	return &(g_EBullet[0]);
}

