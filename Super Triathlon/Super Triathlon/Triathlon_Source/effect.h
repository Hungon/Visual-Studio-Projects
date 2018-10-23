#ifndef __EFFECT_H__
#define __EFFECT_H__

//���}�N���̒�`
#define EFFECT_MAX 10		//�G�t�F�N�g�̎�ސ�

//���w�b�_�[���C���N���[�h
#include "graphics.h"
#include "animation.h"


//���\���̂̒�`
typedef struct tagEFFECT{

	CHARACTER ef;	
	ANIMATION ani;
	int type;
	
} EFFECT,*LPEFFECT;


//���֐��̒�`
void InitEffect(HWND hWnd);		//������
void UpdateEffect();			//�X�V
void DrawEffect();		//�\��
void ReleaseEffect();			//���
void CreateEffect(float x,float y);		//�G�t�F�N�g���w��̍��W�ɔ���������
//�X�e�[�W�ύX���̃G�t�F�N�g�̍X�V
void ResetEffect();

#endif
