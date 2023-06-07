//=============================================================================
//
// �t�@�C��	: ���[�t�B���O���� [dragon.cpp]
// Author	: �����@GP 006 AUNG PYAE SAN
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_MAX	(3)
#define MAX_DRAGON		(4)					// dragon�[�̐�
#define MORPHIN_SIZE	(5.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct DRAGON
{
	BOOL			load;						// ���[�h�����t���O
	BOOL			use;
	float			time;						// ���`��ԗp�̃^�C�}�[
	float			size;						// �����蔻��̑傫��

	int				sign;						// ���`��ԗp�̕���
	int				index;						// ���`��ԗp�̃��f���ԍ�
	int				shadowIdx;					// �e�̃C���f�b�N�X�ԍ�

	DX11_MODEL		model;						// ���f���f�[�^

	XMFLOAT3		pos;						// ���W
	XMFLOAT3		rot;						// ��]
	XMFLOAT3		scl;						// �g�k
	XMFLOAT4X4		mtxWorld;					// ���[���h�}�g���b�N�X


	// padding 4byte

	XMFLOAT4	diffuse[MODEL_MAX_MATERIAL];	// �F

	// 344�o�C�g
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitDragon(void);
void UninitDragon(void);
void UpdateDragon(void);
void DrawDragon(void);

DRAGON *GetDragon(void);
