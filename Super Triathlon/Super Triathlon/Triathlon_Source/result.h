#ifndef __RESULT_H__
#define __RESULT_H__

//▼ヘッダーをインクルード
#include "graphics.h"
#include "score.h"
#include "talk.h"
#include "SelectStage.h"

//▼マクロの定義
#define RESULT_IMG_MAX		4
//読み込むファイル数
#define RESULT_FILE_MAX		5

//kind of back ground image
enum tagRESULTBACKIMAGE {
	RESULT_BACK_IMAGE_STAGE_RESULT,					//stage's result
	RESULT_BACK_IMAGE_THE_HALF_WAY_RESULT,			//the half way result
	RESULT_KIND_OF_BACK_IMAGE,						//kind
};
//for fixed pattern file
enum tagRESULTFIXEDFILE {
	RESULT_FIXED_PATTERN_FILE_1,			//file 1
	RESULT_FIXED_PATTERN_FILE_2,			//file 2
	RESULT_FIXED_PATTERN_FILE_3,			//file 3
	RESULT_KIND_OF_FIXED_PATTERN_FILE,		//kind
};
//scene transition in result
enum tagRESULTTRANSITION {
	RESULT_SCENE_STAGE_RECORD,				//stage's record
	RESULT_SCENE_THE_HALF_WAY_RECORD,		//the half way result
	RESULT_SCENE_NEXT,						//the end result
	RESULT_KIND_SCENE,						//kind of scene
};
//player's bonus point and kind of bonus
enum tagRESULTBONUS {
	RESULT_BONUS_FIRST = 10000,				//the first
	RESULT_BONUS_SECOND = 7500,				//the second
	RESULT_BONUS_THIRD = 5000,				//the third
	RESULT_BONUS_FOURTH = 3000,				//the fourth
	RESULT_BONUS_FIFTH = 1500,				//the fifth
	RESULT_BONUS_SIXTH = 750,				//the sixth
	RESULT_BONUS_SEVENTH = 300,				//the seventh
	RESULT_BONUS_EIGHTTH = 200,				//the eightth
	RESULT_BONUS_NINETH = 100,				//the nineth
	RESULT_BONUS_TENTH = 50,				//the tenth
	RESULT_KIND_OF_BONUS = 10,				//kind
};

//kind of bonus' image
enum tagRESULTBONUSIMAGE {
	RESULT_BONUS_IMAGE_BONUS,				//bonus
	RESULT_BONUS_IMAGE_TOTAL,				//total bonus
	RESULT_KIND_OF_BONUS_IMAGE,				//kind
};

//treat scene's flag in the half way result and terminate result
enum tagRESULTRANKINGEACHTURN {
	RESULT_RANKING_TURN_CHAIN,				//chain's turn
	RESULT_RANKING_TURN_TIME,				//time's turn
	RESULT_RANKING_TURN_TOTAL,				//total's turn
	RESULT_RANKING_TUNR_NEXT_SCENE,			//next scene
	RESULT_KIND_OF_TURN,					//kind
};

//setting back image
const int RESULT_BACK_IMAGE_THE_HALF_WAY_RESULT_WIDTH = 580;
const int RESULT_BACK_IMAGE_THE_HALF_WAY_RESULT_HEIGHT = 400;
const float RESULT_BACK_IMAGE_THE_HALF_WAY_RESULT_INDICATE_Y = 70.0f;

//結果表示後の空き時間
const int RESULT_SCORE_INDICATED_BLANK_TIME = 20;

//balan time for change scene
const int	RESULT_SCENE_BLANK_TIME = 20;

//各種ステージのスコアの色
//通常時
const tagSCORECOLOR RESULT_SCORE_COLOR_COMMON_USUAL = SCORE_BLACK;
//更新時
const tagSCORECOLOR RESULT_SCORE_COLOR_COMMON_RENEWAL = SCORE_YELOW;

//set default value for three stage's score
//first item
const float RESULT_SCORE_FIRST_INDICATE_X = 220.0f;
const float RESULT_SCORE_FIRST_INDICATE_Y = 185.0f;
const float RESULT_SCORE_FIRST_INDICATE_SPACE_Y = 34.0f;
const float RESULT_SCORE_FIRST_DEFAULT_SCALE = 0.3f;
const int	RESULT_SCORE_FIRST_DIGIT = 3;
const int	RESULT_SCORE_FIRST_DIRECTION_ROLLING_DEFAULT_TIME = 240;
//second item
const float RESULT_SCORE_SECOND_INDICATE_X = 550.0f;
const float RESULT_SCORE_SECOND_INDICATE_Y = 185.0f;
const float RESULT_SCORE_SECOND_INDICATE_SPACE_Y = 34.0f;
const float RESULT_SCORE_SECOND_DEFAULT_SCALE = 0.3f;
const int	RESULT_SCORE_SECOND_DIGIT = 3;
const int	RESULT_SCORE_SECOND_DIRECTION_ROLLING_DEFAULT_TIME = 240;
//third item
const float RESULT_SCORE_THIRD_INDICATE_X = 200.0f;
const float RESULT_SCORE_THIRD_INDICATE_Y = 110.0f;
const float RESULT_SCORE_THIRD_INDICATE_SPACE_Y = 0.0f;
const float RESULT_SCORE_THIRD_INDICATE_SPACE_X = 10.0f;
const float RESULT_SCORE_THIRD_DEFAULT_SCALE = 0.35f;
const int	RESULT_SCORE_THIRD_DIGIT = 2;
const int	RESULT_SCORE_THIRD_DIRECTION_ROLLING_DEFAULT_TIME = 440;
//fourth item
const float RESULT_SCORE_FOURTH_INDICATE_X = 410.0f;
const float RESULT_SCORE_FOURTH_INDICATE_Y = 100.0f;
const float RESULT_SCORE_FOURTH_DEFAULT_SCALE = 0.5f;
const int	RESULT_SCORE_FOURTH_DIGIT = 3;
const int	RESULT_SCORE_FOURTH_DIRECTION_ROLLING_DEFAULT_TIME = 200;
//fifth item to total point
const float RESULT_SCORE_FIFTH_INDICATE_X = 410.0f;
const float RESULT_SCORE_FIFTH_INDICATE_Y = 380.0f;
const float RESULT_SCORE_FIFTH_DEFAULT_SCALE = 0.75f;
const int	RESULT_SCORE_FIFTH_DIGIT = 6;
const int	RESULT_SCORE_FIFTH_DIRECTION_GRADUALLY_DEFAULT_INTERVAL_TIME = 30;
//fifth item to player's ranking
const float RESULT_SCORE_FIFTH_PLAYER_RANKING_INDICATE_X = 70.0f;
const float RESULT_SCORE_FIFTH_PLAYER_RANKING_INDICATE_Y = 380.0f;
const float RESULT_SCORE_FIFTH_PLAYER_RANKING_DEFAULT_SCALE = 1.0f;
const int	RESULT_SCORE_FIFTH_PLAYER_RANKING_DIGIT = 2;
const int	RESULT_SCORE_FIFTH_PLAYER_RANKING_DIRECTION_ROLLING_DEFAULT_TIME = 200;
//if stage is road
const float RESULT_SCORE_FIFTH_INDICATE_X_IN_ROAD = 490.0f;				//change total point's indicationX

//set digit in only offroad stage
const int RESULT_SCORE_FOURTH_DIGIT_IN_OFF_ROAD  = 6;		//digit of technical point
//if stage is off-road, each technical action counts change indication X
const float RESULT_SCORE_FIRST_INDICATE_X_IN_OFF_ROAD_AND_ENGLISH = 315.0f;			//Only English
const float RESULT_SCORE_SECOND_INDICATE_X_IN_OFF_ROAD = 500.0f;

//The half way result
//title image
const float RESULT_IMAGE_THE_HALF_WAY_RESULT_TOTAL_POINT_TITLE_INDICATE_X = 110.0f;
const float RESULT_IMAGE_THE_HALF_WAY_RESULT_TOTAL_POINT_TITLE_INDICATE_Y = 50.0f;
const float RESULT_IMAGE_THE_HALF_WAY_RESULT_TOTAL_POINT_TITLE_SCALE_RATE = 0.6f;
//total point
const float RESULT_SCORE_THE_HALF_WAY_RESULT_TOTAL_POINT_INDICATE_X = 200.0f;
const float RESULT_SCORE_THE_HALF_WAY_RESULT_TOTAL_POINT_INDICATE_Y = 80.0f;
const float RESULT_SCORE_THE_HALF_WAY_RESULT_TOTAL_POINT_INDICATE_SPACE_Y = 30.0f;
const float RESULT_SCORE_THE_HALF_WAY_RESULT_TOTAL_POINT_SCALE_RATE = 0.3f;
const int RESULT_SCORE_THE_HALF_WAY_RESULT_TOTAL_POINT_DIGIT = 6;
const int RESULT_SCORE_THE_HALF_WAY_RESULT_TOTAL_POINT_DIRECTION_GRADUALLY_INTERVAL_TIME = 20;
//title image
const float RESULT_IMAGE_THE_HALF_WAY_RESULT_TIME_RECORD_TITLE_INDICATE_X = 285.0f;
const float RESULT_IMAGE_THE_HALF_WAY_RESULT_TIME_RECORD_TITLE_INDICATE_Y = 50.0f;
const float RESULT_IMAGE_THE_HALF_WAY_RESULT_TIME_RECORD_TITLE_SCALE_RATE = 0.5f;
//time record
const float RESULT_SCORE_THE_HALF_WAY_RESULT_TIME_RECORD_INDICATE_X = 385.0f;
const float RESULT_SCORE_THE_HALF_WAY_RESULT_TIME_RECORD_INDICATE_Y = 80.0f;
const float RESULT_SCORE_THE_HALF_WAY_RESULT_TIME_RECORD_INDICATE_SPACE_X = 10.0f;
const float RESULT_SCORE_THE_HALF_WAY_RESULT_TIME_RECORD_INDICATE_SPACE_Y = 30.0f;
const float RESULT_SCORE_THE_HALF_WAY_RESULT_TIME_RECORD_SCALE_RATE = 0.3f;
const int RESULT_SCORE_THE_HALF_WAY_RESULT_TIME_RECORD_DIGIT = 2;
const int RESULT_SCORE_THE_HALF_WAY_RESULT_TIME_RECORD_DIRECTION_ROLLING_TIME = 180;
//title image
const float RESULT_IMAGE_THE_HALF_WAY_RESULT_CHAIN_RECORD_TITLE_INDICATE_X = 450.0f;
const float RESULT_IMAGE_THE_HALF_WAY_RESULT_CHAIN_RECORD_TITLE_INDICATE_Y = 60.0f;
const float RESULT_IMAGE_THE_HALF_WAY_RESULT_CHAIN_RECORD_TITLE_SCALE_RATE = 0.4f;
//chain record
const float RESULT_SCORE_THE_HALF_WAY_RESULT_CHAIN_RECORD_INDICATE_X = 500.0f;
const float RESULT_SCORE_THE_HALF_WAY_RESULT_CHAIN_RECORD_INDICATE_Y = 80.0f;
const float RESULT_SCORE_THE_HALF_WAY_RESULT_CHAIN_RECORD_INDICATE_SPACE_Y = 30.0f;
const float RESULT_SCORE_THE_HALF_WAY_RESULT_CHAIN_RECORD_SCALE_RATE = 0.3f;
const int RESULT_SCORE_THE_HALF_WAY_RESULT_CHAIN_RECORD_DIGIT = 3;
const int RESULT_SCORE_THE_HALF_WAY_RESULT_CHAIN_RECORD_DIRECTION_ROLLING_TIME = 150;
//ranking
const float RESULT_SCORE_THE_HALF_WAY_RESULT_RANKING_INDICATE_X = 50.0f;
const float RESULT_SCORE_THE_HALF_WAY_RESULT_RANKING_INDICATE_Y = 115.0f;
const float RESULT_SCORE_THE_HALF_WAY_RESULT_RANKING_SCALE_RATE = 0.4f;					//default scale rate
const float RESULT_SCORE_THE_HALF_WAY_RESULT_RANKING_THE_FIRST_SCALE_RATE = 0.6f;		//scale rate for the first
const float RESULT_SCORE_THE_HALF_WAY_RESULT_RANKING_INDICATE_SPACE_Y = 30.0f;
//setting score' bonus
const float RESULT_SCORE_THE_HALF_WAY_RESULT_BONUS_COMMON_INIDICATE_Y = 405.0f;			//each coordinateY
const float RESULT_SCORE_THE_HALF_WAY_RESULT_BONUS_COMMON_SCALE_RATE = 0.3f;			//each scale rate
const int	RESULT_SCORE_THE_HALF_WAY_RESULT_BONUS_COMMON_RECORD_DIGIT = 5;				//drawing digit
//setting bonus' image
const float RESULT_IMAGE_THE_HALF_WAY_RESULT_BONUS_COMMON_INIDICATE_Y = 415.0f;	//each coordinateY
const int RESULT_IMAGE_THE_HALF_WAY_RESULT_BONUS_COMMON_WIDTH = 320;			//common width
const int RESULT_IMAGE_THE_HALF_WAY_RESULT_BONUS_COMMON_HEIGHT = 64;			//common height
const float RESULT_IMAGE_THE_HALF_WAY_RESULT_BONUS_SCALE_RATE = 0.3f;			//bonus' scale rate
const float RESULT_IMAGE_THE_HALF_WAY_RESULT_TOTAL_BONUS_SCALE_RATE = 0.4f;		//total bonus' scale rate

//Each other files
//Japanese
//File 1
const float RESULT_INITIALIZE_FILE_1_INDICATE_X_IN_JAPANESE = 55.0f;
const float RESULT_INITIALIZE_FILE_1_INDICATE_Y_IN_JAPANESE = 100.0f;
const int RESULT_INITIALIZE_FILE_1_FONT_SIZE_IN_JAPANESE = 25;
const int RESULT_INITIALIZE_FILE_1_INDICATE_SPACE_Y_IN_JAPANESE = 5;
//File 2
const float RESULT_INITIALIZE_FILE_2_INDICATE_X_IN_JAPANESE = 30.0f;
const float RESULT_INITIALIZE_FILE_2_INDICATE_Y_IN_JAPANESE = 185.0f;
const int RESULT_INITIALIZE_FILE_2_FONT_SIZE_IN_JAPANESE = 25;
const int RESULT_INITIALIZE_FILE_2_INDICATE_SPACE_Y_IN_JAPANESE = 10;
//File 3
const float RESULT_INITIALIZE_FILE_3_INDICATE_X_IN_JAPANESE = 330.0f;
const float RESULT_INITIALIZE_FILE_3_INDICATE_Y_IN_JAPANESE = 185.0f;
const int RESULT_INITIALIZE_FILE_3_FONT_SIZE_IN_JAPANESE = 25;
const int RESULT_INITIALIZE_FILE_3_INDICATE_SPACE_Y_IN_JAPANESE = 10;

//English
//File 1
const float RESULT_INITIALIZE_FILE_1_INDICATE_X_IN_ENGLISH = 25.0f;
const float RESULT_INITIALIZE_FILE_1_INDICATE_Y_IN_ENGLISH = 100.0f;
const int RESULT_INITIALIZE_FILE_1_FONT_SIZE_IN_ENGLISH = 20;
const int RESULT_INITIALIZE_FILE_1_INDICATE_SPACE_Y_IN_ENGLISH = 20;
//File 2
const float RESULT_INITIALIZE_FILE_2_INDICATE_X_IN_ENGLISH = 20.0f;
const float RESULT_INITIALIZE_FILE_2_INDICATE_Y_IN_ENGLISH = 185.0f;
const int RESULT_INITIALIZE_FILE_2_FONT_SIZE_IN_ENGLISH = 18;
const int RESULT_INITIALIZE_FILE_2_INDICATE_SPACE_Y_IN_ENGLISH = 18;
//File 3
const float RESULT_INITIALIZE_FILE_3_INDICATE_X_IN_ENGLISH = 305.0f;
const float RESULT_INITIALIZE_FILE_3_INDICATE_Y_IN_ENGLISH = 185.0f;
const int RESULT_INITIALIZE_FILE_3_FONT_SIZE_IN_ENGLISH = 18;
const int RESULT_INITIALIZE_FILE_3_INDICATE_SPACE_Y_IN_ENGLISH = 18;
//if current stage is off=road, change technical point's indication
const float RESULT_INITIALIZE_FILE_3_INDICATE_X_IN_ENGLISH_AND_OFF_ROAD = 370.0f;

//▼関数の定義

//各ステージ共通
void LoadResultFile();				//ファイルの読み込み
void InitCommonResult( HWND hWnd );	//初期化
void UpdateResult();				//更新
void DrawResult();					//表示
void ReleaseResult();				//解放
/*
Initialize bonus, that score and image
*/
void	InitializeResultBonusForRanking(SCOREVALUE *lpScore, CHARACTER *lpCh);

//①海ステージ用
void InitSeaResult();				//初期化

//②ロードステージ用
void InitRoadResult();				//初期化

//③オフロード用
void InitOffRoadResult();			//初期化
/*
Set score direction and indication value for three stage's score
*/
void	SetResultStageIndicationValue(SCOREVALUE *lpScore, int recordNum, int material);
/*
Set file for the current language
*/
void	SetResultFixedPatternFile(TALK *lpTalk, tagRESULTFIXEDFILE fileNum);
/*
difference setting for drawing score value or fixed pattern file
*/
void	DeferScoreOrFixedFileSetting(SCOREVALUE *lpScore, TALK *lpTalk, tagSELECTSTAGE stageNum, int recordNum);
/*
indiacte present stage's record
*/
void	PresentStageResult();
/*
indicate the half way result
*/
void	StartingInitializerTheHalfWayResult();
/*
Update the half way result
*/
void	UpdateTheHalfWayResult();
/*
Initialize the half way result
*/
void InitializeTheHalfWayResult();
/*
subsutitute the half way value for global variable
*/
void	SetResultTheHalfWayValue(SCOREVALUE *lpScore, tagRECORDTHEHALFWAYITEM recordNum, int material, tagSCORECOLOR color);

#endif
