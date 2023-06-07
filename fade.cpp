//=============================================================================
//
// �t�@�C��	: �t�F�[�h���� [fade.cpp]
// Author	: �����@GP 006 AUNG PYAE SAN
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(2)				// �e�N�X�`���̐�

#define	FADE_RATE					(15.0f)			// �t�F�[�h�W��
#define FADE_MAX					(2)				// �t�F�[�h�̐�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
static void SetVertex(float X, float Y, float Width, float Height, float U, float V, float UW, float VH, XMFLOAT4 color);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*vertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView	*texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *textureName[] = {

	"data/TEXTURE/doorup.jpg",
	"data/TEXTURE/doordown.jpg",

};

static BOOL		isLoad = FALSE;		// ���������s�������̃t���O
static FADE		fade[FADE_MAX];


//=============================================================================
// ����������
//=============================================================================
HRESULT InitFade(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			textureName[i],
			NULL,
			NULL,
			&texture[i],
			NULL);
	}


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &vertexBuffer);

	// �ϐ��̏�����

	for (int i = 0; i < FADE_MAX; i++)
	{
		fade[i].w = TEXTURE_WIDTH;
		fade[i].h = TEXTURE_HEIGHT;
		fade[i].texNo = i;

		fade[i].flag = FADE_IN;
		fade[i].nextMode = MODE_MAX;
		fade[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	fade[0].pos = XMFLOAT3(0.0f, -SCREEN_HEIGHT, 0.0f);
	fade[1].pos = XMFLOAT3(0.0f, SCREEN_HEIGHT, 0.0f);

	isLoad = TRUE;	// �f�[�^�̏��������s����
	return S_OK;


}

//=============================================================================
// �I������
//=============================================================================
void UninitFade(void)
{
	if (isLoad == FALSE) return;

	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (texture[i])
		{
			texture[i]->Release();
			texture[i] = NULL;
		}
	}

	isLoad = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFade(void)
{

	const float offset = 0.1f;

	if (fade[0].flag != FADE_NONE)
	{// �t�F�[�h������
		if (fade[0].flag == FADE_OUT)
		{// �t�F�[�h�A�E�g����

			fade[0].pos.y += FADE_RATE;		// �ォ�牺�ɍs��
			fade[1].pos.y -= FADE_RATE;		// �������ɍs��

			// �^�񒆂ɂ����
			if ((fade[0].pos.y >= -SCREEN_HEIGHT * 0.5f + offset) && fade[1].pos.y <= SCREEN_HEIGHT * 0.5f - offset)
			{
				// �t�F�[�h�C�������ɐ؂�ւ�
				fade[0].pos.y = -SCREEN_HEIGHT * 0.5f;
				fade[1].pos.y = SCREEN_HEIGHT * 0.5f;

				Sleep(800);

				SetFade(FADE_IN, fade[0].nextMode);

				// ���[�h��ݒ�
				SetMode(fade[0].nextMode);
			}

			// SE
			PlaySound(SOUND_LABEL_SE_defend001);

		}
		else if (fade[0].flag == FADE_IN)
		{// �t�F�[�h�C������
			fade[0].pos.y -= FADE_RATE;		// �^�񒆂����ɍs��
			fade[1].pos.y += FADE_RATE;		// �^�񒆂��牺�ɍs��

			if ((fade[0].pos.y <= -SCREEN_HEIGHT) && (fade[1].pos.y >= SCREEN_HEIGHT))
			{
				// �t�F�[�h�����I��
				fade[0].pos.y = -SCREEN_HEIGHT;
				fade[1].pos.y = SCREEN_HEIGHT;

				SetFade(FADE_NONE, fade[0].nextMode);
			}

			// SE
			PlaySound(SOUND_LABEL_SE_defend001);

		}
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawFade(void)
{
	if (fade[0].flag == FADE_NONE) return;	// �t�F�[�h���Ȃ��̂Ȃ�`�悵�Ȃ�

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);


	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();


	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);


	// �^�C�g���̔w�i��`��
	for (int i = 0; i < FADE_MAX; i++)
	{
		// �t�F�[�h�p�|���S����`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &texture[fade[i].texNo]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetVertex(fade[i].pos.x, fade[i].pos.y, fade[i].w, fade[i].h, 0.0f, 0.0f, 1.0f, 1.0f, fade[i].color);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}

	}

}


//=============================================================================
// ���_�f�[�^�ݒ�
//=============================================================================
static void SetVertex(float X, float Y, float Width, float Height, float U, float V, float UW, float VH, XMFLOAT4 color)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D *vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = XMFLOAT3(X, Y, 0.0f);
	vertex[0].Diffuse = color;
	vertex[0].TexCoord = XMFLOAT2(U, V);

	vertex[1].Position = XMFLOAT3(X + Width, Y, 0.0f);
	vertex[1].Diffuse = color;
	vertex[1].TexCoord = XMFLOAT2(U + UW, V);

	vertex[2].Position = XMFLOAT3(X, Y + Height, 0.0f);
	vertex[2].Diffuse = color;
	vertex[2].TexCoord = XMFLOAT2(U, V + VH);

	vertex[3].Position = XMFLOAT3(X + Width, Y + Height, 0.0f);
	vertex[3].Diffuse = color;
	vertex[3].TexCoord = XMFLOAT2(U + UW, V + VH);


	GetDeviceContext()->Unmap(vertexBuffer, 0);
}


//=============================================================================
// �t�F�[�h�̏�Ԑݒ�
//=============================================================================
void SetFade(int fadeFlg, int nextMode)
{
	fade[0].flag = fadeFlg;
	fade[0].nextMode = nextMode;
}

//=============================================================================
// �t�F�[�h�̏�Ԏ擾
//=============================================================================
int GetFade(void)
{
	return fade[0].flag;
}



