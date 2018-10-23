#ifndef __CONFIG_H__
#define __CONFIG_H__

//�}�N���̒�`
#define ACTION_MAX 3	//�s���̐�
#define ATTACK 0		//�U��
#define JUMP 1		//�W�����v
#define DUSH 2		//�_�b�V��
#define BUTTON_MAX 10	//�R���g���[���[�̃{�^����

#define CONFIG_IMG_MAX	1				//�摜�̎g�p����
#define CONFIG_SOUND_EFFECT_MAX	3		//���̎g�p��


#include "graphics.h"


//�v���g�^�C�v�錾
void InitConfig( HWND hWnd );	//������
void UpdateConfig();		//�X�V
void DrawConfig();	//�\��
void ReleaseConfig();		//���
int GetConfig(int action);		//�g�p�L�[�̕ύX

#endif