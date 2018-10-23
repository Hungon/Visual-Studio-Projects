#ifndef __OPENING_H__
#define __OPENING_H__

//▼ 列挙型の定義
enum	tagOPENINGSCENEFLAG{
	OPENING_SCENE_TITLE_LOGO_DIRECTION ,			//タイトルロゴ演出中
	OPENING_SCENE_SELECT_START_FUNCTION ,			//ゲームスタートか、オプションの選択
	OPENING_SCENE_SELECT_LEVEL_FUNCTION ,			//ゲーム中のレベル設定
};

//▼マクロの定義
#define OPENING_IMAGE_MAX		6		//使用画像枚数
//音用
#define OPENING_SOUND_EFFECT_MAX 3		//使用効果音の数

//操作制限時間
const int OPENING_CURSOR_DONT_ACCEPT_CONTROL_TIME = 10;

//タイトル用画像表示設定
//タイトルロゴの設定
const int OPENING_TITLE_LOGO_KIND_OF_IMAGE	= 7;			//画像の種類
const int OPENING_TITLE_STAGE_IMAGING_KIND_OF_IMAGE	= 3;	//各種ステージイメージ画像の種類
const int OPENING_IMAGE_TITLE_LOGO_WIDTH	= 600;			//幅
const int OPENING_IMAGE_TITLE_LOGO_HEIGHT	= 120;			//高さ
//表示先
//始点
const float OPENING_TITLE_LOGO_BIGIN_INDICATE_X	= 840.0f;		//X
const float OPENING_TITLE_LOGO_BIGIN_INDICATE_Y	= -400.0f;		//Y
//ベジェ曲線の要素数
const int	OPENING_TITLE_LOGO_SET_BEZIER = 4;
//終点
const float OPENING_TITLE_LOGO_TERMINATE_INDICATE_Y	= 50.0f;	//Y
//移動量
const float OPENING_TITLE_LOGO_MOVE_Y		= 5.0f;				//Y
//タイトルロゴ背景の図形の表示速度
const int	OPENING_TITLE_LOGO_FIGURE_DRAWING_SPEED = 10;

//各種ステージイメージ画像の設定
const float OPENING_STAGE_IMAGE_CHARACTER_INDICATE_X = 800.0f;
const float OPENING_STAGE_IMAGE_CHARACTER_INDICATE_Y = 320.0f;
//終点
const float OPENING_STAGE_IMAGE_CHARACTER_TERMINATE_INDICATE_X	= 90.0f;	//X
//表示間隔
const float OPENING_STAGE_IMAGE_CHARACTER_SPACE_X	= 10.0f;				//X
//移動量
const float OPENING_STAGE_IMAGE_CHARACTER_MOVE_X	= -9.0f;				//X
//拡大率
const float	OPENING_STAGE_IMAGE_CHARACTER_SCALE		= 0.75f;


//各種ボタン画像の設定
const int OPENING_BUTTON_IMAGE_WIDTH	= 128;		//幅
const int OPENING_BUTTON_IMAGE_HEIGHT	= 42;		//高さ
const int OPENING_BUTTON_KIND_OF_IMAGE	= 6;		//ボタン画像の種類
//表示先
//始めのボタン設定
const float OPENING_BUTTON_BEGIN_IMAGE_INDICATE_Y	= 200.0f;		//Y
//レベル設定時
const float OPENING_BUTTON_LEVEL_IMAGE_INDICATE_Y	= 200.0f;		//Y
//表示間隔
const float OPENING_BUTTON_IMAGE_INDICATE_SPACE_Y	= 70.0f;		//Y
//背景のマージン
const float	OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN = 10.0f;

//▼ヘッダーをインクルード
#include "graphics.h"

//▼構造体の定義
typedef struct tagBUTTON{
	CHARACTER ch;
}BUTTON,*LPBUTTON;


//▼自作関数の宣言
void InitOpening(HWND hWnd);		//初期化
void UpdateOpening();				//更新
void DrawOpening();					//表示
void ReleaseOpening();				//解放
/*
**************************************************************************************************
タイトルロゴ演出の更新
**************************************************************************************************
*/
void	UpdateTitleLogoDirect();
/*
**************************************************************************************************
ボタン設定1（Start , Option）
**************************************************************************************************
*/
void	SetFirstButton();
/*
**************************************************************************************************
ボタン設定2レベル設定
**************************************************************************************************
*/
void	SetSecondButton();


#endif