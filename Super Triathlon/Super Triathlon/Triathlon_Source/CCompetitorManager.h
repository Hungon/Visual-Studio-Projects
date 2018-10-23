#ifndef		CCOMPETITORMANAGER_H__
#define		CCOMPETITORMANAGER_H__

//▼ヘッダーをインクルード
#include	"graphics.h"
#include	"CCharacter.h"

//▼ 前方宣言
class	CCompetitor;
class	CEffectManager;
class	CRoadEnemyManager;

//▼ 競争者クラス
class	CCompetitorManager{

private:
	
	//競争者クラス
	static CCompetitor		*m_lpCCompetitorRunner;

	//エフェクト管理クラス
	CEffectManager	*m_lpCEffect;

	//画像の要素
	int		*m_lpCompetitorImg;

	//効果音の要素
	int		*m_lpCompetitorEffectSound;

	//競争者の数
	static char	m_CompetitorMax;

	//各競争者の座標を扱う
	static TPOINT<float>	*m_lpCompetitorPos;

	//各障害物の効果を発揮した障害物の要素
	static int		*m_lpCompetitorBumpObstacleNow;


protected:

public:

	//▼ 静的メンバ変数の定義
	static const int	COMPETITORMANAGER_IMAGE_MAX;		//使用画像枚数
	static const int	COMPETITORMANAGER_EFFECT_SOUND_MAX;	//使用効果音数
	static const HWND	m_hWnd;


	CCompetitorManager();
	~CCompetitorManager();

	//▼プロトタイプ宣言
	//競争者の初期化
	void	InitCompetitorManager( int runnerMax , CRoadEnemyManager	*lpRoadEnemy , float roadDistance );
	//競争者の更新
	void	UpdateCompetitorManager();
	//競争者の表示
	void	DrawCompetitorManager();
	//競争者の解放
	void	ReleaseCompetitorManager();
	
	//各種取得用
	//各競争者の座標を返す
	TPOINT<float>*	GetCompetitorManagerPosition() const { return this->m_lpCompetitorPos; }
	//効果を発揮した障害物の要素を取得する
	int*	GetCompetitorManagerObstacleMaterial() const { return this->m_lpCompetitorBumpObstacleNow; }
	//競争者の最大数
	int		GetCompetitorMax() const { return this->m_CompetitorMax; }

	//各種設定用
	//障害物の要素
	void	SetCompetitorManagerObstacleMaterial( int material , int runner );

};


#endif
