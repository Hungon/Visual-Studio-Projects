#include	"CRoadEnemy.h"
#include	"CPlayerRunner.h"
#include	"CCompetitorManager.h"
#include	"CRunnerManager.h"
#include	"map.h"
#include	"sound.h"
#include	"play.h"
#include	<iostream>

using namespace std;


//▼ 静的メンバ変数の再定義
const int CObstacle::ROADENEMY_OBSTACLE_MAX = 12;			//障害物の出現総数

//画面スクロールに合わせて拡大する障害物の画面下端での最終的なX座標のずれ
const float CObstacle::OBSTACLE_FITTING_SCROL_POSITION_X = 120.0f;

//横一列に並べる最大数
const int	CObstacle::OBSTACLE_HURDLE_LINE_MAX = 5;
//横一列に並べる際の配列の種類
const int	CObstacle::OBSTACLE_HURDLE_LINE_ARRANGE_MAX = 5;

//配列の種類
const char	CObstacle::OBSTACLE_HURDLE_ARRANGE_TYPE_STRAIGHT_LINE = 0;			//横一直線に道を塞ぐ
const char	CObstacle::OBSTACLE_HURDLE_ARRANGE_TYPE_STRAIGHT_RANDOM = 1;		//横一直線にランダムに配置
const char	CObstacle::OBSTACLE_HURDLE_KIND_OF_ARRANGE = 2;						//種類数

//ハードルの配置始点座標
const float CObstacle::OBSTACLE_HURDLE_POSITION_START_X	= 130.0f;		//X
//ハードルの間隔
const float CObstacle::OBSTACLE_HURDLE_SPACE_X = 15.0f;					//X

//各種障害物の表示設定
//幅
const int CObstacle::ROADENEMY_OBSTACLE_HURDLE_SIZE_WIDTH	= 64;
const int CObstacle::ROADENEMY_OBSTACLE_ROCK_SIZE_WIDTH		= 64;
//高さ
const int CObstacle::ROADENEMY_OBSTACLE_HURDLE_SIZE_HEIGHT	= 32;
const int CObstacle::ROADENEMY_OBSTACLE_ROCK_SIZE_HEIGHT	= 64;
//拡縮倍率
const float CObstacle::ROADENEMY_OBSTACLE_HURDLE_SIZE_SCALE	= 1.0f;
const float CObstacle::ROADENEMY_OBSTACLE_ROCK_SIZE_SCALE	= 1.0f;
//移動量Y絶対値
const float CObstacle::ROADENEMY_OBSTACLE_HURDLE_ABSOLUTE_MOVE_Y = 1.0f;
const float CObstacle::ROADENEMY_OBSTACLE_ROCK_ABSOLUTE_MOVE_Y	 = 1.5f;
//移動量X絶対値
const float CObstacle::ROADENEMY_OBSTACLE_HURDLE_ABSOLUTE_MOVE_X = 0.5f;
const float CObstacle::ROADENEMY_OBSTACLE_ROCK_ABSOLUTE_MOVE_X	 = 0.5f;

//各種障害物のアニメーション設定
//フレーム数
const int CObstacle::ROADENEMY_OBSTACLE_HURDLE_ANIMATION_FRAM = 5;
const int CObstacle::ROADENEMY_OBSTACLE_ROCK_ANIMATION_FRAM = 5;
//コマ数
const int CObstacle::ROADENEMY_OBSTACLE_HURDLE_ANIMATION_CNT_MAX = 4;
const int CObstacle::ROADENEMY_OBSTACLE_ROCK_ANIMATION_CNT_MAX = 4;
//障害物のアニメーションフラグ
const BYTE CObstacle::ROADENEMY_ANIMATION_FLAG_NORMAL = 0x00;	//通常状態
const BYTE CObstacle::ROADENEMY_ANIMATION_FLAG_BREAK  = 0x01;	//破壊フラグ


//衝突する面を扱う
const char CObstacle::OBSTACLE_NOT_BUMP		=	-1;			//衝突なし
const char CObstacle::OBSTACLE_BUMP_TOP		=	0x00;		//上
const char CObstacle::OBSTACLE_BUMP_BOTTOM	=	0x01;		//下
const char CObstacle::OBSTACLE_BUMP_LEFT	=	0x02;		//左
const char CObstacle::OBSTACLE_BUMP_RIGHT	=	0x03;		//右


/*
*******************************************************************************************
デフォルトコンストラクタ
*******************************************************************************************
*/
CObstacle::CObstacle(){

}

/*
*******************************************************************************************
コンストラクタ
*******************************************************************************************
*/
CObstacle::CObstacle( int max ){

	//今回生成する最大数を取得
	this->m_ObstacleCreateMax = max;
	this->m_CreatedObstacleY = 999999.9f;

}

/*
*******************************************************************************************
ディストラクタ
*******************************************************************************************
*/
CObstacle::~CObstacle(){

}

//*******************************************************************************************
//初期化
//*******************************************************************************************
void   CObstacle::InitRoadEnemy( CCompetitorManager	*lpCCompetitorManager , CPlayerRunner *lpCPlayerRunner ){

	//ゲーム難易度を取得
	tagGAMELEVEL	gameLevel = GetGameLevel();

	//各種ハードルの生成確率
	//列：青 ⇒ 緑 ⇒ 赤の順
	int	CreateRateTbl[ LEVEL_MAX ][ RUNNER_EFFECT_KIND_OF_TYPE ] = {
	
		{ 80 , 30 , 30 } ,			//EASY
		{ 80 , 15 , 10 } ,			//NORMAL
		{ 80 ,  5 ,  5 } ,			//HARD
	};
	//各種ハードルの生成する最大数
	//列：赤 ⇒ 青 ⇒ 緑の順
	int	CreateCntMaxTbl[ LEVEL_MAX ][ RUNNER_EFFECT_KIND_OF_TYPE ] = {
	
		{ 3 , 3 , 2 } ,			//EASY
		{ 2 , 4 , 1 } ,			//NORMAL
		{ 1 , 5 , 1 } ,			//HARD
	};

	//難易度に応じて障害物の表示間隔を変更
	int	CreateSpaceYTbl[ LEVEL_MAX ] = { 400 , 380 , 360 };

	//表示間隔を代入
	this->m_CreateObstacleSpaceY = CreateSpaceYTbl[ gameLevel ];

	//▼ 各種メモリの確保
	//障害物を扱う構造体
	this->m_lpRoadObstacle = new OBSTACLE [ this->m_ObstacleCreateMax ];
	//各種0で初期化
	ZeroMemory( this->m_lpRoadObstacle , sizeof( OBSTACLE ) * this->m_ObstacleCreateMax );
	//各種障害物カウント用
	this->m_lpObstacleCnt = new int [ ROADENEMY_KIND_OF_OBSTACLE ];
	//各種障害物生成カウント用
	this->m_lpCreationOBstacleCnt = new int[ROADENEMY_KIND_OF_OBSTACLE];
	//各種ハードルの出す確率
	this->m_lpCreateRateToHurdle = new int [ RUNNER_EFFECT_KIND_OF_TYPE ];
	//各種ハードルのカウント用
	this->m_lpCreateHurdleCnt = new int [ ROADENEMY_OBSTACLE_HURDLE_KIND_OF_COLOR ];
	//各種ハードルの最大生成数
	this->m_lpCreateHurdleCntMax = new int [ ROADENEMY_OBSTACLE_HURDLE_KIND_OF_COLOR ];
		
	//▼ 各種ハードルに対した設定
	//各種ハードルの出す確率
	for( int i = 0 ; i < RUNNER_EFFECT_KIND_OF_TYPE ; i++ ){
		this->m_lpCreateRateToHurdle[i] = CreateRateTbl[ gameLevel ][i];
	}
	//各種ハードルの生成最大数
	for( int i = 0 ; i < ROADENEMY_OBSTACLE_HURDLE_KIND_OF_COLOR ; i++ ){
		this->m_lpCreateHurdleCntMax[i] = CreateCntMaxTbl[ gameLevel ][i];
	}
	//各種障害物カウント用
	ZeroMemory( this->m_lpObstacleCnt , sizeof( int ) * ROADENEMY_KIND_OF_OBSTACLE );
	//各種生成した障害物カウント用
	ZeroMemory(this->m_lpCreationOBstacleCnt, sizeof(int) * ROADENEMY_KIND_OF_OBSTACLE);
	//各種ハードルのカウント用
	ZeroMemory( this->m_lpCreateHurdleCnt , sizeof( int ) * ROADENEMY_OBSTACLE_HURDLE_KIND_OF_COLOR );

	//各種ランナーと関連付け
	//プレイヤー
	this->m_lpCPlayerRunner = lpCPlayerRunner;
	//競争者管理クラス
	this->m_lpCCompetitorManager = lpCCompetitorManager;

}

//*******************************************************************************************
//更新
//*******************************************************************************************
void   CObstacle::UpdateRoadEnemy(){


	//プレイヤーの情報を取得
	RUNNER	*lpRunner = this->m_lpCPlayerRunner->GetRunner();

	//ランダムの値
	int obstacleType = RandomNum( ROADENEMY_OBSTACLE_TYPE_ROCK , ROADENEMY_OBSTACLE_TYPE_HURDLE );

	//障害物カウント用
	int	obstacleCnt[ ROADENEMY_KIND_OF_OBSTACLE ] = {0};

	//各種ハードルカウント用
	int	hurdleCnt[ ROADENEMY_OBSTACLE_HURDLE_KIND_OF_COLOR ] = {0};

	//設定した表示間隔毎に障害物を生成する
	if( ( this->m_CreatedObstacleY - this->m_CreateObstacleSpaceY ) > lpRunner->ch.y ){
		//生成した座標を更新
		this->m_CreatedObstacleY = lpRunner->ch.y;
		//出現させる種類番号をランダムで渡す
		this->CreateObstacle( obstacleType );
	}

	//障害物の最大出現数まで回す
	for( int i = 0 ; i < this->m_ObstacleCreateMax ; i++ ){

		//画面外に出た障害物のフラグをfalseに
		if( CollisionCamera( &this->m_lpRoadObstacle[i].ch ) == false ){

			//生存フラグ
			this->m_lpRoadObstacle[i].ch.flag = false;
			
			//種類番号を何もなしに
			this->m_lpRoadObstacle[i].bitType = ROADENEMY_OBSTACLE_BIT_TYPE_VACANT;
			continue;

		}

		//falseなら次の要素へ
		if( this->m_lpRoadObstacle[i].ch.flag == false ) continue;
				
		//各種障害物の数をカウント
		obstacleCnt[ this->m_lpRoadObstacle[i].ch.type ]++;
		//各種ハードルの数をカウント
		if( this->m_lpRoadObstacle[i].ch.type == ROADENEMY_OBSTACLE_TYPE_HURDLE ) hurdleCnt[ this->m_lpRoadObstacle[i].colorType ]++;

		//▼行動の更新の分岐
		switch( this->m_lpRoadObstacle[i].ch.type ){
			case ROADENEMY_OBSTACLE_TYPE_HURDLE :
				this->UpdateActionObstacleHurdle( i );
				break;
			case ROADENEMY_OBSTACLE_TYPE_ROCK :
				this->UpdateActionObstacleRock( i );
				break;
		}
		continue;
	
	}

	//障害物の数を更新
	for( int i = 0 ; i < ROADENEMY_KIND_OF_OBSTACLE ; i++ ){ this->m_lpObstacleCnt[i] = obstacleCnt[i]; }
	//各種ハードルの数
	for( int i = 0 ; i < ROADENEMY_OBSTACLE_HURDLE_KIND_OF_COLOR ; i++ ){ this->m_lpCreateHurdleCnt[i] = hurdleCnt[i]; }


}

//*******************************************************************************************
//表示
//*******************************************************************************************
void   CObstacle::DrawRoadEnemy(){

	//カメラの座標を取得
	float px = GetCameraX();				//X
	float py = GetCameraY();				//Y

	char mes[255] = {0};

	char *lpHurdle[4] = {
		"黒" ,
		"赤" ,
		"青" ,
		"緑" ,
	};
	char *lpObstacle[2] = {
		"ハードル" ,
		"岩石" ,
	};
	TPOINT<int> pos = { 450 , 460 };
	
	//▼障害物
	//出現させる数だけループを回す
	for( int i = 0 ; i < this->m_ObstacleCreateMax ; i++ ){

		//生存フラグがtrueなら
		if( this->m_lpRoadObstacle[i].ch.flag ){

			//拡縮表示
			DDScaleBlt(
				this->m_lpRoadObstacle[i].ch.x - px ,
				this->m_lpRoadObstacle[i].ch.y - py ,
				this->m_lpRoadObstacle[i].ch.w ,
				this->m_lpRoadObstacle[i].ch.h ,
				this->m_lpRoadEnemyImg[this->m_lpRoadObstacle[i].ch.bitnum] ,
				this->m_lpRoadObstacle[i].ch.picx ,
				this->m_lpRoadObstacle[i].ch.picy ,
				this->m_lpRoadObstacle[i].ch.scale);
		
		}
	
	}

/*
	//障害物の最大数
	wsprintf( &mes[0] , "障害物の最大数%d" , this->m_ObstacleCreateMax );
	DDTextOut( &mes[0] , pos.x , pos.y );

	//出現中の各種ハードルの数
	for( int i = 0 ; i < ROADENEMY_OBSTACLE_HURDLE_KIND_OF_COLOR ; i++ ){
		wsprintf( &mes[0] , "ハードル%sの数%d" , lpHurdle[i] , this->m_lpCreateHurdleCnt[i] );
		DDTextOut( &mes[0] , pos.x  , ( pos.y - 20 ) - ( i * 20) );
	}
	//障害物の数
	for( int i = 0 ; i < ROADENEMY_KIND_OF_OBSTACLE ; i++ ){
		wsprintf( &mes[0] , "%sの数%d" , lpObstacle[i] , this->m_lpObstacleCnt[i] );
		DDTextOut( &mes[0] , pos.x  , ( pos.y - 100 ) - ( i * 20) );
	}

*/

}

//*******************************************************************************************
//解放
//*******************************************************************************************
void   CObstacle::ReleaseRoadEnemy(){

	
	//メモリの解放
	if( this->m_lpRoadEnemyImg != NULL ){
		delete []this->m_lpRoadEnemyImg;		//画像
		this->m_lpRoadEnemyImg = NULL;
	}
	if( this->m_lpRoadEnemyEffectSound != NULL ){
		delete []this->m_lpRoadEnemyEffectSound;//効果音
		this->m_lpRoadEnemyEffectSound = NULL;
	}

	if( this->m_lpRoadObstacle != NULL ){
		delete []this->m_lpRoadObstacle;		//障害物
		this->m_lpRoadObstacle = NULL;
	}
	if( this->m_lpCreateRateToHurdle != NULL ){
		delete []this->m_lpCreateRateToHurdle;	//各種ハードルの出す確率
		this->m_lpCreateRateToHurdle = NULL;
	}
	if( this->m_lpCreateHurdleCnt != NULL ){
		delete []this->m_lpCreateHurdleCnt;		//各種ハードルのカウント用
		this->m_lpCreateHurdleCnt = NULL;
	}
	if( this->m_lpCreateHurdleCntMax != NULL ){
		delete []this->m_lpCreateHurdleCntMax;	//各種ハードルの最大生成数
		this->m_lpCreateHurdleCntMax = NULL;
	}
	if( this->m_lpObstacleCnt != NULL ){
		delete []this->m_lpObstacleCnt;			//各種障害物カウント用
		this->m_lpObstacleCnt = NULL;
	}
	delete []this->m_lpCreationOBstacleCnt;		//各種障害物生成数カウント用
	this->m_lpCreationOBstacleCnt = NULL;
}

//*******************************************************************************************
//障害物の生成
//*******************************************************************************************
void   CObstacle::CreateObstacle( int type ){

	//プレイヤーの効果を発揮した障害物の要素を取得
	RUNNER	*lpRunner = this->m_lpCPlayerRunner->GetRunner();

	//競争者の最大数を取得
	int	compeMax = this->m_lpCCompetitorManager->GetCompetitorMax();


	//横一直線に配置する際のランダム値を取得する
	this->m_ObstacleRandomArrangeMax = RandomNum( ( CObstacle::OBSTACLE_HURDLE_LINE_ARRANGE_MAX - 1 ) , 0 );

	//今回のハードルの配列の種類の要素
	this->m_ObstacleInitArrangeRandomType = RandomNum( CObstacle::OBSTACLE_HURDLE_ARRANGE_TYPE_STRAIGHT_RANDOM , 
		CObstacle::OBSTACLE_HURDLE_ARRANGE_TYPE_STRAIGHT_LINE );

	//障害物のカウントをリセット
	this->m_lpCreationOBstacleCnt[0] = 0;				//ハードル
	this->m_lpCreationOBstacleCnt[1] = 0;				//岩石

	//最大表示数までループを回す
	for( int i = 0 ; i < this->m_ObstacleCreateMax  ; i++ ){

		//生存フラグがfalseなら
		if( this->m_lpRoadObstacle[i].ch.flag == false ){
			//キャラクターに効果を発揮した要素数なら、キャラクターが取得した障害物の要素をリセットする
			//プレイヤー
			if( i == lpRunner->obstacleBumpNow ) lpRunner->obstacleBumpNow = -1;
			//競争者
			for( int j = 0 ; j < compeMax ; j++ ){
				if( i == *( this->m_lpCCompetitorManager->GetCompetitorManagerObstacleMaterial() + j ) ){
					this->m_lpCCompetitorManager->SetCompetitorManagerObstacleMaterial( -1 , j );
				}
			}

			//▼　共通の設定
			this->m_lpRoadObstacle[i].ch.type = type;										//種類番号
			this->m_lpRoadObstacle[i].ch.flag = true;										//生存フラグ
					
			//▼　各種障害物の初期化
			switch( this->m_lpRoadObstacle[i].ch.type ){
				case ROADENEMY_OBSTACLE_TYPE_HURDLE :
					//生成完了ならreturn
					if( this->InitObstacleHurdle( i ) ) return;
					break;
				case ROADENEMY_OBSTACLE_TYPE_ROCK :
					if( this->InitObstacleRock( i ) ) return;
					break;
			}
		}
	}
}


//********************************************************************************************
//障害物1「ハードル」の行動の初期化
//********************************************************************************************
bool	CObstacle::InitObstacleHurdle( int material ){

	//プレイヤーの体力に応じた分岐を作る
	RUNNER	*lpRunner = this->m_lpCPlayerRunner->GetRunner();

	//出現させるハードルの種類をまとめたデータテーブル
	//赤と緑を青より優先的に出やすくする配置
	tagHURDLECOLORTYPE hurdleTypeTbl[ RUNNER_EFFECT_KIND_OF_TYPE ] = {

		ROADENEMY_OBSTACLE_HURDLE_COLOR_BLUE	,		//青
		ROADENEMY_OBSTACLE_HURDLE_COLOR_GREEN ,			//緑
		ROADENEMY_OBSTACLE_HURDLE_COLOR_RED	,			//赤	
	};

	//各種ハードルの生成確率
	//何も該当しなければ黒いハードルを出す
	int hurdleType = ROADENEMY_OBSTACLE_HURDLE_COLOR_BLACK;

	//各種ハードルを出す確率の可変値
	int variableType[ RUNNER_EFFECT_KIND_OF_TYPE ] = {0};

	//プレイヤーの体力が減っていたら、赤のハードルと緑のハードルを出す確率を上げる
	switch( lpRunner->ch.hp ){
		case 1 :
			variableType[2] = 10;
			variableType[1] = 60;
			break;
		case 2 :
			variableType[2] = 5;
			variableType[1] = 40;
			break;

	}

	//生成されるハードルの種類を各種に設定した確率に応じて決定
	for( int i = 0 ; i < RUNNER_EFFECT_KIND_OF_TYPE ; i++ ){
		//生成するタイプを代入するか調べる
		if( GetLikelihood( this->m_lpCreateRateToHurdle[i] + variableType[i] ) ) hurdleType = hurdleTypeTbl[i]; continue;
	}

	//▼　各種設定
	this->m_lpRoadObstacle[material].ch.w = CObstacle::ROADENEMY_OBSTACLE_HURDLE_SIZE_WIDTH;			//幅
	this->m_lpRoadObstacle[material].ch.h = CObstacle::ROADENEMY_OBSTACLE_HURDLE_SIZE_HEIGHT;			//高さ
	this->m_lpRoadObstacle[material].ch.bitnum = ROADENEMY_OBSTACLE_TYPE_HURDLE;						//表示要素番号
	this->m_lpRoadObstacle[material].ch.scale = CObstacle::ROADENEMY_OBSTACLE_HURDLE_SIZE_SCALE;		//拡縮倍率
	this->m_lpRoadObstacle[material].ch.defaultScale = CObstacle::ROADENEMY_OBSTACLE_HURDLE_SIZE_SCALE;	//拡縮倍率初期値
	this->m_lpRoadObstacle[material].bitType = ROADENEMY_OBSTACLE_BIT_TYPE_HURDLE;						//キャラクター用種類番号

	//配列が終了したらtrueを受け取って生成終了する
	bool ret = false;

	//配置処理の分岐
	switch( this->m_ObstacleInitArrangeRandomType ){
		//戻り値に配置完了のフラグを受け取る
		case CObstacle::OBSTACLE_HURDLE_ARRANGE_TYPE_STRAIGHT_LINE :
			ret = this->InitArrangeHurdleStraightLine( material );
			break;
		case CObstacle::OBSTACLE_HURDLE_ARRANGE_TYPE_STRAIGHT_RANDOM :
			ret = this->InitArrangeHurdleRandomStraightLine( material );
			break;
	}
		

	//障害物の出現座標を取得
	this->m_lpRoadObstacle[material].ch.sx = this->m_lpRoadObstacle[material].ch.x;									//X

	//▼　速度を代入
	this->m_lpRoadObstacle[material].ch.speed = CObstacle::ROADENEMY_OBSTACLE_HURDLE_ABSOLUTE_MOVE_Y;//速度倍率
	
	//▼　アニメーション設定
	this->m_lpRoadObstacle[material].ani.fram = CObstacle::ROADENEMY_OBSTACLE_HURDLE_ANIMATION_FRAM;	//フレーム数
	this->m_lpRoadObstacle[material].ani.max = CObstacle::ROADENEMY_OBSTACLE_HURDLE_ANIMATION_CNT_MAX;	//最大コマ数

	//出現させる種類をランダムで選定
	this->m_lpRoadObstacle[material].colorType = hurdleType;

	//表示元座標Yに種類に応じた座標を代入
	this->m_lpRoadObstacle[material].ch.picy = this->m_lpRoadObstacle[material].colorType * this->m_lpRoadObstacle[material].ch.h;
	this->m_lpRoadObstacle[material].ch.picx = 0;

	//アニメーション時の表示元Y座標を取得
	this->m_lpRoadObstacle[material].ch.direction = this->m_lpRoadObstacle[material].colorType;

	//設定数まで初期化を行う
	return ret;

}


//********************************************************************************************
//障害物1「ハードル」配置の初期化
//横一直線に配置する
//********************************************************************************************
bool	CObstacle::InitArrangeHurdleStraightLine( int material ){

	//カメラ座標を取得
	float comeCameraX = GetCameraX();						//X
	float comeCameraY = GetCameraY();						//Y

	//出現座標
	//ハードルは道路の道幅にだけ出現させる
	char posX = this->m_lpCreationOBstacleCnt[0];			//X

	//表示先Y
	char posY = 3;

	//出現させて画面スクロールに合わせた座標変更用データテーブル
	float fitScrolX[ 1 ][ CObstacle::OBSTACLE_HURDLE_LINE_MAX ] = {
		
		{ CObstacle::OBSTACLE_FITTING_SCROL_POSITION_X , 
		CObstacle::OBSTACLE_FITTING_SCROL_POSITION_X - 50.0f ,
		CObstacle::OBSTACLE_FITTING_SCROL_POSITION_X ,
		CObstacle::OBSTACLE_FITTING_SCROL_POSITION_X - 50.0f , 
		CObstacle::OBSTACLE_FITTING_SCROL_POSITION_X } ,
	};

	//出現間隔
	float spaceX[ 1 ][ CObstacle::OBSTACLE_HURDLE_LINE_MAX ] = {
	
		{ 0 , CObstacle::OBSTACLE_HURDLE_SPACE_X ,										//中央より左側
		CObstacle::OBSTACLE_HURDLE_SPACE_X ,											//中央
		CObstacle::OBSTACLE_HURDLE_SPACE_X , CObstacle::OBSTACLE_HURDLE_SPACE_X } ,		//中央より右側
	
	};
	

	//▼ 配置
	this->m_lpRoadObstacle[material].ch.x = CObstacle::OBSTACLE_HURDLE_POSITION_START_X  + 
		(float)( ( this->m_lpRoadObstacle[material].ch.w * this->m_lpRoadObstacle[material].ch.scale + spaceX[0][posX] ) * posX );	//表示先X
	this->m_lpRoadObstacle[material].ch.y = (float)( comeCameraY - this->m_lpRoadObstacle[material].ch.h * posY );					//表示先Y

	//障害物の画面位置に応じたX軸の可変値を取得
	this->m_lpRoadObstacle[material].fitPos.fX = fitScrolX[0][posX];

	//出現数をカウントアップ
	this->m_lpCreationOBstacleCnt[0]++;

	//横一直線に並べたらreturn
	if (this->m_lpCreationOBstacleCnt[0] >= CObstacle::OBSTACLE_HURDLE_LINE_MAX) return true;


	//設定数まで初期化を行う
	return false;

}

/**************************************************************************************************
ハードルの配置の初期化
横一直線にランダムで並べる
***************************************************************************************************
*/
bool	CObstacle::InitArrangeHurdleRandomStraightLine( int material ){
	
	//カメラ座標を取得
	float comeCameraX = GetCameraX();						//X
	float comeCameraY = GetCameraY();						//Y

	//出現座標
	//ランダムで配置を決める
	char posXTbl[ CObstacle::OBSTACLE_HURDLE_LINE_ARRANGE_MAX ][ 4 ] = {

		{ 0 , 2 , 4 } ,		 //右端、中央、左端の配置
		{ 1 , 2 , 3 } ,		 //端だけ空けた配置
		{ 1 , 2 , 3 , 4 } ,  //左端だけ空けた配置
		{ 0 , 1 , 2 , 3 } ,	 //右端だけ空けた配置
		{ 0 , 1 , 3 , 4 } ,  //真ん中だけ空けた配置
	
	};

	//出現数
	char popMaxTbl[ CObstacle::OBSTACLE_HURDLE_LINE_ARRANGE_MAX ] = { 3 , 3 , 4 , 4 , 4 };

	//出現間隔
	float spaceX[ CObstacle::OBSTACLE_HURDLE_LINE_MAX ] = {
	
		 0 , CObstacle::OBSTACLE_HURDLE_SPACE_X ,										//中央より左側
		CObstacle::OBSTACLE_HURDLE_SPACE_X ,											//中央
		CObstacle::OBSTACLE_HURDLE_SPACE_X , CObstacle::OBSTACLE_HURDLE_SPACE_X			//中央より右側
	
	};

	//表示先Y
	char posY = 3;

	//出現させて画面スクロールに合わせた座標変更用データテーブル
	float fitScrolX[ 1 ][ CObstacle::OBSTACLE_HURDLE_LINE_MAX ] = {
		
		{ CObstacle::OBSTACLE_FITTING_SCROL_POSITION_X , 
		CObstacle::OBSTACLE_FITTING_SCROL_POSITION_X - 50.0f ,
		CObstacle::OBSTACLE_FITTING_SCROL_POSITION_X ,
		CObstacle::OBSTACLE_FITTING_SCROL_POSITION_X - 50.0f ,
		CObstacle::OBSTACLE_FITTING_SCROL_POSITION_X } ,
	};

	//▼ 配置
	this->m_lpRoadObstacle[material].ch.x = CObstacle::OBSTACLE_HURDLE_POSITION_START_X  + 
		(float)( ( this->m_lpRoadObstacle[material].ch.w * this->m_lpRoadObstacle[material].ch.scale + 
		spaceX[ posXTbl[ this->m_ObstacleRandomArrangeMax ][this->m_lpCreationOBstacleCnt[0]] ] ) *
		posXTbl[ this->m_ObstacleRandomArrangeMax ][this->m_lpCreationOBstacleCnt[0]] );							//表示先X

	this->m_lpRoadObstacle[material].ch.y = (float)( comeCameraY - this->m_lpRoadObstacle[material].ch.h * posY );	//表示先Y

	//障害物の画面位置に応じたX軸の可変値を取得
	this->m_lpRoadObstacle[material].fitPos.fX = fitScrolX[ 0 ][ posXTbl [ this->m_ObstacleRandomArrangeMax ]
	[this->m_lpCreationOBstacleCnt[0]] ];

	//出現数をカウントアップ
	this->m_lpCreationOBstacleCnt[0]++;

	//一定の数を並べたらreturn
	if (this->m_lpCreationOBstacleCnt[0] >= popMaxTbl[this->m_ObstacleRandomArrangeMax]) return true;

	//設定数まで初期化を行う
	return false;

}

//********************************************************************************************
//障害物1「ハードル」の行動の更新
//********************************************************************************************
void   CObstacle::UpdateActionObstacleHurdle( int material ){

	//カメラの座標を取得
	float comeCameraY = GetCameraY();

	//障害物のローカル座標を求める
	float localCameraY = this->m_lpRoadObstacle[material].ch.y - comeCameraY;


	//破壊フラグが立っていたらアニメーションさせる
	if( this->m_lpRoadObstacle[material].ani.type == CObstacle::ROADENEMY_ANIMATION_FLAG_BREAK ){

		//破壊フラグ中、かつ、最後のアニメーションまでコマが来たらフラグを消す
		if( UpdateAnimation( &this->m_lpRoadObstacle[material].ch , &this->m_lpRoadObstacle[material].ani ) == false ){

			//生存フラグを消す
			this->m_lpRoadObstacle[material].ch.flag = false;

			//破壊フラグを通常に戻す
			this->m_lpRoadObstacle[material].ani.type &= ~CObstacle::ROADENEMY_ANIMATION_FLAG_BREAK;
			return;
		
		}
	}

	
	//▼　手前に来るほどに大きく見せる処理
	FittingPositionToBackCameraAngle( &this->m_lpRoadObstacle[material].ch , 380.0f / 640.0f );

	//障害物のローカル座標が縦画面サイズの、
	//どの割合に位置しているかを計算
	float localRateY = localCameraY / 480.0f;


	//▼移動処理

	//配置の中央（posX = 2）に位置を基点に左右に応じて画面スクロールに合わせて位置をずらしていく
	//画面中央から左に配置する角度
	if( this->m_lpRoadObstacle[material].ch.sx < ( 320 - this->m_lpRoadObstacle[material].ch.w / 2 ) ){

		//始点座標を元に現在のY座標に対して位置をずらす
		this->m_lpRoadObstacle[material].ch.x = this->m_lpRoadObstacle[material].ch.sx - ( this->m_lpRoadObstacle[material].fitPos.fX * localRateY );
	}

	//中央より右に配置された場合
	if( this->m_lpRoadObstacle[material].ch.sx > ( 320 + this->m_lpRoadObstacle[material].ch.w / 2 ) ){

		//始点座標を元に現在のY座標に対して位置をずらす
		this->m_lpRoadObstacle[material].ch.x = this->m_lpRoadObstacle[material].ch.sx + ( this->m_lpRoadObstacle[material].fitPos.fX * localRateY );	
	}

}


//********************************************************************************************
//障害物2「岩石」の行動の初期化
//********************************************************************************************
bool	CObstacle::InitObstacleRock( int material ){

	//移動量Xを扱う
	float direction[2] = {
		( -1 * CObstacle::ROADENEMY_OBSTACLE_ROCK_ABSOLUTE_MOVE_X ) , CObstacle::ROADENEMY_OBSTACLE_ROCK_ABSOLUTE_MOVE_X
	};
	//表示座標
	char posX = RandomNum( 9 , 0 );		//X
	char posY = RandomNum( 5 , 1 );		//Y

	//▼　各種設定
	this->m_lpRoadObstacle[material].ch.w = CObstacle::ROADENEMY_OBSTACLE_ROCK_SIZE_WIDTH;			//幅
	this->m_lpRoadObstacle[material].ch.h = CObstacle::ROADENEMY_OBSTACLE_ROCK_SIZE_HEIGHT;			//高さ
	this->m_lpRoadObstacle[material].ch.x = (float)( GetCameraX() + this->m_lpRoadObstacle[material].ch.w * posX );				//表示先X
	this->m_lpRoadObstacle[material].ch.y = (float)( GetCameraY() - this->m_lpRoadObstacle[material].ch.h * posY );				//表示先Y
	this->m_lpRoadObstacle[material].ch.bitnum = ROADENEMY_OBSTACLE_TYPE_ROCK;						//表示要素番号
	this->m_lpRoadObstacle[material].ch.scale = CObstacle::ROADENEMY_OBSTACLE_ROCK_SIZE_SCALE;		//現在の拡縮倍率
	this->m_lpRoadObstacle[material].ch.defaultScale = CObstacle::ROADENEMY_OBSTACLE_ROCK_SIZE_SCALE;//拡縮倍率初期値
	this->m_lpRoadObstacle[material].bitType = ROADENEMY_OBSTACLE_BIT_TYPE_ROCK;					//キャラクター用種類番号
	
	//生成した数をカウント
	this->m_lpCreationOBstacleCnt[1]++;

	//▼　速度を代入
	this->m_lpRoadObstacle[material].ch.mx = direction[ RandomNum( 1 , 0 ) ];						//X移動量
	this->m_lpRoadObstacle[material].ch.my = CObstacle::ROADENEMY_OBSTACLE_ROCK_ABSOLUTE_MOVE_Y;	//Y移動量
	this->m_lpRoadObstacle[material].ch.speed = 1.0f;												//速度倍率
	
	//▼　アニメーション設定
	this->m_lpRoadObstacle[material].ani.fram = CObstacle::ROADENEMY_OBSTACLE_ROCK_ANIMATION_FRAM;		//フレーム数
	this->m_lpRoadObstacle[material].ani.max = CObstacle::ROADENEMY_OBSTACLE_ROCK_ANIMATION_CNT_MAX;	//最大コマ数

	//岩が転がる音
	PlaySound( this->m_lpRoadEnemyEffectSound[0] , 0 , false );

	//表示元
	this->m_lpRoadObstacle[material].ch.picx = 0;

	//岩石が一定の数生成されていたらreturn
	if (this->m_lpCreationOBstacleCnt[1] >= (this->m_ObstacleCreateMax - (CObstacle::OBSTACLE_HURDLE_LINE_MAX + 2))) return true;

	//設定数まで初期化を行う
	return false;

}

//********************************************************************************************
//障害物2「岩石」の行動の更新
//********************************************************************************************
void   CObstacle::UpdateActionObstacleRock( int material ){

	//カメラの座標を取得
	float comeCameraY = GetCameraY();

	//障害物のローカル座標を求める
	float localCameraY = this->m_lpRoadObstacle[material].ch.y - comeCameraY;

	//アニメーションのデータテーブル
	char obstacleAnimationTbl[2] = {
	
		CObstacle::ROADENEMY_ANIMATION_FLAG_NORMAL ,		//通常時
		CObstacle::ROADENEMY_ANIMATION_FLAG_BREAK  ,		//破壊
	
	};

	//破壊フラグが立っていたらアニメーションを変化させる
	if( this->m_lpRoadObstacle[material].ani.type == obstacleAnimationTbl[this->m_lpRoadObstacle[material].ani.type] ){

		//表示元座標を変える
		this->m_lpRoadObstacle[material].ch.direction = obstacleAnimationTbl[this->m_lpRoadObstacle[material].ani.type];

		//破壊フラグ中、かつ、最後のアニメーションまでコマが来たらフラグを消す
		if( UpdateAnimation( &this->m_lpRoadObstacle[material].ch , &this->m_lpRoadObstacle[material].ani ) == false && 
			this->m_lpRoadObstacle[material].ani.type == obstacleAnimationTbl[1] ){

			//生存フラグを消す
			this->m_lpRoadObstacle[material].ch.flag = false;

			//破壊フラグを通常に戻す
			this->m_lpRoadObstacle[material].ani.type &= ~obstacleAnimationTbl[1];
			return;
		
		}
	}

	//▼　手前に来るほどに大きく見せる処理
	FittingPositionToBackCameraAngle( &this->m_lpRoadObstacle[material].ch , 380.0f / 640.0f );

	//▼　移動量を更新
	this->m_lpRoadObstacle[material].ch.x += this->m_lpRoadObstacle[material].ch.mx * this->m_lpRoadObstacle[material].ch.speed;			//X
	this->m_lpRoadObstacle[material].ch.y += this->m_lpRoadObstacle[material].ch.my * this->m_lpRoadObstacle[material].ch.speed;			//Y
}

/*
***********************************************************************************************
引数のキャラクター構造体の索敵範囲で判定を取る
引数   : キャラクター構造体
戻り値 : 障害物の種類番号を返す
***********************************************************************************************
*/
BYTE	CObstacle::BroadenCollisionSquareObstacle( CHARACTER* pch ){

	//戻り値用
	BYTE ret = ROADENEMY_OBSTACLE_BIT_TYPE_VACANT;			//障害物なし

	//障害物の数だけまわす
	for( int i = 0 ; i < this->m_ObstacleCreateMax ; i++ ){

		//引数のキャラクターが広げている索敵範囲と判定を取る
		if( BroadenCollisionSquare( &this->m_lpRoadObstacle[i].ch , pch ) ){

			//衝突する障害物の種類のビット目を返す
			ret |= this->m_lpRoadObstacle[i].bitType;
			
			break;
		}
	}

	//最終的な判定を返す
	return ret;

}


//************************************************************************************************
//障害物と仮の当たり判定を取る　AttempCollisionSquare
//************************************************************************************************
BYTE	CObstacle::AttemptCollisionSquare( CHARACTER* pch1 ){


	//障害物のどの面と当たったかを返す
	BYTE bump = CObstacle::OBSTACLE_NOT_BUMP;

	//キャラクターの1フレーム分の移動量を求めて移動先の座標を求める
	float moveX = pch1->x + ( pch1->mx * pch1->speed );								//X
	float moveY = pch1->y + ( pch1->my * pch1->speed );								//Y

	//幅高さを拡縮倍率通りに計算
	//キャラクター
	float charaRectW = pch1->w * pch1->scale;										//幅
	float charaRectH = pch1->h * pch1->scale;										//高さ


	//▼画面上に残っている障害物と判定を取る
	for( int i = 0 ; i < this->m_ObstacleCreateMax ; i++ ){

		if( this->m_lpRoadObstacle[i].ch.flag ){

			//障害物の幅高さを計算
			float obstaRectW = this->m_lpRoadObstacle[i].ch.w * this->m_lpRoadObstacle[i].ch.scale;//幅
			float obstaRectH = this->m_lpRoadObstacle[i].ch.h * this->m_lpRoadObstacle[i].ch.scale;//高さ

			//pch1の左端と障害物の右端の矩形で判定
			if( moveX < this->m_lpRoadObstacle[i].ch.x + obstaRectW &&
				moveX > this->m_lpRoadObstacle[i].ch.x &&
				moveY < this->m_lpRoadObstacle[i].ch.y + obstaRectH &&
				moveY + charaRectH > this->m_lpRoadObstacle[i].ch.y ){
				
				//戻り値を代入
				bump = CObstacle::OBSTACLE_BUMP_RIGHT;											//右
				break;

			}				

			//pch1の上端と障害物の下端の矩形で判定
			if( moveX < this->m_lpRoadObstacle[i].ch.x + obstaRectW &&
				moveX + charaRectW > this->m_lpRoadObstacle[i].ch.x &&
				moveY < this->m_lpRoadObstacle[i].ch.y + obstaRectH &&
				moveY > this->m_lpRoadObstacle[i].ch.y ){
				
				//戻り値を代入
				bump = CObstacle::OBSTACLE_BUMP_BOTTOM;										//下
				break;
			}

			//pch1の右端と障害物の左端の矩形で判定
			if( moveX + charaRectW < this->m_lpRoadObstacle[i].ch.x + obstaRectW &&
				moveX + charaRectW > this->m_lpRoadObstacle[i].ch.x &&
				moveY < this->m_lpRoadObstacle[i].ch.y + obstaRectH &&
				moveY + charaRectH > this->m_lpRoadObstacle[i].ch.y ){
				
				//戻り値を代入
				bump = CObstacle::OBSTACLE_BUMP_LEFT;											//左
				break;
			}

			//pch1の下端と障害物の上端の矩形で判定
			if( moveX < this->m_lpRoadObstacle[i].ch.x + obstaRectW &&
				moveX + charaRectW > this->m_lpRoadObstacle[i].ch.x &&
				moveY + charaRectH < this->m_lpRoadObstacle[i].ch.y + obstaRectH &&
				moveY + charaRectH > this->m_lpRoadObstacle[i].ch.y ){

				//戻り値を代入
				bump = CObstacle::OBSTACLE_BUMP_TOP;											//上
				break;
			}
		}
	}

	//最終的にどこの面と接触するかを返す
	return bump;

}


//************************************************************************************************
//画像の要素を設定する
//************************************************************************************************
void	CObstacle::SetRoadEnemyImage( int* lpImage , int max ){

	//引数の最大数だけ画像の要素を確保する
	this->m_lpRoadEnemyImg = new int [ max ];

	//引数の最大数だけ画像の要素を設定する
	for( int i = 0 ; i < max ; i++ ){ this->m_lpRoadEnemyImg[i] = lpImage[i]; }
	
}

/*
****************************************************************************************************
効果音の要素を設定する
****************************************************************************************************
*/
void	CObstacle::SetRoadEnemyEffectSound( int *lpSound , int max ){

	//引数の数だけメモリ確保
	this->m_lpRoadEnemyEffectSound = new int [ max ];
	//効果音の要素を生成
	for( int i = 0 ; i < max ; i++ ){ this->m_lpRoadEnemyEffectSound[i] = lpSound[i]; }
}

//*******************************************************************************************
//当たり判定
//*******************************************************************************************
char	CObstacle::CollisionRoadEnemy(RUNNER* pRunner) {

	//戻り値用
	char ret = RUNNER_NOT_BUMP;				//衝突なし

	//障害物の数だけ回す
	for (int i = 0; i < this->m_ObstacleCreateMax; i++) {

		//生存フラグがtrue、かつ、破壊フラグが立っていなかったら判定を取る
		if (this->m_lpRoadObstacle[i].ch.flag &&
			this->m_lpRoadObstacle[i].ani.type == CObstacle::ROADENEMY_ANIMATION_FLAG_NORMAL) {

			//引数のキャラクターと衝突する障害物との判定を少し、ランナーの矩形に喰い込ませて判定する
			//その際に、ランナーの前方の衝突
			if (this->m_lpCPlayerRunner->CollisionRunner(&pRunner->ch, &this->m_lpRoadObstacle[i].ch)) {

				//initialize obstacles when player bump to obstacle
				switch (this->m_lpRoadObstacle[i].ch.type) {
				case ROADENEMY_OBSTACLE_TYPE_HURDLE:
					ret = this->CollisionInitializeHurdle(pRunner, i);				//initialize hurdle
					if(ret != RUNNNER_BUMP_TO_HURDLE) return ret;					//for breaking hurdle process
					break;
				case ROADENEMY_OBSTACLE_TYPE_ROCK:
					ret = this->CollisionInitializeRock(pRunner);					//initialize rock
					break;
				}
				//キャラクターが攻撃か無敵状態なら破壊フラグを立てる
				if (pRunner->actionF == RUNNER_ACTION_ATTACK ||
					pRunner->effectType == RUNNER_EFFECT_TYPE_ABSOLUTE ||
					pRunner->effectType == RUNNER_EFFECT_TYPE_SPEED_AND_ABSOLUTE) {
					//無敵状態ならカウントしない
					if (pRunner->effectType != RUNNER_EFFECT_TYPE_ABSOLUTE ||
						pRunner->effectType != RUNNER_EFFECT_TYPE_SPEED_AND_ABSOLUTE) {
						//破壊数をカウント
						pRunner->defeatCnt[this->m_lpRoadObstacle[i].ch.type]++;
					}
					PlaySound(this->m_lpRoadEnemyEffectSound[1], 0, false);			//destroyed obstacle sound
					//破壊フラグを立てる
					this->m_lpRoadObstacle[i].ani.type |= CObstacle::ROADENEMY_ANIMATION_FLAG_BREAK;
					//アニメーション設定
					this->m_lpRoadObstacle[i].ani.time = 0;		//時間
					this->m_lpRoadObstacle[i].ani.cnt = 0;		//コマ数
					//フラグは衝突していない状態とする
					ret = RUNNER_NOT_BUMP;
					continue;
				}
				break;
			}
		}
	}
	//最終的に当たっているかをいないかを返す
	return ret;
}

/*
initialize process for hurdle when player bump to hurdle
*/
char	CObstacle::CollisionInitializeHurdle( RUNNER *pRunner , int material) {

	//一度接触したハードルとは判定を取らない
	if (pRunner->obstacleBumpNow == material) return RUNNER_NOT_BUMP;

	//効果フラグデータテーブル
	BYTE bumpTbl[RUNNER_EFFECT_KIND_OF_TYPE] = {

		RUNNER_EFFECT_FLAG_ABSOLUTE				,		//無敵状態
		RUNNER_EFFECT_FLAG_SPEED_UP 			,		//速度上昇
		RUNNER_EFFECT_FLAG_HEALING				,		//回復

	};
	//出現させるハードルの種類をまとめたデータテーブル
	tagHURDLECOLORTYPE hurdleTypeTbl[RUNNER_EFFECT_KIND_OF_TYPE] = {

		ROADENEMY_OBSTACLE_HURDLE_COLOR_RED	,			//赤
		ROADENEMY_OBSTACLE_HURDLE_COLOR_BLUE	,		//青
		ROADENEMY_OBSTACLE_HURDLE_COLOR_GREEN ,			//緑
	};

	//戻り値用
	BYTE	ret = RUNNNER_BUMP_TO_HURDLE;

	//障害物の要素数を取得
	pRunner->obstacleBumpNow = material;
	
	//障害物① ハードル
	//障害物がハードル、キャラクターの行動がジャンプ中（アニメーション）であれば
	//破壊処理を行わないで当たり判定で効果フラグを返す
	if (pRunner->actionF == RUNNER_ACTION_JUMP ||
		pRunner->actionF == RUNNER_ACTION_JUMP_AND_ATTACK) {
		//黒のハードルなら判定を取らずにreturnする
		if (this->m_lpRoadObstacle[material].colorType == ROADENEMY_OBSTACLE_HURDLE_COLOR_BLACK) return RUNNER_NOT_BUMP;
		//ハードルの色の種類だけ回す
		for (int j = 0; j < RUNNER_EFFECT_KIND_OF_TYPE; j++) {
			//ハードルの色に応じて効果を分岐させる
			if (this->m_lpRoadObstacle[material].colorType == hurdleTypeTbl[j]) {
				//効果の種類を返す
				ret = bumpTbl[j];
				break;
			}
		}
	}
	return ret;
}

/*
initialize process for rock when player bump to hurdle
*/
char	CObstacle::CollisionInitializeRock(RUNNER *pRunner) {
	return RUNNNER_BUMP_TO_ROCK;
}