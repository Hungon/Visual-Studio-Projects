#ifndef		CROADSTAGE_H__
#define		CROADSTAGE_H__


//▼ヘッダーインクルード
#include	"graphics.h"
#include	"map.h"

//it is road's margin that subtract road's bottom from road's top
const float	ROADSTAGE_MARGIN_WIDTH_FOR_TOP_AND_BOTTOM_IN_ROAD = 105.0f;
//it's subtlty position acoording to road's width
const float	ROADSTAGE_SUBTLTY_POSITIONX_FOR_ROAD_WIDTH = 140.0f;

//▼ 前方宣言
class	CPlayerRunner;

//ステージクラス
class	CRoadStage{

private:

	//画像の要素
	int		*m_lpRoadStageImg;

	//背景画像を扱う
	STAGE	*m_lpRoadBackGround;

	//オブジェクトの情報を扱う
	STAGE	*m_lpRoadObject;

	//for railings
	CHARACTER	*m_lpRailings;

	//ステージの範囲を扱う
	float m_RoadStageBottom;		//下辺
	float m_RoadStageTop;			//上辺
	float m_RoadStageWidth;			//幅
	static float m_RoadStageHeight;	//高さ

	//プレイヤークラス
	CPlayerRunner	*m_lpCPlayerRunner;


protected:

public:

	//▼ 静的メンバ変数の定義
	static HWND	m_hWnd;
	static const int  ROADSTAGE_IMAGE_MAX;		//使用枚数
	static const int  ROADSTAGE_BACK_GROUND_MAX;//背景の数
	static const int  ROADSTAGE_OBJECT_MAX;		//オブジェクトを扱う数

	//ステージの距離
	static const float  ROADSTAGE_DISTANCE_EAZY;		//EAZYモード
	static const float  ROADSTAGE_DISTANCE_NORMAL;	//NORMALモード
	static const float  ROADSTAGE_DISTANCE_HARD;		//HARDモード

	//道路の道幅（ピクセル）
	static const float  ROADSTAGE_THE_WIDTH_OF_A_ROAD;	//画面上端の道幅

	//白線の初期値を扱う
	static const int	ROADSTAGE_OBJECT_WHITE_LINE_WIDTH;				//幅
	static const int	ROADSTAGE_OBJECT_WHITE_LINE_HEIGHT;				//高さ
	static const float	ROADSTAGE_OBJECT_WHITE_LINE_INDICATE_SPACE_Y;	//表示間隔Y
	static const float  ROADSTAGE_OBJECT_WHITE_LINE_DEFAULT_SCALE;		//拡縮率
	static const int	ROADSTAGE_OBJECT_WHITE_LINE_MAX;				//白線の数

	//setting railings
	static const int	ROADSTAGE_IMAGE_RAILING_WIDTH;
	static const int	ROADSTAGE_IMAGE_RAILING_HEIGHT;
	static const float	ROADSTAGE_IMAGE_RAILING_DEFAULT_SCALE;
	static const int	ROADSTAGE_IMAGE_RAILING_MAX;

	//▼ 自作関数の定義
	
	CRoadStage();
	~CRoadStage();

	//陸上ステージの初期化
	void InitRoadStage();
	//道路の更新
	void UpdateRoadStage();
	//陸上ステージの表示
	void DrawRoadStage();
	//解放
	void ReleaseRoadStage();


	//各種取得用関数
	//****************************************************************************************************
	//ステージの全体距離を取得
	//****************************************************************************************************
	float GetRoadStageDistance(){ return this->m_RoadStageHeight; }


};

#endif
