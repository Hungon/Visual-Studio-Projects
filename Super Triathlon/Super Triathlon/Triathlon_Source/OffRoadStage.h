#ifndef OFFROADSTAGE_H__
#define OFFROADSTAGE_H__


//▼ ヘッダーのインクルード
#include	"graphics.h"
#include	<windows.h>

//▼ 前方宣言
class COffRoadRacer;

//▼オフロードステージクラスの定義
class COffRoadStage{

private:

	//画像の要素を扱う
	int		*m_lpCOffRoadImg;

	//背景画像設定用
	CHARACTER		*m_lpOffRoad;

	//ジャンプ台設定用
	CHARACTER		*m_lpOffRoadJumpPoint;

	//for brief map
	CHARACTER	*m_lpMinimap;

	//ジャンプ台取得用
	static CHARACTER		*m_lpOffRoadGetJumpPoint;

	//ライダークラス
	COffRoadRacer	*m_lpCOffRoadRacer;

	//ステージの距離を扱う
	float		m_distance;
	static float m_wholeDistance;

	//ジャンプ台の生成間隔を扱う
	int		m_CreatrionFram;


protected:


public:

	//▼ 静的メンバ変数の定義
	static const int OFFROADSTAGE_IMAGE_MAX;				//画像使用枚数

	//背景のサイズ
	//地面
	static const int OFFROADSTAGE_BACK_IMAGE_GROUND_WIDTH;			//幅
	static const int OFFROADSTAGE_BACK_IMAGE_GROUND_HEIGHT;			//高さ
	//ゴール地点
	static const int OFFROADSTAGE_BACK_IMAGE_GOAL_POINT_WIDTH;		//幅
	static const int OFFROADSTAGE_BACK_IMAGE_GOAL_POINT_HEIGHT;		//高さ

	//背景の種類数
	static const int OFFROADSTAGE_BACK_IMAGE_KIND_OF_TYPE;

	//ジャンプポイントのサイズ
	static const int OFFROADSTAGE_JUMP_POINT_IMAGE_WIDTH;	//幅
	static const int OFFROADSTAGE_JUMP_POINT_IMAGE_HEIGHT;	//高さ
	//拡縮倍率初期値
	static const float OFFROADSTAGE_JUMP_POINT_DEFAULT_SCALE;
	//表示Ｘの間隔
	static const int OFFROADSTAGE_JUMP_POINT_INDICATE_X_SPACE;
	//表示Ｘの配置最大数
	static const int OFFROADSTAGE_JUMP_POINT_INDICATE_X_POSITION_MAX;
	//配置最大数
	static const int OFFROADSTAGE_JUMP_POINT_POSITION_MAX;

	//setting brief map
	static const int OFFROADSTAGE_IMAGE_MINIMAP_WIDTH;						//for back ground
	static const int OFFROADSTAGE_IMAGE_MINIMAP_HEIGHT;
	static const float OFFROADSTAGE_IMAGE_MINIMAP_INDICATE_X;
	static const float OFFROADSTAGE_IMAGE_MINIMAP_INDICATE_Y;
	static const float OFFROADSTAGE_IMAGE_MINIMAP_RACER_DEFAULT_SCALE;		//for minimum racer
	static const int OFFROADSTAGE_MINIMAP_KIND_OF_IMAGE;					//kind of image

	//ウィンドウハンドル
	static HWND	m_hWnd;

	//ステージの距離
	static const int OFFROADSTAGE_DISTANCE_EAZY;	//EAZYモード
	static const int OFFROADSTAGE_DISTANCE_NORMAL;	//NORMALモード
	static const int OFFROADSTAGE_DISTANCE_HARD;	//HARDモード

	//今回のステージの画面サイズ
	static const int OFFROADSTAGE_SCREEN_WIDTH;		//幅
	static const int OFFROADSTAGE_SCREEN_HEIGHT;	//高さ


	//コンストラクタ
	COffRoadStage() :
	//画像の要素を扱う
	m_lpCOffRoadImg(NULL) ,
	//背景画像設定用
	m_lpOffRoad(NULL) ,
	//ジャンプ台設定用
	m_lpOffRoadJumpPoint(NULL) ,
	//ライダークラス
	m_lpCOffRoadRacer(NULL) 
	{}

	//ディストラクタ
	~COffRoadStage();
	
	//▼ 自作メンバ関数の定義
	//陸上ステージの初期化
	void InitOffRoadStage();
	//道路の更新
	void UpdateOffRoadStage();
	//陸上ステージの表示
	void DrawOffRoadStage();
	//解放
	void ReleaseOffRoadStage();
	//ステージの全長を返す
	float GetOffRoadStageDistance() const { return this->m_wholeDistance; }
	/*
	ジャンプ地点の生成
	*/
	void CreateJumpPoint();
	/*
	ジャンプ地点とプレイヤーの重なりを調べる
	*/
	bool	CollisionJumpPoint( CHARACTER *pch );

	//現在出現中のジャンプ地点情報を設定する
	void	SetJumpPointInfo( CHARACTER* pJump , int element ){ *( this->m_lpOffRoadGetJumpPoint + element ) = *( pJump ); }


};


#endif
