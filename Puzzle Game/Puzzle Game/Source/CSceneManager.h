#ifndef		CSCENEMANAGER_H__
#define		CSCENEMANAGER_H__

// ヘッダインクルード
#include	<windows.h>
#include	"CScene.h"								// CSceneクラスを継承するため

// GAMESCENE_NUMBER列挙型
enum	GAMESCENE_NUMBER{

	GAMESCENE_OPENING = 0	,						// オープニング
	GAMESCENE_PLAY_PUZZLE	,						//who is playing puzzle
	GAMESCENE_STORE,								//designed my knit to sell
	GAMESCENE_DESIGN,								//to design knit
	GAMESCENE_SELECT_MODE,							//who select mode to play
	GAMESCENE_GAMEOVER		,						// ゲームオーバー
	GAMESCENE_ENDING		,						// エンディング
	GAMESCENE_NOTHING			,					// 何もない( NULLのようなもの )

};

//front define
class CWipeManager;

// CSceneManagerクラス
//  ゲームシーンを管理するためのクラス
class		CSceneManager: public CScene{
private:
	static HWND					m_hWnd;				// ウィンドウハンドル
	static GAMESCENE_NUMBER		m_NextGameScene;	// 次のシーンを表す
	CScene						*m_lpcGameScene;	// 現在のゲームシーンのクラスをアップキャストで扱う
	CWipeManager	*m_lpCWipe;						//for create wipe
public:
	CSceneManager
	( GAMESCENE_NUMBER StartScene );
	~CSceneManager
	();

	// ここでもCSceneクラスのメンバ関数をオーバーライド
	SCENE_NUM		Init();
	SCENE_NUM		Update();
	SCENE_NUM		Release();
	void			Draw();

	static void		SetNextScene		// 次のシーンを設定する
	( GAMESCENE_NUMBER NextScene );
	
	static HWND		GetWindowHandle		// ウィンドウハンドルの取得
	();

private:
	CScene*		CreateGameScene			// ゲームシーンの作成
	( GAMESCENE_NUMBER GameScene );		// ( クラス外から呼ばれるとメモリリークの原因になるのでprivate )
};

#endif