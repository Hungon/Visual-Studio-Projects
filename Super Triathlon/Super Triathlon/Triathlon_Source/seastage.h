#ifndef __SEASTAGE_H__
#define __SEASTAGE_H__


//���}�N���̒�`
//�摜
#define SEA_MAP_IMAGE_MAX	9			//�g�p�摜����
const int SEASTAGE_BACKGROUND_MAX = 3;	//�w�i�̐�
#define SEA_OBJECT_MAX	1				//�I�u�W�F�N�g�̐�
#define SEA_SIGN_MAX	1				//�T�C���\���̐�
//���ʉ�
#define SEA_EFFECT_SOUND_MAX	3		//�g�p���ʉ��̐�
//�~�j�}�b�v�摜����
#define SEA_MINIMAP_KIMD_OF_IMAGE	4
//indication setting
//back ground
const int SEA_MINIMAP_IMAGE_BACK_GROUND_WIDTH = 500;
const int SEA_MINIMAP_IMAGE_BACK_GROUND_HEIGHT = 40;
const float SEA_MINIMAP_IMAGE_BACK_GROUND_INDICATE_X = 135.0f;
const float SEA_MINIMAP_IMAGE_BACK_GROUND_INDICATE_Y = 425.0f;
//minimum swimer
const float	SEA_MINIMAP_IMAGE_SWIMER_SCALE_RATE = 0.5f;
//flags
const int	SEA_MINIMAP_IMAGE_FLAG_WIDTH = 40;
const int	SEA_MINIMAP_IMAGE_FLAG_HEIGHT = 40;

//���w�b�_�[�C���N���[�h
#include "map.h"

//������֐��̒�`
//�C�X�e�[�W�̏�����
void InitSea(HWND hWnd);
//�C�̍X�V
void UpdateSea();
//���H�̍X�V
void UpdateRoad();
//�C�X�e�[�W�̕\��
void DrawSea();
//�C�X�e�[�W�̉��
void ReleaseSea();
//�C�X�e�[�W�̏����擾�ł���
STAGE* GetSeaStage();

#endif
