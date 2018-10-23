#ifndef		CRUNNER_H__
#define		CRUNNER_H__


//▼ヘッダーをインクルード
#include	"graphics.h"
#include	"animation.h"


//▼ マクロの定義
//ランナーの画像設定
const int RUNNER_DEFAULT_IMAGE_WIDTH	= 32;			//幅
const int RUNNER_DEFAULT_IMAGE_HEIGHT	= 96;			//高さ
const float RUNNER_DEFAULT_IMAGE_SCALE = 1.0f;		//拡縮倍率

//アニメーション設定
//通常時
const int RUNNER_ANIMATION_TYPE_NORMAL_CNT_MAX	= 4;			//最大コマ数
const int RUNNER_ANIMATION_TYPE_NORMAL_FRAM		= 10;			//フレーム数
//ジャンプ
const int RUNNER_ANIMATION_TYPE_JUMP_CNT_MAX	= 18;			//最大コマ数
const int RUNNER_ANIMATION_TYPE_JUMP_FRAM		= 2;			//フレーム数
//攻撃
const int RUNNER_ANIMATION_TYPE_ATTACK_CNT_MAX	= 14;			//最大コマ数
const int RUNNER_ANIMATION_TYPE_ATTACK_FRAM		= 2;			//フレーム数


//▼ステータス
//体力
const int RUNNER_DEFAULT_STAMINA_MAX	 = 3;			

//移動
const float RUNNER_DEFAULT_LINE_MOVE		= 5.0f;			//横軸の移動量
const float RUNNER_DEFAULT_MOVE_RIGHT		= 5.0f;			//左移動
const float RUNNER_DEFAULT_MOVE_LEFT		= -5.0f;		//右移動
const float RUNNER_DEFAULT_MOVE_FRONT		= -5.0f;		//Y移動量「上移動」
const float RUNNER_DEFAULT_MOVE_BACK		= 5.0f;			//Y移動量「下移動」

//速度
const float RUNNER_DEFAULT_MOVE_Y			= -3.0f;		//Y移動量
const float RUNNER_DEFAULT_SPEEDMAX_RATE	= 3.5f;			//速度倍率上限
const float RUNNER_DEFAULT_SPEEDMIN_RATE	= 0.5f;			//速度倍率下限
const float RUNNER_DEFAULT_SPEED_RATE		= 1.0f;			//速度倍率
const int RUNNER_DEFAULT_CHARGEMAX_RATE	= 500;			//速度チャージ値の最大値
const int RUNNER_DEFAULT_AVOID_TIME		= 100;			//回避時間
//速度制限
const float RUNNER_CONSTRAIN_MINIMUM_SPEED = -3.0f;

//アニメーションを扱う
const BYTE	RUNNER_ANIMATION_ROUND		= 0x00;				//接地状態
const BYTE	RUNNER_ANIMATION_JUMP		= 0x01;				//ジャンプ中
const BYTE	RUNNER_ANIMATION_ATTACK		= 0x02;				//攻撃中
const BYTE	RUNNER_ANIMATION_MAX		= 3;		   		//変化するアニメーションの種類

//移動方向を扱う
const BYTE RUNNER_NOT_MOVE				= 0x00;			//停止状態
const BYTE RUNNER_MOVE_DIRECTION_RIGHT  = 0x01;			//右
const BYTE RUNNER_MOVE_DIRECTION_LEFT	= 0x02;			//左
const BYTE RUNNER_MOVE_DIRECTION_FRONT	= 0x03;			//前
const BYTE RUNNER_MOVE_DIRECTION_BACK	= 0x04;			//後

//回避フラグ
const BYTE RUNNER_SAFTY				= 0x00;				//安全フラグ
const BYTE RUNNER_BUMP_ATTENTION	= 0x01;				//衝突フラグ
const BYTE RUNNER_WHILE_AVOID		= 0x02;				//回避中フラグ

//行動フラグ
const BYTE RUNNER_ACTION_ROUND				= 0x00;				//接地フラグ
const BYTE RUNNER_ACTION_JUMP				= 0x01;				//ジャンプフラグ
const BYTE RUNNER_ACTION_ATTACK				= 0x02;				//攻撃フラグ
const BYTE RUNNER_ACTION_JUMP_AND_ATTACK	= 0x03;				//ジャンプ＆攻撃フラグ
const BYTE RUNNER_ACTION_LINE_MOVE			= 0x04;				//横移動フラグ
const BYTE RUNNER_KIND_OF_ACTION			= 5;				//アクションの種類数

//障害物との判定によるフラグ
const char RUNNER_NOT_BUMP				= -1;						//衝突なし
const char RUNNER_BUMP_TO_OBSTACLE		= 0;						//衝突フラグ
const char RUNNER_EFFECT_FLAG_SPEED_UP	= 1;						//速度上昇フラグ
const char RUNNER_EFFECT_FLAG_ABSOLUTE	= 2;						//無敵フラグ
const char RUNNER_EFFECT_FLAG_HEALING	= 3;						//回復フラグ	
const char RUNNNER_BUMP_TO_HURDLE		= 4;						//bump to hurdle
const char RUNNNER_BUMP_TO_ROCK			= 5;						//bump to rock
//強化効果を扱うタイプ番号
const BYTE RUNNER_TYPE_NORMAL					= 0x00;
const BYTE RUNNER_EFFECT_TYPE_SPEED_UP			= 0x01;				//速度上昇
const BYTE RUNNER_EFFECT_TYPE_ABSOLUTE			= 0x02;				//無敵状態
const BYTE RUNNER_EFFECT_TYPE_SPEED_AND_ABSOLUTE = 0x03;			//速度＆無敵
const BYTE RUNNER_EFFECT_TYPE_HEALING			= 0x04;				//回復
const BYTE RUNNER_EFFECT_KIND_OF_TYPE			= 3;				//強化タイプの種類

//無敵効果時間
const short RUNNER_ABSOLUTE_EFFECT_TIME = 1000;

//索敵範囲
const int RUNNER_DEFAULT_SEARCH_FRONT	= -70;				//前方範囲
const int RUNNER_DEFAULT_SEARCH_WIDE	= 96;				//幅範囲



//速度に応じたアニメーションを扱う
enum tagRUNNERANIMATIONSPEED{

	RUNNER_SPEED_SLOW	= 40 ,			//遅い	
	RUNNER_SPEED_NORMAL = 20 ,			//普通
	RUNNER_SPEED_DECENT = 15 ,			//結構速い
	RUNNER_SPEED_RAPID	= 10 ,			//速い
	RUNNER_SPEED_ANIMATION_RATE = 2 ,	//アニメーションの速度を変化させる倍率
};

//▼構造体の定義
//ランナー用
typedef struct tagRUNNER{

	CHARACTER ch;						//CHARACTER構造体
	ANIMATION ani;						//ANIMATION構造体
	float pos_x;						//位置変更用X
	float pos_y;						//位置変更用Y
	int speed_charge;					//スピードチャージ値
	int speedChargeMax;					//スピードチャージ値の上限
	float speed_max;					//スピード最大値
	float speed_min;					//スピード最小値
	tagRUNNERANIMATIONSPEED speedLevel;	//速度に応じたアニメーションを変化させる
	int avoidtime;						//回避時間
	int speedTime;						//速度に扱う時間
	int absoluteTime;					//無敵時間を扱う
	BYTE actionF;						//行動フラグ
	BYTE avoidF;						//回避フラグ
	char actionType;					//行動種類
	int avoidDistance;					//回避移動距離を計る
	int designateDistance;				//回避先までの移動距離
	BYTE getType;						//種類を受け取る
	float aggregateSpeed;				//速度の合計を扱う
	BYTE effectType;					//強化効果のタイプ
	char bumpType;						//障害物との衝突時のタイプ分け用
	int obstacleBumpNow;				//現在の衝突中障害物の要素数用

	//記録用
	int jumpedHurdleCnt[3];				//ハードルを飛び越えた数
	int jumpedHurdleChain;				//ハードルを連続で飛び越えた数
	int jumpedHurdleChainMax;			//ハードルを連続で飛び越えた最大数
	int defeatCnt[2];					//破壊した数

}RUNNER , *LPRUNNER;


//▼ 前方宣言
class	CEffectManager;
class	CRoadEnemyManager;

//ランナーの基底クラス
class	CRunner{

private:

protected:

	//走者のデフォルト構造体
	RUNNER m_RunnerDefault;

	//走者の影のデフォルト構造体
	RUNNER m_ShadowDefault;

public:


	//▼ 静的メンバ変数の定義

	//▼関数のプロトタイプ宣言
	CRunner();
	~CRunner();

	//初期化
	virtual void InitRunner( int runnerNum , CRoadEnemyManager	*lpRoadEnemy ){}
	//更新
	virtual void UpdateRunner( int runner , CEffectManager *lpCEffect ){}
	//表示
	virtual void DrawRunner() = 0;
	//解放
	virtual void ReleaseRunner() = 0;
	
	/*************************************************************************************************
	ハードルを飛び越えた際のプレイヤーの強化効果１「速度上昇」
	初期化関数
	**************************************************************************************************
	*/
	void RunnerEffectSpeedUp();
	/*************************************************************************************************
	ハードルを飛び越えた際のプレイヤーの強化効果２「無敵状態」
	初期化関数
	**************************************************************************************************
	*/
	void RunnerEffectAbsolute();
	/*************************************************************************************************
	ハードルを飛び越えた際のプレイヤーの強化効果１「速度上昇」
	更新関数
	**************************************************************************************************
	*/
	void RepealRunnerEffectSpeedUp();
	/*************************************************************************************************
	時間経過で無敵状態を無効にする
	**************************************************************************************************
	*/
	bool RepealRunnerEffectAbsolute();
	//************************************************************************************************
	//当たり判定　CollisionRunner
	//************************************************************************************************
	bool CollisionRunner( CHARACTER* pch1 , CHARACTER* pch2 );

};


#endif
