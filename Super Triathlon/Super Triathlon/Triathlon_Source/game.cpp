#include    "game.h"
#include    "fps.h"
#include    "input.h"
#include    "play.h"
#include    "opening.h"
#include    "gameover.h"
#include    "result.h"
#include    "config.h"
#include    "wipe.h"
#include    "sound.h"
#include    "talk.h"
#include    "option.h"
#include    "menu.h"
#include    "ending.h"
#include	"SelectStage.h"
#include	<stdio.h>


//▼グローバル変数

//画面遷移を扱う変数
tagGAMETRASITION g_scene_now = SCENE_OPENING;		//現在のシーン
tagGAMETRASITION g_scene_next = g_scene_now;		//次のシーン

//using current language
tagGAMELANGUAGE	g_GameUsingLanguage = GAME_LANGUAGE_ENGLISH;


//************************************************************************************************
//▼InitGame関数「初期化」
//************************************************************************************************
void InitGame(HWND hWnd){

	//シーン切り替えを扱う関数ポインタ
	void (*pInit[SCENE_MAX])(HWND) = {
		
		InitOpening ,
		InitSelectStage,
		InitPlay ,
		InitCommonResult,
		InitGameOver ,
		InitConfig ,
		InitOption ,
		InitEnding ,
	};

	//ウィンドウモードの初期化
	if(InitDDWindow(hWnd) == false){
		MessageBox(NULL , "DD初期化失敗\n" , "" , MB_OK);
		return;
	}
	//DirectSoundの初期化
	if(InitDirectSound(hWnd) == false){
		MessageBox(NULL,"DS読み込み失敗","InitGame",MB_OK);
		return;
	}

	//文字表示の初期化
	InitTalk();

	//ワイプの初期化
	InitWipe(hWnd);

	//シーン切り替え
	pInit[g_scene_now](hWnd);
}

//************************************************************************************************
//▼UpdateGame関数「更新」
//************************************************************************************************
void UpdateGame(){

	//シーン切り替えを扱う関数ポインタ
	void (*pUpdate[SCENE_MAX])() = {

		UpdateOpening ,
		UpdateSelectStage,
		UpdatePlay ,
		UpdateResult ,
		UpdateGameOver ,
		UpdateConfig ,
		UpdateOption ,
		UpdateEnding ,
	};

	//最初にキーの更新
	UpdateInput();

	//文章表示の更新
	UpdateTalk();

	//ワイプが未発生なら他の更新を行う
	if(UpdateWipe() == false){
		pUpdate[g_scene_now]();
	}
}


//************************************************************************************************	
//▼DrawGame関数「表示」
//************************************************************************************************
void DrawGame(HWND hWnd){

	HDC hDC = NULL;
	//バックサーフェイスの取得
	LPDIRECTDRAWSURFACE7 lpDDSBack = GetBackSurface();

	//シーン切り替えを扱う関数ポインタ
	void (*pDraw[SCENE_MAX])() = {

		DrawOpening ,
		DrawSelectStage,
		DrawPlay ,
		DrawResult ,
		DrawGameOver ,
		DrawConfig ,
		DrawOption ,
		DrawEnding ,
	};

	//塗りつぶし
	FillBackSurface();

	//シーン切り替え
	pDraw[g_scene_now]();

	//文章の表示
	DrawTalk();

	//ワイプの描画
	DrawWipe();

	//FPS
//	DrawFps(0,0);
		
	//仮想画面のhDCを取得
	lpDDSBack->GetDC(&hDC);

	//仮想画面のhDCを解放
	lpDDSBack->ReleaseDC(hDC);

	//最終出力
	UpdatePrimary(hWnd);

}

//************************************************************************************************
//▼ReleaseGame関数「解放」
//************************************************************************************************
void ReleaseGame(){

	//文章の解放
	ReleaseTalk();

	//DirectDrawの解放
	ReleaseDirectDraw();

	//DirectSoundの解放
	ReleaseDirectSound();
	
}

//************************************************************************************************
//シーン変更
//************************************************************************************************
void CheckScene(HWND hWnd){

	//初期化の分岐関数ポインタ
	void(*pInit[SCENE_MAX])(HWND) = {
		
		InitOpening ,
		InitSelectStage,
		InitPlay ,
		InitCommonResult,
		InitGameOver ,
		InitConfig ,
		InitOption ,
		InitEnding ,
	};

	//解放の分岐関数ポインタ
	void(*pRelease[SCENE_MAX])() = {
		
		ReleaseOpening ,
		ReleaseSelectStage,
		ReleasePlay ,
		ReleaseResult ,
		ReleaseGameOver ,
		ReleaseConfig ,
		ReleaseOption ,
		ReleaseEnding ,
	};

	//現在のシーンと次のシーンを比較
	if( g_scene_now != g_scene_next ){

		//異なる値ならシーン変更
		//現在のシーンを解放
		pRelease[g_scene_now]();

		//現在のシーンの更新
		g_scene_now = g_scene_next;

		//次のシーンを初期化
		pInit[g_scene_now](hWnd);

	}
}

//************************************************************************************************
//渡された値を元にシーン番号を変更する関数
//************************************************************************************************
void ChangeScene( tagGAMETRASITION scene ){ g_scene_next = scene; }


//************************************************************************************************
//現在のシーンを取得する
//************************************************************************************************
tagGAMETRASITION GetSceneNow(){ return g_scene_now; }


//************************************************************************************************
//タイトル画面へ遷移	
//************************************************************************************************
void ToTitle() {
	//ステージテーブル
	tagSELECTSTAGE stageTbl[STAGE_MAX] = { STAGE_SEA , STAGE_ROAD ,STAGE_OFF_ROAD };
	//クリアしたステージをリセット
	for (int i = 0; i < STAGE_MAX; i++) { SetCompleteStage(SELECTSTAGE_STAGE_NOT_COMPLETE, stageTbl[i]); }
	//ゲームオーバー
	ChangeScene(SCENE_GAMEOVER);
}

/*
ステージ選択へ遷移
*/
void ToSelectStage() { ChangeScene(SCENE_SELECTSTAGE); }

/*
Set current language
*/
void	SetGameLanguage(tagGAMELANGUAGE language) {
	//declaration for save
	RECORD record = { 0 };
	int item = 1;
	g_GameUsingLanguage = language; 
	//subsutitute present language for record variable
	record.recordBox[0][0] = (int)language;
	//save present language to file
	SaveRecord("Data\\Language.dat", 1, &item, &record);
}
/*
Get current language
*/
tagGAMELANGUAGE	GetGameLanguage() { return g_GameUsingLanguage; }