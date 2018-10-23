#ifndef __MENU_H__
#define __MENU_H__

//▼ 列挙型
//メニュー内遷移
enum tagMENUSELECTION {
	MENU_SELECT_VACANT = -1,
	MENU_SELECT_ITEM,
	MENU_SELECT_ANSWER,
	MENU_SELECT_CLOSE,
	MENU_KIND_OF_SLECTION,
};

//▼マクロの定義
#define MENU_IMAGE_MAX			4	//使用画像枚数
#define MENU_EFFECT_SOUND_MAX	3	//使用効果音
#define MENU_BACK_IMAGE_MAX		1	//メニュー背景用	
const int MENU_KIND_OF_ITEM = 4;	//項目の数(タイトルmenuを含む)


//メニュー項目の表示設定
//表示先
const float MENU_INDICATE_X = 240.0f;			//X
const float MENU_INDICATE_Y = 240.0f;			//Y
//サイズ
const int MENU_IMAGE_ITEM_WIDTH = 140;			//幅
const int MENU_IMAGE_ITEM_HEIGHT = 42;			//高さ
//表示間隔
const float MENU_ITEM_INDICATE_SPACE_Y = 10.0f;		//Y

//yes no
const float MENU_ANSWER_INDICATE_X = 290.0f;		//表示先X
const float MENU_ANSWER_INDICATE_Y = 290.0f;		//表示先Y
//サイズ
const int MENU_IMAGE_ANSWER_WIDTH = 60;				//幅
const int MENU_IMAGE_ANSWER_HEIGHT = 42;			//高さ
const float MENU_ANSWER_INDICATE_SPACE_Y = 60.0f;	//表示間隔Y


//操作制限時間
const int MENU_DONT_CONTROL_TIME = 20;

//▼ヘッダーのインクルード
#include "graphics.h"


//▼プロトタイプ宣言
//初期化
void InitMenu(HWND hwnd);
//更新
void UpdateMenu();
//表示
void DrawMenu();
//解放
void ReleaseMenu();
//メニューを開く
void OpenMenu();
//メニュー中か取得
bool GetMenu();
/*
メニューを閉じる
*/
void	CloseMenu();

/*
各種項目選択
*/
void	MenuSelectItem();
/*
各種選択への返答
*/
void	MenuAnswer();

#endif