#ifndef		SELECTSTAGE_H__
#define		SELECTSTAGE_H__

//▼列挙型の定義
//ステージの種類
enum tagSELECTSTAGE{

	STAGE_VACANT = -1	,
	STAGE_SEA					,	//海のステージ
	STAGE_ROAD					,   //陸上のステージ
	STAGE_OFF_ROAD				,	//オフロードステージ
	STAGE_MAX					,	//ステージの総数
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

//▼ヘッダーをインクルード
#include "graphics.h"

//タイプ宣言
typedef unsigned char COMPLETESTAGE;

//▼ マクロの定義
//クリア済みステージのビット目
const COMPLETESTAGE	SELECTSTAGE_STAGE_NOT_COMPLETE	 = 0;
const COMPLETESTAGE	SELECTSTAGE_STAGE_COMPLETE		 = 1;

const unsigned int SELECTSTAGE_IMAGE_MAX = 10;		//画像の枚数
const unsigned int SELECTSTAGE_KIND_OF_IMAGE = 7;	//画像の種類

//効果音設定
const unsigned int SELECTSTAGE_EFFECT_SOUND_MAX = 3;//効果音数


//画像設定
//背景
const unsigned int SELECTSTAGE_BACKGROUND_IMAGE_WIDTH	= 640;		//画像の幅
const unsigned int SELECTSTAGE_BACKGROUND_IMAGE_HEIGHT	= 480;		//画像の高さ

//説明画面
const unsigned int SELECTSTAGE_BRIEF_IMAGE_WIDTH	= 640;		//画像の幅
const unsigned int SELECTSTAGE_BRIEF_IMAGE_HEIGHT	= 480;		//画像の高さ
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

//ステージ選択文字列
const unsigned int SELECTSTAGE_STAGE_TOP_TITLE_IMAGE_WIDTH	= 445;		//画像の幅
//各種ステージ文字列イメージ
const unsigned int SELECTSTAGE_STAGE_TITLE_IMAGE_WIDTH		= 320;		//画像の幅
const unsigned int SELECTSTAGE_STAGE_TITLE_IMAGE_HEIGHT		= 96;		//画像の高さ
const  float SELECTSTAGE_STAGE_TITLE_IMAGE_INDICATE_Y		= 120.0f;	//表示先Y
const  float SELECTSTAGE_STAGE_TITLE_IMAGE_INDICATE_SPACE_X	= 20.0f;	//表示先X表示間隔
const  float SELECTSTAGE_STAGE_TITLE_IMAGE_DEFAULT_SCALE	= 0.5f;		//拡縮倍率
//ステージ選択中で扱う画像の種類（カーソル以外）
const unsigned int SELECTSTAGE_SCENE_SELECT_STAGE_KIND_OF_IMAGE = 5;	//画像の種類

//カーソル
const  float SELECTSTAGE_CURSOR_DEFAULT_ALPHA	= 0.5f;					//透過倍率
//決定、取り消しを受付けない時間
const int	SELECTSTAGE_CURSOR_DONT_ACCEPT_CONTROL_TIME = 20;


//▼関数宣言
void	InitSelectStage( HWND hWnd );
void	UpdateSelectStage();
void	DrawSelectStage();
void	ReleaseSelectStage();
//クリア済ステージを設定する
void	SetCompleteStage( COMPLETESTAGE complete , tagSELECTSTAGE stage );
//クリア済みステージを取得
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