#ifndef __WIPE_H__
#define __WIPE_H__

//���}�N���̒�`
//��ʑJ��
//�񋓌^
enum tagWIPE_TRANSITION{

	NOT_TRANSITION = -1 ,	//�V�[���ύX���Ȃ���
	CHANGE_SCENE ,			//�V�[���؂�ւ�
	CHANGE_STAGE ,			//�X�e�[�W�؂�ւ�
	RESTART_RACE ,			//�X�^�[�g����n�߂�
	TO_TITLE	 ,			//�^�C�g����
	TO_SELECT	 ,			//�X�e�[�W�Z���N�g��

};

//�摜
//���C�v
#define WIPE_IMAGE_MAX		4		//�摜�ő喇��
#define WIPE_MAX			6		//���C�v�g�p����
//���[�h���C���[�W
#define LOAD_IMAGE_MAX		1		//�ő喇��

//���C�v���o���̉ϒl
//���߉��o
#define WIPE_DEFAULT_ALPHA_ADD		50.0f		//���Z�l
#define WIPE_DEFAULT_ALPHA_SUB		-50.0f		//���Z�l
//�g�k���o
#define WIPE_DEFAULT_SCALE_SPEED		0.020f	//���Z�l
#define WIPE_DEFAULT_CONTRACTION_SPEED	-0.020f	//���Z�l
//stay time
const int WIPE_DEFAULT_STAY_TIME = 100;

//���C�v���o�̗񋓌^
enum tagWIPE_DIRECT{

	WIPE_FEEDOUT	 ,	//���߉��o
	WIPE_CONTRACTION ,	//��ʒ��S�֎��k���鉉�o
	WIPE_DIRECT_MAX  ,	//���o�̐�

};
//���w�b�_�[���C���N���[�h
#include "graphics.h"
#include "animation.h"
#include "game.h"

//���\���̂��`
typedef struct tagWIPE{

	CHARACTER ch;				//�L�����N�^�[�\����
	ANIMATION ani;				//�A�j���[�V�����\����
	tagGAMETRASITION next;		//���̃V�[���ԍ�
	tagWIPE_TRANSITION type;	//���C�v���
	int stay;					//���C�v���Ƃǂ߂鎞��
	float alpha;				//�A���t�@�l
	float add_alpha;			//�A���t�@���Z�l
	float sub_alpha;			//�A���t�@���Z�l
	float contraction_speed;	//�k�����x
	float scale_speed;			//�g�呬�x

}WIPE,*LPWIPE;

//���v���g�^�C�v�錾
void InitWipe(HWND hWnd);//������
bool UpdateWipe();		//�߂�l�ɐ����t���O��Ԃ�
void DrawWipe();		//�\��
void ReleaseWipe();		//���
//���C�v����
void CreateWipe(tagGAMETRASITION next , tagWIPE_TRANSITION type , tagWIPE_DIRECT direct);
//���C�v�����i���߉��o�j
void DirectWipe0();
//���C�v�����i���k���o�j
void DirectWipe1();


#endif