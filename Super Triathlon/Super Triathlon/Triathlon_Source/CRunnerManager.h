#ifndef		CRUNNERMANAGER_H__
#define		CRUNNERMANAGER_H__

//▼ヘッダーをインクルード
#include	"graphics.h"
#include	"CCharacter.h"

//▼ 前方宣言
class	CCompetitorManager;
class	CRoadEnemyManager;
class	CPlayerRunner;
class	CEffectManager;
class	CRoadStage;

//▼ 競争者クラス
class	CRunnerManager{

private:
	
		
	//画像の要素数
	int		*m_lpRunnerManagerImg;

	//競争者の数
	int		m_CompetitorMax;

	//プレイヤーの現在の順位
	static int		m_PlayerRanking;

	//ステージの全長
	float	m_StageWholeDistance;

	//順位設定用
	CHARACTER	m_RunnerRanking;
	//ミニマップ用
	CHARACTER	*m_lpMiniMap;

	//競争者の管理クラス
	static CCompetitorManager	*m_lpCCompetitorManager;
	//プレイヤークラス
	static CPlayerRunner		*m_lpCPlayerRunner;
	//エフェクト管理クラス
	CEffectManager				*m_lpCEffectManager;
	//ステージクラス
	CRoadStage					*m_lpCRoadStage;


protected:

public:

	//▼ 静的メンバ変数の定義
	static const HWND	m_hWnd;
	//画像の要素数
	static const int RUNNERMANAGER_IMAGE_MAX;

	//競争者の最大数
	static const int RUNNERMANAGER_COMPETITOR_MAX;

	//プレイヤーの順位
	static const float RUNNERMANAGER_RANKING_INDICATE_X;		//表示先X
	static const float RUNNERMANAGER_RANKING_INDICATE_Y;		//幅表示先Y
	static const int RUNNERMANAGER_RANKING_IMAGE_WIDTH;			//幅
	static const int RUNNERMANAGER_RANKING_IMAGE_HEIGHT;		//高さ
	static const float RUNNERMANAGER_RANKING_DEFAULT_SCALE;		//拡縮倍率

	//ミニマップ
	static const int RUNNERMANAGER_MINIMAP_IMAGE_MAX;			//画像の数
	static const float RUNNERMANAGER_MINIMAP_INDICATE_X;		//表示先X
	static const float RUNNERMANAGER_MINIMAP_INDICATE_Y;		//表示先Y
	static const int RUNNERMANAGER_MINIMAP_IMAGE_WDITH;			//幅
	static const int RUNNERMANAGER_MINIMAP_IMAGE_HEIGHT;		//高さ

	//ミニマップ上のプレイヤー
	static const int RUNNERMANAGER_MINIMAP_PLAYER_IMAGE_WDITH;		//幅
	static const int RUNNERMANAGER_MINIMAP_PLAYER_IMAGE_HEIGHT;		//高さ
	static const float RUNNERMANAGER_MINIMAP_PLAYER_DEFAULT_SCALE;	//拡縮倍率




	
	CRunnerManager();
	~CRunnerManager();

	//▼プロトタイプ宣言
	//競争者の初期化
	void	InitRunnerManager( CRoadEnemyManager	*lpRoadEnemy );
	//競争者の更新
	void	UpdateRunnerManager();
	//競争者の表示
	void	DrawRunnerManager();
	//競争者の解放
	void	ReleaseRunnerManager();

	//競争者のアドレス取得
	CCompetitorManager	*GetCCompetitorManager() const { return this->m_lpCCompetitorManager; }
	//プレイヤーのアドレス取得
	CPlayerRunner	*GetCPlayerRunner() const { return this->m_lpCPlayerRunner; }
	//Get player's ranking in this race
	int		GetPlayerRunnerRanking() const { return this->m_PlayerRanking; }
	

};


#endif
