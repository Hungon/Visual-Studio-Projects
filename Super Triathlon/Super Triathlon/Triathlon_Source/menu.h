#ifndef __MENU_H__
#define __MENU_H__

//�� �񋓌^
//���j���[���J��
enum tagMENUSELECTION {
	MENU_SELECT_VACANT = -1,
	MENU_SELECT_ITEM,
	MENU_SELECT_ANSWER,
	MENU_SELECT_CLOSE,
	MENU_KIND_OF_SLECTION,
};

//���}�N���̒�`
#define MENU_IMAGE_MAX			4	//�g�p�摜����
#define MENU_EFFECT_SOUND_MAX	3	//�g�p���ʉ�
#define MENU_BACK_IMAGE_MAX		1	//���j���[�w�i�p	
const int MENU_KIND_OF_ITEM = 4;	//���ڂ̐�(�^�C�g��menu���܂�)


//���j���[���ڂ̕\���ݒ�
//�\����
const float MENU_INDICATE_X = 240.0f;			//X
const float MENU_INDICATE_Y = 240.0f;			//Y
//�T�C�Y
const int MENU_IMAGE_ITEM_WIDTH = 140;			//��
const int MENU_IMAGE_ITEM_HEIGHT = 42;			//����
//�\���Ԋu
const float MENU_ITEM_INDICATE_SPACE_Y = 10.0f;		//Y

//yes no
const float MENU_ANSWER_INDICATE_X = 290.0f;		//�\����X
const float MENU_ANSWER_INDICATE_Y = 290.0f;		//�\����Y
//�T�C�Y
const int MENU_IMAGE_ANSWER_WIDTH = 60;				//��
const int MENU_IMAGE_ANSWER_HEIGHT = 42;			//����
const float MENU_ANSWER_INDICATE_SPACE_Y = 60.0f;	//�\���ԊuY


//���쐧������
const int MENU_DONT_CONTROL_TIME = 20;

//���w�b�_�[�̃C���N���[�h
#include "graphics.h"


//���v���g�^�C�v�錾
//������
void InitMenu(HWND hwnd);
//�X�V
void UpdateMenu();
//�\��
void DrawMenu();
//���
void ReleaseMenu();
//���j���[���J��
void OpenMenu();
//���j���[�����擾
bool GetMenu();
/*
���j���[�����
*/
void	CloseMenu();

/*
�e�퍀�ڑI��
*/
void	MenuSelectItem();
/*
�e��I���ւ̕ԓ�
*/
void	MenuAnswer();

#endif