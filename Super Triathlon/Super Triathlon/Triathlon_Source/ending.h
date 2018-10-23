#ifndef		ENDING_H__
#define		ENDING_H__

//���w�b�_�[���C���N���[�h
#include "graphics.h"

//�� �ŏI���ʃX�R�A���e
enum tagENDTHERESULTSCORE{
	ENDING_SCORE_CHAIN,				//CHAIN
	ENDING_SCORE_TIME			,	//����
	ENDING_SCORE_TOTAL_POINT	,	//�W�v�|�C���g
	ENDING_KIND_OF_SCOREVALUE	,	//�\������X�R�A�̎�ސ�
	ENDING_SCORE_AND_TITLE		,	//�e��^�C�g����
};

// �����ʕ\���t���O�p
enum tagENDTHERESULTITEM{
	ENDING_ITEM_SEA		,
	ENDING_ITEM_ROAD	,
	ENDING_ITEM_OFFROAD ,
	ENDING_ITEM_END		,
	ENDING_ITEM_MAX		,
};
//scene transition in ending scene
enum tagENDINGTRANSITION {
	ENDING_SCENE_STAGE_RECORD,				//stage's record
	ENDING_SCENE_THE_TERMINATE_RECORD,		//the terminate result
	ENDING_SCENE_TERMINATE_RANKING,			//the end ranking
	ENDING_SCENE_CREDIT,					//the end credit
	ENDING_KIND_SCENE,						//kind of scene
};
//�� �G���h�N���W�b�g�p�̉摜�v�f�ԍ�
enum tagENDINGENDTHECREDIT{

	ENDING_CREDIT_MATERIAL_PROVIDERS					,
	ENDING_CREDIT_BGM_PROVIDERS							,
	ENDING_CREDIT_SOUND_EFFECT_PROVIDERS				,
	ENDING_CREDIT_BGM_PROVIDER_TAM						,
	ENDING_CREDIT_BGM_AND_SOUND_EFFECT_PROVIDER_CIRCUIT	,
	ENDING_CREDIT_SOUND_EFFECT_PROVIDER_ONJIN			,
	ENDING_CREDIT_TANKS_PROVIDERS						,
	ENDING_CREDIT_PRESENTED_OWN_NAME					,
	ENDING_CREDIT_TANKS_PLAYERS							,
	ENDING_CREDIT_MAX									,
};

//�� �}�N���̒�`
const int ENDING_IMAGE_MAX = 6;						//�摜����

//�e��\���ݒ�
//�G���f�B���O�V�[���摜
const int ENDING_IMAGE_END_SCENE_WIDTH	= 640;		//��
const int ENDING_IMAGE_END_SCENE_HEIGHT = 4000;		//����

//�ŏI���ʔw�i�摜
const int ENDING_IMAGE_END_RESULT_SCENE_WIDTH	= 640;		//��
const int ENDING_IMAGE_END_RESULT_SCENE_HEIGHT	= 480;		//����

//�e�팋�ʕ\��������̐ݒ�
const int	ENDING_END_THE_RESULT_IMAGE_WDIHT			 = 320;		//��
const int	ENDING_END_THE_RESULT_IMAGE_HEIGHT			 = 96;		//����
const float ENDING_END_THE_RESULT_IMAGE_INDICATE_SPACE_Y = 100.0f;	//�\���ԊuY
const float ENDING_END_RESULT_IMAGE_INDICATE_X			 = 640.0f;	//�\����X
const float ENDING_END_RESULT_IMAGE_INDICATE_Y			 = 10.0f;	//�\����Y
const float ENDING_END_RESULT_IMAGE_MOVE_X				 = -10.0f;	//�ړ���X
const float ENDING_END_RESULT_IMAGE_DEFAULT_SCALE		 = 0.75f;	//�g��{��

//�L�^�\���ݒ�
//����
const float ENDING_END_THE_RESULT_TIME_DEFAULT_SCALE		= 0.5f;	//�g��{��
const int	ENDING_THE_RESULT_TIME_DIGIT					= 2;	//����
//�W�v�|�C���g
const float ENDING_END_THE_RESULT_TOTAL_POINT_DEFAULT_SCALE = 0.75f;//�g��{��
const int	ENDING_THE_RESULT_TOTAL_POINT_DIGIT				= 6;	//����

//�N���W�b�g�p�摜�ݒ�
//�N���W�b�g��ʃX�N���[�����x
const float ENDING_END_THE_CREDIT_SCROLL_SPEED_Y		 = 1.0f;
const int	ENDING_END_THE_CREDIT_IMAGE_WDIHT			 = 640;		//��
const int	ENDING_END_THE_CREDIT_IMAGE_HEIGHT			 = 96;		//����
const float ENDING_END_THE_CREDIT_IMAGE_INDICATE_SPACE_Y = 370.0f;	//�\���ԊuY
const float ENDING_END_CREDIT_IMAGE_INDICATE_X			 = 640.0f;	//�\����X
const float ENDING_END_CREDIT_IMAGE_ABSOLUTE_MOVE_X		 = 5.5f;	//�ړ���X�i��Βl�j
const float ENDING_END_CREDIT_IMAGE_DEFAULT_SCALE		 = 0.75f;	//�g��{��

//each last result
//chain
const float ENDING_LAST_RESULT_CHAIN_INDICATE_X = 340.0f;
const float ENDING_LAST_RESULT_CHAIN_INDICATE_Y = 150.0f;
//time
const float ENDING_LAST_RESULT_TIME_INDICATE_X = 350.0f;
const float ENDING_LAST_RESULT_TIME_INDICATE_Y = 250.0f;
//total
const float ENDING_LAST_RESULT_TOTAL_POINT_INDICATE_X = 350.0f;
const float ENDING_LAST_RESULT_TOTAL_POINT_INDICATE_Y = 350.0f;


//���֐��錾
void InitEnding(HWND hWnd);		//������
void UpdateEnding();			//�X�V
void DrawEnding();				//�\��
void ReleaseEnding();			//���
/*
*************************************************************************
�t�@�C���̕\���X�V����
*************************************************************************
*/
void	UpdateCreateFile();
/*
*************************************************************************
�ŏI���ʕ\���X�V����
*************************************************************************
*/
void	UpdateEndTheResult();
/*
*************************************************************************
�G���f�B���O�摜�X�N���[������
*************************************************************************
*/
void	UpdateEndingScrollScene();
/*
Initialize each competitor's terminate result
*/
void InitializeTerminateResult();
/*
Update termiinate result
Draw each record's score
*/
void	UpdateTerminateRankingResult();
/*
Update stage's record
each stage's record draw score to consecutive 
*/
void	UpdateEndingStageRecord();
/*
Update the terminate record
*/
void	UpdateTerminateRecord();
/*
End ranking process to the end credit
*/
void	EndRankingToTheEndCredit();

#endif