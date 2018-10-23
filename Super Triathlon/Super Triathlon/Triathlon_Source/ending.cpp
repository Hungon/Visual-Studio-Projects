#include	"ending.h"
#include	"game.h"
#include	"input.h"
#include	"wipe.h"
#include	"option.h"
#include	"talk.h"
#include	"SelectStage.h"
#include	"map.h"
#include	"sound.h"
#include	"score.h"
#include	"result.h"
#include	"play.h"

//���O���[�o���ϐ�
//�摜�̗v�f�������ϐ�
int g_EndingImg[ ENDING_IMAGE_MAX ] = {0};

//�w�i�摜�\���ݒ�p
CHARACTER g_EndingBg[ 2 ] = {0};

//�e��X�e�[�W������p
CHARACTER g_EndingStage = {0};

//�ŏI���ʕ\���p
CHARACTER g_EndingResult[ENDING_SCORE_AND_TITLE] = {0};

//�e��N���W�b�g�p
CHARACTER g_EndingCredit[ ENDING_CREDIT_MAX + 1 ] = {0};

//�t�@�C��������
TALK	g_EndingFile[ OPTION_FILE_MAX ] = {0};

//�L�^�\���p
SCOREVALUE	g_EndingScore[RECORD_MAX][RECORD_MAX] = {0};

//�e���[�X�̏W�v���ʕ\��
SCOREVALUE	g_EndTheResult[ENDING_KIND_OF_SCOREVALUE] = {0};

//���ݕ\�����Ă���X�e�[�W�ԍ�
tagENDTHERESULTITEM	g_EndResultViewNow = ENDING_ITEM_SEA;

//���ɕ\������X�e�[�W�ԍ�
tagENDTHERESULTITEM	g_EndResultViewNext = ENDING_ITEM_SEA;

//�ǂݍ��݊����t���O
bool	g_EndResultReadFlag = false;

//scene transition in ending
tagENDINGTRANSITION g_EndingSceneNow = ENDING_SCENE_STAGE_RECORD;		//present viewing
tagENDINGTRANSITION g_EndingSceneNext = ENDING_SCENE_STAGE_RECORD;		//next view
//records of the half way
SCOREVALUE	g_EndingTheHalWayRecord[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT][RECORD_KIND_OF_PARTICIPANT] = { 0 };
//ranking for the half way result
SCOREVALUE	g_EndingTheHalfRanking[RECORD_KIND_OF_PARTICIPANT] = { 0 };
//drawing record's turn
tagRESULTRANKINGEACHTURN g_EndingTheHalfWayTurn = RESULT_RANKING_TURN_CHAIN;
//player's ranking
int g_EndingPlayerBonusMaterial[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = { 0 };
//competitor's ranking
int	g_EndingCompetitorBonusMaterial[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT][PARTICIPANT_PLAYER] = { 0 };
//record's title image
CHARACTER	g_EndingRecordCh[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = { 0 };

//for draw bonue point in the half way result
SCOREVALUE	g_EndingBonusScore[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = { 0 };
//for draw bonus' image
CHARACTER	g_EndingBonusCh[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = { 0 };


//************************************************************************************************
//������
//************************************************************************************************
void InitEnding(HWND hWnd){


	//�摜�f�[�^
	char	*ImageTbl[ ENDING_IMAGE_MAX ] = {
		{ "Image\\Ending.bmp" }				,		//�G���f�B���O�摜
		{ "Image\\EndResult.bmp" }			,		//�ŏI���ʉ摜
		{ "Image\\SelectStage.bmp" }		,		//�e��X�e�[�W������
		{ "Image\\EndTheResult.bmp" }		,		//�ŏI���ʗp������
		{ "Image\\Credit.bmp" }		,				//�N���W�b�g
		{ "Image\\Bonus.bmp" }		,				//bonus
	};
	//���摜�̓ǂݍ���
	for( int i = 0 ; i < ENDING_IMAGE_MAX ; i++ ){
		g_EndingImg[i] = CreateDDImage( ImageTbl[i] , hWnd );
	}

	//�� �Ȃ̓ǂݍ���
	//�n�߂̓��U���g�pBGM
	CreateStreaming( "BGM\\result.wav" );

	//scene transition in ending
	g_EndingSceneNow = ENDING_SCENE_STAGE_RECORD;		//present viewing
	g_EndingSceneNext = ENDING_SCENE_STAGE_RECORD;		//next view

	//drawing record's turn
	g_EndingTheHalfWayTurn = RESULT_RANKING_TURN_CHAIN;		//first record is chain
	//each player's ranking of record
	for (int i = 0; i < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; i++) {
		g_EndingPlayerBonusMaterial[i] = 0;
	}

	//ENDING�摜�̒l�ݒ�
	g_EndingBg[0].y = ( STAGE_HEIGHT - ENDING_IMAGE_END_SCENE_HEIGHT );	//�\����Y
	g_EndingBg[0].h = ENDING_IMAGE_END_SCENE_HEIGHT;						//����
	g_EndingBg[0].w = ENDING_IMAGE_END_SCENE_WIDTH;						//��
	g_EndingBg[0].my = ENDING_END_THE_CREDIT_SCROLL_SPEED_Y;				//�ړ���Y
	g_EndingBg[0].flag = false;											//�\���t���O

	//�ŏI���ʉ摜
	g_EndingBg[1].x = 0.0f;									//X���W
	g_EndingBg[1].y = 0.0f;									//Y���W
	g_EndingBg[1].h = ENDING_IMAGE_END_RESULT_SCENE_HEIGHT;	//����
	g_EndingBg[1].w = ENDING_IMAGE_END_RESULT_SCENE_WIDTH;	//��
	g_EndingBg[1].picx = 0;									//�\����X
	g_EndingBg[1].picy = 0;									//�\����Y
	g_EndingBg[1].flag = true;								//�\���t���O

	//�e��X�e�[�W�摜�\���ݒ�
	g_EndingStage.x = ENDING_END_RESULT_IMAGE_INDICATE_X;						//�\����X
	g_EndingStage.y = ENDING_END_RESULT_IMAGE_INDICATE_Y;						//�\����Y
	g_EndingStage.mx = ENDING_END_RESULT_IMAGE_MOVE_X;							//�ړ���X
	g_EndingStage.h = ENDING_END_THE_RESULT_IMAGE_HEIGHT;						//����
	g_EndingStage.w = ENDING_END_THE_RESULT_IMAGE_WDIHT;						//��
	g_EndingStage.scale = ENDING_END_RESULT_IMAGE_DEFAULT_SCALE;				//�g�k�{��
	g_EndingStage.flag = true;													//�\���t���O
	//�C���[�W�摜�ɍ��킹�čŏI�\������W�����߂�
	g_EndingStage.ex = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH( g_EndingStage.w * g_EndingStage.scale );

	//�C�X�e�[�W����\��
	g_EndResultViewNext = ENDING_ITEM_SEA;
	g_EndResultViewNow = ENDING_ITEM_SEA;
	
	//�e��N���W�b�g�p�摜�ݒ�
	for( int i = 0 ; i < ( ENDING_CREDIT_MAX + 1 ); i++ ){
		g_EndingCredit[i].x = ENDING_END_CREDIT_IMAGE_INDICATE_X;						//�\����X
		g_EndingCredit[i].mx = -1 * ENDING_END_CREDIT_IMAGE_ABSOLUTE_MOVE_X;			//�ړ���X
		g_EndingCredit[i].h = ENDING_END_THE_CREDIT_IMAGE_HEIGHT;						//����
		g_EndingCredit[i].w = ENDING_END_THE_CREDIT_IMAGE_WDIHT;						//��
		g_EndingCredit[i].scale = ENDING_END_CREDIT_IMAGE_DEFAULT_SCALE;				//�g�k�{��
		g_EndingCredit[i].flag = false;													//�\���t���O
		//�C���[�W�摜�ɍ��킹�čŏI�\������W�����߂�
		g_EndingCredit[i].ex = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH( g_EndingCredit[i].w * g_EndingCredit[i].scale );

	}

	//�e��ŏI���ʕ�����\���p�ݒ�
	//picY
	int picYTbl[ENDING_SCORE_AND_TITLE] = {
		0,															//title
		ENDING_END_THE_RESULT_IMAGE_HEIGHT * 3,						//chain
		ENDING_END_THE_RESULT_IMAGE_HEIGHT,							//time
		ENDING_END_THE_RESULT_IMAGE_HEIGHT * 2,						//total
	};
	for (int i = 0; i < ENDING_SCORE_AND_TITLE; i++) {
		g_EndingResult[i].x = ENDING_END_RESULT_IMAGE_INDICATE_X;						//�\����X
		g_EndingResult[i].y = ENDING_END_RESULT_IMAGE_INDICATE_Y +
			(ENDING_END_THE_RESULT_IMAGE_INDICATE_SPACE_Y * i);						//�\����Y
		g_EndingResult[i].mx = ENDING_END_RESULT_IMAGE_MOVE_X - (i * 1.0f);			//�ړ���X
		g_EndingResult[i].h = ENDING_END_THE_RESULT_IMAGE_HEIGHT;						//����
		g_EndingResult[i].w = ENDING_END_THE_RESULT_IMAGE_WDIHT;						//��
		g_EndingResult[i].scale = ENDING_END_RESULT_IMAGE_DEFAULT_SCALE;				//�g�k�{��
		g_EndingResult[i].picy = picYTbl[i];											//�\����Y
		g_EndingResult[i].flag = false;													//�\���t���O
		g_EndingResult[i].ex = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(ENDING_END_THE_RESULT_IMAGE_WDIHT * ENDING_END_RESULT_IMAGE_DEFAULT_SCALE);
	}

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
		g_EndTheResult[i].score_x = posTbl[i].fX;
		g_EndTheResult[i].score_y = posTbl[i].fY;
		g_EndTheResult[i].spaceX = 10.0f;												//�\���ԊuX
		g_EndTheResult[i].spaceY = 0.0f;												//�\���ԊuY
		g_EndTheResult[i].score_scale = ENDING_END_THE_RESULT_TIME_DEFAULT_SCALE;		//�g�k��
		g_EndTheResult[i].scoreColor = SCORE_BLUE;										//�\���F
		g_EndTheResult[i].ch.type = SCORE_NUMBER_TYPE_NORMAL;							//�X�R�A�̕\�����
		g_EndTheResult[i].ti.cntFlag = false;											//���Ԍo�߃t���O
		g_EndTheResult[i].digit = digitTbl[i];											//���̐�
		g_EndTheResult[i].gradually = true;												//���X�ɉ��Z�E���Z�t���O
		g_EndTheResult[i].graduallyDirect = directionTbl[i];							//�X�R�A�̉��o
		g_EndTheResult[i].rollTime = rollingTimeTbl[i];									//��]����
		g_EndTheResult[i].renewalColor = SCORE_YELOW;									//�X�V���̕ύX�F
		g_EndTheResult[ENDING_SCORE_TOTAL_POINT].numD = 0;								//�n�߂ɕ\�����鐔�l
		g_EndTheResult[ENDING_SCORE_TOTAL_POINT].interval = intervalTbl[i];				//�\���Ԋu
	}
	
	//�� �X�R�A�̏�����
	InitScore( hWnd );

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
		g_EndingRecordCh[i].x = 640.0f +
			((ENDING_END_THE_RESULT_IMAGE_WDIHT * scaleTbl[i]) * i);
		g_EndingRecordCh[i].ex = indicationXTbl[i];
		g_EndingRecordCh[i].y = indicationYTbl[i];
		g_EndingRecordCh[i].mx = ENDING_END_RESULT_IMAGE_MOVE_X - (i * 2.0f);
		g_EndingRecordCh[i].h = ENDING_END_THE_RESULT_IMAGE_HEIGHT;
		g_EndingRecordCh[i].w = ENDING_END_THE_RESULT_IMAGE_WDIHT;
		g_EndingRecordCh[i].scale = scaleTbl[i];
		g_EndingRecordCh[i].picy = ENDING_END_THE_RESULT_IMAGE_HEIGHT * (i + 1);
		g_EndingRecordCh[i].flag = false;
	}
	//first drawing is chian's record
	g_EndingRecordCh[RECORD_HALF_WAY_RECORD_CHAIN].flag = true;

	// initialize each competitor's terminate result
	InitializeTerminateResult();
	//initialize ranking image
	for (int i = 0; i < RECORD_KIND_OF_PARTICIPANT; i++) {
		g_EndingTheHalfRanking[i].score_x = RESULT_SCORE_THE_HALF_WAY_RESULT_RANKING_INDICATE_X;
		g_EndingTheHalfRanking[i].score_y = RESULT_SCORE_THE_HALF_WAY_RESULT_RANKING_INDICATE_Y +
			(RESULT_SCORE_THE_HALF_WAY_RESULT_RANKING_INDICATE_SPACE_Y * i);
		g_EndingTheHalfRanking[i].score_scale = RESULT_SCORE_THE_HALF_WAY_RESULT_RANKING_SCALE_RATE;
		g_EndingTheHalfRanking[i].num = (i + 1);
		g_EndingTheHalfRanking[i].ch.flag = false;
		g_EndingTheHalfRanking[i].scoreColor = SCORE_RANING_COLOR_RED;
	}
	g_EndingTheHalfRanking[0].scoreColor = SCORE_RANING_COLOR_RED;				//change color, white into red

	//Initialize bonus' value, that score and image
	InitializeResultBonusForRanking(g_EndingBonusScore, g_EndingBonusCh);
}

//************************************************************************************************
//�X�V
//************************************************************************************************
void UpdateEnding(){

	//BGM�̍Đ�
	PlayStreaming( true );

	//present scene diverge scene's flag of global variable
	switch (g_EndingSceneNow) {
	case ENDING_SCENE_STAGE_RECORD:							//stage record
		UpdateEndingStageRecord();
		break;
	case ENDING_SCENE_THE_TERMINATE_RECORD:					//the terminate record
		UpdateTerminateRecord();
		break;
	case ENDING_SCENE_TERMINATE_RANKING:					//each participants' terminate record
		if (g_EndingTheHalfWayTurn == RESULT_RANKING_TUNR_NEXT_SCENE && 
			g_EndingBonusScore[RECORD_HALF_WAY_RECORD_TOTAL_POINT].directEnd) {
			if (JustKey('Z') || JustButton(BUTTON_1)) { 					//change scene
				if (g_EndingSceneNext != ENDING_SCENE_CREDIT) {				//once only process
					EndRankingToTheEndCredit();								//end ranking process to the end credit process
					//to the end credit
					g_EndingSceneNext = ENDING_SCENE_CREDIT;
				}
			}
		}
		else {
			UpdateTerminateRankingResult();		//Update each score of participants
		}
		//draw record's title
		for (int i = 0; i < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; i++) {
			if (g_EndingRecordCh[i].flag) {
				//move to indication X of terminate
				if (g_EndingRecordCh[i].x > g_EndingRecordCh[i].ex) g_EndingRecordCh[i].x += g_EndingRecordCh[i].mx;		//move
				if (g_EndingRecordCh[i].x <= g_EndingRecordCh[i].ex) g_EndingRecordCh[i].x = g_EndingRecordCh[i].ex;		//terminate
			}
		}
		break;
	case ENDING_SCENE_CREDIT:								//the end
		//credit process
		UpdateEndingScrollScene();
		break;
	}
	//blank time to change scene
	if (g_EndingSceneNow != g_EndingSceneNext) {
		//count time
		g_EndingBg[0].time++;
		//if count time more than 20 fram, subsutitute next scene for present scene
		if (g_EndingBg[0].time >= RESULT_SCENE_BLANK_TIME) {
			//subsutitute scene for present flag
			g_EndingSceneNow = g_EndingSceneNext;
			//reset count time
			g_EndingBg[0].time = 0;
		}
	}
}

//************************************************************************************************
//�\��
//************************************************************************************************
void DrawEnding() {

	//�J�������W
	float cY = GetCameraY();

	//�X�R�A�\���֐��̕���
	void(*pscore[RECORD_MAX])(SCOREVALUE*) = {

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
	char ScoreItemTbl[STAGE_MAX][RECORD_MAX] = {

		{ 5 , 5 , 1 , 1 , 1 } ,			//�C

		{ 2 , 3 , 1 , 1 , 2 } ,			//���H

		{ 4 , 1 , 1 , 1 , 1 } ,			//�I�t���[�h

	};

	//kind of item for the terminate result
	tagRECORDMATRIX terminateRecordTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = { RECORD_TOTAL, RECORD_TIME };

	//�ŏI���ʉ摜�̕\��
	if (g_EndingBg[1].flag) {

		//�w�i�摜
		DDBlt(
			(int)g_EndingBg[1].x,		//�\�����WX
			(int)g_EndingBg[1].y,		//�\�����W�x
			g_EndingBg[1].w,			//��
			g_EndingBg[1].h,			//����
			g_EndingImg[1],				//�摜�̗v�f�ԍ�
			g_EndingBg[1].picx,			//�\����X
			g_EndingBg[1].picy);		//�\�������WY
	}


	//present scene diverge scene's flag of global variable
	switch (g_EndingSceneNow) {
	case ENDING_SCENE_STAGE_RECORD:							//stage record
		//���e���ڂ̒l��\��
		if (g_EndingStage.flag) {
			for (int j = 0; j < RECORD_MAX; j++) {
				for (int i = 0; i < ScoreItemTbl[g_EndResultViewNow][j]; i++) {
					//if draw ranking
					if (g_EndResultViewNow == STAGE_ROAD && j == RECORD_TOTAL && i == 1) {
						//for ranking drawing
						DrawRanking(g_EndingScore[RECORD_TOTAL][1].score_x,
							g_EndingScore[RECORD_TOTAL][1].score_y,
							g_EndingScore[RECORD_TOTAL][1].score_scale,
							g_EndingScore[RECORD_TOTAL][1].num,
							(tagSCORERANKINGCOLOR)g_EndingScore[RECORD_TOTAL][1].scoreColor);
						break;
					}
					//�X�R�A�\���̕���
					pscore[j](&g_EndingScore[j][i]);
				}
			}
		}
		//�e��X�e�[�W������摜
		if (g_EndingStage.flag)
		{
			DDScaleBltSelectIndicate(
				g_EndingStage.x,			//�\�����WX
				g_EndingStage.y,			//�\�����W�x
				g_EndingStage.w,			//��
				g_EndingStage.h,			//����
				g_EndingImg[2],			//�摜�̗v�f�ԍ�
				g_EndingStage.picx,		//�\����X
				g_EndingStage.picy,		//�\����Y
				g_EndingStage.scale,		//�g�k�{��
				0							//�\���N�_���W�t���O
				);
		}
		break;
	case ENDING_SCENE_THE_TERMINATE_RECORD:					//the terminate record
		//�� �ŏI���ʂ�\��
		DrawScore(&g_EndTheResult[ENDING_SCORE_CHAIN]);			//chain
		DrawTime(&g_EndTheResult[ENDING_SCORE_TIME]);			//����
		DrawScore(&g_EndTheResult[ENDING_SCORE_TOTAL_POINT]);	//�W�v
		//�e��ŏI���ʕ�����摜
		for (int i = 0; i < ENDING_SCORE_AND_TITLE; i++) {
			if (g_EndingResult[i].flag)
			{
				DDScaleBltSelectIndicate(
					g_EndingResult[i].x,			//�\�����WX
					g_EndingResult[i].y,			//�\�����W�x
					g_EndingResult[i].w,			//��
					g_EndingResult[i].h,			//����
					g_EndingImg[3],					//�摜�̗v�f�ԍ�
					g_EndingResult[i].picx,			//�\����X
					g_EndingResult[i].picy,						//�\����Y
					g_EndingResult[i].scale,					//�g�k�{��
					DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP	//�\���N�_���W�t���O
					);
			}
		}

		break;
	case ENDING_SCENE_TERMINATE_RANKING:					//each participants' their terminate record
		//draw the half way result
		for (int j = 0; j < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; j++) {
			for (int i = 0; i < RECORD_KIND_OF_PARTICIPANT; i++) {
				if (g_EndingTheHalWayRecord[j][i].ch.flag) {
					//draw score of the half way result
					pscore[terminateRecordTbl[j]](&g_EndingTheHalWayRecord[j][i]);
				}
			}
		}
		//draw ranking for the half way result
		for (int i = 0; i < RECORD_KIND_OF_PARTICIPANT; i++) {
			if (g_EndingTheHalfRanking[i].ch.flag) {
				DrawRanking(g_EndingTheHalfRanking[i].score_x,
					g_EndingTheHalfRanking[i].score_y,
					g_EndingTheHalfRanking[i].score_scale,
					g_EndingTheHalfRanking[i].num,
					(tagSCORERANKINGCOLOR)g_EndingTheHalfRanking[i].scoreColor);
			}
		}
		//draw title of the half way record
		for (int i = 0; i < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; i++) {
			if (g_EndingRecordCh[i].flag) {
				//indication way is scaleBlt
				DDScaleBltSelectIndicate(
					g_EndingRecordCh[i].x,
					g_EndingRecordCh[i].y,
					g_EndingRecordCh[i].w,
					g_EndingRecordCh[i].h,
					g_EndingImg[3],
					g_EndingRecordCh[i].picx,
					g_EndingRecordCh[i].picy,
					g_EndingRecordCh[i].scale,
					DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP);
			}
		}
		//draw bonus
		for (int i = 0; i < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; i++) {
			if (g_EndingBonusScore[i].ch.flag) {					//socre
				DrawScore(&g_EndingBonusScore[i]);
			}
			if (g_EndingBonusCh[i].flag) {							//image
				//indication way is scaleBlt
				DDScaleBltSelectIndicate(
					g_EndingBonusCh[i].x,
					g_EndingBonusCh[i].y,
					g_EndingBonusCh[i].w,
					g_EndingBonusCh[i].h,
					g_EndingImg[5],
					g_EndingBonusCh[i].picx,
					g_EndingBonusCh[i].picy,
					g_EndingBonusCh[i].scale,
					DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP);
			}
		}
		break;
	case ENDING_SCENE_CREDIT:								//the end
		//�G���f�B���O�摜�̕\��
		if (g_EndingBg[0].flag) {
			DDBlt(
				(int)g_EndingBg[0].x,		//�\�����WX
				(int)g_EndingBg[0].y,		//�\�����W�x
				g_EndingBg[0].w,			//��
				g_EndingBg[0].h,			//����
				g_EndingImg[0],				//�摜�̗v�f�ԍ�
				g_EndingBg[0].picx,			//�\����X
				g_EndingBg[0].picy);		//�\�������WY

			//�e��N���W�b�g������摜
			for (int i = 0; i < (ENDING_CREDIT_MAX + 1); i++) {
				if (g_EndingCredit[i].flag) {
					DDScaleBltSelectIndicate(
						g_EndingCredit[i].x,					//�\�����WX
						g_EndingCredit[i].y - g_EndingBg[0].y,	//�\�����W�x
						g_EndingCredit[i].w,					//��
						g_EndingCredit[i].h,					//����
						g_EndingImg[4],						//�摜�̗v�f�ԍ�
						g_EndingCredit[i].picx,				//�\����X
						g_EndingCredit[i].picy,				//�\����Y
						g_EndingCredit[i].scale,				//�g�k�{��
						0										//�\���N�_���W�t���O
						);
				}
			}
		}
		break;
	}
}

//************************************************************************************************
//���
//************************************************************************************************
void ReleaseEnding(){
	
	//�X�e�[�W
	tagSELECTSTAGE stageTbl[ STAGE_MAX ] = { STAGE_SEA , STAGE_ROAD , STAGE_OFF_ROAD };
	//�摜�̉��
	for( int i = 0 ; i < ENDING_IMAGE_MAX ; i++ ){ReleaseDDImage( g_EndingImg[i] );}
	//�t�@�C���̉��
	ZeroMemory( g_EndingFile , sizeof( TALK ) * OPTION_FILE_MAX );
	//�X�R�A�̉��
	ReleaseScore();
	//�ǂݍ��񂾃t�@�C���̉��
	ReleaseTalk();
	//�Ȃ̉��
	ReleaseStreaming();
	//�� �N���A�σX�e�[�W�����Z�b�g
	for( int i = 0 ; i < STAGE_MAX ; i++ ){
		SetCompleteStage( SELECTSTAGE_STAGE_NOT_COMPLETE , stageTbl[i] );
	}

	//�� �e��X�e�[�W���ʕ\���p��������
	for( int j = 0 ; j < RECORD_MAX ; j++ ){
		for( int i = 0 ; i < RECORD_MAX ; i++ ){
			ZeroMemory( &g_EndingScore[j][i] , sizeof( SCOREVALUE ) );	
		}
	}
	//�� �ŏI���ʕ\���p�̐ݒ�
	//�e��X�R�A�l�\���p��0�ŏ�����
	ZeroMemory( &g_EndTheResult[0] , sizeof( SCOREVALUE ) * ENDING_KIND_OF_SCOREVALUE );
	//�\���ݒ�p��0�ŏ�����
	ZeroMemory(&g_EndingResult[0], sizeof(CHARACTER) * ENDING_SCORE_AND_TITLE);

	//reset the half way result
	for (int j = 0; j < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; j++) {
		for (int i = 0; i < RECORD_KIND_OF_PARTICIPANT; i++) {
			ZeroMemory(&g_EndingTheHalWayRecord[j][i], sizeof(SCOREVALUE));
		}
	}
	//bonus score
	ZeroMemory(g_EndingBonusScore, sizeof(SCOREVALUE) * RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT);
}


/*
*************************************************************************
�t�@�C���̕\���X�V����
*************************************************************************
*/
void	UpdateCreateFile(){

	//�Ō�̃t�@�C���܂œǂݍ���ł��Ȃ�������ǂݍ���
	if( g_EndingFile[ OPTION_FILE_MAX - 1 ].fRead == false ){		
		//�ǂݍ��݂����t�@�C�����܂Ń��[�v����
		for( int i = 2 ; i < OPTION_FILE_MAX ; i++ ){	
			//�e�t�@�C�������Ԃɓǂݍ���ł���
			if( g_EndingFile[i].fRead == false ){
				//�����񐶐�
				CreateTalk( &g_EndingFile[i] );
				//�ǂݍ��񂾃t���O��true
				g_EndingFile[i].fRead = true;
			}
		}
	}

}

/*
*************************************************************************
�ŏI���ʕ\���X�V����
*************************************************************************
*/
void	UpdateEndTheResult(){

	//Each stage's items
	int scoreItemTbl[STAGE_MAX][RECORD_MAX] = {

		{ 5 , 5 , 1 , 1 , 1 } ,			//Sea
		{ 2 , 3 , 1 , 1 , 1 } ,			//Road
		{ 4 , 1 , 1 , 1 , 1 } ,			//Off-road
	};
	//�ǂݍ��ރX�e�[�W
	tagENDTHERESULTITEM itemTbl[ ENDING_ITEM_MAX ] = { 	
		ENDING_ITEM_SEA		,
		ENDING_ITEM_ROAD	,
		ENDING_ITEM_OFFROAD ,
		ENDING_ITEM_END		,
	};
	
	//�� �e�X�e�[�W�̋L�^�������v�f
	tagRECORDMATRIX recordTypeTbl[ RECORD_MAX ] = {

		RECORD_ONE			,				//�v�f1
		RECORD_TWO			,				//�v�f2
		RECORD_TIME			,				//���Ԃ̗v�f
		RECORD_FOUR			,				//�v�f4
		RECORD_TOTAL		,				//�W�v�|�C���g�̗v�f
	};

	//�ǂݍ��݂͈��̂�
	if( g_EndResultReadFlag == false ){
		//�ǂݍ��ނ̃X�e�[�W�̋L�^����
		for( int i = g_EndResultViewNext ; i < STAGE_MAX ; i++ ){
			//���̃X�e�[�W�ԍ���ǂݍ���
			if( g_EndResultViewNext == itemTbl[i] ){
				//�ǂݍ��ރX�e�[�W�ԍ����X�V
				g_EndResultViewNow = g_EndResultViewNext;
				//����L�^�����e�X�e�[�W�̃t�@�C����ǂݍ��݋L�^�����ɕ\�����Ă���
				//�L�^�t�@�C���̓ǂݍ���
				LoadOptionStageFile( &g_EndingScore[0][0].re , &g_EndingFile[0] , g_EndResultViewNow , RECORD_PREVIEW );
				//�� �X�R�A�\���p�̐ݒ�
				//���ڂ̐������ݒ�
				for (int k = 0; k < RECORD_MAX; k++) {
					for (int j = 0; j < scoreItemTbl[g_EndResultViewNow][k]; j++) {
						SetOptionStageScoreDirection(&g_EndingScore[k][j], k, j);
					}
				}

				//�X�R�A�\���p�ɒl����
				for( int j = 0 ; j < RECORD_MAX ; j++ ){
					SetOptionStageScoreForDraw( &g_EndingScore[ recordTypeTbl[j] ][0] , &g_EndingScore[0][0].re , g_EndResultViewNow , recordTypeTbl[j] );
					//difference setting for stage and present language
					DeferScoreOrFixedFileSetting(g_EndingScore[j], g_EndingFile, (tagSELECTSTAGE)g_EndResultViewNow, j);
				}
				//�\���p�Ɋe�X�e�[�W�̎��ԂƏW�v�|�C���g�̍��v�����߂Ă���
				//chain
				g_EndTheResult[ENDING_SCORE_CHAIN].num += g_EndingScore[RECORD_FOUR][0].num;
				//����
				g_EndTheResult[ENDING_SCORE_TIME].ti.minutes += g_EndingScore[RECORD_TIME][0].ti.minutes;		//��
				g_EndTheResult[ENDING_SCORE_TIME].ti.second += g_EndingScore[RECORD_TIME][0].ti.second;			//�b
				g_EndTheResult[ENDING_SCORE_TIME].ti.millisecond += g_EndingScore[RECORD_TIME][0].ti.millisecond;		//�~���b
				//�W�v�|�C���g
				g_EndTheResult[ENDING_SCORE_TOTAL_POINT].num += g_EndingScore[RECORD_TOTAL][0].num;

				//���Ԃ̌J��グ�v�Z
				//�~���b
				if( g_EndTheResult[ENDING_SCORE_TIME].ti.millisecond > 99 ){
					g_EndTheResult[ENDING_SCORE_TIME].ti.millisecond = g_EndTheResult[ENDING_SCORE_TIME].ti.millisecond - 100;
					g_EndTheResult[ENDING_SCORE_TIME].ti.second++;
				}
				//�b
				if( g_EndTheResult[ENDING_SCORE_TIME].ti.second > 59 ){
					g_EndTheResult[ENDING_SCORE_TIME].ti.second = g_EndTheResult[ENDING_SCORE_TIME].ti.second - 60;
					g_EndTheResult[ENDING_SCORE_TIME].ti.minutes++;
				}

				//�e��X�e�[�W�̃C���[�W�摜�̕\���ݒ�
				g_EndingStage.flag = true;											//�\���t���O
				g_EndingStage.picy = ( g_EndResultViewNow * g_EndingStage.h );		//�\����Y
				g_EndingStage.x = ENDING_END_RESULT_IMAGE_INDICATE_X;				//�\����X

				//�ǂݍ��݃t���O��true
				g_EndResultReadFlag = true;
				//���̃X�e�[�W�ԍ�����
				g_EndResultViewNext = itemTbl[i + 1];
				break;
			}
		}
	}

}


/*
*************************************************************************
�G���f�B���O�摜�X�N���[������
*************************************************************************
*/
void	UpdateEndingScrollScene(){

	//�� �G���h�N���W�b�g�p�̉摜�v�f�ԍ�
	tagENDINGENDTHECREDIT creditTbl[ ENDING_CREDIT_MAX + 1 ] = {

		ENDING_CREDIT_MATERIAL_PROVIDERS					,
		ENDING_CREDIT_BGM_PROVIDERS							,
		ENDING_CREDIT_BGM_PROVIDER_TAM						,
		ENDING_CREDIT_BGM_AND_SOUND_EFFECT_PROVIDER_CIRCUIT	,
		ENDING_CREDIT_SOUND_EFFECT_PROVIDERS				,
		ENDING_CREDIT_BGM_AND_SOUND_EFFECT_PROVIDER_CIRCUIT	,
		ENDING_CREDIT_SOUND_EFFECT_PROVIDER_ONJIN			,
		ENDING_CREDIT_TANKS_PROVIDERS						,
		ENDING_CREDIT_PRESENTED_OWN_NAME					,
		ENDING_CREDIT_TANKS_PLAYERS							,

	};

	//draw back ground image of the end credit
	g_EndingBg[0].flag = true;
	
	//����L�[�i�{�^���j���͂ŃI�[�v�j���O�V�[���֕ύX
	if( JustKey( 'Z' ) || JustButton(BUTTON_1)) CreateWipe( SCENE_OPENING , CHANGE_SCENE , WIPE_CONTRACTION );

	//�w�i�摜��Y���W�ɍ��킹�ăN���W�b�g���o���Ă���
	for( int i = 0 ; i < ( ENDING_CREDIT_MAX + 1 ); i++ ){
		if( g_EndingCredit[i].flag ) continue;
		if( ( g_EndingBg[0].h - STAGE_HEIGHT ) - abs( (int)g_EndingBg[0].y ) > ( i * ENDING_END_THE_CREDIT_IMAGE_INDICATE_SPACE_Y ) ){
			//��ʒ�����Y���W�ɑΏۂ̃N���W�b�g��z�u
			g_EndingCredit[ i ].y = ( g_EndingBg[0].y + ENDING_END_THE_CREDIT_IMAGE_INDICATE_SPACE_Y );
			//�\����
			g_EndingCredit[ i ].picy = ( creditTbl[i] * ENDING_END_THE_CREDIT_IMAGE_HEIGHT );
			//�\��
			g_EndingCredit[ i ].flag = true;
			//�f�ޒ񋟌��͏����傫��
			g_EndingCredit[2].scale = 1.0f;				//�g�k�{��
			g_EndingCredit[3].scale = 1.0f;				//�g�k�{��
			g_EndingCredit[5].scale = 1.0f;				//�g�k�{��
			g_EndingCredit[6].scale = 1.0f;				//�g�k�{��
			continue;
		}
	}
	//�摜�����փX�N���[��
	g_EndingBg[0].y += g_EndingBg[0].my;

	//�摜�����[�܂œ��B������摜�̈ړ����~�߂�
	if( g_EndingBg[0].y > 0 ){
		//Y���W���摜���[����ʉ��[�ɏd�Ȃ����l�Œ�~������
		g_EndingBg[0].y = 0.0f;
		//�ړ��ʂ����Z�b�g
		g_EndingBg[0].my = 0.0f;
	}

	//�e��N���W�b�g������摜�̈ړ��ʂ̍X�V
	for( int i = 0 ; i < ( ENDING_CREDIT_MAX + 1 ) ; i++ ){
		if( g_EndingCredit[i].flag ){
			g_EndingCredit[i].x += g_EndingCredit[i].mx;						//�ړ���X
			//�ŏI�I�ȕ\����
			if( g_EndingCredit[i].ex > g_EndingCredit[i].x ) g_EndingCredit[i].x = g_EndingCredit[i].ex;
		
		}
	}

}

/*
Initialize the half way result
*/
void InitializeTerminateResult() {

	//kind of score for the half way result
	tagRECORDTHEHALFWAYITEM recordNumTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		RECORD_HALF_WAY_RECORD_TOTAL_POINT,				//aggregate point
		RECORD_HALF_WAY_RECORD_RACE_TIME,				//time of race
		RECORD_HALF_WAY_RECORD_CHAIN,					//consecutive something
	};
	//stage
	tagSELECTSTAGE stage[STAGE_MAX] = { STAGE_SEA, STAGE_ROAD, STAGE_OFF_ROAD };
	//for getting record
	RECORD	*lpRecord[STAGE_MAX];
	//get present game's level
	tagGAMELEVEL gamelevel = GetGameLevel();
	//record's item
	tagRECORDMATRIX recordTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		RECORD_TIME, RECORD_FOUR, RECORD_TOTAL,				//time, chain, total point
	};
	//get every competitor's records
	for (int i = 0; i < STAGE_MAX; i++) {
		lpRecord[i] = GetRecordCompetitors(stage[i]);
	}

	//subsutitute score's direction and indication value for the half way record
	for (int k = 0; k < STAGE_MAX; k++) {																		//kind of stage
		for (int j = 0; j < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; j++) {									//kind of reocrd
			for (int i = 0; i < RECORD_KIND_OF_PARTICIPANT; i++) {												//kind of competitor
				//subsutitute indication value for drawing variable
				SetResultTheHalfWayValue(&g_EndingTheHalWayRecord[j][i], recordNumTbl[j], i, SCORE_BLUE);
				//when player's turn, nothing subsutitution value
				if (i == PARTICIPANT_PLAYER)	break;
				//subsutitute the half way record for score's variable
				switch (recordTbl[j])
				{
				case RECORD_TIME:		//time
					g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti.minutes += lpRecord[k][i].recordBox[RECORD_TIME][0];			//minute
					g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti.second += lpRecord[k][i].recordBox[RECORD_TIME][1];				//second
					g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti.millisecond += lpRecord[k][i].recordBox[RECORD_TIME][2];		//millisecond

					if (g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti.millisecond > 99) {
						g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti.millisecond = 0;
						g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti.second++;
					}
					if (g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti.second > 59) {
						g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti.second = 0;
						g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti.minutes++;
					}
					break;
				case RECORD_FOUR:		//chain
					g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_CHAIN][i].num += lpRecord[k][i].recordBox[RECORD_FOUR][0];
					break;
				case RECORD_TOTAL:		//total
					g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][i].num += lpRecord[k][i].recordBox[RECORD_TOTAL][0];
					break;
				default:
					break;
				}
			}
		}
	}
}

/*
Update stage record
each stage's record draw score to consecutive 
*/
void	UpdateEndingStageRecord() {

	//�� �ŏI���ʏ���
	if (g_EndingBg[1].flag && g_EndResultViewNow != ENDING_ITEM_END)
	{
		//�� �e��X�e�[�W���ʂ̕\���X�V
		UpdateEndTheResult();

		//�� �t�@�C���̕\���X�V
		UpdateCreateFile();

		//�S�Ă̌��ʂ�\�����
		if (g_EndingScore[RECORD_TOTAL][0].directEnd) {
			//�ǂݍ��݊�����L�[����Ŏ��̃X�e�[�W��ǂݍ���ł���
			if (JustKey('Z') || JustButton(BUTTON_1) &&
				g_EndResultReadFlag && g_EndResultViewNow != g_EndResultViewNext) {
				//�e�탊�Z�b�g
				//Talk�̉��
				ReleaseTalk();
				//�ǂݍ��񂾃t�@�C���̃t���O��false��
				for (int i = 2; i < OPTION_FILE_MAX; i++) { g_EndingFile[i].fRead = false; }
				//�ǂݍ��݃t���O��false
				g_EndResultReadFlag = false;
				//���ʃC���[�W������̔�\��
				g_EndingStage.flag = false;
				//�e��X�R�A�̉��o�I���t���O��false��
				for (int j = 0; j < RECORD_MAX; j++) {
					for (int i = 0; i < RECORD_MAX; i++) {
						g_EndingScore[j][i].directEnd = false;
					}
				}
				//�����ŏI���ʂȂ�L�^�\���̗v�f���X�V
				if (g_EndResultViewNext == ENDING_ITEM_END) {
					g_EndResultViewNow = g_EndResultViewNext;
					//�ŏI���ʕ�����̕\���t���O��true��
					for (int i = 0; i < ENDING_SCORE_AND_TITLE; i++) { g_EndingResult[i].flag = true; }
					//subsutitute scene flag for
					g_EndingSceneNext = ENDING_SCENE_THE_TERMINATE_RECORD;
					return;
				}
			}
		}
	}

	//�e��X�e�[�W������摜�̈ړ��ʂ̍X�V
	if (g_EndingStage.flag)
	{
		g_EndingStage.x += g_EndingStage.mx;						//�ړ���X
																	//�ŏI�I�ȕ\����
		if (g_EndingStage.ex > g_EndingStage.x) g_EndingStage.x = g_EndingStage.ex;
	}
}

/*
Update the terminate record
*/
void	UpdateTerminateRecord() {

	//�S�Ă̌��ʂ�\�����
	if (g_EndTheResult[ENDING_SCORE_TOTAL_POINT].directEnd) {
		if (JustKey('Z') || JustButton(BUTTON_1)) {
			//�� subsutitute total point and time record in present stage for player's item
			//total point
			g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][PARTICIPANT_PLAYER].num = g_EndTheResult[ENDING_SCORE_TOTAL_POINT].num;
			//time record
			g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][PARTICIPANT_PLAYER].ti.minutes = g_EndTheResult[ENDING_SCORE_TIME].ti.minutes;
			g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][PARTICIPANT_PLAYER].ti.second = g_EndTheResult[ENDING_SCORE_TIME].ti.second;
			g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][PARTICIPANT_PLAYER].ti.millisecond = g_EndTheResult[ENDING_SCORE_TIME].ti.millisecond;
			//chain record
			g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_CHAIN][PARTICIPANT_PLAYER].num = g_EndTheResult[ENDING_SCORE_CHAIN].num;
			//signal color for player
			g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][PARTICIPANT_PLAYER].scoreColor = SCORE_YELOW;
			g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][PARTICIPANT_PLAYER].scoreColor = SCORE_YELOW;
			g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_CHAIN][PARTICIPANT_PLAYER].scoreColor = SCORE_YELOW;
			//reset bonus materials
			for (int i = 0; i < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; i++) { g_EndingPlayerBonusMaterial[i] = 0; }
			//decide player's ranking and get that value to time's record
			for (int i = 0; i < PARTICIPANT_PLAYER; i++) {
				//player's material of bonus point
				g_EndingPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_RACE_TIME] += CompairRaceTimeRecord(&g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][PARTICIPANT_PLAYER].ti,
					&g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti);
				//competitor
				//if player's material more than competitor's own material, competitor beats player
				if (g_EndingPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_RACE_TIME] > i) {
					g_EndingCompetitorBonusMaterial[RECORD_HALF_WAY_RECORD_RACE_TIME][i] = i;
				}
				else {
					//player beats competitor
					g_EndingCompetitorBonusMaterial[RECORD_HALF_WAY_RECORD_RACE_TIME][i] = i + 1;
				}
			}
			//decide player's ranking of chain
			for (int i = 0; i < PARTICIPANT_PLAYER; i++) {
				g_EndingPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_CHAIN] +=
					CompairRecord((int)g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_CHAIN][PARTICIPANT_PLAYER].num,
						(int)g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_CHAIN][i].num);
				//competitor
				//if player's material more than competitor's own material, competitor beats player
				if (g_EndingPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_CHAIN] > i) {
					g_EndingCompetitorBonusMaterial[RECORD_HALF_WAY_RECORD_CHAIN][i] = i;
				}
				else {
					//player beats competitor
					g_EndingCompetitorBonusMaterial[RECORD_HALF_WAY_RECORD_CHAIN][i] = i + 1;
				}
			}
			//compair total point between player and competitor
			for (int i = 0; i < PARTICIPANT_PLAYER; i++) {
				g_EndingPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_TOTAL_POINT] +=
					CompairRecord((int)g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][PARTICIPANT_PLAYER].num,
						(int)g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][i].num);
			}
			//player's ranking bonus points
			tagRESULTBONUS bonusTbl[RESULT_KIND_OF_BONUS] = {
				RESULT_BONUS_FIRST,					//the first
				RESULT_BONUS_SECOND,				//the second
				RESULT_BONUS_THIRD,					//the third
				RESULT_BONUS_FOURTH,				//the fourth
				RESULT_BONUS_FIFTH,					//the fifth
				RESULT_BONUS_SIXTH,					//the sixth
				RESULT_BONUS_SEVENTH,				//the seventh
				RESULT_BONUS_EIGHTTH,				//the eightth
				RESULT_BONUS_NINETH,				//the nineth
				RESULT_BONUS_TENTH,					//the tenth
			};
			//subsutition bonus point
			int bonusPointTbl2[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
				(int)bonusTbl[g_EndingPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_CHAIN]],			//chain
				(int)bonusTbl[g_EndingPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_RACE_TIME]],		//time
				(int)(bonusTbl[g_EndingPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_CHAIN]] +
					bonusTbl[g_EndingPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_RACE_TIME]]),		//total
			};
			//kind of record
			tagRECORDTHEHALFWAYITEM recordTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
				RECORD_HALF_WAY_RECORD_CHAIN,			//chain
				RECORD_HALF_WAY_RECORD_RACE_TIME,		//time
				RECORD_HALF_WAY_RECORD_TOTAL_POINT,		//total
			};

			//subsutitute bonus point for score
			for (int i = 0; i < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; i++) {
				g_EndingBonusScore[recordTbl[i]].num = bonusPointTbl2[i];
			}
			//next scene
			g_EndingSceneNext = ENDING_SCENE_TERMINATE_RANKING;
			return;
		}
	}
	//�e��ŏI���ʕ�����摜�̈ړ��ʂ̍X�V
	for (int i = 0; i < ENDING_SCORE_AND_TITLE; i++) {
		if (g_EndingResult[i].flag) {
			g_EndingResult[i].x += g_EndingResult[i].mx;						//�ړ���X
																				//�ŏI�I�ȕ\����
			if (g_EndingResult[i].ex > g_EndingResult[i].x) g_EndingResult[i].x = g_EndingResult[i].ex;
		}
	}
}


/*
Update the half way result
*/
void	UpdateTerminateRankingResult() {

	if (g_EndingTheHalfWayTurn == RESULT_RANKING_TUNR_NEXT_SCENE) return;		//check eror

	//player's ranking bonus points
	tagRESULTBONUS bonusTbl[RESULT_KIND_OF_BONUS] = {
		RESULT_BONUS_FIRST,					//the first
		RESULT_BONUS_SECOND,				//the second
		RESULT_BONUS_THIRD,					//the third
		RESULT_BONUS_FOURTH,				//the fourth
		RESULT_BONUS_FIFTH,					//the fifth
		RESULT_BONUS_SIXTH,					//the sixth
		RESULT_BONUS_SEVENTH,				//the seventh
		RESULT_BONUS_EIGHTTH,				//the eightth
		RESULT_BONUS_NINETH,				//the nineth
		RESULT_BONUS_TENTH,					//the tenth
	};
	//kind of record
	tagRECORDTHEHALFWAYITEM recordTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		RECORD_HALF_WAY_RECORD_CHAIN,			//chain
		RECORD_HALF_WAY_RECORD_RACE_TIME,		//time
		RECORD_HALF_WAY_RECORD_TOTAL_POINT,		//total
	};

	//material of each turns
	//kind of drawing turn
	//treat scene's flag in the half way result and terminate result
	tagRESULTRANKINGEACHTURN turnTbl[RESULT_KIND_OF_TURN - 1] = {
		RESULT_RANKING_TURN_CHAIN,				//chain's turn
		RESULT_RANKING_TURN_TIME,				//time's turn
		RESULT_RANKING_TURN_TOTAL,				//total's turn
	};
	//next turn
	tagRESULTRANKINGEACHTURN nextTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		RESULT_RANKING_TURN_TIME,				//time's turn
		RESULT_RANKING_TURN_TOTAL,				//total's turn
		RESULT_RANKING_TURN_TOTAL,				//still total
	};
	int nextTitleTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = { 0, 1, 1 };				//material of score's title

	if (g_EndingTheHalWayRecord[recordTbl[g_EndingTheHalfWayTurn]][PARTICIPANT_PLAYER].directEnd) {
		//draw each bonus, that score and image
		g_EndingBonusScore[recordTbl[g_EndingTheHalfWayTurn]].ch.flag = true;		//draw bonus score
		g_EndingBonusCh[recordTbl[g_EndingTheHalfWayTurn]].flag = true;				//draw bonus' images
		for (int i = 0; i < (RESULT_KIND_OF_TURN - 1); i++) {				//when present record indicate last item, to next record
			if (g_EndingTheHalfWayTurn == turnTbl[i]) {
				g_EndingTheHalfWayTurn = nextTbl[i];						//next record's score
				g_EndingRecordCh[nextTitleTbl[i]].flag = true;				//draw record's title
				break;
			}
		}
	}
	//draw half way result, that is total point, time record and chain's record
	//first drawing is chain's record
	for (int j = 0; j < RECORD_KIND_OF_PARTICIPANT; j++) {
		//each participants' score
		if (g_EndingTheHalWayRecord[recordTbl[g_EndingTheHalfWayTurn]][j].ch.flag == false) {
			g_EndingTheHalWayRecord[recordTbl[g_EndingTheHalfWayTurn]][j].ch.flag = true;
		}
		//ranking
		if (g_EndingTheHalfRanking[j].ch.flag == false) {
			g_EndingTheHalfRanking[j].ch.flag = true;
		}
	}

	//when end the last record drawing, subsutitute bonus point of current ranking for global variable of total point
	if (g_EndingTheHalWayRecord[recordTbl[g_EndingTheHalfWayTurn]][PARTICIPANT_PLAYER].directEnd) {
		//each other execute only once process
		//if player's ranking is less than five, add bonus point to total point
		if (g_EndingTheHalfWayTurn == RESULT_RANKING_TURN_TOTAL) {				//check out of allocation
			g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][PARTICIPANT_PLAYER].num +=					//add bonus point to total point
				(float)bonusTbl[g_EndingPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_CHAIN]] +
				(float)bonusTbl[g_EndingPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_RACE_TIME]];

			//ranking's image add scale rate for the first
			g_EndingTheHalfRanking[0].score_scale = RESULT_SCORE_THE_HALF_WAY_RESULT_RANKING_THE_FIRST_SCALE_RATE;

			//subsutitute each competitors' bonus point for total point
			for (int i = 0; i < PARTICIPANT_PLAYER; i++) {
				g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][i].num +=					//add bonus point to total point
					(float)bonusTbl[g_EndingCompetitorBonusMaterial[RECORD_HALF_WAY_RECORD_CHAIN][i]] +
					(float)bonusTbl[g_EndingCompetitorBonusMaterial[RECORD_HALF_WAY_RECORD_RACE_TIME][i]];
				//reset score's direction
				g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][i].directEnd = false;
				g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][i].numD = 0;				//reset score's drawing number
			}

			//reset score's direction for player
			g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][PARTICIPANT_PLAYER].directEnd = false;
			g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][PARTICIPANT_PLAYER].numD = 0;				//reset score's drawing number
			
																				
			//reset total's material
			g_EndingPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_TOTAL_POINT] = 0;
			for (int i = 0; i < PARTICIPANT_PLAYER; i++) {			//compair total point between player and competitor
				g_EndingPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_TOTAL_POINT] +=
					CompairRecord((int)g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][PARTICIPANT_PLAYER].num,
						(int)g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][i].num);
			}
		}

		//process for replace position Y with each participants
		//get competitor's position Y
		float competitorPosY[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = { 0 };
		//count participant
		int next = 0;
		//loop to kind of record
		for (int i = 0; i < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; i++) {
			//next item
			next = (g_EndingPlayerBonusMaterial[i] + 1);
			for (int j = g_EndingPlayerBonusMaterial[i]; j < PARTICIPANT_PLAYER; j++) {
				competitorPosY[i] = g_EndingTheHalWayRecord[i][j].score_y;					//get competitor's position Y
																							//next is less than 10
				if (next < RECORD_KIND_OF_PARTICIPANT) {
					//player's score replace with competitor's position
					g_EndingTheHalWayRecord[i][j].score_y = g_EndingTheHalWayRecord[i][next].score_y;
					g_EndingTheHalWayRecord[i][next].score_y = competitorPosY[i];
				}
				next = (j + 2);						//this is subsutitution material
			}
		}
		if (g_EndingTheHalfWayTurn == RESULT_RANKING_TURN_TOTAL) {
			g_EndingTheHalfWayTurn = RESULT_RANKING_TUNR_NEXT_SCENE;		//the end process
			return;
		}
	}
}

/*
End ranking process to the end credit
*/
void	EndRankingToTheEndCredit() {

	//release back music
	ReleaseStreaming();
	//load back music for credit
	CreateStreaming("BGM\\Ending.wav");
	//don't draw the end result of back ground image
	g_EndingBg[1].flag = false;
	//don't draw each the end score of image
	for (int i = 0; i < ENDING_SCORE_AND_TITLE; i++) { g_EndingResult[i].flag = false; }
	//don't draw ranking
	for (int i = 0; i < RECORD_KIND_OF_PARTICIPANT; i++) {
		for (int j = 0; j < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; j++) {
			g_EndingTheHalWayRecord[j][i].ch.flag = false;
			if (i == 0) {
				g_EndingBonusCh[j].flag = false;
				g_EndingBonusScore[j].ch.flag = false;
				g_EndingRecordCh[j].flag = false;
			}
		}
		g_EndingTheHalfRanking[i].ch.flag = false;
	}

	//�� save present terminate record
	//each record's item
	int		itemTbl[ENDING_KIND_OF_SCOREVALUE] = { 1 , 3 , 1 };
	//subsutitute each value for each record
	//chain
	g_EndTheResult[0].re.recordBox[ENDING_SCORE_CHAIN][0] = (int)g_EndTheResult[ENDING_SCORE_CHAIN].num;
	//time record
	g_EndTheResult[0].re.recordBox[ENDING_SCORE_TIME][0] = g_EndTheResult[ENDING_SCORE_TIME].ti.minutes;
	g_EndTheResult[0].re.recordBox[ENDING_SCORE_TIME][1] = g_EndTheResult[ENDING_SCORE_TIME].ti.second;
	g_EndTheResult[0].re.recordBox[ENDING_SCORE_TIME][2] = g_EndTheResult[ENDING_SCORE_TIME].ti.millisecond;
	//total point
	g_EndTheResult[0].re.recordBox[ENDING_SCORE_TOTAL_POINT][0] = (int)g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][PARTICIPANT_PLAYER].num;

	//write each record to these files
	SaveRecord("Data\\EndTheResult\\Preview.dat",
		ENDING_KIND_OF_SCOREVALUE, itemTbl, &g_EndTheResult[0].re);

	//renewal each record and write these record to these file
	RenewalRecordEndTheResult("Data\\EndTheResult\\Preview.dat", "Data\\EndTheResult\\Best.dat",
		ENDING_KIND_OF_SCOREVALUE, itemTbl);

	//save present terminate ranking and each records
	//kind of score
	tagRECORDTHEHALFWAYITEM recordTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		RECORD_HALF_WAY_RECORD_TOTAL_POINT,				//total
		RECORD_HALF_WAY_RECORD_RACE_TIME,				//time
		RECORD_HALF_WAY_RECORD_CHAIN,					//chain
	};
	//get present game's level
	tagGAMELEVEL level = GetGameLevel();
	//loop to kind_of record
	for (int j = 0; j < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; j++) {
		for (int i = 0; i < RECORD_KIND_OF_PARTICIPANT; i++) {			//loop to kind of participant
			switch (recordTbl[j]) {
			case RECORD_HALF_WAY_RECORD_TOTAL_POINT:					//total
				g_EndingTheHalWayRecord[0][0].re.recordBox[i][RECORD_RANKING_ITEM_TOTAL_POINT] =
					g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][i].num;
				//save each participants' color for each identification
				g_EndingTheHalWayRecord[0][0].re.recordBox[i][RECORD_RANKING_ITEM_COLOR_FOR_TOTAL] =
					g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][i].scoreColor;					//color
				break;
			case RECORD_HALF_WAY_RECORD_RACE_TIME:						//time
				g_EndingTheHalWayRecord[0][0].re.recordBox[i][RECORD_RANKING_ITEM_MINUTE] =
					g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti.minutes;					//minute
				g_EndingTheHalWayRecord[0][0].re.recordBox[i][RECORD_RANKING_ITEM_SECOND] =
					g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti.second;						//second
				g_EndingTheHalWayRecord[0][0].re.recordBox[i][RECORD_RANKING_ITEM_MILLISECOND] =
					g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti.millisecond;				//millisecond
				g_EndingTheHalWayRecord[0][0].re.recordBox[i][RECORD_RANKING_ITEM_COLOR_FOR_TIME] =
					g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].scoreColor;					//color
				break;
			case RECORD_HALF_WAY_RECORD_CHAIN:							//chain
				g_EndingTheHalWayRecord[0][0].re.recordBox[i][RECORD_RANKING_ITEM_CHAIN] =
					g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_CHAIN][i].num;
				g_EndingTheHalWayRecord[0][0].re.recordBox[i][RECORD_RANKING_ITEM_COLOR_FOR_CHAIN] =
					g_EndingTheHalWayRecord[RECORD_HALF_WAY_RECORD_CHAIN][i].scoreColor;						//color
				break;
			}
		}
	}
	//kind of replacing
	tagRECORDRANKINGRESULTITEM replaceTbl[RECORD_KIND_OF_RANKING_ITEM] = {
		RECORD_RANKING_ITEM_TOTAL_POINT,		//total
		RECORD_RANKING_ITEM_MINUTE,				//minute
		RECORD_RANKING_ITEM_SECOND,				//second
		RECORD_RANKING_ITEM_MILLISECOND,		//millisecond
		RECORD_RANKING_ITEM_CHAIN,				//chain
		RECORD_RANKING_ITEM_COLOR_FOR_TOTAL,	//each characters' identification
		RECORD_RANKING_ITEM_COLOR_FOR_TIME,		//each characters' identification
		RECORD_RANKING_ITEM_COLOR_FOR_CHAIN,	//each characters' identification
	};
	//kind of score relate replacing
	tagRECORDTHEHALFWAYITEM relateTbl[RECORD_KIND_OF_RANKING_ITEM] = {
		RECORD_HALF_WAY_RECORD_TOTAL_POINT,				//for total
		RECORD_HALF_WAY_RECORD_RACE_TIME,				//for minute
		RECORD_HALF_WAY_RECORD_RACE_TIME,				//for second
		RECORD_HALF_WAY_RECORD_RACE_TIME,				//for millisecond
		RECORD_HALF_WAY_RECORD_CHAIN,					//for chain
		RECORD_HALF_WAY_RECORD_TOTAL_POINT,				//for total ranking
		RECORD_HALF_WAY_RECORD_RACE_TIME,				//for time ranking
		RECORD_HALF_WAY_RECORD_CHAIN,					//for chain ranking
	};

	//process for replace value with each participants
	//for get competitor's value
	int value[RECORD_KIND_OF_RANKING_ITEM] = { 0 };
	//count participant
	int next = 0;
	//loop to kind of record
	for (int k = 0; k < RECORD_KIND_OF_RANKING_ITEM; k++) {
		for (int j = g_EndingPlayerBonusMaterial[relateTbl[k]]; j < RECORD_KIND_OF_PARTICIPANT; j++) {
			//player into competitor
			if (j == g_EndingPlayerBonusMaterial[relateTbl[k]]) {
				value[k] = g_EndingTheHalWayRecord[0][0].re.recordBox[j][k];		//get value
				g_EndingTheHalWayRecord[0][0].re.recordBox[j][k] =
					g_EndingTheHalWayRecord[0][0].re.recordBox[PARTICIPANT_PLAYER][k];
				g_EndingTheHalWayRecord[0][0].re.recordBox[PARTICIPANT_PLAYER][k] = value[k];
				next = PARTICIPANT_PLAYER;
				continue;
			}
			value[k] = g_EndingTheHalWayRecord[0][0].re.recordBox[j][k];		//get value
			//next is less than 10
			if (next < RECORD_KIND_OF_PARTICIPANT) {
				//player's score replace with competitor's position
				g_EndingTheHalWayRecord[0][0].re.recordBox[j][k] = g_EndingTheHalWayRecord[0][0].re.recordBox[next][k];
				g_EndingTheHalWayRecord[0][0].re.recordBox[next][k] = value[k];
			}
		}
	}
	//kind of file
	char	*lpFile[LEVEL_MAX] = {
		{ "Data\\EndTheResult\\Easy\\Ranking.dat" },			//easy
		{ "Data\\EndTheResult\\Normal\\Ranking.dat" },			//normal
		{ "Data\\EndTheResult\\Hard\\Ranking.dat" },			//hard
	};
	//each items
	int	itemTbl2[RECORD_KIND_OF_PARTICIPANT] = {				//all eight, these are total, time(minute, second and millisecond), chain and color
		RECORD_KIND_OF_RANKING_ITEM, RECORD_KIND_OF_RANKING_ITEM,
		RECORD_KIND_OF_RANKING_ITEM, RECORD_KIND_OF_RANKING_ITEM,
		RECORD_KIND_OF_RANKING_ITEM, RECORD_KIND_OF_RANKING_ITEM,
		RECORD_KIND_OF_RANKING_ITEM, RECORD_KIND_OF_RANKING_ITEM,
		RECORD_KIND_OF_RANKING_ITEM, RECORD_KIND_OF_RANKING_ITEM,
	};
	//save present record of the end ranking
	SaveRecord(lpFile[level], RECORD_KIND_OF_PARTICIPANT, itemTbl2, &g_EndingTheHalWayRecord[0][0].re);

}