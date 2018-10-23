
#include     "result.h"
#include	 "record.h"
#include     "map.h"
#include     "swimer.h"
#include     "input.h"
#include     "game.h"
#include     "wipe.h"
#include     "play.h"
#include     "seaenemy.h"
#include     "item.h"
#include     "sound.h"
#include	"CRunnerManager.h"
#include     "CPlayerRunner.h"
#include     "OffRoadRacer.h"
#include	 <stdio.h>
#include	 <string.h>
#include	 "ending.h"

//���O���[�o���ϐ�

//�摜�̗v�f�ԍ�������
int g_resultImg[RESULT_IMG_MAX] = {0}; 

//back ground image
CHARACTER g_ResultBg[2] = { 0 };
//�t�@�C���p
TALK g_resultFile[RESULT_FILE_MAX] = {0};

//���U���g�X�R�A������
SCOREVALUE g_resultScore[RECORD_MAX][RECORD_MAX] = {0};

//���U���g���ʕ\���ς݃t���O
bool	g_ResultIndicatedFlag = false;

//���U���g�ň����e���ڂ̐�
//�@�C�X�e�[�W�F�G�����A�l���A�C�e���A���ԁA�G����CHAIN���A�W�v�|�C���g�̏���
//�A���H�X�e�[�W�F�j�󂵂���Q���A��񂾃n�[�h���A���ԁA�n�[�h������CHAIN���A�W�v�|�C���g�A���ʂ̏���
//�B�I�t���[�h�X�e�[�W�F���������A�N�V�����A���v�A�N�V����Pt�A���ԁA�A���A�N�V�������A�W�v�|�C���g�̏���
char g_resultScoreItemTbl[STAGE_MAX][RECORD_MAX] = {
	{ 5 , 5 , 1 , 1 , 1 } ,			//�C
	{ 2 , 3 , 1 , 1 , 2 } ,			//���H
	{ 4 , 1 , 1 , 1 , 1 } ,			//�I�t���[�h
};

//RunnerManager class
CRunnerManager	g_CRunnerManagerForResult;
//�v���C���[�����i�[�N���X
CPlayerRunner	g_CPlayerRunnerForResult;
//���C�_�[�N���X
COffRoadRacer	g_ResultCOffRoadRacer;

//scene transition in result
tagRESULTTRANSITION g_ResultSceneNow = RESULT_SCENE_STAGE_RECORD;		//present viewing
tagRESULTTRANSITION g_ResultSceneNext = RESULT_SCENE_STAGE_RECORD;		//next view
//records of the half way
SCOREVALUE	g_ResultTheHalWayRecord[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT][RECORD_KIND_OF_PARTICIPANT] = { 0 };
//ranking for the half way result
SCOREVALUE	g_ResultTheHalfRanking[RECORD_KIND_OF_PARTICIPANT] = { 0 };
//drawing record's turn
tagRESULTRANKINGEACHTURN g_ResultTheHalfWayTurn = RESULT_RANKING_TURN_CHAIN;
//player's ranking
int g_ResultPlayerBonusMaterial[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = { 0 };
//competitor's ranking
int	g_ResultCompetitorBonusMaterial[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT][PARTICIPANT_PLAYER] = { 0 };
//record's title image
CHARACTER	g_ResultRecordCh[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = { 0 };

//for draw bonue point in the half way result
SCOREVALUE	g_ResultBonusScore[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = { 0 };
//for draw bonus' image
CHARACTER	g_ResultBonusCh[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = { 0 };

/*
************************************************************************************************

�t�@�C���̓ǂݍ���
************************************************************************************************
*/
void LoadResultFile(){

	//get current stage's number
	tagSELECTSTAGE stageNum = GetStage();
	//get the current language
	tagGAMELANGUAGE language = GetGameLanguage();
	//�� �e�X�e�[�W�̓ǂݍ��݃t�@�C���f�[�^�e�[�u��
	//for Japanese language
	char *lpJapaneseFileTbl[STAGE_MAX][RESULT_FILE_MAX] = {

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
	char *lpEnglishFileTbl[STAGE_MAX][RESULT_FILE_MAX] = {

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

	//�{���������X�e�[�W�̃t�@�C����������R�s�[
	for (int i = 0; i < RESULT_FILE_MAX; i++) {
		switch (language) {
		case GAME_LANGUAGE_JAPANESE:
			strcpy(&g_resultFile[i].pass[0], lpJapaneseFileTbl[stageNum][i]);
			break;
		case GAME_LANGUAGE_ENGLISH:
			strcpy(&g_resultFile[i].pass[0], lpEnglishFileTbl[stageNum][i]);
			break;
		}
	}
}


//************************************************************************************************
//������
//************************************************************************************************
void InitCommonResult( HWND hWnd ){


	//�L�^�X�V���擾
	RECORD* pre = GetRecord( RECORD_PREVIEW );

	//difficulty in playing
	tagGAMELEVEL	gameLevel = GetGameLevel();
	//bonus point according to difficulty
	int		bonusPointTbl[LEVEL_MAX] = { 0 , 7500 , 15000 };
	//get current stage's number
	tagSELECTSTAGE getStage = GetStage();

	//�� �X�e�[�W�������e�[�u��
	void ( *pInitStage[STAGE_MAX] )() = {

		InitSeaResult		,			//�C			
		InitRoadResult		,			//���H
		InitOffRoadResult	,			//�I�t���[�h
	};

	//�摜�e�[�u��
	char *pImagePass[STAGE_MAX] = {
		//�e�팋�ʉ��
		"Image\\Sea\\Result.bmp" ,			//�C
		"Image\\Road\\Result.bmp" ,			//���H
		"Image\\OffRoad\\Result.bmp" ,		//�I�t���[�h
	};
	//image
	char *lpImageTbl[RESULT_IMG_MAX] = {
		pImagePass[getStage],					//stage's image
		"Image\\EndTheResult.bmp",				//record's image
		"Image\\Bonus.bmp",						//bonus' image
		"Image\\EndResult.bmp",					//back ground image for the half way result
	};

	//���U���g���ʕ\���ς݃t���O
	g_ResultIndicatedFlag = false;

	//starting scene
	g_ResultSceneNow = RESULT_SCENE_STAGE_RECORD;		//present viewing
	g_ResultSceneNext = RESULT_SCENE_STAGE_RECORD;		//next view
	//drawing record's turn
	g_ResultTheHalfWayTurn = RESULT_RANKING_TURN_CHAIN;		//first turn is chain
	//reset each player's ranking of record
	for (int i = 0; i < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; i++) {
		g_ResultPlayerBonusMaterial[i] = 0;
	}
	//reset competitor's ranking
	for (int i = 0; i < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; i++) {
		for (int j = 0; j < PARTICIPANT_PLAYER; j++) {
			g_ResultCompetitorBonusMaterial[i][j] = 0;
		}
	}
	
	//�� �N���A�σX�e�[�W��ݒ肷��
	SetCompleteStage( SELECTSTAGE_STAGE_COMPLETE , getStage );

	//�X�R�A�̏�����
	InitScore( hWnd );

	//�L�^�̏�����
	InitRecord();

	//���@�摜�̓ǂݍ���
	for (int i = 0; i < RESULT_IMG_MAX; i++) {
		g_resultImg[i] = CreateDDImage(lpImageTbl[i], hWnd);
	}

	//initialize back ground image
	//each width
	int backWidth[RESULT_KIND_OF_BACK_IMAGE] = { 640,RESULT_BACK_IMAGE_THE_HALF_WAY_RESULT_WIDTH };
	//each height
	int backHeight[RESULT_KIND_OF_BACK_IMAGE] = { 480, RESULT_BACK_IMAGE_THE_HALF_WAY_RESULT_HEIGHT };
	for (int i = 0; i < RESULT_KIND_OF_BACK_IMAGE; i++) {
		g_ResultBg[i].h = backHeight[i];
		g_ResultBg[i].w = backWidth[i];
		g_ResultBg[i].flag = false;
		g_ResultBg[i].x = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(backWidth[i]);
		g_ResultBg[i].y = CHARACTER_CENTER_POSITION_Y_TO_STAGE_HEIGHT(backHeight[i]);
		g_ResultBg[i].picy = 0;
	}
	//first drawing is stage's back ground
	g_ResultBg[RESULT_BACK_IMAGE_STAGE_RESULT].flag = true;
	g_ResultBg[RESULT_BACK_IMAGE_THE_HALF_WAY_RESULT].y = RESULT_BACK_IMAGE_THE_HALF_WAY_RESULT_INDICATE_Y;	//change coordinateY for the half result
	
	//���@�Ȃ̓ǂݍ���
	CreateStreaming("BGM\\result.wav");

	//subsutitute score's direction and indication value
	for (int j = 0; j < RECORD_MAX; j++) {
		for (int i = 0; i < g_resultScoreItemTbl[getStage][j]; i++) {
			SetResultStageIndicationValue(&g_resultScore[j][i], j, i);
			//indication flag
			g_resultScore[j][i].ch.flag = true;
		}
	}
	
	//��Փx�ɉ������{�[�i�X�|�C���g��\�ߑ��
	// �L�^�p
	g_resultScore[0][0].re.recordBox[RECORD_TOTAL][0] = bonusPointTbl[gameLevel];
	// �\���p
	g_resultScore[RECORD_TOTAL][0].num = (float)bonusPointTbl[gameLevel];

	//�� load file of current stage
	LoadResultFile();
	//for fixed pattern file
	tagRESULTFIXEDFILE fixedFile[RESULT_KIND_OF_FIXED_PATTERN_FILE] = {
		RESULT_FIXED_PATTERN_FILE_1,			//file 1
		RESULT_FIXED_PATTERN_FILE_2,			//file 2
		RESULT_FIXED_PATTERN_FILE_3,			//file 3
	};
	//Set fixed pattern file
	for (int i = 2; i < (RESULT_KIND_OF_FIXED_PATTERN_FILE + 2); i++) {
		SetResultFixedPatternFile(&g_resultFile[i], fixedFile[i - 2]);
	}

	//�� �e�X�e�[�W�̏�����
	pInitStage[getStage]();

	//different setting by stage and record
	for (int i = 0; i < RECORD_MAX; i++) {
		DeferScoreOrFixedFileSetting(g_resultScore[i], &g_resultFile[0], getStage, i);
	}
	
	//*************************�L�^�̍X�V*************************************

	//�� ����̋L�^��ۑ�����
	SaveRecordForStage( &g_resultFile[RECORD_PREVIEW].pass[0] , &g_resultScore[0][0].re , RECORD_PREVIEW );

	//�� ����̋L�^�Ɖߋ��ɋL�^�����ō��̋L�^��ۑ������t�@�C�����J����
	//���ȃx�X�g�L�^���X�V����
	RenewalRecord( &g_resultFile[RECORD_BEST].pass[0] );

	//�e���ڂ̃X�R�A�̐F��ύX����
	for( int j = 0 ; j < RECORD_MAX ; j++ ){
		for( int i = 0 ; i < g_resultScoreItemTbl[getStage][j] ; i++ ){
			//�X�V�t���O�Ȃ�\���F��ύX����
			if( pre->renewalFlag[j][i] == RECORD_RENEWAL_FLAG ){
				//�\���p�ɍX�V�t���O���󂯎��
				g_resultScore[j][i].renealF = pre->renewalFlag[j][i];
			}
		}
	}

	//initialize the half result
	//record's images
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
		g_ResultRecordCh[i].x = 640.0f +
			((ENDING_END_THE_RESULT_IMAGE_WDIHT * scaleTbl[i]) * i);
		g_ResultRecordCh[i].ex = indicationXTbl[i];
		g_ResultRecordCh[i].y = indicationYTbl[i];
		g_ResultRecordCh[i].mx = ENDING_END_RESULT_IMAGE_MOVE_X - (i * 2.0f);
		g_ResultRecordCh[i].h = ENDING_END_THE_RESULT_IMAGE_HEIGHT;
		g_ResultRecordCh[i].w = ENDING_END_THE_RESULT_IMAGE_WDIHT;
		g_ResultRecordCh[i].scale = scaleTbl[i];
		g_ResultRecordCh[i].picy = ENDING_END_THE_RESULT_IMAGE_HEIGHT * (i + 1);
		g_ResultRecordCh[i].flag = false;
	}

	//subsutitute each record for drawing value and get competitors' records
	InitializeTheHalfWayResult();
	//subsutitute total point and time record in present stage for player's item
	//total point
	g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][PARTICIPANT_PLAYER].num = g_resultScore[RECORD_TOTAL][0].num;
	//time record
	g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][PARTICIPANT_PLAYER].ti.minutes = g_resultScore[RECORD_TIME][0].ti.minutes;
	g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][PARTICIPANT_PLAYER].ti.second = g_resultScore[RECORD_TIME][0].ti.second;
	g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][PARTICIPANT_PLAYER].ti.millisecond = g_resultScore[RECORD_TIME][0].ti.millisecond;
	//chain record
	g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_CHAIN][PARTICIPANT_PLAYER].num = g_resultScore[RECORD_FOUR][0].num;
	//signal color for player
	g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][PARTICIPANT_PLAYER].scoreColor = SCORE_YELOW;
	g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][PARTICIPANT_PLAYER].scoreColor = SCORE_YELOW;
	g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_CHAIN][PARTICIPANT_PLAYER].scoreColor = SCORE_YELOW;
	//decide player's ranking and get that value to time's record
	for (int i = 0; i < PARTICIPANT_PLAYER; i++) {
		//player's material of bonus point
		g_ResultPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_RACE_TIME] += CompairRaceTimeRecord(&g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][PARTICIPANT_PLAYER].ti,
			&g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti);
		//competitor
		//if player's material more than competitor's own material, competitor beats player
		if (g_ResultPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_RACE_TIME] > i) {
			g_ResultCompetitorBonusMaterial[RECORD_HALF_WAY_RECORD_RACE_TIME][i] = i;
		}
		else {
			//player beats competitor
			g_ResultCompetitorBonusMaterial[RECORD_HALF_WAY_RECORD_RACE_TIME][i] = i + 1;
		}
	}
	//decide player's ranking of chain
	for (int i = 0; i < PARTICIPANT_PLAYER; i++) {
		g_ResultPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_CHAIN] += 
			CompairRecord((int)g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_CHAIN][PARTICIPANT_PLAYER].num,
			(int)g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_CHAIN][i].num);
		//competitor
		//if player's material more than competitor's own material, competitor beats player
		if (g_ResultPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_CHAIN] > i) {
			g_ResultCompetitorBonusMaterial[RECORD_HALF_WAY_RECORD_CHAIN][i] = i;
		}
		else {
			//player beats competitor
			g_ResultCompetitorBonusMaterial[RECORD_HALF_WAY_RECORD_CHAIN][i] = i + 1;
		}
	}
	//initialize ranking image
	for (int i = 0; i < RECORD_KIND_OF_PARTICIPANT; i++) {
		g_ResultTheHalfRanking[i].score_x = RESULT_SCORE_THE_HALF_WAY_RESULT_RANKING_INDICATE_X;
		g_ResultTheHalfRanking[i].score_y = RESULT_SCORE_THE_HALF_WAY_RESULT_RANKING_INDICATE_Y + 
			(RESULT_SCORE_THE_HALF_WAY_RESULT_RANKING_INDICATE_SPACE_Y * i);
		g_ResultTheHalfRanking[i].score_scale = RESULT_SCORE_THE_HALF_WAY_RESULT_RANKING_SCALE_RATE;
		g_ResultTheHalfRanking[i].num = (i + 1);
		g_ResultTheHalfRanking[i].ch.flag = false;
		g_ResultTheHalfRanking[i].scoreColor = SCORE_RANING_COLOR_WHITE;
	}
	g_ResultTheHalfRanking[0].scoreColor = SCORE_RANING_COLOR_RED;				//change color, white into red

	//setting bonus, that score and image
	InitializeResultBonusForRanking(g_ResultBonusScore, g_ResultBonusCh);
}


/*
Initialize bonus, that score and image
*/
void	InitializeResultBonusForRanking(SCOREVALUE *lpScore, CHARACTER *lpCh) {
	
	//bonus' score setting
	//each coordinates
	FPOSITION posTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		{ (RESULT_SCORE_THE_HALF_WAY_RESULT_CHAIN_RECORD_INDICATE_X + 50.0f), RESULT_SCORE_THE_HALF_WAY_RESULT_BONUS_COMMON_INIDICATE_Y },	//chain
		{ RESULT_SCORE_THE_HALF_WAY_RESULT_TIME_RECORD_INDICATE_X, RESULT_SCORE_THE_HALF_WAY_RESULT_BONUS_COMMON_INIDICATE_Y },				//time
		{ RESULT_SCORE_THE_HALF_WAY_RESULT_TOTAL_POINT_INDICATE_X, RESULT_SCORE_THE_HALF_WAY_RESULT_BONUS_COMMON_INIDICATE_Y },				//total
	};
	//kind_of bonus record
	tagRECORDTHEHALFWAYITEM recordTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		RECORD_HALF_WAY_RECORD_CHAIN, RECORD_HALF_WAY_RECORD_RACE_TIME, RECORD_HALF_WAY_RECORD_TOTAL_POINT };
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
		(int)bonusTbl[g_ResultPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_CHAIN]],			//chain
		(int)bonusTbl[g_ResultPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_RACE_TIME]],		//time
		(int)(bonusTbl[g_ResultPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_CHAIN]] +
			bonusTbl[g_ResultPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_RACE_TIME]]),		//total
	};
	//initialize inidication settings
	for (int i = 0; i < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; i++) {
		SetResultTheHalfWayValue(&lpScore[i], RECORD_HALF_WAY_RECORD_TOTAL_POINT, 0, SCORE_GREEN);
	}
	//difference inidication settings
	//subsutitute each coordiinate for global variable
	//loop to kind
	for (int i = 0; i < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; i++) {
		lpScore[recordTbl[i]].score_x = posTbl[i].fX;													//coordinateX
		lpScore[recordTbl[i]].score_y = posTbl[i].fY;													//coordinateY
		lpScore[recordTbl[i]].score_scale = RESULT_SCORE_THE_HALF_WAY_RESULT_BONUS_COMMON_SCALE_RATE;	//scale rate
		lpScore[recordTbl[i]].digit = RESULT_SCORE_THE_HALF_WAY_RESULT_BONUS_COMMON_RECORD_DIGIT;		//digit
		lpScore[recordTbl[i]].num = bonusPointTbl2[i];													//subsutitute bonus point for each bonus' scores
		lpScore[recordTbl[i]].ch.flag = false;															//drawing flag
	}

	//setting bonus' image
	//each scale rate
	float	scaleTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		RESULT_IMAGE_THE_HALF_WAY_RESULT_BONUS_SCALE_RATE,				//chain
		RESULT_IMAGE_THE_HALF_WAY_RESULT_BONUS_SCALE_RATE,				//time
		RESULT_IMAGE_THE_HALF_WAY_RESULT_TOTAL_BONUS_SCALE_RATE };		//total
	//origin coordinateY
	float	picYTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		0, 0, RESULT_IMAGE_THE_HALF_WAY_RESULT_BONUS_COMMON_HEIGHT,
	};
	//coordinateX
	float	PosXTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		RESULT_IMAGE_THE_HALF_WAY_RESULT_CHAIN_RECORD_TITLE_INDICATE_X,		//chain
		RESULT_IMAGE_THE_HALF_WAY_RESULT_TIME_RECORD_TITLE_INDICATE_X,		//time
		RESULT_IMAGE_THE_HALF_WAY_RESULT_TOTAL_POINT_TITLE_INDICATE_X,		//total
	};
	//subsutitute each default value for global variable
	for (int i = 0; i < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; i++) {
		lpCh[recordTbl[i]].w = RESULT_IMAGE_THE_HALF_WAY_RESULT_BONUS_COMMON_WIDTH;
		lpCh[recordTbl[i]].h = RESULT_IMAGE_THE_HALF_WAY_RESULT_BONUS_COMMON_HEIGHT;
		lpCh[recordTbl[i]].scale = scaleTbl[i];
		lpCh[recordTbl[i]].picy = picYTbl[i];
		lpCh[recordTbl[i]].flag = false;
		lpCh[recordTbl[i]].x = PosXTbl[i];
		lpCh[recordTbl[i]].y = RESULT_IMAGE_THE_HALF_WAY_RESULT_BONUS_COMMON_INIDICATE_Y;
	}
}

//************************************************************************************************
//�C�X�e�[�W�̏�����
//************************************************************************************************
void InitSeaResult(){

	char mes[256] = {0};	//�����������
	int time = 0;			//���Ԃ̍��v����|�C���g���Z�o
	int defeat = 0;			//�G�����|�C���g
	int cnt = 0;			//�v�f�J�E���g�p

	//�G�̏����擾
	ENEMY* pen = GetEnemy();
	//�N���A���Ԃ��擾
	TIME* pt = GetStageCompleteTime();
	//�A�C�e���̏����擾
	ITEM* pi = GetItem();
	//�v���C���[�̏����擾
	SWIMER* pch = GetSwimer();

	//���@�e���ڂ̃|�C���g���v�Z
	
	//�N���A���Ԃ̃|�C���g���v�Z
	time = 20000 - MINUTES_INTO_MILLISECOND( pt->minutes ) +	//��
		SECONDS_INTO_MILLISECOND( pt->second ) +				//�b
		pt->millisecond;										//�~���b


	//�G���j������|�C���g���Z�o
	for( int i = 0 ; i < KIND_ENEMY_SEA ; i++ ){
		defeat += ( pen->defeat[i] * 25 ) + ( (i * 10 ) * pen->defeat[i] );
	}

	//�� �e�퍀�ڂ̒l���擾����

	//******************�C�X�e�[�W************************

	//�@ �e��G�̓��������󂯎��
	//�� �L�^�p
	g_resultScore[0][0].re.recordBox[RECORD_ONE][0] = pen->defeat[JELLYFISH];			//�N���Q
	g_resultScore[0][0].re.recordBox[RECORD_ONE][1] = pen->defeat[SHARK];				//�T��
	g_resultScore[0][0].re.recordBox[RECORD_ONE][2] = pen->defeat[SHOAL];				//���Q
	g_resultScore[0][0].re.recordBox[RECORD_ONE][3] = pen->defeat[SUNFISH];			//�}���{�[
	g_resultScore[0][0].re.recordBox[RECORD_ONE][4] = pen->defeat[RAY];				//�G�C
	//�� �\���p
	for( int i = 0 ; i < g_resultScoreItemTbl[STAGE_SEA][RECORD_ONE] ; i++ ){
		g_resultScore[RECORD_ONE][i].num = (float)pen->defeat[i];
	}


	//�A �e��A�C�e���̊l�������󂯎��
	g_resultScore[0][0].re.recordBox[RECORD_TWO][0] = pi->get[ITEM_TYPE_SPEED_UP];		//���x�㏸
	g_resultScore[0][0].re.recordBox[RECORD_TWO][1] = pi->get[ITEM_TYPE_ABSOLUTE];		//���G
	g_resultScore[0][0].re.recordBox[RECORD_TWO][2] = pi->get[ITEM_TYPE_BIG];			//�g��
	g_resultScore[0][0].re.recordBox[RECORD_TWO][3] = pi->get[ITEM_TYPE_SPEED_DOWN];	//���x����
	g_resultScore[0][0].re.recordBox[RECORD_TWO][4] = pi->get[ITEM_TYPE_SMALL];		//�k��
	//�� �\���p
	for( int i = 0 ; i <= ITEM_TYPE_SMALL ; i++ ){
	
		g_resultScore[RECORD_TWO][cnt].num = (float)pi->get[i];

		//�v�f���J�E���g
		cnt++;
		
		//�����A�C�e���擾��A�㏬�A�C�e���̗v�f�ɍ��킹��
		if( i == ITEM_TYPE_BIG ) i = ( ITEM_TYPE_SPEED_DOWN - 1 );
	}

	//�B �N���A���Ԃ��󂯎��
	//�� �L�^�p
	//��
	g_resultScore[0][0].re.recordBox[RECORD_TIME][0] = pt->minutes;
	//�b
	g_resultScore[0][0].re.recordBox[RECORD_TIME][1] = pt->second;
	//�~���b
	g_resultScore[0][0].re.recordBox[RECORD_TIME][2] = pt->millisecond;
	//�� �\���p
	g_resultScore[RECORD_TIME][0].ti.minutes = pt->minutes;			//��
	g_resultScore[RECORD_TIME][0].ti.second = pt->second;			//�b
	g_resultScore[RECORD_TIME][0].ti.millisecond = pt->millisecond;		//�~���b
	

	//�C CHAIN�����󂯎��
	g_resultScore[0][0].re.recordBox[RECORD_FOUR][0] = pch->chain_max;
	//�� �\���p
	g_resultScore[RECORD_FOUR][0].num = (float)pch->chain_max;
	

	//�D �W�v�|�C���g
	g_resultScore[0][0].re.recordBox[RECORD_TOTAL][0] += time + defeat + ( pch->chain_max * 200 );
	//�� �\���p
	g_resultScore[RECORD_TOTAL][0].num += (float)( time + defeat + ( pch->chain_max * 200 ) );

}


/*
***************************************************************************************************
���[�h�X�e�[�W�p�̏�����
***************************************************************************************************
*/
void InitRoadResult(){

	//���ԏ����擾
	TIME* pt = GetStageCompleteTime();
	//�����i�[�����擾
	RUNNER* pPrunner = g_CPlayerRunnerForResult.GetRunner();
	//Get player's ranking in this race
	int	ranking = g_CRunnerManagerForResult.GetPlayerRunnerRanking();

	//�|�C���g�v�Z�p
	int time = 0;					//���Ԃ̍��v����|�C���g���Z�o
	int defeat = 0;					//�j��y�i���e�B

	//���@�e���ڂ̃|�C���g���v�Z
	
	//�N���A���Ԃ̃|�C���g���v�Z
	time = 20000 - MINUTES_INTO_MILLISECOND( pt->minutes ) +	//��
		SECONDS_INTO_MILLISECOND( pt->second ) +				//�b
		pt->millisecond;										//�~���b

	//�@ �j�󂵂���
	for( int i = 0 ; i < g_resultScoreItemTbl[STAGE_ROAD][RECORD_DFEAT_OBSTACLES] ; i++ ){
		//�L�^�p
		g_resultScore[0][0].re.recordBox[0][i] = pPrunner->defeatCnt[i];
		//�\���p
		g_resultScore[0][i].num = (float)pPrunner->defeatCnt[i];
		//�|�C���g�v�Z�p�ɑ��
		defeat -= pPrunner->defeatCnt[i] * 100;
	}

	//�A ��щz�����n�[�h���̐�
	for( int i = 0 ; i < g_resultScoreItemTbl[STAGE_ROAD][RECORD_JUMPED_HURDLE] ; i++ ){
		//�L�^�p
		g_resultScore[0][0].re.recordBox[1][i] = pPrunner->jumpedHurdleCnt[i];
		//�\���p
		g_resultScore[1][i].num = (float)pPrunner->jumpedHurdleCnt[i];
	}

	//�B �N���A���Ԃ��󂯎��
	//�� �L�^�p
	//��
	g_resultScore[0][0].re.recordBox[RECORD_TIME][0] = pt->minutes;
	//�b
	g_resultScore[0][0].re.recordBox[RECORD_TIME][1] = pt->second;
	//�~���b
	g_resultScore[0][0].re.recordBox[RECORD_TIME][2] = pt->millisecond;
	//�� �\���p
	g_resultScore[RECORD_TIME][0].ti.minutes = pt->minutes;				//��
	g_resultScore[RECORD_TIME][0].ti.second = pt->second;				//�b
	g_resultScore[RECORD_TIME][0].ti.millisecond = pt->millisecond;		//�~���b

	//�C �n�[�h����A���Ŕ�щz�����ő吔
	//�L�^�p
	g_resultScore[0][0].re.recordBox[RECORD_FOUR][0] = pPrunner->jumpedHurdleChainMax;
	//�\���p
	g_resultScore[RECORD_FOUR][0].num = (float)pPrunner->jumpedHurdleChainMax;

	//ranking
	//for record
	g_resultScore[0][0].re.recordBox[RECORD_TOTAL][1] = ranking;					//point in this race
	//for drawing
	g_resultScore[RECORD_TOTAL][1].num = ranking;
	
	//�D �W�v�|�C���g
	//�L�^�p
	g_resultScore[0][0].re.recordBox[RECORD_TOTAL][0] += time + defeat + ( pPrunner->jumpedHurdleChainMax * 200 ) + 
		(5100 - (ranking * 100));
	//�\���p
	g_resultScore[RECORD_TOTAL][0].num += (float)( time + defeat + ( pPrunner->jumpedHurdleChainMax * 200 ) ) + 
		(5100 - (ranking * 100));

}


/*
***************************************************************************************************
�I�t���[�h�p�̏�����
***************************************************************************************************
*/
void InitOffRoadResult(){

	//���ԏ����擾
	TIME* pt = GetStageCompleteTime();
	//���C�_�[�̏����擾
	int		totalActionPt = g_ResultCOffRoadRacer.GetTotalActionPoint();					//���v�A�N�V����P��
	int		*lpSuccessAction = g_ResultCOffRoadRacer.GetSuccessActionCount();				//���������A�N�V������
	int		succeedActionConsecutive = g_ResultCOffRoadRacer.GetScceedActionConsecutive();	//�A���A�N�V����������
	
	//get present language
	tagGAMELANGUAGE language = GetGameLanguage();
	
	//�|�C���g�v�Z�p
	int time = 0;					//���Ԃ̍��v����|�C���g���Z�o
	int action = 0;					//�A�N�V�����������ɉ������{�[�i�X

	//���@�e���ڂ̃|�C���g���v�Z
	
	//�N���A���Ԃ̃|�C���g���v�Z
	time = 20000 - MINUTES_INTO_MILLISECOND( pt->minutes ) +	//��
		SECONDS_INTO_MILLISECOND( pt->second ) +				//�b
		pt->millisecond;										//�~���b

	//�@ �����A�N�V������
	for( int i = 0 ; i < g_resultScoreItemTbl[STAGE_OFF_ROAD][0] ; i++ ){

		//�L�^�p
		g_resultScore[RECORD_ONE][0].re.recordBox[0][i] = *( lpSuccessAction + i );
		//�\���p
		g_resultScore[RECORD_ONE][i].num = (float)*(lpSuccessAction + i);
		//�|�C���g�v�Z�p�ɑ��
		action += lpSuccessAction[i] * 100;
	}

	//�A���v�A�N�V�����|�C���g
	//�L�^�p
	g_resultScore[0][0].re.recordBox[1][0] = totalActionPt;
	//�\���p
	g_resultScore[RECORD_TWO][0].num = (float)totalActionPt;
	//������ύX
	g_resultScore[RECORD_TWO][0].digit = RESULT_SCORE_FOURTH_DIGIT_IN_OFF_ROAD;
	//change indicationX
	g_resultScore[RECORD_TWO][0].score_x = RESULT_SCORE_SECOND_INDICATE_X_IN_OFF_ROAD;

	//�B �N���A���Ԃ��󂯎��
	//�� �L�^�p
	//��
	g_resultScore[0][0].re.recordBox[RECORD_TIME][0] = pt->minutes;
	//�b
	g_resultScore[0][0].re.recordBox[RECORD_TIME][1] = pt->second;
	//�~���b
	g_resultScore[0][0].re.recordBox[RECORD_TIME][2] = pt->millisecond;
	//�� �\���p
	g_resultScore[RECORD_TIME][0].ti.minutes = pt->minutes;			//��
	g_resultScore[RECORD_TIME][0].ti.second = pt->second;			//�b
	g_resultScore[RECORD_TIME][0].ti.millisecond = pt->millisecond;		//�~���b

	//�C �A���A�N�V����������
	//�L�^�p
	g_resultScore[0][0].re.recordBox[RECORD_FOUR][0] = succeedActionConsecutive;
	//�\���p
	g_resultScore[RECORD_FOUR][0].num = (float)succeedActionConsecutive;

	//�D �W�v�|�C���g
	//�L�^�p
	g_resultScore[0][0].re.recordBox[RECORD_TOTAL][0] += time + action + (succeedActionConsecutive * 200 ) + totalActionPt;
	//�\���p
	g_resultScore[RECORD_TOTAL][0].num += (float)( time + action + (succeedActionConsecutive * 200 ) ) + totalActionPt;

}



//************************************************************************************************
//�X�V
//************************************************************************************************
void UpdateResult() {

	//�Ȃ̍Đ�
	PlayStreaming(true);

	//The result diverge scene flag
	switch (g_ResultSceneNow) {
	case RESULT_SCENE_STAGE_RECORD:
		PresentStageResult();							//result of this race
		break;
	case RESULT_SCENE_THE_HALF_WAY_RECORD:
		//when subsutituted next scene flag for g_ResultTheHalfWayTurn
		if (g_ResultTheHalfWayTurn == RESULT_RANKING_TUNR_NEXT_SCENE) {
			//subsutitute scene flag
			g_ResultSceneNext = RESULT_SCENE_NEXT;				//the end result
		}
		else {//Update each score, these are chain, time and total
			UpdateTheHalfWayResult();													//Update the half way result
		}
		break;
	case RESULT_SCENE_NEXT:																//transition scene
		if (g_ResultBonusScore[RECORD_HALF_WAY_RECORD_TOTAL_POINT].directEnd) { 		//change scene
			if (JustKey('Z') || JustButton(BUTTON_1)) {
				CreateWipe(SCENE_SELECTSTAGE, CHANGE_SCENE, WIPE_CONTRACTION);
			}
		}
		break;
	}
	//record's titles move
	for (int i = 0; i < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; i++) {
		if (g_ResultRecordCh[i].flag) {
			//move to indication X of terminate
			if (g_ResultRecordCh[i].x > g_ResultRecordCh[i].ex) g_ResultRecordCh[i].x += g_ResultRecordCh[i].mx;		//move
			if (g_ResultRecordCh[i].x <= g_ResultRecordCh[i].ex) g_ResultRecordCh[i].x = g_ResultRecordCh[i].ex;		//terminate
		}
	}
	//blank time to change scene
	if (g_ResultSceneNow != g_ResultSceneNext) {
		//count time
		g_ResultBg[RESULT_BACK_IMAGE_STAGE_RESULT].time++;
		//if count time more than 20 fram, subsutitute next scene for present scene
		if (g_ResultBg[RESULT_BACK_IMAGE_STAGE_RESULT].time >= RESULT_SCENE_BLANK_TIME) {
			//subsutitute scene for present flag
			g_ResultSceneNow = g_ResultSceneNext;
			//reset count time
			g_ResultBg[RESULT_BACK_IMAGE_STAGE_RESULT].time = 0;
		}
	}
}


//************************************************************************************************
//�\��
//************************************************************************************************
void DrawResult(){

	//���݂̃X�e�[�W�ԍ����擾
	int getStage = GetStage();

	//�X�R�A�\���֐��̕���
	void (*pscore[RECORD_MAX])(SCOREVALUE*) = {

		DrawScore	,			//�G�\��
		DrawScore	,			//�A�C�e���\��
		DrawTime	,			//���ԕ\��
		DrawScore	,			//CHAIN��
		DrawScore	,			//�W�v�|�C���g
	};

	//draw back ground image
	int image[RESULT_KIND_OF_BACK_IMAGE] = { g_resultImg[0],g_resultImg[3] };
	for (int i = 0; i < RESULT_KIND_OF_BACK_IMAGE; i++) {
		if (g_ResultBg[i].flag) {			
			DDBlt(
				g_ResultBg[i].x,
				g_ResultBg[i].y,
				g_ResultBg[i].w,
				g_ResultBg[i].h,
				image[i],
				g_ResultBg[i].picx,
				g_ResultBg[i].picy);
		}
	}

	//���e���ڂ̒l��\��
	for (int j = 0; j < RECORD_MAX; j++) {
		for (int i = 0; i < g_resultScoreItemTbl[getStage][j]; i++) {
			if (g_resultScore[j][i].ch.flag) {
				//if draw ranking
				if (getStage == STAGE_ROAD && j == RECORD_TOTAL && i == 1) {
					//for ranking drawing
					DrawRanking(g_resultScore[RECORD_TOTAL][1].score_x,
						g_resultScore[RECORD_TOTAL][1].score_y,
						g_resultScore[RECORD_TOTAL][1].score_scale,
						g_resultScore[RECORD_TOTAL][1].num,
						(tagSCORERANKINGCOLOR)g_resultScore[RECORD_TOTAL][1].scoreColor);
					break;
				}
				//�X�R�A�\���̕���
				pscore[j](&g_resultScore[j][i]);
			}
		}
	}
	
	//kind of item for the half way result
	tagRECORDMATRIX halfRecordTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = { RECORD_TOTAL, RECORD_TIME };
	//draw the half way result
	for (int j = 0; j < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; j++) {
		for (int i = 0; i < RECORD_KIND_OF_PARTICIPANT; i++) {
			if (g_ResultTheHalWayRecord[j][i].ch.flag) {
				//draw score of the half way result
				pscore[halfRecordTbl[j]](&g_ResultTheHalWayRecord[j][i]);
			}
		}
	}
	//draw ranking for the half way result
	for (int i = 0; i < RECORD_KIND_OF_PARTICIPANT; i++) {
		if (g_ResultTheHalfRanking[i].ch.flag) {
			DrawRanking(g_ResultTheHalfRanking[i].score_x,
				g_ResultTheHalfRanking[i].score_y,
				g_ResultTheHalfRanking[i].score_scale,
				g_ResultTheHalfRanking[i].num,
				(tagSCORERANKINGCOLOR)g_ResultTheHalfRanking[i].scoreColor);
		}
	}
	//draw title of the half way record
	for (int i = 0; i < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT;i++){
		if (g_ResultRecordCh[i].flag) {
			//indication way is scaleBlt
			DDScaleBltSelectIndicate(
				g_ResultRecordCh[i].x,
				g_ResultRecordCh[i].y,
				g_ResultRecordCh[i].w,
				g_ResultRecordCh[i].h,
				g_resultImg[1],
				g_ResultRecordCh[i].picx,
				g_ResultRecordCh[i].picy,
				g_ResultRecordCh[i].scale,
				DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP);
		}
	}

	//draw bonus score in the half way result
	for (int i = 0; i < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; i++) {
		if (g_ResultBonusScore[i].ch.flag) {				//score
			DrawScore(&g_ResultBonusScore[i]);
		}
		if (g_ResultBonusCh[i].flag) {						//image
			//indication way is scaleBlt
			DDScaleBltSelectIndicate(
				g_ResultBonusCh[i].x,
				g_ResultBonusCh[i].y,
				g_ResultBonusCh[i].w,
				g_ResultBonusCh[i].h,
				g_resultImg[2],
				g_ResultBonusCh[i].picx,
				g_ResultBonusCh[i].picy,
				g_ResultBonusCh[i].scale,
				DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP);
		}
	}
}

//************************************************************************************************
//���
//************************************************************************************************
void ReleaseResult(){
	
	//���݂̃X�e�[�W�ԍ����擾
	tagSELECTSTAGE getStage = GetStage();

	//�摜�̉��
	for (int i = 0; i < RESULT_IMG_MAX; i++) { ReleaseDDImage(g_resultImg[i]); }
	//*************************�L�^�̍X�V*************************************

	//�� ����̋L�^��ۑ�����
	SaveRecordForStage(&g_resultFile[RECORD_PREVIEW].pass[0], &g_resultScore[0][0].re, RECORD_PREVIEW);
	//�� ����̋L�^�Ɖߋ��ɋL�^�����ō��̋L�^��ۑ������t�@�C�����J����
	//���ȃx�X�g�L�^���X�V����
	RenewalRecord(&g_resultFile[RECORD_BEST].pass[0]);

	//�Ȃ̉��
	ReleaseStreaming();
	
	//�t�@�C���̉��
	for( int i = 0 ; i < RESULT_FILE_MAX ; i++ ){
		//�O�ŏ�����
		ZeroMemory( &g_resultFile[i] , sizeof( TALK ) );
	}
	//���U���g���ʂ̃��Z�b�g
	for( int j = 0 ; j < RECORD_MAX ; j++ ){
		for( int i = 0 ; i < g_resultScoreItemTbl[getStage][j] ; i++ ){
			ZeroMemory( &g_resultScore[j][i] , sizeof( SCOREVALUE ) );	
		}
	}
	//reset the half way result
	for (int j = 0; j < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; j++) {
		for (int i = 0; i < RECORD_KIND_OF_PARTICIPANT; i++) {
			ZeroMemory(&g_ResultTheHalWayRecord[j][i], sizeof(SCOREVALUE));
		}
	}
	//bonus score
	ZeroMemory(g_ResultBonusScore, sizeof(SCOREVALUE) * RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT);

	//Talk�̉��
	ReleaseTalk();
	//�X�R�A�̉��
	ReleaseScore();
}

/*
Set score direction and indication value for three stage's score
*/
void	SetResultStageIndicationValue(SCOREVALUE *lpScore, int recordNum, int material) {


	//set indication position
	FPOSITION posTbl[RECORD_MAX] = {
		{RESULT_SCORE_FIRST_INDICATE_X,RESULT_SCORE_FIRST_INDICATE_Y },				//first item
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
		SCORE_GRADUALLY_DIRECT_ROLLING ,				//first
		SCORE_GRADUALLY_DIRECT_ROLLING ,				//second
		SCORE_GRADUALLY_DIRECT_ROLLING ,				//third
		SCORE_GRADUALLY_DIRECT_ROLLING ,				//fourth
		SCORE_GRADUALLY_DIRECT_NORMAL  ,				//fifth
	};
	//score's direction type, which is gradually direction
	//interval time of indication
	int intervalTbl[RECORD_MAX] = { 0,0,0,0,RESULT_SCORE_FIFTH_DIRECTION_GRADUALLY_DEFAULT_INTERVAL_TIME };
	//score's direction type, which is rolling direction
	//rolling time
	int rollingTimeTbl[RECORD_MAX] = {
		RESULT_SCORE_FIRST_DIRECTION_ROLLING_DEFAULT_TIME,	//first
		RESULT_SCORE_SECOND_DIRECTION_ROLLING_DEFAULT_TIME,	//second
		RESULT_SCORE_THIRD_DIRECTION_ROLLING_DEFAULT_TIME,	//third
		RESULT_SCORE_FOURTH_DIRECTION_ROLLING_DEFAULT_TIME,	//fourth
		0,													//fifth
	};
	//set default digit
	int digitTbl[RECORD_MAX] = {
		RESULT_SCORE_FIRST_DIGIT,						//first
		RESULT_SCORE_SECOND_DIGIT,						//second
		RESULT_SCORE_THIRD_DIGIT,						//third
		RESULT_SCORE_FOURTH_DIGIT,						//fourth
		RESULT_SCORE_FIFTH_DIGIT,						//fifth
	};

	//subsutitute default value
	lpScore->spaceX = spaceTbl[recordNum].fX;
	lpScore->spaceY = spaceTbl[recordNum].fY;
	lpScore->score_x = posTbl[recordNum].fX;							
	lpScore->score_y = posTbl[recordNum].fY + ( lpScore->spaceY * material);						
	lpScore->score_scale = scaleTbl[recordNum];					
	lpScore->ch.type = SCORE_NUMBER_TYPE_NORMAL;		
	lpScore->scoreColor = RESULT_SCORE_COLOR_COMMON_USUAL;					
	lpScore->digit = digitTbl[recordNum];									
	lpScore->gradually = true;							
	lpScore->graduallyDirect = directionTbl[recordNum];
	lpScore->interval = intervalTbl[recordNum];
	lpScore->rollTime = rollingTimeTbl[recordNum] + (material * 10);
	lpScore->ch.time = 0;
	lpScore->renewalColor = RESULT_SCORE_COLOR_COMMON_RENEWAL;
	lpScore->numD = 0;
}

/*
Set file for the current language
*/
void	SetResultFixedPatternFile(TALK *lpTalk, tagRESULTFIXEDFILE fileNum) {
	
	//get current language
	tagGAMELANGUAGE language = GetGameLanguage();

	//each other default value
	//position X
	float posXTbl[RESULT_KIND_OF_FIXED_PATTERN_FILE][GAME_KIND_OF_LANGUAGE] = {
		{RESULT_INITIALIZE_FILE_1_INDICATE_X_IN_JAPANESE,RESULT_INITIALIZE_FILE_1_INDICATE_X_IN_ENGLISH},		//File 1
		{ RESULT_INITIALIZE_FILE_2_INDICATE_X_IN_JAPANESE,RESULT_INITIALIZE_FILE_2_INDICATE_X_IN_ENGLISH },		//File 2
		{ RESULT_INITIALIZE_FILE_3_INDICATE_X_IN_JAPANESE,RESULT_INITIALIZE_FILE_3_INDICATE_X_IN_ENGLISH },		//File 3
	};
	//position Y
	float posYTbl[RESULT_KIND_OF_FIXED_PATTERN_FILE][GAME_KIND_OF_LANGUAGE] = {
		{ RESULT_INITIALIZE_FILE_1_INDICATE_Y_IN_JAPANESE,RESULT_INITIALIZE_FILE_1_INDICATE_Y_IN_ENGLISH },		//File 1
		{ RESULT_INITIALIZE_FILE_2_INDICATE_Y_IN_JAPANESE,RESULT_INITIALIZE_FILE_2_INDICATE_Y_IN_ENGLISH },		//File 2
		{ RESULT_INITIALIZE_FILE_3_INDICATE_Y_IN_JAPANESE,RESULT_INITIALIZE_FILE_3_INDICATE_Y_IN_ENGLISH },		//File 3
	};
	//Font size
	int fontTbl[RESULT_KIND_OF_FIXED_PATTERN_FILE][GAME_KIND_OF_LANGUAGE] = {
		{RESULT_INITIALIZE_FILE_1_FONT_SIZE_IN_JAPANESE,RESULT_INITIALIZE_FILE_1_FONT_SIZE_IN_ENGLISH},			//File 1
		{ RESULT_INITIALIZE_FILE_2_FONT_SIZE_IN_JAPANESE,RESULT_INITIALIZE_FILE_2_FONT_SIZE_IN_ENGLISH },		//File 2
		{ RESULT_INITIALIZE_FILE_3_FONT_SIZE_IN_JAPANESE,RESULT_INITIALIZE_FILE_3_FONT_SIZE_IN_ENGLISH },		//File 3
	};
	//column space
	int	spaceYTbl[RESULT_KIND_OF_FIXED_PATTERN_FILE][GAME_KIND_OF_LANGUAGE] = {
		{ RESULT_INITIALIZE_FILE_1_INDICATE_SPACE_Y_IN_JAPANESE,RESULT_INITIALIZE_FILE_1_INDICATE_SPACE_Y_IN_ENGLISH },		//File 1
		{ RESULT_INITIALIZE_FILE_2_INDICATE_SPACE_Y_IN_JAPANESE,RESULT_INITIALIZE_FILE_2_INDICATE_SPACE_Y_IN_ENGLISH },		//File 2
		{ RESULT_INITIALIZE_FILE_3_INDICATE_SPACE_Y_IN_JAPANESE,RESULT_INITIALIZE_FILE_3_INDICATE_SPACE_Y_IN_ENGLISH },		//File 3
	};

	//subsutitute each other's value for global variable
	lpTalk->baseX = posXTbl[fileNum][language];			//�\���w���W
	lpTalk->baseY = posYTbl[fileNum][language];			//�\���x���W
	lpTalk->next = 1;									//��������̊Ԋu
	lpTalk->size = fontTbl[fileNum][language];			//�����̃T�C�Y
	lpTalk->line_space = spaceYTbl[fileNum][language];	//�s�̊Ԋu
}


/*
difference setting for drawing score value or fixed pattern file
*/
void	DeferScoreOrFixedFileSetting(SCOREVALUE *lpScore, TALK *lpTalk, tagSELECTSTAGE stageNum,int recordNum) {

	//get present language
	tagGAMELANGUAGE language = GetGameLanguage();

	switch (stageNum) {
	case STAGE_SEA:
		break;
	case STAGE_ROAD:
		//if current stage is road-stage
		if (recordNum == RECORD_TOTAL) {													//fifth item
			lpScore[0].score_x = RESULT_SCORE_FIFTH_INDICATE_X_IN_ROAD;						//total point
			//subsutitute default value for player's ranking
			lpScore[1].score_x = RESULT_SCORE_FIFTH_PLAYER_RANKING_INDICATE_X;				//ranking
			lpScore[1].score_y = RESULT_SCORE_FIFTH_PLAYER_RANKING_INDICATE_Y;
			lpScore[1].score_scale = RESULT_SCORE_FIFTH_PLAYER_RANKING_DEFAULT_SCALE;
			lpScore[1].ch.type = SCORE_NUMBER_TYPE_NORMAL;
			lpScore[1].scoreColor = SCORE_RANING_COLOR_WHITE;
			lpScore[1].digit = RESULT_SCORE_FIFTH_PLAYER_RANKING_DIGIT;
			lpScore[1].gradually = true;
			lpScore[1].graduallyDirect = SCORE_GRADUALLY_DIRECT_ROLLING;
			lpScore[1].interval = 0;
			lpScore[1].rollTime = RESULT_SCORE_FIFTH_PLAYER_RANKING_DIRECTION_ROLLING_DEFAULT_TIME;
			lpScore[1].renewalColor = RESULT_SCORE_COLOR_COMMON_RENEWAL;
			lpScore[1].numD = 0;
		}
		break;
	case STAGE_OFF_ROAD:
		if (recordNum == RECORD_ONE) {																//first item
			//change indication of fixed pattern file
			if (language == GAME_LANGUAGE_ENGLISH) {
				lpTalk[4].baseX = RESULT_INITIALIZE_FILE_3_INDICATE_X_IN_ENGLISH_AND_OFF_ROAD;
				//subsutitute default value for each technical actions' indication X
				for (int i = 0; i < 4; i++) {
					lpScore[i].score_x = RESULT_SCORE_FIRST_INDICATE_X_IN_OFF_ROAD_AND_ENGLISH;
				}
			}
		}
		//if stage is off=road, change digit
		//subsutitute default positionX for technical point's indication X
		if (recordNum == RECORD_TWO) {																//second item
			lpScore[0].score_x = RESULT_SCORE_SECOND_INDICATE_X_IN_OFF_ROAD;
			//change digit
			lpScore[0].digit = RESULT_SCORE_FOURTH_DIGIT_IN_OFF_ROAD;
		}
		break;
	}
}

/*
Initialize the half way result
*/
void InitializeTheHalfWayResult() {

	//kind of score for the half way result
	tagRECORDTHEHALFWAYITEM recordNumTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		RECORD_HALF_WAY_RECORD_TOTAL_POINT,				//aggregate point
		RECORD_HALF_WAY_RECORD_RACE_TIME,				//time of race
		RECORD_HALF_WAY_RECORD_CHAIN,					//consecutive something
	};

	//get present game's level
	tagGAMELEVEL gamelevel = GetGameLevel();
	//kind of file
	char *lpFile[LEVEL_MAX][STAGE_MAX] = {
		{ "Data\\SeaStage\\Easy\\TheHalfWay.dat",
		"Data\\RoadStage\\Easy\\TheHalfWay.dat",
		"Data\\OffRoadStage\\Easy\\TheHalfWay.dat" },		//Easy

		{ "Data\\SeaStage\\Normal\\TheHalfWay.dat",
		"Data\\RoadStage\\Normal\\TheHalfWay.dat",
		"Data\\OffRoadStage\\Normal\\TheHalfWay.dat" },		//Normal
		
		{ "Data\\SeaStage\\Hard\\TheHalfWay.dat",
		"Data\\RoadStage\\Hard\\TheHalfWay.dat",
		"Data\\OffRoadStage\\Hard\\TheHalfWay.dat" },		//Hard
	};
	//get preset stage
	tagSELECTSTAGE stage = GetStage();
	//subsutitute the half way record for gloabl variable of the half way record
	g_ResultTheHalWayRecord[0][0].re = *(LoadRecordFile(lpFile[gamelevel][stage], RECORD_PREVIEW));

	//subsutitute score's direction and indication value for the half way record
	for (int j = 0; j < RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT; j++) {
		for (int i = 0; i < RECORD_KIND_OF_PARTICIPANT; i++) {

			//subsutitute indication value for drawing variable
			SetResultTheHalfWayValue(&g_ResultTheHalWayRecord[j][i], recordNumTbl[j], i, SCORE_BLUE);

			if (i == PARTICIPANT_PLAYER && j == (RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT - 1)) return;				//when player's item, nothing file's record
			//subsutitute the half way record for score's variable
			switch (j)
			{
			case RECORD_HALF_WAY_RECORD_TOTAL_POINT:	//total point
				g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][i].num = g_ResultTheHalWayRecord[0][0].re.recordBox[i][0];
				break;
			case RECORD_HALF_WAY_RECORD_RACE_TIME:		//record time
				g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti.minutes = g_ResultTheHalWayRecord[0][0].re.recordBox[i][1];
				g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti.second = g_ResultTheHalWayRecord[0][0].re.recordBox[i][2];
				g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti.millisecond = g_ResultTheHalWayRecord[0][0].re.recordBox[i][3];
				break;
			case RECORD_HALF_WAY_RECORD_CHAIN:
				g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_CHAIN][i].num = g_ResultTheHalWayRecord[0][0].re.recordBox[i][4];
				break;
			default:
				break;
			}
		}
	}
}

/*
indiacte present stage's record
*/
void	PresentStageResult() {

	//�W�v�|�C���g�\���ς݂�
	//�L�[���͂Ŏ��̃V�[����ύX
	//�X�^�[�g�{�^�����A�y�L�[�őJ��
	if (g_ResultIndicatedFlag) {
		if (JustButton(BUTTON_1) || JustKey('Z')) {
			//subsutitute next scene for next flag of global variable
			g_ResultSceneNext = RESULT_SCENE_THE_HALF_WAY_RECORD;			//next is the half way result
			//initialize the half way result 
			StartingInitializerTheHalfWayResult();
			return;
		}
	}
	//�Ō�̃t�@�C���܂œǂݍ���ł��Ȃ�������ǂݍ���
	if (g_resultFile[RESULT_FILE_MAX - 1].fRead == false) {

		//�ǂݍ��݂����t�@�C�����܂Ń��[�v����
		for (int i = 2; i < RESULT_FILE_MAX; i++) {
			//�e�t�@�C�������Ԃɓǂݍ���ł���
			if (g_resultFile[i].fRead == false) {
				//�����񐶐�
				CreateTalk(&g_resultFile[i]);
				//�ǂݍ��񂾃t���O��true
				g_resultFile[i].fRead = true;
			}
		}
	}
	//���o�I���Ȃ玞�Ԍo�߂ŃV�[���J�ڂ��\�ɂ���
	if (g_resultScore[RECORD_TOTAL][0].directEnd) {
		//���Ԍo��
		g_resultScore[RECORD_TOTAL][0].ch.time++;
		//�Ԋu��20�t���[��
		if (g_resultScore[RECORD_TOTAL][0].ch.time >= RESULT_SCORE_INDICATED_BLANK_TIME) {
			//�\���σt���O
			g_ResultIndicatedFlag = true;
		}
	}
}

/*
indicate the half way result
*/
void	StartingInitializerTheHalfWayResult() {

	//get current stage
	tagSELECTSTAGE stage = GetStage();

	//don't draw current file
	ReleaseTalk();
	//don't draw current stage's score
	for (int j = 0; j < RECORD_MAX; j++) {
		for (int i = 0; i < g_resultScoreItemTbl[stage][j]; i++) {
			g_resultScore[j][i].ch.flag = false;
		}
	}
	//change current back ground image
	g_ResultBg[RESULT_BACK_IMAGE_STAGE_RESULT].picy = 480;

	//draw ranking
	for (int j = 0; j < RECORD_KIND_OF_PARTICIPANT; j++) {
		g_ResultTheHalfRanking[j].ch.flag = true;
	}
	//draw back image
	g_ResultBg[RESULT_BACK_IMAGE_THE_HALF_WAY_RESULT].flag = true;

}

/*
Update the half way result
*/
void	UpdateTheHalfWayResult() {

	if (g_ResultTheHalfWayTurn == RESULT_RANKING_TUNR_NEXT_SCENE) return;		//check eror

	//draw title of chain record
	if (g_ResultRecordCh[RECORD_HALF_WAY_RECORD_CHAIN].flag == false) g_ResultRecordCh[RECORD_HALF_WAY_RECORD_CHAIN].flag = true;
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
	
	if (g_ResultTheHalWayRecord[recordTbl[g_ResultTheHalfWayTurn]][PARTICIPANT_PLAYER].directEnd) {
		//draw each bonus, that score and image
		g_ResultBonusScore[recordTbl[g_ResultTheHalfWayTurn]].ch.flag = true;		//draw bonus score
		g_ResultBonusCh[recordTbl[g_ResultTheHalfWayTurn]].flag = true;				//draw bonus' images
		for (int i = 0; i < (RESULT_KIND_OF_TURN - 1); i++) {				//when present record indicate last item, to next record
			if (g_ResultTheHalfWayTurn == turnTbl[i]) {
				g_ResultTheHalfWayTurn = nextTbl[i];						//next record's score
				g_ResultRecordCh[nextTitleTbl[i]].flag = true;				//draw record's title
				break;
			}
		}
	}
	//draw half way result, that is total point, time record and chain's record
	//first drawing is chain's record
	for (int j = 0; j < RECORD_KIND_OF_PARTICIPANT; j++) {
		g_ResultTheHalWayRecord[recordTbl[g_ResultTheHalfWayTurn]][j].ch.flag = true;
	}

	//when end the last record drawing, subsutitute bonus point of current ranking for global variable of total point
	if (g_ResultTheHalWayRecord[recordTbl[g_ResultTheHalfWayTurn]][PARTICIPANT_PLAYER].directEnd) {
		//each other execute only once process
		//if player's ranking is less than five, add bonus point to total point
		if (g_ResultTheHalfWayTurn == RESULT_RANKING_TURN_TOTAL) {				//check out of allocation
			g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][PARTICIPANT_PLAYER].num +=					//add bonus point to total point
				(float)bonusTbl[g_ResultPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_CHAIN]] +
				(float)bonusTbl[g_ResultPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_RACE_TIME]];

			//ranking's image add scale rate for the first
			g_ResultTheHalfRanking[0].score_scale = RESULT_SCORE_THE_HALF_WAY_RESULT_RANKING_THE_FIRST_SCALE_RATE;

			//subsutitute each competitors' record for record's box
			RECORD	lpRecord[PARTICIPANT_PLAYER] = { 0 };					//for temporary save
			//get present stage
			tagSELECTSTAGE	stage = GetStage();

			//subsutitute each competitors' bonus point for total point
			for (int i = 0; i < PARTICIPANT_PLAYER; i++) {
				g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][i].num +=					//add bonus point to total point
					(float)bonusTbl[g_ResultCompetitorBonusMaterial[RECORD_HALF_WAY_RECORD_CHAIN][i]] +
					(float)bonusTbl[g_ResultCompetitorBonusMaterial[RECORD_HALF_WAY_RECORD_RACE_TIME][i]];
				//reset score's direction
				g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][i].directEnd = false;
				g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][i].numD = 0;				//reset score's drawing number
				lpRecord[i].recordBox[RECORD_TIME][0] = g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti.minutes;		//minute
				lpRecord[i].recordBox[RECORD_TIME][1] = g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti.second;			//second
				lpRecord[i].recordBox[RECORD_TIME][2] = g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_RACE_TIME][i].ti.millisecond;	//millisecond
				lpRecord[i].recordBox[RECORD_FOUR][0] = g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_CHAIN][i].num;					//chain
				lpRecord[i].recordBox[RECORD_TOTAL][0] = g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][i].num;			//total
			}
			//save competitors' records, these are chain, time and total point
			SaveCompetitorRecordTemporary(lpRecord, RECORD_TIME, stage);							//time
			SaveCompetitorRecordTemporary(lpRecord, RECORD_FOUR, stage);							//chain
			SaveCompetitorRecordTemporary(lpRecord, RECORD_TOTAL, stage);							//total

			//reset score's direction for player
			g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][PARTICIPANT_PLAYER].directEnd = false;
			g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][PARTICIPANT_PLAYER].numD = 0;				//reset score's drawing number
			//subsutitute total point for record
			g_resultScore[0][0].re.recordBox[RECORD_TOTAL][0] = (int)g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][PARTICIPANT_PLAYER].num;
			//compair total point between player and competitor
			for (int i = 0; i < PARTICIPANT_PLAYER; i++) {
				g_ResultPlayerBonusMaterial[RECORD_HALF_WAY_RECORD_TOTAL_POINT] +=
					CompairRecord((int)g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][PARTICIPANT_PLAYER].num,
						(int)g_ResultTheHalWayRecord[RECORD_HALF_WAY_RECORD_TOTAL_POINT][i].num);
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
			next = (g_ResultPlayerBonusMaterial[i] + 1);
			for (int j = g_ResultPlayerBonusMaterial[i]; j < PARTICIPANT_PLAYER; j++) {
				competitorPosY[i] = g_ResultTheHalWayRecord[i][j].score_y;					//get competitor's position Y
				//next is less than 10
				if (next < RECORD_KIND_OF_PARTICIPANT) {
					//player's score replace with competitor's position
					g_ResultTheHalWayRecord[i][j].score_y = g_ResultTheHalWayRecord[i][next].score_y;
					g_ResultTheHalWayRecord[i][next].score_y = competitorPosY[i];
				}
				next = (j + 2);						//this is subsutitution material
			}
		}
		if (g_ResultTheHalfWayTurn == RESULT_RANKING_TURN_TOTAL) {
			g_ResultTheHalfWayTurn = RESULT_RANKING_TUNR_NEXT_SCENE;		//the end process
			return;
		}
	}
}


/*
subsutitute the half way value for global variable
*/
void	SetResultTheHalfWayValue(SCOREVALUE *lpScore, tagRECORDTHEHALFWAYITEM recordNum, int material, tagSCORECOLOR color) {

	//set indication position
	FPOSITION posTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		{ RESULT_SCORE_THE_HALF_WAY_RESULT_TOTAL_POINT_INDICATE_X, 
		RESULT_SCORE_THE_HALF_WAY_RESULT_TOTAL_POINT_INDICATE_Y },			//total point
		{ RESULT_SCORE_THE_HALF_WAY_RESULT_TIME_RECORD_INDICATE_X, 
		RESULT_SCORE_THE_HALF_WAY_RESULT_TIME_RECORD_INDICATE_Y },			//time record
		{ RESULT_SCORE_THE_HALF_WAY_RESULT_CHAIN_RECORD_INDICATE_X,
		RESULT_SCORE_THE_HALF_WAY_RESULT_CHAIN_RECORD_INDICATE_Y },			//CHAIN record
	};
	//set spatial indication
	FPOSITION spaceTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		{ 0, RESULT_SCORE_THE_HALF_WAY_RESULT_TOTAL_POINT_INDICATE_SPACE_Y },//total point
		{ RESULT_SCORE_THE_HALF_WAY_RESULT_TIME_RECORD_INDICATE_SPACE_X,
		  RESULT_SCORE_THE_HALF_WAY_RESULT_TIME_RECORD_INDICATE_SPACE_Y },	//time record
		{ 0, RESULT_SCORE_THE_HALF_WAY_RESULT_CHAIN_RECORD_INDICATE_SPACE_Y },	//CHAIN record
	};
	//set default scale rate
	float scaleTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		RESULT_SCORE_THE_HALF_WAY_RESULT_TOTAL_POINT_SCALE_RATE,			//total point
		RESULT_SCORE_THE_HALF_WAY_RESULT_TIME_RECORD_SCALE_RATE,			//time record
		RESULT_SCORE_THE_HALF_WAY_RESULT_CHAIN_RECORD_SCALE_RATE,			//CHAIN record
	};
	//score's direction type
	char directionTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		SCORE_GRADUALLY_DIRECT_NORMAL  ,				//total point
		SCORE_GRADUALLY_DIRECT_ROLLING ,				//time record
		SCORE_GRADUALLY_DIRECT_ROLLING ,				//chain record
	};
	//score's direction type, which is gradually direction
	//interval time of indication
	int intervalTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = { 
		RESULT_SCORE_THE_HALF_WAY_RESULT_TOTAL_POINT_DIRECTION_GRADUALLY_INTERVAL_TIME,			//total point
		0,																						//time record
		0,																						//chain record
	};
	//score's direction type, which is rolling direction
	//rolling time
	int rollingTimeTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		0,																				//total point
		RESULT_SCORE_THE_HALF_WAY_RESULT_TIME_RECORD_DIRECTION_ROLLING_TIME,			//time record
		RESULT_SCORE_THE_HALF_WAY_RESULT_CHAIN_RECORD_DIRECTION_ROLLING_TIME,			//CHAIN record
	};
	//set default digit
	int digitTbl[RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT] = {
		RESULT_SCORE_THE_HALF_WAY_RESULT_TOTAL_POINT_DIGIT,							//total point
		RESULT_SCORE_THE_HALF_WAY_RESULT_TIME_RECORD_DIGIT,							//time record
		RESULT_SCORE_THE_HALF_WAY_RESULT_CHAIN_RECORD_DIGIT,						//CHAIN record
	};

	//subsutitute default value
	lpScore->spaceX = spaceTbl[recordNum].fX;
	lpScore->spaceY = spaceTbl[recordNum].fY;
	lpScore->score_x = posTbl[recordNum].fX;
	lpScore->score_y = posTbl[recordNum].fY + (lpScore->spaceY * material);
	lpScore->score_scale = scaleTbl[recordNum];
	lpScore->ch.type = SCORE_NUMBER_TYPE_NORMAL;
	lpScore->scoreColor = color;
	lpScore->digit = digitTbl[recordNum];
	lpScore->gradually = true;
	lpScore->graduallyDirect = directionTbl[recordNum];
	lpScore->interval = intervalTbl[recordNum];
	lpScore->rollTime = rollingTimeTbl[recordNum] + (material * 10);
	lpScore->ch.time = 0;
	lpScore->renewalColor = RESULT_SCORE_COLOR_COMMON_RENEWAL;
	lpScore->numD = 0;
}