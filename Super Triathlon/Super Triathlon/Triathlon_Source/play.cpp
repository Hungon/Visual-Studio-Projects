#include    "play.h"
#include    "map.h"
#include    "seastage.h"
#include    "CRoadStage.h"
#include    "swimer.h"
#include    "CRunnerManager.h"
#include    "gadget.h"
#include    "seaenemy.h"
#include	"CRoadEnemyManager.h"
#include    "item.h"
#include    "effect.h"
#include    "result.h"
#include    "score.h"
#include    "wipe.h"
#include    "game.h"
#include    "menu.h"
#include    "wipe.h"
#include    "sound.h"
#include    "talk.h"
#include    "input.h"
#include	"OffRoadStage.h"
#include	"OffRoadRacer.h"
#include	<iostream>

//���O���[�o���ϐ�
//�X�e�[�W�̎�ނ�����
tagSELECTSTAGE g_stage = STAGE_VACANT;
//�v���C����������
bool g_play = false;
//�Q�[���̓�Փx������
tagGAMELEVEL g_level = EASY_MODE;

//�I�t���[�h�X�e�[�W
COffRoadStage	g_COffRoad;					//�X�e�[�W
COffRoadRacer	g_COffRoadRacer;			//�v���C���[

//���[�h�X�e�[�W
//�����ҊǗ��N���X
CRunnerManager		g_CRunnerManager;
//��Q���Ǘ��N���X
CRoadEnemyManager	g_CRoadEnemyForPlay;
//�X�e�[�W�N���X
CRoadStage			g_CRoadStage;

//************************************************************************************************
//������
//************************************************************************************************
void InitPlay(HWND hWnd) {

	//�v���C���t���O
	g_play = false;

	//�}�b�v�̏�����
	InitMap(hWnd);

	//�� �X�e�[�W�ԍ��ɉ�����������
	switch (g_stage) {

		//�C�X�e�[�W
	case STAGE_SEA:
		InitSea(hWnd);			//�X�e�[�W
		InitSwimer(hWnd);			//�v���C���[
		InitEnemy(hWnd);			//�G
		InitItem(hWnd);			//�A�C�e���̏�����
		InitEffect(hWnd);			//�G�t�F�N�g�̏�����

		break;

		//���H�X�e�[�W
	case STAGE_ROAD:
		g_CRoadStage.InitRoadStage();									//�X�e�[�W
		g_CRoadEnemyForPlay.InitRoadEnemyManager(&g_CRunnerManager);	//�G
		g_CRunnerManager.InitRunnerManager(&g_CRoadEnemyForPlay);		//������
		break;

		//�I�t���[�h�X�e�[�W
	case STAGE_OFF_ROAD:
		g_COffRoad.InitOffRoadStage();			//�X�e�[�W
		g_COffRoadRacer.InitOffRoadRacer();		//�v���C���[
		break;
	}

	//�K�W�F�b�g�̏�����
	InitGadget(hWnd);

	InitScore(hWnd);		//�X�R�A�̏�����

	InitMenu(hWnd);		//���j���[�̏�����
}

//************************************************************************************************
//�X�V
//************************************************************************************************
void UpdatePlay(){

	UpdateMap();		//�}�b�v�̍X�V

	//���[�X���X�^�[�g������G�Ȃǂ��X�V
	if(g_play == true){

		//***********�f�o�b�O�p************
		if (JustKey('R')) CreateWipe(SCENE_RESULT, CHANGE_SCENE, WIPE_CONTRACTION);
		if (JustKey('E')) CreateWipe(SCENE_ENDING, CHANGE_SCENE, WIPE_CONTRACTION);

	//�� �X�e�[�W�ԍ��ɉ�����������
	switch( g_stage ){

		//�C�X�e�[�W
		case STAGE_SEA :
			UpdateSea();			//�X�e�[�W
			UpdateSwimer();			//�v���C���[
			UpdateEnemy();			//�G
			UpdateItem();			//�A�C�e���̍X�V
			UpdateEffect();			//�G�t�F�N�g�̍X�V
			UpdateValueation();		//�X�R�A�]���̍X�V
			break;

		//���H�X�e�[�W
		case STAGE_ROAD:
			g_CRoadStage.UpdateRoadStage();						//�X�e�[�W
			g_CRoadEnemyForPlay.UpdateRoadEnemyManager();		//�G
			g_CRunnerManager.UpdateRunnerManager();				//������
			UpdateValueation();									//�X�R�A�]���̍X�V
			break;

		//�I�t���[�h�X�e�[�W
		case STAGE_OFF_ROAD:
			g_COffRoad.UpdateOffRoadStage();		//�X�e�[�W
			g_COffRoadRacer.UpdateOffRoadRacer();	//�v���C���[
			break;
	}
		//�K�W�F�b�g�̍X�V
		UpdateGadget();
	}
	
	UpdateMenu();		//���j���[�̍X�V

	//���j���[���J���Ă�����ꎞ��~
	if( GetMenu() ) g_play = false;

}

//************************************************************************************************
//�\��
//************************************************************************************************
void DrawPlay() {

	//�� �X�e�[�W�ԍ��ɉ�����������
	switch (g_stage) {

		//�C�X�e�[�W
	case STAGE_SEA:
		DrawSea();			//�X�e�[�W
		break;

		//���H�X�e�[�W
	case STAGE_ROAD:
		g_CRoadStage.DrawRoadStage();	//�X�e�[�W
		break;

		//�I�t���[�h�X�e�[�W
	case STAGE_OFF_ROAD:
		g_COffRoad.DrawOffRoadStage();			//�X�e�[�W
		break;
	}

	DrawMap();		//�}�b�v�̕\��

	//���@���[�X�J�n��̕\��
	if (g_play == true) {

		//�� �X�e�[�W�ԍ��ɉ�����������
		switch (g_stage) {

			//�C�X�e�[�W
		case STAGE_SEA:
			DrawSwimer();			//�v���C���[
			DrawEnemy();			//�G
			DrawItem();				//�A�C�e���̕\��
			DrawEffect();			//�G�t�F�N�g�̕\��
			break;

			//���H�X�e�[�W
		case STAGE_ROAD:
			g_CRoadEnemyForPlay.DrawRoadEnemyManager();		//�G
			g_CRunnerManager.DrawRunnerManager();			//������
			break;

			//�I�t���[�h�X�e�[�W
		case STAGE_OFF_ROAD:
			g_COffRoadRacer.DrawOffRoadRacer();				//�v���C���[
			break;
		}

		//�K�W�F�b�g�̕\��
		DrawGadget();
	}


	DrawMenu();		//���j���[�̕\��

}

//************************************************************************************************
//���
//************************************************************************************************
void ReleasePlay() {

	ReleaseMap();					//�}�b�v�̉��

	//�� �X�e�[�W�ԍ��ɉ�����������
	switch (g_stage) {

		//�C�X�e�[�W
	case STAGE_SEA:
		ReleaseSea();			//�X�e�[�W
		ReleaseSwimer();		//�v���C���[
		ReleaseEnemy();			//�G
		ReleaseItem();			//�A�C�e���̉��
		ReleaseEffect();		//�G�t�F�N�g�̉��
		break;

		//���H�X�e�[�W
	case STAGE_ROAD:
		g_CRoadStage.ReleaseRoadStage();						//�X�e�[�W
		g_CRunnerManager.ReleaseRunnerManager();				//������
		g_CRoadEnemyForPlay.ReleaseRoadEnemyManager();			//�G
		break;

		//�I�t���[�h�X�e�[�W
	case STAGE_OFF_ROAD:
		g_COffRoad.ReleaseOffRoadStage();			//�X�e�[�W
		g_COffRoadRacer.ReleaseOffRoadRacer();		//�v���C���[
		break;
	}

	ReleaseGadget();	//�K�W�F�b�g�̉��
	ReleaseMenu();		//���j���[�̉��
	ReleaseScore();		//�X�R�A�̉��

}

//************************************************************************************************
//�v���C���t���O�𗧂Ă�
//************************************************************************************************
void Playing(bool play){ g_play = play; }

//************************************************************************************************
//�v���C�����擾�ł���
//************************************************************************************************
bool GetPlay(){	return g_play; }

//************************************************************************************************
//�X�e�[�W��ύX����֐�
//************************************************************************************************
void ChangeStage( tagSELECTSTAGE select ) { g_stage = select; }

//************************************************************************************************
//���݂̃X�e�[�W�ԍ����擾����֐�
//************************************************************************************************
tagSELECTSTAGE GetStage() { return g_stage; }

//************************************************************************************************
//�Q�[���̓�Փx��ݒ�
//************************************************************************************************
void SetGameLevel(tagGAMELEVEL level){ g_level = level; }

//************************************************************************************************
//�Q�[���̓�Փx���擾
//************************************************************************************************
tagGAMELEVEL GetGameLevel(){ return g_level; }

//************************************************************************************************
//��Փx�ύX�����Ƀ��[�X���X�^�[�g����n�߂�
//************************************************************************************************
void RestartRace() {
	//�E�B���h�E�n���h��
	HWND hWnd = NULL;
	//�v���C�̉��
	ReleasePlay();
	//�v���C�̏�����
	InitPlay(hWnd);
}