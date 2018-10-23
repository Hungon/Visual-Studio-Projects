#ifndef		CROADENEMYMANAGER_H__
#define		CROADENEMYMANAGER_H__

//▼ヘッダーをインクルード
#include	"graphics.h"
#include	"CRunner.h"

//▼ 前方宣言
class	CObstacle;
class	CRunnerManager;

//▼ 競争者クラス
class	CRoadEnemyManager{

private:
	

	//画像の要素
	int		*m_lpRoadEnemyManagerImg;

	//効果音の要素
	int		*m_lpRoadEnemyManagerEffectSound;

	//障害物クラス
	CObstacle	*m_lpCObstacle;

	//障害物最大生成数
	int		m_ObstacleCreateMax;


protected:

public:

	//▼ 静的メンバ変数の定義
	static const int	ROADENEMYMANAGER_IMAGE_MAX;			//使用画像枚数
	static const int	ROADENEMYMANAGER_EFFECT_SOUND_MAX;	//使用効果音数
	static const HWND	m_hWnd;


	CRoadEnemyManager();
	~CRoadEnemyManager();

	//▼プロトタイプ宣言
	//競争者の初期化
	void	InitRoadEnemyManager( CRunnerManager *lpRunnerManager );
	//競争者の更新
	void	UpdateRoadEnemyManager();
	//競争者の表示
	void	DrawRoadEnemyManager();
	//競争者の解放
	void	ReleaseRoadEnemyManager();
	/*
	*************************************************************************************************
	引数のランナーと重なりの判定を取る
	*************************************************************************************************
	*/
	char	CollisionRoadEnemyManager( RUNNER *lpRunner );
	/*
	*************************************************************************************************
	引数のキャラクター構造体の索敵範囲で判定を取る
	引数   : キャラクター構造体
	戻り値 : 障害物の種類番号を返す
	***********************************************************************************************
	*/
	BYTE	BroadenCollisionSquareToObstacle( CHARACTER *lpCh );


	

};


#endif
