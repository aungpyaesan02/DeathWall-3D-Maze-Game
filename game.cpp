//=============================================================================
//
// ファイル	: ゲーム画面処理 [game.cpp]
// Author	: 東京　GP 006 AUNG PYAE SAN
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "camera.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "morphing.h"
#include "tower.h"
#include "meshfield.h"
#include "meshwall.h"
#include "shadow.h"
#include "tree.h"
#include "bullet.h"
#include "score.h"
#include "particle.h"
#include "particle2.h"
#include "collision.h"
#include "debugproc.h"
#include "vibration.h"
#include "ui.h"
#include "skybox.h"
#include "dragon.h"
#include "ebullet.h"
#include "cannon.h"



//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void CheckHit(void);



//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int	g_ViewPortType_Game = TYPE_FULL_SCREEN;

static BOOL	g_bPause = TRUE;	// ポーズON/OFF


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{
	g_ViewPortType_Game = TYPE_FULL_SCREEN;

	InitSkybox();

	// フィールドの初期化
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 100, 100, 13.0f, 13.0f);

	// ライトを有効化	// 影の初期化処理
	InitShadow();

	// プレイヤーの初期化
	InitPlayer();

	// エネミーの初期化
	InitEnemy();

	// Towerの初期化
	InitTower();

	// モーフィングの初期化
	InitMorphing();

	// モーフィングの初期化
	InitDragon();

	// モーフィングの初期化
	InitCannon();


	// 壁の初期化
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f,  XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);

	// 壁(裏側用の半透明)
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f,    XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f,   XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);

	// 木を生やす
	InitTree();


	// スコアの初期化
	InitScore();

	InitVibration();

	InitUI();

	// パーティクルの初期化
	InitParticle();

	// パーティクルの初期化
	InitParticle2();

	// 弾の初期化
	InitEBullet();



	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	// 弾の終了処理
	UninitEBullet();

	// パーティクルの終了処理
	UninitParticle2();

	// パーティクルの終了処理
	UninitParticle();

	// スコアの終了処理
	UninitScore();

	UninitVibration();

	UninitUI();


	// 木の終了処理
	UninitTree();

	// 壁の終了処理
	UninitMeshWall();

	// 地面の終了処理
	UninitMeshField();

	UninitSkybox();

	// エネミーの終了処理
	UninitEnemy();

	// TOWERの終了処理
	UninitTower();


	// モーフィングの終了処理
	UninitMorphing();

	// モーフィングの終了処理
	UninitDragon();


	// モーフィングの終了処理
	UninitCannon();


	// プレイヤーの終了処理
	UninitPlayer();

	// 影の終了処理
	UninitShadow();

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{

	if (GetKeyboardTrigger(DIK_P))
	{
		g_bPause = g_bPause ? FALSE : TRUE;
	}

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_V))
	{
		g_ViewPortType_Game = (g_ViewPortType_Game + 1) % TYPE_NONE;
		SetViewPort(g_ViewPortType_Game);
	}

	if (GetKeyboardTrigger(DIK_P))
	{
		g_bPause = g_bPause ? FALSE : TRUE;
	}


#endif

	if(g_bPause == FALSE)
		return;

	UpdateSkybox();

	// 地面処理の更新
	UpdateMeshField();

	// プレイヤーの更新処理
	UpdatePlayer();

	// エネミーの更新処理
	UpdateEnemy();

	// Towerの更新処理
	UpdateTower();

	// モーフィングの更新処理
	UpdateMorphing();

	// モーフィングの更新処理
	UpdateDragon();


	// モーフィングの更新処理
	UpdateCannon();


	// 壁処理の更新
	UpdateMeshWall();

	// 木の更新処理
	UpdateTree();


	// パーティクルの更新処理
	UpdateParticle();

	// パーティクルの更新処理
	UpdateParticle2();


	// 弾の更新処理
	UpdateEBullet();


	// 影の更新処理
	UpdateShadow();

	// 当たり判定処理
	CheckHit();

	// スコアの更新処理
	UpdateScore();

	UpdateVibration();

	UpdateUI();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame0(void)
{
	// 3Dの物を描画する処理
	// 地面の描画処理

	DrawSkybox();

	DrawMeshField();

	// モーフィングの描画処理
	DrawMorphing();

	// モーフィングの描画処理
	DrawDragon();

	// モーフィングの描画処理
	DrawCannon();

	// エネミーの描画処理
	DrawEnemy();

	// Towerの描画処理
	DrawTower();


	// プレイヤーの描画処理
	DrawPlayer();

	// 弾の描画処理
	DrawEBullet();

	// 壁の描画処理
	DrawMeshWall();

	// 木の描画処理
	DrawTree();

	// パーティクルの描画処理
	DrawParticle();

	// パーティクルの描画処理
	DrawParticle2();

	// 2Dの物を描画する処理
	// Z比較なし
	SetDepthEnable(FALSE);

	// ライティングを無効
	SetLightEnable(FALSE);

	// スコアの描画処理
	DrawScore();

	DrawVibration();

	DrawUI();


	// ライティングを有効に
	SetLightEnable(TRUE);

	// Z比較あり
	SetDepthEnable(TRUE);


}


void DrawGame(void)
{
	XMFLOAT3 pos;


#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Game);

#endif

	// プレイヤー視点
	pos = GetPlayer()->pos;
	pos.y = 30.0f;			// カメラ酔いを防ぐためにクリアしている
	SetCameraAT(pos);
	SetCamera();

	switch(g_ViewPortType_Game)
	{
	case TYPE_FULL_SCREEN:
		SetViewPort(TYPE_FULL_SCREEN);
		DrawGame0();
		break;

	case TYPE_LEFT_HALF_SCREEN:
	case TYPE_RIGHT_HALF_SCREEN:
		SetViewPort(TYPE_LEFT_HALF_SCREEN);
		DrawGame0();

		// エネミー視点
		pos = GetEnemy()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_RIGHT_HALF_SCREEN);
		DrawGame0();
		break;

	case TYPE_UP_HALF_SCREEN:
	case TYPE_DOWN_HALF_SCREEN:
		SetViewPort(TYPE_UP_HALF_SCREEN);
		DrawGame0();

		// エネミー視点
		pos = GetEnemy()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_DOWN_HALF_SCREEN);
		DrawGame0();
		break;

	}

}


//=============================================================================
// 当たり判定処理
//=============================================================================
void CheckHit(void)
{
	ENEMY *enemy   = GetEnemy();	// エネミーのポインターを初期化
	PLAYER *player = GetPlayer();	// プレイヤーのポインターを初期化
	EBULLET *ebullet = GetEBullet();	// 弾のポインターを初期化
	TOWER  *tower  = GetTower();	//Towerのポインターを初期化
	UI     *ui     = GetUI();	    //UIのポインターを初期化
	PARTICLE *particle = GetParticle();
	MORPHING	*morphing = GetMorphing();
	DRAGON		*dragon = GetDragon();
	CANNON		*cannon = GetCannon();

	// 敵とプレイヤーキャラ
	for (int i = 0; i < MAX_ENEMY; i++)
	{

			//敵の有効フラグをチェックする
			if (enemy[i].use == FALSE)
				continue;

				//BCの当たり判定
				if (CollisionBC(player->pos, enemy[i].pos, player->size, enemy[i].size))
				{
					// 敵キャラクターは倒される
					SetVibration();

					//SE
					PlaySound(SOUND_LABEL_SE_hugeexplosion);

					SetFade(FADE_OUT, MODE_RESULT);

				}

			
	}

	for (int i = 0; i < MAX_MORPHIN; i++)
	{

		//敵の有効フラグをチェックする
		if (morphing[i].use == FALSE)
			continue;

		//BCの当たり判定
		if (CollisionBC(player->pos, morphing[i].pos, player->size, morphing[i].size))
		{

			morphing[i].use = FALSE;

			//SE
			PlaySound(SOUND_LABEL_SE_hit000);


			player->HP -= 100;
			if (player->HP <= 0)
			{
				SetFade(FADE_OUT, MODE_RESULT);
			}

		}


	}


	for (int i = 0; i < MAX_DRAGON; i++)
	{

		//敵の有効フラグをチェックする
		if (dragon[i].use == FALSE)
			continue;

		//BCの当たり判定
		if (CollisionBC(player->pos, dragon[i].pos, player->size, dragon[i].size))
		{

			dragon[i].use = FALSE;

			//SE
			PlaySound(SOUND_LABEL_SE_hit000);


			player->HP -= 100;
			if (player->HP <= 0)
			{
				SetFade(FADE_OUT, MODE_RESULT);
			}

		}


	}


	for (int i = 0; i < MAX_EBULLET; i++)
	{

		//敵の有効フラグをチェックする
		if (ebullet[i].bUse == FALSE)
			continue;

		//BCの当たり判定
		if (CollisionBC(ebullet[i].pos, player->pos, 10.0f, player->size))
		{

			ebullet[i].bUse = FALSE;
			ReleaseShadow(ebullet[i].nIdxShadow);

			//SE
			PlaySound(SOUND_LABEL_SE_hit000);


			player->HP -= 100;
			if (player->HP <= 0)
			{
				SetFade(FADE_OUT, MODE_RESULT);
			}

		}


	}


	for (int i = 0; i < MAX_TOWER; i++)
	{
		//敵の有効フラグをチェックする
		if (tower[i].use == FALSE)
			continue;

		//BCの当たり判定
		if (CollisionBC(player->pos, tower[i].pos, player->size, tower[i].size))
		{
			// 当たったから未使用に戻す
			tower[i].use = FALSE;
			ReleaseShadow(tower[i].shadowIdx);


			// スコアを足す
			AddScore(10);

		}
	}






	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		//弾の有効フラグをチェックする
		if (particle[i].bUse == FALSE)
			continue;

		// 敵と当たってるか調べる
		for (int j = 0; j < MAX_TOWER; j++)
		{
			//敵の有効フラグをチェックする
			if (tower[j].use == FALSE)
				continue;

			//BCの当たり判定
			if (CollisionBC(particle[i].pos, tower[j].pos, 10.0f, tower[j].size))
			{
				// 当たったから未使用に戻す
				particle[i].bUse = FALSE;
				ReleaseShadow(particle[i].nIdxShadow);

				// 敵キャラクターは倒される
				tower[j].use = FALSE;
				ReleaseShadow(tower[j].shadowIdx);

				// スコアを足す
				AddScore(50);

			}
		}

	}

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		//弾の有効フラグをチェックする
		if (particle[i].bUse == FALSE)
			continue;

		// 敵と当たってるか調べる
		for (int j = 0; j < MAX_MORPHIN; j++)
		{
			//敵の有効フラグをチェックする
			if (morphing[j].use == FALSE)
				continue;

			//BCの当たり判定
			if (CollisionBC(particle[i].pos, morphing[j].pos, 10.0f, morphing[j].size))
			{
				// 当たったから未使用に戻す
				particle[i].bUse = FALSE;
				ReleaseShadow(particle[i].nIdxShadow);

				// 敵キャラクターは倒される
				morphing[j].use = FALSE;

				// スコアを足す
				AddScore(100);

			}
		}

	}

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		//弾の有効フラグをチェックする
		if (particle[i].bUse == FALSE)
			continue;

		// 敵と当たってるか調べる
		for (int j = 0; j < MAX_DRAGON; j++)
		{
			//敵の有効フラグをチェックする
			if (dragon[j].use == FALSE)
				continue;

			//BCの当たり判定
			if (CollisionBC(particle[i].pos, dragon[j].pos, 10.0f, dragon[j].size))
			{
				// 当たったから未使用に戻す
				particle[i].bUse = FALSE;
				ReleaseShadow(particle[i].nIdxShadow);

				// 敵キャラクターは倒される
				dragon[j].use = FALSE;

				// スコアを足す
				AddScore(100);

			}
		}

	}

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		//弾の有効フラグをチェックする
		if (particle[i].bUse == FALSE)
			continue;

		// 敵と当たってるか調べる
		for (int j = 0; j < MAX_CANNON; j++)
		{
			//敵の有効フラグをチェックする
			if (cannon[j].use == FALSE)
				continue;

			//BCの当たり判定
			if (CollisionBC(particle[i].pos, cannon[j].pos, 10.0f, cannon[j].size))
			{
				// 当たったから未使用に戻す
				particle[i].bUse = FALSE;
				ReleaseShadow(particle[i].nIdxShadow);

				// 敵キャラクターは倒される
				cannon[j].use = FALSE;

				// スコアを足す
				AddScore(100);

			}
		}

	}


	// towerが全部死亡したら状態遷移
	int tower_count = 0;
	for (int i = 0; i < MAX_TOWER; i++)
	{
		if (tower[i].use == FALSE) continue;
		tower_count++;
	}


	if (tower_count <= 0)

	{
		//SE
		PlaySound(SOUND_LABEL_SE_gamelvlcomplete);

		SetFade(FADE_OUT, MODE_RESULT);
	}



}


