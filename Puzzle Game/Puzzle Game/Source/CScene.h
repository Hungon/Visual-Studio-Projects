#ifndef		CSCENE_H__
#define		CSCENE_H__

// SCENE_NUM列挙型
enum SCENE_NUM{

	SCENE_INIT = 0,				// 初期化
	SCENE_UPDATE,				// 更新・描画
	SCENE_RELEASE,				// 解放
	SCENE_END,					// シーンの終了
	SCENE_ERROR					// エラー

};

#include	"CFps.h"

// CSceneクラス
// 1つのシーンを管理する
class		CScene : public CFps{
private:
	char		m_SceneNum;					// シーン管理用の値
public:
	CScene();								// コンストラクタ
	virtual ~CScene();						// デストラクタ
	
	bool				Scene();			// シーン遷移
	// 各ゲームシーン毎に処理が違うので、純粋仮想関数として定義する
	virtual SCENE_NUM	Init() = 0;			// 初期化
	virtual SCENE_NUM	Update() = 0;		// 更新
	virtual SCENE_NUM	Release() = 0;		// 解放
	virtual void		Draw() = 0;			// 描画
};

#endif