//=============================================================================
//
// ファイル	: モーフィング処理 [dragon.cpp]
// Author	: 東京　GP 006 AUNG PYAE SAN
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "dragon.h"
#include "player.h"
#include "ebullet.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MORPHING_WAIT	(60.0f)	// 60フレームで変形
#define ROTATE_SPD		(0.01f)
#define MORPHIN_OFFSET_Y	(20.0f)						// エネミーの足元をあわせる
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// 回転量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static DRAGON		dragon[MAX_DRAGON];
static MODEL		modelTable[MODEL_MAX];

static char *modelNames[MODEL_MAX] =
{

	"data/MODEL/nessyCube01.obj",	
	"data/MODEL/nessyCube01.obj",	
	"data/MODEL/nessyCube02.obj",

};

static BOOL enableRotate = TRUE;
static BOOL				g_Load = FALSE;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitDragon(void)
{
	// OBJデータの読み込み＆初期化
	for (int i = 0; i < MAX_DRAGON; i++)
	{
		LoadModel(modelNames[0], &dragon[i].model);
		dragon[i].load = TRUE;
		dragon[i].use  = TRUE;
		dragon[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		dragon[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		dragon[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		dragon[i].size = MORPHIN_SIZE;
		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&dragon[i].model, &dragon[i].diffuse[0]);

		dragon[i].time = 0.0f;
		dragon[i].sign = 1;
		dragon[i].index = 0;
	}

	dragon[0].pos = XMFLOAT3( 312.0f, MORPHIN_OFFSET_Y, -40.0f);
	dragon[1].pos = XMFLOAT3(-497.0f, MORPHIN_OFFSET_Y,-144.0f);
	dragon[2].pos = XMFLOAT3(-235.0f, MORPHIN_OFFSET_Y,-537.0f);
	dragon[3].pos = XMFLOAT3( 253.0f, MORPHIN_OFFSET_Y,-263.0f);


	// 頂点情報読み込み
	for (int i = 0; i < MODEL_MAX; i++)
	{
		LoadObj(modelNames[i], &modelTable[i]);
	}

	g_Load = TRUE;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitDragon(void)
{
	if (g_Load == FALSE) return;

	// モデルの解放処理
	for (int i = 0; i < MAX_DRAGON; i++)
	{
		if (dragon[i].load)
		{
			UnloadModel(&dragon[i].model);
			dragon[i].load = FALSE;

		}

	}

	for (int j = 0; j < MODEL_MAX; j++)
	{
		delete[] modelTable[j].VertexArray;
		delete[] modelTable[j].IndexArray;
		delete[] modelTable[j].SubsetArray;
	}

	g_Load = FALSE;

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateDragon(void)
{
	PLAYER *g_Player = GetPlayer(); // プレイヤーの情報をすべて取得している。

	for (int i = 0; i < MAX_DRAGON; i++)
	{

		if (dragon[i].use == TRUE)
		{


			int playerDistance;
			int enemyDistance;
			int minDistance = 50;
			int distance;

			playerDistance = (int)sqrt(g_Player->pos.x * g_Player->pos.x + g_Player->pos.z * g_Player->pos.z);

			enemyDistance = (int)sqrt(dragon[i].pos.x * dragon[i].pos.x + dragon[i].pos.z * dragon[i].pos.z);

			distance = playerDistance - enemyDistance;

			distance = abs(distance);

			if (distance <= minDistance)
			{


				XMVECTOR pos1 = XMLoadFloat3(&g_Player[0].pos);
				XMVECTOR pos2 = XMLoadFloat3(&dragon[i].pos);
				XMVECTOR rot1 = XMLoadFloat3(&g_Player[0].rot);
				XMVECTOR rot2 = XMLoadFloat3(&dragon[i].rot);

				pos2   += (pos1 - pos2) * 0.01f;
				rot2   += (rot1 - rot2) * 0.05f;

				XMStoreFloat3(&dragon[i].pos, pos2);
				XMStoreFloat3(&dragon[i].rot, rot2);


				// 線形補間用のタイマー更新処理
				dragon[i].time += (1.0f / MORPHING_WAIT);
				if (dragon[i].time > 1.0f)
				{
					dragon[i].index += dragon[i].sign;
					dragon[i].time = 0.0f;
				}

				if ((dragon[i].sign > 0 && dragon[i].index > MODEL_MAX - 2) ||
					(dragon[i].sign < 0 && dragon[i].index < 1))
				{
					dragon[i].sign *= -0;
				}

				//SetEBullet(dragon[i].pos, dragon[i].rot, XMFLOAT4{ 0.0f,0.0f,1.0f,1.0f }, 10.0f, 10.0f, 400);


			}


			int curIndex = dragon[i].index;
			int nextIndex = curIndex + dragon[i].sign;

			// 頂点の更新処理//Vertex update process
			int vertexNum = modelTable[0].VertexNum;
			VERTEX_3D *vertexArray = new VERTEX_3D[vertexNum];
			for (int j = 0; j < vertexNum; j++)
			{
				XMVECTOR vec;

				// Diffuse
				XMVECTOR d0 = XMLoadFloat4(&modelTable[curIndex].VertexArray[j].Diffuse);	// 現在の場所
				XMVECTOR d1 = XMLoadFloat4(&modelTable[nextIndex].VertexArray[j].Diffuse);	// 次の場所
				vec = d1 - d0;
				XMStoreFloat4(&vertexArray[j].Diffuse, d0 + vec * dragon[i].time);

				// Normal
				XMVECTOR n0 = XMLoadFloat3(&modelTable[curIndex].VertexArray[j].Normal);	// 現在の場所
				XMVECTOR n1 = XMLoadFloat3(&modelTable[nextIndex].VertexArray[j].Normal);	// 次の場所
				vec = n1 - n0;
				XMStoreFloat3(&vertexArray[j].Normal, n0 + vec * dragon[i].time);

				// Position
				XMVECTOR p0 = XMLoadFloat3(&modelTable[curIndex].VertexArray[j].Position);	// 現在の場所
				XMVECTOR p1 = XMLoadFloat3(&modelTable[nextIndex].VertexArray[j].Position);	// 次の場所
				vec = p1 - p0;
				XMStoreFloat3(&vertexArray[j].Position, p0 + vec * dragon[i].time);

	

				// TexCoord
				XMVECTOR t0 = XMLoadFloat2(&modelTable[curIndex].VertexArray[j].TexCoord);	// 現在の場所
				XMVECTOR t1 = XMLoadFloat2(&modelTable[nextIndex].VertexArray[j].TexCoord);	// 次の場所
				vec = t1 - t0;
				XMStoreFloat2(&vertexArray[j].TexCoord, t0 + vec * dragon[i].time);
			}


			// 頂点バッファを作り直すRecreate the vertex buffer
			{
				dragon[i].model.VertexBuffer->Release();

				D3D11_BUFFER_DESC bd;
				ZeroMemory(&bd, sizeof(bd));
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(VERTEX_3D) * vertexNum;
				bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				bd.CPUAccessFlags = 0;

				D3D11_SUBRESOURCE_DATA sd;
				ZeroMemory(&sd, sizeof(sd));
				sd.pSysMem = vertexArray;

				GetDevice()->CreateBuffer(&bd, &sd, &dragon[i].model.VertexBuffer);

				delete[] vertexArray;
			}


#ifdef _DEBUG
			PrintDebugProc(" Rotate ON/OFF\n   →SPACE\n\n");
			PrintDebugProc(" Index   : %d\n", curIndex);
			PrintDebugProc(" Time    : %f\n", dragon[i].time);
#endif // _DEBUG
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawDragon(void)
{
	// カリング無効
	SetCullingMode(CULL_MODE_BACK);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	for (int i = 0; i < MAX_DRAGON; i++)
	{

		if (dragon[i].use == FALSE) continue;

			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(dragon[i].scl.x, dragon[i].scl.y, dragon[i].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(dragon[i].rot.x, dragon[i].rot.y + XM_PI, dragon[i].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(dragon[i].pos.x, dragon[i].pos.y, dragon[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&dragon[i].mtxWorld, mtxWorld);

			// モデル描画
			DrawModel(&dragon[i].model);
		
	}
	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
DRAGON *GetDragon(void)
{
	return &dragon[0];
}

