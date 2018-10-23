#include	"OffRoadRacer.h"
#include	"input.h"
#include	"gadget.h"
#include	"map.h"
#include	"OffRoadStage.h"
#include	"play.h"
#include	"sound.h"
#include	<stdio.h>
#include	<iostream>

//名前空間
using namespace std;

//▼ 静的メンバ変数の定義
const int COffRoadRacer::OFFROADRACER_IMAGE_MAX = 1;		//使用枚数
const int COffRoadRacer::OFFROADRACER_EFFECT_SOUND_MAX = 2;	//効果音
//ウィンドウハンドル
HWND	COffRoadRacer::m_hWnd = NULL;
//各種初期値
//表示座標（ローカル座標）
const float COffRoadRacer::OFFFROADRACER_DEFAULT_POSITION_X = 304.0f;			//表示座標X
const float COffRoadRacer::OFFFROADRACER_DEFAULT_POSITION_Y = 350.0f;			//表示座標Y
//移動量
const float COffRoadRacer::OFFFROADRACER_DEFAULT_MOVE_UP = -3.0f;		//前方
const float COffRoadRacer::OFFFROADRACER_DEFAULT_MOVE_RIGHT = 3.0f;		//右
const float COffRoadRacer::OFFFROADRACER_DEFAULT_MOVE_BACK = 3.0f;		//後方
const float COffRoadRacer::OFFFROADRACER_DEFAULT_MOVE_LEFT = -3.0f;		//左
//速度倍率
const float	COffRoadRacer::OFFROADRACER_DEFAULT_SPEED_RATE = 1.0f;
//速度合計
float COffRoadRacer::m_AggregateSpeed = 0.0f;
//コマンドアクション成功情報用
BYTE  COffRoadRacer::m_CommandSuccess = OFFROADACTION_COMMAND_BIT_TYPE_VACANT;
//画像サイズ
const int COffRoadRacer::OFFROADRACER_IMAGE_WIDTH = 32;		//幅
const int COffRoadRacer::OFFROADRACER_IMAGE_HEIGHT = 64;	//高さ
//拡縮倍率
const float COffRoadRacer::OFFFROADRACER_DEFAULT_SCALE = 1.0f;
//アクション時のプレイヤーの最大拡縮倍率
const float COffRoadRacer::OFFROADRACER_ACTION_SCALE_MAX = 5.0f;

//拡縮倍率の変数値
//加算値
const float	COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_ADD_EASY	= 0.01f;
const float	COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_ADD_NORMAL	= 0.02f;
const float	COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_ADD_HARD	= 0.03f;
//減算値
const float	COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_SUB_EASY	= 0.05f;
const float	COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_SUB_NORMAL	= 0.06f;
const float	COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_SUB_HARD	= 0.07f;
//アニメーション設定
//通常時
const int COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL_CNT_MAX	= 2;	//最大コマ数
const int COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL_FRAM	= 5;	//フレーム

//回転アニメーション時
const int COffRoadRacer::OFFROADRACER_ANIMATION_ROTATE_IMAGE_WIDTH  = 64;	//幅
const int COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_ROTATE_CNT_MAX = 19;	//最大コマ数
const int COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_ROTATE_FRAM	= 2;	//フレーム

//半回転アニメーション時
const int COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_CNT_MAX	= 5;	//最大コマ数
const int COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_FRAM		= 2;	//フレーム

//アニメーション時のビット目
//回転アニメーション
const BYTE COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL			= 0x00;	//通常時
const BYTE COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_ROTATE		= 0x01;	//右回転
const BYTE COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_ROTATE		= 0x02;	//左回転
const BYTE COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_HALF_ROTATE = 0x04;	//右半回転
const BYTE COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_HALF_ROTATE	= 0x08;	//左半回転
//アニメーションの種類
const int COffRoadRacer::OFFROADRACER_ANIMATION_KIND_OF_TYPE = 5;

//アニメーション時の表示元座標Y
const int COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL_PICY			= 0;	//通常時
const int COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_HALF_ROTATE_PICY = 1;	//右半回転
const int COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_HALF_ROTATE_PICY	= 2;	//左半回転
const int COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_ROTATE_PICY		= 1;	//右1回転
const int COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_ROTATE_PICY		= 2;	//左1回転

//アクションコマンド最大入力数
const int COffRoadRacer::OFFROADRACER_COMMAND_INPUT_MAX = 4;

//アクションポイント表示先座標
TPOINT<float>	COffRoadRacer::m_IndicateActionPoint = { 310.0f , 50.0f };
//合計アクションポイント
int		COffRoadRacer::m_ActionPoint = 0;
//アクションポイントの表示桁数
const int	COffRoadRacer::OFFROADRACER_ACTION_POINT_DIGIT = 0;

//ライダーの座標取得用
TPOINT<float>	COffRoadRacer::m_RiderPosition = {0};

//成功したアクションの成功数を収める
int		COffRoadRacer::m_SuccessActionCount[ OFFROADACTION_COMMAND_KIND_OF_TYPE ] = {0};
//連続でアクション成功した数を扱う
int		COffRoadRacer::m_SucceedActoinConsecutive = 0;
int		COffRoadRacer::m_SucceedActoinConsecutiveMax = 0;
//連続で成功したアクション数の表示先座標
TPOINT<float>	COffRoadRacer::m_IndicateActionConsecutive = { 460.0f , 50.0f };
//process for scale of number score image
//default add scale rate
const float COffRoadRacer::OFFROADRACER_CONSECUTIVE_ACTION_ADD_SCALE_RATE = 0.015f;
//add scale max
const float COffRoadRacer::OFFROADRACER_CONSECUTIVE_ACTION_ADD_SCALE_RATE_MAX = 0.3f;
//scale up consecutive success count
const int COffRoadRacer::OFFROADRACER_CONSECUTIVE_ACTION_SCALE_UP_NUMBER_SUCCESS_COUNT = 3;

//process for scale of total number score image
//default add scale rate
const float COffRoadRacer::OFFROADRACER_TOTAL_ACTION_POINT_ADD_SCALE_RATE = 0.015f;
//add scale max
const float COffRoadRacer::OFFROADRACER_TOTAL_ACTION_POINT_ADD_SCALE_RATE_MAX = 0.3f;
//starting point of scale up
const int COffRoadRacer::OFFROADRACER_TOTAL_ACTION_POINT_STARTING_POINT_SCALE_UP = 1000;
//limit time for scale up of score image
const int COffRoadRacer::OFFROADRACER_LIMIT_TIME_FOR_SCALE_UP_SCORE_IMAGE = 130;

//アクションコマンド成功時の速度変化値
const float COffRoadRacer::OFFROADRACER_SUCCEED_ACTION_VARIABLE_SPEED_RATE = 0.10f;

//アクション中かを扱う
bool COffRoadRacer::m_actionFlag = false;

//blank time that before player can input
const int COffRoadRacer::OFFROADRACER_BLANK_TIME_FOR_INPUT = 10;

/*
************************************************************************************************
ディストラクタ
************************************************************************************************
*/
COffRoadRacer::~COffRoadRacer(){
	
}

//************************************************************************************************
//ライダーの初期化
//************************************************************************************************
void COffRoadRacer::InitOffRoadRacer() {

	//難易度を取得
	tagGAMELEVEL	gameLevel = GetGameLevel();

	//ゲームレベル依存設定
	//拡縮倍率加算値
	float	scaleRateAddTbl[LEVEL_MAX] = {
		COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_ADD_EASY		,
		COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_ADD_NORMAL		,
		COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_ADD_HARD		,
	};
	//拡縮倍率減算値
	float	scaleRateSubTbl[LEVEL_MAX] = {
		COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_SUB_EASY		,
		COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_SUB_NORMAL		,
		COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_SUB_HARD		,
	};

	//▼ 読み込む画像のデータテーブル
	char	*imageTbl[COffRoadRacer::OFFROADRACER_IMAGE_MAX] = {

		{"Image\\OffRoad\\cyclist.bmp"} ,
	};
	//▼ 読み込む効果音のデータテーブル
	char	*lpSeTbl[COffRoadRacer::OFFROADRACER_EFFECT_SOUND_MAX] = {

		{"SE\\strongitem.wav"} ,
		{"SE\\jump.wav"} ,

	};

	//▼ 画像の設定
	//今回使用する画像の枚数分だけメモリを確保する
	this->m_lpCOffRoadRacerImg = new int[COffRoadRacer::OFFROADRACER_IMAGE_MAX];
	//効果音
	this->m_lpOffRoadRacerSe = new int[COffRoadRacer::OFFROADRACER_EFFECT_SOUND_MAX];
	//ステージのクラス
	this->m_lpCOffRoadStage = new COffRoadStage;
	//スコア管理クラス
	this->m_lpScoreManager = new COffRoadScoreManager[OFFROADRACER_KIND_OF_SCORE];
	//add scale rate to score image for recer's action
	this->m_lpScoreScaleAdd = new float[OFFROADRACER_KIND_OF_SCORE];
	ZeroMemory(this->m_lpScoreScaleAdd, sizeof(float) * OFFROADRACER_KIND_OF_SCORE);
	//count time to scale up of score image
	this->m_lpScoreScaleUpCountTime = new int[OFFROADRACER_KIND_OF_SCORE];
	ZeroMemory(this->m_lpScoreScaleUpCountTime, sizeof(int) * OFFROADRACER_KIND_OF_SCORE);
	//after acale up number
	this->m_lpNumberAfterScaleUp = new int[OFFROADRACER_KIND_OF_SCORE];
	ZeroMemory(this->m_lpNumberAfterScaleUp, sizeof(int) * OFFROADRACER_KIND_OF_SCORE);

	//アクションの成功数のリセット
	ZeroMemory(this->m_SuccessActionCount, sizeof(int) * OFFROADACTION_COMMAND_KIND_OF_ACTION);

	//合計アクションポイントをリセット
	this->m_ActionPoint = 0;

	//連続でアクション成功した数をリセット
	this->m_SucceedActoinConsecutive = 0;
	//連続でアクション成功した最大数をリセット
	this->m_SucceedActoinConsecutiveMax = 0;

	//▼ 表示設定
	//①ライダー
	this->m_OffRoadRacerCh.x = COffRoadRacer::OFFFROADRACER_DEFAULT_POSITION_X;										//表示座標X
	this->m_OffRoadRacerCh.y = this->m_lpCOffRoadStage[0].GetOffRoadStageDistance() -
		(this->m_lpCOffRoadStage[0].OFFROADSTAGE_SCREEN_HEIGHT - COffRoadRacer::OFFFROADRACER_DEFAULT_POSITION_Y);//表示座標Y
	this->m_OffRoadRacerCh.w = COffRoadRacer::OFFROADRACER_IMAGE_WIDTH;												//幅
	this->m_OffRoadRacerCh.h = COffRoadRacer::OFFROADRACER_IMAGE_HEIGHT;											//高さ
	this->m_OffRoadRacerCh.flag = true;																				//表示フラグ
	this->m_OffRoadRacerCh.my = COffRoadRacer::OFFFROADRACER_DEFAULT_MOVE_UP;										//移動量Y
	this->m_OffRoadRacerCh.speed = COffRoadRacer::OFFROADRACER_DEFAULT_SPEED_RATE;									//速度倍率
	this->m_AggregateSpeed = this->m_OffRoadRacerCh.my * this->m_OffRoadRacerCh.speed;								//速度合計値
	this->m_OffRoadRacerCh.scale = COffRoadRacer::OFFFROADRACER_DEFAULT_SCALE;

	//拡縮倍率変数値
	//加算値
	this->m_actionScaleAdd = scaleRateAddTbl[gameLevel];
	//減算値
	this->m_actionScaleSub = scaleRateSubTbl[gameLevel];

	//座標取得用
	this->m_RiderPosition.x = this->m_OffRoadRacerCh.x;			//X
	this->m_RiderPosition.y = this->m_OffRoadRacerCh.y;			//Y

	//▼ 画像の読み込み
	for (int i = 0; i < COffRoadRacer::OFFROADRACER_IMAGE_MAX; i++) {

		this->m_lpCOffRoadRacerImg[i] = CreateDDImage(imageTbl[i], COffRoadRacer::m_hWnd);
	}
	//▼ 効果音の読み込み
	for (int i = 0; i < COffRoadRacer::OFFROADRACER_EFFECT_SOUND_MAX; i++) {

		this->m_lpOffRoadRacerSe[i] = CreateSound(lpSeTbl[i]);
	}

	//カメラ座標の設定
	SetCamera(0.0f, this->m_OffRoadRacerCh.y - (this->m_OffRoadRacerCh.y - COffRoadRacer::OFFFROADRACER_DEFAULT_POSITION_Y));


	// ▼アニメーション設定
	this->m_OffRoadRacerAni.max = COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL_CNT_MAX;		//コマ数
	this->m_OffRoadRacerAni.fram = COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL_FRAM;			//フレーム
	this->m_OffRoadRacerAni.type = COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL;				//タイプ

	//アニメーション情報の設定
	SetAnimation(&this->m_OffRoadRacerCh, &this->m_OffRoadRacerAni, this->m_OffRoadRacerAni.sx, this->m_OffRoadRacerAni.sy,
		this->m_OffRoadRacerAni.max, this->m_OffRoadRacerAni.fram, this->m_OffRoadRacerAni.type, true);

	//アクションコマンド用
	this->m_CommandSuccess = OFFROADACTION_COMMAND_TYPE_VACANT;
	this->m_DidCommandAction = OFFROADACTION_COMMAND_TYPE_VACANT;

	//▼ アクションコマンドの初期化
	this->InitOffRoadAction();

	//▼ スコアの初期化
	for (int i = 0; i < OFFROADRACER_KIND_OF_SCORE; i++) { this->m_lpScoreManager[i].InitScoreManager(); }

}

//************************************************************************************************
//ライダーの更新
//************************************************************************************************
void COffRoadRacer::UpdateOffRoadRacer(){

	//カメラの座標を取得
	float pY = GetCameraY();

	//▼ ジャンプ地点との判定 
	bool jumpPoint = this->m_lpCOffRoadStage[0].CollisionJumpPoint( &this->m_OffRoadRacerCh );

	//ジャンプ音
	if( jumpPoint && this->m_actionFlag == false ) PlaySound( this->m_lpOffRoadRacerSe[1] , 1 , false );

	//▼ アクションコマンドの生成
	//ジャンプ地点と判定を取り、重なっていたら処理
	if (this->CreateActionCommand(jumpPoint)) {		//when indicate command, get flag of action

		//when count time more than default blank time, player can input
		if (this->m_OffRoadRacerCh.time > COffRoadRacer::OFFROADRACER_BLANK_TIME_FOR_INPUT) {
			this->m_actionFlag = true;
		}
		else {
			//cout time for player can input
			this->m_OffRoadRacerCh.time++;
		}
		//▼ アクションコマンドの入力状況
		//戻り値に対応したコマンドのビット目を受け取る
		if (this->m_actionFlag && this->GetCommandActionCretaeFlag()) {			//when player is doing action and draw command, execute proccess
			this->SetOffRoadRacerCommandInfo(this->InputActionCommand());
			//▼ アクションコマンドの更新
			if (this->UpdateOffRoadAction()) this->m_SucceedActoinConsecutive = 0;	//reset consecutive action point
		}
		//設定した倍率まで加算
		if( this->m_OffRoadRacerCh.scale < COffRoadRacer::OFFROADRACER_ACTION_SCALE_MAX ){
			//倍率に加算
			this->m_OffRoadRacerCh.scale += this->m_actionScaleAdd;
		}
	} 
	else 
	{ 	
		//▼ 移動
		this->MoveOffRoadRacer();
		//倍率を減算
		this->m_OffRoadRacerCh.scale -= this->m_actionScaleSub;
		//縮小処理は等倍まで
		if( this->m_OffRoadRacerCh.scale <= COffRoadRacer::OFFFROADRACER_DEFAULT_SCALE ){ 
			//アクションフラグ
			this->m_actionFlag = false;
			//成功したコマンドのビット目をリセット
			this->m_CommandSuccess = OFFROADACTION_COMMAND_BIT_TYPE_VACANT; 
			//プレイヤーを等倍にする
			this->m_OffRoadRacerCh.scale = COffRoadRacer::OFFFROADRACER_DEFAULT_SCALE;
			this->m_OffRoadRacerCh.time = 0;				//reset count time
		}
	}

	//▼ 移動の制限
	this->ConstrainOffRoadRacerMove();

	//速度合計の設定
	this->SetOffRoadRacerSpeed( (this->m_OffRoadRacerCh.my * this->m_OffRoadRacerCh.speed) - 
		( this->m_SucceedActoinConsecutive * COffRoadRacer::OFFROADRACER_SUCCEED_ACTION_VARIABLE_SPEED_RATE) );

	//移動量の更新
	this->m_OffRoadRacerCh.x += this->m_OffRoadRacerCh.mx;			//移動量X
	this->m_OffRoadRacerCh.y += this->m_AggregateSpeed;				//移動量Y

	//移動量のリセット
	this->m_OffRoadRacerCh.mx = 0.0f;								//移動量X

	//▼ ライダーの座標を取得用に代入
	this->m_RiderPosition.x = this->m_OffRoadRacerCh.x;
	this->m_RiderPosition.y = this->m_OffRoadRacerCh.y;

	//▼ キャラクターの移動量を速度メーターに反映させる
	SetStatus( this->m_AggregateSpeed );

	//▼ カメラ座標の設定
	SetCamera( 0.0f , this->m_OffRoadRacerCh.y - ( this->m_OffRoadRacerCh.y - pY ) );

	//▼ アクション設定
	//通常アニメーションで、実行したアクションコマンドのビット目で現在のビット目が違ったら、アクション設定
	if( this->m_OffRoadRacerAni.type == COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL &&
		this->m_DidCommandAction != this->m_CommandSuccess ){

		this->SetRotateActionOffRoadRacer();
	}

	//▼ アニメーション情報の更新
	this->ChangeAnimationOffRoadRacer();

	//▼ アクションの更新
	this->UpdateRotateActionOffRoadRacer();
	
	//▼ スコアの更新
	// Only Sccess Image
	this->m_lpScoreManager[0].UpdateScoreManagerSucceedAction();

	//Starting point of scale up for number image
	this->RacerStartingPointScaleUpScoreNumber();

}


//************************************************************************************************
//ライダーの表示
//************************************************************************************************
void COffRoadRacer::DrawOffRoadRacer() {

	//カメラの座標を取得
	float	pY = GetCameraY();

	//ライダーの表示
	if( this->m_OffRoadRacerCh.flag ){

		DDScaleBlt( 
			this->m_OffRoadRacerCh.x ,
			this->m_OffRoadRacerCh.y - pY ,
			this->m_OffRoadRacerCh.w ,
			this->m_OffRoadRacerCh.h ,
			this->m_lpCOffRoadRacerImg[0] ,
			this->m_OffRoadRacerCh.picx ,
			this->m_OffRoadRacerCh.picy ,
			this->m_OffRoadRacerCh.scale
			);
	}

	//コマンドの表示
	this->DrawOffRoadAction();

	//Draw Succeed image and consecutive action image
	for (int i = 0; i < OFFROADRACER_KIND_OF_SCORE; i++) {
		this->m_lpScoreManager[i].DrawScoreManager();
	}

	//獲得したアクションポイントの表示
	this->m_lpScoreManager[0].DrawScoreNumber( this->m_IndicateActionPoint , 
		this->m_ActionPoint , 
		COffRoadRacer::OFFROADRACER_ACTION_POINT_DIGIT , SCOREMANAGER_COLOR_BLACK , this->m_lpScoreScaleAdd[0] );

	//連続で成功したアクション数を表示
	this->m_lpScoreManager[1].DrawScoreNumber(this->m_IndicateActionConsecutive,
		this->m_SucceedActoinConsecutive,
		COffRoadRacer::OFFROADRACER_ACTION_POINT_DIGIT , SCOREMANAGER_COLOR_RED , this->m_lpScoreScaleAdd[1]);
	
}

//***************************************************************************************************
//解放
//***************************************************************************************************
void COffRoadRacer::ReleaseOffRoadRacer(){

	//画像
	//未解放なら解放処理
	if( this->m_lpCOffRoadRacerImg != NULL ){
		for( int i = 0 ; i < COffRoadRacer::OFFROADRACER_IMAGE_MAX ; i++ ){
			ReleaseDDImage( this->m_lpCOffRoadRacerImg[i] );

		}
	}
	delete []this->m_lpCOffRoadRacerImg;
	this->m_lpCOffRoadRacerImg = NULL;

	//効果音
	//未解放なら解放処理
	if( this->m_lpOffRoadRacerSe != NULL ){
		for( int i = 0 ; i < COffRoadRacer::OFFROADRACER_EFFECT_SOUND_MAX ; i++ ){
			ReleaseSound( this->m_lpOffRoadRacerSe[i] );

		}
	}
	delete []this->m_lpOffRoadRacerSe;
	this->m_lpOffRoadRacerSe = NULL;

	//スコアの解放
	for (int i = 0; i < OFFROADRACER_KIND_OF_SCORE; i++) { this->m_lpScoreManager[i].ReleaseScoreManager(); }
	
	//ステージクラスの解放
	delete this->m_lpCOffRoadStage;
	this->m_lpCOffRoadStage = NULL;

	//Delete ScoreManager class
	delete []this->m_lpScoreManager;
	this->m_lpScoreManager = NULL;

	//Delete add scale for score
	delete[]this->m_lpScoreScaleAdd;
	this->m_lpScoreScaleAdd = NULL;

	//delete count time to scale up of score image
	delete[]this->m_lpScoreScaleUpCountTime;
	this->m_lpScoreScaleUpCountTime = NULL;
	//after scale up number
	delete[]this->m_lpNumberAfterScaleUp;
	this->m_lpNumberAfterScaleUp = NULL;

	//コマンドの解放
	this->ReleaseOffRoadAction();
}


/*
************************************************************************************
操作
************************************************************************************
*/
void COffRoadRacer::MoveOffRoadRacer(){

	//▼ ライダーの操作
	//上入力
	if( KeepKey( VK_UP ) || KeepLever(LEVER_UP)){ this->m_OffRoadRacerCh.y += COffRoadRacer::OFFFROADRACER_DEFAULT_MOVE_UP; }
	//右入力
	if( KeepKey( VK_RIGHT ) || KeepLever(LEVER_RIGHT)){
		this->m_OffRoadRacerCh.mx = COffRoadRacer::OFFFROADRACER_DEFAULT_MOVE_RIGHT + 
			abs(this->m_SucceedActoinConsecutive * COffRoadRacer::OFFROADRACER_SUCCEED_ACTION_VARIABLE_SPEED_RATE);
	}
	//下入力
	if( KeepKey( VK_DOWN ) || KeepLever(LEVER_DOWN)){ this->m_OffRoadRacerCh.y += COffRoadRacer::OFFFROADRACER_DEFAULT_MOVE_BACK; }
	//左入力
	if( KeepKey( VK_LEFT ) || KeepLever(LEVER_LEFT)){
		this->m_OffRoadRacerCh.mx = COffRoadRacer::OFFFROADRACER_DEFAULT_MOVE_LEFT -
			abs(this->m_SucceedActoinConsecutive * COffRoadRacer::OFFROADRACER_SUCCEED_ACTION_VARIABLE_SPEED_RATE); 
	}

}


//************************************************************************************************
//移動範囲を制限する
//************************************************************************************************
void COffRoadRacer::ConstrainOffRoadRacerMove(){


	//カメラの座標を取得
	float comeCameraY = GetCameraY();

	//▼移動範囲を制限
	//画面左端
	if( this->m_OffRoadRacerCh.x < 0 ){
	
		this->m_OffRoadRacerCh.x = 0.0f;
	}

	//画面右端
	if( this->m_OffRoadRacerCh.x + ( this->m_OffRoadRacerCh.w * this->m_OffRoadRacerCh.scale ) >
		STAGE_WIDTH ){
		
		this->m_OffRoadRacerCh.x = STAGE_WIDTH - ( this->m_OffRoadRacerCh.w * this->m_OffRoadRacerCh.scale );
	}

	//画面上端
	if( this->m_OffRoadRacerCh.y < comeCameraY ){
		
		this->m_OffRoadRacerCh.y = comeCameraY;
	}

	//画面下端
	if( this->m_OffRoadRacerCh.y + ( this->m_OffRoadRacerCh.h * this->m_OffRoadRacerCh.scale ) > 
		comeCameraY + STAGE_HEIGHT ){
		
			this->m_OffRoadRacerCh.y = comeCameraY + STAGE_HEIGHT - 
			( this->m_OffRoadRacerCh.h * this->m_OffRoadRacerCh.scale );
	}

}

/*
************************************************************************************************
ライダーのアニメーション更新
************************************************************************************************
*/
void COffRoadRacer::ChangeAnimationOffRoadRacer(){

	//画像データテーブル（Y座標で変化）
	char animationTbl[ COffRoadRacer::OFFROADRACER_ANIMATION_KIND_OF_TYPE ] = {

		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL			,		//通常時
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_ROTATE		,		//右回転
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_ROTATE		,		//左回転
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_HALF_ROTATE,		//右半回転
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_HALF_ROTATE	,		//左半回転
	
	};

	//各画像の表示元座標を扱うデータテーブル
	int animationPicYTbl[ COffRoadRacer::OFFROADRACER_ANIMATION_KIND_OF_TYPE ] = {

		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL_PICY				,	//通常時
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_HALF_ROTATE_PICY	,	//右半回転
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_HALF_ROTATE_PICY	,	//左半回転
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_ROTATE_PICY		,	//右回転
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_ROTATE_PICY			,	//左回転

	};
	
	//アクション実行用フラグデータテーブル
	BYTE	doActionTbl[ OFFROADACTION_COMMAND_KIND_OF_ACTION ] = { 

		OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_HALF_ROTATE_ACTION	 ,		//上から時計回り180度回転のアクションフラグ
		OFFROADACTION_COMMAND_BIT_TYPE_LEFT_HALF_ROTATE_ACTION	 ,		//上から反時計回り180度回転のアクションフラグ
		OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_ROTATE_ACTION		 ,		//上から時計回りの入力時のアクションフラグ
		OFFROADACTION_COMMAND_BIT_TYPE_LEFT_ROTATE_ACTION		 ,		//上から反時計回りの入力時のアクションフラグ

	};

	//現在のアニメーションのタイプ番号に応じて画像を変化させる
	for( int i = 0 ; i < COffRoadRacer::OFFROADRACER_ANIMATION_KIND_OF_TYPE ; i++ ){

		//アニメーションに応じて画像分岐
		if( animationTbl[i] == this->m_OffRoadRacerAni.type ){

			//表示元Y座標を反映する
			this->m_OffRoadRacerCh.direction = animationPicYTbl[i];

			//▼ プレイヤーのアニメーション更新
			//通常時と１回転アクション
			if( this->m_OffRoadRacerAni.type == COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL || 
				this->m_OffRoadRacerAni.type == COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_ROTATE ||
				this->m_OffRoadRacerAni.type == COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_ROTATE ){

				//加算式アニメーション
				if( UpdateAnimation( &this->m_OffRoadRacerCh , &this->m_OffRoadRacerAni ) == false ){

					//現在のアニメーションのビット目を0にする
					this->m_OffRoadRacerAni.type &= ~animationTbl[i];
					break;
				}
			
			}
			//半回転アクション時のアニメーション更新
			if( this->m_OffRoadRacerAni.type == COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_HALF_ROTATE ||
				this->m_OffRoadRacerAni.type == COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_HALF_ROTATE ){
			
				//加算アニメーション後、減算でアニメーション
				if( UpdateReverseAnimation( &this->m_OffRoadRacerCh , &this->m_OffRoadRacerAni ) == false ){

					//現在のアニメーションのビット目を0にする
					this->m_OffRoadRacerAni.type &= ~animationTbl[i];
					break;
				}
			}
			
		}

	}

}

/*
************************************************************************************************************
ライダーの回転アクションの設定
*************************************************************************************************************
*/
void COffRoadRacer::SetRotateActionOffRoadRacer(){

	//回転アニメーションのデータテーブル
	BYTE	rotateAction[ OFFROADACTION_COMMAND_KIND_OF_ACTION ] = {

		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_HALF_ROTATE,		//右半回転
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_HALF_ROTATE	,		//左半回転
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_ROTATE		,		//右回転
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_ROTATE		,		//左回転
	
	};

	//アクション実行用フラグデータテーブル
	BYTE	doActionTbl[ OFFROADACTION_COMMAND_KIND_OF_ACTION ] = { 

		OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_HALF_ROTATE_ACTION	 ,		//上から時計回り180度回転のアクションフラグ
		OFFROADACTION_COMMAND_BIT_TYPE_LEFT_HALF_ROTATE_ACTION	 ,		//上から反時計回り180度回転のアクションフラグ
		OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_ROTATE_ACTION		 ,		//上から時計回りの入力時のアクションフラグ
		OFFROADACTION_COMMAND_BIT_TYPE_LEFT_ROTATE_ACTION		 ,		//上から反時計回りの入力時のアクションフラグ

	};

	//アニメーションコマ数
	int		animationCntTbl[ OFFROADACTION_COMMAND_KIND_OF_ACTION ] = {

		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_CNT_MAX  ,	//半回転
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_CNT_MAX  ,	//半回転
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_ROTATE_CNT_MAX		,	//1回転
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_ROTATE_CNT_MAX		,	//1回転
	};

	//フレーム数
	int		framTbl[ OFFROADACTION_COMMAND_KIND_OF_ACTION ] = {
		
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_FRAM		,	//半回転
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_FRAM		,	//半回転
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_ROTATE_FRAM			,	//1回転
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_ROTATE_FRAM			,	//1回転
	};

	//各コマンド成功時の得点
	int		actionPointTbl[ OFFROADACTION_COMMAND_KIND_OF_ACTION ] = {
	
		OFFROADACTION_COMMAND_TYPE_RIGHT_HALF_ROTATE_ACTION_POINT    ,		//上から時計回り180度回転のアクションフラグ
		OFFROADACTION_COMMAND_TYPE_LEFT_HALF_ROTATE_ACTION_POINT     ,		//上から反時計回り180度回転のアクションフラグ
		OFFROADACTION_COMMAND_TYPE_RIGHT_ROTATE_ACTION_POINT	     ,		//上から時計回りの入力時のアクションフラグ
		OFFROADACTION_COMMAND_TYPE_LEFT_ROTATE_ACTION_POINT			 ,		//上から反時計回りの入力時のアクションフラグ
	
	};

	//コマンド入力に成功
	for( int i = 0 ; i < OFFROADACTION_COMMAND_KIND_OF_ACTION ; i++ ){
		if( this->m_CommandSuccess == doActionTbl[i] ){

			//効果音
			PlaySound( this->m_lpOffRoadRacerSe[0] , 0 , false );

			//アクション成功数をカウント
			this->m_SuccessActionCount[i]++;

			//連続成功数カウント
			this->m_SucceedActoinConsecutive++;

			//連続で成功した最大値を更新
			if (this->m_SucceedActoinConsecutive > this->m_SucceedActoinConsecutiveMax) this->m_SucceedActoinConsecutiveMax = this->m_SucceedActoinConsecutive;

			//タイプ変更
			this->m_OffRoadRacerAni.type |= rotateAction[i];

			//画像の幅を変更
			this->m_OffRoadRacerCh.w = COffRoadRacer::OFFROADRACER_ANIMATION_ROTATE_IMAGE_WIDTH;

			//アニメのコマ数リセット
			this->m_OffRoadRacerAni.cnt= 0;
			//時間をリセット
			this->m_OffRoadRacerAni.time = 0;
			//最大コマ数を変更
			this->m_OffRoadRacerAni.max = animationCntTbl[i];
			//フレームを変更
			this->m_OffRoadRacerAni.fram = framTbl[i];
			
			//ポイント加算
			this->m_ActionPoint += actionPointTbl[i] + ( this->m_SucceedActoinConsecutive * OFFROADACTION_SUCCESS_ACTION_BONUS_POINT);
		
			break;

		}
	}

}

/*
************************************************************************************************************
ライダーの回転アクションの更新
*************************************************************************************************************
*/
void COffRoadRacer::UpdateRotateActionOffRoadRacer(){


	//アニメーションタイプが通常に戻っていたら
	if( this->m_OffRoadRacerAni.type == COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL ){

		//画像の幅を元に戻す
		this->m_OffRoadRacerCh.w = COffRoadRacer::OFFROADRACER_IMAGE_WIDTH;

		//現在実行したアクションコマンドのビット目を取得
		this->m_DidCommandAction = this->m_CommandSuccess;

		//アニメのコマ数リセット
		this->m_OffRoadRacerAni.cnt= 0;
		//時間をリセット
		this->m_OffRoadRacerAni.time = 0;
		//最大コマ数を変更
		this->m_OffRoadRacerAni.max = COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL_CNT_MAX;
		//フレームを変更
		this->m_OffRoadRacerAni.fram = COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL_FRAM;

		
	}

}


/*
*************************************************************************************************************
ライダーの入力情報取得用
*************************************************************************************************************
*/
void	COffRoadRacer::SetOffRoadRacerCommandInfo( BYTE CommandSuccess ){ 

	//引数のビット目がコマンド失敗なら何も処理をせずにreturn
	if (CommandSuccess == OFFROADACTION_COMMAND_BIT_TYPE_VACANT) return;
	
	//once success bonus point
	this->m_ActionPoint += (this->m_SucceedActoinConsecutive * OFFROADACTION_SUCCESS_ACTION_BONUS_POINT);
	
	//コマンドのビット目のデータテーブル
	BYTE	commandBitTbl[ OFFROADACTION_COMMAND_KIND_OF_ACTION ] = {

		OFFROADACTION_COMMAND_BIT_TYPE_RIGHT	,		//右
		OFFROADACTION_COMMAND_BIT_TYPE_LEFT		,		//左
		OFFROADACTION_COMMAND_BIT_TYPE_LEFT		,		//左
		OFFROADACTION_COMMAND_BIT_TYPE_RIGHT	,		//右
	};

	//アクション派生用フラグデータテーブル
	BYTE	technicalTbl[ OFFROADACTION_COMMAND_KIND_OF_ACTION ] = {
		
		OFFROADACTION_COMMAND_BIT_TYPE_UP					,		//上
		OFFROADACTION_COMMAND_BIT_TYPE_UP					,		//上
		OFFROADACTION_COMMAND_BIT_TYPE_UP_RIGHT_DOWN		,		//上入力、右入力と下入力
		OFFROADACTION_COMMAND_BIT_TYPE_UP_LEFT_DOWN			,		//上入力と左、下入力

	};

	//アクション発生用フラグ
	BYTE	doActionTbl[ OFFROADACTION_COMMAND_KIND_OF_ACTION ] = { 

		OFFROADACTION_COMMAND_BIT_TYPE_HALF_RIGHT_ROTATE	,		//上から時計回りに180度
		OFFROADACTION_COMMAND_BIT_TYPE_HALF_LEFT_ROTATE		,		//上から反時計回りに180度
		OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_ROTATE			,		//上から時計回りの入力
		OFFROADACTION_COMMAND_BIT_TYPE_LEFT_ROTATE			,		//上から反時計回りの入力
	};
	
	//現在のビット目と引数のビット目を調べる
	//引数のビット目
	for( int j = 0 ; j < OFFROADACTION_COMMAND_KIND_OF_ACTION ; j++ ){

		//引数のビット目がデータテーブルのj番目になければアクションは不成立で、次の要素へ
		if( CommandSuccess != commandBitTbl[j] ) continue; 
		
		//派生用ビット目
		for( int i = j ; i < OFFROADACTION_COMMAND_KIND_OF_ACTION ; i++ ){

			//現在のビット目と引数のビット目とで、対応した派生アクションを調べる
			if( this->m_CommandSuccess == technicalTbl[i] ){

				//発生用フラグを代入
				this->m_CommandSuccess |= doActionTbl[i];
				this->m_CommandSuccess |= CommandSuccess;
				return;
			}
		}
	}

	//現在のアクションが半回転で、次のビット目が右か左なら反対のビット目を消す
	//右半回転 ⇒ 左半回転アクション
	if( OFFROADACTION_COMMAND_BIT_TYPE_LEFT == CommandSuccess &&
		OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_HALF_ROTATE_ACTION == this->m_CommandSuccess ){

			//左半回転発生のために、現在の入力情報をリセット
			this->m_CommandSuccess = OFFROADACTION_COMMAND_BIT_TYPE_VACANT;
			//実行アクション用ビット目を代入
			this->m_CommandSuccess |= OFFROADACTION_COMMAND_BIT_TYPE_LEFT_HALF_ROTATE_ACTION;
			return;
	}
	//左半回転 ⇒ 右半回転アクション
	if( OFFROADACTION_COMMAND_BIT_TYPE_RIGHT == CommandSuccess &&
		OFFROADACTION_COMMAND_BIT_TYPE_LEFT_HALF_ROTATE_ACTION == this->m_CommandSuccess ){

			//左半回転発生のために、現在の入力情報をリセット
			this->m_CommandSuccess = OFFROADACTION_COMMAND_BIT_TYPE_VACANT;
			//実行アクション用ビット目を代入
			this->m_CommandSuccess |= OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_HALF_ROTATE_ACTION;
			return;
	}
	//１回転アクションならアクション実行ビット目をリセット
	if( this->m_CommandSuccess == OFFROADACTION_COMMAND_BIT_TYPE_LEFT_ROTATE_ACTION || 
		this->m_CommandSuccess == OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_ROTATE_ACTION ){
	
		//コマンドをリセット
		this->m_CommandSuccess = OFFROADACTION_COMMAND_BIT_TYPE_VACANT;
	}
	//最後に引数の値を代入
	this->m_CommandSuccess |= CommandSuccess;

}


/*
Starting point of scale up for number image
*/
void	COffRoadRacer::RacerStartingPointScaleUpScoreNumber() {

	//starting point of scale up
	int	startingpoint[OFFROADRACER_KIND_OF_SCORE] = {
		COffRoadRacer::OFFROADRACER_TOTAL_ACTION_POINT_STARTING_POINT_SCALE_UP,
		COffRoadRacer::OFFROADRACER_CONSECUTIVE_ACTION_SCALE_UP_NUMBER_SUCCESS_COUNT,
	};
	//each other action point
	int actionpoint[OFFROADRACER_KIND_OF_SCORE] = {
		this->m_ActionPoint,
		this->m_SucceedActoinConsecutive,
	};
	//add scale
	float addscale[OFFROADRACER_KIND_OF_SCORE] = {
		COffRoadRacer::OFFROADRACER_TOTAL_ACTION_POINT_ADD_SCALE_RATE,
		COffRoadRacer::OFFROADRACER_CONSECUTIVE_ACTION_ADD_SCALE_RATE,
	};
	//limit add scale
	float limitAddscale[OFFROADRACER_KIND_OF_SCORE] = {
		COffRoadRacer::OFFROADRACER_TOTAL_ACTION_POINT_ADD_SCALE_RATE_MAX,
		COffRoadRacer::OFFROADRACER_CONSECUTIVE_ACTION_ADD_SCALE_RATE_MAX,
	};


	//Process for scale of number score image
	//loop by racer's kind of score
	for (int i = 0; i < OFFROADRACER_KIND_OF_SCORE; i++) {
		// subsutitute consecutive action for local variable
		int consecutive = actionpoint[i];
		// if consecutive is zero during scale up process, initialize global scale rate and reset count time
		// then continue
		if (consecutive == 0 && this->m_lpScoreScaleAdd[i] > 0) {
			this->m_lpScoreScaleAdd[i] = 0; this->m_lpScoreScaleUpCountTime[i] = 0; continue;
		}
		//count time more than limit time
		if (this->m_lpScoreScaleUpCountTime[i] >= COffRoadRacer::OFFROADRACER_LIMIT_TIME_FOR_SCALE_UP_SCORE_IMAGE) {
			//subtract add scale rate from global variable
			this->m_lpScoreScaleAdd[i] -= addscale[i];
			//limit to subtract scale rate
			if (this->m_lpScoreScaleAdd[i] <= 0.0f) {
				//subsutitute scale up number for
				this->m_lpNumberAfterScaleUp[i] = consecutive;
				this->m_lpScoreScaleAdd[i] = 0.0f;			//reset scale
				this->m_lpScoreScaleUpCountTime[i] = 0;		//reset time
			}
			continue;										//don't need to add scale rate because came to limit scale rate
		}
		//case after scale up number to process
		if ((this->m_lpNumberAfterScaleUp[i] + startingpoint[i]) <= consecutive) {
			//limit time for scale up process
			if (this->m_lpScoreScaleAdd[i] == limitAddscale[i]) {
				//count time for scale up
				this->m_lpScoreScaleUpCountTime[i]++;
				//count time less than limit time
				if (this->m_lpScoreScaleUpCountTime[i] < COffRoadRacer::OFFROADRACER_LIMIT_TIME_FOR_SCALE_UP_SCORE_IMAGE)  continue;
			}
			//rest for total point
			int rest = 0;
			//case total point round off fractions less than one thousand
			if (i == 0) rest = actionpoint[0] % startingpoint[0]; consecutive -= rest;
			//scale up process
			if (consecutive % startingpoint[i] == 0 &&
				consecutive >= startingpoint[i]) {
				//add scale rate to global variabal
				this->m_lpScoreScaleAdd[i] += addscale[i];
			}
			//limit scale
			if (this->m_lpScoreScaleAdd[i] >= limitAddscale[i]) {
				//subsutitute scale max for
				this->m_lpScoreScaleAdd[i] = limitAddscale[i];
			}
		}
	}
}