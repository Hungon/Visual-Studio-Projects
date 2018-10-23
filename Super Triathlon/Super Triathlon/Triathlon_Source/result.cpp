
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

//▼グローバル変数

//画像の要素番号を扱う
int g_resultImg[RESULT_IMG_MAX] = {0}; 

//back ground image
CHARACTER g_ResultBg[2] = { 0 };
//ファイル用
TALK g_resultFile[RESULT_FILE_MAX] = {0};

//リザルトスコアを扱う
SCOREVALUE g_resultScore[RECORD_MAX][RECORD_MAX] = {0};

//リザルト結果表示済みフラグ
bool	g_ResultIndicatedFlag = false;

//リザルトで扱う各項目の数
//①海ステージ：敵討伐、獲得アイテム、時間、敵討伐CHAIN数、集計ポイントの順番
//②道路ステージ：破壊した障害物、飛んだハードル、時間、ハードル跳躍CHAIN数、集計ポイント、順位の順番
//③オフロードステージ：成功したアクション、合計アクションPt、時間、連続アクション数、集計ポイントの順番
char g_resultScoreItemTbl[STAGE_MAX][RECORD_MAX] = {
	{ 5 , 5 , 1 , 1 , 1 } ,			//海
	{ 2 , 3 , 1 , 1 , 2 } ,			//道路
	{ 4 , 1 , 1 , 1 , 1 } ,			//オフロード
};

//RunnerManager class
CRunnerManager	g_CRunnerManagerForResult;
//プレイヤーランナークラス
CPlayerRunner	g_CPlayerRunnerForResult;
//ライダークラス
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

ファイルの読み込み
************************************************************************************************
*/
void LoadResultFile(){

	//get current stage's number
	tagSELECTSTAGE stageNum = GetStage();
	//get the current language
	tagGAMELANGUAGE language = GetGameLanguage();
	//▼ 各ステージの読み込みファイルデータテーブル
	//for Japanese language
	char *lpJapaneseFileTbl[STAGE_MAX][RESULT_FILE_MAX] = {

		//海ステージ
		{ "Data\\SeaStage\\PreviewScore.dat" ,								//過去の記録
		"Data\\SeaStage\\BestScore.dat" ,									//最高の記録
		"Data\\SeaStage\\Japanese\\ResultValue.dat" ,						//時間、CHAIN数
		"Data\\SeaStage\\Japanese\\ResultEnemy.dat" ,						//敵討伐数
		"Data\\SeaStage\\Japanese\\ResultItem.dat" }						//獲得アイテム数
		,

		//道路ステージ
		{ "Data\\RoadStage\\PreviewScore.dat" ,								//過去の記録
		"Data\\RoadStage\\BestScore.dat" ,									//最高の記録
		"Data\\RoadStage\\Japanese\\ResultValue.dat" ,						//時間、CHAIN数
		"Data\\RoadStage\\Japanese\\ResultObstacles.dat" ,					//障害物破壊数
		"Data\\RoadStage\\Japanese\\ResultHurdle.dat" }						//飛んだハードル数
		,

		//オフロードステージ
		{ "Data\\OffRoadStage\\PreviewScore.dat" ,							//過去の記録
		"Data\\OffRoadStage\\BestScore.dat" ,								//最高の記録
		"Data\\OffRoadStage\\Japanese\\ResultValue.dat" ,					//時間、連続アクション数
		"Data\\OffRoadStage\\Japanese\\ResultSuccessAction.dat" ,			//成功アクション
		"Data\\OffRoadStage\\Japanese\\ResultTotalActionPoint.dat" }		//合計アクションポイント
		,
	};
	//for English language
	char *lpEnglishFileTbl[STAGE_MAX][RESULT_FILE_MAX] = {

		//海ステージ
		{ "Data\\SeaStage\\PreviewScore.dat" ,								//過去の記録
		"Data\\SeaStage\\BestScore.dat" ,									//最高の記録
		"Data\\SeaStage\\English\\ResultValue.dat" ,						//時間、CHAIN数
		"Data\\SeaStage\\English\\ResultEnemy.dat" ,						//敵討伐数
		"Data\\SeaStage\\English\\ResultItem.dat" }							//獲得アイテム数
		,

		//道路ステージ
		{ "Data\\RoadStage\\PreviewScore.dat" ,								//過去の記録
		"Data\\RoadStage\\BestScore.dat" ,									//最高の記録
		"Data\\RoadStage\\English\\ResultValue.dat" ,						//時間、CHAIN数
		"Data\\RoadStage\\English\\ResultObstacles.dat" ,					//障害物破壊数
		"Data\\RoadStage\\English\\ResultHurdle.dat" }						//飛んだハードル数
		,

		//オフロードステージ
		{ "Data\\OffRoadStage\\PreviewScore.dat" ,							//過去の記録
		"Data\\OffRoadStage\\BestScore.dat" ,								//最高の記録
		"Data\\OffRoadStage\\English\\ResultValue.dat" ,					//時間、連続アクション数
		"Data\\OffRoadStage\\English\\ResultSuccessAction.dat" ,			//成功アクション
		"Data\\OffRoadStage\\English\\ResultTotalActionPoint.dat" }			//合計アクションポイント
		,
	};

	//for fixed pattern file
	tagRESULTFIXEDFILE fixedFile[RESULT_KIND_OF_FIXED_PATTERN_FILE] = {
		RESULT_FIXED_PATTERN_FILE_1,			//file 1
		RESULT_FIXED_PATTERN_FILE_2,			//file 2
		RESULT_FIXED_PATTERN_FILE_3,			//file 3
	};

	//閲覧したいステージのファイル文字列をコピー
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
//初期化
//************************************************************************************************
void InitCommonResult( HWND hWnd ){


	//記録更新を取得
	RECORD* pre = GetRecord( RECORD_PREVIEW );

	//difficulty in playing
	tagGAMELEVEL	gameLevel = GetGameLevel();
	//bonus point according to difficulty
	int		bonusPointTbl[LEVEL_MAX] = { 0 , 7500 , 15000 };
	//get current stage's number
	tagSELECTSTAGE getStage = GetStage();

	//▼ ステージ初期化テーブル
	void ( *pInitStage[STAGE_MAX] )() = {

		InitSeaResult		,			//海			
		InitRoadResult		,			//道路
		InitOffRoadResult	,			//オフロード
	};

	//画像テーブル
	char *pImagePass[STAGE_MAX] = {
		//各種結果画面
		"Image\\Sea\\Result.bmp" ,			//海
		"Image\\Road\\Result.bmp" ,			//道路
		"Image\\OffRoad\\Result.bmp" ,		//オフロード
	};
	//image
	char *lpImageTbl[RESULT_IMG_MAX] = {
		pImagePass[getStage],					//stage's image
		"Image\\EndTheResult.bmp",				//record's image
		"Image\\Bonus.bmp",						//bonus' image
		"Image\\EndResult.bmp",					//back ground image for the half way result
	};

	//リザルト結果表示済みフラグ
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
	
	//▼ クリア済ステージを設定する
	SetCompleteStage( SELECTSTAGE_STAGE_COMPLETE , getStage );

	//スコアの初期化
	InitScore( hWnd );

	//記録の初期化
	InitRecord();

	//▼　画像の読み込み
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
	
	//▼　曲の読み込み
	CreateStreaming("BGM\\result.wav");

	//subsutitute score's direction and indication value
	for (int j = 0; j < RECORD_MAX; j++) {
		for (int i = 0; i < g_resultScoreItemTbl[getStage][j]; i++) {
			SetResultStageIndicationValue(&g_resultScore[j][i], j, i);
			//indication flag
			g_resultScore[j][i].ch.flag = true;
		}
	}
	
	//難易度に応じたボーナスポイントを予め代入
	// 記録用
	g_resultScore[0][0].re.recordBox[RECORD_TOTAL][0] = bonusPointTbl[gameLevel];
	// 表示用
	g_resultScore[RECORD_TOTAL][0].num = (float)bonusPointTbl[gameLevel];

	//▼ load file of current stage
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

	//▼ 各ステージの初期化
	pInitStage[getStage]();

	//different setting by stage and record
	for (int i = 0; i < RECORD_MAX; i++) {
		DeferScoreOrFixedFileSetting(g_resultScore[i], &g_resultFile[0], getStage, i);
	}
	
	//*************************記録の更新*************************************

	//▼ 今回の記録を保存する
	SaveRecordForStage( &g_resultFile[RECORD_PREVIEW].pass[0] , &g_resultScore[0][0].re , RECORD_PREVIEW );

	//▼ 今回の記録と過去に記録した最高の記録を保存したファイルを開いて
	//自己ベスト記録を更新する
	RenewalRecord( &g_resultFile[RECORD_BEST].pass[0] );

	//各項目のスコアの色を変更する
	for( int j = 0 ; j < RECORD_MAX ; j++ ){
		for( int i = 0 ; i < g_resultScoreItemTbl[getStage][j] ; i++ ){
			//更新フラグなら表示色を変更する
			if( pre->renewalFlag[j][i] == RECORD_RENEWAL_FLAG ){
				//表示用に更新フラグを受け取る
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
//海ステージの初期化
//************************************************************************************************
void InitSeaResult(){

	char mes[256] = {0};	//文字列を扱う
	int time = 0;			//時間の合計からポイントを算出
	int defeat = 0;			//敵討伐ポイント
	int cnt = 0;			//要素カウント用

	//敵の情報を取得
	ENEMY* pen = GetEnemy();
	//クリア時間を取得
	TIME* pt = GetStageCompleteTime();
	//アイテムの情報を取得
	ITEM* pi = GetItem();
	//プレイヤーの情報を取得
	SWIMER* pch = GetSwimer();

	//▼　各項目のポイントを計算
	
	//クリア時間のポイントを計算
	time = 20000 - MINUTES_INTO_MILLISECOND( pt->minutes ) +	//分
		SECONDS_INTO_MILLISECOND( pt->second ) +				//秒
		pt->millisecond;										//ミリ秒


	//敵撃破数からポイントを算出
	for( int i = 0 ; i < KIND_ENEMY_SEA ; i++ ){
		defeat += ( pen->defeat[i] * 25 ) + ( (i * 10 ) * pen->defeat[i] );
	}

	//▼ 各種項目の値を取得する

	//******************海ステージ************************

	//① 各種敵の討伐数を受け取る
	//▼ 記録用
	g_resultScore[0][0].re.recordBox[RECORD_ONE][0] = pen->defeat[JELLYFISH];			//クラゲ
	g_resultScore[0][0].re.recordBox[RECORD_ONE][1] = pen->defeat[SHARK];				//サメ
	g_resultScore[0][0].re.recordBox[RECORD_ONE][2] = pen->defeat[SHOAL];				//魚群
	g_resultScore[0][0].re.recordBox[RECORD_ONE][3] = pen->defeat[SUNFISH];			//マンボー
	g_resultScore[0][0].re.recordBox[RECORD_ONE][4] = pen->defeat[RAY];				//エイ
	//▼ 表示用
	for( int i = 0 ; i < g_resultScoreItemTbl[STAGE_SEA][RECORD_ONE] ; i++ ){
		g_resultScore[RECORD_ONE][i].num = (float)pen->defeat[i];
	}


	//② 各種アイテムの獲得数を受け取る
	g_resultScore[0][0].re.recordBox[RECORD_TWO][0] = pi->get[ITEM_TYPE_SPEED_UP];		//速度上昇
	g_resultScore[0][0].re.recordBox[RECORD_TWO][1] = pi->get[ITEM_TYPE_ABSOLUTE];		//無敵
	g_resultScore[0][0].re.recordBox[RECORD_TWO][2] = pi->get[ITEM_TYPE_BIG];			//拡大
	g_resultScore[0][0].re.recordBox[RECORD_TWO][3] = pi->get[ITEM_TYPE_SPEED_DOWN];	//速度減少
	g_resultScore[0][0].re.recordBox[RECORD_TWO][4] = pi->get[ITEM_TYPE_SMALL];		//縮小
	//▼ 表示用
	for( int i = 0 ; i <= ITEM_TYPE_SMALL ; i++ ){
	
		g_resultScore[RECORD_TWO][cnt].num = (float)pi->get[i];

		//要素数カウント
		cnt++;
		
		//強化アイテム取得後、弱小アイテムの要素に合わせる
		if( i == ITEM_TYPE_BIG ) i = ( ITEM_TYPE_SPEED_DOWN - 1 );
	}

	//③ クリア時間を受け取る
	//▼ 記録用
	//分
	g_resultScore[0][0].re.recordBox[RECORD_TIME][0] = pt->minutes;
	//秒
	g_resultScore[0][0].re.recordBox[RECORD_TIME][1] = pt->second;
	//ミリ秒
	g_resultScore[0][0].re.recordBox[RECORD_TIME][2] = pt->millisecond;
	//▼ 表示用
	g_resultScore[RECORD_TIME][0].ti.minutes = pt->minutes;			//分
	g_resultScore[RECORD_TIME][0].ti.second = pt->second;			//秒
	g_resultScore[RECORD_TIME][0].ti.millisecond = pt->millisecond;		//ミリ秒
	

	//④ CHAIN数を受け取る
	g_resultScore[0][0].re.recordBox[RECORD_FOUR][0] = pch->chain_max;
	//▼ 表示用
	g_resultScore[RECORD_FOUR][0].num = (float)pch->chain_max;
	

	//⑤ 集計ポイント
	g_resultScore[0][0].re.recordBox[RECORD_TOTAL][0] += time + defeat + ( pch->chain_max * 200 );
	//▼ 表示用
	g_resultScore[RECORD_TOTAL][0].num += (float)( time + defeat + ( pch->chain_max * 200 ) );

}


/*
***************************************************************************************************
ロードステージ用の初期化
***************************************************************************************************
*/
void InitRoadResult(){

	//時間情報を取得
	TIME* pt = GetStageCompleteTime();
	//ランナー情報を取得
	RUNNER* pPrunner = g_CPlayerRunnerForResult.GetRunner();
	//Get player's ranking in this race
	int	ranking = g_CRunnerManagerForResult.GetPlayerRunnerRanking();

	//ポイント計算用
	int time = 0;					//時間の合計からポイントを算出
	int defeat = 0;					//破壊ペナルティ

	//▼　各項目のポイントを計算
	
	//クリア時間のポイントを計算
	time = 20000 - MINUTES_INTO_MILLISECOND( pt->minutes ) +	//分
		SECONDS_INTO_MILLISECOND( pt->second ) +				//秒
		pt->millisecond;										//ミリ秒

	//① 破壊した数
	for( int i = 0 ; i < g_resultScoreItemTbl[STAGE_ROAD][RECORD_DFEAT_OBSTACLES] ; i++ ){
		//記録用
		g_resultScore[0][0].re.recordBox[0][i] = pPrunner->defeatCnt[i];
		//表示用
		g_resultScore[0][i].num = (float)pPrunner->defeatCnt[i];
		//ポイント計算用に代入
		defeat -= pPrunner->defeatCnt[i] * 100;
	}

	//② 飛び越えたハードルの数
	for( int i = 0 ; i < g_resultScoreItemTbl[STAGE_ROAD][RECORD_JUMPED_HURDLE] ; i++ ){
		//記録用
		g_resultScore[0][0].re.recordBox[1][i] = pPrunner->jumpedHurdleCnt[i];
		//表示用
		g_resultScore[1][i].num = (float)pPrunner->jumpedHurdleCnt[i];
	}

	//③ クリア時間を受け取る
	//▼ 記録用
	//分
	g_resultScore[0][0].re.recordBox[RECORD_TIME][0] = pt->minutes;
	//秒
	g_resultScore[0][0].re.recordBox[RECORD_TIME][1] = pt->second;
	//ミリ秒
	g_resultScore[0][0].re.recordBox[RECORD_TIME][2] = pt->millisecond;
	//▼ 表示用
	g_resultScore[RECORD_TIME][0].ti.minutes = pt->minutes;				//分
	g_resultScore[RECORD_TIME][0].ti.second = pt->second;				//秒
	g_resultScore[RECORD_TIME][0].ti.millisecond = pt->millisecond;		//ミリ秒

	//④ ハードルを連続で飛び越えた最大数
	//記録用
	g_resultScore[0][0].re.recordBox[RECORD_FOUR][0] = pPrunner->jumpedHurdleChainMax;
	//表示用
	g_resultScore[RECORD_FOUR][0].num = (float)pPrunner->jumpedHurdleChainMax;

	//ranking
	//for record
	g_resultScore[0][0].re.recordBox[RECORD_TOTAL][1] = ranking;					//point in this race
	//for drawing
	g_resultScore[RECORD_TOTAL][1].num = ranking;
	
	//⑤ 集計ポイント
	//記録用
	g_resultScore[0][0].re.recordBox[RECORD_TOTAL][0] += time + defeat + ( pPrunner->jumpedHurdleChainMax * 200 ) + 
		(5100 - (ranking * 100));
	//表示用
	g_resultScore[RECORD_TOTAL][0].num += (float)( time + defeat + ( pPrunner->jumpedHurdleChainMax * 200 ) ) + 
		(5100 - (ranking * 100));

}


/*
***************************************************************************************************
オフロード用の初期化
***************************************************************************************************
*/
void InitOffRoadResult(){

	//時間情報を取得
	TIME* pt = GetStageCompleteTime();
	//ライダーの情報を取得
	int		totalActionPt = g_ResultCOffRoadRacer.GetTotalActionPoint();					//合計アクションPｔ
	int		*lpSuccessAction = g_ResultCOffRoadRacer.GetSuccessActionCount();				//成功したアクション数
	int		succeedActionConsecutive = g_ResultCOffRoadRacer.GetScceedActionConsecutive();	//連続アクション成功数
	
	//get present language
	tagGAMELANGUAGE language = GetGameLanguage();
	
	//ポイント計算用
	int time = 0;					//時間の合計からポイントを算出
	int action = 0;					//アクション成功数に応じたボーナス

	//▼　各項目のポイントを計算
	
	//クリア時間のポイントを計算
	time = 20000 - MINUTES_INTO_MILLISECOND( pt->minutes ) +	//分
		SECONDS_INTO_MILLISECOND( pt->second ) +				//秒
		pt->millisecond;										//ミリ秒

	//① 成功アクション数
	for( int i = 0 ; i < g_resultScoreItemTbl[STAGE_OFF_ROAD][0] ; i++ ){

		//記録用
		g_resultScore[RECORD_ONE][0].re.recordBox[0][i] = *( lpSuccessAction + i );
		//表示用
		g_resultScore[RECORD_ONE][i].num = (float)*(lpSuccessAction + i);
		//ポイント計算用に代入
		action += lpSuccessAction[i] * 100;
	}

	//②合計アクションポイント
	//記録用
	g_resultScore[0][0].re.recordBox[1][0] = totalActionPt;
	//表示用
	g_resultScore[RECORD_TWO][0].num = (float)totalActionPt;
	//桁数を変更
	g_resultScore[RECORD_TWO][0].digit = RESULT_SCORE_FOURTH_DIGIT_IN_OFF_ROAD;
	//change indicationX
	g_resultScore[RECORD_TWO][0].score_x = RESULT_SCORE_SECOND_INDICATE_X_IN_OFF_ROAD;

	//③ クリア時間を受け取る
	//▼ 記録用
	//分
	g_resultScore[0][0].re.recordBox[RECORD_TIME][0] = pt->minutes;
	//秒
	g_resultScore[0][0].re.recordBox[RECORD_TIME][1] = pt->second;
	//ミリ秒
	g_resultScore[0][0].re.recordBox[RECORD_TIME][2] = pt->millisecond;
	//▼ 表示用
	g_resultScore[RECORD_TIME][0].ti.minutes = pt->minutes;			//分
	g_resultScore[RECORD_TIME][0].ti.second = pt->second;			//秒
	g_resultScore[RECORD_TIME][0].ti.millisecond = pt->millisecond;		//ミリ秒

	//④ 連続アクション成功数
	//記録用
	g_resultScore[0][0].re.recordBox[RECORD_FOUR][0] = succeedActionConsecutive;
	//表示用
	g_resultScore[RECORD_FOUR][0].num = (float)succeedActionConsecutive;

	//⑤ 集計ポイント
	//記録用
	g_resultScore[0][0].re.recordBox[RECORD_TOTAL][0] += time + action + (succeedActionConsecutive * 200 ) + totalActionPt;
	//表示用
	g_resultScore[RECORD_TOTAL][0].num += (float)( time + action + (succeedActionConsecutive * 200 ) ) + totalActionPt;

}



//************************************************************************************************
//更新
//************************************************************************************************
void UpdateResult() {

	//曲の再生
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
//表示
//************************************************************************************************
void DrawResult(){

	//現在のステージ番号を取得
	int getStage = GetStage();

	//スコア表示関数の分岐
	void (*pscore[RECORD_MAX])(SCOREVALUE*) = {

		DrawScore	,			//敵表示
		DrawScore	,			//アイテム表示
		DrawTime	,			//時間表示
		DrawScore	,			//CHAIN数
		DrawScore	,			//集計ポイント
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

	//▼各項目の値を表示
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
				//スコア表示の分岐
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
//解放
//************************************************************************************************
void ReleaseResult(){
	
	//現在のステージ番号を取得
	tagSELECTSTAGE getStage = GetStage();

	//画像の解放
	for (int i = 0; i < RESULT_IMG_MAX; i++) { ReleaseDDImage(g_resultImg[i]); }
	//*************************記録の更新*************************************

	//▼ 今回の記録を保存する
	SaveRecordForStage(&g_resultFile[RECORD_PREVIEW].pass[0], &g_resultScore[0][0].re, RECORD_PREVIEW);
	//▼ 今回の記録と過去に記録した最高の記録を保存したファイルを開いて
	//自己ベスト記録を更新する
	RenewalRecord(&g_resultFile[RECORD_BEST].pass[0]);

	//曲の解放
	ReleaseStreaming();
	
	//ファイルの解放
	for( int i = 0 ; i < RESULT_FILE_MAX ; i++ ){
		//０で初期化
		ZeroMemory( &g_resultFile[i] , sizeof( TALK ) );
	}
	//リザルト結果のリセット
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

	//Talkの解放
	ReleaseTalk();
	//スコアの解放
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
	lpTalk->baseX = posXTbl[fileNum][language];			//表示Ｘ座標
	lpTalk->baseY = posYTbl[fileNum][language];			//表示Ｙ座標
	lpTalk->next = 1;									//文字送りの間隔
	lpTalk->size = fontTbl[fileNum][language];			//文字のサイズ
	lpTalk->line_space = spaceYTbl[fileNum][language];	//行の間隔
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

	//集計ポイント表示済みで
	//キー入力で次のシーンを変更
	//スタートボタンか、Ｚキーで遷移
	if (g_ResultIndicatedFlag) {
		if (JustButton(BUTTON_1) || JustKey('Z')) {
			//subsutitute next scene for next flag of global variable
			g_ResultSceneNext = RESULT_SCENE_THE_HALF_WAY_RECORD;			//next is the half way result
			//initialize the half way result 
			StartingInitializerTheHalfWayResult();
			return;
		}
	}
	//最後のファイルまで読み込んでいなかったら読み込み
	if (g_resultFile[RESULT_FILE_MAX - 1].fRead == false) {

		//読み込みたいファイル数までループを回す
		for (int i = 2; i < RESULT_FILE_MAX; i++) {
			//各ファイルを順番に読み込んでいく
			if (g_resultFile[i].fRead == false) {
				//文字列生成
				CreateTalk(&g_resultFile[i]);
				//読み込んだフラグをtrue
				g_resultFile[i].fRead = true;
			}
		}
	}
	//演出終了なら時間経過でシーン遷移を可能にする
	if (g_resultScore[RECORD_TOTAL][0].directEnd) {
		//時間経過
		g_resultScore[RECORD_TOTAL][0].ch.time++;
		//間隔は20フレーム
		if (g_resultScore[RECORD_TOTAL][0].ch.time >= RESULT_SCORE_INDICATED_BLANK_TIME) {
			//表示済フラグ
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