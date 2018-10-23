#include	<iostream>
#include	"CSceneManager.h"
#include	"COpening.h"
#include	"CPlayPuzzle.h"
#include	"CStore.h"
#include	"CDesign.h"
#include	"CSelectMode.h"
#include	"CGameOver.h"
#include	"CEnding.h"
#include	"window.h"
#include	"event.h"
#include	"input.h"
#include	"graphics.h"
#include	"sound.h"
#include	"CWipeManager.h"

using namespace std;

// 静的メンバ関数の初期化
HWND			CSceneManager::m_hWnd = NULL;
GAMESCENE_NUMBER	CSceneManager::m_NextGameScene = GAMESCENE_NOTHING;

//コンストラクタ
CSceneManager::CSceneManager
( GAMESCENE_NUMBER StartScene )
{
	// シーンの作成
	// アップキャストで受け取る
	this->m_lpcGameScene = this->CreateGameScene( StartScene );

	// ウィンドウハンドルの初期化
	this->m_hWnd = NULL;
}


//ディストラクタ
CSceneManager::~CSceneManager
()
{
	this->Release();
}


// CSceneManager::CreateGameScene
//
// 引数
//  GAMESCENE_NUMBER GameScene		作成するゲームシーンを表す値
//
// 戻り値
//  CScene*						GameSceneに対応するゲームシーンが確保出来たらそのオブジェクトのアドレス
//								確保に失敗した場合はNULL
//
// 説明
//  GameSceneに合わせてゲームシーンクラスの作成を行います。
CScene*		CSceneManager::CreateGameScene
( GAMESCENE_NUMBER GameScene )
{

	// 引数のゲームシーンの値に合わせて派生クラスを確保し、そのまま戻り値にする
	switch ( GameScene ){

		case	GAMESCENE_OPENING:
			return	new	COpening();

		case	GAMESCENE_PLAY_PUZZLE:
			return	new CPlayPuzzle();

		case	GAMESCENE_STORE:
			return new CStore();

		case	GAMESCENE_DESIGN:
			return new CDesign();

		case GAMESCENE_SELECT_MODE:				//during select mode
			return new CSelectMode();
		
		case	GAMESCENE_GAMEOVER:
			return	new CGameOver();

		case	GAMESCENE_ENDING:
			return	new CEnding();


	}

	// switch文に引っかからなかったらGAMESCENE_NOTHINGや、追加し忘れになるのでNULLを返す
	return	NULL;
}


/*
*******************************************************************************************
ゲーム全体の初期化
*******************************************************************************************
*/
SCENE_NUM		CSceneManager::Init()
{
	//create window
	CSceneManager::m_hWnd = InitWindow( "MY GAME" , 0 , 0 );

	//ウィンドウモードの初期化
	if( InitDDWindow( CSceneManager::m_hWnd ) == false ){
		MessageBox( NULL , "DD初期化失敗\n" , "CManagerInit" , MB_OK );
		return 	SCENE_ERROR;
	}
	//DirectSoundの初期化
	if( InitDirectSound( CSceneManager::m_hWnd ) == false ){	
		MessageBox( NULL , "DS読み込み失敗" , "CManagerInit" , MB_OK );
		return 	SCENE_ERROR;
	}

	//allot memory
	this->m_lpCWipe = new CWipeManager;				//for create wipe

	this->m_lpCWipe->InitWipeManager();				//initialize wipe

	return	SCENE_UPDATE;

}


/*
*******************************************************************************************
ゲーム全体の更新
*******************************************************************************************
*/
SCENE_NUM		CSceneManager::Update()
{

	// ゲームシーンが確保されているかを調べる
	if (this->m_lpcGameScene == NULL)				return	SCENE_ERROR;

	//最初にキーの更新
	UpdateInput();

	//フレームの最初に消失を調べる
	RestoreSurface(CSceneManager::m_hWnd);


	//▼ ゲームシーンの処理を行う
	// ゲームシーンが終わった場合はif文の中の処理を行う
	if (this->m_lpCWipe->UpdateWipeManager() == false) {			//when don't create wipe, transition to next scene
		if (!this->m_lpcGameScene->Scene()) {
			// 確保していたゲームシーンの解放・不正参照防止
			delete this->m_lpcGameScene;
			this->m_lpcGameScene = NULL;
			// 次のシーンが設定されてなかったら解放処理へ
			if (CSceneManager::m_NextGameScene == GAMESCENE_NOTHING)	return	SCENE_RELEASE;
			// 次のゲームシーンを確保
			this->m_lpcGameScene = this->CreateGameScene(CSceneManager::m_NextGameScene);
			// 次のシーンを無しに設定
			CSceneManager::m_NextGameScene = GAMESCENE_NOTHING;
		}
	}
	//メッセージ管理を常に行う
	DoEvents();

	return	SCENE_UPDATE;

}

/*
*******************************************************************************************
ゲーム全体の解放
*******************************************************************************************
*/
SCENE_NUM		CSceneManager::Release()
{
	// ゲーム中1度しか行わない解放処理
	// DirectDrawやDirectSoundなどの解放を行う

	//時間精度のリセット
	timeEndPeriod(1);

	// ゲームシーンが確保されていたら
	if ( this->m_lpcGameScene != NULL ){
		// ゲームシーンの解放
		this->m_lpcGameScene->Release();
		// 解放・不正参照防止
		delete this->m_lpcGameScene;
		this->m_lpcGameScene = NULL;
	}
	//DirectDrawの解放
	ReleaseDirectDraw();
	//DirectSoundの解放
	ReleaseDirectSound();


	return	SCENE_END;
}


/*
*******************************************************************************************
ゲーム全体の表示
*******************************************************************************************
*/
void			CSceneManager::Draw()
{
	// 「各ゲームシーンのDraw」→「CSceneManagerクラスのDraw」
	// の順番で呼ばれるので、トークやワイプの描画、最終描画をここに
	
	//ハンドルコンテキスト
	HDC hDC = NULL;
	//バックサーフェイスの取得
	LPDIRECTDRAWSURFACE7 lpDDSBack = GetBackSurface();

	//draw wipe
	this->m_lpCWipe->DrawWipeManager();

	//仮想画面のhDCを取得
	lpDDSBack->GetDC( &hDC );

	//仮想画面のhDCを解放
	lpDDSBack->ReleaseDC( hDC );

	//最終出力
	UpdatePrimary( CSceneManager::m_hWnd );

}



// CSceneManager::SetNextScene
//
// 引数
//  GAMESCENE_NUMBER GameScene		作成するゲームシーンを表す値
//
// 戻り値
//  無し
//
// 説明
//  次のシーンを設定します。
void			CSceneManager::SetNextScene
( GAMESCENE_NUMBER NextScene )
{

	// 次のシーンを表す値を設定
	CSceneManager::m_NextGameScene = NextScene;
}


// CSceneManager::GetWindowHandle
//
// 引数
//  無し
//
// 戻り値
//  HWND						ウィンドウハンドル
//
// 説明
//  ウィンドウハンドルを取得します。
HWND			CSceneManager::GetWindowHandle
()
{
	return	CSceneManager::m_hWnd;
}
