#ifndef COFFROADSCOREMANAGER_H__
#define COFFROADSCOREMANAGER_H__


//▼ヘッダーをインクルード
#include	"CCharacter.h"
#include	<windows.h>


//列挙型
//スコアの色を扱う
enum tagSCOREMANAGERCOLORTYPE {
	SCOREMANAGER_COLOR_BLACK,
	SCOREMANAGER_COLOR_RED,
	SCOREMANAGER_KIND_OF_COLOR,
};

//▼構造体の定義
//数値演出用
typedef struct tagSCOREDIRECT{

	int	indicateNum;
	int	restNum;
	int	terminateNum;
	int intervalFram;

} SCOREDIRECT;


//▼ 前方定義
class	COffRoadScore;
class	COffRoadRacer;
class	COffRoadAction;

//▼ オフロードスコアクラスの定義
class COffRoadScoreManager{

private:

	//画像の要素を扱う
	int		*m_lpOffRoadScoreImg;

	//スコアクラス
	COffRoadScore	*m_lpOffRoadScoreImage;

	//数字画像用
	COffRoadScore	*m_lpOffRoadScoreNumber;

	//ライダークラス
	COffRoadRacer	*m_lpCOffRoadRacer;

	//アクションクラス
	COffRoadAction	*m_lpCOffRoadAction;

	//スコアの表示先座標を扱う
	TPOINT<float>	m_ScoreArriveCoordinate;

	//スコアの出発表示を扱う
	TPOINT<float>	m_ScoreLeaveCoordinate;

	//スコアの表示先での待機時間を扱う
	int		m_ScoreStayTime;

	//数字画像の表示する数値を扱う
	int		m_DisplayNumber;

	//プレイヤーの前回行ったアクションビット目を扱う
	BYTE	m_CommandActionOld;

	//前回表示した評価画像の要素を扱う
	int		m_ValueTypeNext;

	//数字画像演出用構造体
	SCOREDIRECT		*m_lpScoreDirect;

protected:
	
public:

	//▼ 静的メンバ変数の定義
	static const int OFFROADSCOREMANAGER_SCORE_IMAGE_MAX;		//画像使用枚数

	//各種初期値
	//①アクション成功時の画像
	//スコアのサイズ
	static const int OFFROADSCOREMANAGER_SCORE_IMAGE_WIDTH;	//幅
	static const int OFFROADSCOREMANAGER_SCORE_IMAGE_HEIGHT;//高さ
	//Chain
	static const int OFFROADSCOREMANAGER_SCORE_IMAGE_CHAIN_WIDTH;
	//EXCELLENTのサイズ
	static const int	OFFROADSCOREMANAGER_SCORE_IMAGE_EXCELLENT_WIDTH;

	//拡縮倍率
	static const float OFFROADSCOREMANAGER_DEFAULT_SCORE_IMAGE_SCALE;	
	//スコアの移動量
	static const float	OFFROADSCOREMANAGER_SCORE_MOVE_X;	//X
	static const float	OFFROADSCOREMANAGER_SCORE_MOVE_Y;	//Y
	//スコアの待機時間
	static const int	OFFROADSCOREMANAGER_SCORE_STAY_TIME_MAX;

	//②数字画像表示用
	//スコアのサイズ
	static const int OFFROADSCOREMANAGER_SCORE_NUMBER_IMAGE_WIDTH;	//幅
	static const int OFFROADSCOREMANAGER_SCORE_NUMBER_IMAGE_HEIGHT;	//高さ
	//拡縮倍率
	static const float OFFROADSCOREMANAGER_DEFAULT_SCORE_NUMBER_IMAGE_SCALE;
	//種類数
	static const char	OFFROADSCOREMANAGER_SCORE_NUMBER_KIND_OF_TYPE;
	//▼ 数字画像演出用
	//演出したい数字スコアの総数
	static const char	OFFROADSCOREMANAGER_SCORE_NUMBER_DIRECTION_MAX;
	//加算・減算演出時のフレーム数
	static const int	OFFROADSCOREMANAGER_SCORE_NUMBER_DEFAULT_DIRECTION_INTERVAL_FRAM;



	//スコアの種類
	//SUCCEED
	static const char	OFFROADSCOREMANAGER_SCORE_TYPE_SUCCEED;
	//SCORE
	static const char	OFFROADSCOREMANAGER_SCORE_TYPE_SCORE;
	//GOOD
	static const char	OFFROADSCOREMANAGER_SCORE_TYPE_GOOD;
	//COOL
	static const char	OFFROADSCOREMANAGER_SCORE_TYPE_COOL;
	//EXCELLENT
	static const char	OFFROADSCOREMANAGER_SCORE_TYPE_EXCELLENT;

	//種類数
	static const char	OFFROADSCOREMANAGER_SCORE_IMAGE_KIND_OF_TYPE;

	//ウィンドウハンドル
	static HWND	m_hWnd;


	//▼プロトタイプ宣言
	//コンストラクタ
	COffRoadScoreManager();
	//ディストラクタ
	~COffRoadScoreManager();
	
	void InitScoreManager();					//初期化
	void UpdateScoreManagerSucceedAction();		//成功したアクション数に応じた評価画像の更新
	void DrawScoreManager();					//表示
	void ReleaseScoreManager();					//解放
	/*
	数字画像表示
	*/
	void DrawScoreNumber( TPOINT<float> pos , int number , int digit, tagSCOREMANAGERCOLORTYPE color, float addScale);
	/*
	目標の数値まで徐々に加算（減算）する
	*/
	int GraduallyNumber( SCOREDIRECT* pScore );



	
};


#endif