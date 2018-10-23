#include    "play.h"
#include    "map.h"
#include    "seastage.h"
#include    "CRoadStage.h"
#include    "swimer.h"
#include    "CRunnerManager.h"
#include    "gadget.h"
#include    "seaenemy.h"
#include	"CRoadEnemyManager.h"
#include    "item.h"
#include    "effect.h"
#include    "result.h"
#include    "score.h"
#include    "wipe.h"
#include    "game.h"
#include    "menu.h"
#include    "wipe.h"
#include    "sound.h"
#include    "talk.h"
#include    "input.h"
#include	"OffRoadStage.h"
#include	"OffRoadRacer.h"
#include	<iostream>

//▼グローバル変数
//ステージの種類を扱う
tagSELECTSTAGE g_stage = STAGE_VACANT;
//プレイ中かを扱う
bool g_play = false;
//ゲームの難易度を扱う
tagGAMELEVEL g_level = EASY_MODE;

//オフロードステージ
COffRoadStage	g_COffRoad;					//ステージ
COffRoadRacer	g_COffRoadRacer;			//プレイヤー

//ロードステージ
//競争者管理クラス
CRunnerManager		g_CRunnerManager;
//障害物管理クラス
CRoadEnemyManager	g_CRoadEnemyForPlay;
//ステージクラス
CRoadStage			g_CRoadStage;

//************************************************************************************************
//初期化
//************************************************************************************************
void InitPlay(HWND hWnd) {

	//プレイ中フラグ
	g_play = false;

	//マップの初期化
	InitMap(hWnd);

	//▼ ステージ番号に応じた初期化
	switch (g_stage) {

		//海ステージ
	case STAGE_SEA:
		InitSea(hWnd);			//ステージ
		InitSwimer(hWnd);			//プレイヤー
		InitEnemy(hWnd);			//敵
		InitItem(hWnd);			//アイテムの初期化
		InitEffect(hWnd);			//エフェクトの初期化

		break;

		//道路ステージ
	case STAGE_ROAD:
		g_CRoadStage.InitRoadStage();									//ステージ
		g_CRoadEnemyForPlay.InitRoadEnemyManager(&g_CRunnerManager);	//敵
		g_CRunnerManager.InitRunnerManager(&g_CRoadEnemyForPlay);		//競争者
		break;

		//オフロードステージ
	case STAGE_OFF_ROAD:
		g_COffRoad.InitOffRoadStage();			//ステージ
		g_COffRoadRacer.InitOffRoadRacer();		//プレイヤー
		break;
	}

	//ガジェットの初期化
	InitGadget(hWnd);

	InitScore(hWnd);		//スコアの初期化

	InitMenu(hWnd);		//メニューの初期化
}

//************************************************************************************************
//更新
//************************************************************************************************
void UpdatePlay(){

	UpdateMap();		//マップの更新

	//レースがスタートしたら敵などを更新
	if(g_play == true){

		//***********デバッグ用************
		if (JustKey('R')) CreateWipe(SCENE_RESULT, CHANGE_SCENE, WIPE_CONTRACTION);
		if (JustKey('E')) CreateWipe(SCENE_ENDING, CHANGE_SCENE, WIPE_CONTRACTION);

	//▼ ステージ番号に応じた初期化
	switch( g_stage ){

		//海ステージ
		case STAGE_SEA :
			UpdateSea();			//ステージ
			UpdateSwimer();			//プレイヤー
			UpdateEnemy();			//敵
			UpdateItem();			//アイテムの更新
			UpdateEffect();			//エフェクトの更新
			UpdateValueation();		//スコア評価の更新
			break;

		//道路ステージ
		case STAGE_ROAD:
			g_CRoadStage.UpdateRoadStage();						//ステージ
			g_CRoadEnemyForPlay.UpdateRoadEnemyManager();		//敵
			g_CRunnerManager.UpdateRunnerManager();				//競争者
			UpdateValueation();									//スコア評価の更新
			break;

		//オフロードステージ
		case STAGE_OFF_ROAD:
			g_COffRoad.UpdateOffRoadStage();		//ステージ
			g_COffRoadRacer.UpdateOffRoadRacer();	//プレイヤー
			break;
	}
		//ガジェットの更新
		UpdateGadget();
	}
	
	UpdateMenu();		//メニューの更新

	//メニューを開いていたら一時停止
	if( GetMenu() ) g_play = false;

}

//************************************************************************************************
//表示
//************************************************************************************************
void DrawPlay() {

	//▼ ステージ番号に応じた初期化
	switch (g_stage) {

		//海ステージ
	case STAGE_SEA:
		DrawSea();			//ステージ
		break;

		//道路ステージ
	case STAGE_ROAD:
		g_CRoadStage.DrawRoadStage();	//ステージ
		break;

		//オフロードステージ
	case STAGE_OFF_ROAD:
		g_COffRoad.DrawOffRoadStage();			//ステージ
		break;
	}

	DrawMap();		//マップの表示

	//▼　レース開始後の表示
	if (g_play == true) {

		//▼ ステージ番号に応じた初期化
		switch (g_stage) {

			//海ステージ
		case STAGE_SEA:
			DrawSwimer();			//プレイヤー
			DrawEnemy();			//敵
			DrawItem();				//アイテムの表示
			DrawEffect();			//エフェクトの表示
			break;

			//道路ステージ
		case STAGE_ROAD:
			g_CRoadEnemyForPlay.DrawRoadEnemyManager();		//敵
			g_CRunnerManager.DrawRunnerManager();			//競争者
			break;

			//オフロードステージ
		case STAGE_OFF_ROAD:
			g_COffRoadRacer.DrawOffRoadRacer();				//プレイヤー
			break;
		}

		//ガジェットの表示
		DrawGadget();
	}


	DrawMenu();		//メニューの表示

}

//************************************************************************************************
//解放
//************************************************************************************************
void ReleasePlay() {

	ReleaseMap();					//マップの解放

	//▼ ステージ番号に応じた初期化
	switch (g_stage) {

		//海ステージ
	case STAGE_SEA:
		ReleaseSea();			//ステージ
		ReleaseSwimer();		//プレイヤー
		ReleaseEnemy();			//敵
		ReleaseItem();			//アイテムの解放
		ReleaseEffect();		//エフェクトの解放
		break;

		//道路ステージ
	case STAGE_ROAD:
		g_CRoadStage.ReleaseRoadStage();						//ステージ
		g_CRunnerManager.ReleaseRunnerManager();				//競争者
		g_CRoadEnemyForPlay.ReleaseRoadEnemyManager();			//敵
		break;

		//オフロードステージ
	case STAGE_OFF_ROAD:
		g_COffRoad.ReleaseOffRoadStage();			//ステージ
		g_COffRoadRacer.ReleaseOffRoadRacer();		//プレイヤー
		break;
	}

	ReleaseGadget();	//ガジェットの解放
	ReleaseMenu();		//メニューの解放
	ReleaseScore();		//スコアの解放

}

//************************************************************************************************
//プレイ中フラグを立てる
//************************************************************************************************
void Playing(bool play){ g_play = play; }

//************************************************************************************************
//プレイ中を取得できる
//************************************************************************************************
bool GetPlay(){	return g_play; }

//************************************************************************************************
//ステージを変更する関数
//************************************************************************************************
void ChangeStage( tagSELECTSTAGE select ) { g_stage = select; }

//************************************************************************************************
//現在のステージ番号を取得する関数
//************************************************************************************************
tagSELECTSTAGE GetStage() { return g_stage; }

//************************************************************************************************
//ゲームの難易度を設定
//************************************************************************************************
void SetGameLevel(tagGAMELEVEL level){ g_level = level; }

//************************************************************************************************
//ゲームの難易度を取得
//************************************************************************************************
tagGAMELEVEL GetGameLevel(){ return g_level; }

//************************************************************************************************
//難易度変更せずにレースをスタートから始める
//************************************************************************************************
void RestartRace() {
	//ウィンドウハンドル
	HWND hWnd = NULL;
	//プレイの解放
	ReleasePlay();
	//プレイの初期化
	InitPlay(hWnd);
}