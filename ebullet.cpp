//=============================================================================
//
// �t�@�C��	: �p�[�e�B�N������ [ebullet.cpp]
// Author	: �����@GP 006 AUNG PYAE SAN
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
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX			(1)			// �e�N�X�`���̐�

#define	PARTICLE_SIZE_X		(30.0f)		// ���_�T�C�Y
#define	PARTICLE_SIZE_Y		(30.0f)		// ���_�T�C�Y
#define	VALUE_MOVE_PARTICLE	(5.0f)		// �ړ����x
#define BULLER_DISTANCE		(100)

#define	DISP_SHADOW						// �e�̕\��
//#undef DISP_SHADOW

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexEBullet(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;				// ���_�o�b�t�@

static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static int							g_TexNo;							// �e�N�X�`���ԍ�

static EBULLET					g_EBullet[MAX_EBULLET];					// �p�[�e�B�N�����[�N
static XMFLOAT3					g_posBase;								// �r���{�[�h�����ʒu
static float					g_fWidthBase = 5.0f;					// ��̕�
static float					g_fHeightBase = 10.0f;					// ��̍���
static float					g_roty = 0.0f;							// �ړ�����
static float					g_spd = 0.0f;							// �ړ��X�s�[�h

static char *g_TextureName[TEXTURE_MAX] =
{
	"data/TEXTURE/power.jpg",
};

static BOOL						g_Load = FALSE;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEBullet(void)
{
	// ���_���̍쐬
	MakeVertexEBullet();

	// �e�N�X�`������
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

	// �p�[�e�B�N�����[�N�̏�����
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
// �I������
//=============================================================================
void UninitEBullet(void)
{
	if (g_Load == FALSE) return;

	//�e�N�X�`���̉��
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (g_Texture[nCntTex] != NULL)
		{
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̉��
	if (g_VertexBuffer != NULL)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEBullet(void)
{

	{
		for(int nEBullet = 0; nEBullet < MAX_EBULLET; nEBullet++)
		{
			if(g_EBullet[nEBullet].bUse)
			{
				// �e�̈ړ�����
				g_EBullet[nEBullet].pos.x -= sinf(g_EBullet[nEBullet].rot.y) * g_EBullet[nEBullet].spd;
				g_EBullet[nEBullet].pos.z -= cosf(g_EBullet[nEBullet].rot.y) * g_EBullet[nEBullet].spd;

				// �e�̈ʒu�ݒ�
				SetPositionShadow(g_EBullet[nEBullet].nIdxShadow, XMFLOAT3(g_EBullet[nEBullet].pos.x, 0.1f, g_EBullet[nEBullet].pos.z));


				// �t�B�[���h�̊O�ɏo����e����������
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
// �`�揈��
//=============================================================================
void DrawEBullet(void)
{
	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	CAMERA *cam = GetCamera();

	// ���C�e�B���O�𖳌���
	SetLightEnable(FALSE);

	// ���Z�����ɐݒ�
	SetBlendState(BLEND_MODE_ADD);

	// Z��r����
	SetDepthEnable(FALSE);

	// �t�H�O����
	SetFogEnable(FALSE);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	for(int nEBullet = 0; nEBullet < MAX_EBULLET; nEBullet++)
	{
		if(g_EBullet[nEBullet].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �r���[�}�g���b�N�X���擾
			mtxView = XMLoadFloat4x4(&cam->mtxView);


			// ������������������
			mtxWorld.r[0].m128_f32[0] = mtxView.r[0].m128_f32[0];
			mtxWorld.r[0].m128_f32[1] = mtxView.r[1].m128_f32[0];
			mtxWorld.r[0].m128_f32[2] = mtxView.r[2].m128_f32[0];

			mtxWorld.r[1].m128_f32[0] = mtxView.r[0].m128_f32[1];
			mtxWorld.r[1].m128_f32[1] = mtxView.r[1].m128_f32[1];
			mtxWorld.r[1].m128_f32[2] = mtxView.r[2].m128_f32[1];

			mtxWorld.r[2].m128_f32[0] = mtxView.r[0].m128_f32[2];
			mtxWorld.r[2].m128_f32[1] = mtxView.r[1].m128_f32[2];
			mtxWorld.r[2].m128_f32[2] = mtxView.r[2].m128_f32[2];

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_EBullet[nEBullet].scale.x, g_EBullet[nEBullet].scale.y, g_EBullet[nEBullet].scale.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_EBullet[nEBullet].pos.x, g_EBullet[nEBullet].pos.y, g_EBullet[nEBullet].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			// �}�e���A���ݒ�
			SetMaterial(g_EBullet[nEBullet].material);

			// �|���S���̕`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// �ʏ�u�����h�ɖ߂�
	SetBlendState(BLEND_MODE_ALPHABLEND);

	// Z��r�L��
	SetDepthEnable(TRUE);

	// �t�H�O�L��
	SetFogEnable(TRUE);

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexEBullet(void)
{
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	{//���_�o�b�t�@�̒��g�𖄂߂�
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// ���_���W�̐ݒ�
		vertex[0].Position = XMFLOAT3(-PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[1].Position = XMFLOAT3(PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[2].Position = XMFLOAT3(-PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[3].Position = XMFLOAT3(PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);

		// �@���̐ݒ�
		vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

		// ���ˌ��̐ݒ�
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		vertex[0].TexCoord = { 0.0f, 0.0f };
		vertex[1].TexCoord = { 1.0f, 0.0f };
		vertex[2].TexCoord = { 0.0f, 1.0f };
		vertex[3].TexCoord = { 1.0f, 1.0f };

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	return S_OK;
}

//=============================================================================
// �}�e���A���J���[�̐ݒ�
//=============================================================================
void SetColorEBullet(int nIdxParticle, XMFLOAT4 col)
{
	g_EBullet[nIdxParticle].material.Diffuse = col;
}

//=============================================================================
// �p�[�e�B�N���̔�������
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
			// �e�̐ݒ�
			g_EBullet[nEBullet].nIdxShadow = CreateShadow(XMFLOAT3(pos.x, 0.1f, pos.z), 0.8f, 0.8f);		// �e�̐ݒ�
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

