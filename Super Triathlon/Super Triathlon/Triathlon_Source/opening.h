#ifndef __OPENING_H__
#define __OPENING_H__

//�� �񋓌^�̒�`
enum	tagOPENINGSCENEFLAG{
	OPENING_SCENE_TITLE_LOGO_DIRECTION ,			//�^�C�g�����S���o��
	OPENING_SCENE_SELECT_START_FUNCTION ,			//�Q�[���X�^�[�g���A�I�v�V�����̑I��
	OPENING_SCENE_SELECT_LEVEL_FUNCTION ,			//�Q�[�����̃��x���ݒ�
};

//���}�N���̒�`
#define OPENING_IMAGE_MAX		6		//�g�p�摜����
//���p
#define OPENING_SOUND_EFFECT_MAX 3		//�g�p���ʉ��̐�

//���쐧������
const int OPENING_CURSOR_DONT_ACCEPT_CONTROL_TIME = 10;

//�^�C�g���p�摜�\���ݒ�
//�^�C�g�����S�̐ݒ�
const int OPENING_TITLE_LOGO_KIND_OF_IMAGE	= 7;			//�摜�̎��
const int OPENING_TITLE_STAGE_IMAGING_KIND_OF_IMAGE	= 3;	//�e��X�e�[�W�C���[�W�摜�̎��
const int OPENING_IMAGE_TITLE_LOGO_WIDTH	= 600;			//��
const int OPENING_IMAGE_TITLE_LOGO_HEIGHT	= 120;			//����
//�\����
//�n�_
const float OPENING_TITLE_LOGO_BIGIN_INDICATE_X	= 840.0f;		//X
const float OPENING_TITLE_LOGO_BIGIN_INDICATE_Y	= -400.0f;		//Y
//�x�W�F�Ȑ��̗v�f��
const int	OPENING_TITLE_LOGO_SET_BEZIER = 4;
//�I�_
const float OPENING_TITLE_LOGO_TERMINATE_INDICATE_Y	= 50.0f;	//Y
//�ړ���
const float OPENING_TITLE_LOGO_MOVE_Y		= 5.0f;				//Y
//�^�C�g�����S�w�i�̐}�`�̕\�����x
const int	OPENING_TITLE_LOGO_FIGURE_DRAWING_SPEED = 10;

//�e��X�e�[�W�C���[�W�摜�̐ݒ�
const float OPENING_STAGE_IMAGE_CHARACTER_INDICATE_X = 800.0f;
const float OPENING_STAGE_IMAGE_CHARACTER_INDICATE_Y = 320.0f;
//�I�_
const float OPENING_STAGE_IMAGE_CHARACTER_TERMINATE_INDICATE_X	= 90.0f;	//X
//�\���Ԋu
const float OPENING_STAGE_IMAGE_CHARACTER_SPACE_X	= 10.0f;				//X
//�ړ���
const float OPENING_STAGE_IMAGE_CHARACTER_MOVE_X	= -9.0f;				//X
//�g�嗦
const float	OPENING_STAGE_IMAGE_CHARACTER_SCALE		= 0.75f;


//�e��{�^���摜�̐ݒ�
const int OPENING_BUTTON_IMAGE_WIDTH	= 128;		//��
const int OPENING_BUTTON_IMAGE_HEIGHT	= 42;		//����
const int OPENING_BUTTON_KIND_OF_IMAGE	= 6;		//�{�^���摜�̎��
//�\����
//�n�߂̃{�^���ݒ�
const float OPENING_BUTTON_BEGIN_IMAGE_INDICATE_Y	= 200.0f;		//Y
//���x���ݒ莞
const float OPENING_BUTTON_LEVEL_IMAGE_INDICATE_Y	= 200.0f;		//Y
//�\���Ԋu
const float OPENING_BUTTON_IMAGE_INDICATE_SPACE_Y	= 70.0f;		//Y
//�w�i�̃}�[�W��
const float	OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN = 10.0f;

//���w�b�_�[���C���N���[�h
#include "graphics.h"

//���\���̂̒�`
typedef struct tagBUTTON{
	CHARACTER ch;
}BUTTON,*LPBUTTON;


//������֐��̐錾
void InitOpening(HWND hWnd);		//������
void UpdateOpening();				//�X�V
void DrawOpening();					//�\��
void ReleaseOpening();				//���
/*
**************************************************************************************************
�^�C�g�����S���o�̍X�V
**************************************************************************************************
*/
void	UpdateTitleLogoDirect();
/*
**************************************************************************************************
�{�^���ݒ�1�iStart , Option�j
**************************************************************************************************
*/
void	SetFirstButton();
/*
**************************************************************************************************
�{�^���ݒ�2���x���ݒ�
**************************************************************************************************
*/
void	SetSecondButton();


#endif