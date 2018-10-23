#include	"CompetitorRunner.h"
#include	"CPlayerRunner.h"
#include	"CEffectManager.h"
#include	"CRoadEnemyManager.h"
#include	"CRoadEnemy.h"
#include	"CRoadStage.h"
#include	"animation.h"
#include	"map.h"
#include	"sound.h"
#include	"play.h"
#include	<iostream>
#include	<math.h>

using namespace std;

//アクションの種類数
const int	CCompetitor::COMPETITOR_KIND_OF_EFFECT = 2;
//座標取得用
TPOINT<float>	CCompetitor::m_CompetitorPos = {0};
//表示間隔
const int	CCompetitor::COMPETITOR_INDICATE_SPACE_X = 30;
//画面外に出た際の制限座標
const float	CCompetitor::COMPETITOR_CONSTRAIN_FRONT_POSITION_Y = 500.0f;		//前方
const float	CCompetitor::COMPETITOR_CONSTRAIN_BACK_POSITION_Y = 200.0f;			//後方

//飛び越えた障害物の要素を扱う（取得用）
int CCompetitor::m_getJumpedMaterial = -1;


/*
************************************************************************************************
コンストラクタ
************************************************************************************************
*/
CCompetitor::CCompetitor(){

	this->m_CompetitorRunnerImg = NULL;
	this->m_CompetitorShadowImg = NULL;
	this->m_lpActionRateToHurdle = NULL;
	this->m_lpActionRateToRock = NULL;
	this->m_VariableSpeedNow = 0.0f;
	this->m_GetVariableSpeed = 0.0f;
	this->m_ConstarinSpeedMax = 0.0f;
	this->m_lpCRoadEnemy = NULL;

}


/*
************************************************************************************************
ディストラクタ
************************************************************************************************
*/
CCompetitor::~CCompetitor(){

	//メモリの解放[
	if( this->m_lpActionRateToHurdle != NULL ){
		delete []this->m_lpActionRateToHurdle;	//ハードルに対するアクション確率
		this->m_lpActionRateToHurdle = NULL;
	}
	if( this->m_lpActionRateToRock != NULL ){
		delete []this->m_lpActionRateToRock;	//岩石に対するアクション確率	
		this->m_lpActionRateToRock = NULL;
	}
	if( this->m_lpCPlayerRunner != NULL ){
		delete this->m_lpCPlayerRunner;			//プレイヤー
		this->m_lpCPlayerRunner = NULL;
	}

}

//************************************************************************************************
//競争者の初期化
//************************************************************************************************
void   CCompetitor::InitRunner( int runnerNum , CRoadEnemyManager	*lpRoadEnemy , float roadDistance ){

	//各種メモリの確保
	this->m_lpCPlayerRunner = new CPlayerRunner;				//プレイヤークラス
	//アクションの数だけメモリを確保
	this->m_lpActionRateToHurdle = new int [ COMPETITOR_RUNNER_KIND_OF_ACTION ];		//ハードル
	this->m_lpActionRateToRock = new int [ COMPETITOR_RUNNER_KIND_OF_ACTION ];			//岩石

	//座標取得用に宣言
	TPOINT<float> pos = {0};

	//ゲーム難易度を取得
	tagGAMELEVEL gameLevel = GetGameLevel();

	//プレイヤーの情報取得
	RUNNER	*lpPlayer = this->m_lpCPlayerRunner->GetRunner();

	//キャラクター１人の表示サイズ
	int charaSizeW = (int)( RUNNER_DEFAULT_IMAGE_WIDTH * RUNNER_DEFAULT_IMAGE_SCALE );

	//※優先度が高いアクションほど、後ろのほうに配置する
	//ゲーム難易度に応じて発生確率を変化
	//① ハードル
	int hurdlePerTbl[LEVEL_MAX][COMPETITOR_RUNNER_KIND_OF_ACTION] = {

		//列要素 ： ジャンプ ⇒ 攻撃 ⇒ 移動回避
		{ 80 , 30 , 60 } ,						//EASY
		
		{ 90 , 20 , 50 } ,						//NORMAL

		{ 100 , 0 , 0 } ,						//HARD	
		
	};
	//② 岩石
	int rockPerTbl[LEVEL_MAX][COMPETITOR_RUNNER_KIND_OF_ACTION] = {

		//列要素 ： ジャンプ ⇒ 攻撃 ⇒ 移動回避
		{ 0 , 80 , 60 } ,						//EASY
		
		{ 0 , 90 , 50 } ,						//NORMAL

		{ 0 , 100 , 40 } ,						//HARD	
		
	};

	//現在の自身の位置とプレイヤーの位置に応じた速度の変化
	float	variableSpeedTbl[ LEVEL_MAX ] = { 1.0f , 1.3f , 1.6f };

	//速度制限
	float	constrainSpeedTbl[ LEVEL_MAX ] = { -4.5f , -5.0f , -6.0f };

	//競争者の表示先X座標データ
	float	PosXTbl[10] = {

		( lpPlayer->ch.x + charaSizeW ) + CCompetitor::COMPETITOR_INDICATE_SPACE_X ,				 //1人目
		( lpPlayer->ch.x + ( charaSizeW * 2 ) ) + ( CCompetitor::COMPETITOR_INDICATE_SPACE_X * 2 ) , //2人目
		( lpPlayer->ch.x + ( charaSizeW * 3 ) ) + ( CCompetitor::COMPETITOR_INDICATE_SPACE_X * 3 ) , //3人目
		( lpPlayer->ch.x - charaSizeW ) - CCompetitor::COMPETITOR_INDICATE_SPACE_X ,				 //4人目
		( lpPlayer->ch.x - ( charaSizeW * 2 ) ) - ( CCompetitor::COMPETITOR_INDICATE_SPACE_X * 2 ) , //5人目
		( lpPlayer->ch.x - ( charaSizeW * 3 ) ) - ( CCompetitor::COMPETITOR_INDICATE_SPACE_X * 3 ) , //6人目
		( lpPlayer->ch.x + ( charaSizeW * 4 ) ) + ( CCompetitor::COMPETITOR_INDICATE_SPACE_X * 4 ) , //7人目
		( lpPlayer->ch.x + ( charaSizeW * 5 ) ) + ( CCompetitor::COMPETITOR_INDICATE_SPACE_X * 5 ) , //8人目
		( lpPlayer->ch.x - ( charaSizeW * 4 ) ) - ( CCompetitor::COMPETITOR_INDICATE_SPACE_X * 4 ) , //9人目
		( lpPlayer->ch.x - ( charaSizeW * 5 ) ) - ( CCompetitor::COMPETITOR_INDICATE_SPACE_X * 5 ) , //10人目

	};
	
		
	//▼ 各アクション発生確率
	//障害物管理クラス
	this->m_lpCRoadEnemy = lpRoadEnemy;	
	
	//▼ 表示設定
	//各値を代入
	this->m_RunnerDefault.ch.x = PosXTbl[runnerNum];										//表示先X
	this->m_RunnerDefault.ch.y = roadDistance - CAMERA_FOCUS_POSITION_Y;					//表示先Y
	this->m_RunnerDefault.ch.mx = 0.0f;														//X移動量
	this->m_RunnerDefault.ch.flag = true;													//生存フラグ
	//取得用座標
	pos.x = this->m_RunnerDefault.ch.x;
	pos.y = this->m_RunnerDefault.ch.y;
	this->m_CompetitorPos = pos;
	//速度変化値
	this->m_GetVariableSpeed = variableSpeedTbl[ gameLevel ];
	//速度制限
	this->m_ConstarinSpeedMax = constrainSpeedTbl[ gameLevel ];
	
	//▼ アクション精度を代入			
	//① ハードル
	//ジャンプ
	this->m_lpActionRateToHurdle[0] = hurdlePerTbl[gameLevel][0];
	//攻撃
	this->m_lpActionRateToHurdle[1] = hurdlePerTbl[gameLevel][1];
	//移動回避
	this->m_lpActionRateToHurdle[2] = hurdlePerTbl[gameLevel][2];

	//② 岩石
	//ジャンプ
	this->m_lpActionRateToRock[0] = rockPerTbl[gameLevel][0];
	//攻撃
	this->m_lpActionRateToRock[1] = rockPerTbl[gameLevel][1];
	//移動回避
	this->m_lpActionRateToRock[2] = rockPerTbl[gameLevel][2];



	//アニメーション情報の設定
	SetAnimation( &this->m_RunnerDefault.ch , &this->m_RunnerDefault.ani ,
		this->m_RunnerDefault.ani.sx , this->m_RunnerDefault.ani.sy ,
		this->m_RunnerDefault.ani.max , this->m_RunnerDefault.ani.fram , 
		this->m_RunnerDefault.ani.type , true);

}

//************************************************************************************************
//競争者の更新
//************************************************************************************************
void   CCompetitor::UpdateRunner( int runner , CEffectManager *lpCEffect ){

	//プレイヤーランナーの情報を取得する
	RUNNER *prunner = this->m_lpCPlayerRunner->GetRunner();

	//当たり判定を取らない矩形部分
	//上、右、下、左の順
	TSAFETYRECT<int> safetyRect = { 60 , 10 , 60 , 10 };

	//取得座標用
	TPOINT<float> Pos = { this->m_RunnerDefault.ch.x , this->m_RunnerDefault.ch.y };

	//衝突時に取得する効果データテーブル
	BYTE bumpTbl[ CCompetitor::COMPETITOR_KIND_OF_EFFECT ] = {

		RUNNER_EFFECT_FLAG_ABSOLUTE			,		//無敵状態
		RUNNER_EFFECT_FLAG_SPEED_UP 			,		//速度上昇
	
	};

	//効果データテーブル
	BYTE effectTbl[ CCompetitor::COMPETITOR_KIND_OF_EFFECT ] = {

		RUNNER_EFFECT_TYPE_ABSOLUTE			,		//無敵状態
		RUNNER_EFFECT_TYPE_SPEED_UP			,		//速度上昇
	
	};

	//カメラの座標を取得
	float comeCameraY = GetCameraY();

	//▼ 競争者のアクションフラグテーブル
	BYTE actionTbl[COMPETITOR_RUNNER_KIND_OF_ACTION] = {

		RUNNER_ACTION_JUMP				,	//ジャンプフラグ
		RUNNER_ACTION_ATTACK			,	//攻撃フラグ
		RUNNER_ACTION_LINE_MOVE			,	//横移動フラグ

	};
	
	//▼ 速度の管理
	this->ManageCompetitorRunnerSpeed();

	//▼ ランナー同士の判定（位置修正）
	CheckOverlapCharacter( &prunner->ch , &this->m_RunnerDefault.ch , safetyRect );
	
	//▼ 障害物に対してのアクション設定
	this->SetActionToObstacle();

	//▼ アクション行動のフラグが立っていたら更新処理
	for( int i = 0 ; i < COMPETITOR_RUNNER_KIND_OF_ACTION ; i++ ){
	
		//対応したアクションの更新処理
		if( this->m_RunnerDefault.actionF == actionTbl[i] ){

			//行動の分岐
			switch( i ){
				case 0:
					this->UpdateCompetitorRunnerEvadeHardol();
					break;
				case 1:
					this->UpdateCompetitorRunnerAttack();
					break;
				case 2:
					this->UpdateCompetitorRunnerLineMove();
					break;
			}
			break;
		}
	}

	//▼ 障害物との当たり判定
	//生存フラグがtrueなら
	if( this->m_RunnerDefault.ch.flag ){

		//障害物と判定を取ってフラグを受け取る
		this->m_RunnerDefault.bumpType = this->m_lpCRoadEnemy->CollisionRoadEnemyManager( &this->m_RunnerDefault );
				
		//▼ 強化効果の分岐
		//通常の衝突、衝突していないなら処理をしない
		if (this->m_RunnerDefault.bumpType != RUNNER_NOT_BUMP ||
			this->m_RunnerDefault.bumpType != RUNNNER_BUMP_TO_HURDLE ||
			this->m_RunnerDefault.bumpType != RUNNNER_BUMP_TO_ROCK) {
			//タイプの数だけ回す
			for (int i = 0; i < (RUNNER_EFFECT_KIND_OF_TYPE - 1); i++) {
				//取得した衝突フラグの番号を元に分岐
				if (this->m_RunnerDefault.bumpType == bumpTbl[i]) {
					//衝突した障害物の要素数を更新
					this->m_getJumpedMaterial = this->m_RunnerDefault.obstacleBumpNow;
					//▼ エフェクトの生成
					lpCEffect->CreateEffectManaager(&this->m_RunnerDefault, runner);
					//▼ 効果の分岐
					switch (i) {
					case 0:
						CRunner::RunnerEffectAbsolute();
						break;
					case 1:
						CRunner::RunnerEffectSpeedUp();
						break;
					}
					//衝突フラグをなくす
					this->m_RunnerDefault.bumpType = RUNNER_NOT_BUMP;
					break;
				}
			}
		}
		//▼ 普通に衝突したら処理
		if (this->m_RunnerDefault.bumpType == RUNNNER_BUMP_TO_HURDLE ||
			this->m_RunnerDefault.bumpType == RUNNNER_BUMP_TO_ROCK) {
			//効果音
			PlaySound( this->m_CompetitorEffectSound , 0 , false );
			//▼ 速度上昇効果を無効に
			CRunner::RepealRunnerEffectSpeedUp();		
			//エフェクトの非表示
			lpCEffect->RepealEffectManager( 0 , runner );
			//衝突フラグをなくす
			this->m_RunnerDefault.bumpType = RUNNER_NOT_BUMP; 
			//生存フラグ
			this->m_RunnerDefault.ch.flag = false;
		}
	}

	//▼ 無敵中の処理
	//無敵中なら時間を測り、一定の時間になったら無敵状態を無効に
	if( CRunner::RepealRunnerEffectAbsolute() == false){
		//エフェクトの非表示
		lpCEffect->RepealEffectManager( 1 , runner );
	}

	//▼ エフェクトの更新
	lpCEffect->UpdateEffectManager( &this->m_RunnerDefault , runner );

	//▼ プレイヤー回避処理
	AvoidEnemy( &this->m_RunnerDefault.ch , this->m_RunnerDefault.avoidtime );	

	//▼ 後方カメラに合わせてキャラクターを拡縮小
	FittingPositionToBackCameraAngle( &this->m_RunnerDefault.ch , 380.0f / 640.0f );
	
	//▼ アニメーションの更新
	this->ChangeCompetitorRunnerAnimation();
	
	//▼ 移動範囲の制限
	this->ManageConstrainCompetitorRunnerMove();

	//▼ 現在の座標を代入
	this->m_CompetitorPos = Pos;

}


//************************************************************************************************
//競争者の表示
//************************************************************************************************
void   CCompetitor::DrawRunner(){

	//カメラの座標を取得する
	float comeCameraX = GetCameraX();		//X座標
	float comeCameraY = GetCameraY();		//Y座標

	char mes[32] = {0};

	int x = 480 , y = 0;
/*
	wsprintf( &mes[0] , "Cのタイプ%d" , this->m_RunnerDefault.effectType );
	DDTextOut( &mes[0] , x, y );

	wsprintf( &mes[0] , "Cの衝突タイプ%d" , this->m_RunnerDefault.bumpType );
	DDTextOut( &mes[0] , x , y + 20 );

	sprintf( &mes[0] , "Cの速度%f" , (this->m_RunnerDefault.ch.speed * this->m_RunnerDefault.ch.my) * 10 );
	DDTextOut( &mes[0] , x , y + 40 );
	
	wsprintf( &mes[0] , "CのY座標%d" , (int)this->m_RunnerDefault.ch.y );
	DDTextOut( &mes[0] , x , y + 60 );

	wsprintf( &mes[0] , "Cの無敵時間%d" , this->m_RunnerDefault.absoluteTime );
	DDTextOut( &mes[0] , x , y + 80 );

*/

	//競争者の表示
	if( this->m_RunnerDefault.ch.time % 2 == 0 ){
		
		//影
		DDAlphaBlt(
			(int)( ( this->m_RunnerDefault.ch.x - 5.0f ) - comeCameraX ) ,
			(int)( ( this->m_RunnerDefault.ch.y + this->m_RunnerDefault.ch.h - 
			( m_ShadowDefault.ch.h / 2 ) ) - comeCameraY ) ,
			m_ShadowDefault.ch.w ,
			m_ShadowDefault.ch.h ,
			this->m_CompetitorShadowImg ,
			m_ShadowDefault.ch.picx ,
			m_ShadowDefault.ch.picy ,
			m_ShadowDefault.ch.alpha
			);


		//競争者自身の表示
		DDScaleBlt(
			this->m_RunnerDefault.ch.x  - comeCameraX ,
			this->m_RunnerDefault.ch.y  - comeCameraY ,
			this->m_RunnerDefault.ch.w ,
			this->m_RunnerDefault.ch.h ,
			this->m_CompetitorRunnerImg ,
			this->m_RunnerDefault.ch.picx ,
			this->m_RunnerDefault.ch.picy ,
			this->m_RunnerDefault.ch.scale);

	}

}

//************************************************************************************************
//競争者の解放
//************************************************************************************************
void   CCompetitor::ReleaseRunner(){

	//メモリの解放
	if( this->m_lpActionRateToHurdle != NULL ){
		delete []this->m_lpActionRateToHurdle;	//ハードルに対するアクション確率
		this->m_lpActionRateToHurdle = NULL;
	}
	if( this->m_lpActionRateToRock != NULL ){
		delete []this->m_lpActionRateToRock;	//岩石に対するアクション確率	
		this->m_lpActionRateToRock = NULL;
	}

}

//************************************************************************************************
//競争者の速度を管理する
//************************************************************************************************
void   CCompetitor::ManageCompetitorRunnerSpeed(){

	//速度に準じたタイプ分け
	tagRUNNERANIMATIONSPEED speedType[4] = {

		RUNNER_SPEED_SLOW	,		//遅い
		RUNNER_SPEED_NORMAL ,		//普通
		RUNNER_SPEED_DECENT ,		//少し速い
		RUNNER_SPEED_RAPID	,		//速い	
	
	};

	//現在の速度合計を更新
	this->m_RunnerDefault.aggregateSpeed = ( this->m_RunnerDefault.ch.my * this->m_RunnerDefault.ch.speed ) + 
		this->m_VariableSpeedNow;
	
	//速度の上限
	if( fabs( this->m_RunnerDefault.aggregateSpeed ) >= fabs( this->m_ConstarinSpeedMax ) ){
		//上限を代入
		this->m_RunnerDefault.aggregateSpeed = this->m_ConstarinSpeedMax;
	}

	//速度の下限
	if( fabs( this->m_RunnerDefault.aggregateSpeed ) <= fabs( RUNNER_CONSTRAIN_MINIMUM_SPEED ) ){
		//下限を代入
		this->m_RunnerDefault.aggregateSpeed = RUNNER_CONSTRAIN_MINIMUM_SPEED;
	}

	//▼速度レベルを更新
	//高い順から調べて現在速度が上回ったらループを抜けて、アニメーションを更新
	//通常時のみ反映
	if( this->m_RunnerDefault.ani.type == RUNNER_ANIMATION_ROUND ){
		for( int i = 3 ; i >= 0 ; i-- ){
			
			//現在の速度が一定の値を上回っていたら
			if( fabs( this->m_RunnerDefault.ch.my ) * this->m_RunnerDefault.ch.speed > 
				i * RUNNER_SPEED_ANIMATION_RATE ){
				
				//速度レベルのマクロを代入
				this->m_RunnerDefault.ani.fram = speedType[i];
				break;
			}
		}
	}
	//生存フラグがtrueなら移動量を加算する
	if( this->m_RunnerDefault.ch.flag ){
		this->m_RunnerDefault.ch.y += this->m_RunnerDefault.aggregateSpeed;	//Y
	}

}


//************************************************************************************************
//競争者の移動範囲を制御する
//************************************************************************************************
void   CCompetitor::ManageConstrainCompetitorRunnerMove(){


	//カメラの座標を取得
	float comeCameraY = GetCameraY();
	//プレイヤーの情報を取得
	RUNNER	*lpPlayer = this->m_lpCPlayerRunner->GetRunner();

	//▼移動範囲を制限
	//画面上端
	//引き離しすぎないように制限を設ける
	if( this->m_RunnerDefault.ch.y < ( comeCameraY - CCompetitor::COMPETITOR_CONSTRAIN_FRONT_POSITION_Y ) ){

		//画面上端から制限座標まで
		this->m_RunnerDefault.ch.y = ( comeCameraY - CCompetitor::COMPETITOR_CONSTRAIN_FRONT_POSITION_Y );
	}
	
	//calculate competitor's coordinateY in local coordinateY
	float localY = this->m_RunnerDefault.ch.y - comeCameraY;
	//calculate railing's coordinateY into rate of coordinateY in stage's height
	float rateY = ((float)STAGE_HEIGHT - localY) / (float)(STAGE_HEIGHT - ROADSTAGE_SUBTLTY_POSITIONX_FOR_ROAD_WIDTH);
	//constrain move
	float margin = ROADSTAGE_MARGIN_WIDTH_FOR_TOP_AND_BOTTOM_IN_ROAD * rateY;
	//画面左端
	if (this->m_RunnerDefault.ch.x < margin) this->m_RunnerDefault.ch.x = margin;

	//画面右端
	if (this->m_RunnerDefault.ch.x + (this->m_RunnerDefault.ch.w * this->m_RunnerDefault.ch.scale) >
		(STAGE_WIDTH - margin)) {
		this->m_RunnerDefault.ch.x = (STAGE_WIDTH - margin) - (this->m_RunnerDefault.ch.w * this->m_RunnerDefault.ch.scale);
	}
		
	//画面下端
	//引き離されすぎないように制限を設ける
	if( this->m_RunnerDefault.ch.y + ( this->m_RunnerDefault.ch.h * 
		this->m_RunnerDefault.ch.scale ) > ( comeCameraY + STAGE_HEIGHT ) + CCompetitor::COMPETITOR_CONSTRAIN_BACK_POSITION_Y ){
		
		//画面下端から制限座標まで
		this->m_RunnerDefault.ch.y = 
		( ( comeCameraY + STAGE_HEIGHT ) + CCompetitor::COMPETITOR_CONSTRAIN_BACK_POSITION_Y ) - 
		( this->m_RunnerDefault.ch.h * this->m_RunnerDefault.ch.scale );
		//プレイヤーの現在の速度より少し速度を上げる
		this->m_VariableSpeedNow = -1 * this->m_GetVariableSpeed;
	}

	//追い上げ中にプレイヤー付近のY座表示達したら、速度変化値をリセットする
	if( this->m_RunnerDefault.ch.y <= lpPlayer->ch.y ) this->m_VariableSpeedNow = 0;

}



/**********************************************************************************************************************
障害物の種類に応じて適切な行動を確率で判断する
**************************************************************************************************************************/
void   CCompetitor::SetActionToObstacle(){


	//▼ 障害物のビット目の種類をまとめたデータテーブル
	BYTE obstacleTbl[ ROADENEMY_KIND_OF_OBSTACLE ] = {

		ROADENEMY_OBSTACLE_BIT_TYPE_HURDLE	,			//ハードル
		ROADENEMY_OBSTACLE_BIT_TYPE_ROCK	,			//岩石
	
	};
	
	//▼ 各障害物に対するアクション発生確率をまとめたデータテーブル
	int actionPerTbl[ROADENEMY_KIND_OF_OBSTACLE][COMPETITOR_RUNNER_KIND_OF_ACTION] = {

		//① ハードル
		//列：攻撃 ⇒ 移動回避 ⇒ ジャンプ
		{ this->m_lpActionRateToHurdle[1] , this->m_lpActionRateToHurdle[2] , this->m_lpActionRateToHurdle[0] } ,

		//② 岩石
		//列：ジャンプ ⇒ 移動回避 ⇒ 攻撃
		{ this->m_lpActionRateToRock[0] , this->m_lpActionRateToRock[2] , this->m_lpActionRateToRock[1] } ,
	
	};

	
	//▼ 競争者のアクションフラグテーブル
	tagCOMPETITORACTIONTYPE actionTbl[ROADENEMY_KIND_OF_OBSTACLE][COMPETITOR_RUNNER_KIND_OF_ACTION] = {
	
		//ハードル
		{ COMPETITOR_RUNNER_ACTION_TYPE_ATTACK			,	//攻撃フラグ
		  COMPETITOR_RUNNER_ACTION_TYPE_EVADE			,	//回避移動フラグ
  		  COMPETITOR_RUNNER_ACTION_TYPE_JUMP			,	//ジャンプフラグ
		},

		//岩石
		{ COMPETITOR_RUNNER_ACTION_TYPE_JUMP			,	//ジャンプフラグ
		  COMPETITOR_RUNNER_ACTION_TYPE_EVADE			,	//回避移動フラグ
  		  COMPETITOR_RUNNER_ACTION_TYPE_ATTACK			,	//攻撃フラグ
		},

	};
					
	//▼ 障害物を索敵
	//何も見つけていなかったら
	if( this->m_RunnerDefault.getType == ROADENEMY_OBSTACLE_BIT_TYPE_VACANT ){
	
		//障害物の種類を取得する
		this->m_RunnerDefault.getType |= 
			this->m_lpCRoadEnemy->BroadenCollisionSquareToObstacle( &this->m_RunnerDefault.ch );

		//アクションの種類をリセット
		this->m_RunnerDefault.actionType = COMPETITOR_RUNNER_ACTION_TYPE_VACANT;
	
	}

	//障害物の種類だけ回す
	for( int i = 0 ; i < ROADENEMY_KIND_OF_OBSTACLE ; i++ ){

		//取得した障害物の種類に応じて処理を分岐させる
		//障害物を見つけていたら処理する		
		if( this->m_RunnerDefault.actionF == RUNNER_ACTION_ROUND && 
			this->m_RunnerDefault.getType == obstacleTbl[i] ){
				
			//衝突フラグにする
			this->m_RunnerDefault.avoidF |= RUNNER_BUMP_ATTENTION;

			//障害物に対して行動の精度を確率で取得
			//アクションタイプを代入
			for( int j = 0 ; j < COMPETITOR_RUNNER_KIND_OF_ACTION ; j++ ){


				//引数に確率を渡して、どのアクションを行うかを決める
				this->m_RunnerDefault.actionType = 
					this->ConmpetitorAttemptDoAction( actionPerTbl[i][j] , actionTbl[i][j] );

				//何かのアクションを取得していたらループを抜ける
				if( this->m_RunnerDefault.actionType != COMPETITOR_RUNNER_ACTION_TYPE_VACANT )	break;
			
			}

		//もしアクションタイプを取得できなかった場合は、returnして何もしない
		if( this->m_RunnerDefault.actionType == COMPETITOR_RUNNER_ACTION_TYPE_VACANT ) return;

		//対応した障害物があったのでループを抜ける
		break;	
		
		}
		
	}
		
	//衝突フラグが立っていて、なおかつ、回避フラグ中でなければ行動の設定
	if( this->m_RunnerDefault.avoidF == RUNNER_BUMP_ATTENTION && 
		this->m_RunnerDefault.avoidF != RUNNER_WHILE_AVOID ){

		//衝突フラグをなくす
		this->m_RunnerDefault.avoidF &= ~RUNNER_BUMP_ATTENTION;

		//▼ 設定の分岐
		switch( this->m_RunnerDefault.actionType ){
			case 0:
				this->SetCompetitorRunnerEvadeHardol();
				break;
			case 1:
				this->SetCompetitorRunnerAttack();
				break;
			case 2:
				this->SetCompetitorRunnerLineMove();
				break;
		}
	}

}


//*************************************************************************************************
//競争者の行動１の初期化「前方に障害物があれば横に移動」
//*************************************************************************************************
void   CCompetitor::SetCompetitorRunnerLineMove() {

	//get current global coordinateY
	float gY = GetCameraY();

	//回避フラグをONにする
	this->m_RunnerDefault.avoidF |= RUNNER_WHILE_AVOID;

	//横移動フラグをON
	this->m_RunnerDefault.actionF |= RUNNER_ACTION_LINE_MOVE;

	//回避移動距離を０にリセット
	this->m_RunnerDefault.avoidDistance = 0;

	//プレイヤーの右端の座標を求める
	float rectRightPos = this->m_RunnerDefault.ch.x + (this->m_RunnerDefault.ch.w * this->m_RunnerDefault.ch.scale);

	//ランダム値
	//移動方向をとりあえずランダムで
	int selectRoot = RandomNum(1, 0);

	//▼プレイヤーの座標が画面に近いなら反対の移動方向、移動先を指定する
	//左端なら
	if (this->m_RunnerDefault.ch.x >= 0 &&
		this->m_RunnerDefault.ch.x <= this->m_RunnerDefault.ch.w) {
		//移動方向、移動先はプラスの座標へ
		selectRoot = 0;
	}

	//calculate competitor's coordinateY in local coordinateY
	float localY = this->m_RunnerDefault.ch.y - gY;
	//calculate competitor's coordinateY into rate of coordinateY in stage's height
	float rateY = ((float)STAGE_HEIGHT - localY) / (float)STAGE_HEIGHT;
	//constrain move
	float margin = 125.0f * rateY;
	//右端なら
	if (rectRightPos <= (STAGE_WIDTH - margin) &&
		rectRightPos >= ((STAGE_WIDTH - this->m_RunnerDefault.ch.w) - margin)) {
		//移動方向、移動先はプラスの座標へ
		selectRoot = 1;
	}
	//移動距離を収める
	int distance[2] = {
		(int)(this->m_RunnerDefault.ch.w * this->m_RunnerDefault.ch.scale) * 3 ,			//右方向
		(int)(this->m_RunnerDefault.ch.w * this->m_RunnerDefault.ch.scale) * -3			//左方向
	};
	//回避先までの移動距離を受け取る
	this->m_RunnerDefault.designateDistance = abs(distance[selectRoot]);
	//移動先座標を収める
	float moveEndX[2] = {
		this->m_RunnerDefault.ch.x + distance[selectRoot] ,					//移動先座標X
		this->m_RunnerDefault.ch.x + distance[selectRoot]					//移動先座標X
	};
	//移動量のデータテーブル
	float lineMove[2] = {
		RUNNER_DEFAULT_MOVE_RIGHT ,			//右移動
		RUNNER_DEFAULT_MOVE_LEFT  ,			//左移動
	};
	//移動先座標を特定する
	//自身の幅高さ分の距離を移動先とする
	this->m_RunnerDefault.ch.ex = moveEndX[selectRoot];
	//移動量を設定
	this->m_RunnerDefault.ch.mx = lineMove[selectRoot];

}


//*************************************************************************************************
//競争者の行動２の初期化「対象がハードルならジャンプして回避する」
//*************************************************************************************************
void   CCompetitor::SetCompetitorRunnerEvadeHardol(){

	
	//着地中ならジャンプ
	if( this->m_RunnerDefault.actionF == RUNNER_ACTION_ROUND ){

		//現在のアニメーションのコマ数をリセット
		this->m_RunnerDefault.ani.cnt = 0;

		//時間をリセット
		this->m_RunnerDefault.ani.time = 0;

		//最大コマ数をセット
		this->m_RunnerDefault.ani.max = RUNNER_ANIMATION_TYPE_JUMP_CNT_MAX;

		//フレーム数をセット
		this->m_RunnerDefault.ani.fram = RUNNER_ANIMATION_TYPE_JUMP_FRAM;


		//回避フラグをONにする
		this->m_RunnerDefault.avoidF |= RUNNER_WHILE_AVOID;

		//ジャンプフラグを立てる
		this->m_RunnerDefault.actionF |= RUNNER_ACTION_JUMP;

		//アニメーションタイプをジャンプ中に変化させる
		this->m_RunnerDefault.ani.type |= RUNNER_ANIMATION_JUMP;		//ジャンプのアニメーション
		
	
	}

}


//*************************************************************************************************
//競争者の行動３の初期化「対象を攻撃する」
//*************************************************************************************************
void   CCompetitor::SetCompetitorRunnerAttack(){

	
	//着地中なら攻撃
	if( this->m_RunnerDefault.actionF == RUNNER_ACTION_ROUND ){

		//現在のアニメーションのコマ数をリセット
		this->m_RunnerDefault.ani.cnt = 0;

		//時間をリセット
		this->m_RunnerDefault.ani.time = 0;

		//最大コマ数をセット
		this->m_RunnerDefault.ani.max = RUNNER_ANIMATION_TYPE_ATTACK_CNT_MAX;

		//フレーム数をセット
		this->m_RunnerDefault.ani.fram = RUNNER_ANIMATION_TYPE_ATTACK_FRAM;


		//攻撃フラグに
		this->m_RunnerDefault.actionF |= RUNNER_ACTION_ATTACK;

		//アニメーションタイプを攻撃の動作に
		this->m_RunnerDefault.ani.type |= RUNNER_ANIMATION_ATTACK;
	
	}

}


//*************************************************************************************************
//競争者の行動１「前方に障害物があれば横に移動」
//*************************************************************************************************
void   CCompetitor::UpdateCompetitorRunnerLineMove(){


	//移動先に達したら移動を止める
	if( this->m_RunnerDefault.avoidDistance >= this->m_RunnerDefault.designateDistance ){

		//移動量を０にする
		this->m_RunnerDefault.ch.mx = 0.0f;
			
		//横移動フラグをOFF
		this->m_RunnerDefault.actionF &= ~RUNNER_ACTION_LINE_MOVE;
		
		//回避行動フラグをOFFにする
		this->m_RunnerDefault.avoidF &= ~RUNNER_WHILE_AVOID;

		//取得した障害物の種類をリセット
		this->m_RunnerDefault.getType &= ~this->m_RunnerDefault.getType;

		//更新終了
		return;
	}

	//移動先に達っするまで移動を更新
	if( this->m_RunnerDefault.avoidDistance <= this->m_RunnerDefault.designateDistance ){

		//移動量を更新
		this->m_RunnerDefault.ch.x += this->m_RunnerDefault.ch.mx;

		//回避移動距離を更新
		this->m_RunnerDefault.avoidDistance += (int)( fabs( this->m_RunnerDefault.ch.mx ) );

	}

	//キャラクターの右端の座標を求める
	float rightPosX = this->m_RunnerDefault.ch.x + ( this->m_RunnerDefault.ch.w * this->m_RunnerDefault.ch.scale );

	//移動先が画面端を越えるなら移動量を反転させる
	//右端
	if( rightPosX >= STAGE_WIDTH ){

		//移動量を反転
		this->m_RunnerDefault.ch.mx = RUNNER_DEFAULT_MOVE_LEFT;
	}
	//左端
	if( this->m_RunnerDefault.ch.x <= 0 ){

		//移動量を反転
		this->m_RunnerDefault.ch.mx = RUNNER_DEFAULT_MOVE_RIGHT;
	}

}


//*************************************************************************************************
//競争者の行動２の更新「対象がハードルならジャンプして回避する」
//*************************************************************************************************
void   CCompetitor::UpdateCompetitorRunnerEvadeHardol(){
	
	//接地状態に戻っていたらジャンプフラグをなくす
	if( this->m_RunnerDefault.ani.type == RUNNER_ACTION_ROUND ){

		//フラグをOFF
		this->m_RunnerDefault.actionF &= ~RUNNER_ACTION_JUMP;

		//回避行動フラグをOFFにする
		this->m_RunnerDefault.avoidF &= ~RUNNER_WHILE_AVOID;

		//取得した障害物の種類をリセット
		this->m_RunnerDefault.getType &= ~this->m_RunnerDefault.getType;

	}

}


//*************************************************************************************************
//競争者の行動３の更新「対象を攻撃する」
//*************************************************************************************************
void   CCompetitor::UpdateCompetitorRunnerAttack(){

	//接地状態に戻っていたら攻撃フラグをなくす
	if( this->m_RunnerDefault.ani.type == RUNNER_ACTION_ROUND ){

		//攻撃フラグをOFF
		this->m_RunnerDefault.actionF &= ~RUNNER_ACTION_ATTACK;

		//取得した障害物の種類をリセット
		this->m_RunnerDefault.getType &= ~this->m_RunnerDefault.getType;
		
	}

}


//***********************************************************************************************
//引数のキャラクター構造体の索敵範囲を判定を取る
//***********************************************************************************************
bool	CCompetitor::BroadenCollisionSquareCompetitor( CHARACTER* pch ){


	//戻り値用
	bool ret = false;

	//引数のキャラクターの索敵範囲矩形と、自身の矩形で
	//判定を取る
	if( BroadenCollisionSquare( &this->m_RunnerDefault.ch , pch ) ){


		//衝突したことを返す
		ret = true;
	}

	//最終的な判定を返す
	return ret;

}


//************************************************************************************************
//競争者ランナーのアニメーションを更新
//************************************************************************************************
void   CCompetitor::ChangeCompetitorRunnerAnimation(){

	//画像データテーブル（Y座標で変化）
	char animationTbl[ RUNNER_ANIMATION_MAX ] = {

		RUNNER_ANIMATION_ROUND	,		//接地状態
		RUNNER_ANIMATION_JUMP	,		//ジャンプ中
		RUNNER_ANIMATION_ATTACK	,		//攻撃中
	
	};

	//現在のアニメーションのタイプ番号に応じて画像を変化させる
	for( int i = 0 ; i < RUNNER_ANIMATION_MAX ; i++ ){

		//アニメーションに応じて画像分岐
		if( animationTbl[i] == this->m_RunnerDefault.ani.type ){

			//表示元Y座標を反映する
			this->m_RunnerDefault.ch.direction = animationTbl[i];

			//▼ 競争者のアニメーション更新
			if( UpdateAnimation( &this->m_RunnerDefault.ch , &this->m_RunnerDefault.ani ) == false ){

				//現在のアニメーションのビット目を0にする
				this->m_RunnerDefault.ani.type &= ~animationTbl[i];
				//通常のアニメーションに変更
				this->m_RunnerDefault.ani.max = RUNNER_ANIMATION_TYPE_NORMAL_CNT_MAX;	//最大コマ数
				this->m_RunnerDefault.ani.fram = RUNNER_ANIMATION_TYPE_NORMAL_FRAM;		//フレーム数

			
			}

			break;

		}

	}

}

/*****************************************************************************************************
引数の値で確率で行動を決める
引数１：確率　1～100
引数２：行いたいアクションの種類番号

戻り値：アクションの種類番号
※失敗したら-1を返す
******************************************************************************************************/
char	CCompetitor::ConmpetitorAttemptDoAction( char likelihood , BYTE actionType ){
	
	//戻り値用
	char ret = COMPETITOR_RUNNER_ACTION_TYPE_VACANT;

	//引数の値で確率を計る
	if( GetLikelihood( likelihood ) ){
			
		//確率が的中したので引数の値のアクションの種類番号を返す
		ret = actionType;
	}
	
	return ret;

}



/*
//*************************************************************************************************
//競争者の行動２（回避行動）
//*************************************************************************************************
void   CCompetitor::UpdateCompetitorRunnerAvoid(){

	//衝突する面を扱う
	BYTE bumpAction = OBSTACLE_NOT_BUMP;		//何も当たってない状態	
	
	//障害物と仮の判定を取って、接触する面に応じて移動量を変化させる
	//衝突面を受け取る
	bumpAction = AttemptCollisionSquare( &this->m_RunnerDefault.ch );

	switch(bumpAction){
		
		case OBSTACLE_NOT_BUMP:

			break;

		case OBSTACLE_BUMP_TOP:

			this->m_RunnerDefault.ch.my = -5.0f;
			break;
			
		case OBSTACLE_BUMP_BOTTOM:

			this->m_RunnerDefault.ch.my = 5.0f;
			break;
			
		case OBSTACLE_BUMP_LEFT:

			this->m_RunnerDefault.ch.mx = -5.0f;
			break;
			
		case OBSTACLE_BUMP_RIGHT:

			this->m_RunnerDefault.ch.mx = 5.0f;
			break;
	}

}

*/
