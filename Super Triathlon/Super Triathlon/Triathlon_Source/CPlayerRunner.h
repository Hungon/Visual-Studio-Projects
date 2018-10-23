#ifndef  PLAYERRUNNER_H__
#define  PLAYERRUNNER_H__


//▼ヘッダーファイルの読み込み
#include "CRunner.h"
#include "score.h"

//▼ 前方宣言
class	CEffectManager;			//エフェクト管理クラス
class	CRoadEnemyManager;		//障害物管理クラス

//プレイヤーランナークラス
class	CPlayerRunner : public CRunner{

private:

	//画像の要素を扱う
	int		*m_lpPlayerRunnerImg;
	//効果音
	int		*m_lpPlayerRunnerEffectSound;

	//画像設定用
	//体力
	RUNNER	*m_lpPlayerStatus;

	//障害物管理クラス
	CRoadEnemyManager	*m_lpCRoadEnemyForPlayer;

	//情報取得用
	static RUNNER	m_GetRunner;
	
	//for draw chain
	SCOREVALUE	m_PlayerScore;

protected:

public:

	//▼ 静的メンバ変数の定義
	static const HWND	m_hWnd;
	//画像
	static const int PLAYER_RUNNER_IMAGE_MAX;		//使用枚数

	//音
	static const int PLAYER_RUNNER_EFFECT_SOUND_MAX;//使用数

	//ステータス情報
	//体力
	static const int PLAYER_RUNNER_STAMINA_MAX;	


	CPlayerRunner();
	~CPlayerRunner();
	//▼ 自作関数の定義
	//オーバーライド
	//ランナーの初期化
	void    InitRunner( int runnerNum , CRoadEnemyManager	*lpRoadEnemy , float roadDistance );
	//ランナーの更新
	void    UpdateRunner( int runner , CEffectManager *lpCEffect );
	//表示する関数
	void    DrawRunner();
	//解放を行う関数
	void    ReleaseRunner();

	//プレイヤーランナーの速度の更新
	void    UpdatePlayerRunnerSpeed();
	//プレイヤーランナーの移動の更新
	void    UpdatePlayerRunnerMove();
	//プレイヤーランナーのジャンプ処理の更新
	void    UpdateActionPlayerRunnerJump();
	//プレイヤーランナーの攻撃処理の更新
	void    UpdateActionPlayerRunnerAttack();
	//プレイヤーランナーのアニメーションを更新
	void    ChangePlayerRunnerAnimation();
	//移動を制限する
	void    ConstrainPlayerRunnerMove();
	//引数のキャラクターの索敵範囲で判定を取る
	bool	 BroadenCollisionSquarePlayer( CHARACTER* pch );
	/*************************************************************************************************
	ハードルを飛び越えた際のプレイヤーの強化効果３「回復」
	初期化関数
	**************************************************************************************************
	*/
	void    RunnerEffectHealing();

	//▼ 設定用関数
	//自身の情報を設定
	void	SetRunner( RUNNER* lpRunner ) { this->m_RunnerDefault = *( lpRunner ); }

	//▼ 取得用関数
	//自身の情報を返す
	RUNNER	*GetRunner() const { return &this->m_GetRunner; }
	
};


#endif