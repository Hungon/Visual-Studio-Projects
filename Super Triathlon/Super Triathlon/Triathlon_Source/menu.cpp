#include "menu.h"
#include "sound.h"
#include "input.h"
#include "wipe.h"
#include "game.h"
#include "swimer.h"
#include "seaenemy.h"
#include "item.h"
#include "score.h"
#include "map.h"
#include "play.h"
#include <stdio.h>

//▼グローバル変数
int g_menuImg[MENU_IMAGE_MAX] = {0};
//CHARACTER構造体
CHARACTER g_menu[MENU_BACK_IMAGE_MAX] = {0};		//メニュー背景
CHARACTER g_menuItem[MENU_KIND_OF_ITEM] = { 0 };	//メニュー項目
CHARACTER g_menuCursor = {0};						//カーソル
CHARACTER g_menuAnswer[2] = { 0 };					//yes no
//メニュー開いた直後の空き時間
int g_menuDontCloseTime = 0;

//効果音
int g_menu_sound[MENU_EFFECT_SOUND_MAX] = {0};

//画面遷移する要素を取得用
tagWIPE_TRANSITION g_transition = NOT_TRANSITION;

//メニュー内の遷移
tagMENUSELECTION	g_menuSelection = MENU_SELECT_VACANT;
tagMENUSELECTION	g_menuSelectionNext = MENU_SELECT_VACANT;


//**************************************************************************************************************************
//初期化
//**************************************************************************************************************************
void InitMenu(HWND hWnd){

	//▼画像の読み込み
	//メニュー背景
	g_menuImg[0] = CreateDDImage("Image\\menu.bmp" , hWnd);
	//メニュー項目
	g_menuImg[1] = CreateDDImage("Image\\menu_word.bmp" , hWnd);
	//カーソル
	g_menuImg[2] = CreateDDImage("Image\\cursor.bmp",hWnd);
	//選択「YES NO」
	g_menuImg[3] = CreateDDImage("Image\\select.bmp",hWnd);


	//▼効果音の読み込み
	//カーソル音
	g_menu_sound[0] = CreateSound("SE\\cursor.wav");
	//決定音
	g_menu_sound[1] = CreateSound("SE\\ok.wav");
	//キャンセル音
	g_menu_sound[2] = CreateSound("SE\\cancel.wav");

	//メニュー背景
	ZeroMemory(&g_menu[0], sizeof(CHARACTER) * MENU_BACK_IMAGE_MAX);
	//カーソルの初期化
	ZeroMemory(&g_menuCursor, sizeof(CHARACTER) * 1);
	//yes no
	ZeroMemory(&g_menuAnswer[0], sizeof(CHARACTER) * 2);

	//画面遷移する要素を取得用
	g_transition = NOT_TRANSITION;
	//メニュー内の遷移
	g_menuSelection = MENU_SELECT_VACANT;
	g_menuSelectionNext = MENU_SELECT_VACANT;
	//メニューを閉じることができない時間
	g_menuDontCloseTime = 0;

	//▼各画像の設定
	//メニュー背景
	g_menu[0].x = MENU_INDICATE_X;
	g_menu[0].y = MENU_INDICATE_Y;
	g_menu[0].w = 160;
	g_menu[0].h = 224;
	g_menu[0].flag = false;
	
	//メニュー項目
	for (int i = 0; i < MENU_KIND_OF_ITEM; i++) {
		g_menuItem[i].x = MENU_INDICATE_X;
		g_menuItem[i].y = MENU_INDICATE_Y +
			( ( MENU_IMAGE_ITEM_HEIGHT + MENU_ITEM_INDICATE_SPACE_Y) * i);
		g_menuItem[i].w = MENU_IMAGE_ITEM_WIDTH;
		g_menuItem[i].h = MENU_IMAGE_ITEM_HEIGHT;
		g_menuItem[i].picx = 0;
		g_menuItem[i].picy = (MENU_IMAGE_ITEM_HEIGHT * i);
		g_menuItem[i].flag = false;
	}


	//yes no
	for (int i = 0; i < 2; i++) {
		g_menuAnswer[i].x = MENU_ANSWER_INDICATE_X;
		g_menuAnswer[i].y = (MENU_ANSWER_INDICATE_Y + 
			( ( MENU_ANSWER_INDICATE_SPACE_Y + MENU_IMAGE_ANSWER_HEIGHT ) * i ));
		g_menuAnswer[i].w = MENU_IMAGE_ANSWER_WIDTH;
		g_menuAnswer[i].h = MENU_IMAGE_ANSWER_HEIGHT;
		g_menuAnswer[i].picx = 0;
		g_menuAnswer[i].picy = (MENU_IMAGE_ANSWER_HEIGHT * i);
		g_menuAnswer[i].flag = false;
	}

	//カーソル
	g_menuCursor.x = g_menuItem[1].x;
	g_menuCursor.y = g_menuItem[1].y;
	g_menuCursor.w = g_menuItem[1].w;
	g_menuCursor.h = 48;
	g_menuCursor.my = MENU_IMAGE_ITEM_HEIGHT + MENU_ITEM_INDICATE_SPACE_Y;
	
}

//**************************************************************************************************************************
//更新
//**************************************************************************************************************************
void UpdateMenu(){

	//メニュー画面を開く
	if(g_menu[0].flag == false) {
		if (JustKey('M') || JustButton(BUTTON_8)) {
			//効果音
			PlaySound(g_menu_sound[1], 0, false);
			//背景　各種項目を表示
			OpenMenu();
		}
	}

	//メニューを開いたら
	if (g_menu[0].flag) {
		//開いた直後はメニューを閉じることができない
		g_menuDontCloseTime++;
		if (g_menuDontCloseTime > MENU_DONT_CONTROL_TIME) { g_menuDontCloseTime = MENU_DONT_CONTROL_TIME; }

		//メニュー内遷移
		switch (g_menuSelection) {
		case MENU_SELECT_ITEM:
			//各種項目の選択
			MenuSelectItem();
			break;
		case MENU_SELECT_ANSWER:
			//選択
			MenuAnswer();
			break;
		case MENU_SELECT_CLOSE:
			//メニューの初期化
			CloseMenu();
			break;
		}
		//操作不能時間を作る
		if (g_menuSelection != g_menuSelectionNext) {
			g_menu[0].time++;
			if (g_menu[0].time >= MENU_DONT_CONTROL_TIME) {
				g_menuSelection = g_menuSelectionNext;
				g_menu[0].time = 0;
			}
		}
		//メニューを閉じる
		if (JustKey('M') || JustButton(BUTTON_8)){
			if (g_menuDontCloseTime >= MENU_DONT_CONTROL_TIME) {
				g_menuSelectionNext = MENU_SELECT_CLOSE;
				//効果音
				PlaySound(g_menu_sound[2], 0, false);
			}
		}
	}
}

//**************************************************************************************************************************
//表示
//**************************************************************************************************************************
void DrawMenu(){
	
	//メニューの表示
	if(g_menu[0].flag){
		//メニュー背景
		DDBlt((int)g_menu[0].x ,
			(int)g_menu[0].y ,
			g_menu[0].w ,
			g_menu[0].h ,
			g_menuImg[0] ,
			g_menu[0].picx ,
			g_menu[0].picy);
		
		//メニュー項目の表示
		for (int i = 0; i < MENU_KIND_OF_ITEM; i++) {
			if (g_menuItem[i].flag) {
				DDBlt((int)g_menuItem[i].x,
					(int)g_menuItem[i].y,
					g_menuItem[i].w,
					g_menuItem[i].h,
					g_menuImg[1],
					g_menuItem[i].picx,
					g_menuItem[i].picy);
			}
		}
	
		//yes no
		for (int i = 0; i < 2; i++) {
			if (g_menuAnswer[i].flag) {
				//「YES」
				DDBlt((int)g_menuAnswer[i].x,
					(int)g_menuAnswer[i].y,
					g_menuAnswer[i].w,
					g_menuAnswer[i].h,
					g_menuImg[3],
					g_menuAnswer[i].picx,
					g_menuAnswer[i].picy);
			}
		}

		//カーソル
		if (g_menuCursor.flag) {
			DDAlphaBlt(
				(int)g_menuCursor.x,
				(int)g_menuCursor.y,
				g_menuCursor.w,
				g_menuCursor.h,
				g_menuImg[2],
				g_menuCursor.picx,
				g_menuCursor.picy,
				120.0f);
		}
	}
}

//**************************************************************************************************************************
//解放
//**************************************************************************************************************************
void ReleaseMenu(){

	//▼画像の解放
	for (int i = 0; i < MENU_IMAGE_MAX; i++) {
		ReleaseDDImage(g_menuImg[i]);
	}
	//▼効果音の解放
	for (int i = 0; i < MENU_EFFECT_SOUND_MAX; i++) {
		ReleaseSound(g_menu_sound[i]);
	}
}

//**************************************************************************************************************************
//メニューを開く
//**************************************************************************************************************************
void OpenMenu(){
	//背景表示
	g_menu[0].flag = true;
	//各種項目を表示
	for (int i = 0; i < MENU_KIND_OF_ITEM; i++) { g_menuItem[i].flag = true; }
	//カーソル
	g_menuCursor.x = g_menuItem[1].x;
	g_menuCursor.y = g_menuItem[1].y;
	g_menuCursor.w = g_menuItem[1].w;
	g_menuCursor.h = 48;
	//遷移フラグ
	g_menuSelection = MENU_SELECT_VACANT;
	g_menuSelectionNext = MENU_SELECT_ITEM;
	//メニューを閉じることができない時間
	g_menuDontCloseTime = 0;

}

//**************************************************************************************************************************
//メニュー中か取得
//**************************************************************************************************************************
bool GetMenu(){return g_menu[0].flag; }

/*
メニューを閉じる
*/
void	CloseMenu() {
	//各種背景
	for (int i = 0; i < MENU_BACK_IMAGE_MAX; i++) { g_menu[i].flag = false; }
	//項目
	for (int i = 0; i < MENU_KIND_OF_ITEM; i++) { g_menuItem[i].flag = false; }
	// yes no
	for (int i = 0; i < 2; i++) { g_menuAnswer[i].flag = false; }
	g_menu[0].time = 0;
	g_menuCursor.flag = false;
	//遷移フラグ
	g_menuSelection = MENU_SELECT_VACANT;
}

/*
各種項目選択
*/
void	MenuSelectItem() {

	tagWIPE_TRANSITION selectTbl[MENU_KIND_OF_ITEM - 1] = {

		RESTART_RACE,		//スタートから始める
		TO_TITLE,			//タイトルへ
		TO_SELECT,			//ステージセレクトへ
	};


	//▼キー入力
	//メニュー選択
	if (g_menuItem[0].flag) {
		//カーソルの移動量
		g_menuCursor.my = MENU_IMAGE_ITEM_HEIGHT + MENU_ITEM_INDICATE_SPACE_Y;
		//カーソル表示フラグ
		g_menuCursor.flag = true;

		//下入力
		if (JustKey(VK_DOWN) || JustLever(LEVER_DOWN)) {
			//カーソル音
			PlaySound(g_menu_sound[0], 0, false);
			//ボタンの間隔だけ動かす
			g_menuCursor.y += g_menuCursor.my;
			//下の項目まで来たら上の項目へ
			if (g_menuCursor.y > g_menuItem[3].y) g_menuCursor.y = g_menuItem[1].y;
		}
		//上入力
		if (JustKey(VK_UP) || JustLever(LEVER_UP)) {
			//カーソル音
			PlaySound(g_menu_sound[0], 0, false);
			//ボタンの間隔だけ動かす
			g_menuCursor.y -= g_menuCursor.my;
			//下の項目まで来たら上の項目へ
			if (g_menuCursor.y < g_menuItem[1].y) g_menuCursor.y = g_menuItem[3].y;
		}

		//▼決定キー
		for (int i = 0; i < (MENU_KIND_OF_ITEM - 1); i++) {
			//「RESTART」
			if (g_menuCursor.y == g_menuItem[i + 1].y) {
				//幅を合わせる
				g_menuCursor.w = g_menuItem[i + 1].w;
				//高さを合わせる
				g_menuCursor.h = g_menuItem[i + 1].h;
				//決定
				if (JustUpKey('Z') || JustButton(BUTTON_1)) {
					//効果音
					PlaySound(g_menu_sound[1], 0, false);
					//項目を非表示
					for (int i = 0; i < MENU_KIND_OF_ITEM; i++) { g_menuItem[i].flag = false; }
					//カーソル表示フラグ
					g_menuCursor.flag = false;
					//yes noを表示
					for (int i = 0; i < 2; i++) { g_menuAnswer[i].flag = true; }
					//画面遷移する要素を取得
					g_transition = selectTbl[i];
					//遷移フラグ
					g_menuSelectionNext = MENU_SELECT_ANSWER;
					//カーソルをyesに合わせる
					g_menuCursor.y = g_menuAnswer[0].y;
					g_menuCursor.x = g_menuAnswer[0].x;
				}
			}
		}
	}
}

/*
各種選択への返答
*/
void	MenuAnswer() {

	//▼画面遷移の選択
	if (g_menuAnswer[0].flag) {
		//カーソルの移動量
		g_menuCursor.my = MENU_IMAGE_ANSWER_HEIGHT + MENU_ANSWER_INDICATE_SPACE_Y;
		//カーソル表示フラグ
		g_menuCursor.flag = true;
		//下入力
		if (JustKey(VK_DOWN) || JustLever(LEVER_DOWN)) {
			//カーソル音
			PlaySound(g_menu_sound[0], 0, false);
			//ボタンの間隔だけ動かす
			g_menuCursor.y += g_menuCursor.my;
			//下の項目まで来たら上の項目へ
			if (g_menuCursor.y > g_menuAnswer[1].y) {
				g_menuCursor.y = g_menuAnswer[0].y;
			}
		}
		//上入力
		if (JustKey(VK_UP) || JustLever(LEVER_UP)) {
			//カーソル音
			PlaySound(g_menu_sound[0], 0, false);
			//ボタンの間隔だけ動かす
			g_menuCursor.y -= g_menuCursor.my;
			//下の項目まで来たら上の項目へ
			if (g_menuCursor.y < g_menuAnswer[0].y) {
				g_menuCursor.y = g_menuAnswer[1].y;
			}
		}
		//決定「YES」を選択
		if (g_menuCursor.y == g_menuAnswer[0].y) {
			//幅を合わせる
			g_menuCursor.w = g_menuAnswer[0].w;
			//高さを合わせる
			g_menuCursor.h = g_menuAnswer[0].h;
			//決定
			if (JustKey('Z') || JustButton(BUTTON_1)) {
				//効果音
				PlaySound(g_menu_sound[1], 0, false);
				//画面遷移
				CreateWipe(MENU_TRANSITION, g_transition, WIPE_CONTRACTION);
			}
		}
		//決定「NO」を選択
		if (g_menuCursor.y == g_menuAnswer[1].y) {
			//幅を合わせる
			g_menuCursor.w = g_menuAnswer[1].w;
			//高さを合わせる
			g_menuCursor.h = g_menuAnswer[1].h;
			//決定
			if (JustUpKey('Z') || JustUpButton(BUTTON_1)) {
				//効果音
				PlaySound(g_menu_sound[2], 0, false);
				//項目を表示
				for (int i = 0; i < MENU_KIND_OF_ITEM; i++) { g_menuItem[i].flag = true; }
				//yes noを非表示
				for (int i = 0; i < 2; i++) { g_menuAnswer[i].flag = false; }
				//カーソル表示フラグ
				g_menuCursor.flag = false;
				//カーソルを項目に合わせる
				g_menuCursor.y = g_menuItem[1].y;
				g_menuCursor.x = g_menuItem[1].x;
				//遷移する要素をリセット
				g_transition = NOT_TRANSITION;
				//遷移フラグ
				g_menuSelectionNext = MENU_SELECT_ITEM;
			}
		}
	}

}