#include	"CScene.h"
#include	"graphics.h"
#include	"input.h"

// コンストラクタ
CScene::CScene(){
	// シーンは必ず初期化から始めるようにする
	this->m_SceneNum = SCENE_INIT;
}

// デストラクタ
CScene::~CScene(){
}

// CScene::Scene
//
// 引数
//  無し
//
// 戻り値
//  bool						true:	ゲームシーンの処理中
//								false:	ゲームシーンの終わり
//
// 説明
//  シーン遷移を行います。
bool	CScene::Scene(){


	//strict fps
	if (this->WaitFps()) {
		// シーン管理値によって分岐
		switch (this->m_SceneNum) {

		case	SCENE_INIT:

			// 初期化処理
			this->m_SceneNum = this->Init();
			break;

		case	SCENE_UPDATE:

			// 更新処理
			this->m_SceneNum = this->Update();
			// 描画処理
			this->Draw();
			//draw fps
			this->DrawFps(0, 0);
			//end the game
			if (JustKey(VK_ESCAPE)) this->m_SceneNum = SCENE_END;
			break;

		case	SCENE_RELEASE:

			// 解放処理
			this->m_SceneNum = this->Release();

			break;
		}

		// エラーの値が返ってきたら
		if (this->m_SceneNum == SCENE_ERROR) {

			// 次は解放処理を行うように変更
			this->m_SceneNum = SCENE_RELEASE;
		}

		// 終了の値が返ってきたら
		if (this->m_SceneNum == SCENE_END) {

			// シーンの終わり
			return	false;
		}
	}

	// シーンの処理中
	return	true;

}
