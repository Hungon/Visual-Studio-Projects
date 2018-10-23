#ifndef		SELECTSTAGE_H__
#define		SELECTSTAGE_H__

//���񋓌^�̒�`
//�X�e�[�W�̎��
enum tagSELECTSTAGE{

	STAGE_VACANT = -1	,
	STAGE_SEA					,	//�C�̃X�e�[�W
	STAGE_ROAD					,   //����̃X�e�[�W
	STAGE_OFF_ROAD				,	//�I�t���[�h�X�e�[�W
	STAGE_MAX					,	//�X�e�[�W�̑���
};
//each scenes in SelectStage
enum tagSELECTSTAGESCENE {
	SELECTSTAGE_SCENE_SELECT_STAGE,		//select stage to play
	SELECTSTAGE_SCENE_STAGE_BRIEF1,		//explain how to play in the stage
	SELECTSTAGE_SCENE_STAGE_BRIEF2,		//explain how to play in the stage
	SELECTSTAGE_SCENE_STAGE_BRIEF3,		//explain how to play in the stage
	SELECTSTAGE_KIND_OF_SCENE,			//kind of scene
	SELECTSTAGE_KIND_OF_BRIEF,			//kind of brief
};

//���w�b�_�[���C���N���[�h
#include "graphics.h"

//�^�C�v�錾
typedef unsigned char COMPLETESTAGE;

//�� �}�N���̒�`
//�N���A�ς݃X�e�[�W�̃r�b�g��
const COMPLETESTAGE	SELECTSTAGE_STAGE_NOT_COMPLETE	 = 0;
const COMPLETESTAGE	SELECTSTAGE_STAGE_COMPLETE		 = 1;

const unsigned int SELECTSTAGE_IMAGE_MAX = 10;		//�摜�̖���
const unsigned int SELECTSTAGE_KIND_OF_IMAGE = 7;	//�摜�̎��

//���ʉ��ݒ�
const unsigned int SELECTSTAGE_EFFECT_SOUND_MAX = 3;//���ʉ���


//�摜�ݒ�
//�w�i
const unsigned int SELECTSTAGE_BACKGROUND_IMAGE_WIDTH	= 640;		//�摜�̕�
const unsigned int SELECTSTAGE_BACKGROUND_IMAGE_HEIGHT	= 480;		//�摜�̍���

//�������
const unsigned int SELECTSTAGE_BRIEF_IMAGE_WIDTH	= 640;		//�摜�̕�
const unsigned int SELECTSTAGE_BRIEF_IMAGE_HEIGHT	= 480;		//�摜�̍���
const float SELECTSTAGE_BRIEF_MOVE_SPEED_X = 12.0f;				//change speed of brief
//set character for the brief
const float	SELECTSTAGE_IMAGE_SEA_BRIEF_CHARACTER_INDICATE_Y = 175.0f;		//for swimer
const float	SELECTSTAGE_IMAGE_ROAD_BRIEF_CHARACTER_INDICATE_Y = 150.0f;		//for runner
const float	SELECTSTAGE_IMAGE_OFFROAD_BRIEF_CHARACTER_INDICATE_Y = 150.0f;	//for rider
//set words for describe each characters' action 
//SEA
const int	SELECTSTAGE_IMAGE_WORDS_FOR_SWIMER_WIDTH = 128;
const int	SELECTSTAGE_IMAGE_WORDS_FOR_SWIMER_HEIGHT = 32;
#define		SELECTSTAGE_IMAGE_KIND_OF_WORD_FOR_SWIMER  3
//ROAD
const int	SELECTSTAGE_IMAGE_WORDS_FOR_RUNNER_WIDTH = 80;
const int	SELECTSTAGE_IMAGE_WORDS_FOR_RUNNER_HEIGHT = 32;
#define		SELECTSTAGE_IMAGE_KIND_OF_WORD_FOR_RUNNER  2
//OFF-ROAD
const int	SELECTSTAGE_IMAGE_WORDS_FOR_RIDER_WIDTH = 160;
const int	SELECTSTAGE_IMAGE_WORDS_FOR_RIDER_HEIGHT = 32;
#define		SELECTSTAGE_IMAGE_KIND_OF_WORD_FOR_RIDER   4

//�X�e�[�W�I�𕶎���
const unsigned int SELECTSTAGE_STAGE_TOP_TITLE_IMAGE_WIDTH	= 445;		//�摜�̕�
//�e��X�e�[�W������C���[�W
const unsigned int SELECTSTAGE_STAGE_TITLE_IMAGE_WIDTH		= 320;		//�摜�̕�
const unsigned int SELECTSTAGE_STAGE_TITLE_IMAGE_HEIGHT		= 96;		//�摜�̍���
const  float SELECTSTAGE_STAGE_TITLE_IMAGE_INDICATE_Y		= 120.0f;	//�\����Y
const  float SELECTSTAGE_STAGE_TITLE_IMAGE_INDICATE_SPACE_X	= 20.0f;	//�\����X�\���Ԋu
const  float SELECTSTAGE_STAGE_TITLE_IMAGE_DEFAULT_SCALE	= 0.5f;		//�g�k�{��
//�X�e�[�W�I�𒆂ň����摜�̎�ށi�J�[�\���ȊO�j
const unsigned int SELECTSTAGE_SCENE_SELECT_STAGE_KIND_OF_IMAGE = 5;	//�摜�̎��

//�J�[�\��
const  float SELECTSTAGE_CURSOR_DEFAULT_ALPHA	= 0.5f;					//���ߔ{��
//����A����������t���Ȃ�����
const int	SELECTSTAGE_CURSOR_DONT_ACCEPT_CONTROL_TIME = 20;


//���֐��錾
void	InitSelectStage( HWND hWnd );
void	UpdateSelectStage();
void	DrawSelectStage();
void	ReleaseSelectStage();
//�N���A�σX�e�[�W��ݒ肷��
void	SetCompleteStage( COMPLETESTAGE complete , tagSELECTSTAGE stage );
//�N���A�ς݃X�e�[�W���擾
COMPLETESTAGE	*GetCompleteStage();
/*
control cursor
*/
void	SelectStageControlCursor();
/*
Select stage to play
*/
void	SelectStageChoiceStage();
/*
explain each stages' brief
*/
void	SelectStageExplainStageBrief1();
/*
explain each stages' brief
*/
void	SelectStageExplainStageBrief2();
/*
explain each stages' brief
*/
void	SelectStageExplainStageBrief3();
/*
Initialize character for explain the brief in the stage
*/
void	InitializeCharacterForBrief();
/*
Update character's action for the explanation to sea's stage
*/
void	UpdateCharacterForBriefInSea();
/*
Update character's action for the explanation to road's stage
*/
void	UpdateCharacterForBriefInRoad();
/*
Update character's action for the explanation to off-road's stage
*/
void	UpdateCharacterForBriefInOffRoad();

#endif