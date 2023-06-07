//=============================================================================
//
// �t�@�C��	: �G�l�~�[���f������ [enemy.cpp]
// Author	: �����@GP 006 AUNG PYAE SAN
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
// �}�N����`
//*****************************************************************************
#define	MODEL_ENEMY			"data/MODEL/wall4.obj"		// �ǂݍ��ރ��f����
#define	MODEL_ENEMY2		"data/MODEL/wall5.obj"		// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(5.0f)						// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// ��]��

#define ENEMY_SHADOW_SIZE	(0.4f)						// �e�̑傫��
#define ENEMY_OFFSET_Y		(1.0f)						// �G�l�~�[�̑��������킹��

#define ENEMY_PARTS_MAX	(2)								// �v���C���[�̃p�[�c�̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ENEMY			g_Enemy[MAX_ENEMY];				// �G�l�~�[

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
// ����������
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

		g_Enemy[i].spd = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Enemy[i].size = ENEMY_SIZE;	// �����蔻��̑傫��

		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_Enemy[0].model, &g_Enemy[0].diffuse[0]);

		XMFLOAT3 pos = g_Enemy[i].pos;
		pos.y -= (ENEMY_OFFSET_Y - 0.1f);
		g_Enemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);

		g_Enemy[i].move_time = 0.0f;	// ���`��ԗp�̃^�C�}�[���N���A
		g_Enemy[i].tbl_adr = NULL;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		g_Enemy[i].tbl_size = 0;		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		g_Enemy[i].use = TRUE;			// TRUE:�����Ă�

	}


	// 0�Ԃ������`��Ԃœ������Ă݂�
	g_Enemy[0].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[0].tbl_adr = move_tbl;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[0].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


	g_Enemy[1].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[1].tbl_adr = move_tb2;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[1].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[2].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[2].tbl_adr = move_tb3;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[2].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[3].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[3].tbl_adr = move_tb4;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[3].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[4].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[4].tbl_adr = move_tb5;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[4].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[5].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[5].tbl_adr = move_tb6;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[5].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[6].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[6].tbl_adr = move_tb7;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[6].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[7].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[7].tbl_adr = move_tb8;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[7].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[8].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[8].tbl_adr = move_tb9;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[8].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[9].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[9].tbl_adr = move_tb10;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[9].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[10].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[10].tbl_adr = move_tb11;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[10].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[11].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[11].tbl_adr = move_tb12;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[11].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[12].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[12].tbl_adr = move_tb13;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[12].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[13].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[13].tbl_adr = move_tb14;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[13].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[14].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[14].tbl_adr = move_tb15;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[14].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[15].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[15].tbl_adr = move_tb16;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[15].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[16].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[16].tbl_adr = move_tb17;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[16].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[17].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[17].tbl_adr = move_tb18;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[17].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[18].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[18].tbl_adr = move_tb19;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[18].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[19].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[19].tbl_adr = move_tb20;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[19].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[20].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[20].tbl_adr = move_tb21;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[20].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[21].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[21].tbl_adr = move_tb22;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[21].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	LoadModel(MODEL_ENEMY2, &g_Enemy[21].model);

	g_Enemy[22].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[22].tbl_adr = move_tb23;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[22].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	LoadModel(MODEL_ENEMY2, &g_Enemy[22].model);


	g_Enemy[23].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[23].tbl_adr = move_tb24;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[23].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	LoadModel(MODEL_ENEMY2, &g_Enemy[23].model);


	g_Enemy[24].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[24].tbl_adr = move_tb25;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[24].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	LoadModel(MODEL_ENEMY2, &g_Enemy[24].model);

	g_Enemy[26].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[26].tbl_adr = move_tb27;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[26].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	LoadModel(MODEL_ENEMY2, &g_Enemy[26].model);

	g_Enemy[27].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[27].tbl_adr = move_tb28;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[27].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	LoadModel(MODEL_ENEMY2, &g_Enemy[27].model);

	g_Enemy[28].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[28].tbl_adr = move_tb29;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[28].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	LoadModel(MODEL_ENEMY2, &g_Enemy[28].model);

	g_Enemy[29].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[29].tbl_adr = move_tb30;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[29].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[30].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[30].tbl_adr = move_tb31;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[30].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	LoadModel(MODEL_ENEMY2, &g_Enemy[30].model);

	g_Enemy[31].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[31].tbl_adr = move_tb32;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[31].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[32].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[32].tbl_adr = move_tb33;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[32].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[33].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[33].tbl_adr = move_tb34;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[33].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	LoadModel(MODEL_ENEMY2, &g_Enemy[33].model);

	g_Enemy[34].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[34].tbl_adr = move_tb35;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[34].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[35].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[35].tbl_adr = move_tb36;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[35].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	LoadModel(MODEL_ENEMY2, &g_Enemy[35].model);

	g_Enemy[36].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[36].tbl_adr = move_tb37;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[36].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[37].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[37].tbl_adr = move_tb38;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[37].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[38].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[38].tbl_adr = move_tb39;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[38].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[39].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[39].tbl_adr = move_tb40;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[39].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	LoadModel(MODEL_ENEMY2, &g_Enemy[39].model);

	g_Enemy[40].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[40].tbl_adr = move_tb41;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[40].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	LoadModel(MODEL_ENEMY2, &g_Enemy[40].model);

	g_Enemy[41].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[41].tbl_adr = move_tb26;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[41].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	LoadModel(MODEL_ENEMY2, &g_Enemy[41].model);

	g_Enemy[42].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[42].tbl_adr = move_tb42;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[42].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[43].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[43].tbl_adr = move_tb43;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[43].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[44].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[44].tbl_adr = move_tb44;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[44].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[45].move_time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[45].tbl_adr = move_tb45;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[45].tbl_size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
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
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	// �G�l�~�[�𓮂����ꍇ�́A�e�����킹�ē���������Y��Ȃ��悤�ɂˁI
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == TRUE)			// ���̃G�l�~�[���g���Ă���H
		{									// Yes
			if (g_Enemy[i].tbl_adr != NULL)	// ���`��Ԃ����s����H
			{								// ���`��Ԃ̏���
				// �ړ�����
				int		index = (int)g_Enemy[i].move_time;
				float	time = g_Enemy[i].move_time - index;
				int		size = g_Enemy[i].tbl_size;

				float dt = 1.0f / g_Enemy[i].tbl_adr[index].frame;	// 1�t���[���Ői�߂鎞��
				g_Enemy[i].move_time += dt;							// �A�j���[�V�����̍��v���Ԃɑ���

				if (index > (size - 2))	// �S�[�����I�[�o�[���Ă�����A�ŏ��֖߂�
				{
					g_Enemy[i].move_time = 0.0f;
					index = 0;
				}

				// ���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
				XMVECTOR p1 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 1].pos);	// ���̏ꏊ
				XMVECTOR p0 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 0].pos);	// ���݂̏ꏊ
				XMVECTOR vec = p1 - p0;
				XMStoreFloat3(&g_Enemy[i].pos, p0 + vec * time);

				// ��]�����߂�	R = StartX + (EndX - StartX) * ���̎���
				XMVECTOR r1 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 1].rot);	// ���̊p�x
				XMVECTOR r0 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 0].rot);	// ���݂̊p�x
				XMVECTOR rot = r1 - r0;
				XMStoreFloat3(&g_Enemy[i].rot, r0 + rot * time);

				// scale�����߂� S = StartX + (EndX - StartX) * ���̎���
				XMVECTOR s1 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 1].scl);	// ����Scale
				XMVECTOR s0 = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 0].scl);	// ���݂�Scale
				XMVECTOR scl = s1 - s0;
				XMStoreFloat3(&g_Enemy[i].scl, s0 + scl * time);

			}

			// �e���v���C���[�̈ʒu�ɍ��킹��
			XMFLOAT3 pos = g_Enemy[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			SetPositionShadow(g_Enemy[i].shadowIdx, pos);
		}
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == FALSE) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_Enemy[i].model);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
ENEMY *GetEnemy()
{
	return &g_Enemy[0];
}
