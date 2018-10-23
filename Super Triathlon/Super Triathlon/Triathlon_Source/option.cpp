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

//▼グローバル変数

//オプション項目
CHARACTER	g_OptionFunction[OPTION_KIND_OF_FUNCTION] = {0};

//ステージセレクト画像
CHARACTER	g_OptionSelectStage[ STAGE_MAX ] = {0};

//最終結果表示画像
CHARACTER	g_OptionEndTheResult[ENDING_SCORE_AND_TITLE] = { 0 };

//カーソル
CHARACTER	g_OptionCursor = {0};

//画像を扱う
int		g_OptionImg[OPTION_IMAGE_MAX] = {0};

//ファイルを扱う
TALK	g_OptionFile[OPTION_FILE_MAX] = {0};

//オプションの遷移フラグ
tagOPTIONTRANSITION	g_OPtionTransitionFlagNext = OPTION_TRANSITION_VACANT;
tagOPTIONTRANSITION	g_OPtionTransitionFlagNow = OPTION_TRANSITION_VACANT;

//閲覧するステージ番号
tagSELECTSTAGE	 g_OptionRecordView = STAGE_VACANT;

//効果音の要素を扱う
int		g_OptionEffectSound[OPTION_EFFECT_SOUND_MAX] = {0};

//記録閲覧フラグ(ステージ選択時)
bool	g_OptionViewStageRecordFlag = false;
//最終結果閲覧フラグ
bool	g_OptionViewEndRecordFlag = false;

//記録表示用
SCOREVALUE	g_OptionScore[RECORD_MAX][RECORD_MAX] = { 0 };		//ステージ用
SCOREVALUE	g_OptionEndScore[ENDING_KIND_OF_SCOREVALUE] = { 0 };//最終結果用

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
//初期化
//************************************************************************************************
void InitOption( HWND hWnd ){
	
	//使用する画像
	char	*lpImageTbl[ OPTION_IMAGE_MAX ] = {
		
		{ "Image\\option.bmp" }			,			//OPTION画像
		{ "Image\\OptionItem.bmp" }		,			//オプション項目
		{ "Image\\SelectStage.bmp" }	,			//ステージセレクト画像
		{ "Image\\cursor.bmp" }			,			//カーソル画像
		{ "Image\\EndTheResult.bmp" }	,			//最終結果文字列画像
		{ "Image\\select.bmp"},						//image yes no
		{ "Image\\Language.bmp" },					//using language
		{ "Image\\OpeningButton.bmp"},				//game's level image
	};
	//使用する効果音データ
	char	*lpSoundTbl[ OPTION_EFFECT_SOUND_MAX ] = {

		{ "SE\\cursor.wav" }	,				//カーソル音
		{ "SE\\ok.wav" }		,				//決定音
		{ "SE\\cancel.wav" }	,				//キャンセル音	
	};

	//▼ 画像の読み込み
	for( int i = 0 ; i < OPTION_IMAGE_MAX ; i++ ){ 
		g_OptionImg[i] = CreateDDImage( lpImageTbl[i] , hWnd );
	}

	//▼ 音データの読み込み
	for( int i = 0 ; i < OPTION_EFFECT_SOUND_MAX ; i++ ){ 
		g_OptionEffectSound[i] = CreateSound( lpSoundTbl[i] );
	}

	//▼ 曲の読み込み
	CreateStreaming("BGM\\option.wav");

	//▼ スコアの初期化
	InitScore( hWnd );

	//▼ 各種閲覧フラグ
	//閲覧項目
	g_OptionRecordView = STAGE_VACANT;
	//記録閲覧フラグ(ステージ選択時)
	g_OptionViewStageRecordFlag = false;
	//最終結果閲覧フラグ
	g_OptionViewEndRecordFlag = false;
	//オプションの機能選択フラグ
	g_OPtionTransitionFlagNext = OPTION_TRANSITION_SELECTION;
	g_OPtionTransitionFlagNow = OPTION_TRANSITION_SELECTION;
	//Reset record flag
	g_OptionResetRecordFlag = OPTION_DONT_RESET;
	//change flag of current language for explanation
	g_OptionLanguageFlag = OPTION_DONT_CHANGE_LANGUAGE;
	//kind of language during selection
	g_OptionLanguageNow = GAME_LANGUAGE_JAPANESE;


	//▼ 各種画像設定
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


	//▼ 記録閲覧可能ステージ
	//画像の表示サイズを計算
	//高さ
	float	SelectSizeH = OPTION_DEFAULT_SELECT_IMAGE_SCALE * SELECTSTAGE_STAGE_TITLE_IMAGE_HEIGHT;
	//幅
	float	SelectSizeW = OPTION_DEFAULT_SELECT_IMAGE_SCALE * SELECTSTAGE_STAGE_TITLE_IMAGE_WIDTH;
	//各種代入
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

	//カーソルの設定
	g_OptionCursor.x = g_OptionFunction[0].x;
	g_OptionCursor.y = g_OptionFunction[0].y;
	g_OptionCursor.w = (int)(g_OptionFunction[0].w * g_OptionFunction[0].scale);
	g_OptionCursor.h = (int)(g_OptionFunction[0].h * g_OptionFunction[0].scale);
	g_OptionCursor.alpha = 120.0f;
	g_OptionCursor.flag = true;
	//move
	g_OptionCursor.my = (imageHeight2 * OPTION_FUNCTION_RECORD_INDICATE_SPACE_Y);

	//各種最終結果文字列表示用設定
	//picY
	int picYTbl[ENDING_SCORE_AND_TITLE] = {
		0,															//title
		ENDING_END_THE_RESULT_IMAGE_HEIGHT * 3,						//chain
		ENDING_END_THE_RESULT_IMAGE_HEIGHT,							//time
		ENDING_END_THE_RESULT_IMAGE_HEIGHT * 2,						//total
	};
	for (int i = 0; i < ENDING_SCORE_AND_TITLE; i++) {
		g_OptionEndTheResult[i].x = 
			CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(ENDING_END_THE_RESULT_IMAGE_WDIHT * ENDING_END_RESULT_IMAGE_DEFAULT_SCALE);		//表示先X
		g_OptionEndTheResult[i].y = ENDING_END_RESULT_IMAGE_INDICATE_Y +
			(ENDING_END_THE_RESULT_IMAGE_INDICATE_SPACE_Y * i);								//表示先Y
		g_OptionEndTheResult[i].mx = ENDING_END_RESULT_IMAGE_MOVE_X - (i * 1.0f);			//移動量X
		g_OptionEndTheResult[i].h = ENDING_END_THE_RESULT_IMAGE_HEIGHT;						//高さ
		g_OptionEndTheResult[i].w = ENDING_END_THE_RESULT_IMAGE_WDIHT;						//幅
		g_OptionEndTheResult[i].scale = ENDING_END_RESULT_IMAGE_DEFAULT_SCALE;				//拡縮倍率
		g_OptionEndTheResult[i].picy = picYTbl[i];											//表示元Y
		g_OptionEndTheResult[i].flag = false;												//表示フラグ
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
		g_OptionEndScore[i].spaceX = 10.0f;												//表示間隔X
		g_OptionEndScore[i].spaceY = 0.0f;												//表示間隔Y
		g_OptionEndScore[i].score_scale = ENDING_END_THE_RESULT_TIME_DEFAULT_SCALE;		//拡縮率
		g_OptionEndScore[i].scoreColor = SCORE_BLUE;										//表示色
		g_OptionEndScore[i].ch.type = SCORE_NUMBER_TYPE_NORMAL;							//スコアの表示種類
		g_OptionEndScore[i].ti.cntFlag = false;											//時間経過フラグ
		g_OptionEndScore[i].digit = digitTbl[i];											//桁の数
		g_OptionEndScore[i].gradually = true;												//徐々に加算・減算フラグ
		g_OptionEndScore[i].graduallyDirect = directionTbl[i];							//スコアの演出
		g_OptionEndScore[i].rollTime = rollingTimeTbl[i];									//回転時間
		g_OptionEndScore[i].renewalColor = SCORE_YELOW;									//更新時の変更色
		g_OptionEndScore[ENDING_SCORE_TOTAL_POINT].numD = 0;								//始めに表示する数値
		g_OptionEndScore[ENDING_SCORE_TOTAL_POINT].interval = intervalTbl[i];				//表示間隔
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
//更新
//************************************************************************************************
void UpdateOption(){
	
	//曲の再生
	PlayStreaming( true );

	//カーソルの挙動（移動と移動制限）
	if (g_OptionCursor.flag) OptionCursorMove(); ConstrainOptionCursor();

	//▼ 機能により分岐
	if (g_OPtionTransitionFlagNow == g_OPtionTransitionFlagNext) {
		switch (g_OPtionTransitionFlagNow) {

			//機能選択時
		case OPTION_TRANSITION_SELECTION:
			//▼モード選択
			OptionSelectFunction();
			break;

			//記録閲覧選択時
		case OPTION_TRANSITION_SELECT_RECORD:
			//記録閲覧
			OptionSelectStageRecord();									//各種ステージ
			if (g_OptionEndTheResult[0].flag) OptionEndRecordView();	//最終結果
			//記録を閲覧をキャンセル
			OptionCancelRecordView();
			break;

			//記録閲覧中
		case OPTION_TRANSITION_DURING_RECORD_VIEW:
			//▼　最高記録を表示(ステージ選択時)
			if (g_OptionViewStageRecordFlag) {
				//dont draw ent the result image
				g_OptionEndTheResult[0].flag = false;
				//最後のファイルまで読み込んでいなかったら読み込み
				if (g_OptionFile[OPTION_FILE_MAX - 1].fRead == false) {
					//読み込みたいファイル数までループを回す
					for (int i = 2; i < OPTION_FILE_MAX; i++) {
						//各ファイルを順番に読み込んでいく
						if (g_OptionFile[i].fRead == false) {
							//文字列生成
							CreateTalk(&g_OptionFile[i]);
							//読み込んだフラグをtrue
							g_OptionFile[i].fRead = true;
						}
					}
				}
			}
			//最終結果閲覧時
			if (g_OptionViewEndRecordFlag) {
				//各種文字列画像の表示
				for (int i = 0; i < ENDING_SCORE_AND_TITLE; i++) { g_OptionEndTheResult[i].flag = true; };
			}
			//ステージ記録閲覧終了して選択へ
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
//表示
//************************************************************************************************
void DrawOption(){

	//スコア表示関数の分岐
	void (*pscore[RECORD_MAX])(SCOREVALUE*) = {

		DrawScore	,			//敵表示
		DrawScore	,			//アイテム表示
		DrawTime	,			//時間表示
		DrawScore	,			//CHAIN数
		DrawScore	,			//集計ポイント
	};

	//扱う各項目の数
	//①海ステージ：敵討伐、獲得アイテム、時間、敵討伐CHAIN数、集計ポイントの順番
	//②道路ステージ：破壊した障害物、飛んだハードル、時間、ハードル跳躍CHAIN数、集計ポイントの順番
	//③オフロードステージ：成功したアクション、合計アクションPt、時間、連続アクション数、集計ポイントの順番
	char ScoreItemTbl[ STAGE_MAX ][ RECORD_MAX ] = {

		{ 5 , 5 , 1 , 1 , 1 } ,			//海

		{ 2 , 3 , 1 , 1 , 2 } ,			//道路

		{ 4 , 1 , 1 , 1 , 1 } ,			//オフロード

	};


	//▼画像の表示
	//背景画像
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

	//各種ステージセレクト画像の表示
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

	//各種最終結果文字列画像
	for (int i = 0; i < ENDING_SCORE_AND_TITLE; i++)
	{
		if (g_OptionEndTheResult[i].flag)
		{
			DDScaleBltSelectIndicate(
				g_OptionEndTheResult[i].x,			//表示座標X
				g_OptionEndTheResult[i].y,			//表示座標Ｙ
				g_OptionEndTheResult[i].w,			//幅
				g_OptionEndTheResult[i].h,			//高さ
				g_OptionImg[4],						//画像の要素番号
				g_OptionEndTheResult[i].picx,		//表示元X
				g_OptionEndTheResult[i].picy,		//表示元Y
				g_OptionEndTheResult[i].scale,		//拡縮倍率
				DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP);
		}
	}

	//▼各項目の値を表示
	//ステージの記録
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
				//スコア表示の分岐
				pscore[j](&g_OptionScore[j][i]);
			}
		}
	}
	//最終結果の記録
	if (g_OptionViewEndRecordFlag) {
		//chain
		DrawScore(&g_OptionEndScore[ENDING_SCORE_CHAIN]);
		//時間
		DrawTime(&g_OptionEndScore[ENDING_SCORE_TIME]);
		//集計ポイント
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

	//カーソル
	if (g_OptionCursor.flag) {
		//カーソル
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
//解放
//************************************************************************************************
void ReleaseOption(){
	
	
	//画像の解放
	for( int i = 0 ; i < OPTION_IMAGE_MAX ; i++ ){ ReleaseDDImage( g_OptionImg[i] ); }
	//ファイルの解放
	ZeroMemory( g_OptionFile , sizeof( TALK ) * OPTION_FILE_MAX );

	//効果音の解放
	for( int i = 0 ; i < OPTION_EFFECT_SOUND_MAX ; i++ ){ ReleaseSound(g_OptionEffectSound[i]); }
	//曲の解放
	ReleaseStreaming();
	//スコアの解放
	ReleaseScore();
	//読み込んだファイルの解放
	ReleaseTalk();
}


/*
************************************************************************************************
ファイルの読み込み
⇒引数のRECORD型、TALK型に読み込みたいファイルの値とファイルパスを代入する
************************************************************************************************
*/
void	LoadOptionStageFile( RECORD *lpRecord , TALK *lpTalk , char stageNum , tagRENEWAL recordType ){

	//get the current language
	tagGAMELANGUAGE language = GetGameLanguage();
	//▼ 各ステージの読み込みファイルデータテーブル
	//for Japanese language
	char *lpJapaneseFileTbl[ STAGE_MAX ][ OPTION_FILE_MAX ] = {
		
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
	char *lpEnglishFileTbl[STAGE_MAX][OPTION_FILE_MAX] = {

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
	//Set fixed pattern file
	for (int i = 2; i < (RESULT_KIND_OF_FIXED_PATTERN_FILE + 2); i++) {
		SetResultFixedPatternFile(&lpTalk[i], fixedFile[i - 2]);
	}

	//閲覧したいステージのファイル文字列をコピー
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
	//閲覧するファイルのデータを取得する
	*( lpRecord ) = *( LoadRecordFile( lpJapaneseFileTbl[ stageNum ][ recordType ] , recordType ) );
}

/*
************************************************************************************************
カーソルの移動
************************************************************************************************
*/
void	OptionCursorMove(){
	
	//上
	if (JustKey(VK_UP) || JustLever(LEVER_UP)) {
		//カーソル音
		PlaySound(g_OptionEffectSound[0], 0, false);
		g_OptionCursor.y -= g_OptionCursor.my;
		//if subsutituted flag of changing language for gloabal variable, subsutute flag of not changing language for 
		if(g_OptionLanguageFlag == OPTION_CHANGE_LANGUAGE) g_OptionLanguageFlag = OPTION_DONT_CHANGE_LANGUAGE; 	
	}
	//下
	if (JustKey(VK_DOWN) || JustLever(LEVER_DOWN)) {
		//カーソル音
		PlaySound(g_OptionEffectSound[0], 0, false);
		g_OptionCursor.y += g_OptionCursor.my;
		//if subsutituted flag of changing language for gloabal variable, subsutute flag of not changing language for 
		if (g_OptionLanguageFlag == OPTION_CHANGE_LANGUAGE) g_OptionLanguageFlag = OPTION_DONT_CHANGE_LANGUAGE;
	}
}

/*
************************************************************************************************
カーソルの移動制限
************************************************************************************************
*/
void	ConstrainOptionCursor(){
	
	//上の項目なら下の項目に
	if (g_OptionCursor.y < g_constrainCursorMoveTop) {
		g_OptionCursor.y = g_constrainCursorMoveBottom;
	}
	//下の項目なら上の項目に
	if (g_OptionCursor.y > g_constrainCursorMoveBottom) {
		g_OptionCursor.y = g_constrainCursorMoveTop;
	}
}


/*
************************************************************************************************
ステージ記録閲覧処理
************************************************************************************************
*/
void	OptionSelectStageRecord(){

	//Each stage's items
	int scoreItemTbl[STAGE_MAX][RECORD_MAX] = {

		{ 5 , 5 , 1 , 1 , 1 } ,			//Sea
		{ 2 , 3 , 1 , 1 , 1 } ,			//Road
		{ 4 , 1 , 1 , 1 , 1 } ,			//Off-road
	};
	//▼ stage's number
	tagSELECTSTAGE	StageTbl[ STAGE_MAX ] = {
		STAGE_SEA		,
		STAGE_ROAD		,
		STAGE_OFF_ROAD	,
	};
	
	//▼ 各ステージの記録を扱う要素
	tagRECORDMATRIX recordTypeTbl[ RECORD_MAX ] = {

		RECORD_ONE			,				//要素1
		RECORD_TWO			,				//要素2
		RECORD_TIME			,				//時間の要素
		RECORD_FOUR			,				//要素4
		RECORD_TOTAL		,				//集計ポイントの要素
	};

	//cursor move
	g_OptionCursor.my = ( g_OptionSelectStage[0].h * 
		g_OptionSelectStage[0].scale ) + OPTION_SELECT_STAGE_INDICATE_SPACE_Y;
	//cursor constrain move
	g_constrainCursorMoveTop = g_OptionSelectStage[0].y;
	g_constrainCursorMoveBottom = g_OptionEndTheResult[0].y;
	//draw cursor
	g_OptionCursor.flag = true;
	//各種ステージの表示
	for (int i = 0; i < STAGE_MAX; i++) { g_OptionSelectStage[i].flag = true; }
	//最終結果文字列の表示
	g_OptionEndTheResult[0].flag = true;

	//各種ステージの選択
	for( int i = 0 ; i < STAGE_MAX ; i++ ){
		//表示中なら
		if( g_OptionSelectStage[i].flag ){

			//選択 閲覧出来るのは各種ステージと最終結果
			//各種ステージ
			if( g_OptionCursor.y == g_OptionSelectStage[i].y ){
				//サイズを合わせる
				g_OptionCursor.w = (int)( g_OptionSelectStage[i].w * g_OptionSelectStage[i].scale );
				g_OptionCursor.h = (int)( g_OptionSelectStage[i].h * g_OptionSelectStage[i].scale) ;

				//決定
				if( JustKey( 'Z' ) || JustButton( BUTTON_1 ) ){
					//決定音
					PlaySound( g_OptionEffectSound[1] , 0 , false );
					//カーソルを消去
					g_OptionCursor.flag = false;
					//記録を表示する値を代入
					g_OptionRecordView = StageTbl[i];
					
					//▼ 記録閲覧するファイルの読み込み
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
					//閲覧可能フラグ
					g_OptionViewStageRecordFlag = true;
					//閲覧遷移フラグ
					g_OPtionTransitionFlagNext = OPTION_TRANSITION_DURING_RECORD_VIEW;

					//ステージセレクト画像を非表示に
					for( int i = 0 ; i < STAGE_MAX ; i++ ){ g_OptionSelectStage[i].flag = false; }
					//最終結果文字列を非表示に
					g_OptionEndTheResult[0].flag = false;

					//選択されたステージ文字列の画像の表示先y座標を変更
					g_OptionSelectStage[i].y = ENDING_END_RESULT_IMAGE_INDICATE_Y;
					//表示フラグ
					g_OptionSelectStage[i].flag = true;
					return;
				}
			}
		}
	}
}

/*
最終結果閲覧
*/
void	OptionEndRecordView() {
	
	//最終結果
	if (g_OptionCursor.y == g_OptionEndTheResult[0].y) {

		//サイズを合わせる
		g_OptionCursor.w = (int)(g_OptionEndTheResult[0].w * g_OptionEndTheResult[0].scale);
		g_OptionCursor.h = (int)(g_OptionEndTheResult[0].h * g_OptionEndTheResult[0].scale);

		//決定
		if (JustKey('Z') || JustButton(BUTTON_1)) {
			//決定音
			PlaySound(g_OptionEffectSound[1], 0, false);
			//カーソルを消去
			g_OptionCursor.flag = false;
			//ステージセレクト画像を非表示に
			for (int i = 0; i < STAGE_MAX; i++) { g_OptionSelectStage[i].flag = false; }
			//最終結果文字列を非表示
			g_OptionEndTheResult[0].flag = false;
			//最終結果文字列の座標を変更する
			g_OptionEndTheResult[0].y = ENDING_END_RESULT_IMAGE_INDICATE_Y;

			//過去に記録した最高記録の読み込み
			g_OptionEndScore[0].re = *(LoadRecordFile("Data\\EndTheResult\\Best.dat", RECORD_BEST));
			//取得した値を表示用に設定する
			//CHAIN
			g_OptionEndScore[ENDING_SCORE_CHAIN].num = g_OptionEndScore[0].re.recordBox[ENDING_SCORE_CHAIN][0];
			//時間の記録
			g_OptionEndScore[ENDING_SCORE_TIME].ti.minutes = g_OptionEndScore[0].re.recordBox[ENDING_SCORE_TIME][0];			//分
			g_OptionEndScore[ENDING_SCORE_TIME].ti.second = g_OptionEndScore[0].re.recordBox[ENDING_SCORE_TIME][1];				//秒
			g_OptionEndScore[ENDING_SCORE_TIME].ti.millisecond = g_OptionEndScore[0].re.recordBox[ENDING_SCORE_TIME][2];		//ミリ秒
			//集計ポイントの記録
			g_OptionEndScore[ENDING_SCORE_TOTAL_POINT].num = (float)(g_OptionEndScore[0].re.recordBox[ENDING_SCORE_TOTAL_POINT][0]);

			//記録閲覧可フラグ
			g_OptionViewEndRecordFlag = true;
			//機能遷移フラグ
			g_OPtionTransitionFlagNext = OPTION_TRANSITION_DURING_RECORD_VIEW;

		}
	}

}

/*
記録閲覧をキャンセル
*/
void	OptionCancelRecordView() {

	//キャンセル
	if (JustKey('X') || JustButton(BUTTON_2)) {
		//キャンセル音
		PlaySound(g_OptionEffectSound[2], 0, false);
		//各種ステージ画像の非表示
		for (int i = 0; i < STAGE_MAX; i++) { g_OptionSelectStage[i].flag = false; }
		//最終結果文字列を非表示
		g_OptionEndTheResult[0].flag = false;
		//dont draw cursor
		g_OptionCursor.flag = false;
		//カーソルを一番上に
		g_OptionCursor.x = g_OptionFunction[OPTION_FUNCTION_RECORD].x;
		g_OptionCursor.y = g_OptionFunction[OPTION_FUNCTION_RECORD].y;
		//移動制限
		g_constrainCursorMoveTop = g_OptionFunction[OPTION_FUNCTION_RECORD].y;
		g_constrainCursorMoveBottom = g_OptionFunction[OPTION_FUNCTION_RANKING].y;
		//機能選択フラグへ
		g_OPtionTransitionFlagNext = OPTION_TRANSITION_SELECTION;
	}
}
/*
************************************************************************************************
スコア表示用の設定
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
スコア表示に値を設定
****************************************************************************************
*/
void	SetOptionStageScoreForDraw( SCOREVALUE *lpScore , RECORD *lpRecord , char stageNum , tagRECORDMATRIX itemType ){
	
	
	//扱う各項目の数
	//①海ステージ：敵討伐、獲得アイテム、時間、敵討伐CHAIN数、集計ポイントの順番
	//②道路ステージ：破壊した障害物、飛んだハードル、時間、ハードル跳躍CHAIN数、集計ポイントの順番
	//③オフロードステージ：成功したアクション、合計アクションPt、時間、連続アクション数、集計ポイントの順番
	char ScoreItemTbl[ STAGE_MAX ][ RECORD_MAX ] = {

		{ 5 , 5 , 1 , 1 , 1 } ,			//海

		{ 2 , 3 , 1 , 1 , 2 } ,			//道路

		{ 4 , 1 , 1 , 1 , 1 } ,			//オフロード

	};

	//▼ 表示用の値を代入
	for( int i = 0 ; i < ScoreItemTbl[ stageNum ][ itemType ] ; i++ ){
		//時間の項目
		if( itemType == RECORD_TIME ){ 
			lpScore->ti.minutes = lpRecord->recordBox[ RECORD_TIME ][0];	//分
			lpScore->ti.second = lpRecord->recordBox[ RECORD_TIME ][1];		//秒
			lpScore->ti.millisecond = lpRecord->recordBox[ RECORD_TIME ][2];	//ミリ秒
			break;
		}
		//時間以外の項目
		lpScore[i].num = (float)lpRecord->recordBox[ itemType ][i];
	}

}

/*
オプション機能の選択
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
			//幅を合わせる
			g_OptionCursor.w = (int)(g_OptionFunction[i].w * g_OptionFunction[i].scale);
			//高さを合わせる
			g_OptionCursor.h = (int)(g_OptionFunction[i].h * g_OptionFunction[i].scale);
			if (JustUpKey('Z') || JustUpButton(BUTTON_1)) {
				//決定音
				PlaySound(g_OptionEffectSound[1], 0, false);
				//過去の記録を閲覧できるモードに遷移
				//画像を消去
				for (int i = 0; i < OPTION_KIND_OF_FUNCTION; i++) { g_OptionFunction[i].flag = false; }
				//カーソルを合わせる
				g_OptionCursor.x = CursorPos[i].fX;
				g_OptionCursor.y = CursorPos[i].fY;
				//dont draw cursor
				g_OptionCursor.flag = false;
				//遷移フラグ
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
	//キャンセルでオープニングへ
	if (JustButton(BUTTON_2) || JustKey('X')) {
		//キャンセル音
		PlaySound(g_OptionEffectSound[2], 0, false);
		//タイトル画面へ遷移
		CreateWipe(SCENE_OPENING, CHANGE_SCENE, WIPE_CONTRACTION);
	}


}

/*
記録閲覧からステージ選択画面への遷移
*/
void	OptionRecordViewToSelectRecordView(){

	//キャンセルでステージ選択へ
	if (JustButton(BUTTON_2) || JustKey('X')) {
		//キャンセル音
		PlaySound(g_OptionEffectSound[2], 0, false);
		//読み込んだファイルの解放
		ReleaseTalk();
		//読み込んだファイルのフラグをfalseに
		for (int i = 2; i < OPTION_FILE_MAX; i++) { g_OptionFile[i].fRead = false; }
		//各種文字列画像の非表示
		//ステージ
		g_OptionSelectStage[g_OptionRecordView].flag = false;
		//最終結果
		for (int i = 0; i < ENDING_SCORE_AND_TITLE; i++) { g_OptionEndTheResult[i].flag = false; };

		//表示数値をリセット
		//ステージ
		for (int i = 0; i < RECORD_MAX; i++) {
			for (int j = 0; j < RECORD_MAX; j++) {
				g_OptionScore[i][j].directEnd = false;
				g_OptionScore[i][j].numD = 0;
				g_OptionScore[i][j].ch.time = 0;
			}
		}
		//最終結果
		for (int i = 0; i < ENDING_SCORE_AND_TITLE; i++) {
			g_OptionEndScore[i].directEnd = false;
			g_OptionEndScore[i].numD = 0; 
		}

		//変更したステージ文字列の表示先Yを変更する
		g_OptionSelectStage[g_OptionRecordView].y = OPTION_SELECT_STAGE_INDICATE_Y +
			((OPTION_DEFAULT_SELECT_IMAGE_SCALE * SELECTSTAGE_STAGE_TITLE_IMAGE_HEIGHT) +
				OPTION_SELECT_STAGE_INDICATE_SPACE_Y) * g_OptionRecordView;

		//最終結果文字列の座標を変更
		g_OptionEndTheResult[0].y = g_OptionSelectStage[2].y + ((
			g_OptionSelectStage[2].h * g_OptionSelectStage[2].scale) + OPTION_SELECT_STAGE_INDICATE_SPACE_Y);
		//カーソルを一番上に
		g_OptionCursor.x = g_OptionSelectStage[0].x;
		g_OptionCursor.y = g_OptionSelectStage[0].y;
		//読み込むステージをリセット
		g_OptionRecordView = STAGE_VACANT;
		//記録閲覧可能フラグ
		g_OptionViewStageRecordFlag = false;		//ステージ
		g_OptionViewEndRecordFlag = false;			//最終結果
		//記録閲覧遷移フラグ
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
		{"過去に記録した最高記録を閲覧できます"},
		{"過去に記録した最高記録を消去します"},
		{"現在の言語設定を変更できます"},
		{"最新の順位を閲覧できます"}
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
		{ "過去に記録した最高記録を消去しますか？" },
		{ "過去の最高記録を消去しました" },
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
		{ "日本語" },
		{ "英語" },
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
			wsprintf(lpMes, "現在の言語設定を%sに変更しますか？", lpJapanese[g_OptionLanguageNow]);
		}
		//if change current language
		if (g_OptionLanguageFlag == OPTION_CHANGE_LANGUAGE) {
			wsprintf(lpMes, "現在の言語設定を%sに変更しました", lpJapanese[g_OptionLanguageNow]);
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