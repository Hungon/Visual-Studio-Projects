#include	"CPlayerRunner.h"
#include	"CompetitorRunner.h"
#include	"RunnerEffect.h"
#include	"input.h"
#include	"sound.h"
#include	"map.h"
#include	"gadget.h"
#include	"wipe.h"
#include	"CEffectManager.h"
#include	"CRoadEnemy.h"
#include	"CRoadEnemyManager.h"
#include	"CRoadStage.h"
#include	<stdio.h>
#include	<iostream>
#include	<math.h>

using namespace std;


//▼ 静的メンバ変数の再定義
const HWND	CPlayerRunner::m_hWnd = NULL;
//画像
const int CPlayerRunner::PLAYER_RUNNER_IMAGE_MAX = 3;			//使用枚数
//音
const int CPlayerRunner::PLAYER_RUNNER_EFFECT_SOUND_MAX = 8;	//使用数
//ステータス情報
//体力
const int CPlayerRunner::PLAYER_RUNNER_STAMINA_MAX = 3;	

//情報取得用
RUNNER	CPlayerRunner::m_GetRunner = { 0 };


/*
************************************************************************************************
コンストラクタ
************************************************************************************************
*/
CPlayerRunner::CPlayerRunner(){}


/*
************************************************************************************************
ディストラクタ
************************************************************************************************
*/
CPlayerRunner::~CPlayerRunner(){

}


//************************************************************************************************
//ステージ１番「陸上ステージ」での初期化
//************************************************************************************************
void   CPlayerRunner::InitRunner( int runnerNum , CRoadEnemyManager	*lpRoadEnemy , float roadDistance ){

	//▼ 画像データ
	char	*lpImageTbl[ CPlayerRunner::PLAYER_RUNNER_IMAGE_MAX ] = {
		
		{ "Image\\Road\\PlayerRunner.bmp" }	,		//player's own
		{ "Image\\Road\\shadow.bmp" }	,			//player's shadow
		{ "Image\\Road\\stamina.bmp" }	,			//player's stamina
	};
	//▼ 効果音データ
	char	*lpSoundTbl[ CPlayerRunner::PLAYER_RUNNER_EFFECT_SOUND_MAX ] = {

		{ "SE\\speedcharge.wav" }		,			//speed up
		{ "SE\\chargedown.wav" }		,			//speed down
		{ "SE\\attack.wav" }		,				//attack
		{ "SE\\jump.wav"},							//jump
		{ "SE\\strongitem.wav"},					//when player got an invincible body
		{ "SE\\weakitem.wav"},						//when time out that is invincible body
		{ "SE\\Healing.wav"},						//healing body
	};


	//各種メモリの確保
	this->m_lpPlayerRunnerImg = new int [ CPlayerRunner::PLAYER_RUNNER_IMAGE_MAX ];					//画像
	this->m_lpPlayerRunnerEffectSound = new int [ CPlayerRunner::PLAYER_RUNNER_EFFECT_SOUND_MAX ];	//効果音
	this->m_lpPlayerStatus = new RUNNER [ CPlayerRunner::PLAYER_RUNNER_STAMINA_MAX ];				//体力

	//障害物のアドレスを取得
	this->m_lpCRoadEnemyForPlayer = lpRoadEnemy;

	//▼ 表示の設定

	//各値を代入する
	this->m_RunnerDefault.ch.x = (float)( CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH( this->m_RunnerDefault.ch.w ) );	//表示先X
	this->m_RunnerDefault.ch.y = roadDistance - CAMERA_FOCUS_POSITION_Y;				//表示先Y
	this->m_RunnerDefault.pos_x = this->m_RunnerDefault.ch.x;											//表示位置X
	this->m_RunnerDefault.pos_y = STAGE_HEIGHT - CAMERA_FOCUS_POSITION_Y;								//表示位置Y
	this->m_RunnerDefault.ch.flag = true;																//生存フラグ
	this->m_RunnerDefault.ch.hp = RUNNER_DEFAULT_STAMINA_MAX;											//体力

	//記録用
	this->m_RunnerDefault.jumpedHurdleChain = 0;				//ハードルを連続で飛び越えた数							
	this->m_RunnerDefault.jumpedHurdleChainMax = 0;				//連続で飛び越えた最高記録
	
	//記録用
	//飛び越えたハードルの数
	for( int i = 0 ; i < ROADENEMY_OBSTACLE_HURDLE_KIND_OF_COLOR ; i++ ){ this->m_RunnerDefault.jumpedHurdleCnt[i] = 0; }

	//アニメーション情報の設定
	SetAnimation( &this->m_RunnerDefault.ch , &this->m_RunnerDefault.ani ,
				this->m_RunnerDefault.ani.sx , this->m_RunnerDefault.ani.sy ,
				this->m_RunnerDefault.ani.max , this->m_RunnerDefault.ani.fram , 
				this->m_RunnerDefault.ani.type , true);


	//②ステータス情報の設定
	//体力
	//1個目
	this->m_lpPlayerStatus[0].ch.x = 30.0f;							//表示先X座標
	this->m_lpPlayerStatus[0].ch.y = 120.0f;						//表示先Y座標
	this->m_lpPlayerStatus[0].ch.w = 32;							//幅
	this->m_lpPlayerStatus[0].ch.h = 32;							//高さ
	this->m_lpPlayerStatus[0].ch.flag = true;						//表示フラグ
	this->m_lpPlayerStatus[0].ch.picx = 0;							//表示元X座標
	this->m_lpPlayerStatus[0].ch.picy = 0;							//表示元Y座標
	this->m_lpPlayerStatus[0].ch.scale = 1.0f;						//拡縮率
	//2個目
	this->m_lpPlayerStatus[1] = this->m_lpPlayerStatus[0];
	this->m_lpPlayerStatus[1].ch.x += this->m_lpPlayerStatus[1].ch.w;		//表示先X座標
	//3個目
	this->m_lpPlayerStatus[2] = this->m_lpPlayerStatus[1];
	this->m_lpPlayerStatus[2].ch.x += this->m_lpPlayerStatus[2].ch.w;		//表示先X座標

	//画像の要素を生成
	for( int i = 0 ; i < CPlayerRunner::PLAYER_RUNNER_IMAGE_MAX ; i++ ){
		this->m_lpPlayerRunnerImg[i] = CreateDDImage( lpImageTbl[i] , CPlayerRunner::m_hWnd );
	}
	//効果音の要素を生成
	for( int i = 0 ; i < CPlayerRunner::PLAYER_RUNNER_EFFECT_SOUND_MAX ; i++ ){
		this->m_lpPlayerRunnerEffectSound[i] = CreateSound( lpSoundTbl[i] );
	}

	//取得用に情報を設定する
	this->m_GetRunner = this->m_RunnerDefault;

	//initialize score setting
	ZeroMemory(&this->m_PlayerScore, sizeof(SCOREVALUE));
	this->m_PlayerScore.score_x = 450.0f;					
	this->m_PlayerScore.score_y = 50.0f;					
	this->m_PlayerScore.score_scale = 0.5f;				
	this->m_PlayerScore.dicimals_flag = false;				
	this->m_PlayerScore.ch.type = SCORE_NUMBER_TYPE_NORMAL;	
	this->m_PlayerScore.scoreColor = SCORE_BLACK;			
	this->m_PlayerScore.digit = 3;							
	this->m_PlayerScore.ch.flag = true;
}


//************************************************************************************************
//「ランナー」の更新
//************************************************************************************************
void   CPlayerRunner::UpdateRunner( int runner , CEffectManager *lpCEffect ){


	//衝突時に取得する効果データテーブル
	BYTE bumpTbl[RUNNER_EFFECT_KIND_OF_TYPE] = {

		RUNNER_EFFECT_FLAG_ABSOLUTE				,		//無敵状態
		RUNNER_EFFECT_FLAG_SPEED_UP 			,		//速度上昇
		RUNNER_EFFECT_FLAG_HEALING				,		//回復
	
	};

	//効果データテーブル
	BYTE effectTbl[RUNNER_EFFECT_KIND_OF_TYPE] = {

		RUNNER_EFFECT_TYPE_ABSOLUTE			,		//無敵状態
		RUNNER_EFFECT_TYPE_SPEED_UP			,		//速度上昇
		RUNNER_EFFECT_TYPE_HEALING			,		//回復
	};

	//カメラの座標を取得
	float comeCameraY = GetCameraY();
		
	//各種アクション
	if (this->m_RunnerDefault.ani.type == RUNNER_ANIMATION_ROUND) {
		//▼ ジャンプの更新
		this->UpdateActionPlayerRunnerJump();
		//▼ 攻撃の更新
		this->UpdateActionPlayerRunnerAttack();
	}

	//▼　移動の更新
	this->UpdatePlayerRunnerMove();
	
	//▼　速度の更新
	//通常時のみ
	if( this->m_RunnerDefault.ani.type == RUNNER_ANIMATION_ROUND ){
		this->UpdatePlayerRunnerSpeed();
	}
	//▼キャラクターの移動量を速度メーターに反映させる
	SetStatus( this->m_RunnerDefault.aggregateSpeed );	

	//▼ 障害物を索敵しタイプ番号を取得する
	//取得する種類番号は障害物のビット目
	this->m_RunnerDefault.getType = this->m_lpCRoadEnemyForPlayer->BroadenCollisionSquareToObstacle( &this->m_RunnerDefault.ch );
	
	//▼ 障害物との当たり判定
	//生存フラグがtrueなら
	if( this->m_RunnerDefault.ch.flag ){
		//障害物と判定を取ってフラグを受け取る
		this->m_RunnerDefault.bumpType = this->m_lpCRoadEnemyForPlayer->CollisionRoadEnemyManager( &this->m_RunnerDefault );
		//▼ 強化効果の分岐
		//通常の衝突、衝突していないなら処理をしない
		if (this->m_RunnerDefault.bumpType != RUNNER_NOT_BUMP ||
			this->m_RunnerDefault.bumpType != RUNNNER_BUMP_TO_HURDLE ||
			this->m_RunnerDefault.bumpType != RUNNNER_BUMP_TO_ROCK) {
			//タイプの数だけ回す
			for (int i = 0; i < RUNNER_EFFECT_KIND_OF_TYPE; i++) {
				//取得した衝突フラグの番号を元に分岐
				if (this->m_RunnerDefault.bumpType == bumpTbl[i]) {
					//▼ エフェクトの生成
					lpCEffect->CreateEffectManaager(&this->m_RunnerDefault, runner);
					//▼ 記録用
					//飛び越えたハードルの数をカウント
					this->m_RunnerDefault.jumpedHurdleChain++;
					//for draw
					this->m_PlayerScore.num = this->m_RunnerDefault.jumpedHurdleChain;
					//タイプ毎にカウントする
					this->m_RunnerDefault.jumpedHurdleCnt[this->m_RunnerDefault.bumpType - 1]++;
					//飛び越えた数が最高記録を超えたら更新する
					if (this->m_RunnerDefault.jumpedHurdleChain > this->m_RunnerDefault.jumpedHurdleChainMax) {
						//記録を塗り替える
						this->m_RunnerDefault.jumpedHurdleChainMax = this->m_RunnerDefault.jumpedHurdleChain;
					}
					//▼ 効果の分岐
					switch (i) {
					case 0:
						CRunner::RunnerEffectAbsolute();
						PlaySound(this->m_lpPlayerRunnerEffectSound[4], 0, false);		//invincible sound
						break;
					case 1:
						CRunner::RunnerEffectSpeedUp();
						PlaySound(this->m_lpPlayerRunnerEffectSound[0], 0, false);		//speed up sound
						break;
					case 2:
						this->RunnerEffectHealing();
						PlaySound(this->m_lpPlayerRunnerEffectSound[6], 0, false);		//Healing soung
						break;
					}
					//衝突フラグをなくす
					this->m_RunnerDefault.bumpType = RUNNER_NOT_BUMP;
					break;
				}
			}
		}
		//▼ 普通に衝突したら処理
		if( this->m_RunnerDefault.bumpType == RUNNNER_BUMP_TO_HURDLE ||
			this->m_RunnerDefault.bumpType == RUNNNER_BUMP_TO_ROCK){
			PlaySound( this->m_lpPlayerRunnerEffectSound[2] , 0 , false );			//beating sound
			if (fabs(this->m_RunnerDefault.aggregateSpeed) > fabs((RUNNER_DEFAULT_SPEED_RATE * RUNNER_DEFAULT_MOVE_Y))) {
				PlaySound(this->m_lpPlayerRunnerEffectSound[1], 0, false);			//speed down sound
			}
			//reset consecutive to chain of jump
			this->m_RunnerDefault.jumpedHurdleChain = 0;
			//for draw
			this->m_PlayerScore.num = this->m_RunnerDefault.jumpedHurdleChain;
			//▼ 速度上昇効果を無効に
			CRunner::RepealRunnerEffectSpeedUp();
			//エフェクトを非表示
			lpCEffect->RepealEffectManager( 0 , runner );
			//生存フラグ
			this->m_RunnerDefault.ch.flag = false;
			//体力を減算
			this->m_RunnerDefault.ch.hp--;
			//現在の体力がステータスの要素数外なら処理を行わない
			if( this->m_RunnerDefault.ch.hp < PLAYER_RUNNER_STAMINA_MAX &&
				this->m_RunnerDefault.ch.hp >= 0 ){
				//現在の体力に応じて体力の画像表示元座標を変更する
				this->m_lpPlayerStatus[this->m_RunnerDefault.ch.hp].ch.picx = this->m_lpPlayerStatus[0].ch.w;
				//衝突フラグをなくす
				this->m_RunnerDefault.bumpType = RUNNER_NOT_BUMP;
			}
		}
	}

	//▼ 無敵中の処理
	//無敵中なら時間を測り、一定の時間になったら通常の無敵状態を無効に
	if (this->m_RunnerDefault.effectType == RUNNER_EFFECT_TYPE_ABSOLUTE ||
		this->m_RunnerDefault.effectType == RUNNER_EFFECT_TYPE_SPEED_AND_ABSOLUTE) {
		if (CRunner::RepealRunnerEffectAbsolute() == false) {
			PlaySound(this->m_lpPlayerRunnerEffectSound[5], 0, false);			//into ordinary
			//エフェクト非表示に
			lpCEffect->RepealEffectManager(1, runner);
		}
	}
	//▼　体力が0になったらゲームオーバーへ遷移
	if( this->m_RunnerDefault.ch.hp <= 0 ) CreateWipe( SCENE_GAMEOVER , CHANGE_SCENE , WIPE_CONTRACTION );

	//▼ エフェクトの更新
	lpCEffect->UpdateEffectManager( &this->m_RunnerDefault , runner );

	//プレイヤーのローカル座標を更新
	this->m_RunnerDefault.pos_y = this->m_RunnerDefault.ch.y - comeCameraY;
	
	//▼ カメラの焦点をプレイヤー座標で更新されるカメラ座標に設定する
	SetCamera( 0 , this->m_RunnerDefault.ch.y - this->m_RunnerDefault.pos_y  ); 

	//▼ 敵回避処理
	AvoidEnemy( &this->m_RunnerDefault.ch , this->m_RunnerDefault.avoidtime );

	//▼　手前に来るほどに大きく見せる処理
	FittingPositionToBackCameraAngle( &this->m_RunnerDefault.ch , 380.0f / 640.0f );

	//▼ プレイヤーアニメーションの分岐
	this->ChangePlayerRunnerAnimation();

	//▼ 移動範囲を制限
	this->ConstrainPlayerRunnerMove();

	//取得用に情報を設定する
	this->m_GetRunner = this->m_RunnerDefault;

}

//************************************************************************************************
//表示を行う関数
//************************************************************************************************
void   CPlayerRunner::DrawRunner(){

	//カメラ座標を取得
	float comeCameraX = GetCameraX();		//X
	float comeCameraY = GetCameraY();		//Y

	//プレイヤーランナーの表示
	if( this->m_RunnerDefault.ch.time % 2 == 0 ){
		//影
		DDAlphaBlt(
			(int)( ( this->m_RunnerDefault.ch.x - 5.0f ) - comeCameraX ) ,
			(int)( ( this->m_RunnerDefault.ch.y + this->m_RunnerDefault.ch.h - 
			( this->m_ShadowDefault.ch.h / 2 ) ) - comeCameraY ) ,
			this->m_ShadowDefault.ch.w ,
			this->m_ShadowDefault.ch.h ,
			this->m_lpPlayerRunnerImg[1] ,
			this->m_ShadowDefault.ch.picx ,
			this->m_ShadowDefault.ch.picy ,
			this->m_ShadowDefault.ch.alpha );

		//プレイヤー
		DDScaleBlt(
			this->m_RunnerDefault.ch.x  - comeCameraX ,
			this->m_RunnerDefault.ch.y  - comeCameraY ,
			this->m_RunnerDefault.ch.w ,
			this->m_RunnerDefault.ch.h ,
			this->m_lpPlayerRunnerImg[0] ,
			this->m_RunnerDefault.ch.picx ,
			this->m_RunnerDefault.ch.picy ,
			this->m_RunnerDefault.ch.scale );
	}

	//プレイヤーランナーの体力を表示
	//3個表示
	for( int i = 0 ; i < CPlayerRunner::PLAYER_RUNNER_STAMINA_MAX ; i++ ){
		if( this->m_lpPlayerStatus[i].ch.flag ){
			//「ハート画像」
			DDScaleBlt(
				this->m_lpPlayerStatus[i].ch.x ,
				this->m_lpPlayerStatus[i].ch.y ,
				this->m_lpPlayerStatus[i].ch.w ,
				this->m_lpPlayerStatus[i].ch.h ,
				this->m_lpPlayerRunnerImg[2] ,
				this->m_lpPlayerStatus[i].ch.picx ,
				this->m_lpPlayerStatus[i].ch.picy ,
				this->m_lpPlayerStatus[i].ch.scale );
		}
	}
	//draw chain's score
	if (this->m_PlayerScore.ch.flag) DrawScore(&this->m_PlayerScore);
	//draw score's valueation
	DrawValueation(this->m_PlayerScore.score_x +
		(this->m_PlayerScore.score_scale * SCORE_NUMBER_AND_PUNCTUAL_IMAGE_WIDTH),
		this->m_PlayerScore.score_y,
		this->m_RunnerDefault.jumpedHurdleChain);
}


//************************************************************************************************
//解放を行う関数
//************************************************************************************************
void   CPlayerRunner::ReleaseRunner(){

	//画像の解放
	if( this->m_lpPlayerRunnerImg != NULL ){
		for( int i = 0 ; i < PLAYER_RUNNER_IMAGE_MAX ; i++ ){
			ReleaseDDImage( this->m_lpPlayerRunnerImg[i] );
		}
	}
	//効果音の解放
	if( this->m_lpPlayerRunnerEffectSound != NULL ){
		for( int i = 0 ; i < PLAYER_RUNNER_EFFECT_SOUND_MAX ; i++ ){
			ReleaseSound( this->m_lpPlayerRunnerEffectSound[i] );
		}
	}
	
	//確保したメモリの解放
	delete []this->m_lpPlayerRunnerImg;			//画像
	this->m_lpPlayerRunnerImg = NULL;
	delete []this->m_lpPlayerRunnerEffectSound;	//効果音
	this->m_lpPlayerRunnerEffectSound = NULL;

	//メモリの解放
	if( this->m_lpPlayerStatus != NULL ){
		delete []this->m_lpPlayerStatus;			//ステータス
		this->m_lpPlayerStatus = NULL;
	}



}

//************************************************************************************************
//「ランナーの移動」の更新
//************************************************************************************************
void   CPlayerRunner::UpdatePlayerRunnerMove(){

	//▼操作
	//右移動
	if( KeepKey( VK_RIGHT ) || KeepLever(LEVER_RIGHT)) this->m_RunnerDefault.ch.mx = RUNNER_DEFAULT_MOVE_RIGHT;
	//左移動
	if( KeepKey( VK_LEFT ) || KeepLever(LEVER_LEFT)) this->m_RunnerDefault.ch.mx = RUNNER_DEFAULT_MOVE_LEFT;
	//生存フラグがtrueなら移動可能
	if( this->m_RunnerDefault.ch.flag ){
		//上移動
		if( KeepKey( VK_UP ) || KeepLever(LEVER_UP)) this->m_RunnerDefault.ch.y += RUNNER_DEFAULT_MOVE_FRONT;
	}
	//下移動
	if( KeepKey( VK_DOWN ) || KeepLever(LEVER_DOWN)) this->m_RunnerDefault.ch.y += RUNNER_DEFAULT_MOVE_BACK;

	//速度の合計値を更新
	this->m_RunnerDefault.aggregateSpeed = this->m_RunnerDefault.ch.my * this->m_RunnerDefault.ch.speed;

	//移動量を加算
	this->m_RunnerDefault.ch.y += this->m_RunnerDefault.aggregateSpeed;
	//X移動量
	this->m_RunnerDefault.ch.x += this->m_RunnerDefault.ch.mx;

	//移動量をリセット
	this->m_RunnerDefault.ch.mx = 0.0f;							//X
	this->m_RunnerDefault.ch.my = RUNNER_DEFAULT_MOVE_Y;		//Y


}


//************************************************************************************************
//「ランナーの速度」の更新
//************************************************************************************************
void   CPlayerRunner::UpdatePlayerRunnerSpeed(){


	//速度に準じたタイプ分け
	tagRUNNERANIMATIONSPEED speedType[4] = {

		RUNNER_SPEED_SLOW	,		//遅い
		RUNNER_SPEED_NORMAL ,		//普通
		RUNNER_SPEED_DECENT ,		//少し速い
		RUNNER_SPEED_RAPID	,		//速い	
	
	};

	//▼速度レベルを更新
	//高い順から調べて現在速度が上回ったらループを抜けて、アニメーションを更新
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


//************************************************************************************************
//プレイヤーランナーのジャンプ処理の更新
//************************************************************************************************
void   CPlayerRunner::UpdateActionPlayerRunnerJump() {


	//着地中「Wキー or Bボタン」でジャンプ
	if (JustKey('W') || JustButton(BUTTON_2)) {
		PlaySound(this->m_lpPlayerRunnerEffectSound[3], 0, false);		//jump sound
		//現在のアニメーションのコマ数をリセット
		this->m_RunnerDefault.ani.cnt = 0;
		//時間をリセット
		this->m_RunnerDefault.ani.time = 0;
		//最大コマ数をセット
		this->m_RunnerDefault.ani.max = RUNNER_ANIMATION_TYPE_JUMP_CNT_MAX;
		//フレーム数をセット
		this->m_RunnerDefault.ani.fram = RUNNER_ANIMATION_TYPE_JUMP_FRAM;
		//ジャンプフラグを立てる
		this->m_RunnerDefault.actionF |= RUNNER_ACTION_JUMP;
		//アニメーションタイプをジャンプ中に変化させる
		this->m_RunnerDefault.ani.type |= RUNNER_ANIMATION_JUMP;		//ジャンプのアニメーション
	}
	//接地状態に戻っていたらジャンプフラグをなくす
	if (this->m_RunnerDefault.ani.type == RUNNER_ACTION_ROUND) {
		//フラグをOFF
		this->m_RunnerDefault.actionF &= ~RUNNER_ACTION_JUMP;
	}

}


//************************************************************************************************
//プレイヤーランナーの攻撃処理の更新
//************************************************************************************************
void   CPlayerRunner::UpdateActionPlayerRunnerAttack() {

	//着地中、かつ、Aキー or Aボタンで攻撃
	if (JustKey('A') || JustButton(BUTTON_1)) {
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
	//接地状態に戻っていたら攻撃フラグをなくす
	if (this->m_RunnerDefault.ani.type == RUNNER_ACTION_ROUND) {
		//フラグをOFF
		this->m_RunnerDefault.actionF &= ~RUNNER_ACTION_ATTACK;
	}

}


//************************************************************************************************
//プレイヤーランナーのアニメーションを更新
//************************************************************************************************
void   CPlayerRunner::ChangePlayerRunnerAnimation(){

	//画像データテーブル（Y座標で変化）
	char animationTbl[RUNNER_ANIMATION_MAX] = {

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

			//▼ プレイヤーのアニメーション更新
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


//************************************************************************************************
//移動範囲を制限する
//************************************************************************************************
void   CPlayerRunner::ConstrainPlayerRunnerMove() {


	//カメラ座標を取得
	float comeCameraY = GetCameraY();

	//▼移動範囲を制限
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

	//画面上端
	if (this->m_RunnerDefault.ch.y < comeCameraY) this->m_RunnerDefault.ch.y = comeCameraY;
	//画面下端
	if (this->m_RunnerDefault.ch.y + (this->m_RunnerDefault.ch.h * this->m_RunnerDefault.ch.scale) >
		comeCameraY + STAGE_HEIGHT) {
		this->m_RunnerDefault.ch.y = comeCameraY + STAGE_HEIGHT -
			(this->m_RunnerDefault.ch.h * this->m_RunnerDefault.ch.scale);
	}

}



//***********************************************************************************************
//引数のキャラクター構造体の索敵範囲を判定を取る
//***********************************************************************************************
bool	CPlayerRunner::BroadenCollisionSquarePlayer( CHARACTER* pch ){


	//戻り値用
	bool ret = false;

	//引数のキャラクターの索敵範囲矩形と、自身の矩形で
	//判定を取る
	if( BroadenCollisionSquare( &this->m_RunnerDefault.ch , pch ) ) ret = true;

	//最終的な判定を返す
	return ret;

}

/*************************************************************************************************
ハードルを飛び越えた際のランナーの強化効果３「回復」
**************************************************************************************************
*/
void   CPlayerRunner::RunnerEffectHealing(){

	//配列外ならreturn
	if( this->m_RunnerDefault.ch.hp >= RUNNER_DEFAULT_STAMINA_MAX ) return;

	//現在の効果タイプ
	this->m_RunnerDefault.effectType |= RUNNER_EFFECT_TYPE_HEALING;

	//現在の体力に応じて体力の画像表示元座標を変更する
	this->m_lpPlayerStatus[this->m_RunnerDefault.ch.hp].ch.picx = 0;

	//体力を１個回復
	this->m_RunnerDefault.ch.hp++;

	//体力の上限値
	if( this->m_RunnerDefault.ch.hp > RUNNER_DEFAULT_STAMINA_MAX ) this->m_RunnerDefault.ch.hp = RUNNER_DEFAULT_STAMINA_MAX;

	//現在の効果タイプを無効
	this->m_RunnerDefault.effectType &= ~RUNNER_EFFECT_TYPE_HEALING;

	cout << "Heal" << endl;


}
