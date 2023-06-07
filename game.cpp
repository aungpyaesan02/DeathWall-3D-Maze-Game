//=============================================================================
//
// �t�@�C��	: �Q�[����ʏ��� [game.cpp]
// Author	: �����@GP 006 AUNG PYAE SAN
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
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void CheckHit(void);



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int	g_ViewPortType_Game = TYPE_FULL_SCREEN;

static BOOL	g_bPause = TRUE;	// �|�[�YON/OFF


//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{
	g_ViewPortType_Game = TYPE_FULL_SCREEN;

	InitSkybox();

	// �t�B�[���h�̏�����
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 100, 100, 13.0f, 13.0f);

	// ���C�g��L����	// �e�̏���������
	InitShadow();

	// �v���C���[�̏�����
	InitPlayer();

	// �G�l�~�[�̏�����
	InitEnemy();

	// Tower�̏�����
	InitTower();

	// ���[�t�B���O�̏�����
	InitMorphing();

	// ���[�t�B���O�̏�����
	InitDragon();

	// ���[�t�B���O�̏�����
	InitCannon();


	// �ǂ̏�����
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f,  XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);

	// ��(�����p�̔�����)
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f,    XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f,   XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);

	// �؂𐶂₷
	InitTree();


	// �X�R�A�̏�����
	InitScore();

	InitVibration();

	InitUI();

	// �p�[�e�B�N���̏�����
	InitParticle();

	// �p�[�e�B�N���̏�����
	InitParticle2();

	// �e�̏�����
	InitEBullet();



	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
	// �e�̏I������
	UninitEBullet();

	// �p�[�e�B�N���̏I������
	UninitParticle2();

	// �p�[�e�B�N���̏I������
	UninitParticle();

	// �X�R�A�̏I������
	UninitScore();

	UninitVibration();

	UninitUI();


	// �؂̏I������
	UninitTree();

	// �ǂ̏I������
	UninitMeshWall();

	// �n�ʂ̏I������
	UninitMeshField();

	UninitSkybox();

	// �G�l�~�[�̏I������
	UninitEnemy();

	// TOWER�̏I������
	UninitTower();


	// ���[�t�B���O�̏I������
	UninitMorphing();

	// ���[�t�B���O�̏I������
	UninitDragon();


	// ���[�t�B���O�̏I������
	UninitCannon();


	// �v���C���[�̏I������
	UninitPlayer();

	// �e�̏I������
	UninitShadow();

}

//=============================================================================
// �X�V����
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

	// �n�ʏ����̍X�V
	UpdateMeshField();

	// �v���C���[�̍X�V����
	UpdatePlayer();

	// �G�l�~�[�̍X�V����
	UpdateEnemy();

	// Tower�̍X�V����
	UpdateTower();

	// ���[�t�B���O�̍X�V����
	UpdateMorphing();

	// ���[�t�B���O�̍X�V����
	UpdateDragon();


	// ���[�t�B���O�̍X�V����
	UpdateCannon();


	// �Ǐ����̍X�V
	UpdateMeshWall();

	// �؂̍X�V����
	UpdateTree();


	// �p�[�e�B�N���̍X�V����
	UpdateParticle();

	// �p�[�e�B�N���̍X�V����
	UpdateParticle2();


	// �e�̍X�V����
	UpdateEBullet();


	// �e�̍X�V����
	UpdateShadow();

	// �����蔻�菈��
	CheckHit();

	// �X�R�A�̍X�V����
	UpdateScore();

	UpdateVibration();

	UpdateUI();

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame0(void)
{
	// 3D�̕���`�悷�鏈��
	// �n�ʂ̕`�揈��

	DrawSkybox();

	DrawMeshField();

	// ���[�t�B���O�̕`�揈��
	DrawMorphing();

	// ���[�t�B���O�̕`�揈��
	DrawDragon();

	// ���[�t�B���O�̕`�揈��
	DrawCannon();

	// �G�l�~�[�̕`�揈��
	DrawEnemy();

	// Tower�̕`�揈��
	DrawTower();


	// �v���C���[�̕`�揈��
	DrawPlayer();

	// �e�̕`�揈��
	DrawEBullet();

	// �ǂ̕`�揈��
	DrawMeshWall();

	// �؂̕`�揈��
	DrawTree();

	// �p�[�e�B�N���̕`�揈��
	DrawParticle();

	// �p�[�e�B�N���̕`�揈��
	DrawParticle2();

	// 2D�̕���`�悷�鏈��
	// Z��r�Ȃ�
	SetDepthEnable(FALSE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);

	// �X�R�A�̕`�揈��
	DrawScore();

	DrawVibration();

	DrawUI();


	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// Z��r����
	SetDepthEnable(TRUE);


}


void DrawGame(void)
{
	XMFLOAT3 pos;


#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Game);

#endif

	// �v���C���[���_
	pos = GetPlayer()->pos;
	pos.y = 30.0f;			// �J����������h�����߂ɃN���A���Ă���
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

		// �G�l�~�[���_
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

		// �G�l�~�[���_
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
// �����蔻�菈��
//=============================================================================
void CheckHit(void)
{
	ENEMY *enemy   = GetEnemy();	// �G�l�~�[�̃|�C���^�[��������
	PLAYER *player = GetPlayer();	// �v���C���[�̃|�C���^�[��������
	EBULLET *ebullet = GetEBullet();	// �e�̃|�C���^�[��������
	TOWER  *tower  = GetTower();	//Tower�̃|�C���^�[��������
	UI     *ui     = GetUI();	    //UI�̃|�C���^�[��������
	PARTICLE *particle = GetParticle();
	MORPHING	*morphing = GetMorphing();
	DRAGON		*dragon = GetDragon();
	CANNON		*cannon = GetCannon();

	// �G�ƃv���C���[�L����
	for (int i = 0; i < MAX_ENEMY; i++)
	{

			//�G�̗L���t���O���`�F�b�N����
			if (enemy[i].use == FALSE)
				continue;

				//BC�̓����蔻��
				if (CollisionBC(player->pos, enemy[i].pos, player->size, enemy[i].size))
				{
					// �G�L�����N�^�[�͓|�����
					SetVibration();

					//SE
					PlaySound(SOUND_LABEL_SE_hugeexplosion);

					SetFade(FADE_OUT, MODE_RESULT);

				}

			
	}

	for (int i = 0; i < MAX_MORPHIN; i++)
	{

		//�G�̗L���t���O���`�F�b�N����
		if (morphing[i].use == FALSE)
			continue;

		//BC�̓����蔻��
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

		//�G�̗L���t���O���`�F�b�N����
		if (dragon[i].use == FALSE)
			continue;

		//BC�̓����蔻��
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

		//�G�̗L���t���O���`�F�b�N����
		if (ebullet[i].bUse == FALSE)
			continue;

		//BC�̓����蔻��
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
		//�G�̗L���t���O���`�F�b�N����
		if (tower[i].use == FALSE)
			continue;

		//BC�̓����蔻��
		if (CollisionBC(player->pos, tower[i].pos, player->size, tower[i].size))
		{
			// �����������疢�g�p�ɖ߂�
			tower[i].use = FALSE;
			ReleaseShadow(tower[i].shadowIdx);


			// �X�R�A�𑫂�
			AddScore(10);

		}
	}






	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		//�e�̗L���t���O���`�F�b�N����
		if (particle[i].bUse == FALSE)
			continue;

		// �G�Ɠ������Ă邩���ׂ�
		for (int j = 0; j < MAX_TOWER; j++)
		{
			//�G�̗L���t���O���`�F�b�N����
			if (tower[j].use == FALSE)
				continue;

			//BC�̓����蔻��
			if (CollisionBC(particle[i].pos, tower[j].pos, 10.0f, tower[j].size))
			{
				// �����������疢�g�p�ɖ߂�
				particle[i].bUse = FALSE;
				ReleaseShadow(particle[i].nIdxShadow);

				// �G�L�����N�^�[�͓|�����
				tower[j].use = FALSE;
				ReleaseShadow(tower[j].shadowIdx);

				// �X�R�A�𑫂�
				AddScore(50);

			}
		}

	}

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		//�e�̗L���t���O���`�F�b�N����
		if (particle[i].bUse == FALSE)
			continue;

		// �G�Ɠ������Ă邩���ׂ�
		for (int j = 0; j < MAX_MORPHIN; j++)
		{
			//�G�̗L���t���O���`�F�b�N����
			if (morphing[j].use == FALSE)
				continue;

			//BC�̓����蔻��
			if (CollisionBC(particle[i].pos, morphing[j].pos, 10.0f, morphing[j].size))
			{
				// �����������疢�g�p�ɖ߂�
				particle[i].bUse = FALSE;
				ReleaseShadow(particle[i].nIdxShadow);

				// �G�L�����N�^�[�͓|�����
				morphing[j].use = FALSE;

				// �X�R�A�𑫂�
				AddScore(100);

			}
		}

	}

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		//�e�̗L���t���O���`�F�b�N����
		if (particle[i].bUse == FALSE)
			continue;

		// �G�Ɠ������Ă邩���ׂ�
		for (int j = 0; j < MAX_DRAGON; j++)
		{
			//�G�̗L���t���O���`�F�b�N����
			if (dragon[j].use == FALSE)
				continue;

			//BC�̓����蔻��
			if (CollisionBC(particle[i].pos, dragon[j].pos, 10.0f, dragon[j].size))
			{
				// �����������疢�g�p�ɖ߂�
				particle[i].bUse = FALSE;
				ReleaseShadow(particle[i].nIdxShadow);

				// �G�L�����N�^�[�͓|�����
				dragon[j].use = FALSE;

				// �X�R�A�𑫂�
				AddScore(100);

			}
		}

	}

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		//�e�̗L���t���O���`�F�b�N����
		if (particle[i].bUse == FALSE)
			continue;

		// �G�Ɠ������Ă邩���ׂ�
		for (int j = 0; j < MAX_CANNON; j++)
		{
			//�G�̗L���t���O���`�F�b�N����
			if (cannon[j].use == FALSE)
				continue;

			//BC�̓����蔻��
			if (CollisionBC(particle[i].pos, cannon[j].pos, 10.0f, cannon[j].size))
			{
				// �����������疢�g�p�ɖ߂�
				particle[i].bUse = FALSE;
				ReleaseShadow(particle[i].nIdxShadow);

				// �G�L�����N�^�[�͓|�����
				cannon[j].use = FALSE;

				// �X�R�A�𑫂�
				AddScore(100);

			}
		}

	}


	// tower���S�����S�������ԑJ��
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


