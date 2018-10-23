#ifndef __GAME_H__
#define __GAME_H__

//▼マクロの定義

//画面を遷移を扱う列挙型
enum tagGAMETRASITION{
	
	//メニューからの画面遷移での要素
	 MENU_TRANSITION = -1 ,
	 SCENE_OPENING	,
	 SCENE_SELECTSTAGE	,
	 SCENE_PLAY		,
	 SCENE_RESULT	,
	 SCENE_GAMEOVER	,
	 SCENE_CONFIG	,
	 SCENE_OPTION	,
	 SCENE_ENDING   ,
	 SCENE_MAX		,

};

//using kind of language
enum tagGAMELANGUAGE {
	GAME_LANGUAGE_JAPANESE,			//Japanese
	GAME_LANGUAGE_ENGLISH,			//English
	GAME_KIND_OF_LANGUAGE,			//kind of language
};

//▼ヘッダーファイルの読み込み
#include "graphics.h"


//▼自作関数を定義

//初期化を行う関数
void InitGame(HWND hWnd);	
//更新を行う関数
void UpdateGame();		
//表示を行う関数
void DrawGame(HWND hWnd);	
//解放を行う関数
void ReleaseGame();	
//シーンの切り替えを確認する関数
void CheckScene(HWND hWnd);
//シーンの切り替えを行う関数
void ChangeScene(tagGAMETRASITION scene);
//現在のシーンを取得する
tagGAMETRASITION GetSceneNow();
//タイトルへ戻る
void ToTitle();	
/*
ステージ選択へ遷移
*/
void ToSelectStage();
/*
Set current language
*/
void	SetGameLanguage(tagGAMELANGUAGE language);
/*
Get current language
*/
tagGAMELANGUAGE	GetGameLanguage();

#endif