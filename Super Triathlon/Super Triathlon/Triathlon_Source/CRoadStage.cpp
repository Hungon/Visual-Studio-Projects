#include	"CRoadStage.h"
#include	"CPlayerRunner.h"
#include	"play.h"
#include	"wipe.h"
#include	"game.h"
#include	<iostream>

using namespace std;


//▼ 静的メンバ変数の再定義
HWND	CRoadStage::m_hWnd = NULL;
const int  CRoadStage::ROADSTAGE_IMAGE_MAX = 5;				//使用枚数
const int CRoadStage::ROADSTAGE_BACK_GROUND_MAX = 2;		//背景の数
//ステージの距離
const float  CRoadStage::ROADSTAGE_DISTANCE_EAZY	= 15000.0f;		//EAZYモード
const float  CRoadStage::ROADSTAGE_DISTANCE_NORMAL	= 20000.0f;		//NORMALモード
const float  CRoadStage::ROADSTAGE_DISTANCE_HARD	= 30000.0f;		//HARDモード

//道路の道幅（ピクセル）
const float  CRoadStage::ROADSTAGE_THE_WIDTH_OF_A_ROAD = 380.0f;	//画面上端の道幅

//各種オブジェクトの設定
const int  CRoadStage::ROADSTAGE_OBJECT_MAX = 4;		//扱う数
//オブジェクト白線の設定
const int	CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_WIDTH = 20;					//幅
const int	CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_HEIGHT = 120;				//高さ
const float	CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_INDICATE_SPACE_Y = 200.0f;	//表示間隔Y
//拡縮率
const float  CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_DEFAULT_SCALE = 1.0f;
const int	CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_MAX = 3;	//白線の数

//ステージの全長を扱う
float CRoadStage::m_RoadStageHeight = 0.0f;

//setting railings
 const int	CRoadStage::ROADSTAGE_IMAGE_RAILING_WIDTH = 60;
 const int	CRoadStage::ROADSTAGE_IMAGE_RAILING_HEIGHT = 130;
 const float	CRoadStage::ROADSTAGE_IMAGE_RAILING_DEFAULT_SCALE = 1.0f;
 const int	CRoadStage::ROADSTAGE_IMAGE_RAILING_MAX = 6;


/*
**********************************************************************************
コンストラクタ
**********************************************************************************
*/
CRoadStage::CRoadStage(){}


/*
**********************************************************************************
ディストラクタ
**********************************************************************************
*/
CRoadStage::~CRoadStage(){

}


//************************************************************************************************
//陸上ステージでの初期化
//************************************************************************************************
void    CRoadStage::InitRoadStage(){
	
	//画像データ
	char	*lpImageTbl[ CRoadStage::ROADSTAGE_IMAGE_MAX ] = {
	
		{ "Image\\Road\\road.bmp" }					,		//道路
		{ "Image\\Road\\roadline.bmp" }				,		//中央分離帯白線
		{ "Image\\Road\\aroundRoad.bmp" }			,		//路肩画像
		{ "Image\\Road\\roadStageGoal.bmp" }		,		//ゴール
		{ "Image\\Road\\Railing.bmp"},						//railing
	};

	//ゲームの難易度を取得
	tagGAMELEVEL getGameLevel = GetGameLevel();


	//ステージの距離を扱うデータテーブル
	float stageLevelTbl[ LEVEL_MAX ] = {

		CRoadStage::ROADSTAGE_DISTANCE_EAZY			,		//EAZY
		CRoadStage::ROADSTAGE_DISTANCE_NORMAL		,		//NORMAL
		CRoadStage::ROADSTAGE_DISTANCE_HARD			,		//HARD
	};

	//▼ 各種メモリの確保
	this->m_lpRoadStageImg = new int [ CRoadStage::ROADSTAGE_IMAGE_MAX ];			//画像
	//表示設定用
	this->m_lpRoadBackGround = new STAGE [ CRoadStage::ROADSTAGE_BACK_GROUND_MAX ];	//背景
	this->m_lpRoadObject = new STAGE [ CRoadStage::ROADSTAGE_OBJECT_MAX ];			//オブジェクト
	this->m_lpCPlayerRunner = new CPlayerRunner;									//プレイヤークラス
	this->m_lpRailings = new CHARACTER[(CRoadStage::ROADSTAGE_IMAGE_RAILING_MAX * 2)]; //railings

	//▼難易度に応じてステージの距離を決定する
	this->m_RoadStageBottom = stageLevelTbl[getGameLevel];				//下辺
	this->m_RoadStageTop = stageLevelTbl[getGameLevel] - STAGE_HEIGHT;	//上辺
	this->m_RoadStageWidth = STAGE_WIDTH;								//幅
	this->m_RoadStageHeight= stageLevelTbl[getGameLevel];				//高さ

	//▼ カメラの範囲を設定
	ChangeCamera( this->m_RoadStageTop , this->m_RoadStageBottom , 
	this->m_RoadStageWidth , this->m_RoadStageHeight );

	//▼ カメラの焦点を決定
	SetCamera( 0 ,  this->m_RoadStageHeight- 
		( ( STAGE_HEIGHT - CAMERA_FOCUS_POSITION_Y ) + CAMERA_FOCUS_POSITION_Y ) ); 

	//▼ 画像の読み込み
	for( int i = 0 ; i < CRoadStage::ROADSTAGE_IMAGE_MAX ; i++ ){
		this->m_lpRoadStageImg[i] = CreateDDImage( lpImageTbl[i] , CRoadStage::m_hWnd );
	}


	//▼ 表示設定

	//背景の設定
	//0で初期化
	ZeroMemory( this->m_lpRoadBackGround , sizeof( STAGE ) * CRoadStage::ROADSTAGE_BACK_GROUND_MAX );
	//道路画像
	this->m_lpRoadBackGround[0].ch.x = 0.0f;				//X座標
	this->m_lpRoadBackGround[0].ch.y = 0.0f;				//Y座標
	this->m_lpRoadBackGround[0].ch.w = 640;					//幅
	this->m_lpRoadBackGround[0].ch.h = 480;					//高さ
	this->m_lpRoadBackGround[0].ch.flag = true;				//表示フラグ

	//路肩画像
	this->m_lpRoadBackGround[1].ch.x = 0.0f;				//X座標
	this->m_lpRoadBackGround[1].ch.y = -1440.0f;			//Y座標
	this->m_lpRoadBackGround[1].ch.w = 640;					//幅
	this->m_lpRoadBackGround[1].ch.h = 1920;				//高さ
	this->m_lpRoadBackGround[1].ch.flag = true;				//表示フラグ

	//オブジェクトの設定
	//0で初期化
	ZeroMemory( this->m_lpRoadObject , sizeof( STAGE ) * CRoadStage::ROADSTAGE_OBJECT_MAX );
	//車線
	for( int i = 0 ; i < CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_MAX ; i++ ){
		this->m_lpRoadObject[i].ch.x = 310.0f;											//X
		this->m_lpRoadObject[i].ch.y = this->m_RoadStageTop + 
			( CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_INDICATE_SPACE_Y * i );			//Y
		this->m_lpRoadObject[i].ch.w = CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_WIDTH;	//幅
		this->m_lpRoadObject[i].ch.h = CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_HEIGHT;	//高さ
		this->m_lpRoadObject[i].ch.flag = true;											//フラグ
		this->m_lpRoadObject[i].ch.scale = CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_DEFAULT_SCALE;		//拡縮率
		this->m_lpRoadObject[i].ch.defaultScale = CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_DEFAULT_SCALE;//初期拡縮倍率
	}

	//ゴールライン画像
	this->m_lpRoadObject[3].ch.x = 135.0f;				//X座標
	this->m_lpRoadObject[3].ch.y = -250.0f;				//Y座標
	this->m_lpRoadObject[3].ch.w = 370;					//幅
	this->m_lpRoadObject[3].ch.h = 200;					//高さ
	this->m_lpRoadObject[3].ch.flag = true;				//表示フラグ
	this->m_lpRoadObject[3].ch.scale = 1.35f;			//拡縮率
	this->m_lpRoadObject[3].ch.defaultScale = 1.35f;	//拡縮倍率初期値

	//車線のスタート時点での拡縮率を求める
	for( int i = 0 ; i < CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_MAX ; i++ ){
		FittingPositionToBackCameraAngle( &this->m_lpRoadObject[i].ch , CRoadStage::ROADSTAGE_THE_WIDTH_OF_A_ROAD / 640.0f );
	}

	//set railings
	ZeroMemory(&this->m_lpRailings[0], sizeof(CHARACTER) * (CRoadStage::ROADSTAGE_IMAGE_RAILING_MAX * 2));
	//top height
	float topH = CRoadStage::ROADSTAGE_IMAGE_RAILING_HEIGHT * CRoadStage::ROADSTAGE_IMAGE_RAILING_DEFAULT_SCALE;
	//for material
	int cnt = 0;
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < CRoadStage::ROADSTAGE_IMAGE_RAILING_MAX; i++) {
			this->m_lpRailings[cnt].w = CRoadStage::ROADSTAGE_IMAGE_RAILING_WIDTH;
			this->m_lpRailings[cnt].h = CRoadStage::ROADSTAGE_IMAGE_RAILING_HEIGHT;
			this->m_lpRailings[cnt].scale = CRoadStage::ROADSTAGE_IMAGE_RAILING_DEFAULT_SCALE;
			this->m_lpRailings[cnt].y = this->m_RoadStageTop + (i * topH);
			//local coordinateY
			float localY = this->m_lpRailings[cnt].y - this->m_RoadStageTop;
			//calculate railing's coordinateY into rate of coordinateY in stage's height
			float rateY = ((float)STAGE_HEIGHT - localY) / (float)(STAGE_HEIGHT - ROADSTAGE_SUBTLTY_POSITIONX_FOR_ROAD_WIDTH);
			//constrain move
			float margin = ROADSTAGE_MARGIN_WIDTH_FOR_TOP_AND_BOTTOM_IN_ROAD * rateY;
			//coordinateX
			float posX[2] = { ((float)STAGE_WIDTH - margin) , margin - (ROADSTAGE_SUBTLTY_POSITIONX_FOR_ROAD_WIDTH / 1.7)};
			//subtract strict move area from stage's width
			this->m_lpRailings[cnt].x = posX[j];
			this->m_lpRailings[cnt].flag = true;
			this->m_lpRailings[cnt].defaultScale = CRoadStage::ROADSTAGE_IMAGE_RAILING_DEFAULT_SCALE;
			this->m_lpRailings[cnt].picy = j * CRoadStage::ROADSTAGE_IMAGE_RAILING_HEIGHT;
			cnt++;
		}
	}
}

//************************************************************************************************
//陸上ステージの更新
//************************************************************************************************
void    CRoadStage::UpdateRoadStage(){

	//▼ ランナーの情報を取得
	//プレイヤー
	RUNNER* pPrunner = this->m_lpCPlayerRunner->GetRunner();

	//カメラの座標を取得
	float comeCameraY = GetCameraY();
	

	//▼　道路背景周辺画像をキャラクターの移動に合わせて
	//スクロールさせる
	this->m_lpRoadBackGround[1].ch.y -= pPrunner->aggregateSpeed;
	
	//Y座標が０まで達したら元の場所に再配置する
	if( this->m_lpRoadBackGround[1].ch.y >= 0.0f ){

		//Y座標を元に戻す
		this->m_lpRoadBackGround[1].ch.y = -1440.0f;
	}

	//▼ 画面外に出た車線は上へ配置
	for( int i = 0 ; i < CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_MAX ; i++ ){
		
		//▼　手前に来るほどに大きく見せる処理
		//車線の幅を変更する
		FittingPositionToBackCameraAngle( &this->m_lpRoadObject[i].ch , ( CRoadStage::ROADSTAGE_THE_WIDTH_OF_A_ROAD / 640.0f ) / 2 );

		//画面外に出たら上に再配置
		if( this->m_lpRoadObject[i].ch.y - ( this->m_lpRoadObject[i].ch.h * 
			( ( this->m_lpRoadObject[i].ch.scale - ROADSTAGE_OBJECT_WHITE_LINE_DEFAULT_SCALE ) / 2 ) ) >=
			(comeCameraY + STAGE_HEIGHT) ){
			//Y座標
			this->m_lpRoadObject[i].ch.y = (float)( comeCameraY - this->m_lpRoadObject[i].ch.h );
			//拡縮率をデフォルトに
			this->m_lpRoadObject[i].ch.scale = ROADSTAGE_OBJECT_WHITE_LINE_DEFAULT_SCALE;
		}
	}


	//▼　手前に来るほどに大きくゴールを拡大する
	FittingPositionToBackCameraAngle( &this->m_lpRoadObject[3].ch , 0.7f );

	//fitting railings' size according to present coordinateY in local coordinateY
	int cnt = 0;
	for (int j = 0; j < 2; j++){
		for (int i = 0; i < CRoadStage::ROADSTAGE_IMAGE_RAILING_MAX; i++) {
			if (this->m_lpRailings[cnt].flag) {
				//local coordinateY
				float localY = this->m_lpRailings[cnt].y - this->m_RoadStageTop;
				//calculate railing's coordinateY into rate of coordinateY in stage's height
				float rateY = ((float)STAGE_HEIGHT - localY) / (float)(STAGE_HEIGHT - ROADSTAGE_SUBTLTY_POSITIONX_FOR_ROAD_WIDTH);
				//constrain move
				float margin = ROADSTAGE_MARGIN_WIDTH_FOR_TOP_AND_BOTTOM_IN_ROAD * rateY;
				//coordinateX
				float posX[2] = { ((float)STAGE_WIDTH - margin) , margin - (ROADSTAGE_SUBTLTY_POSITIONX_FOR_ROAD_WIDTH / 1.7)};
				//subtract strict move area from stage's width
				this->m_lpRailings[cnt].x = posX[j];
				//top height
				float topH = this->m_lpRailings[cnt].h * this->m_lpRailings[cnt].scale;
				FittingPositionToBackCameraAngle(&this->m_lpRailings[cnt], (CRoadStage::ROADSTAGE_THE_WIDTH_OF_A_ROAD / 640.0f) / 2);
				//rearrange coordinateY as a railing gets off from stage's bottom
				if (this->m_lpRailings[cnt].y >= (comeCameraY + (float)STAGE_HEIGHT)) {
					//runner's facing position
					this->m_lpRailings[cnt].y = comeCameraY - topH;
					this->m_lpRailings[cnt].scale = CRoadStage::ROADSTAGE_IMAGE_RAILING_DEFAULT_SCALE;
					cnt++;
					continue;
				}
				cnt++;
			}
		}
	}
	//▼ ランナーの移動量を基に画面の下辺と上辺を更新
	//下辺
	this->m_RoadStageBottom += pPrunner->aggregateSpeed;
	//上辺
	this->m_RoadStageTop = this->m_RoadStageBottom - STAGE_HEIGHT;

	//▼カメラの範囲を変更
	ChangeCamera( this->m_RoadStageTop , this->m_RoadStageBottom , 
	this->m_RoadStageWidth , this->m_RoadStageHeight );


	//▼ ゴール設定
	//プレイヤーがゴール地点まで達したら、
	//リザルト画面に遷移
	if( pPrunner->ch.y + pPrunner->ch.h * pPrunner->ch.scale < 
		this->m_lpRoadObject[3].ch.y + this->m_lpRoadObject[3].ch.h * this->m_lpRoadObject[3].ch.scale ){
		//タイマーをOFFにする
		ControlStageTime(false);
		//リザルトへ
		CreateWipe( SCENE_RESULT , CHANGE_SCENE , WIPE_CONTRACTION );
	}
	
}


//************************************************************************************************
//陸上ステージの表示
//************************************************************************************************
void    CRoadStage::DrawRoadStage(){


	//カメラの座標を取得
	float comeCameraY = GetCameraY();

	char mes[32] = {0};

/*
	wsprintf( &mes[0] , "距離%d" , (int)this->m_RoadStageBottom );
	DDTextOut( &mes[0] , 500 , 85 );
*/

	//▼ 背景画像
	//陸上ステージ「道路周辺」の背景を表示
	if( this->m_lpRoadBackGround[1].ch.flag ){
	
		DDBlt(
			(int)this->m_lpRoadBackGround[1].ch.x ,
			(int)this->m_lpRoadBackGround[1].ch.y ,
			this->m_lpRoadBackGround[1].ch.w ,
			this->m_lpRoadBackGround[1].ch.h ,
			this->m_lpRoadStageImg[2] ,
			this->m_lpRoadBackGround[1].ch.picx ,
			this->m_lpRoadBackGround[1].ch.picy );

	}

	//陸上ステージ「道路」の背景を表示
	if( this->m_lpRoadBackGround[0].ch.flag ){
	
		DDBlt(
			(int)this->m_lpRoadBackGround[0].ch.x ,
			(int)this->m_lpRoadBackGround[0].ch.y ,
			this->m_lpRoadBackGround[0].ch.w ,
			this->m_lpRoadBackGround[0].ch.h ,
			this->m_lpRoadStageImg[0] ,
			this->m_lpRoadBackGround[0].ch.picx ,
			this->m_lpRoadBackGround[0].ch.picy );

	}

	//▼白線画像
	for( int i = 0 ; i < CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_MAX ; i++ ){

		if( this->m_lpRoadObject[i].ch.flag ){
		
			DDScaleBlt(
				this->m_lpRoadObject[i].ch.x ,
				this->m_lpRoadObject[i].ch.y - comeCameraY ,
				this->m_lpRoadObject[i].ch.w ,
				this->m_lpRoadObject[i].ch.h ,
				this->m_lpRoadStageImg[1] ,
				this->m_lpRoadObject[i].ch.picx ,
				this->m_lpRoadObject[i].ch.picy ,
				this->m_lpRoadObject[i].ch.scale );

		}

	}

	//▼陸上ステージ「ゴール」の画像を表示
	if( this->m_lpRoadObject[3].ch.flag ){
	
		DDScaleBlt(
			this->m_lpRoadObject[3].ch.x ,
			this->m_lpRoadObject[3].ch.y - comeCameraY ,
			this->m_lpRoadObject[3].ch.w ,
			this->m_lpRoadObject[3].ch.h ,
			this->m_lpRoadStageImg[3] ,
			this->m_lpRoadObject[3].ch.picx ,
			this->m_lpRoadObject[3].ch.picy ,
			this->m_lpRoadObject[3].ch.scale );

	}

	//draw railings
	for (int i = 0; i < (CRoadStage::ROADSTAGE_IMAGE_RAILING_MAX * 2); i++) {
		if (this->m_lpRailings[i].flag) {
			DDScaleBltSelectIndicate(
				this->m_lpRailings[i].x,
				this->m_lpRailings[i].y - comeCameraY,
				this->m_lpRailings[i].w,
				this->m_lpRailings[i].h,
				this->m_lpRoadStageImg[4],
				this->m_lpRailings[i].picx,
				this->m_lpRailings[i].picy,
				this->m_lpRailings[i].scale,
				DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP);
		}
	}
	
}

//***************************************************************************************************
//解放
//***************************************************************************************************
void    CRoadStage::ReleaseRoadStage(){

	//画像の解放
	if( this->m_lpRoadStageImg != NULL ){
		for( int i = 0 ; i < CRoadStage::ROADSTAGE_IMAGE_MAX ; i++ ){
			ReleaseDDImage( this->m_lpRoadStageImg[i] );
		}
	}

	//メモリの解放
	delete []this->m_lpRoadStageImg;		//画像
	this->m_lpRoadStageImg = NULL;
	
	//メモリの解放
	if( this->m_lpRoadBackGround != NULL ){
		delete []this->m_lpRoadBackGround;		//背景
		this->m_lpRoadBackGround = NULL;
	}
	if( this->m_lpRoadObject != NULL ){
		delete []this->m_lpRoadObject;			//オブジェクト
		this->m_lpRoadObject = NULL;
	}
	if( this->m_lpCPlayerRunner != NULL ){
		delete this->m_lpCPlayerRunner;			//プレイヤークラス
		this->m_lpCPlayerRunner = NULL;
	}
	delete[]this->m_lpRailings;				//railings
	this->m_lpRailings = NULL;
}
