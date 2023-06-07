//=============================================================================
//
// ファイル	: モーフィング処理 [morphing.cpp]
// Author	: 東京　GP 006 AUNG PYAE SAN
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "morphing.h"
#include "player.h"
#include "particle.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MORPHING_WAIT		(70.0f)	// 60フレームで変形
#define ROTATE_SPD			(0.01f)
#define MORPHIN_OFFSET_Y	(10.0f)		// morphingの足元をあわせる

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static MORPHING		morphing[MAX_MORPHIN];
static MODEL		modelTable[MODEL_MAX];

static char *modelNames[MODEL_MAX] =
{
	"data/MODEL/droid01.obj",	// ベースモデル
	"data/MODEL/droid02.obj",
	"data/MODEL/droid03.obj",
	"data/MODEL/droid04.obj",
	"data/MODEL/droid05.obj",

};

static BOOL enableRotate = TRUE;
static BOOL				g_Load = FALSE;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMorphing(void)
{
	// OBJデータの読み込み＆初期化
	for (int i = 0; i < MAX_MORPHIN; i++)
	{
		LoadModel(modelNames[0], &morphing[i].model);
		morphing[i].load = TRUE;
		morphing[i].use  = TRUE;
		morphing[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		morphing[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		morphing[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		morphing[i].size = MORPHIN_SIZE;
		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&morphing[i].model, &morphing[i].diffuse[0]);

		morphing[i].time = 0.0f;
		morphing[i].sign = 1;
		morphing[i].index = 0;
	}

	morphing[0].pos = XMFLOAT3(-80.0f, MORPHIN_OFFSET_Y, -387.0f),
	morphing[1].pos = XMFLOAT3(-440.0f, MORPHIN_OFFSET_Y,-490.0f);
	morphing[2].pos = XMFLOAT3(-600.0f, MORPHIN_OFFSET_Y,-232.0f);
	morphing[3].pos = XMFLOAT3( 600.0f, MORPHIN_OFFSET_Y, 450.0f);
	morphing[4].pos = XMFLOAT3( 190.0f, MORPHIN_OFFSET_Y, -85.0f);
	morphing[5].pos = XMFLOAT3(-151.0f, MORPHIN_OFFSET_Y, 500.0f);
	morphing[6].pos = XMFLOAT3(-278.0f, MORPHIN_OFFSET_Y, 108.0f);
	morphing[7].pos = XMFLOAT3(  29.0f, MORPHIN_OFFSET_Y, 378.0f);


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
void UninitMorphing(void)
{
	if (g_Load == FALSE) return;

	// モデルの解放処理
	for (int i = 0; i < MAX_MORPHIN; i++)
	{
		if (morphing[i].load)
		{
			UnloadModel(&morphing[i].model);
			morphing[i].load = FALSE;

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
void UpdateMorphing(void)
{
	PLAYER *g_Player = GetPlayer(); // プレイヤーの情報をすべて取得している。

	for (int i = 0; i < MAX_MORPHIN; i++)
	{

		if (morphing[i].use == TRUE)
		{



			int playerDistance;
			int enemyDistance;
			int minDistance = 60;
			int distance;

			playerDistance = (int)sqrt(g_Player->pos.x * g_Player->pos.x + g_Player->pos.z * g_Player->pos.z);

			enemyDistance = (int)sqrt(morphing[i].pos.x * morphing[i].pos.x + morphing[i].pos.z * morphing[i].pos.z);

			distance = playerDistance - enemyDistance;

			distance = abs(distance);

			if (distance <= minDistance)
			{


				XMVECTOR pos1 = XMLoadFloat3(&g_Player->pos);
				XMVECTOR pos2 = XMLoadFloat3(&morphing[i].pos);
				XMVECTOR rot1 = XMLoadFloat3(&g_Player->rot);
				XMVECTOR rot2 = XMLoadFloat3(&morphing[i].rot);

				pos2 += (pos1 - pos2) * 0.01f;
				rot2 += (rot1 - rot2) * 0.05f;

				XMStoreFloat3(&morphing[i].pos, pos2);
				XMStoreFloat3(&morphing[i].rot, rot2);

				// 線形補間用のタイマー更新処理
				morphing[i].time += (1.0f / MORPHING_WAIT);
				if (morphing[i].time > 1.0f)
				{
					morphing[i].index += morphing[i].sign;
					morphing[i].time = 0.0f;
				}

				if ((morphing[i].sign > 0 && morphing[i].index > MODEL_MAX - 2) ||
					(morphing[i].sign < 0 && morphing[i].index < 1))
				{
					morphing[i].sign *= -0;
				}


			}



			
			int curIndex = morphing[i].index;
			int nextIndex = curIndex + morphing[i].sign;

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
				XMStoreFloat4(&vertexArray[j].Diffuse, d0 + vec * morphing[i].time);

				// Normal
				XMVECTOR n0 = XMLoadFloat3(&modelTable[curIndex].VertexArray[j].Normal);	// 現在の場所
				XMVECTOR n1 = XMLoadFloat3(&modelTable[nextIndex].VertexArray[j].Normal);	// 次の場所
				vec = n1 - n0;
				XMStoreFloat3(&vertexArray[j].Normal, n0 + vec * morphing[i].time);

				// Position
				XMVECTOR p0 = XMLoadFloat3(&modelTable[curIndex].VertexArray[j].Position);	// 現在の場所
				XMVECTOR p1 = XMLoadFloat3(&modelTable[nextIndex].VertexArray[j].Position);	// 次の場所
				vec = p1 - p0;
				XMStoreFloat3(&vertexArray[j].Position, p0 + vec * morphing[i].time);

				// TexCoord
				XMVECTOR t0 = XMLoadFloat2(&modelTable[curIndex].VertexArray[j].TexCoord);	// 現在の場所
				XMVECTOR t1 = XMLoadFloat2(&modelTable[nextIndex].VertexArray[j].TexCoord);	// 次の場所
				vec = t1 - t0;
				XMStoreFloat2(&vertexArray[j].TexCoord, t0 + vec * morphing[i].time);
			}


			// 頂点バッファを作り直すRecreate the vertex buffer
			{
				morphing[i].model.VertexBuffer->Release();

				D3D11_BUFFER_DESC bd;
				ZeroMemory(&bd, sizeof(bd));
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(VERTEX_3D) * vertexNum;
				bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				bd.CPUAccessFlags = 0;

				D3D11_SUBRESOURCE_DATA sd;
				ZeroMemory(&sd, sizeof(sd));
				sd.pSysMem = vertexArray;

				GetDevice()->CreateBuffer(&bd, &sd, &morphing[i].model.VertexBuffer);

				delete[] vertexArray;
			}


#ifdef _DEBUG
			PrintDebugProc(" Rotate ON/OFF\n   →SPACE\n\n");
			PrintDebugProc(" Index   : %d\n", curIndex);
			PrintDebugProc(" Time    : %f\n", morphing[i].time);
#endif // _DEBUG
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMorphing(void)
{
	// カリング無効
	SetCullingMode(CULL_MODE_BACK);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	for (int i = 0; i < MAX_MORPHIN; i++)
	{

		if (morphing[i].use == FALSE) continue;

			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(morphing[i].scl.x, morphing[i].scl.y, morphing[i].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(morphing[i].rot.x, morphing[i].rot.y + XM_PI, morphing[i].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(morphing[i].pos.x, morphing[i].pos.y, morphing[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&morphing[i].mtxWorld, mtxWorld);


			// モデル描画
			DrawModel(&morphing[i].model);
		
	}
	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
MORPHING *GetMorphing(void)
{
	return &morphing[0];
}

