#ifndef __PLAY_H__
#define __PLAY_H__

//▼マクロの定義


//難易度設定
enum tagGAMELEVEL{

	 EASY_MODE   ,		//EASYモード
	 NORMAL_MODE ,		//NORMALモード
	 HARD_MODE   ,		//HARDモード
	 LEVEL_MAX	 ,		//レベル設定の総数
};
	

//▼ヘッダーをインクルード
#include	"graphics.h"
#include	"SelectStage.h"


//▼自作関数の宣言
void InitPlay(HWND hWnd);		//初期化
void UpdatePlay();				//更新
void DrawPlay();			//表示
void ReleasePlay();				//解放
//ステージ番号変更
void ChangeStage( tagSELECTSTAGE select );
//現在のステージ番号を取得する
tagSELECTSTAGE	 GetStage();
//プレイ中を扱う
void Playing(bool play);
//プレイ中かを取得する
bool GetPlay();
//難易度を設定
void SetGameLevel(tagGAMELEVEL level);
//難易度を取得
tagGAMELEVEL GetGameLevel();
//レースをスタートから始める
void RestartRace();


#endif
