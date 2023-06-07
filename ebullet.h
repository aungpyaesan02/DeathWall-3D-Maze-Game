//=============================================================================
//
// �t�@�C��	: �p�[�e�B�N������ [particle.h]
// Author	: �����@GP 006 AUNG PYAE SAN
//
//=============================================================================
#pragma once

#define	MAX_EBULLET		(2)			// �p�[�e�B�N���ő吔

typedef struct
{
	XMFLOAT3		pos;			// �ʒu
	XMFLOAT3		playerpos;
	XMFLOAT3		rot;			// ��]
	XMFLOAT3		scale;			// �X�P�[��
	XMFLOAT3		move;			// �ړ���
	MATERIAL		material;		// �}�e���A��
	float			fSizeX;			// ��
	float			fSizeY;			// ����
	float			spd;
	int				nIdxShadow;		// �eID
	int				nLife;			// ����

	BOOL			bUse;			// �g�p���Ă��邩�ǂ���

} EBULLET;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEBullet(void);
void UninitEBullet(void);
void UpdateEBullet(void);
void DrawEBullet(void);

int SetEBullet(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife);
void SetColorEBullet(int nIdxParticle, XMFLOAT4 col);
EBULLET *GetEBullet(void);

