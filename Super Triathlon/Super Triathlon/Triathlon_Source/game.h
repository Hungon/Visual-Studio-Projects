#ifndef __GAME_H__
#define __GAME_H__

//���}�N���̒�`

//��ʂ�J�ڂ������񋓌^
enum tagGAMETRASITION{
	
	//���j���[����̉�ʑJ�ڂł̗v�f
	 MENU_TRANSITION = -1 ,
	 SCENE_OPENING	,
	 SCENE_SELECTSTAGE	,
	 SCENE_PLAY		,
	 SCENE_RESULT	,
	 SCENE_GAMEOVER	,
	 SCENE_CONFIG	,
	 SCENE_OPTION	,
	 SCENE_ENDING   ,
	 SCENE_MAX		,

};

//using kind of language
enum tagGAMELANGUAGE {
	GAME_LANGUAGE_JAPANESE,			//Japanese
	GAME_LANGUAGE_ENGLISH,			//English
	GAME_KIND_OF_LANGUAGE,			//kind of language
};

//���w�b�_�[�t�@�C���̓ǂݍ���
#include "graphics.h"


//������֐����`

//���������s���֐�
void InitGame(HWND hWnd);	
//�X�V���s���֐�
void UpdateGame();		
//�\�����s���֐�
void DrawGame(HWND hWnd);	
//������s���֐�
void ReleaseGame();	
//�V�[���̐؂�ւ����m�F����֐�
void CheckScene(HWND hWnd);
//�V�[���̐؂�ւ����s���֐�
void ChangeScene(tagGAMETRASITION scene);
//���݂̃V�[�����擾����
tagGAMETRASITION GetSceneNow();
//�^�C�g���֖߂�
void ToTitle();	
/*
�X�e�[�W�I���֑J��
*/
void ToSelectStage();
/*
Set current language
*/
void	SetGameLanguage(tagGAMELANGUAGE language);
/*
Get current language
*/
tagGAMELANGUAGE	GetGameLanguage();

#endif