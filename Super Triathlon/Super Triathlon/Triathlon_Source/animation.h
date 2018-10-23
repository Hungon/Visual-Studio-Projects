#ifndef __ANIMATION_H__
#define __ANIMATION_H__


//���w�b�_�[�̃C���N���[�h
#include "graphics.h"


//ANIMATION�\���̂̒�`
typedef struct tagANIMATION{

	int sx;					//1�R�}�ڂ̂w
	int sy;					//�P�R�}�ڂ̂x
	int max;				//�ő�R�}��
	int fram;				//�t���[����
	int time;				//�o�ߎ���
	int cnt;				//���݂̃R�}��
	int type;				//��ޔԍ�
	int type_now;			//���݂̎�ޔԍ�
	bool reverseF;			//���΃A�j���[�V�����t���O

}ANIMATION,*LPANIMATION;


//���v���g�^�C�v�錾

//�A�j���[�V�����������L�����N�^�[�ɐݒ���s��
void SetAnimation(CHARACTER* ch,ANIMATION* ani,int sx,int sy,int max,int fram,int type,bool flag);
//�A�j���[�V�����̍X�V���s��
bool UpdateAnimation(CHARACTER* ch,ANIMATION* ani);
//�L�����N�^�[�̃A�j���[�V�������X�V
bool UpdateReverseAnimation( CHARACTER* ch , ANIMATION* ani );


#endif
