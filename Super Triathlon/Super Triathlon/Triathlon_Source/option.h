#ifndef		OPTION_H__
#define		OPTION_H__

#include	"graphics.h"
#include	"record.h"
#include	"SelectStage.h"
#include	"talk.h"
#include	"score.h"

//�� �񋓌^�̒�`
//transition in option
enum tagOPTIONTRANSITION{

	OPTION_TRANSITION_VACANT = -1,			//not transition
	OPTION_TRANSITION_SELECTION,			//select function
	OPTION_TRANSITION_SELECT_RECORD,		//select view record
	OPTION_TRANSITION_DURING_RECORD_VIEW,	//during view best record
	OPTION_TRANSITION_RESET_RECORD,			//reset best record
	OPTION_TRANSITION_CHANGE_LANGUAGE,		//change curent language
	OPTION_TRANSITION_SELECT_RANKING,		//just select ranking and choice game's level
	OPTION_TRANSIITON_DURING_VIEW_RANKING,	//during viewing ranking records
	OPTION_KIND_OF_TRANSITION,				//kind of transition
};
//�I�v�V�����@�\
enum tagOPTIONFUNCTION {
	OPTION_FUNCTION_VACANT = -1,
	OPTION_FUNCTION_RECORD,				//view best record
	OPTION_FUNCTION_RESET,				//reset best record
	OPTION_FUNCTION_LANGUAGE,			//change curent language
	OPTION_FUNCTION_RANKING,			//view ranking in the past
	OPTION_KIND_OF_FUNCTION,			//kind of function
};

//change flag of current language for explanation
enum tagOPTIONLANGUAGE {
	OPTION_DONT_CHANGE_LANGUAGE,
	OPTION_CHANGE_LANGUAGE,
	OPTION_KIND_OF_EXPLANATION,
};

typedef char RESET;
//reset record flag
const RESET OPTION_DONT_RESET = 0;
const RESET OPTION_DID_RESET = 1;

//���}�N���̒�`
#define OPTION_IMAGE_MAX	8		//�g�p�摜����

#define OPTION_FILE_MAX		5		//�t�@�C���̐�

#define OPTION_EFFECT_SOUND_MAX	3	//���ʉ��̐�

//�e��摜�ݒ�

//Function Image
const int OPTION_FUNCTION_IMAGE_COMMON_HEIGHT = 42;			//CommonImage
const int OPTION_FUNCTION_IMAGE_RECORD_WIDTH = 120;			//RecordImage
const int OPTION_FUNCTION_IMAGE_RESET_WIDTH = 96;			//ResetImage
const float OPTION_FUNCTION_DEFAULT_SCALE = 1.25f;			//default scale
const float OPTION_FUNCTION_RECORD_INDICATE_Y = 100.0f;
const float OPTION_FUNCTION_RECORD_INDICATE_SPACE_Y = 20.0f;

//Level image
const float	OPTION_GAME_LEVEL_BEGINING_INDICATE_Y = 167.0f;		//top coordinateY

//Stage Image
const float OPTION_SELECT_STAGE_INDICATE_Y = 50.0f;			//�\����Y
const float OPTION_SELECT_STAGE_INDICATE_SPACE_Y = 25.0f;	//�\���ԊuY
const float OPTION_DEFAULT_SELECT_IMAGE_SCALE = 0.75f;		//�g�k�{��

//Select Answer
const float OPTION_ANSWER_INDICATE_Y = 150.0f;
const float OPTION_ANSWER_INDICATE_SPACE_Y = 20.0f;
const float OPTION_ANSWER_IMAGE_SCALE = 1.0f;

//language Image
const int OPTION_LANGUAGE_IMAGE_COMMON_HEIGHT = 42;			//Common
const int OPTION_LANGUAGE_IMAGE_JAPANESE_WIDTH = 120;		//JapaneseImage
const int OPTION_LANGUAGE_IMAGE_ENGLISH_WIDTH = 96;			//EnglishImage
const float OPTION_LANGUAGE_DEFAULT_SCALE = 1.25f;			//default scale
const float OPTION_LANGUAGE_JAPANESE_INDICATE_Y = 170.0f;
const float OPTION_LANGUAGE_INDICATE_SPACE_Y = 20.0f;

//Three stage's direction
//one of the score's direction, which is gradually direction
//interval time for increase number or decrease 
const int OPTION_SCORE_FIRST_DIRECTION_GRADUALLY_DEFAULT_INTERVAL_TIME = 20;
const int OPTION_SCORE_SECOND_DIRECTION_GRADUALLY_DEFAULT_INTERVAL_TIME = 20;
const int OPTION_SCORE_THIRD_DIRECTION_GRADUALLY_DEFAULT_INTERVAL_TIME = 20;
const int OPTION_SCORE_FOURTH_DIRECTION_GRADUALLY_DEFAULT_INTERVAL_TIME = 20;
const int OPTION_SCORE_FIFTH_DIRECTION_GRADUALLY_DEFAULT_INTERVAL_TIME = 20;


//�e��X�e�[�W�̃X�R�A�̐F
//�ʏ펞
const tagSCORECOLOR OPTION_SCORE_COLOR_COMMON_USUAL = SCORE_GREEN;
//�X�V��
const tagSCORECOLOR OPTION_SCORE_COLOR_COMMON_RENEWAL = SCORE_YELOW;

//dont control time
const int OPTION_DONT_CONTROL_TIME = 10;

//Explain function
const float OPTION_EXPLAIN_WORDS_INDICATE_Y = 400.0f;
const int OPTION_EXPLAIN_FUNCTION_FONT_SIZE = 18;
const COLORREF OPTION_EXPLAIN_FUNCTION_FONT_COLOR = RGB(255, 255, 255);

//���v���g�^�C�v�錾
void InitOption( HWND hWnd );	//������
void UpdateOption();			//�X�V
void DrawOption();				//�\��
void ReleaseOption();			//���

/*
************************************************************************************************
�t�@�C���̓ǂݍ���
************************************************************************************************
*/
void	LoadOptionStageFile( RECORD *lpRecord , TALK *lpTalk , char stageNum , tagRENEWAL recordType );
/*
�J�[�\���̈ړ�
*/
void	OptionCursorMove();
/*
************************************************************************************************
�J�[�\���̈ړ�����
************************************************************************************************
*/
void	ConstrainOptionCursor();
/*
************************************************************************************************
�L�^�{������
************************************************************************************************
*/
void	OptionSelectStageRecord();

/*
�ŏI���ʉ{��
*/
void	OptionEndRecordView();
/*
�L�^�{�����L�����Z��
*/
void	OptionCancelRecordView();
/*
************************************************************************************************
�X�R�A�\���p�̐ݒ�
************************************************************************************************
*/
void	SetOptionStageScoreDirection(SCOREVALUE *lpScore, int recordNum, int material);
/*
�ŏI���ʃX�R�A�\���p�̐ݒ�
*/
void	SetOptionEndScoreDirection();
/*
****************************************************************************************
�X�R�A�\���p�ɒl��ݒ�
****************************************************************************************
*/
void	SetOptionStageScoreForDraw( SCOREVALUE *lpScore , RECORD *lpRecord , char stageNum , tagRECORDMATRIX itemType );
/*
�I�v�V�����@�\�̑I��
*/
void	OptionSelectFunction();
/*
�L�^�{������L�^���{����������ʂւ̑J��
*/
void	OptionRecordViewToSelectRecordView();
/*
Reset best record function
return = record reset flag
*/
void	OptionFunctionResetRecord();
/*
Explain selected function
*/
void	OptionDrawExplainFunction();
/*
Explain selected reset record
*/
void	OPtionDrawExplainResetRecord();
/*
one of function, that is change current language
*/
void	OptionFunctionChangeLanguage();
/*
explain to change current language
*/
void	OPtionDrawExplainLanguage();
/*
select ranking
draw each game's level
*/
void	OptionSelectRanking();
/*
Initialize score for ranking
*/
void InitializeRankingScoreInOption();
/*
viewing ranking
*/
void	ViewingRanking();

#endif