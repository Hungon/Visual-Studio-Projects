#ifndef OFFROADRACER_H__
#define OFFROADRACER_H__


//▼ ヘッダーのインクルード
#include	"graphics.h"
#include	"animation.h"
#include	<windows.h>
#include	"OffRoadAction.h"
#include	"OffRoadScoreManager.h"

//列挙型
//レーサーのスコアを扱う
enum tagRACERSCORE {
	OFFROADRACER_SCORE_ACTION_POINT,					//アクションポイント			
	OFFROADRACER_SCORE_ACTION_CONSECUTIVE_MAX,			//連続で成功したアクション最大数
	OFFROADRACER_KIND_OF_SCORE,							//スコアの種類数
};

//kind of racer's action
enum tagRACERTECHNIQUE {
	RACERTECHNIQUE_TYPE_NORMAL,					//run on the road
	RACERTECHNIQUE_TYPE_RIGHT_ROTATE,			//once turn right in the sky
	RACERTECHNIQUE_TYPE_LEFT_ROTATE,			//once turn left in the sky
	RACERTECHNIQUE_TYPE_RIGHT_HALF_ROTATE,		//half turn right in the sky
	RACERTECHNIQUE_TYPE_LEFT_HALF_ROTATE,		//half trun left in the sky
	RACERTECHNIQUE_KIND_OF_TYPE,				//kind of technique
};
//racer's sizes
const int OFFROADRACER_IMAGE_RACER_WIDTH = 32;
const int OFFROADRACER_IMAGE_RACER_HEIGHT = 64;	
//set animation's value
//for normal style
const int OFFROADRACER_ANIMATION_TYPE_NORMAL_CNT_MAX = 2;			//max count
const int OFFROADRACER_ANIMATION_TYPE_NORMAL_FRAM = 5;				//fram speed
//for once rotate
const int OFFROADRACER_ANIMATION_ROTATE_IMAGE_WIDTH = 64;			//change width
const int OFFROADRACER_ANIMATION_TYPE_ROTATE_CNT_MAX = 19;			//count max
const int OFFROADRACER_ANIMATION_TYPE_ROTATE_FRAM = 2;				//fram speed
//for half rotate
const int OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_CNT_MAX = 5;		//count max
const int OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_FRAM = 2;			//fram speed


//▼ 前方宣言
class	COffRoadStage;			//ステージクラス
class   COffRoadScoreManager;	//スコア管理クラス

//▼オフロードプレイヤークラスの定義
class COffRoadRacer : public COffRoadAction {

private:

	//画像の要素を扱う
	int		*m_lpCOffRoadRacerImg;
	
	//効果音の要素を扱う
	int		*m_lpOffRoadRacerSe;

	//スコア管理クラス
	COffRoadScoreManager	*m_lpScoreManager;

	//画像設定用
	CHARACTER		m_OffRoadRacerCh;

	//アニメーション設定用
	ANIMATION		m_OffRoadRacerAni;

	//ステージのクラス
	COffRoadStage	*m_lpCOffRoadStage;

	//ライダーの速度の合計を扱う
	static float	m_AggregateSpeed;

	//ライダーの現在の座標を取得
	static TPOINT<float>	m_RiderPosition;

	//成功したアクションコマンドのビット目を扱う
	static BYTE	m_CommandSuccess;

	//実行したアクションコマンドのビット目を扱う
	BYTE	m_DidCommandAction;

	//プレイヤーのアクションフラグ
	static bool	m_actionFlag;

	//アクション実行時の拡縮倍率の加算（減算）値を扱う
	float	m_actionScaleAdd;
	float	m_actionScaleSub;

	//process for scale up score number image
	float	*m_lpScoreScaleAdd;			//add scale rate to score image for racer's action
	int		*m_lpNumberAfterScaleUp;	//treat after scale up number

	//アクションポイント
	static int		m_ActionPoint;
	//アクションポイントの表示先座標
	static TPOINT<float>	m_IndicateActionPoint;
	
	//成功したアクションの成功数を収める
	static int	m_SuccessActionCount[OFFROADACTION_COMMAND_KIND_OF_ACTION];
	//連続でアクション成功した数を扱う
	static int	m_SucceedActoinConsecutive;
	//連続でアクション成功した最大数を扱う
	static int	m_SucceedActoinConsecutiveMax;
	//連続で成功したアクション数の表示先座標
	static TPOINT<float>	m_IndicateActionConsecutive;

	//count time for scale up score image
	int		*m_lpScoreScaleUpCountTime;

protected:


public:

	//▼ 静的メンバ変数の定義
	static const int OFFROADRACER_IMAGE_MAX;			//画像使用枚数
	static const int OFFROADRACER_EFFECT_SOUND_MAX;		//効果音使用数


	//ウィンドウハンドル
	static HWND	m_hWnd;

	//各種初期値
	//表示座標（ローカル座標）
	static const float OFFFROADRACER_DEFAULT_POSITION_X;		//表示座標X
	static const float OFFFROADRACER_DEFAULT_POSITION_Y;		//表示座標Y
	//移動量
	static const float OFFFROADRACER_DEFAULT_MOVE_UP;			//前方
	static const float OFFFROADRACER_DEFAULT_MOVE_RIGHT;		//右
	static const float OFFFROADRACER_DEFAULT_MOVE_BACK;			//後方
	static const float OFFFROADRACER_DEFAULT_MOVE_LEFT;			//左
	//速度倍率
	static const float	OFFROADRACER_DEFAULT_SPEED_RATE;
	//画像サイズ
	static const int OFFROADRACER_IMAGE_WIDTH;					//幅
	static const int OFFROADRACER_IMAGE_HEIGHT;					//高さ
	//拡縮倍率
	static const float OFFFROADRACER_DEFAULT_SCALE;
	//拡縮倍率の変数値
	//加算値
	static const float	OFFROADRACER_VARIABLE_SCALE_RATE_ADD_EASY;
	static const float	OFFROADRACER_VARIABLE_SCALE_RATE_ADD_NORMAL;
	static const float	OFFROADRACER_VARIABLE_SCALE_RATE_ADD_HARD;
	//減算値
	static const float	OFFROADRACER_VARIABLE_SCALE_RATE_SUB_EASY;
	static const float	OFFROADRACER_VARIABLE_SCALE_RATE_SUB_NORMAL;
	static const float	OFFROADRACER_VARIABLE_SCALE_RATE_SUB_HARD;


	//アニメーション設定
	//通常時
	static const int OFFROADRACER_ANIMATION_TYPE_NORMAL_CNT_MAX;	//最大コマ数
	static const int OFFROADRACER_ANIMATION_TYPE_NORMAL_FRAM;		//フレーム
	
	//回転アニメーション時
	static const int OFFROADRACER_ANIMATION_ROTATE_IMAGE_WIDTH;		//幅
	static const int OFFROADRACER_ANIMATION_TYPE_ROTATE_CNT_MAX;	//最大コマ数
	static const int OFFROADRACER_ANIMATION_TYPE_ROTATE_FRAM;		//フレーム

	//半回転アニメーション時
	static const int OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_CNT_MAX;//最大コマ数
	static const int OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_FRAM;	//フレーム

	//アニメーション時のビット目
	//回転アニメーション
	static const BYTE OFFROADRACER_ANIMATION_TYPE_NORMAL;			//通常時
	static const BYTE OFFROADRACER_ANIMATION_TYPE_RIGHT_HALF_ROTATE;//右半回転
	static const BYTE OFFROADRACER_ANIMATION_TYPE_LEFT_HALF_ROTATE;	//左半回転
	static const BYTE OFFROADRACER_ANIMATION_TYPE_RIGHT_ROTATE;		//右1回転
	static const BYTE OFFROADRACER_ANIMATION_TYPE_LEFT_ROTATE;		//左1回転
	//アニメーション時の表示元座標Y
	static const int OFFROADRACER_ANIMATION_TYPE_NORMAL_PICY;				//通常時
	static const int OFFROADRACER_ANIMATION_TYPE_RIGHT_HALF_ROTATE_PICY;	//右半回転
	static const int OFFROADRACER_ANIMATION_TYPE_LEFT_HALF_ROTATE_PICY;	//左半回転
	static const int OFFROADRACER_ANIMATION_TYPE_RIGHT_ROTATE_PICY;		//右1回転
	static const int OFFROADRACER_ANIMATION_TYPE_LEFT_ROTATE_PICY;			//左1回転

	//アニメーションの種類
	static const int OFFROADRACER_ANIMATION_KIND_OF_TYPE;

	//アクション時のプレイヤーの最大拡縮倍率
	static const float OFFROADRACER_ACTION_SCALE_MAX;

	//アクションポイントの表示桁数
	static const int	OFFROADRACER_ACTION_POINT_DIGIT;
	//アクションコマンド最大入力数
	static const int OFFROADRACER_COMMAND_INPUT_MAX;
	//アクションコマンド成功時の速度変化値
	static const float OFFROADRACER_SUCCEED_ACTION_VARIABLE_SPEED_RATE;

	//process for scale of total number score image
	//default add scale rate
	static const float OFFROADRACER_TOTAL_ACTION_POINT_ADD_SCALE_RATE;
	//add scale max
	static const float OFFROADRACER_TOTAL_ACTION_POINT_ADD_SCALE_RATE_MAX;
	//starting point of scale up
	static const int OFFROADRACER_TOTAL_ACTION_POINT_STARTING_POINT_SCALE_UP;

	//process for scale of action consecutive number score image
	//default add scale rate
	static const float OFFROADRACER_CONSECUTIVE_ACTION_ADD_SCALE_RATE;
	//add scale max
	static const float OFFROADRACER_CONSECUTIVE_ACTION_ADD_SCALE_RATE_MAX;
	//scale up consecutive success count
	static const int OFFROADRACER_CONSECUTIVE_ACTION_SCALE_UP_NUMBER_SUCCESS_COUNT;
	//limit time for scale up of score image
	static const int OFFROADRACER_LIMIT_TIME_FOR_SCALE_UP_SCORE_IMAGE;

	//blank time that before player can input
	static const int OFFROADRACER_BLANK_TIME_FOR_INPUT;


	//コンストラクタ
	COffRoadRacer() :	
		//画像の要素を扱う
		m_lpCOffRoadRacerImg(NULL) ,
		//ステージのクラス
		m_lpCOffRoadStage(NULL)	   ,
		m_actionScaleAdd(NULL)	   ,
		m_actionScaleSub(NULL)	   
	{}

	//ディストラクタ
	~COffRoadRacer();
	
	//▼ 自作メンバ関数の定義
	//オフロードプレイヤーの初期化
	void InitOffRoadRacer();
	//道路の更新
	void UpdateOffRoadRacer();
	//オフロードプレイヤーの表示
	void DrawOffRoadRacer();
	//解放
	void ReleaseOffRoadRacer();
	//移動
	void MoveOffRoadRacer();
	//移動範囲を制限する
	void ConstrainOffRoadRacerMove();
	//ライダーのアニメーションを更新
	void ChangeAnimationOffRoadRacer();

	/*
	ライダーの回転アクションの設定
	*/
	void SetRotateActionOffRoadRacer();
	/*
	ライダーの回転アクションの更新
	*/
	void UpdateRotateActionOffRoadRacer();
	/*
	Starting point of scale up for number image
	*/
	void	RacerStartingPointScaleUpScoreNumber();



	
	//取得用
	//ライダーの座標を取得
	TPOINT<float>	GetRiderPosition() const { return this->m_RiderPosition; }
	//ライダーの速度取得用
	float GetOffRoadRacerSpeed() const { return this->m_AggregateSpeed; }
	//ライダーの入力情報取得用
	BYTE	GetOffRoadRacerCommandInfo() const { return this->m_CommandSuccess; }
	//アクションポイント表示座標
	TPOINT<float>	GetActionPointIndicate() const { return this->m_IndicateActionPoint; }
	//アクションポイント取得用
	int		GetTotalActionPoint() const { return this->m_ActionPoint; }
	//成功したアクションの数取得用
	int*	GetSuccessActionCount() const { return this->m_SuccessActionCount; }
	//連続アクション成功数を取得
	int		GetScceedActionConsecutive() const { return this->m_SucceedActoinConsecutiveMax; }
	//連続で成功したアクション数表示先座標を返す
	TPOINT<float>	GetOffRoadRacerIndicateConsecutiveAction() const { return this->m_IndicateActionConsecutive; }
	//アクション中かを返す
	bool	GetRacerActionFlag() const { return this->m_actionFlag; }

	//設定用
	void SetOffRoadRacerSpeed( float AggregateSpeed ) { this->m_AggregateSpeed = AggregateSpeed; }
	//ライダーの入力情報取得用
	void	SetOffRoadRacerCommandInfo( BYTE CommandSuccess );


};


#endif
