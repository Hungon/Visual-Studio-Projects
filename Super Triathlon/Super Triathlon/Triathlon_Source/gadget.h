#ifndef __GADGET_H__
#define __GADGET_H__


//���}�N���̒�`
//�摜�p
#define GADGET_IMAGE_MAX	3		//�g�p����
#define GADGET_SET_MAX		3		//�ݒ�p�̗v�f��

//���x�ɏ������摜�����ւ��p
#define GADGET_SPEED_SCORE_ITALIC	100.0f		//100.0Km�ȏ�Ȃ�Α̂ɂ���

//���p

//�X�e�[�^�X�\���p
#define GADGET_SCORE_MAX	1		//�g�p�v�f��

//���w�b�_�[���C���N���[�h
#include "graphics.h"
#include "animation.h"

//���\���̂̒�`
typedef struct tagGADGET{
	CHARACTER ch;
	ANIMATION ani;
	float indicator;	//���x���[�^�[�̐j�̊p�x������
	float coordinate;	//���x���[�^�[�����݂̃v���C���[�̈ړ��ʂɑΉ�������
}GADGET , *LPGADGET;


//���֐��錾
//������
void InitGadget(HWND hWnd);
//�X�V
void UpdateGadget();
//�\��
void DrawGadget();
//���
void ReleaseGadget();
//�K�W�F�b�g�̏���n��
GADGET* GetGadget();
//�v���C���[�̃X�e�[�^�X�𔽉f������
void SetStatus(float status);


#endif