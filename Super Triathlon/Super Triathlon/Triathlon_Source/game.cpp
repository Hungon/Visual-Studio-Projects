#include    "game.h"
#include    "fps.h"
#include    "input.h"
#include    "play.h"
#include    "opening.h"
#include    "gameover.h"
#include    "result.h"
#include    "config.h"
#include    "wipe.h"
#include    "sound.h"
#include    "talk.h"
#include    "option.h"
#include    "menu.h"
#include    "ending.h"
#include	"SelectStage.h"
#include	<stdio.h>


//���O���[�o���ϐ�

//��ʑJ�ڂ������ϐ�
tagGAMETRASITION g_scene_now = SCENE_OPENING;		//���݂̃V�[��
tagGAMETRASITION g_scene_next = g_scene_now;		//���̃V�[��

//using current language
tagGAMELANGUAGE	g_GameUsingLanguage = GAME_LANGUAGE_ENGLISH;


//************************************************************************************************
//��InitGame�֐��u�������v
//************************************************************************************************
void InitGame(HWND hWnd){

	//�V�[���؂�ւ��������֐��|�C���^
	void (*pInit[SCENE_MAX])(HWND) = {
		
		InitOpening ,
		InitSelectStage,
		InitPlay ,
		InitCommonResult,
		InitGameOver ,
		InitConfig ,
		InitOption ,
		InitEnding ,
	};

	//�E�B���h�E���[�h�̏�����
	if(InitDDWindow(hWnd) == false){
		MessageBox(NULL , "DD���������s\n" , "" , MB_OK);
		return;
	}
	//DirectSound�̏�����
	if(InitDirectSound(hWnd) == false){
		MessageBox(NULL,"DS�ǂݍ��ݎ��s","InitGame",MB_OK);
		return;
	}

	//�����\���̏�����
	InitTalk();

	//���C�v�̏�����
	InitWipe(hWnd);

	//�V�[���؂�ւ�
	pInit[g_scene_now](hWnd);
}

//************************************************************************************************
//��UpdateGame�֐��u�X�V�v
//************************************************************************************************
void UpdateGame(){

	//�V�[���؂�ւ��������֐��|�C���^
	void (*pUpdate[SCENE_MAX])() = {

		UpdateOpening ,
		UpdateSelectStage,
		UpdatePlay ,
		UpdateResult ,
		UpdateGameOver ,
		UpdateConfig ,
		UpdateOption ,
		UpdateEnding ,
	};

	//�ŏ��ɃL�[�̍X�V
	UpdateInput();

	//���͕\���̍X�V
	UpdateTalk();

	//���C�v���������Ȃ瑼�̍X�V���s��
	if(UpdateWipe() == false){
		pUpdate[g_scene_now]();
	}
}


//************************************************************************************************	
//��DrawGame�֐��u�\���v
//************************************************************************************************
void DrawGame(HWND hWnd){

	HDC hDC = NULL;
	//�o�b�N�T�[�t�F�C�X�̎擾
	LPDIRECTDRAWSURFACE7 lpDDSBack = GetBackSurface();

	//�V�[���؂�ւ��������֐��|�C���^
	void (*pDraw[SCENE_MAX])() = {

		DrawOpening ,
		DrawSelectStage,
		DrawPlay ,
		DrawResult ,
		DrawGameOver ,
		DrawConfig ,
		DrawOption ,
		DrawEnding ,
	};

	//�h��Ԃ�
	FillBackSurface();

	//�V�[���؂�ւ�
	pDraw[g_scene_now]();

	//���͂̕\��
	DrawTalk();

	//���C�v�̕`��
	DrawWipe();

	//FPS
//	DrawFps(0,0);
		
	//���z��ʂ�hDC���擾
	lpDDSBack->GetDC(&hDC);

	//���z��ʂ�hDC�����
	lpDDSBack->ReleaseDC(hDC);

	//�ŏI�o��
	UpdatePrimary(hWnd);

}

//************************************************************************************************
//��ReleaseGame�֐��u����v
//************************************************************************************************
void ReleaseGame(){

	//���͂̉��
	ReleaseTalk();

	//DirectDraw�̉��
	ReleaseDirectDraw();

	//DirectSound�̉��
	ReleaseDirectSound();
	
}

//************************************************************************************************
//�V�[���ύX
//************************************************************************************************
void CheckScene(HWND hWnd){

	//�������̕���֐��|�C���^
	void(*pInit[SCENE_MAX])(HWND) = {
		
		InitOpening ,
		InitSelectStage,
		InitPlay ,
		InitCommonResult,
		InitGameOver ,
		InitConfig ,
		InitOption ,
		InitEnding ,
	};

	//����̕���֐��|�C���^
	void(*pRelease[SCENE_MAX])() = {
		
		ReleaseOpening ,
		ReleaseSelectStage,
		ReleasePlay ,
		ReleaseResult ,
		ReleaseGameOver ,
		ReleaseConfig ,
		ReleaseOption ,
		ReleaseEnding ,
	};

	//���݂̃V�[���Ǝ��̃V�[�����r
	if( g_scene_now != g_scene_next ){

		//�قȂ�l�Ȃ�V�[���ύX
		//���݂̃V�[�������
		pRelease[g_scene_now]();

		//���݂̃V�[���̍X�V
		g_scene_now = g_scene_next;

		//���̃V�[����������
		pInit[g_scene_now](hWnd);

	}
}

//************************************************************************************************
//�n���ꂽ�l�����ɃV�[���ԍ���ύX����֐�
//************************************************************************************************
void ChangeScene( tagGAMETRASITION scene ){ g_scene_next = scene; }


//************************************************************************************************
//���݂̃V�[�����擾����
//************************************************************************************************
tagGAMETRASITION GetSceneNow(){ return g_scene_now; }


//************************************************************************************************
//�^�C�g����ʂ֑J��	
//************************************************************************************************
void ToTitle() {
	//�X�e�[�W�e�[�u��
	tagSELECTSTAGE stageTbl[STAGE_MAX] = { STAGE_SEA , STAGE_ROAD ,STAGE_OFF_ROAD };
	//�N���A�����X�e�[�W�����Z�b�g
	for (int i = 0; i < STAGE_MAX; i++) { SetCompleteStage(SELECTSTAGE_STAGE_NOT_COMPLETE, stageTbl[i]); }
	//�Q�[���I�[�o�[
	ChangeScene(SCENE_GAMEOVER);
}

/*
�X�e�[�W�I���֑J��
*/
void ToSelectStage() { ChangeScene(SCENE_SELECTSTAGE); }

/*
Set current language
*/
void	SetGameLanguage(tagGAMELANGUAGE language) {
	//declaration for save
	RECORD record = { 0 };
	int item = 1;
	g_GameUsingLanguage = language; 
	//subsutitute present language for record variable
	record.recordBox[0][0] = (int)language;
	//save present language to file
	SaveRecord("Data\\Language.dat", 1, &item, &record);
}
/*
Get current language
*/
tagGAMELANGUAGE	GetGameLanguage() { return g_GameUsingLanguage; }