#ifndef		COMPETITORRUNNER_H__
#define		COMPETITORRUNNER_H__


//▼マクロの定義
//行動パターンの種類数
enum tagCOMPETITORACTIONTYPE{

	COMPETITOR_RUNNER_ACTION_TYPE_VACANT = -1 ,			//アクションなし
	COMPETITOR_RUNNER_ACTION_TYPE_JUMP		  ,			//ジャンプ
	COMPETITOR_RUNNER_ACTION_TYPE_ATTACK	  ,			//攻撃
	COMPETITOR_RUNNER_ACTION_TYPE_EVADE		  ,			//回避
	COMPETITOR_RUNNER_KIND_OF_ACTION		  ,			//アクションの種類

};

//▼ヘッダーをインクルード
#include	"CRunner.h"
#include	"CCharacter.h"

//▼　前方宣言
class	CEffectManager;
class	CRoadEnemyManager;
class	CPlayerRunner;

//▼ 競争者クラス
class	CCompetitor : public CRunner{

private:

	//画像の要素番号
	int		m_CompetitorRunnerImg;

	//影用の画像要素
	int		m_CompetitorShadowImg;

	//効果音の要素
	int		m_CompetitorEffectSound;

	//各障害物に対するアクション発動率
	int		*m_lpActionRateToHurdle;	//ハードル
	int		*m_lpActionRateToRock;		//岩石

	//現在の自身の位置とプレイヤーの位置に応じた速度の変化
	float	m_VariableSpeedNow;
	//反映させる速度変化値
	float	m_GetVariableSpeed;

	//速度制限
	float	m_ConstarinSpeedMax;

	//競争者管理クラス
	CRoadEnemyManager	*m_lpCRoadEnemy;
	//プレイヤークラス
	CPlayerRunner		*m_lpCPlayerRunner;

	//飛び越えた障害物の要素を扱う（取得用）
	static int m_getJumpedMaterial;


protected:

public:

	//座標取得用
	static TPOINT<float>	m_CompetitorPos;

	//▼ 静的メンバ変数の定義
	//エフェクト効果の種類
	static const int	COMPETITOR_KIND_OF_EFFECT;
	//表示間隔
	static const int	COMPETITOR_INDICATE_SPACE_X;

	//画面外に出た際の制限座標
	static const float	COMPETITOR_CONSTRAIN_FRONT_POSITION_Y;			//前方
	static const float	COMPETITOR_CONSTRAIN_BACK_POSITION_Y;			//後方

	//コンストラクタ
	CCompetitor();
	//ディストラクタ
	~CCompetitor();

	//▼プロトタイプ宣言
	//オーバーライド
	//競争者の初期化
	void InitRunner( int runnerNum , CRoadEnemyManager	*lpRoadEnemy , float roadDistance );
	//競争者の更新
	void UpdateRunner( int runner , CEffectManager *lpCEffect );
	//競争者の表示
	void DrawRunner();
	//競争者の解放
	void ReleaseRunner();

	//競争者の速度を管理する
	void ManageCompetitorRunnerSpeed();
	//競争者の移動範囲を制限する
	void ManageConstrainCompetitorRunnerMove();
	/*
	障害物の種類に応じて適切な行動を確率で判断する
	*/
	void SetActionToObstacle();
	//行動パターン１の初期化
	void SetCompetitorRunnerLineMove();
	//行動パターン２の初期化
	void SetCompetitorRunnerEvadeHardol();
	//行動パターン３の初期化
	void SetCompetitorRunnerAttack();
	//行動パターン１の横に回避
	void UpdateCompetitorRunnerLineMove();
	//行動パターン２の回避行動
	void UpdateCompetitorRunnerEvadeHardol();
	//行動パターン３の攻撃の更新
	void UpdateCompetitorRunnerAttack();
	//引数のキャラクター構造体の索敵範囲とで判定を取る
	bool BroadenCollisionSquareCompetitor( CHARACTER* pch );
	//アニメーションの更新
	void ChangeCompetitorRunnerAnimation();
	/*****************************************************************************************************
	引数の値で確率で行動を決める
	引数１：確率　1～100
	引数２：行いたいアクションの種類番号

	戻り値：アクションの種類番号
	※失敗したら0を返す
	******************************************************************************************************/
	char ConmpetitorAttemptDoAction( char likelihood , BYTE actionType );

	/*
	********************************************************************************
	ジャンプで飛び越えた障害物の要素を設定する
	********************************************************************************
	*/
	void	SetCompetitorJumpedObstacleMaterial( int material ){ this->m_RunnerDefault.obstacleBumpNow = material; }

	//画像要素設定用
	//競争者用
	void	SetCompetitorImage( int image ) { this->m_CompetitorRunnerImg = image; }
	//競争者の影用
	void	SetCompetitorShadowImage( int image ) { this->m_CompetitorShadowImg = image; }
	//競争者の効果音
	void	SetCompetitorEffectSound( int SE ) { this->m_CompetitorEffectSound = SE; }


	//各種取得用
	/*
	********************************************************************************
	ジャンプで飛び越えた障害物の要素を取得する
	********************************************************************************
	*/
	int			GetCompetitorJumpedObstacleMaterial(){ return this->m_getJumpedMaterial; }
	
	//座標
	TPOINT<float>	GetOneCompetitorPosition() const { return this->m_CompetitorPos; }



};


#endif
