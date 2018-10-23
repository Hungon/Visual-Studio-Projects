#ifndef		CROADENEMY_H__
#define		CROADENEMY_H__


//▼ヘッダーをインクルード
#include	"graphics.h"
#include	"animation.h"
#include	"CRunner.h"


//障害物のタイプ番号（画像用）
enum tagOBSTACLETYPE{

	ROADENEMY_OBSTACLE_TYPE_HURDLE  ,			//ハードル
	ROADENEMY_OBSTACLE_TYPE_ROCK    ,			//岩石
	ROADENEMY_KIND_OF_OBSTACLE		,			//障害物の種類
};

//障害物の種類番号（キャラクター行動分岐用）
const BYTE	ROADENEMY_OBSTACLE_BIT_TYPE_VACANT	= 0x00;	//障害物なし
const BYTE	ROADENEMY_OBSTACLE_BIT_TYPE_HURDLE	= 0x01;	//ハードル
const BYTE	ROADENEMY_OBSTACLE_BIT_TYPE_ROCK	= 0x02;	//岩石

//ハードルの種類
enum tagHURDLECOLORTYPE{
	 ROADENEMY_OBSTACLE_HURDLE_COLOR_BLACK	,		//黒
	 ROADENEMY_OBSTACLE_HURDLE_COLOR_RED	,		//赤
	 ROADENEMY_OBSTACLE_HURDLE_COLOR_BLUE	,		//青
	 ROADENEMY_OBSTACLE_HURDLE_COLOR_GREEN	,		//緑
	 ROADENEMY_OBSTACLE_HURDLE_KIND_OF_COLOR,	//色の種類
};

//▼構造体の定義
//障害物の座標を扱う構造体
typedef struct tagOBSTACLE{

	CHARACTER ch;		//CHARACTER構造体
	ANIMATION ani;		//ANIMATION構造体
	FPOSITION fitPos;	//障害物の座標変更用
	BYTE bitType;		//障害物のビット目を扱う
	char colorType;		//色の種類

}OBSTACLE , *LPOBSTACLE;


//▼前方宣言
class	CCompetitorManager;
class	CPlayerRunner;

//障害物クラス
class	CObstacle{

private:

	//競争者管理クラス
	CCompetitorManager	*m_lpCCompetitorManager;

	//プレイヤークラス
	CPlayerRunner		*m_lpCPlayerRunner;

	//画像の要素を扱う
	int		*m_lpRoadEnemyImg;

	//障害物画像の情報を扱う
	OBSTACLE	*m_lpRoadObstacle;

	//効果音の要素数
	int		*m_lpRoadEnemyEffectSound;

	//各種障害物カウント用
	int		*m_lpObstacleCnt;

	//各種障害物の生成カウント用
	int		*m_lpCreationOBstacleCnt;

	//生成するハードルの種類の確率
	int		*m_lpCreateRateToHurdle;

	//各種ハードルの数カウント用
	int		*m_lpCreateHurdleCnt;

	//各種ハードルの最大生成数
	int		*m_lpCreateHurdleCntMax;

	//配列のランダムの値を扱う
	int		m_ObstacleRandomArrangeMax;

	//配列の種類のランダム値を扱う
	int		m_ObstacleInitArrangeRandomType;

	//障害物の最大生成数
	int		m_ObstacleCreateMax;

	//生成した座標
	float	m_CreatedObstacleY;

	//障害物を生成する座標間隔
	int		m_CreateObstacleSpaceY;


protected:

public:

	//▼ 静的メンバ変数の定義
	//画像
	static const int ROADENEMY_IMAGE_MAX;  		//画像の使用枚数

	static const int ROADENEMY_OBSTACLE_MAX;	//障害物の出現総数

	//効果音
	static const int ROADENEMY_SOUND_EFFECT_MAX; 	//使用効果音数

	//画面スクロールに合わせて拡大する障害物の画面下端での最終的なX座標のずれ
	static const float OBSTACLE_FITTING_SCROL_POSITION_X;

	//横一列に並べる最大数
	static const int	OBSTACLE_HURDLE_LINE_MAX;
	//横一列に並べる際の配列の種類
	static const int	OBSTACLE_HURDLE_LINE_ARRANGE_MAX;

	//配列の種類
	static const char	OBSTACLE_HURDLE_ARRANGE_TYPE_STRAIGHT_LINE;  		//横一直線に道を塞ぐ
	static const char	OBSTACLE_HURDLE_ARRANGE_TYPE_STRAIGHT_RANDOM;  		//横一直線にランダムに配置
	static const char	OBSTACLE_HURDLE_KIND_OF_ARRANGE;  					//種類数

	//ハードルの配置始点座標
	static const float OBSTACLE_HURDLE_POSITION_START_X;		//X
	//ハードルの間隔
	static const float OBSTACLE_HURDLE_SPACE_X;					//X

	//各種障害物の表示設定
	//幅
	static const int ROADENEMY_OBSTACLE_HURDLE_SIZE_WIDTH;
	static const int ROADENEMY_OBSTACLE_ROCK_SIZE_WIDTH;
	//高さ
	static const int ROADENEMY_OBSTACLE_HURDLE_SIZE_HEIGHT;
	static const int ROADENEMY_OBSTACLE_ROCK_SIZE_HEIGHT;
	//拡縮倍率
	static const float ROADENEMY_OBSTACLE_HURDLE_SIZE_SCALE;
	static const float ROADENEMY_OBSTACLE_ROCK_SIZE_SCALE;
	//移動量Y絶対値
	static const float ROADENEMY_OBSTACLE_HURDLE_ABSOLUTE_MOVE_Y;
	static const float ROADENEMY_OBSTACLE_ROCK_ABSOLUTE_MOVE_Y;
	//移動量X絶対値
	static const float ROADENEMY_OBSTACLE_HURDLE_ABSOLUTE_MOVE_X;
	static const float ROADENEMY_OBSTACLE_ROCK_ABSOLUTE_MOVE_X;

	//各種障害物のアニメーション設定
	//フレーム数
	static const int ROADENEMY_OBSTACLE_HURDLE_ANIMATION_FRAM;
	static const int ROADENEMY_OBSTACLE_ROCK_ANIMATION_FRAM;
	//コマ数
	static const int ROADENEMY_OBSTACLE_HURDLE_ANIMATION_CNT_MAX;
	static const int ROADENEMY_OBSTACLE_ROCK_ANIMATION_CNT_MAX;
	//障害物のアニメーションフラグ
	static const BYTE ROADENEMY_ANIMATION_FLAG_NORMAL;	//通常状態
	static const BYTE ROADENEMY_ANIMATION_FLAG_BREAK;	//破壊フラグ


	//衝突する面を扱う
	static const char OBSTACLE_NOT_BUMP;		//衝突なし
	static const char OBSTACLE_BUMP_TOP;		//上
	static const char OBSTACLE_BUMP_BOTTOM;		//下
	static const char OBSTACLE_BUMP_LEFT;		//左
	static const char OBSTACLE_BUMP_RIGHT;		//右


	//▼ メンバ関数の定義

	CObstacle();
	CObstacle( int max );
	~CObstacle();

	void InitRoadEnemy(	CCompetitorManager	*lpCCompetitorManager , CPlayerRunner *lpCPlayerRunner );			//初期化
	void UpdateRoadEnemy();			//更新
	void DrawRoadEnemy();			//表示
	void ReleaseRoadEnemy();		//解放

	//障害物の生成
	void	CreateObstacle( int type );

	//当たり判定
	char	CollisionRoadEnemy( RUNNER* pRunner );

	//障害物1「ハードル」の行動の初期化
	bool	InitObstacleHurdle( int material );

	//********************************************************************************************
	//障害物1「ハードル」配置の初期化
	//横一直線に配置する
	//********************************************************************************************
	bool	InitArrangeHurdleStraightLine( int material );

	/**************************************************************************************************
	ハードルの配置の初期化
	横一直線にランダムで並べる
	***************************************************************************************************
	*/
	bool	InitArrangeHurdleRandomStraightLine( int material );

	//障害物1「ハードル」の行動の更新
	void	UpdateActionObstacleHurdle( int material );

	//障害物2「岩石」の行動の初期化
	bool	InitObstacleRock(int material);

	//障害2「岩石」物の行動の更新
	void	UpdateActionObstacleRock( int material );

	//引数のキャラクターの索敵範囲と判定を取る
	BYTE	BroadenCollisionSquareObstacle( CHARACTER* pch );

	//仮の当たり判定を取る
	BYTE	AttemptCollisionSquare( CHARACTER* pch1 );
	/*
	initialize process for hurdle when player bump to hurdle
	*/
	char	CollisionInitializeHurdle(RUNNER *pRunner, int material);
	/*
	initialize process for rock when player bump to hurdle
	*/
	char	CollisionInitializeRock(RUNNER *pRunner);


	//▼ 各種設定用
	//障害物用
	void	SetRoadEnemyImage( int *lpImage , int max );
	//障害物の効果音
	void	SetRoadEnemyEffectSound( int *lpSound , int max );


};

#endif