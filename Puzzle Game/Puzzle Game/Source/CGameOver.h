#ifndef		CGAMEOVER_H__
#define		CGAMEOVER_H__

#include	"CScene.h"
#include	<windows.h>

//▼ クラスの定義
// シーン管理用クラスを継承して
// ゲームオーバーシーンクラスを作成する
class		CGameOver: public CScene {

private:
	
	int		m_nBackImg[1];			// 背景画像の要素番号保存用
	//座標などを扱うキャラクター構造体

public:

	//▼ 静的メンバ変数の定義
	//ウィンドウハンドル
	static HWND m_hWnd;
	//画像の使用枚数
	static const char CGAMEOVER_IMAGE_MAX;

	CGameOver();
	~CGameOver();

	// CSceneクラスに定義されている
	// 純粋仮想関数のオーバーライド
	SCENE_NUM	Init();
	SCENE_NUM	Update();
	SCENE_NUM	Release();
	void		Draw();

};

#endif