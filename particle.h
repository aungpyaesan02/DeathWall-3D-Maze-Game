//=============================================================================
//
// �t�@�C��	: �p�[�e�B�N������ [particle.h]
// Author	: �����@GP 006 AUNG PYAE SAN
//
//=============================================================================
#pragma once

#define	MAX_PARTICLE		(10)	// �p�[�e�B�N���ő吔

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

} PARTICLE;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

int SetParticle(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife);
void SetColorParticle(int nIdxParticle, XMFLOAT4 col);
PARTICLE *GetParticle(void);

