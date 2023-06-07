//=============================================================================
//
// ファイル	: エネミーモデル処理 [enemy.cpp]
// Author	: 東京　GP 006 AUNG PYAE SAN
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "model.h"
#include "enemy.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_ENEMY			"data/MODEL/wall4.obj"		// 読み込むモデル名
#define	MODEL_ENEMY2		"data/MODEL/wall5.obj"		// 読み込むモデル名

#define	VALUE_MOVE			(5.0f)						// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// 回転量

#define ENEMY_SHADOW_SIZE	(0.4f)						// 影の大きさ
#define ENEMY_OFFSET_Y		(1.0f)						// エネミーの足元をあわせる

#define ENEMY_PARTS_MAX	(2)								// プレイヤーのパーツの数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ENEMY			g_Enemy[MAX_ENEMY];				// エネミー

static BOOL				g_Load = FALSE;


static INTERPOLATION_DATA move_tbl[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-300.0f, ENEMY_OFFSET_Y,  20.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60*2 },
	{ XMFLOAT3(-100.0f, ENEMY_OFFSET_Y,  20.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60*2 },
	{ XMFLOAT3(-300.0f, ENEMY_OFFSET_Y,  20.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60*2 },

};

static INTERPOLATION_DATA move_tb2[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-300.0f, ENEMY_OFFSET_Y, -167.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-100.0f, ENEMY_OFFSET_Y, -167.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-300.0f, ENEMY_OFFSET_Y, -167.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};


static INTERPOLATION_DATA move_tb3[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-300.0f, ENEMY_OFFSET_Y, -240.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-100.0f, ENEMY_OFFSET_Y, -240.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-300.0f, ENEMY_OFFSET_Y, -240.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb4[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-500.0f, ENEMY_OFFSET_Y, -81.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-300.0f, ENEMY_OFFSET_Y, -81.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-500.0f, ENEMY_OFFSET_Y, -81.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb5[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-600.0f, ENEMY_OFFSET_Y, 200.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-150.0f, ENEMY_OFFSET_Y, 200.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-600.0f, ENEMY_OFFSET_Y, 200.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb6[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-500.0f, ENEMY_OFFSET_Y, -317.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-200.0f, ENEMY_OFFSET_Y, -317.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-500.0f, ENEMY_OFFSET_Y, -317.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};



static INTERPOLATION_DATA move_tb7[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-200.0f, ENEMY_OFFSET_Y, 350.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(  14.0f, ENEMY_OFFSET_Y, 350.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-200.0f, ENEMY_OFFSET_Y, 350.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb8[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-100.0f, ENEMY_OFFSET_Y, 500.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 100.0f, ENEMY_OFFSET_Y, 500.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-100.0f, ENEMY_OFFSET_Y, 500.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb9[] = {	// pos, rot, scl, frame
	{ XMFLOAT3( 150.0f, ENEMY_OFFSET_Y, 300.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 350.0f, ENEMY_OFFSET_Y, 300.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 150.0f, ENEMY_OFFSET_Y, 300.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb10[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(  10.0f, ENEMY_OFFSET_Y, 100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 126.0f, ENEMY_OFFSET_Y, 100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(  10.0f, ENEMY_OFFSET_Y, 100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};
//
static INTERPOLATION_DATA move_tb11[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(   5.0f, ENEMY_OFFSET_Y, -100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 150.0f, ENEMY_OFFSET_Y, -100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(   5.0f, ENEMY_OFFSET_Y, -100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};
//
static INTERPOLATION_DATA move_tb12[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-100.0f, ENEMY_OFFSET_Y, -282.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(  30.0f, ENEMY_OFFSET_Y, -282.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-100.0f, ENEMY_OFFSET_Y, -282.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};
//
static INTERPOLATION_DATA move_tb13[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-100.0f, ENEMY_OFFSET_Y, -500.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(  50.0f, ENEMY_OFFSET_Y, -500.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-100.0f, ENEMY_OFFSET_Y, -500.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};
//
static INTERPOLATION_DATA move_tb14[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(  40.0f, ENEMY_OFFSET_Y, -600.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 120.0f, ENEMY_OFFSET_Y, -600.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(  40.0f, ENEMY_OFFSET_Y, -600.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};
//
static INTERPOLATION_DATA move_tb15[] = {	// pos, rot, scl, frame
	{ XMFLOAT3( 130.0f, ENEMY_OFFSET_Y, -360.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 230.0f, ENEMY_OFFSET_Y, -360.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 130.0f, ENEMY_OFFSET_Y, -360.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};
////
static INTERPOLATION_DATA move_tb16[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(200.0f, ENEMY_OFFSET_Y, -200.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(350.0f, ENEMY_OFFSET_Y, -200.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(200.0f, ENEMY_OFFSET_Y, -200.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};
//
static INTERPOLATION_DATA move_tb17[] = {	// pos, rot, scl, frame
	{ XMFLOAT3( 200.0f, ENEMY_OFFSET_Y,   5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 330.0f, ENEMY_OFFSET_Y,   5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 200.0f, ENEMY_OFFSET_Y,   5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};
//
static INTERPOLATION_DATA move_tb18[] = {	// pos, rot, scl, frame
	{ XMFLOAT3( 200.0f, ENEMY_OFFSET_Y, 180.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 350.0f, ENEMY_OFFSET_Y, 180.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 200.0f, ENEMY_OFFSET_Y, 180.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};
//
static INTERPOLATION_DATA move_tb19[] = {	// pos, rot, scl, frame
	{ XMFLOAT3( 150.0f, ENEMY_OFFSET_Y, 419.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 300.0f, ENEMY_OFFSET_Y, 419.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 150.0f, ENEMY_OFFSET_Y, 419.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};
//
static INTERPOLATION_DATA move_tb20[] = {	// pos, rot, scl, frame
	{ XMFLOAT3( 250.0f, ENEMY_OFFSET_Y,-472.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 400.0f, ENEMY_OFFSET_Y,-472.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 250.0f, ENEMY_OFFSET_Y,-472.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

//besidewall
static INTERPOLATION_DATA move_tb21[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-543.0f, ENEMY_OFFSET_Y, -380.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-543.0f, ENEMY_OFFSET_Y, -500.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-543.0f, ENEMY_OFFSET_Y, -380.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb22[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-165.0f, ENEMY_OFFSET_Y, -362.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-165.0f, ENEMY_OFFSET_Y, -450.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-165.0f, ENEMY_OFFSET_Y, -362.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb23[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-421.0f, ENEMY_OFFSET_Y, -44.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-421.0f, ENEMY_OFFSET_Y, 147.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-421.0f, ENEMY_OFFSET_Y, -44.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb24[] = {	// pos, rot, scl, frame
	{ XMFLOAT3( 350.0f, ENEMY_OFFSET_Y,   80.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 350.0f, ENEMY_OFFSET_Y,  280.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 350.0f, ENEMY_OFFSET_Y,   80.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb25[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-295.0f, ENEMY_OFFSET_Y,  530.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-295.0f, ENEMY_OFFSET_Y,  380.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-295.0f, ENEMY_OFFSET_Y,  530.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb26[] = {	// pos, rot, scl, frame
	{ XMFLOAT3( 422.0f, ENEMY_OFFSET_Y,  600.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 422.0f, ENEMY_OFFSET_Y,  400.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 422.0f, ENEMY_OFFSET_Y,  600.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb27[] = {	// pos, rot, scl, frame
	{ XMFLOAT3( 453.0f, ENEMY_OFFSET_Y,-420.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 453.0f, ENEMY_OFFSET_Y,-220.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 453.0f, ENEMY_OFFSET_Y,-420.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb28[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-364.0f, ENEMY_OFFSET_Y,-357.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-364.0f, ENEMY_OFFSET_Y,-565.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-364.0f, ENEMY_OFFSET_Y,-357.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb29[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-425.0f, ENEMY_OFFSET_Y,-292.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-425.0f, ENEMY_OFFSET_Y,-122.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-425.0f, ENEMY_OFFSET_Y,-292.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb30[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-587.0f, ENEMY_OFFSET_Y, 402.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 402.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-587.0f, ENEMY_OFFSET_Y, 402.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb31[] = {	// pos, rot, scl, frame
	{ XMFLOAT3( -77.0f, ENEMY_OFFSET_Y, 304.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( -77.0f, ENEMY_OFFSET_Y,  90.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( -77.0f, ENEMY_OFFSET_Y, 304.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb32[] = {	// pos, rot, scl, frame
	{ XMFLOAT3( 530.0f, ENEMY_OFFSET_Y, 393.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 360.0f, ENEMY_OFFSET_Y, 393.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 530.0f, ENEMY_OFFSET_Y, 393.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};
//
static INTERPOLATION_DATA move_tb33[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(400.0f, ENEMY_OFFSET_Y, 545.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(250.0f, ENEMY_OFFSET_Y, 545.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(400.0f, ENEMY_OFFSET_Y, 545.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb34[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(100.0f, ENEMY_OFFSET_Y, 430.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(100.0f, ENEMY_OFFSET_Y, 285.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(100.0f, ENEMY_OFFSET_Y, 430.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb35[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(500.0f, ENEMY_OFFSET_Y, -90.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(350.0f, ENEMY_OFFSET_Y, -90.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(500.0f, ENEMY_OFFSET_Y, -90.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb36[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(485.0f, ENEMY_OFFSET_Y, 330.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(485.0f, ENEMY_OFFSET_Y,  90.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(485.0f, ENEMY_OFFSET_Y, 330.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb37[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-300.0f, ENEMY_OFFSET_Y, 299.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-570.0f, ENEMY_OFFSET_Y, 299.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-300.0f, ENEMY_OFFSET_Y, 299.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};
//
static INTERPOLATION_DATA move_tb38[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-300.0f, ENEMY_OFFSET_Y, 567.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-570.0f, ENEMY_OFFSET_Y, 567.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-300.0f, ENEMY_OFFSET_Y, 567.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb39[] = {	// pos, rot, scl, frame
	{ XMFLOAT3( 500.0f, ENEMY_OFFSET_Y,  47.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 310.0f, ENEMY_OFFSET_Y,  47.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 500.0f, ENEMY_OFFSET_Y,  47.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb40[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(457.0f, ENEMY_OFFSET_Y,-600.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(457.0f, ENEMY_OFFSET_Y,-430.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(457.0f, ENEMY_OFFSET_Y,-600.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};


static INTERPOLATION_DATA move_tb41[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-169.0f, ENEMY_OFFSET_Y, 420.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-169.0f, ENEMY_OFFSET_Y, 600.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-169.0f, ENEMY_OFFSET_Y, 420.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb42[] = {	// pos, rot, scl, frame
	{ XMFLOAT3( 250.0f, ENEMY_OFFSET_Y,-331.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 500.0f, ENEMY_OFFSET_Y,-331.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 250.0f, ENEMY_OFFSET_Y,-331.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};
//
static INTERPOLATION_DATA move_tb43[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(620.0f, ENEMY_OFFSET_Y, 227.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(385.0f, ENEMY_OFFSET_Y, 227.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(620.0f, ENEMY_OFFSET_Y, 227.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb44[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-621.0f, ENEMY_OFFSET_Y,-201.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-460.0f, ENEMY_OFFSET_Y,-201.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-621.0f, ENEMY_OFFSET_Y,-201.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

static INTERPOLATION_DATA move_tb45[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-547.0f, ENEMY_OFFSET_Y, 148.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-547.0f, ENEMY_OFFSET_Y, -41.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-547.0f, ENEMY_OFFSET_Y, 148.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },

};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		LoadModel(MODEL_ENEMY, &g_Enemy[i].model);
		g_Enemy[i].load = TRUE;

		g_Enemy[i].pos = XMFLOAT3(-50.0f + i * 30.0f, ENEMY_OFFSET_Y, 20.0f);
		g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		g_Enemy[i].spd = 0.0f;			// 移動スピードクリア
		g_Enemy[i].size = ENEMY_SIZE;	// 当たり判定の大きさ

		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_Enemy[0].model, &g_Enemy[0].diffuse[0]);

		XMFLOAT3 pos = g_Enemy[i].pos;
		pos.y -= (ENEMY_OFFSET_Y - 0.1f);
		g_Enemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);

		g_Enemy[i].move_time = 0.0f;	// 線形補間用のタイマーをクリア
		g_Enemy[i].tbl_adr = NULL;		// 再生するアニメデータの先頭アドレスをセット
		g_Enemy[i].tbl_size = 0;		// 再生するアニメデータのレコード数をセット

		g_Enemy[i].use = TRUE;			// TRUE:生きてる

	}


	// 0番だけ線形補間で動かしてみる
	g_Enemy[0].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[0].tbl_adr = move_tbl;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[0].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット


	g_Enemy[1].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[1].tbl_adr = move_tb2;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[1].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[2].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[2].tbl_adr = move_tb3;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[2].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[3].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[3].tbl_adr = move_tb4;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[3].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[4].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[4].tbl_adr = move_tb5;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[4].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[5].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[5].tbl_adr = move_tb6;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[5].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[6].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[6].tbl_adr = move_tb7;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[6].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[7].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[7].tbl_adr = move_tb8;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[7].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[8].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[8].tbl_adr = move_tb9;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[8].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[9].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[9].tbl_adr = move_tb10;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[9].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[10].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[10].tbl_adr = move_tb11;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[10].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[11].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[11].tbl_adr = move_tb12;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[11].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[12].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[12].tbl_adr = move_tb13;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[12].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[13].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[13].tbl_adr = move_tb14;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[13].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[14].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[14].tbl_adr = move_tb15;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[14].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[15].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[15].tbl_adr = move_tb16;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[15].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[16].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[16].tbl_adr = move_tb17;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[16].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[17].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[17].tbl_adr = move_tb18;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[17].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[18].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[18].tbl_adr = move_tb19;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[18].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[19].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[19].tbl_adr = move_tb20;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[19].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[20].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[20].tbl_adr = move_tb21;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[20].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[21].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[21].tbl_adr = move_tb22;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[21].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	LoadModel(MODEL_ENEMY2, &g_Enemy[21].model);

	g_Enemy[22].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[22].tbl_adr = move_tb23;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[22].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	LoadModel(MODEL_ENEMY2, &g_Enemy[22].model);


	g_Enemy[23].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[23].tbl_adr = move_tb24;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[23].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	LoadModel(MODEL_ENEMY2, &g_Enemy[23].model);


	g_Enemy[24].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[24].tbl_adr = move_tb25;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[24].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	LoadModel(MODEL_ENEMY2, &g_Enemy[24].model);

	g_Enemy[26].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[26].tbl_adr = move_tb27;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[26].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	LoadModel(MODEL_ENEMY2, &g_Enemy[26].model);

	g_Enemy[27].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[27].tbl_adr = move_tb28;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[27].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	LoadModel(MODEL_ENEMY2, &g_Enemy[27].model);

	g_Enemy[28].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[28].tbl_adr = move_tb29;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[28].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	LoadModel(MODEL_ENEMY2, &g_Enemy[28].model);

	g_Enemy[29].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[29].tbl_adr = move_tb30;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[29].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[30].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[30].tbl_adr = move_tb31;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[30].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	LoadModel(MODEL_ENEMY2, &g_Enemy[30].model);

	g_Enemy[31].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[31].tbl_adr = move_tb32;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[31].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[32].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[32].tbl_adr = move_tb33;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[32].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[33].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[33].tbl_adr = move_tb34;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[33].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	LoadModel(MODEL_ENEMY2, &g_Enemy[33].model);

	g_Enemy[34].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[34].tbl_adr = move_tb35;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[34].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[35].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[35].tbl_adr = move_tb36;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[35].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	LoadModel(MODEL_ENEMY2, &g_Enemy[35].model);

	g_Enemy[36].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[36].tbl_adr = move_tb37;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[36].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[37].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[37].tbl_adr = move_tb38;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[37].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[38].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[38].tbl_adr = move_tb39;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[38].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[39].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[39].tbl_adr = move_tb40;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[39].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	LoadModel(MODEL_ENEMY2, &g_Enemy[39].model);

	g_Enemy[40].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[40].tbl_adr = move_tb41;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[40].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	LoadModel(MODEL_ENEMY2, &g_Enemy[40].model);

	g_Enemy[41].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[41].tbl_adr = move_tb26;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[41].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	LoadModel(MODEL_ENEMY2, &g_Enemy[41].model);

	g_Enemy[42].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[42].tbl_adr = move_tb42;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[42].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[43].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[43].tbl_adr = move_tb43;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[43].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[44].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[44].tbl_adr = move_tb44;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[44].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Enemy[45].move_time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[45].tbl_adr = move_tb45;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[45].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	if (g_Load == FALSE) return;

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].load)
		{
			UnloadModel(&g_Enemy[i].model);
			g_Enemy[i].load = FALSE;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	// エネミーを動かく場合は、影も合わせて動かす事を忘れないようにね！
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == TRUE)			// このエネミーが使われている？
		{									// Yes
			if (g_Enemy[i].tbl_adr != NULL)	// 線形補間を実行する？
			{								// 線形補間の処理
				// 移動処理
				int		index = (int)g_Enemy[i].move_time;
				float	time = g_Enemy[i].move_time - index;
				int		size = g_Enemy[i].tbl_size;

				float dt = 1.0f / g_Enemy[i].tbl_adr[index].frame;	// 1フレームで進める時間
				g_Enemy[i].move_time += dt;							// アニメーションの合計時間に足す

				if (index > (size - 2))	// ゴールをオーバーしていたら、最初へ戻す
				{
					g_Enemy[i].move_time = 0.0f;
					index = 0;
				}

				// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
				XMVECTOR p1 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 1].pos);	// 次の場所
				XMVECTOR p0 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 0].pos);	// 現在の場所
				XMVECTOR vec = p1 - p0;
				XMStoreFloat3(&g_Enemy[i].pos, p0 + vec * time);

				// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
				XMVECTOR r1 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 1].rot);	// 次の角度
				XMVECTOR r0 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 0].rot);	// 現在の角度
				XMVECTOR rot = r1 - r0;
				XMStoreFloat3(&g_Enemy[i].rot, r0 + rot * time);

				// scaleを求める S = StartX + (EndX - StartX) * 今の時間
				XMVECTOR s1 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 1].scl);	// 次のScale
				XMVECTOR s0 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 0].scl);	// 現在のScale
				XMVECTOR scl = s1 - s0;
				XMStoreFloat3(&g_Enemy[i].scl, s0 + scl * time);

			}

			// 影もプレイヤーの位置に合わせる
			XMFLOAT3 pos = g_Enemy[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			SetPositionShadow(g_Enemy[i].shadowIdx, pos);
		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == FALSE) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);


		// モデル描画
		DrawModel(&g_Enemy[i].model);
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// エネミーの取得
//=============================================================================
ENEMY *GetEnemy()
{
	return &g_Enemy[0];
}
