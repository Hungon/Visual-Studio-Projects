#ifndef __MAP_H__
#define __MAP_H__


//���}�N���̒�`
#define MAP_IMAGE_MAX	3		//�g�p�摜����
#define MAP_SIGN_MAX	2		//�T�C����
#define STAGE_WIDTH		640		//�X�e�[�W�̉�ʂ̕�
#define STAGE_HEIGHT	480		//�X�e�[�W�̉�ʂ̍���


//�J�����̏œ_
#define CAMERA_FOCUS_POSITION_X		304
#define CAMERA_FOCUS_POSITION_Y		200

//�����̃L�����N�^�[����ʂ̒��S�ɃZ�b�g����
#define CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(w)		( STAGE_WIDTH - w ) / 2
//�����̃L�����N�^�[����ʂ̒��S�ɃZ�b�g����
#define CHARACTER_CENTER_POSITION_Y_TO_STAGE_HEIGHT(h)		( STAGE_HEIGHT - h ) / 2


//���p
#define MAP_SOUND_EFFECT_MAX 3	//�g�p���ʉ��̐�


//���w�b�_�[�C���N���[�h
#include "graphics.h"
#include "animation.h"
#include "window.h"
#include "score.h"



//���\���̂̒�`
typedef struct tagSTAGE{

	CHARACTER ch;			//�L�����N�^�[�\����
	ANIMATION ani;			//�A�j���[�V�����\����
	TIME ti;				//���Ԃ̍\����
	float movex;			//�}�b�v�ړ���X
	float movey;			//�}�b�v�ړ���Y
	bool scrolly;			//�c���̃X�N���[���t���O
	bool scrollx;			//�����̃X�N���[���t���O
	bool scroll_r;			//�E�X�N���[���t���O
	bool scroll_l;			//���X�N���[���t���O
	bool sign;				//�X�^�[�g���}�̃t���O
	float obj_popx;			//�I�u�W�F�N�g�����N�_�w���W
	float obj_endx;			//�I�u�W�F�N�g�����N�_�w���W
	int whole_w;			//�X�e�[�W�S�̂̕�
	float wholeHeight;		//�X�e�[�W�S�̂̍���

} STAGE,*LPSTAGE;

//������֐��̒�`
void InitMap(HWND hWnd);					//������
void UpdateMap();							//�X�V
void DrawMap();								//�\��
void ReleaseMap();							//���
//���݂̃X�e�[�W�̕����擾
float GetStageWidth();
//���݂̃X�e�[�W�̍������擾
float GetStageHeight();
float GetCameraX();								//�J�����̂w���W��Ԃ��֐�
float GetCameraY();								//�J�����̂x���W��Ԃ��֐�
void SetCamera(float cameraX,float cameraY);	//�J�����̍��W��_����ʂ̒��S�ɂ���֐�
//�J�����͈̔͂�ύX����
void ChangeCamera(float top , float bottom , float width , float height);
//�J�����̋�`�Ɣ�������֐�
bool CollisionCamera(CHARACTER* ch);
//�X�^�[�g�̃T�C�����D���ȃ^�C�~���O�ŏo����֐�
bool StartSign(bool flag);
//���Z�����擾
bool GetRacing();
//�X�e�[�W�̃N���A���Ԃ��擾����
TIME* GetStageCompleteTime();
//�X�e�[�W�̎��Ԃ𑀍삷��
void ControlStageTime(bool control);

#endif
