#ifndef		ENDING_H__
#define		ENDING_H__

//▼ヘッダーをインクルード
#include "graphics.h"

//▼ 最終結果スコア内容
enum tagENDTHERESULTSCORE{
	ENDING_SCORE_CHAIN,				//CHAIN
	ENDING_SCORE_TIME			,	//時間
	ENDING_SCORE_TOTAL_POINT	,	//集計ポイント
	ENDING_KIND_OF_SCOREVALUE	,	//表示するスコアの種類数
	ENDING_SCORE_AND_TITLE		,	//各種タイトル数
};

// ▼結果表示フラグ用
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
//▼ エンドクレジット用の画像要素番号
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

//▼ マクロの定義
const int ENDING_IMAGE_MAX = 6;						//画像枚数

//各種表示設定
//エンディングシーン画像
const int ENDING_IMAGE_END_SCENE_WIDTH	= 640;		//幅
const int ENDING_IMAGE_END_SCENE_HEIGHT = 4000;		//高さ

//最終結果背景画像
const int ENDING_IMAGE_END_RESULT_SCENE_WIDTH	= 640;		//幅
const int ENDING_IMAGE_END_RESULT_SCENE_HEIGHT	= 480;		//高さ

//各種結果表示文字列の設定
const int	ENDING_END_THE_RESULT_IMAGE_WDIHT			 = 320;		//幅
const int	ENDING_END_THE_RESULT_IMAGE_HEIGHT			 = 96;		//高さ
const float ENDING_END_THE_RESULT_IMAGE_INDICATE_SPACE_Y = 100.0f;	//表示間隔Y
const float ENDING_END_RESULT_IMAGE_INDICATE_X			 = 640.0f;	//表示先X
const float ENDING_END_RESULT_IMAGE_INDICATE_Y			 = 10.0f;	//表示先Y
const float ENDING_END_RESULT_IMAGE_MOVE_X				 = -10.0f;	//移動量X
const float ENDING_END_RESULT_IMAGE_DEFAULT_SCALE		 = 0.75f;	//拡大倍率

//記録表示設定
//時間
const float ENDING_END_THE_RESULT_TIME_DEFAULT_SCALE		= 0.5f;	//拡大倍率
const int	ENDING_THE_RESULT_TIME_DIGIT					= 2;	//桁数
//集計ポイント
const float ENDING_END_THE_RESULT_TOTAL_POINT_DEFAULT_SCALE = 0.75f;//拡大倍率
const int	ENDING_THE_RESULT_TOTAL_POINT_DIGIT				= 6;	//桁数

//クレジット用画像設定
//クレジット画面スクロール速度
const float ENDING_END_THE_CREDIT_SCROLL_SPEED_Y		 = 1.0f;
const int	ENDING_END_THE_CREDIT_IMAGE_WDIHT			 = 640;		//幅
const int	ENDING_END_THE_CREDIT_IMAGE_HEIGHT			 = 96;		//高さ
const float ENDING_END_THE_CREDIT_IMAGE_INDICATE_SPACE_Y = 370.0f;	//表示間隔Y
const float ENDING_END_CREDIT_IMAGE_INDICATE_X			 = 640.0f;	//表示先X
const float ENDING_END_CREDIT_IMAGE_ABSOLUTE_MOVE_X		 = 5.5f;	//移動量X（絶対値）
const float ENDING_END_CREDIT_IMAGE_DEFAULT_SCALE		 = 0.75f;	//拡大倍率

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


//▼関数宣言
void InitEnding(HWND hWnd);		//初期化
void UpdateEnding();			//更新
void DrawEnding();				//表示
void ReleaseEnding();			//解放
/*
*************************************************************************
ファイルの表示更新処理
*************************************************************************
*/
void	UpdateCreateFile();
/*
*************************************************************************
最終結果表示更新処理
*************************************************************************
*/
void	UpdateEndTheResult();
/*
*************************************************************************
エンディング画像スクロール処理
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