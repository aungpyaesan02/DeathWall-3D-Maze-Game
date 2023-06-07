//=============================================================================
//
// �t�@�C��	: �^�C�g������ [button.cpp]
// Author	: �����@GP 006 AUNG PYAE SAN
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "sprite.h"
#include "button.h"
#include "title.h"
#include "sound.h"
#include "debugproc.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_WIDTH				(300)				// �^�C�g����ʂɂ���v���C���[�̃T�C�Y
#define PLAYER_HEIGHT				(100)				// �^�C�g����ʂɂ���v���C���[�̃T�C�Y
#define	BUTTON_WIDTH				(120)				// �{�^���̃T�C�Y
#define	BUTTON_HEIGHT				(50)				// �{�^���̃T�C�Y
#define	TITLE_WIDTH					(700)				// �^�C�g���̃T�C�Y
#define	TITLE_HEIGHT				(150)				// �^�C�g���̃T�C�Y

#define TEXTURE_MAX					(BUTTON_MAX)		// �e�N�X�`���̐�

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*vertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView	*texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/start.png",
	"data/TEXTURE/exit.png",
	"data/TEXTURE/link.png",
	"data/TEXTURE/dw1.png",

};

static BOOL		isLoad = FALSE;		// ���������s�������̃t���O
static BUTTON	button[BUTTON_MAX];
static BOOL		eventStart = FALSE;
static int		cursorPosition;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitButton(void)
{
	ID3D11Device *pDevice = GetDevice();
	float buttonOffset = 60.0f;
	cursorPosition = 0;

	//�e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
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

	//Button�̏�����
	for (int i = 0; i < BUTTON_MAX; i++)
	{
		button[i].w = BUTTON_WIDTH;
		button[i].h = BUTTON_HEIGHT;
		button[i].pos = { (SCREEN_WIDTH * 0.5f - BUTTON_WIDTH * 0.5f), SCREEN_HEIGHT * 0.5f + buttonOffset + (buttonOffset*i), 0.0f };
		button[i].texNo = i;
		
		// �F�̐ݒ�
		//{
		//	if (button[i].texNo == cursorPosition || button[i].texNo == PLAYER_BUTTON || button[i].texNo == TITLE_LOGO)
		//		button[i].color = { 1.0f,1.0f,1.0f,1.0f };
		//	else
		//		button[i].color = { 0.5f,0.5f,0.5f,1.0f };
		//}

		button[i].use = true;
	}

	// �^�C�g����ʂɂ���v���C���[�̏�����
	button[PLAYER_BUTTON].w = PLAYER_WIDTH;
	button[PLAYER_BUTTON].h = PLAYER_HEIGHT;
	button[PLAYER_BUTTON].pos = { (SCREEN_WIDTH * 0.75f), (SCREEN_HEIGHT * 0.7f), 0.0f };

	// �^�C�g�����S�̏�����
	button[TITLE_LOGO].w = TITLE_WIDTH;
	button[TITLE_LOGO].h = TITLE_HEIGHT;
	button[TITLE_LOGO].pos = { 500.5f,200.5f,0.5f};

	isLoad = TRUE;	// �f�[�^�̏��������s����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitButton(void)
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
void UpdateButton(void)
{
	if (!eventStart)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			switch (cursorPosition)
			{
			case START_BUTTON:
				for (int i = 0; i < BUTTON_MAX; i++)
				{
					if (button[i].texNo != PLAYER_BUTTON && button[i].texNo != TITLE_LOGO) button[i].use = false;

				}

				// SE
				PlaySound(SOUND_LABEL_SE_gamesave);

				eventStart = TRUE;	// �C�x���g�̃X�^�[�g�t���O��ON�ɂ���
				break;

			case EXIT_BUTTON:
				exit(0);
				break;

			default:
				break;
			}
			// SE

			PlaySound(SOUND_LABEL_SE_button);

		}


		if (IsButtonPressed(0, BUTTON_START))
		{
			switch (cursorPosition)
			{
			case START_BUTTON:
				for (int i = 0; i < BUTTON_MAX; i++)
				{
					if (button[i].texNo != PLAYER_BUTTON && button[i].texNo != TITLE_LOGO) button[i].use = false;

				}

				// SE
				PlaySound(SOUND_LABEL_SE_gamesave);

				eventStart = TRUE;	// �C�x���g�̃X�^�[�g�t���O��ON�ɂ���
				break;

			case EXIT_BUTTON:
				exit(0);
				break;

			default:
				break;
			}
			// SE

			PlaySound(SOUND_LABEL_SE_button);



		}

		// ���j���[�I��
		if (GetKeyboardTrigger(DIK_UPARROW))
		{
			cursorPosition = -(cursorPosition - 1) % (BUTTON_MAX);

			// SE
			PlaySound(SOUND_LABEL_SE_button);

		}
		else if (GetKeyboardTrigger(DIK_DOWNARROW))
		{
			cursorPosition = (cursorPosition + 1) % (BUTTON_MAX - 2);

			// SE
			PlaySound(SOUND_LABEL_SE_button);

		}


		if (IsButtonPressed(0, BUTTON_X))
		{
			cursorPosition = -(cursorPosition - 1) % (BUTTON_MAX);

			// SE
			PlaySound(SOUND_LABEL_SE_button);


		}
		else if (IsButtonPressed(0, BUTTON_Y))
		{
			cursorPosition = (cursorPosition + 1) % (BUTTON_MAX - 2);

			// SE
			PlaySound(SOUND_LABEL_SE_button);

		}


		SelectMenu();

	}
	else
	{
		GameStartAnimation();
	}



#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("Cursor Position : %d \n", cursorPosition);

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawButton(void)
{
	for (int i = 0; i < BUTTON_MAX; i++)
	{
		if (button[i].use)
		{
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
			{
				// �e�N�X�`���ݒ�
				GetDeviceContext()->PSSetShaderResources(0, 1, &texture[button[i].texNo]);

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteColor(vertexBuffer, button[i].pos.x, button[i].pos.y, button[i].w, button[i].h, 0.0f, 0.0f, 1.0f, 1.0f, button[i].color);


				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);
			}
		}
	}
}

// �^�C�g����ʂɂ���A�j���[�V��������
void GameStartAnimation(void)
{
	float spd = 4.0f;	// �ړ���
	float fadeRate = 0.03f;

	if (button[PLAYER_BUTTON].pos.x <= (SCREEN_WIDTH * 0.5f) - (PLAYER_WIDTH * 0.5f))
	{
		button[PLAYER_BUTTON].pos.y -= spd * 3.0f;
		button[TITLE_LOGO].use = false;

		if (button[PLAYER_BUTTON].pos.y < 0.0f - PLAYER_WIDTH)
		{
			button[PLAYER_BUTTON].use = false;
			eventStart = FALSE;
			SetFade(FADE_OUT, MODE_TUTORIAL);
		}
	}
	else
	{
		button[PLAYER_BUTTON].pos.x -= spd;
		button[TITLE_LOGO].color.z -= fadeRate;
	}
}

void SelectMenu(void)
{
	for (int i = 0; i < BUTTON_MAX; i++)
	{
		if (button[i].texNo == cursorPosition || button[i].texNo == PLAYER_BUTTON || button[i].texNo == TITLE_LOGO)
			button[i].color = { 1.0f,1.0f,1.0f,1.0f };
		else
			button[i].color = { 0.5f,0.5f,0.5f,1.0f };
	}
}





