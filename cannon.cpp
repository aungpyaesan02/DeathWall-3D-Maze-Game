//=============================================================================
//
// �t�@�C��	: ���[�t�B���O���� [cannon.cpp]
// Author	: �����@GP 006 AUNG PYAE SAN
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "cannon.h"
#include "player.h"
#include "ebullet.h"
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MORPHING_WAIT		(30.0f)					// 60�t���[���ŕό`
#define ROTATE_SPD			(0.01f)
#define MORPHIN_OFFSET_Y	(10.0f)					// �G�l�~�[�̑��������킹��
#define	VALUE_ROTATE		(XM_PI * 0.02f)			// ��]��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CANNON		cannon[MAX_CANNON];
static MODEL		modelTable[MODEL_MAX];

static char *modelNames[MODEL_MAX] =
{
	"data/MODEL/canon01.obj",	
	"data/MODEL/canon02.obj",	
	"data/MODEL/canon03.obj",

};

static BOOL enableRotate = TRUE;
static BOOL				g_Load = FALSE;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitCannon(void)
{
	// OBJ�f�[�^�̓ǂݍ��݁�������
	for (int i = 0; i < MAX_CANNON; i++)
	{
		LoadModel(modelNames[0], &cannon[i].model);
		cannon[i].load = TRUE;
		cannon[i].use  = TRUE;
		cannon[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		cannon[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		cannon[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		cannon[i].size = MORPHIN_SIZE;
		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&cannon[i].model, &cannon[i].diffuse[0]);

		cannon[i].time = 0.0f;
		cannon[i].sign = 1;
		cannon[i].index = 0;
	}

	cannon[0].pos = XMFLOAT3(351.0f, MORPHIN_OFFSET_Y, -405.0f);
	cannon[1].pos = XMFLOAT3(-183.0f, MORPHIN_OFFSET_Y, 258.0f);
	cannon[2].pos = XMFLOAT3(-116.0f, MORPHIN_OFFSET_Y, 409.0f);
	cannon[3].pos = XMFLOAT3(350.0f, MORPHIN_OFFSET_Y, 466.0f);
	cannon[4].pos = XMFLOAT3(539.0f, MORPHIN_OFFSET_Y, -47.0f);



	// ���_���ǂݍ���
	for (int i = 0; i < MODEL_MAX; i++)
	{
		LoadObj(modelNames[i], &modelTable[i]);
	}

	g_Load = TRUE;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitCannon(void)
{
	if (g_Load == FALSE) return;

	// ���f���̉������
	for (int i = 0; i < MAX_CANNON; i++)
	{
		if (cannon[i].load)
		{
			UnloadModel(&cannon[i].model);
			cannon[i].load = FALSE;

		}

	}

	for (int j = 0; j < MODEL_MAX; j++)
	{
		delete[] modelTable[j].VertexArray;
		delete[] modelTable[j].IndexArray;
		delete[] modelTable[j].SubsetArray;
	}

	g_Load = FALSE;

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCannon(void)
{
	PLAYER *g_Player = GetPlayer(); // �v���C���[�̏������ׂĎ擾���Ă���B

	for (int i = 0; i < MAX_CANNON; i++)
	{

		if (cannon[i].use == TRUE)
		{


			int playerDistance;
			int enemyDistance;
			int minDistance = 60;
			int distance;

			playerDistance = (int)sqrt(g_Player->pos.x * g_Player->pos.x + g_Player->pos.z * g_Player->pos.z);

			enemyDistance = (int)sqrt(cannon[i].pos.x * cannon[i].pos.x + cannon[i].pos.z * cannon[i].pos.z);

			distance = playerDistance - enemyDistance;

			distance = abs(distance);

			if (distance <= minDistance)
			{


				XMVECTOR rot1 = XMLoadFloat3(&g_Player[0].rot);
				XMVECTOR rot2 = XMLoadFloat3(&cannon[i].rot);

				rot2   += (rot1 - rot2) * 0.05f;

				XMStoreFloat3(&cannon[i].rot, rot2);


				// ���`��ԗp�̃^�C�}�[�X�V����
				cannon[i].time += (1.0f / MORPHING_WAIT);
				if (cannon[i].time > 1.0f)
				{
					cannon[i].index += cannon[i].sign;
					cannon[i].time = 0.0f;
				}

				if ((cannon[i].sign > 0 && cannon[i].index > MODEL_MAX - 2) ||
					(cannon[i].sign < 0 && cannon[i].index < 1))
				{
					cannon[i].sign *= -1;
				}

				SetEBullet(cannon[i].pos, cannon[i].rot, XMFLOAT4{ 0.0f,0.0f,1.0f,1.0f }, 10.0f, 10.0f, 400);


			}


			int curIndex = cannon[i].index;
			int nextIndex = curIndex + cannon[i].sign;

			// ���_�̍X�V����//Vertex update process
			int vertexNum = modelTable[0].VertexNum;
			VERTEX_3D *vertexArray = new VERTEX_3D[vertexNum];
			for (int j = 0; j < vertexNum; j++)
			{
				XMVECTOR vec;

				// Diffuse
				XMVECTOR d0 = XMLoadFloat4(&modelTable[curIndex].VertexArray[j].Diffuse);	// ���݂̏ꏊ
				XMVECTOR d1 = XMLoadFloat4(&modelTable[nextIndex].VertexArray[j].Diffuse);	// ���̏ꏊ
				vec = d1 - d0;
				XMStoreFloat4(&vertexArray[j].Diffuse, d0 + vec * cannon[i].time);

				// Normal
				XMVECTOR n0 = XMLoadFloat3(&modelTable[curIndex].VertexArray[j].Normal);	// ���݂̏ꏊ
				XMVECTOR n1 = XMLoadFloat3(&modelTable[nextIndex].VertexArray[j].Normal);	// ���̏ꏊ
				vec = n1 - n0;
				XMStoreFloat3(&vertexArray[j].Normal, n0 + vec * cannon[i].time);

				// Position
				XMVECTOR p0 = XMLoadFloat3(&modelTable[curIndex].VertexArray[j].Position);	// ���݂̏ꏊ
				XMVECTOR p1 = XMLoadFloat3(&modelTable[nextIndex].VertexArray[j].Position);	// ���̏ꏊ
				vec = p1 - p0;
				XMStoreFloat3(&vertexArray[j].Position, p0 + vec * cannon[i].time);

	

				// TexCoord
				XMVECTOR t0 = XMLoadFloat2(&modelTable[curIndex].VertexArray[j].TexCoord);	// ���݂̏ꏊ
				XMVECTOR t1 = XMLoadFloat2(&modelTable[nextIndex].VertexArray[j].TexCoord);	// ���̏ꏊ
				vec = t1 - t0;
				XMStoreFloat2(&vertexArray[j].TexCoord, t0 + vec * cannon[i].time);
			}


			// ���_�o�b�t�@����蒼��Recreate the vertex buffer
			{
				cannon[i].model.VertexBuffer->Release();

				D3D11_BUFFER_DESC bd;
				ZeroMemory(&bd, sizeof(bd));
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(VERTEX_3D) * vertexNum;
				bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				bd.CPUAccessFlags = 0;

				D3D11_SUBRESOURCE_DATA sd;
				ZeroMemory(&sd, sizeof(sd));
				sd.pSysMem = vertexArray;

				GetDevice()->CreateBuffer(&bd, &sd, &cannon[i].model.VertexBuffer);

				delete[] vertexArray;
			}


#ifdef _DEBUG
			PrintDebugProc(" Rotate ON/OFF\n   ��SPACE\n\n");
			PrintDebugProc(" Index   : %d\n", curIndex);
			PrintDebugProc(" Time    : %f\n", cannon[i].time);
#endif // _DEBUG
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCannon(void)
{
	// �J�����O����
	SetCullingMode(CULL_MODE_BACK);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	for (int i = 0; i < MAX_CANNON; i++)
	{

		if (cannon[i].use == FALSE) continue;

			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(cannon[i].scl.x, cannon[i].scl.y, cannon[i].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(cannon[i].rot.x, cannon[i].rot.y + XM_PI, cannon[i].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(cannon[i].pos.x, cannon[i].pos.y, cannon[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&cannon[i].mtxWorld, mtxWorld);

			// ���f���`��
			DrawModel(&cannon[i].model);
		
	}
	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// �v���C���[�����擾
//=============================================================================
CANNON *GetCannon(void)
{
	return &cannon[0];
}

