//using strcat
#pragma warning ( disable : 4996 )

#include	"option.h"
#include	"sound.h"
#include	"input.h"
#include	"wipe.h"
#include	"play.h"
#include	"game.h"
#include	"ending.h"
#include	"map.h"
#include	"result.h"
#include	"menu.h"
#include	"opening.h"
#include	"result.h"

//���O���[�o���ϐ�

//�I�v�V��������
CHARACTER	g_OptionFunction[OPTION_KIND_OF_FUNCTION] = {0};

//�X�e�[�W�Z���N�g�摜
CHARACTER	g_OptionSelectStage[ STAGE_MAX ] = {0};

//�ŏI���ʕ\���摜
CHARACTER	g_OptionEndTheResult[ENDING_SCORE_AND_TITLE] = { 0 };

//�J�[�\��
CHARACTER	g_OptionCursor = {0};

//�摜������
int		g_OptionImg[OPTION_IMAGE_MAX] = {0};

//�t�@�C��������
TALK	g_OptionFile[OPTION_FILE_MAX] = {0};

//�I�v�V�����̑J�ڃt���O
tagOPTIONTRANSITION	g_OPtionTransitionFlagNext = OPTION_TRANSITION_VACANT;
tagOPTIONTRANSITION	g_OPtionTransitionFlagNow = OPTION_TRANSITION_VACANT;

//�{������X�e�[�W�ԍ�
tagSELECTSTAGE	 g_OptionRecordView = STAGE_VACANT;

//���ʉ��̗v�f������
int		g_OptionEffectSound[OPTION_EFFECT_SOUND_MAX] = {0};

//�L�^�{���t���O(�X�e�[�W�I����)
bool	g_OptionViewStageRecordFlag = false;
//�ŏI���ʉ{���t���O
bool	g_OptionViewEndRecordFlag = false;

//�L�^�\���p
SCOREVALUE	g_OptionScore[RECORD_MAX][RECORD_MAX] = { 0 };		//�X�e�[�W�p
SCOREVALUE	g_OptionEndScore[ENDING_KIND_OF_SCOREVALUE] = { 0 };//�ŏI���ʗp

//constrain cursor move
float g_constrainCursorMoveTop = 0.0f;
float g_constrainCursorMoveBottom = 0.0f;

//select answer
CHARACTER	g_OptionAnswer[2] = { 0 };
//Reset record flag
RESET	g_OptionResetRecordFlag = OPTION_DONT_RESET;

//set current language to drawing
CHARACTER	g_OptionLanguageCh[GAME_KIND_OF_LANGUAGE] = {0};
//change flag of current language for explanation
tagOPTIONLANGUAGE	g_OptionLanguageFlag = OPTION_DONT_CHANGE_LANGUAGE;
//kind of language during selection
tagGAMELANGUAGE		g_OptionLanguageNow = GAME_LANGUAGE_JAPANESE;

//kind of game's level
CHARACTER	g_OptionLevelCh[LEVEL_MAX] = { 0 };
//select game's level for view ranking
tagGAMELEVEL	g_OptionLevelFlag = EASY_MODE;
//records of the half way
SCOREVALUE	g_OptionTheHalWayRecord[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT][RECORD_KIND_OF_PARTICIPANT] = { 0 };
//ranking for the half way result
SCOREVALUE	g_OptionTheHalfRanking[RECORD_KIND_OF_PARTICIPANT] = { 0 };
//record's title image
CHARACTER	g_OptionRecordCh[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = { 0 };


//************************************************************************************************
//������
//************************************************************************************************
void InitOption( HWND hWnd ){
	
	//�g�p����摜
	char	*lpImageTbl[ OPTION_IMAGE_MAX ] = {
		
		{ "Image\\option.bmp" }			,			//OPTION�摜
		{ "Image\\OptionItem.bmp" }		,			//�I�v�V��������
		{ "Image\\SelectStage.bmp" }	,			//�X�e�[�W�Z���N�g�摜
		{ "Image\\cursor.bmp" }			,			//�J�[�\���摜
		{ "Image\\EndTheResult.bmp" }	,			//�ŏI���ʕ�����摜
		{ "Image\\select.bmp"},						//image yes no
		{ "Image\\Language.bmp" },					//using language
		{ "Image\\OpeningButton.bmp"},				//game's level image
	};
	//�g�p������ʉ��f�[�^
	char	*lpSoundTbl[ OPTION_EFFECT_SOUND_MAX ] = {

		{ "SE\\cursor.wav" }	,				//�J�[�\����
		{ "SE\\ok.wav" }		,				//���艹
		{ "SE\\cancel.wav" }	,				//�L�����Z����	
	};

	//�� �摜�̓ǂݍ���
	for( int i = 0 ; i < OPTION_IMAGE_MAX ; i++ ){ 
		g_OptionImg[i] = CreateDDImage( lpImageTbl[i] , hWnd );
	}

	//�� ���f�[�^�̓ǂݍ���
	for( int i = 0 ; i < OPTION_EFFECT_SOUND_MAX ; i++ ){ 
		g_OptionEffectSound[i] = CreateSound( lpSoundTbl[i] );
	}

	//�� �Ȃ̓ǂݍ���
	CreateStreaming("BGM\\option.wav");

	//�� �X�R�A�̏�����
	InitScore( hWnd );

	//�� �e��{���t���O
	//�{������
	g_OptionRecordView = STAGE_VACANT;
	//�L�^�{���t���O(�X�e�[�W�I����)
	g_OptionViewStageRecordFlag = false;
	//�ŏI���ʉ{���t���O
	g_OptionViewEndRecordFlag = false;
	//�I�v�V�����̋@�\�I���t���O
	g_OPtionTransitionFlagNext = OPTION_TRANSITION_SELECTION;
	g_OPtionTransitionFlagNow = OPTION_TRANSITION_SELECTION;
	//Reset record flag
	g_OptionResetRecordFlag = OPTION_DONT_RESET;
	//change flag of current language for explanation
	g_OptionLanguageFlag = OPTION_DONT_CHANGE_LANGUAGE;
	//kind of language during selection
	g_OptionLanguageNow = GAME_LANGUAGE_JAPANESE;


	//�� �e��摜�ݒ�
	//select answer
	float imageHeight1 = MENU_IMAGE_ANSWER_HEIGHT * OPTION_ANSWER_IMAGE_SCALE;
	float imageWidth = MENU_IMAGE_ANSWER_WIDTH * OPTION_ANSWER_IMAGE_SCALE;
	for (int i = 0; i < 2; i++) {
		g_OptionAnswer[i].x = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(imageWidth);
		g_OptionAnswer[i].y = OPTION_ANSWER_INDICATE_Y + 
			((imageHeight1 + OPTION_ANSWER_INDICATE_SPACE_Y) * i);
		g_OptionAnswer[i].w = MENU_IMAGE_ANSWER_WIDTH;
		g_OptionAnswer[i].h = MENU_IMAGE_ANSWER_HEIGHT;
		g_OptionAnswer[i].picy = MENU_IMAGE_ANSWER_HEIGHT * i;
		g_OptionAnswer[i].scale = OPTION_ANSWER_IMAGE_SCALE;
	}

	//set each other function
	int widthTbl[OPTION_KIND_OF_FUNCTION] = { 
		OPTION_FUNCTION_IMAGE_RECORD_WIDTH,				//record
		OPTION_FUNCTION_IMAGE_RESET_WIDTH,				//reset
		OPTION_FUNCTION_IMAGE_RECORD_WIDTH,				//language
		OPTION_FUNCTION_IMAGE_RECORD_WIDTH,				//ranking
	};
	float imageHeight2 = OPTION_FUNCTION_IMAGE_COMMON_HEIGHT * OPTION_FUNCTION_DEFAULT_SCALE;
	for (int i = 0; i < OPTION_KIND_OF_FUNCTION; i++) {

		g_OptionFunction[i].y = OPTION_FUNCTION_RECORD_INDICATE_Y + 
			((imageHeight2 + OPTION_FUNCTION_RECORD_INDICATE_SPACE_Y) * i);
		g_OptionFunction[i].w = widthTbl[i];
		g_OptionFunction[i].h = OPTION_FUNCTION_IMAGE_COMMON_HEIGHT;
		g_OptionFunction[i].scale = OPTION_FUNCTION_DEFAULT_SCALE;
		g_OptionFunction[i].flag = true;
		g_OptionFunction[i].x = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(g_OptionFunction[i].w * g_OptionFunction[i].scale);
		g_OptionFunction[i].picy = OPTION_FUNCTION_IMAGE_COMMON_HEIGHT * i;
	}

	//constrain cursor move position
	g_constrainCursorMoveTop = g_OptionFunction[OPTION_FUNCTION_RECORD].y;						//top position
	g_constrainCursorMoveBottom = g_OptionFunction[OPTION_FUNCTION_RANKING].y;					//bottom position

	//select current language
	//each other's image width
	int widthTbl1[GAME_KIND_OF_LANGUAGE] = { OPTION_LANGUAGE_IMAGE_JAPANESE_WIDTH,OPTION_LANGUAGE_IMAGE_ENGLISH_WIDTH };
	//common high height
	float imageHeight3 = OPTION_LANGUAGE_IMAGE_COMMON_HEIGHT * OPTION_LANGUAGE_DEFAULT_SCALE;
	//set each other
	for (int i = 0; i < GAME_KIND_OF_LANGUAGE; i++) {
		g_OptionLanguageCh[i].x = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(widthTbl1[i] * OPTION_LANGUAGE_DEFAULT_SCALE);
		g_OptionLanguageCh[i].y = OPTION_LANGUAGE_JAPANESE_INDICATE_Y + ((imageHeight3 + OPTION_LANGUAGE_INDICATE_SPACE_Y) * i);
		g_OptionLanguageCh[i].w = widthTbl1[i];
		g_OptionLanguageCh[i].h = OPTION_LANGUAGE_IMAGE_COMMON_HEIGHT;
		g_OptionLanguageCh[i].picx = 0;
		g_OptionLanguageCh[i].picy = (OPTION_LANGUAGE_IMAGE_COMMON_HEIGHT * i);
		g_OptionLanguageCh[i].scale = OPTION_LANGUAGE_DEFAULT_SCALE;
		g_OptionLanguageCh[i].flag = false;
	}


	//�� �L�^�{���\�X�e�[�W
	//�摜�̕\���T�C�Y���v�Z
	//����
	float	SelectSizeH = OPTION_DEFAULT_SELECT_IMAGE_SCALE * SELECTSTAGE_STAGE_TITLE_IMAGE_HEIGHT;
	//��
	float	SelectSizeW = OPTION_DEFAULT_SELECT_IMAGE_SCALE * SELECTSTAGE_STAGE_TITLE_IMAGE_WIDTH;
	//�e����
	for( int i = 0 ; i < STAGE_MAX ; i++ ){

		g_OptionSelectStage[i].y = OPTION_SELECT_STAGE_INDICATE_Y + 
			( SelectSizeH + OPTION_SELECT_STAGE_INDICATE_SPACE_Y ) * i;
		g_OptionSelectStage[i].w = SELECTSTAGE_STAGE_TITLE_IMAGE_WIDTH;
		g_OptionSelectStage[i].h = SELECTSTAGE_STAGE_TITLE_IMAGE_HEIGHT;
		g_OptionSelectStage[i].scale = OPTION_DEFAULT_SELECT_IMAGE_SCALE;
		g_OptionSelectStage[i].picy = SELECTSTAGE_STAGE_TITLE_IMAGE_HEIGHT * i;
		g_OptionSelectStage[i].x = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(
			g_OptionSelectStage[i].w * g_OptionSelectStage[i].scale);
	}

	//�J�[�\���̐ݒ�
	g_OptionCursor.x = g_OptionFunction[0].x;
	g_OptionCursor.y = g_OptionFunction[0].y;
	g_OptionCursor.w = (int)(g_OptionFunction[0].w * g_OptionFunction[0].scale);
	g_OptionCursor.h = (int)(g_OptionFunction[0].h * g_OptionFunction[0].scale);
	g_OptionCursor.alpha = 120.0f;
	g_OptionCursor.flag = true;
	//move
	g_OptionCursor.my = (imageHeight2 * OPTION_FUNCTION_RECORD_INDICATE_SPACE_Y);

	//�e��ŏI���ʕ�����\���p�ݒ�
	//picY
	int picYTbl[ENDING_SCORE_AND_TITLE] = {
		0,															//title
		ENDING_END_THE_RESULT_IMAGE_HEIGHT * 3,						//chain
		ENDING_END_THE_RESULT_IMAGE_HEIGHT,							//time
		ENDING_END_THE_RESULT_IMAGE_HEIGHT * 2,						//total
	};
	for (int i = 0; i < ENDING_SCORE_AND_TITLE; i++) {
		g_OptionEndTheResult[i].x = 
			CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(ENDING_END_THE_RESULT_IMAGE_WDIHT * ENDING_END_RESULT_IMAGE_DEFAULT_SCALE);		//�\����X
		g_OptionEndTheResult[i].y = ENDING_END_RESULT_IMAGE_INDICATE_Y +
			(ENDING_END_THE_RESULT_IMAGE_INDICATE_SPACE_Y * i);								//�\����Y
		g_OptionEndTheResult[i].mx = ENDING_END_RESULT_IMAGE_MOVE_X - (i * 1.0f);			//�ړ���X
		g_OptionEndTheResult[i].h = ENDING_END_THE_RESULT_IMAGE_HEIGHT;						//����
		g_OptionEndTheResult[i].w = ENDING_END_THE_RESULT_IMAGE_WDIHT;						//��
		g_OptionEndTheResult[i].scale = ENDING_END_RESULT_IMAGE_DEFAULT_SCALE;				//�g�k�{��
		g_OptionEndTheResult[i].picy = picYTbl[i];											//�\����Y
		g_OptionEndTheResult[i].flag = false;												//�\���t���O
	}
	g_OptionEndTheResult[0].y = OPTION_SELECT_STAGE_INDICATE_Y +
		(SelectSizeH + OPTION_SELECT_STAGE_INDICATE_SPACE_Y) * 3;

	//subsutitute each last result for drawing variable
	FPOSITION posTbl[ENDING_KIND_OF_SCOREVALUE] = {
		{ ENDING_LAST_RESULT_CHAIN_INDICATE_X ,ENDING_LAST_RESULT_CHAIN_INDICATE_Y },
		{ ENDING_LAST_RESULT_TIME_INDICATE_X ,ENDING_LAST_RESULT_TIME_INDICATE_Y },
		{ ENDING_LAST_RESULT_TOTAL_POINT_INDICATE_X ,ENDING_LAST_RESULT_TOTAL_POINT_INDICATE_Y },
	};
	//score's direction type
	char directionTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		SCORE_GRADUALLY_DIRECT_ROLLING ,				//chain record
		SCORE_GRADUALLY_DIRECT_ROLLING ,				//time record
		SCORE_GRADUALLY_DIRECT_NORMAL  ,				//total point
	};
	//score's direction type, which is gradually direction
	//interval time of indication
	int intervalTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		0,																						//chain record
		0,																						//time record
		RESULT_SCORE_THE_HALF_WAY_RESULT_TOTAL_POINT_DIRECTION_GRADUALLY_INTERVAL_TIME,			//total point
	};
	//score's direction type, which is rolling direction
	//rolling time
	int rollingTimeTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		RESULT_SCORE_THE_HALF_WAY_RESULT_CHAIN_RECORD_DIRECTION_ROLLING_TIME,			//CHAIN record
		RESULT_SCORE_THE_HALF_WAY_RESULT_TIME_RECORD_DIRECTION_ROLLING_TIME,			//time record
		0,																				//total point
	};
	//set default digit
	int digitTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		RESULT_SCORE_THE_HALF_WAY_RESULT_CHAIN_RECORD_DIGIT,						//CHAIN record
		RESULT_SCORE_THE_HALF_WAY_RESULT_TIME_RECORD_DIGIT,							//time record
		RESULT_SCORE_THE_HALF_WAY_RESULT_TOTAL_POINT_DIGIT,							//total point
	};
	for (int i = 0; i < ENDING_KIND_OF_SCOREVALUE; i++) {
		g_OptionEndScore[i].score_x = posTbl[i].fX;
		g_OptionEndScore[i].score_y = posTbl[i].fY;
		g_OptionEndScore[i].spaceX = 10.0f;												//�\���ԊuX
		g_OptionEndScore[i].spaceY = 0.0f;												//�\���ԊuY
		g_OptionEndScore[i].score_scale = ENDING_END_THE_RESULT_TIME_DEFAULT_SCALE;		//�g�k��
		g_OptionEndScore[i].scoreColor = SCORE_BLUE;										//�\���F
		g_OptionEndScore[i].ch.type = SCORE_NUMBER_TYPE_NORMAL;							//�X�R�A�̕\�����
		g_OptionEndScore[i].ti.cntFlag = false;											//���Ԍo�߃t���O
		g_OptionEndScore[i].digit = digitTbl[i];											//���̐�
		g_OptionEndScore[i].gradually = true;												//���X�ɉ��Z�E���Z�t���O
		g_OptionEndScore[i].graduallyDirect = directionTbl[i];							//�X�R�A�̉��o
		g_OptionEndScore[i].rollTime = rollingTimeTbl[i];									//��]����
		g_OptionEndScore[i].renewalColor = SCORE_YELOW;									//�X�V���̕ύX�F
		g_OptionEndScore[ENDING_SCORE_TOTAL_POINT].numD = 0;								//�n�߂ɕ\�����鐔�l
		g_OptionEndScore[ENDING_SCORE_TOTAL_POINT].interval = intervalTbl[i];				//�\���Ԋu
	}

	//initialize each game's level
	for (int i = 0; i < LEVEL_MAX; i++) {
		g_OptionLevelCh[i].w = OPENING_BUTTON_IMAGE_WIDTH;					
		g_OptionLevelCh[i].h = OPENING_BUTTON_IMAGE_HEIGHT;					
		g_OptionLevelCh[i].picy = (OPENING_BUTTON_IMAGE_HEIGHT * 2) + 
			(i * OPENING_BUTTON_IMAGE_HEIGHT);								
		g_OptionLevelCh[i].flag = false;									
		g_OptionLevelCh[i].x = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(OPENING_BUTTON_IMAGE_WIDTH);
		g_OptionLevelCh[i].y = OPTION_GAME_LEVEL_BEGINING_INDICATE_Y +
			((OPENING_BUTTON_IMAGE_HEIGHT + OPTION_FUNCTION_RECORD_INDICATE_SPACE_Y) * i);
	}
	//select game's level for view ranking
	g_OptionLevelFlag = EASY_MODE;

	//initialize record's images
	//each indication X
	float indicationXTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		RESULT_IMAGE_THE_HALF_WAY_RESULT_TIME_RECORD_TITLE_INDICATE_X,			//time record
		RESULT_IMAGE_THE_HALF_WAY_RESULT_TOTAL_POINT_TITLE_INDICATE_X,			//total point
		RESULT_IMAGE_THE_HALF_WAY_RESULT_CHAIN_RECORD_TITLE_INDICATE_X,			//CHAIN record
	};
	//each indication Y
	float indicationYTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		RESULT_IMAGE_THE_HALF_WAY_RESULT_TIME_RECORD_TITLE_INDICATE_Y,			//time record
		RESULT_IMAGE_THE_HALF_WAY_RESULT_TOTAL_POINT_TITLE_INDICATE_Y,			//total point
		RESULT_IMAGE_THE_HALF_WAY_RESULT_CHAIN_RECORD_TITLE_INDICATE_Y,			//CHAIN record
	};
	//each scale rate
	float scaleTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		RESULT_IMAGE_THE_HALF_WAY_RESULT_TIME_RECORD_TITLE_SCALE_RATE,
		RESULT_IMAGE_THE_HALF_WAY_RESULT_TOTAL_POINT_TITLE_SCALE_RATE,
		RESULT_IMAGE_THE_HALF_WAY_RESULT_CHAIN_RECORD_TITLE_SCALE_RATE,
	};
	for (int i = 0; i < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; i++) {
		g_OptionRecordCh[i].x = 640.0f +
			((ENDING_END_THE_RESULT_IMAGE_WDIHT * scaleTbl[i]) * i);
		g_OptionRecordCh[i].ex = indicationXTbl[i];
		g_OptionRecordCh[i].y = indicationYTbl[i];
		g_OptionRecordCh[i].mx = ENDING_END_RESULT_IMAGE_MOVE_X - (i * 2.0f);
		g_OptionRecordCh[i].h = ENDING_END_THE_RESULT_IMAGE_HEIGHT;
		g_OptionRecordCh[i].w = ENDING_END_THE_RESULT_IMAGE_WDIHT;
		g_OptionRecordCh[i].scale = scaleTbl[i];
		g_OptionRecordCh[i].picy = ENDING_END_THE_RESULT_IMAGE_HEIGHT * (i + 1);
		g_OptionRecordCh[i].flag = false;
	}

	//initialize ranking image
	for (int i = 0; i < RECORD_KIND_OF_PARTICIPANT; i++) {
		g_OptionTheHalfRanking[i].score_x = RESULT_SCORE_THE_HALF_WAY_RESULT_RANKING_INDICATE_X;
		g_OptionTheHalfRanking[i].score_y = RESULT_SCORE_THE_HALF_WAY_RESULT_RANKING_INDICATE_Y +
			(RESULT_SCORE_THE_HALF_WAY_RESULT_RANKING_INDICATE_SPACE_Y * i);
		g_OptionTheHalfRanking[i].score_scale = RESULT_SCORE_THE_HALF_WAY_RESULT_RANKING_SCALE_RATE;
		g_OptionTheHalfRanking[i].num = (i + 1);
		g_OptionTheHalfRanking[i].ch.flag = false;
		g_OptionTheHalfRanking[i].scoreColor = SCORE_RANING_COLOR_WHITE;
	}
	g_OptionTheHalfRanking[0].scoreColor = SCORE_RANING_COLOR_RED;				//change color, white into red
	g_OptionTheHalfRanking[0].score_scale = RESULT_SCORE_THE_HALF_WAY_RESULT_RANKING_THE_FIRST_SCALE_RATE;
}


//************************************************************************************************
//�X�V
//************************************************************************************************
void UpdateOption(){
	
	//�Ȃ̍Đ�
	PlayStreaming( true );

	//�J�[�\���̋����i�ړ��ƈړ������j
	if (g_OptionCursor.flag) OptionCursorMove(); ConstrainOptionCursor();

	//�� �@�\�ɂ�蕪��
	if (g_OPtionTransitionFlagNow == g_OPtionTransitionFlagNext) {
		switch (g_OPtionTransitionFlagNow) {

			//�@�\�I����
		case OPTION_TRANSITION_SELECTION:
			//�����[�h�I��
			OptionSelectFunction();
			break;

			//�L�^�{���I����
		case OPTION_TRANSITION_SELECT_RECORD:
			//�L�^�{��
			OptionSelectStageRecord();									//�e��X�e�[�W
			if (g_OptionEndTheResult[0].flag) OptionEndRecordView();	//�ŏI����
			//�L�^���{�����L�����Z��
			OptionCancelRecordView();
			break;

			//�L�^�{����
		case OPTION_TRANSITION_DURING_RECORD_VIEW:
			//���@�ō��L�^��\��(�X�e�[�W�I����)
			if (g_OptionViewStageRecordFlag) {
				//dont draw ent the result image
				g_OptionEndTheResult[0].flag = false;
				//�Ō�̃t�@�C���܂œǂݍ���ł��Ȃ�������ǂݍ���
				if (g_OptionFile[OPTION_FILE_MAX - 1].fRead == false) {
					//�ǂݍ��݂����t�@�C�����܂Ń��[�v����
					for (int i = 2; i < OPTION_FILE_MAX; i++) {
						//�e�t�@�C�������Ԃɓǂݍ���ł���
						if (g_OptionFile[i].fRead == false) {
							//�����񐶐�
							CreateTalk(&g_OptionFile[i]);
							//�ǂݍ��񂾃t���O��true
							g_OptionFile[i].fRead = true;
						}
					}
				}
			}
			//�ŏI���ʉ{����
			if (g_OptionViewEndRecordFlag) {
				//�e�핶����摜�̕\��
				for (int i = 0; i < ENDING_SCORE_AND_TITLE; i++) { g_OptionEndTheResult[i].flag = true; };
			}
			//�X�e�[�W�L�^�{���I�����đI����
			OptionRecordViewToSelectRecordView();
			break;

			//Reset best record
		case OPTION_TRANSITION_RESET_RECORD:
			OptionFunctionResetRecord();
			break;

			//Change current language
		case OPTION_TRANSITION_CHANGE_LANGUAGE:
			OptionFunctionChangeLanguage();
			break;
			
			//to select stage
		case OPTION_TRANSITION_SELECT_RANKING:
			OptionSelectRanking();
			break;

			//viewing ranking
		case OPTION_TRANSIITON_DURING_VIEW_RANKING:
			ViewingRanking();
			break;

		default:
			break;
		}

	}

	//check transition flag
	if (g_OPtionTransitionFlagNow != g_OPtionTransitionFlagNext) {
		//count time
		g_OptionCursor.time++;
		//if cont time more than set time
		if (g_OptionCursor.time >= OPTION_DONT_CONTROL_TIME) {
			//time reset
			g_OptionCursor.time = 0;
			//substitute transition next flag for transition flag now
			g_OPtionTransitionFlagNow = g_OPtionTransitionFlagNext;
		}
	}
	
}

//************************************************************************************************
//�\��
//************************************************************************************************
void DrawOption(){

	//�X�R�A�\���֐��̕���
	void (*pscore[RECORD_MAX])(SCOREVALUE*) = {

		DrawScore	,			//�G�\��
		DrawScore	,			//�A�C�e���\��
		DrawTime	,			//���ԕ\��
		DrawScore	,			//CHAIN��
		DrawScore	,			//�W�v�|�C���g
	};

	//�����e���ڂ̐�
	//�@�C�X�e�[�W�F�G�����A�l���A�C�e���A���ԁA�G����CHAIN���A�W�v�|�C���g�̏���
	//�A���H�X�e�[�W�F�j�󂵂���Q���A��񂾃n�[�h���A���ԁA�n�[�h������CHAIN���A�W�v�|�C���g�̏���
	//�B�I�t���[�h�X�e�[�W�F���������A�N�V�����A���v�A�N�V����Pt�A���ԁA�A���A�N�V�������A�W�v�|�C���g�̏���
	char ScoreItemTbl[ STAGE_MAX ][ RECORD_MAX ] = {

		{ 5 , 5 , 1 , 1 , 1 } ,			//�C

		{ 2 , 3 , 1 , 1 , 2 } ,			//���H

		{ 4 , 1 , 1 , 1 , 1 } ,			//�I�t���[�h

	};


	//���摜�̕\��
	//�w�i�摜
	DDBlt( 0 , 0 , 640 , 480 , g_OptionImg[0] , 0 , 0 );

	//Select Answer
	for (int i = 0; i < 2; i++) {
		if (g_OptionAnswer[i].flag) {
			DDScaleBltSelectIndicate(
				g_OptionAnswer[i].x,
				g_OptionAnswer[i].y,
				g_OptionAnswer[i].w,
				g_OptionAnswer[i].h,
				g_OptionImg[5],
				g_OptionAnswer[i].picx,
				g_OptionAnswer[i].picy,
				g_OptionAnswer[i].scale,
				DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP);
			//diverge explanation to reset record
			OPtionDrawExplainResetRecord();
		}
	}

	//select current language
	for (int i = 0; i < GAME_KIND_OF_LANGUAGE; i++) {
		if (g_OptionLanguageCh[i].flag) {
			DDScaleBltSelectIndicate(
				g_OptionLanguageCh[i].x,
				g_OptionLanguageCh[i].y,
				g_OptionLanguageCh[i].w,
				g_OptionLanguageCh[i].h,
				g_OptionImg[6],
				g_OptionLanguageCh[i].picx,
				g_OptionLanguageCh[i].picy,
				g_OptionLanguageCh[i].scale,
				DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP);
			//explanation that change current language
			OPtionDrawExplainLanguage();
		}
	}
	
	//Function
	for (int i = 0; i < OPTION_KIND_OF_FUNCTION; i++) {
		if (g_OptionFunction[i].flag) {
			DDScaleBltSelectIndicate(
				g_OptionFunction[i].x,
				g_OptionFunction[i].y,
				g_OptionFunction[i].w,
				g_OptionFunction[i].h,
				g_OptionImg[1],
				g_OptionFunction[i].picx,
				g_OptionFunction[i].picy,
				g_OptionFunction[i].scale,
				DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP);
			//diverge explanation to function
			OptionDrawExplainFunction();

		}
	}

	//�e��X�e�[�W�Z���N�g�摜�̕\��
	for( int i = 0 ; i < STAGE_MAX ; i++ ){
		if(g_OptionSelectStage[i].flag){
			DDScaleBltSelectIndicate(						
				g_OptionSelectStage[i].x ,
				g_OptionSelectStage[i].y ,					
				g_OptionSelectStage[i].w ,
				g_OptionSelectStage[i].h ,					
				g_OptionImg[2] ,	
				g_OptionSelectStage[i].picx ,
				g_OptionSelectStage[i].picy ,
				g_OptionSelectStage[i].scale ,
				DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP);
		}
	}

	//�e��ŏI���ʕ�����摜
	for (int i = 0; i < ENDING_SCORE_AND_TITLE; i++)
	{
		if (g_OptionEndTheResult[i].flag)
		{
			DDScaleBltSelectIndicate(
				g_OptionEndTheResult[i].x,			//�\�����WX
				g_OptionEndTheResult[i].y,			//�\�����W�x
				g_OptionEndTheResult[i].w,			//��
				g_OptionEndTheResult[i].h,			//����
				g_OptionImg[4],						//�摜�̗v�f�ԍ�
				g_OptionEndTheResult[i].picx,		//�\����X
				g_OptionEndTheResult[i].picy,		//�\����Y
				g_OptionEndTheResult[i].scale,		//�g�k�{��
				DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP);
		}
	}

	//���e���ڂ̒l��\��
	//�X�e�[�W�̋L�^
	if (g_OptionViewStageRecordFlag) {
		for (int j = 0; j < RECORD_MAX; j++) {
			for (int i = 0; i < ScoreItemTbl[g_OptionRecordView][j]; i++) {
				//if draw ranking
				if (g_OptionRecordView == STAGE_ROAD && j == RECORD_TOTAL && i == 1) {
					//for ranking drawing
					DrawRanking(g_OptionScore[RECORD_TOTAL][1].score_x,
						g_OptionScore[RECORD_TOTAL][1].score_y,
						g_OptionScore[RECORD_TOTAL][1].score_scale,
						g_OptionScore[RECORD_TOTAL][1].num,
						(tagSCORERANKINGCOLOR)g_OptionScore[RECORD_TOTAL][1].scoreColor);
					break;
				}
				//�X�R�A�\���̕���
				pscore[j](&g_OptionScore[j][i]);
			}
		}
	}
	//�ŏI���ʂ̋L�^
	if (g_OptionViewEndRecordFlag) {
		//chain
		DrawScore(&g_OptionEndScore[ENDING_SCORE_CHAIN]);
		//����
		DrawTime(&g_OptionEndScore[ENDING_SCORE_TIME]);
		//�W�v�|�C���g
		DrawScore(&g_OptionEndScore[ENDING_SCORE_TOTAL_POINT]);
	}

	//draw each game's level for ranking
	for (int i = 0; i < LEVEL_MAX; i++) {
		if (g_OptionLevelCh[i].flag) {
			DDBlt(
				g_OptionLevelCh[i].x,
				g_OptionLevelCh[i].y,
				g_OptionLevelCh[i].w,
				g_OptionLevelCh[i].h,
				g_OptionImg[7],
				g_OptionLevelCh[i].picx,
				g_OptionLevelCh[i].picy);
		}
	}
	//kind of item for the half way result
	tagRECORDMATRIX halfRecordTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = { RECORD_TOTAL, RECORD_TIME };
	//draw the half way result
	for (int j = 0; j < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; j++) {
		for (int i = 0; i < RECORD_KIND_OF_PARTICIPANT; i++) {
			if (g_OptionTheHalWayRecord[j][i].ch.flag) {
				//draw score of the half way result
				pscore[halfRecordTbl[j]](&g_OptionTheHalWayRecord[j][i]);
			}
		}
	}
	//draw ranking for the half way result
	for (int i = 0; i < RECORD_KIND_OF_PARTICIPANT; i++) {
		if (g_OptionTheHalfRanking[i].ch.flag) {
			DrawRanking(g_OptionTheHalfRanking[i].score_x,
				g_OptionTheHalfRanking[i].score_y,
				g_OptionTheHalfRanking[i].score_scale,
				g_OptionTheHalfRanking[i].num,
				(tagSCORERANKINGCOLOR)g_OptionTheHalfRanking[i].scoreColor);
		}
	}
	//draw title of the half way record
	for (int i = 0; i < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; i++) {
		if (g_OptionRecordCh[i].flag) {
			//indication way is scaleBlt
			DDScaleBltSelectIndicate(
				g_OptionRecordCh[i].x,
				g_OptionRecordCh[i].y,
				g_OptionRecordCh[i].w,
				g_OptionRecordCh[i].h,
				g_OptionImg[4],
				g_OptionRecordCh[i].picx,
				g_OptionRecordCh[i].picy,
				g_OptionRecordCh[i].scale,
				DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP);
		}
	}

	//�J�[�\��
	if (g_OptionCursor.flag) {
		//�J�[�\��
		DDAlphaBlt(
			(int)g_OptionCursor.x,
			(int)g_OptionCursor.y,
			g_OptionCursor.w,
			g_OptionCursor.h,
			g_OptionImg[3],
			g_OptionCursor.picx,
			g_OptionCursor.picy,
			g_OptionCursor.alpha);
	}

}

//************************************************************************************************
//���
//************************************************************************************************
void ReleaseOption(){
	
	
	//�摜�̉��
	for( int i = 0 ; i < OPTION_IMAGE_MAX ; i++ ){ ReleaseDDImage( g_OptionImg[i] ); }
	//�t�@�C���̉��
	ZeroMemory( g_OptionFile , sizeof( TALK ) * OPTION_FILE_MAX );

	//���ʉ��̉��
	for( int i = 0 ; i < OPTION_EFFECT_SOUND_MAX ; i++ ){ ReleaseSound(g_OptionEffectSound[i]); }
	//�Ȃ̉��
	ReleaseStreaming();
	//�X�R�A�̉��
	ReleaseScore();
	//�ǂݍ��񂾃t�@�C���̉��
	ReleaseTalk();
}


/*
************************************************************************************************
�t�@�C���̓ǂݍ���
�ˈ�����RECORD�^�ATALK�^�ɓǂݍ��݂����t�@�C���̒l�ƃt�@�C���p�X��������
************************************************************************************************
*/
void	LoadOptionStageFile( RECORD *lpRecord , TALK *lpTalk , char stageNum , tagRENEWAL recordType ){

	//get the current language
	tagGAMELANGUAGE language = GetGameLanguage();
	//�� �e�X�e�[�W�̓ǂݍ��݃t�@�C���f�[�^�e�[�u��
	//for Japanese language
	char *lpJapaneseFileTbl[ STAGE_MAX ][ OPTION_FILE_MAX ] = {
		
		//�C�X�e�[�W
		{ "Data\\SeaStage\\PreviewScore.dat" ,								//�ߋ��̋L�^
		"Data\\SeaStage\\BestScore.dat" ,									//�ō��̋L�^
		"Data\\SeaStage\\Japanese\\ResultValue.dat" ,						//���ԁACHAIN��
		"Data\\SeaStage\\Japanese\\ResultEnemy.dat" ,						//�G������
		"Data\\SeaStage\\Japanese\\ResultItem.dat" }						//�l���A�C�e����
		,

		//���H�X�e�[�W
		{ "Data\\RoadStage\\PreviewScore.dat" ,								//�ߋ��̋L�^
		"Data\\RoadStage\\BestScore.dat" ,									//�ō��̋L�^
		"Data\\RoadStage\\Japanese\\ResultValue.dat" ,						//���ԁACHAIN��
		"Data\\RoadStage\\Japanese\\ResultObstacles.dat" ,					//��Q���j��
		"Data\\RoadStage\\Japanese\\ResultHurdle.dat" }						//��񂾃n�[�h����
		,
		
		//�I�t���[�h�X�e�[�W
		{ "Data\\OffRoadStage\\PreviewScore.dat" ,							//�ߋ��̋L�^
		"Data\\OffRoadStage\\BestScore.dat" ,								//�ō��̋L�^
		"Data\\OffRoadStage\\Japanese\\ResultValue.dat" ,					//���ԁA�A���A�N�V������
		"Data\\OffRoadStage\\Japanese\\ResultSuccessAction.dat" ,			//�����A�N�V����
		"Data\\OffRoadStage\\Japanese\\ResultTotalActionPoint.dat" }		//���v�A�N�V�����|�C���g
		,
	};
	//for English language
	char *lpEnglishFileTbl[STAGE_MAX][OPTION_FILE_MAX] = {

		//�C�X�e�[�W
		{ "Data\\SeaStage\\PreviewScore.dat" ,								//�ߋ��̋L�^
		"Data\\SeaStage\\BestScore.dat" ,									//�ō��̋L�^
		"Data\\SeaStage\\English\\ResultValue.dat" ,						//���ԁACHAIN��
		"Data\\SeaStage\\English\\ResultEnemy.dat" ,						//�G������
		"Data\\SeaStage\\English\\ResultItem.dat" }							//�l���A�C�e����
		,

		//���H�X�e�[�W
		{ "Data\\RoadStage\\PreviewScore.dat" ,								//�ߋ��̋L�^
		"Data\\RoadStage\\BestScore.dat" ,									//�ō��̋L�^
		"Data\\RoadStage\\English\\ResultValue.dat" ,						//���ԁACHAIN��
		"Data\\RoadStage\\English\\ResultObstacles.dat" ,					//��Q���j��
		"Data\\RoadStage\\English\\ResultHurdle.dat" }						//��񂾃n�[�h����
		,

		//�I�t���[�h�X�e�[�W
		{ "Data\\OffRoadStage\\PreviewScore.dat" ,							//�ߋ��̋L�^
		"Data\\OffRoadStage\\BestScore.dat" ,								//�ō��̋L�^
		"Data\\OffRoadStage\\English\\ResultValue.dat" ,					//���ԁA�A���A�N�V������
		"Data\\OffRoadStage\\English\\ResultSuccessAction.dat" ,			//�����A�N�V����
		"Data\\OffRoadStage\\English\\ResultTotalActionPoint.dat" }			//���v�A�N�V�����|�C���g
		,
	};

	//for fixed pattern file
	tagRESULTFIXEDFILE fixedFile[RESULT_KIND_OF_FIXED_PATTERN_FILE] = {
		RESULT_FIXED_PATTERN_FILE_1,			//file 1
		RESULT_FIXED_PATTERN_FILE_2,			//file 2
		RESULT_FIXED_PATTERN_FILE_3,			//file 3
	};
	//Set fixed pattern file
	for (int i = 2; i < (RESULT_KIND_OF_FIXED_PATTERN_FILE + 2); i++) {
		SetResultFixedPatternFile(&lpTalk[i], fixedFile[i - 2]);
	}

	//�{���������X�e�[�W�̃t�@�C����������R�s�[
	for( int i = 0 ; i < OPTION_FILE_MAX ; i++ ){
		switch (language) {
		case GAME_LANGUAGE_JAPANESE:
			strcpy(&lpTalk[i].pass[0], lpJapaneseFileTbl[stageNum][i]);
			break;
		case GAME_LANGUAGE_ENGLISH:
			strcpy(&lpTalk[i].pass[0], lpEnglishFileTbl[stageNum][i]);
			break;
		}
	}
	//�{������t�@�C���̃f�[�^���擾����
	*( lpRecord ) = *( LoadRecordFile( lpJapaneseFileTbl[ stageNum ][ recordType ] , recordType ) );
}

/*
************************************************************************************************
�J�[�\���̈ړ�
************************************************************************************************
*/
void	OptionCursorMove(){
	
	//��
	if (JustKey(VK_UP) || JustLever(LEVER_UP)) {
		//�J�[�\����
		PlaySound(g_OptionEffectSound[0], 0, false);
		g_OptionCursor.y -= g_OptionCursor.my;
		//if subsutituted flag of changing language for gloabal variable, subsutute flag of not changing language for 
		if(g_OptionLanguageFlag == OPTION_CHANGE_LANGUAGE) g_OptionLanguageFlag = OPTION_DONT_CHANGE_LANGUAGE; 	
	}
	//��
	if (JustKey(VK_DOWN) || JustLever(LEVER_DOWN)) {
		//�J�[�\����
		PlaySound(g_OptionEffectSound[0], 0, false);
		g_OptionCursor.y += g_OptionCursor.my;
		//if subsutituted flag of changing language for gloabal variable, subsutute flag of not changing language for 
		if (g_OptionLanguageFlag == OPTION_CHANGE_LANGUAGE) g_OptionLanguageFlag = OPTION_DONT_CHANGE_LANGUAGE;
	}
}

/*
************************************************************************************************
�J�[�\���̈ړ�����
************************************************************************************************
*/
void	ConstrainOptionCursor(){
	
	//��̍��ڂȂ牺�̍��ڂ�
	if (g_OptionCursor.y < g_constrainCursorMoveTop) {
		g_OptionCursor.y = g_constrainCursorMoveBottom;
	}
	//���̍��ڂȂ��̍��ڂ�
	if (g_OptionCursor.y > g_constrainCursorMoveBottom) {
		g_OptionCursor.y = g_constrainCursorMoveTop;
	}
}


/*
************************************************************************************************
�X�e�[�W�L�^�{������
************************************************************************************************
*/
void	OptionSelectStageRecord(){

	//Each stage's items
	int scoreItemTbl[STAGE_MAX][RECORD_MAX] = {

		{ 5 , 5 , 1 , 1 , 1 } ,			//Sea
		{ 2 , 3 , 1 , 1 , 1 } ,			//Road
		{ 4 , 1 , 1 , 1 , 1 } ,			//Off-road
	};
	//�� stage's number
	tagSELECTSTAGE	StageTbl[ STAGE_MAX ] = {
		STAGE_SEA		,
		STAGE_ROAD		,
		STAGE_OFF_ROAD	,
	};
	
	//�� �e�X�e�[�W�̋L�^�������v�f
	tagRECORDMATRIX recordTypeTbl[ RECORD_MAX ] = {

		RECORD_ONE			,				//�v�f1
		RECORD_TWO			,				//�v�f2
		RECORD_TIME			,				//���Ԃ̗v�f
		RECORD_FOUR			,				//�v�f4
		RECORD_TOTAL		,				//�W�v�|�C���g�̗v�f
	};

	//cursor move
	g_OptionCursor.my = ( g_OptionSelectStage[0].h * 
		g_OptionSelectStage[0].scale ) + OPTION_SELECT_STAGE_INDICATE_SPACE_Y;
	//cursor constrain move
	g_constrainCursorMoveTop = g_OptionSelectStage[0].y;
	g_constrainCursorMoveBottom = g_OptionEndTheResult[0].y;
	//draw cursor
	g_OptionCursor.flag = true;
	//�e��X�e�[�W�̕\��
	for (int i = 0; i < STAGE_MAX; i++) { g_OptionSelectStage[i].flag = true; }
	//�ŏI���ʕ�����̕\��
	g_OptionEndTheResult[0].flag = true;

	//�e��X�e�[�W�̑I��
	for( int i = 0 ; i < STAGE_MAX ; i++ ){
		//�\�����Ȃ�
		if( g_OptionSelectStage[i].flag ){

			//�I�� �{���o����̂͊e��X�e�[�W�ƍŏI����
			//�e��X�e�[�W
			if( g_OptionCursor.y == g_OptionSelectStage[i].y ){
				//�T�C�Y�����킹��
				g_OptionCursor.w = (int)( g_OptionSelectStage[i].w * g_OptionSelectStage[i].scale );
				g_OptionCursor.h = (int)( g_OptionSelectStage[i].h * g_OptionSelectStage[i].scale) ;

				//����
				if( JustKey( 'Z' ) || JustButton( BUTTON_1 ) ){
					//���艹
					PlaySound( g_OptionEffectSound[1] , 0 , false );
					//�J�[�\��������
					g_OptionCursor.flag = false;
					//�L�^��\������l����
					g_OptionRecordView = StageTbl[i];
					
					//�� �L�^�{������t�@�C���̓ǂݍ���
					LoadOptionStageFile( &g_OptionScore[0][0].re , &g_OptionFile[0] , g_OptionRecordView , RECORD_BEST );
					
					//subsutitute score's direction and indication value
					for (int j = 0; j < RECORD_MAX; j++) {
						for (int i = 0; i < scoreItemTbl[g_OptionRecordView][j]; i++) {
							SetOptionStageScoreDirection(&g_OptionScore[j][i], j, i);
						}
					}
					//loop to kind of record
					for (int i = 0; i < RECORD_MAX; i++) {
						//subusutitute value for gloabl variable
						SetOptionStageScoreForDraw(&g_OptionScore[recordTypeTbl[i]][0], &g_OptionScore[0][0].re, g_OptionRecordView, recordTypeTbl[i]);
						//difference setting for stage and present language
						DeferScoreOrFixedFileSetting(&g_OptionScore[i][0], g_OptionFile, g_OptionRecordView, i);
					}
					//�{���\�t���O
					g_OptionViewStageRecordFlag = true;
					//�{���J�ڃt���O
					g_OPtionTransitionFlagNext = OPTION_TRANSITION_DURING_RECORD_VIEW;

					//�X�e�[�W�Z���N�g�摜���\����
					for( int i = 0 ; i < STAGE_MAX ; i++ ){ g_OptionSelectStage[i].flag = false; }
					//�ŏI���ʕ�������\����
					g_OptionEndTheResult[0].flag = false;

					//�I�����ꂽ�X�e�[�W������̉摜�̕\����y���W��ύX
					g_OptionSelectStage[i].y = ENDING_END_RESULT_IMAGE_INDICATE_Y;
					//�\���t���O
					g_OptionSelectStage[i].flag = true;
					return;
				}
			}
		}
	}
}

/*
�ŏI���ʉ{��
*/
void	OptionEndRecordView() {
	
	//�ŏI����
	if (g_OptionCursor.y == g_OptionEndTheResult[0].y) {

		//�T�C�Y�����킹��
		g_OptionCursor.w = (int)(g_OptionEndTheResult[0].w * g_OptionEndTheResult[0].scale);
		g_OptionCursor.h = (int)(g_OptionEndTheResult[0].h * g_OptionEndTheResult[0].scale);

		//����
		if (JustKey('Z') || JustButton(BUTTON_1)) {
			//���艹
			PlaySound(g_OptionEffectSound[1], 0, false);
			//�J�[�\��������
			g_OptionCursor.flag = false;
			//�X�e�[�W�Z���N�g�摜���\����
			for (int i = 0; i < STAGE_MAX; i++) { g_OptionSelectStage[i].flag = false; }
			//�ŏI���ʕ�������\��
			g_OptionEndTheResult[0].flag = false;
			//�ŏI���ʕ�����̍��W��ύX����
			g_OptionEndTheResult[0].y = ENDING_END_RESULT_IMAGE_INDICATE_Y;

			//�ߋ��ɋL�^�����ō��L�^�̓ǂݍ���
			g_OptionEndScore[0].re = *(LoadRecordFile("Data\\EndTheResult\\Best.dat", RECORD_BEST));
			//�擾�����l��\���p�ɐݒ肷��
			//CHAIN
			g_OptionEndScore[ENDING_SCORE_CHAIN].num = g_OptionEndScore[0].re.recordBox[ENDING_SCORE_CHAIN][0];
			//���Ԃ̋L�^
			g_OptionEndScore[ENDING_SCORE_TIME].ti.minutes = g_OptionEndScore[0].re.recordBox[ENDING_SCORE_TIME][0];			//��
			g_OptionEndScore[ENDING_SCORE_TIME].ti.second = g_OptionEndScore[0].re.recordBox[ENDING_SCORE_TIME][1];				//�b
			g_OptionEndScore[ENDING_SCORE_TIME].ti.millisecond = g_OptionEndScore[0].re.recordBox[ENDING_SCORE_TIME][2];		//�~���b
			//�W�v�|�C���g�̋L�^
			g_OptionEndScore[ENDING_SCORE_TOTAL_POINT].num = (float)(g_OptionEndScore[0].re.recordBox[ENDING_SCORE_TOTAL_POINT][0]);

			//�L�^�{���t���O
			g_OptionViewEndRecordFlag = true;
			//�@�\�J�ڃt���O
			g_OPtionTransitionFlagNext = OPTION_TRANSITION_DURING_RECORD_VIEW;

		}
	}

}

/*
�L�^�{�����L�����Z��
*/
void	OptionCancelRecordView() {

	//�L�����Z��
	if (JustKey('X') || JustButton(BUTTON_2)) {
		//�L�����Z����
		PlaySound(g_OptionEffectSound[2], 0, false);
		//�e��X�e�[�W�摜�̔�\��
		for (int i = 0; i < STAGE_MAX; i++) { g_OptionSelectStage[i].flag = false; }
		//�ŏI���ʕ�������\��
		g_OptionEndTheResult[0].flag = false;
		//dont draw cursor
		g_OptionCursor.flag = false;
		//�J�[�\������ԏ��
		g_OptionCursor.x = g_OptionFunction[OPTION_FUNCTION_RECORD].x;
		g_OptionCursor.y = g_OptionFunction[OPTION_FUNCTION_RECORD].y;
		//�ړ�����
		g_constrainCursorMoveTop = g_OptionFunction[OPTION_FUNCTION_RECORD].y;
		g_constrainCursorMoveBottom = g_OptionFunction[OPTION_FUNCTION_RANKING].y;
		//�@�\�I���t���O��
		g_OPtionTransitionFlagNext = OPTION_TRANSITION_SELECTION;
	}
}
/*
************************************************************************************************
�X�R�A�\���p�̐ݒ�
************************************************************************************************
*/
void	SetOptionStageScoreDirection(SCOREVALUE *lpScore, int recordNum, int material) {


	//set indication position
	FPOSITION posTbl[RECORD_MAX] = {
		{ RESULT_SCORE_FIRST_INDICATE_X,RESULT_SCORE_FIRST_INDICATE_Y },			//first item
		{ RESULT_SCORE_SECOND_INDICATE_X,RESULT_SCORE_SECOND_INDICATE_Y },			//second item
		{ RESULT_SCORE_THIRD_INDICATE_X,RESULT_SCORE_THIRD_INDICATE_Y },			//third item
		{ RESULT_SCORE_FOURTH_INDICATE_X,RESULT_SCORE_FOURTH_INDICATE_Y },			//fourth item
		{ RESULT_SCORE_FIFTH_INDICATE_X,RESULT_SCORE_FIFTH_INDICATE_Y },			//fifth item
	};
	//set spatial indication
	FPOSITION spaceTbl[RECORD_MAX] = {
		{ 0,RESULT_SCORE_FIRST_INDICATE_SPACE_Y },									//first
		{ 0,RESULT_SCORE_SECOND_INDICATE_SPACE_Y },									//second
		{ RESULT_SCORE_THIRD_INDICATE_SPACE_X,RESULT_SCORE_THIRD_INDICATE_SPACE_Y },//third
		{ 0,0 },																	//fourth
		{ 0,0 },																	//fifth
	};
	//set default scale rate
	float scaleTbl[RECORD_MAX] = {
		RESULT_SCORE_FIRST_DEFAULT_SCALE,				//first
		RESULT_SCORE_SECOND_DEFAULT_SCALE,				//second
		RESULT_SCORE_THIRD_DEFAULT_SCALE,				//third
		RESULT_SCORE_FOURTH_DEFAULT_SCALE,				//fourth
		RESULT_SCORE_FIFTH_DEFAULT_SCALE,				//fifth
	};
	//score's direction type
	char directionTbl[RECORD_MAX] = {
		SCORE_GRADUALLY_DIRECT_NORMAL ,				//first
		SCORE_GRADUALLY_DIRECT_NORMAL ,				//second
		SCORE_GRADUALLY_DIRECT_ROLLING ,			//third
		SCORE_GRADUALLY_DIRECT_NORMAL ,				//fourth
		SCORE_GRADUALLY_DIRECT_NORMAL ,				//fifth
	};
	//set default digit
	int digitTbl[RECORD_MAX] = {
		RESULT_SCORE_FIRST_DIGIT,						//first
		RESULT_SCORE_SECOND_DIGIT,						//second
		RESULT_SCORE_THIRD_DIGIT,						//third
		RESULT_SCORE_FOURTH_DIGIT,						//fourth
		RESULT_SCORE_FIFTH_DIGIT,						//fifth
	};
	//score's direction type, which is rolling direction
	//rolling time
	int rollingTimeTbl[RECORD_MAX] = {
		0,													//first
		0,													//second
		RESULT_SCORE_THIRD_DIRECTION_ROLLING_DEFAULT_TIME,	//third
		0,													//fourth
		0,													//fifth
	};
	//score's direction type, which is gradually direction
	//interval time of indication
	int intervalTbl[RECORD_MAX] = {
		OPTION_SCORE_FIRST_DIRECTION_GRADUALLY_DEFAULT_INTERVAL_TIME,
		OPTION_SCORE_SECOND_DIRECTION_GRADUALLY_DEFAULT_INTERVAL_TIME,
		0,
		OPTION_SCORE_FOURTH_DIRECTION_GRADUALLY_DEFAULT_INTERVAL_TIME,
		OPTION_SCORE_FIFTH_DIRECTION_GRADUALLY_DEFAULT_INTERVAL_TIME,
	};
	//subsutitute default value
	lpScore->spaceX = spaceTbl[recordNum].fX;
	lpScore->spaceY = spaceTbl[recordNum].fY;
	lpScore->score_x = posTbl[recordNum].fX;
	lpScore->score_y = posTbl[recordNum].fY + (lpScore->spaceY * material);
	lpScore->score_scale = scaleTbl[recordNum];
	lpScore->ch.type = SCORE_NUMBER_TYPE_NORMAL;
	lpScore->scoreColor = OPTION_SCORE_COLOR_COMMON_USUAL;
	lpScore->digit = digitTbl[recordNum];
	lpScore->gradually = true;
	lpScore->graduallyDirect = directionTbl[recordNum];
	lpScore->rollTime = rollingTimeTbl[recordNum];
	lpScore->ch.time = 0;
	lpScore->interval = intervalTbl[recordNum];
	lpScore->renewalColor = OPTION_SCORE_COLOR_COMMON_RENEWAL;
	lpScore->numD = 0;

}

/*
****************************************************************************************
�X�R�A�\���ɒl��ݒ�
****************************************************************************************
*/
void	SetOptionStageScoreForDraw( SCOREVALUE *lpScore , RECORD *lpRecord , char stageNum , tagRECORDMATRIX itemType ){
	
	
	//�����e���ڂ̐�
	//�@�C�X�e�[�W�F�G�����A�l���A�C�e���A���ԁA�G����CHAIN���A�W�v�|�C���g�̏���
	//�A���H�X�e�[�W�F�j�󂵂���Q���A��񂾃n�[�h���A���ԁA�n�[�h������CHAIN���A�W�v�|�C���g�̏���
	//�B�I�t���[�h�X�e�[�W�F���������A�N�V�����A���v�A�N�V����Pt�A���ԁA�A���A�N�V�������A�W�v�|�C���g�̏���
	char ScoreItemTbl[ STAGE_MAX ][ RECORD_MAX ] = {

		{ 5 , 5 , 1 , 1 , 1 } ,			//�C

		{ 2 , 3 , 1 , 1 , 2 } ,			//���H

		{ 4 , 1 , 1 , 1 , 1 } ,			//�I�t���[�h

	};

	//�� �\���p�̒l����
	for( int i = 0 ; i < ScoreItemTbl[ stageNum ][ itemType ] ; i++ ){
		//���Ԃ̍���
		if( itemType == RECORD_TIME ){ 
			lpScore->ti.minutes = lpRecord->recordBox[ RECORD_TIME ][0];	//��
			lpScore->ti.second = lpRecord->recordBox[ RECORD_TIME ][1];		//�b
			lpScore->ti.millisecond = lpRecord->recordBox[ RECORD_TIME ][2];	//�~���b
			break;
		}
		//���ԈȊO�̍���
		lpScore[i].num = (float)lpRecord->recordBox[ itemType ][i];
	}

}

/*
�I�v�V�����@�\�̑I��
*/
void	OptionSelectFunction() {

	FPOSITION levelPos[2] = {
		{ CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(OPENING_BUTTON_IMAGE_WIDTH), OPTION_GAME_LEVEL_BEGINING_INDICATE_Y },
		{ CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(OPENING_BUTTON_IMAGE_WIDTH), OPTION_GAME_LEVEL_BEGINING_INDICATE_Y +
		((OPENING_BUTTON_IMAGE_HEIGHT + OPTION_FUNCTION_RECORD_INDICATE_SPACE_Y) * 2) }
	};
	//transition in option
	tagOPTIONTRANSITION functionTbl[OPTION_KIND_OF_FUNCTION] = {

		OPTION_TRANSITION_SELECT_RECORD,		//select view record
		OPTION_TRANSITION_RESET_RECORD,			//reset best record
		OPTION_TRANSITION_CHANGE_LANGUAGE,		//change curent language
		OPTION_TRANSITION_SELECT_RANKING,		//select ranking to select game's level
	};
	//cursor position to next transition
	FPOSITION CursorPos[OPTION_KIND_OF_FUNCTION] = {
		{g_OptionSelectStage[0].x, g_OptionSelectStage[0].y },										//view best record
		{g_OptionAnswer[1].x , OPTION_ANSWER_INDICATE_Y +
		(MENU_IMAGE_ANSWER_HEIGHT * OPTION_ANSWER_IMAGE_SCALE + OPTION_ANSWER_INDICATE_SPACE_Y) },	//reset best record
		{ g_OptionLanguageCh[0].x , g_OptionLanguageCh[0].y },										//change curent language
		{ levelPos[0].fX , levelPos[0].fY },														//select ranking
	};
	//constrain cursor move to next transition
	float constrainPosTbl[OPTION_KIND_OF_FUNCTION][2] = {
		{ g_OptionSelectStage[0].y , g_OptionEndTheResult[0].y},									//view best record
		{ g_OptionAnswer[0].y , OPTION_ANSWER_INDICATE_Y +
		(MENU_IMAGE_ANSWER_HEIGHT * OPTION_ANSWER_IMAGE_SCALE + OPTION_ANSWER_INDICATE_SPACE_Y) },	//reset best record
		{ g_OptionLanguageCh[0].y , g_OptionLanguageCh[1].y },										//change curent language
		{ levelPos[0].fY , levelPos[1].fY },														//select ranking
	};

	//height * scale
	float imageHeight = OPTION_FUNCTION_IMAGE_COMMON_HEIGHT * OPTION_FUNCTION_DEFAULT_SCALE;
	//subsutite reset of function's position for indicate y
	g_OptionFunction[OPTION_FUNCTION_RESET].y = OPTION_FUNCTION_RECORD_INDICATE_Y + (imageHeight + OPTION_FUNCTION_RECORD_INDICATE_SPACE_Y);
	g_OptionFunction[OPTION_FUNCTION_RANKING].y = OPTION_FUNCTION_RECORD_INDICATE_Y + ((imageHeight + OPTION_FUNCTION_RECORD_INDICATE_SPACE_Y) * 3);
	//constrain move
	g_constrainCursorMoveTop = g_OptionFunction[OPTION_FUNCTION_RECORD].y;
	g_constrainCursorMoveBottom = g_OptionFunction[OPTION_FUNCTION_RANKING].y;
	//cursor move
	g_OptionCursor.my = (imageHeight + OPTION_FUNCTION_RECORD_INDICATE_SPACE_Y);
	//draw cursor
	g_OptionCursor.flag = true;
	//Draw function image
	for (int i = 0; i < OPTION_KIND_OF_FUNCTION; i++) { g_OptionFunction[i].flag = true; }

	//select function
	for (int i = 0; i < OPTION_KIND_OF_FUNCTION; i++) {
		if (g_OptionCursor.y == g_OptionFunction[i].y) {
			//fit position x
			g_OptionCursor.x = g_OptionFunction[i].x;
			//�������킹��
			g_OptionCursor.w = (int)(g_OptionFunction[i].w * g_OptionFunction[i].scale);
			//���������킹��
			g_OptionCursor.h = (int)(g_OptionFunction[i].h * g_OptionFunction[i].scale);
			if (JustUpKey('Z') || JustUpButton(BUTTON_1)) {
				//���艹
				PlaySound(g_OptionEffectSound[1], 0, false);
				//�ߋ��̋L�^���{���ł��郂�[�h�ɑJ��
				//�摜������
				for (int i = 0; i < OPTION_KIND_OF_FUNCTION; i++) { g_OptionFunction[i].flag = false; }
				//�J�[�\�������킹��
				g_OptionCursor.x = CursorPos[i].fX;
				g_OptionCursor.y = CursorPos[i].fY;
				//dont draw cursor
				g_OptionCursor.flag = false;
				//�J�ڃt���O
				g_OPtionTransitionFlagNext = functionTbl[i];
				
				//case select record
				if (g_OPtionTransitionFlagNext == functionTbl[i]) {
					//cursor constrain move
					g_constrainCursorMoveTop = constrainPosTbl[i][0];
					g_constrainCursorMoveBottom = constrainPosTbl[i][1];
				}
				return;
			}
		}
	}
	//�L�����Z���ŃI�[�v�j���O��
	if (JustButton(BUTTON_2) || JustKey('X')) {
		//�L�����Z����
		PlaySound(g_OptionEffectSound[2], 0, false);
		//�^�C�g����ʂ֑J��
		CreateWipe(SCENE_OPENING, CHANGE_SCENE, WIPE_CONTRACTION);
	}


}

/*
�L�^�{������X�e�[�W�I����ʂւ̑J��
*/
void	OptionRecordViewToSelectRecordView(){

	//�L�����Z���ŃX�e�[�W�I����
	if (JustButton(BUTTON_2) || JustKey('X')) {
		//�L�����Z����
		PlaySound(g_OptionEffectSound[2], 0, false);
		//�ǂݍ��񂾃t�@�C���̉��
		ReleaseTalk();
		//�ǂݍ��񂾃t�@�C���̃t���O��false��
		for (int i = 2; i < OPTION_FILE_MAX; i++) { g_OptionFile[i].fRead = false; }
		//�e�핶����摜�̔�\��
		//�X�e�[�W
		g_OptionSelectStage[g_OptionRecordView].flag = false;
		//�ŏI����
		for (int i = 0; i < ENDING_SCORE_AND_TITLE; i++) { g_OptionEndTheResult[i].flag = false; };

		//�\�����l�����Z�b�g
		//�X�e�[�W
		for (int i = 0; i < RECORD_MAX; i++) {
			for (int j = 0; j < RECORD_MAX; j++) {
				g_OptionScore[i][j].directEnd = false;
				g_OptionScore[i][j].numD = 0;
				g_OptionScore[i][j].ch.time = 0;
			}
		}
		//�ŏI����
		for (int i = 0; i < ENDING_SCORE_AND_TITLE; i++) {
			g_OptionEndScore[i].directEnd = false;
			g_OptionEndScore[i].numD = 0; 
		}

		//�ύX�����X�e�[�W������̕\����Y��ύX����
		g_OptionSelectStage[g_OptionRecordView].y = OPTION_SELECT_STAGE_INDICATE_Y +
			((OPTION_DEFAULT_SELECT_IMAGE_SCALE * SELECTSTAGE_STAGE_TITLE_IMAGE_HEIGHT) +
				OPTION_SELECT_STAGE_INDICATE_SPACE_Y) * g_OptionRecordView;

		//�ŏI���ʕ�����̍��W��ύX
		g_OptionEndTheResult[0].y = g_OptionSelectStage[2].y + ((
			g_OptionSelectStage[2].h * g_OptionSelectStage[2].scale) + OPTION_SELECT_STAGE_INDICATE_SPACE_Y);
		//�J�[�\������ԏ��
		g_OptionCursor.x = g_OptionSelectStage[0].x;
		g_OptionCursor.y = g_OptionSelectStage[0].y;
		//�ǂݍ��ރX�e�[�W�����Z�b�g
		g_OptionRecordView = STAGE_VACANT;
		//�L�^�{���\�t���O
		g_OptionViewStageRecordFlag = false;		//�X�e�[�W
		g_OptionViewEndRecordFlag = false;			//�ŏI����
		//�L�^�{���J�ڃt���O
		g_OPtionTransitionFlagNext = OPTION_TRANSITION_SELECT_RECORD;

	}

}

/*
Reset best record function
*/
void	OptionFunctionResetRecord() {

	//high image height
	float imageHeight = g_OptionAnswer[0].h * g_OptionAnswer[0].scale;
	//Draw answer image
	for (int i = 0; i < 2; i++) { g_OptionAnswer[i].flag = true; }
	//cursor move
	g_OptionCursor.my = imageHeight + OPTION_ANSWER_INDICATE_SPACE_Y;
	//draw cursor
	g_OptionCursor.flag = true;

	g_OptionAnswer[1].y = OPTION_ANSWER_INDICATE_Y +
		(imageHeight + OPTION_ANSWER_INDICATE_SPACE_Y);

	//select answer yes or no
	for (int i = 0; i < 2; i++) {
		//cursor position
		if (g_OptionCursor.y == g_OptionAnswer[i].y) {
			//fit size
			g_OptionCursor.h = (int)(g_OptionAnswer[i].h * g_OptionAnswer[i].scale);
			g_OptionCursor.w = (int)(g_OptionAnswer[i].w * g_OptionAnswer[i].scale);
			//fit position X
			g_OptionCursor.x = g_OptionAnswer[i].x;

			//Select answer
			if (JustKey('Z') || JustButton(BUTTON_1)) {
				
				//each other file's name
				char *lpFileTbl[STAGE_MAX + 1] = {
					{ "Data\\SeaStage\\BestScore.dat" } ,
					{ "Data\\RoadStage\\BestScore.dat" } ,
					{"Data\\OffRoadStage\\BestScore.dat"} ,
					{"Data\\EndTheResult\\Best.dat" },
				};
				//each other stages' items and end result
				int itemTbl[STAGE_MAX + 1][RECORD_MAX] = {

					{ 5 , 5 , 3 , 1 , 1 } ,			//sea
					{ 2 , 3 , 3 , 1 , 2 } ,			//road
					{ 4 , 1 , 3 , 1 , 1 } ,			//off-road
					{ 1 , 3 , 1 , 0 , 0 } ,			//ent the result
				};
				//each other kind of item
				int		itemKindTbl[STAGE_MAX + 1] = { RECORD_MAX ,RECORD_MAX ,RECORD_MAX ,ENDING_KIND_OF_SCOREVALUE };

				switch (i) {
				case 0:		//yes
					//accept sound
					PlaySound(g_OptionEffectSound[1], 0, false);
					//reset best record
					for (int j = 0; j < (STAGE_MAX + 1); j++) {
						ResetRecord(lpFileTbl[j],
							itemKindTbl[j], itemTbl[j], RECORD_BEST);
					}
					//substitute reset flag for g_OptionResetRecordFlag
					g_OptionResetRecordFlag = OPTION_DID_RESET;
					break;
				case 1:		//no
					//cancel sound
					PlaySound(g_OptionEffectSound[2], 0, false);
					//dont draw cursor
					g_OptionCursor.flag = false;
					//dont draw answer
					for (int i = 0; i < 2; i++) { g_OptionAnswer[i].flag = false; }
					//cursor's position
					g_OptionCursor.x = g_OptionFunction[0].x;
					g_OptionCursor.y = g_OptionFunction[0].y;
					//constrain move
					g_constrainCursorMoveTop = g_OptionFunction[0].y;
					g_constrainCursorMoveBottom = g_OptionFunction[2].y;
					//function's flag to selection of function
					g_OPtionTransitionFlagNext = OPTION_TRANSITION_SELECTION;
					//substitute don't reset flag for g_OptionResetRecordFlag
					g_OptionResetRecordFlag = OPTION_DONT_RESET;
					break;
				}
				return;
			}

		}
	}

	//Cancel reset function
	if (JustKey('X') || JustButton(BUTTON_2)) {
		//cancel sound
		PlaySound(g_OptionEffectSound[2], 0, false);
		//dont draw cursor
		g_OptionCursor.flag = false;
		//dont draw answer
		for (int i = 0; i < 2; i++) { g_OptionAnswer[i].flag = false; }
		//cursor's position
		g_OptionCursor.x = g_OptionFunction[OPTION_FUNCTION_RECORD].x;
		g_OptionCursor.y = g_OptionFunction[OPTION_FUNCTION_RECORD].y;
		//constrain move
		g_constrainCursorMoveTop = g_OptionFunction[OPTION_FUNCTION_RECORD].y;					//Record
		g_constrainCursorMoveBottom = g_OptionFunction[OPTION_FUNCTION_RANKING].y;				//Ranking
		//function's flag to selection of function
		g_OPtionTransitionFlagNext = OPTION_TRANSITION_SELECTION;
		//substitute don't reset flag for g_OptionResetRecordFlag
		g_OptionResetRecordFlag = OPTION_DONT_RESET;
	}
}

/*
Explain selected function
*/
void	OptionDrawExplainFunction() {

	//get current language
	tagGAMELANGUAGE language = GetGameLanguage();
	
	//for drawing
	char lpMes[OPTION_KIND_OF_FUNCTION][256] = { 0 };

	//Japanese words
	char	*lpJapanese[OPTION_KIND_OF_FUNCTION] = {
		{"�ߋ��ɋL�^�����ō��L�^���{���ł��܂�"},
		{"�ߋ��ɋL�^�����ō��L�^���������܂�"},
		{"���݂̌���ݒ��ύX�ł��܂�"},
		{"�ŐV�̏��ʂ��{���ł��܂�"}
	};
	//English words
	char	*lpEnglish[OPTION_KIND_OF_FUNCTION] = {
		{"You can view a best record of the past"},
		{"You can delete all best record of the past"},
		{"You can change the current language"},
		{"You can view latest ranking"}
	};
	//cursor position
	float cursorPosTbl[OPTION_KIND_OF_FUNCTION] = { 
		g_OptionFunction[0].y ,				//record
		g_OptionFunction[1].y ,				//reset
		g_OptionFunction[2].y ,				//language
		g_OptionFunction[3].y ,				//ranking
	};

	//drawing words copy into lpMes
	switch (language) {
	case GAME_LANGUAGE_JAPANESE:			//Japanese
		for (int i = 0; i < OPTION_KIND_OF_FUNCTION; i++) {
			strcpy(lpMes[i], lpJapanese[i]);
		}
		break;
	case GAME_LANGUAGE_ENGLISH:				//English
		for (int i = 0; i < OPTION_KIND_OF_FUNCTION; i++) {
			strcpy(lpMes[i], lpEnglish[i]);
		}
		break;
	}
	//select explain by cursor's positionY
	for (int i = 0; i < OPTION_KIND_OF_FUNCTION; i++) {
		if (g_OptionCursor.y == cursorPosTbl[i]) {
			DDTextOutEx(lpMes[i],
				CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH((int)( lstrlen(lpMes[i]) * (OPTION_EXPLAIN_FUNCTION_FONT_SIZE / 2) ) ),
				(int)OPTION_EXPLAIN_WORDS_INDICATE_Y,
				OPTION_EXPLAIN_FUNCTION_FONT_SIZE,
				OPTION_EXPLAIN_FUNCTION_FONT_COLOR, false);
			break;
		}
	}

}

/*
Explain selected reset record
*/
void	OPtionDrawExplainResetRecord() {

	//get current language
	tagGAMELANGUAGE language = GetGameLanguage();

	char lpMes[2][256] = { 0 };

	//Japanese words
	char	*lpJapanese[2] = {
		{ "�ߋ��ɋL�^�����ō��L�^���������܂����H" },
		{ "�ߋ��̍ō��L�^���������܂���" },
	};
	//English words
	char	*lpEnglish[2] = {
		{ "Do you want to delete all best record of the past?" },
		{ "You deleted all best record of the past" },
	};

	//drawing words copy into lpMes
	switch (language) {
	case GAME_LANGUAGE_JAPANESE:			//Japanese
		for (int i = 0; i < 2; i++) {
			strcpy(lpMes[i], lpJapanese[i]);
		}
		break;
	case GAME_LANGUAGE_ENGLISH:				//English
		for (int i = 0; i < 2; i++) {
			strcpy(lpMes[i], lpEnglish[i]);
		}
		break;
	}
	//diverge explanation
	DDTextOutEx(lpMes[g_OptionResetRecordFlag],
		CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH((int)(lstrlen(lpMes[g_OptionResetRecordFlag]) * (OPTION_EXPLAIN_FUNCTION_FONT_SIZE / 2))),
		(int)OPTION_EXPLAIN_WORDS_INDICATE_Y,
		OPTION_EXPLAIN_FUNCTION_FONT_SIZE,
		OPTION_EXPLAIN_FUNCTION_FONT_COLOR, false);

}


/*
one of function, that is change current language
*/
void	OptionFunctionChangeLanguage() {

	//kind of language
	tagGAMELANGUAGE	languageTbl[GAME_KIND_OF_LANGUAGE] = { GAME_LANGUAGE_JAPANESE, GAME_LANGUAGE_ENGLISH };
	//high image height
	float imageHeight = (OPTION_LANGUAGE_IMAGE_COMMON_HEIGHT * OPTION_LANGUAGE_DEFAULT_SCALE);
	//Draw language image
	for (int i = 0; i < GAME_KIND_OF_LANGUAGE; i++) { g_OptionLanguageCh[i].flag = true; }
	//cursor move
	g_OptionCursor.my = (imageHeight + OPTION_LANGUAGE_INDICATE_SPACE_Y);
	//draw cursor
	g_OptionCursor.flag = true;

	//select current language
	for (int i = 0; i < GAME_KIND_OF_LANGUAGE; i++) {
		//cursor position
		if (g_OptionCursor.y == g_OptionLanguageCh[i].y) {
			//fit size
			g_OptionCursor.h = (int)(g_OptionLanguageCh[i].h * g_OptionLanguageCh[i].scale);
			g_OptionCursor.w = (int)(g_OptionLanguageCh[i].w * g_OptionLanguageCh[i].scale);
			//fit position X
			g_OptionCursor.x = g_OptionLanguageCh[i].x;
			//subsutitute current language for gloabal variable for explanation
			g_OptionLanguageNow = languageTbl[i];
			//Select answer
			if (JustKey('Z') || JustButton(BUTTON_1)) {
				//subsutitute flag of currennt language for global variable
				SetGameLanguage(g_OptionLanguageNow);
				PlaySound(g_OptionEffectSound[1], 0, false);		//accept sound
				//subsutitute flag of changing language for gloabl variable
				g_OptionLanguageFlag = OPTION_CHANGE_LANGUAGE;
				break;
			}
		}
	}

	//Cancel reset function
	if (JustKey('X') || JustButton(BUTTON_2)) {
		//cancel sound
		PlaySound(g_OptionEffectSound[2], 0, false);
		//dont draw cursor
		g_OptionCursor.flag = false;
		//dont draw language image
		for (int i = 0; i < GAME_KIND_OF_LANGUAGE; i++) { g_OptionLanguageCh[i].flag = false; }
		//cursor's position
		g_OptionCursor.x = g_OptionFunction[OPTION_FUNCTION_RECORD].x;
		g_OptionCursor.y = g_OptionFunction[OPTION_FUNCTION_RECORD].y;
		//constrain move
		g_constrainCursorMoveTop = g_OptionFunction[OPTION_FUNCTION_RECORD].y;						//Record
		g_constrainCursorMoveBottom = g_OptionFunction[OPTION_FUNCTION_RANKING].y;					//Ranking
		//function's flag to selection of function
		g_OPtionTransitionFlagNext = OPTION_TRANSITION_SELECTION;
		//subsutitute flag of changing language for gloabal variable
		g_OptionLanguageFlag = OPTION_DONT_CHANGE_LANGUAGE;
	}
}

/*
explain to change current language
*/
void	OPtionDrawExplainLanguage() {

	//get current language
	tagGAMELANGUAGE	language = GetGameLanguage();
	//kind of language
	tagGAMELANGUAGE	languageTbl[GAME_KIND_OF_LANGUAGE] = { GAME_LANGUAGE_JAPANESE, GAME_LANGUAGE_ENGLISH };

	//words of language in Japanese
	char *lpJapanese[GAME_KIND_OF_LANGUAGE] = {
		{ "���{��" },
		{ "�p��" },
	};
	//words of language in English
	char *lpEnglish[GAME_KIND_OF_LANGUAGE] = {
		{ "Japanese" },
		{ "English" },
	};
	//variable for drawing
	char lpMes[256] = { 0 };

	//according to current language
	switch (language) {
	case GAME_LANGUAGE_JAPANESE:					//case current language is Japanese
		//if don't change current language
		if (g_OptionLanguageFlag == OPTION_DONT_CHANGE_LANGUAGE) {
			wsprintf(lpMes, "���݂̌���ݒ��%s�ɕύX���܂����H", lpJapanese[g_OptionLanguageNow]);
		}
		//if change current language
		if (g_OptionLanguageFlag == OPTION_CHANGE_LANGUAGE) {
			wsprintf(lpMes, "���݂̌���ݒ��%s�ɕύX���܂���", lpJapanese[g_OptionLanguageNow]);
		}
		break;
	case GAME_LANGUAGE_ENGLISH:
		//if don't change current language
		if (g_OptionLanguageFlag == OPTION_DONT_CHANGE_LANGUAGE) {
			wsprintf(lpMes, "Do you want to change the current language into %s?", lpEnglish[g_OptionLanguageNow]);
		}
		//if change current language
		if (g_OptionLanguageFlag == OPTION_CHANGE_LANGUAGE) {
			wsprintf(lpMes, "You changed the current language into %s", lpEnglish[g_OptionLanguageNow]);
		}
		break;
	}
	//diverge explanation
	DDTextOutEx(lpMes,
		CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH((int)(lstrlen(lpMes) * (OPTION_EXPLAIN_FUNCTION_FONT_SIZE / 2))),
		(int)OPTION_EXPLAIN_WORDS_INDICATE_Y,
		OPTION_EXPLAIN_FUNCTION_FONT_SIZE,
		OPTION_EXPLAIN_FUNCTION_FONT_COLOR, false);
}


/*
select ranking
draw each game's level
*/
void	OptionSelectRanking() {

	//each game's level
	tagGAMELEVEL levelTbl[LEVEL_MAX] = { EASY_MODE, NORMAL_MODE, HARD_MODE };
	//draw each game's level
	for (int i = 0; i < LEVEL_MAX; i++) { g_OptionLevelCh[i].flag = true; }

	g_OptionLevelCh[EASY_MODE].y = OPTION_GAME_LEVEL_BEGINING_INDICATE_Y;

	//constrain cursor's move
	g_constrainCursorMoveTop = g_OptionLevelCh[EASY_MODE].y;		//top
	g_constrainCursorMoveBottom = g_OptionLevelCh[HARD_MODE].y;		//bottom
	//draw cursor
	g_OptionCursor.flag = true;
	//move
	g_OptionCursor.my = (OPENING_BUTTON_IMAGE_HEIGHT + OPTION_FUNCTION_RECORD_INDICATE_SPACE_Y);

	//select game's level to view ranking
	for (int i = 0; i < LEVEL_MAX; i++) {
		//check cursor's positiion
		if (g_OptionCursor.y == g_OptionLevelCh[i].y) {
			//fitting size
			g_OptionCursor.h = g_OptionLevelCh[i].h;
			g_OptionCursor.w = g_OptionLevelCh[i].w;
			//press accept button
			if (JustKey('Z') || JustButton(BUTTON_1)) {

				//accept sound
				PlaySound(g_OptionEffectSound[1], 0, false);
				//subsutitute game's level for flag
				g_OptionLevelFlag = levelTbl[i];
				//subsutitute flag of next scene for transition
				g_OPtionTransitionFlagNext = OPTION_TRANSIITON_DURING_VIEW_RANKING;

				//don't draw cursor
				g_OptionCursor.flag = false;
				//don't draw each game's level
				for (int i = 0; i < LEVEL_MAX; i++) { g_OptionLevelCh[i].flag = false; }

				//Initialize participants' scores and values for ranking
				InitializeRankingScoreInOption();

				return;
			}

			//press cancel
			if (JustKey('X') || JustButton(BUTTON_2)) {

				//cancel sound
				PlaySound(g_OptionEffectSound[2], 0, false);
				//subsutitute flag of next scene for transition
				g_OPtionTransitionFlagNext = OPTION_TRANSITION_SELECTION;				//select function

				//don't draw cursor
				g_OptionCursor.flag = false;
				//don't draw each game's level
				for (int i = 0; i < LEVEL_MAX; i++) { g_OptionLevelCh[i].flag = false; }

				//fit cursor's coordinateY with record's coordinateY
				g_OptionCursor.y = g_OptionFunction[OPTION_FUNCTION_RECORD].y;
				//change to constrain cursor's move
				g_constrainCursorMoveTop = g_OptionFunction[OPTION_FUNCTION_RECORD].y;			//top
				g_constrainCursorMoveBottom = g_OptionFunction[OPTION_FUNCTION_RANKING].y;		//bottom
				return;
			}
		}
	}
}

/*
Initialize score for ranking
*/
void InitializeRankingScoreInOption() {

	//kind of score for the half way result
	tagRECORDTHEHALFWAYITEM recordNumTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		RECORD_HALF_WAY_RECORD_TOTAL_POINT,				//aggregate point
		RECORD_HALF_WAY_RECORD_RACE_TIME,				//time of race
		RECORD_HALF_WAY_RECORD_CHAIN,					//consecutive something
	};

	//kind of file
	char *lpFile[LEVEL_MAX] = {
		{ "Data\\EndTheResult\\Easy\\Ranking.dat" },			//easy
		{ "Data\\EndTheResult\\Normal\\Ranking.dat" },			//normal
		{ "Data\\EndTheResult\\Hard\\Ranking.dat" },			//hard
	};
	//subsutitute the half way record for gloabl variable of the half way record
	g_OptionTheHalWayRecord[0][0].re = *(LoadRecordFile(lpFile[g_OptionLevelFlag], RECORD_PREVIEW));
	//each score's color
	tagRECORDRANKINGRESULTITEM	color[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		RECORD_RANKING_ITEM_COLOR_FOR_TOTAL,
		RECORD_RANKING_ITEM_COLOR_FOR_TIME,
		RECORD_RANKING_ITEM_COLOR_FOR_CHAIN,
	};

	//subsutitute score's direction and indication value for the half way record
	for (int j = 0; j < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; j++) {
		for (int i = 0; i < RECORD_KIND_OF_PARTICIPANT; i++) {

			//subsutitute indication value for drawing variable
			SetResultTheHalfWayValue(&g_OptionTheHalWayRecord[j][i], recordNumTbl[j], i, 
				(tagSCORECOLOR)g_OptionTheHalWayRecord[0][0].re.recordBox[i][color[j]]);

			//subsutitute the half way record for score's variable
			switch (j)
			{
			case RECORD_HALF_WAY_RECORD_TOTAL_POINT:	//total point
				g_OptionTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][i].num = 
					g_OptionTheHalWayRecord[0][0].re.recordBox[i][RECORD_RANKING_ITEM_TOTAL_POINT];
				break;
			case RECORD_HALF_WAY_RECORD_RACE_TIME:		//record time
				g_OptionTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti.minutes =
					g_OptionTheHalWayRecord[0][0].re.recordBox[i][RECORD_RANKING_ITEM_MINUTE];
				g_OptionTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti.second = 
					g_OptionTheHalWayRecord[0][0].re.recordBox[i][RECORD_RANKING_ITEM_SECOND];
				g_OptionTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti.millisecond = 
					g_OptionTheHalWayRecord[0][0].re.recordBox[i][RECORD_RANKING_ITEM_MILLISECOND];
				break;
			case RECORD_HALF_WAY_RECORD_CHAIN:
				g_OptionTheHalWayRecord[RECORD_HALF_WAY_RECORD_CHAIN][i].num = 
					g_OptionTheHalWayRecord[0][0].re.recordBox[i][RECORD_RANKING_ITEM_CHAIN];
				break;
			default:
				break;
			}
		}
	}
}

/*
viewing ranking
*/
void	ViewingRanking() {

	//draw each scores and image
	for (int j = 0; j < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; j++) {
		for (int i = 0; i < RECORD_KIND_OF_PARTICIPANT; i++) {
			g_OptionTheHalWayRecord[j][i].ch.flag = true;		//score
			if (j > 0) continue;
			g_OptionTheHalfRanking[i].ch.flag = true;			//ranking
		}
		g_OptionRecordCh[j].flag = true;						//score's title
	}
	//press cancel
	if (JustKey('X') || JustButton(BUTTON_2)) {
		//each title's scale rate
		float scaleTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
			RESULT_IMAGE_THE_HALF_WAY_RESULT_TIME_RECORD_TITLE_SCALE_RATE,
			RESULT_IMAGE_THE_HALF_WAY_RESULT_TOTAL_POINT_TITLE_SCALE_RATE,
			RESULT_IMAGE_THE_HALF_WAY_RESULT_CHAIN_RECORD_TITLE_SCALE_RATE,
		};
		//cancel sound
		PlaySound(g_OptionEffectSound[2], 0, false);
		//don't draw each scores and images
		for (int j = 0; j < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; j++) {
			for (int i = 0; i < RECORD_KIND_OF_PARTICIPANT; i++) {
				g_OptionTheHalWayRecord[j][i].ch.flag = false;		//score
				//reset indication value
				g_OptionTheHalWayRecord[j][i].numD = 0;
				//reset count time
				g_OptionTheHalWayRecord[j][i].ch.time = 0;
				//direction flag
				g_OptionTheHalWayRecord[j][i].directEnd = false;
				if (j > 0) continue;
				g_OptionTheHalfRanking[i].ch.flag = false;			//ranking
			}
			g_OptionRecordCh[j].flag = false;						//score's title
			g_OptionRecordCh[j].x = 640.0f +						//begining position
				((ENDING_END_THE_RESULT_IMAGE_WDIHT * scaleTbl[j]) * j);
		}
		//fit cursor's coordinateY
		g_OptionCursor.y = g_OptionLevelCh[EASY_MODE].y;
		//subsutitute flag
		g_OPtionTransitionFlagNext = OPTION_TRANSITION_SELECT_RANKING;
	}
	//record's titles move
	for (int i = 0; i < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; i++) {
		if (g_OptionRecordCh[i].flag) {
			//move to indication X of terminate
			if (g_OptionRecordCh[i].x > g_OptionRecordCh[i].ex) g_OptionRecordCh[i].x += g_OptionRecordCh[i].mx;		//move
			if (g_OptionRecordCh[i].x <= g_OptionRecordCh[i].ex) g_OptionRecordCh[i].x = g_OptionRecordCh[i].ex;		//terminate
		}
	}
}