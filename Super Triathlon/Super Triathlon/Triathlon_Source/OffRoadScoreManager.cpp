#include	"OffRoadScoreManager.h"
#include	"OffRoadScore.h"
#include	"graphics.h"
#include	"OffRoadRacer.h"
#include	"OffRoadAction.h"
#include	"input.h"
#include	<iostream>
#include	<stdio.h>

//名前空間
using namespace	std;


//▼ 静的メンバ変数の定義
const int COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_MAX = 2;		//画像使用枚数

//スコアのサイズ
const int COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_WIDTH = 160;	//幅
const int COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_HEIGHT = 50;	//高さ
//Chain
const int COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_CHAIN_WIDTH = 96;//Width
//EXCELLENTのサイズ
const int	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_EXCELLENT_WIDTH = 288;	//幅
//拡縮倍率
const float COffRoadScoreManager::OFFROADSCOREMANAGER_DEFAULT_SCORE_IMAGE_SCALE = 1.0f;	
//スコアの移動量
const float	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_MOVE_X = -5.0f;	//X
const float	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_MOVE_Y = 5.0f;	//Y
//スコアの待機時間
const int	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_STAY_TIME_MAX = 30;

//②アクションポイント表示用
//スコアのサイズ
const int COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_IMAGE_WIDTH = 40;	//幅
const int COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_IMAGE_HEIGHT = 50;	//高さ
//拡縮倍率
const float COffRoadScoreManager::OFFROADSCOREMANAGER_DEFAULT_SCORE_NUMBER_IMAGE_SCALE = 0.5f;
//スコアの種類数
const char	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_KIND_OF_TYPE = 1;
//演出したい数字スコアの総数
const char	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_DIRECTION_MAX = 1;
//加算・減算演出時のフレーム数
const int	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_DEFAULT_DIRECTION_INTERVAL_FRAM = 20;


//スコアの種類
//SUCCEED
const char	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_SUCCEED = 0;
//SCORE
const char	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_SCORE = 1;
//GOOD
const char	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_GOOD = 2;
//COOL
const char	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_COOL = 3;
//EXCELLENT
const char	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_EXCELLENT = 4;
//スコアの種類数
const char	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_KIND_OF_TYPE = 6;

//ウィンドウハンドル
HWND	COffRoadScoreManager::m_hWnd = NULL;


/*
************************************************************************************************
コンストラクタ
************************************************************************************************
*/
COffRoadScoreManager::COffRoadScoreManager(){

	//クラス
	this->m_lpOffRoadScoreImage = NULL;
	this->m_lpOffRoadScoreNumber = NULL;
	this->m_lpCOffRoadRacer = NULL;
	this->m_lpCOffRoadAction = NULL;
	//画像の要素
	this->m_lpOffRoadScoreImg = NULL;
	//数字画像演出用構造体
	this->m_lpScoreDirect = NULL;
	//Privateメンバ
	this->m_ScoreArriveCoordinate.x = 0.0f;
	this->m_ScoreArriveCoordinate.y = 0.0f;
	this->m_ScoreLeaveCoordinate.x = 0.0f;
	this->m_ScoreLeaveCoordinate.y = 0.0f;
	this->m_ScoreStayTime = 0;
	this->m_DisplayNumber = 0;
	this->m_CommandActionOld = OFFROADACTION_COMMAND_BIT_TYPE_VACANT;
	this->m_ValueTypeNext = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_SUCCEED;

}

/*
************************************************************************************************
ディストラクタ
************************************************************************************************
*/
COffRoadScoreManager::~COffRoadScoreManager(){
	
}

//************************************************************************************************
//スコアの初期化
//************************************************************************************************
void COffRoadScoreManager::InitScoreManager(){

	//▼ 読み込む画像データテーブル
	char	*lpImgTbl[ COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_MAX ] = {

		{ "Image\\OffRoad\\Number.bmp" } ,						//数字画像
		{ "Image\\OffRoad\\ValueImage.bmp" } ,					//Succeedの画像

	};

	//▼ 使用する画像のメモリを確保
	this->m_lpOffRoadScoreImg = new int [ COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_MAX ];

	//▼ 使用するクラスのメモリを確保
	//スコアイメージ
	this->m_lpOffRoadScoreImage = new COffRoadScore [ COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_KIND_OF_TYPE ];
	//数字スコア
	this->m_lpOffRoadScoreNumber = new COffRoadScore [ COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_KIND_OF_TYPE ];
	this->m_lpCOffRoadRacer = new COffRoadRacer;
	this->m_lpCOffRoadAction = new COffRoadAction;
	//数字画像演出用の構造体
	this->m_lpScoreDirect = new SCOREDIRECT[ COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_DIRECTION_MAX ];
	//0で初期化
	ZeroMemory( this->m_lpScoreDirect , sizeof( SCOREDIRECT ) * COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_DIRECTION_MAX );

	//▼ 画像設定
	//① 数字画像
	this->m_lpOffRoadScoreNumber[0].m_Isize.x = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_IMAGE_WIDTH;			//幅
	this->m_lpOffRoadScoreNumber[0].m_Isize.y = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_IMAGE_HEIGHT;			//高さ
	this->m_lpOffRoadScoreNumber[0].m_scale = COffRoadScoreManager::OFFROADSCOREMANAGER_DEFAULT_SCORE_NUMBER_IMAGE_SCALE;
	this->m_lpOffRoadScoreNumber[0].m_existFlag = true;
	//数字画像演出時の設定
	//加算・減算演出時の加算減算間隔
	this->m_lpScoreDirect[0].intervalFram = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_DEFAULT_DIRECTION_INTERVAL_FRAM;

	//アクションコマンドの枠の設定を取得
	//座標
	TPOINT<float> pos = this->m_lpCOffRoadAction->GetFramIndicatePosition();
	//サイズ
	TPOINT<int>	size = this->m_lpCOffRoadAction->GetFramSize();


	//各種イメージ画像の設定
	for( int i = 0 ; i < COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_KIND_OF_TYPE ; i++ ){
	
		this->m_lpOffRoadScoreImage[i].m_Fpos.x = pos.x + size.x;														//表示座標X
		this->m_lpOffRoadScoreImage[i].m_Fpos.y = pos.y - COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_HEIGHT;	//表示座標Y
		this->m_lpOffRoadScoreImage[i].m_Isize.x = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_WIDTH;			//幅
		this->m_lpOffRoadScoreImage[i].m_Isize.y = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_HEIGHT;		//高さ
		this->m_lpOffRoadScoreImage[i].m_scale = COffRoadScoreManager::OFFROADSCOREMANAGER_DEFAULT_SCORE_IMAGE_SCALE;	//拡縮倍率
		this->m_lpOffRoadScoreImage[i].m_Fmove.x = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_MOVE_X;				//移動量X
		this->m_lpOffRoadScoreImage[i].m_Fmove.y = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_MOVE_Y;				//移動量Y
		this->m_lpOffRoadScoreImage[i].m_Ipic.y = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_HEIGHT * i;		//表示元Y		
	}
	//SCORE画像の表示先座標を設定
	//プライヤーのアクションポイント座標を取得
	TPOINT<float> pointPos = this->m_lpCOffRoadRacer[0].GetActionPointIndicate();
	//数字画像の幅サイズ
	float numberSize = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_IMAGE_WIDTH * 
		COffRoadScoreManager::OFFROADSCOREMANAGER_DEFAULT_SCORE_NUMBER_IMAGE_SCALE;
	//SCORE画像の幅サイズ
	float scoreSize = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_WIDTH * 
		COffRoadScoreManager::OFFROADSCOREMANAGER_DEFAULT_SCORE_IMAGE_SCALE;
	//表示先座標の設定
	this->m_lpOffRoadScoreImage[1].m_Fpos.x = pointPos.x - ( scoreSize - numberSize ) / 2;				//X
	this->m_lpOffRoadScoreImage[1].m_Fpos.y = pointPos.y - 
	( this->m_lpOffRoadScoreImage[1].m_Isize.y * this->m_lpOffRoadScoreImage[1].m_scale );			//Y
	//表示フラグ
	this->m_lpOffRoadScoreImage[1].m_existFlag = true;

	//Set Chain image
	//Get player's position
	TPOINT<float> consecutivePos = this->m_lpCOffRoadRacer->GetOffRoadRacerIndicateConsecutiveAction();
	//substitute width for variable number
	this->m_lpOffRoadScoreImage[5].m_Isize.x = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_CHAIN_WIDTH;
	//Drawing position
	//position Y
	this->m_lpOffRoadScoreImage[5].m_Fpos.y = consecutivePos.y - ((COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_CHAIN_WIDTH *
		COffRoadScoreManager::OFFROADSCOREMANAGER_DEFAULT_SCORE_IMAGE_SCALE) / 2);
	//Chain's high size of width
	float chainWidth = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_CHAIN_WIDTH * COffRoadScoreManager::OFFROADSCOREMANAGER_DEFAULT_SCORE_IMAGE_SCALE;
	//position X
	this->m_lpOffRoadScoreImage[5].m_Fpos.x = consecutivePos.x - (chainWidth - numberSize) / 2;
	//Drawing flag
	this->m_lpOffRoadScoreImage[5].m_existFlag = true;

	//EXCELLENTの画像幅を代入
	this->m_lpOffRoadScoreImage[4].m_Isize.x = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_EXCELLENT_WIDTH;			//幅

	//表示出発座標
	this->m_ScoreLeaveCoordinate.x = pos.x + size.x;
	this->m_ScoreLeaveCoordinate.y = pos.y - COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_HEIGHT;
	//表示到着座標
	this->m_ScoreArriveCoordinate.x = pos.x;																//X
	this->m_ScoreArriveCoordinate.y = pos.y - COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_HEIGHT;	//Y

	//画像番号の設定
	for( int i = 0 ; i < COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_MAX ; i++ ){
		this->m_lpOffRoadScoreImg[i] = CreateDDImage( lpImgTbl[i] , COffRoadScoreManager::m_hWnd );
	}
	//各種イメージ画像の設定
	for( int i = 0 ; i < COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_KIND_OF_TYPE ; i++ ){
		this->m_lpOffRoadScoreImage[i].SetCharaImg( this->m_lpOffRoadScoreImg[1] );
	}

}

//************************************************************************************************
//スコアの更新
//************************************************************************************************
void COffRoadScoreManager::UpdateScoreManagerSucceedAction(){

	//アクション実行用フラグデータテーブル
	BYTE	doActionTbl[ OFFROADACTION_COMMAND_KIND_OF_ACTION ] = { 

		OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_HALF_ROTATE_ACTION	 ,		//上から時計回り180度回転のアクションフラグ
		OFFROADACTION_COMMAND_BIT_TYPE_LEFT_HALF_ROTATE_ACTION	 ,		//上から反時計回り180度回転のアクションフラグ
		OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_ROTATE_ACTION		 ,		//上から時計回りの入力時のアクションフラグ
		OFFROADACTION_COMMAND_BIT_TYPE_LEFT_ROTATE_ACTION		 ,		//上から反時計回りの入力時のアクションフラグ

	};

	//表示する評価画像のデータテーブル
	char	valueImageTbl[ COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_KIND_OF_TYPE - 1] = {

		//SUCCEED
		COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_SUCCEED	,
		//GOOD
		COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_GOOD		,
		//COOL
		COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_COOL		,
		//EXCELLENT
		COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_EXCELLENT	,

	};

	//コマンド生成中なら処理
	if( this->m_lpCOffRoadAction[0].GetCommandActionCretaeFlag() ){
		//評価画像の数だけループ
		for( int j = this->m_ValueTypeNext ; j < (COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_KIND_OF_TYPE - 1); j++ ){

			//SCOREの画像なら次の要素へ
			if( j == COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_SCORE ) continue;

			//未表示評価画像のみ表示
			if( this->m_lpOffRoadScoreImage[j].m_existFlag ) continue;

			//アクションコマンドの数だけループ
			for( int i = 0 ; i < OFFROADACTION_COMMAND_KIND_OF_ACTION ; i++ ){

				//コマンド入力に成功し、前回のコマンドアクションと異なれば評価画像を表示
				if( this->m_lpCOffRoadRacer[0].GetOffRoadRacerCommandInfo() == doActionTbl[i] && 
					this->m_CommandActionOld != doActionTbl[i] ){

					//今回のアクションビット目を取得
					this->m_CommandActionOld = doActionTbl[i];

					//次の評価画像の要素を取得
					this->m_ValueTypeNext = ( j + 1 );

					//表示フラグをtrueに
					this->m_lpOffRoadScoreImage[j].m_existFlag = true;
					break;
				}
			}
		}
	} else {
		//評価済み画像をリセット
		this->m_ValueTypeNext = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_SUCCEED;
		//今回のアクションビット目を取得
		this->m_CommandActionOld = OFFROADACTION_COMMAND_TYPE_VACANT;
	}

	//評価画像の数だけループ
	for (int i = 0; i < (COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_KIND_OF_TYPE - 1); i++) {

		//SCOREの画像なら次の要素へ
		if (i == COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_SCORE) continue;

		//表示フラグならスコアの移動
		if (this->m_lpOffRoadScoreImage[i].m_existFlag) {

			//表示先座標に到着するまで移動量を加算
			if (this->m_lpOffRoadScoreImage[i].m_Fpos.x >= this->m_ScoreArriveCoordinate.x) {
				//移動量を座標に加算
				this->m_lpOffRoadScoreImage[i].m_Fpos.x += this->m_lpOffRoadScoreImage[i].m_Fmove.x;		//X
			}
			//表示先座標に到着したら座標を初期値に戻す
			if (this->m_lpOffRoadScoreImage[i].m_Fpos.x <= this->m_ScoreArriveCoordinate.x) {

				//時間経過
				this->m_ScoreStayTime++;
			}

			//経過時間が設定した時間に達したら座標を元に戻して、表示フラグをfalse
			if (this->m_ScoreStayTime > COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_STAY_TIME_MAX) {
				//経過時間をリセット
				this->m_ScoreStayTime = 0;
				this->m_lpOffRoadScoreImage[i].m_Fpos.x = this->m_ScoreLeaveCoordinate.x;
				this->m_lpOffRoadScoreImage[i].m_existFlag = false;
			}
		}
	}

}


//************************************************************************************************
//スコアの表示
//************************************************************************************************
void COffRoadScoreManager::DrawScoreManager(){

	//各種イメージ画像の表示
	for( int i = 0 ; i < COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_KIND_OF_TYPE ; i++ ){

		//拡縮表示
		this->m_lpOffRoadScoreImage[i].DrawChara();
	}
}

/*
***************************************************************************************************
数字画像表示
***************************************************************************************************
*/
void	COffRoadScoreManager::DrawScoreNumber(TPOINT<float> pos, int number,
	int digit, tagSCOREMANAGERCOLORTYPE color, float addScale) {

	//表示する数値を受け取る
	int num = number;
	//表示する数値
	int rest = 0;
	//表示する数値の表示元
	int numD = 0;
	//桁カウント用
	int digitCnt = 0;

	//渡された値が負の値なら正の値にする
	if (num < 0) num *= -1;

	//表示したい値を受け取る
	this->m_lpScoreDirect[0].terminateNum = num;
	//▼ 加算減算演出
	num = this->GraduallyNumber(&this->m_lpScoreDirect[0]);

	//渡された引数の数値の各桁の数値を調べる
	do {

		//余りの数値が求める桁の数値
		rest = num % 10;
		//渡された値を毎回10で割っていく
		num /= 10;
		//表示する数の表示元を扱う
		numD = this->m_lpOffRoadScoreNumber[0].m_Isize.x * rest;

		//数値画像の表示
		DDScaleBlt(pos.x -
			(this->m_lpOffRoadScoreNumber[0].m_Isize.x * this->m_lpOffRoadScoreNumber[0].m_scale) * digitCnt,
			pos.y,
			this->m_lpOffRoadScoreNumber[0].m_Isize.x,
			this->m_lpOffRoadScoreNumber[0].m_Isize.y,
			this->m_lpOffRoadScoreImg[0],
			this->m_lpOffRoadScoreNumber[0].m_Ipic.x + numD,
			this->m_lpOffRoadScoreNumber[0].m_Ipic.y + (this->m_lpOffRoadScoreNumber[0].m_Isize.y * color),
			( this->m_lpOffRoadScoreNumber[0].m_scale + addScale) );

		//計算する毎にカウント
		digitCnt++;

		//渡された値が0未満になったら抜ける
	} while (num > 0 || digitCnt <= digit);

}


//***************************************************************************************************
//解放
//***************************************************************************************************
void COffRoadScoreManager::ReleaseScoreManager(){

	//画像の解放
	//未解放なら解放
	if( this->m_lpOffRoadScoreImg != NULL ){
		for( int i = 0 ; i < COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_MAX ; i++ ){
			ReleaseDDImage( this->m_lpOffRoadScoreImg[i] );
		}
	}
	
	//各種メモリの解放
	//クラス
	delete []this->m_lpOffRoadScoreImage;	//スコア
	this->m_lpOffRoadScoreImage = NULL;

	delete []this->m_lpOffRoadScoreNumber;	//数字スコア
	this->m_lpOffRoadScoreNumber = NULL;
	
	delete this->m_lpCOffRoadRacer;			//ライダー
	this->m_lpCOffRoadRacer = NULL;

	delete this->m_lpCOffRoadAction;		//アクション
	this->m_lpCOffRoadAction = NULL;

	//スコア演出用構造体
	delete []this->m_lpScoreDirect;
	this->m_lpScoreDirect = NULL;

	//画像要素
	delete []this->m_lpOffRoadScoreImg;
	this->m_lpOffRoadScoreImg = NULL;

}


/*
************************************************************************************************
目標の数値まで徐々に加算（減算）する
************************************************************************************************
*/
int COffRoadScoreManager::GraduallyNumber( SCOREDIRECT* pScore  ){

	//▼数値の上限値・下限値の設定
	//下限は0
	if( pScore->terminateNum < 0 ) pScore->terminateNum = 0;
	
	//上限は1億未満
	if( pScore->terminateNum > 100000000 ) pScore->terminateNum = 99999999;

	//▼表示する値と表示したい値が違ったら処理を行う
	//もしくは回転の処理がまだ続いていたら実行する
	if( pScore->terminateNum != pScore->indicateNum ){

		//表示する値が表示したい値より小さかったら増加する
		if( pScore->indicateNum <= pScore->terminateNum ){

			//表示する値と表示したい値の差分を求める
			pScore->restNum = pScore->terminateNum - pScore->indicateNum;

			//フレーム毎に加算する
			pScore->indicateNum += pScore->restNum / pScore->intervalFram;
			
			//表示したい値が表示する値から差分を引いた値付近になったら
			//表示したい値を代入する
			//もしくは、「Zキーか①ボタン」を押したら値を代入
			if( pScore->restNum <= pScore->intervalFram || JustKey('Z') || JustButton(BUTTON_1) ){

				//最終的に表示する値の代入
				pScore->indicateNum = pScore->terminateNum;
			}
		}

		//表示する値が表示したい値より大きかったら減少する
		if( pScore->indicateNum >= pScore->terminateNum ){

			//表示する値と表示したい値の差分を求める
			pScore->restNum = pScore->indicateNum - pScore->terminateNum;

			//フレーム毎に減算する
			pScore->indicateNum -= pScore->restNum / pScore->intervalFram;

			//表示したい値が表示する値から差分を引いた値付近になったら
			//表示したいを値を代入する
			if( pScore->restNum <= pScore->intervalFram || JustKey('Z') || JustButton(BUTTON_1) ){

				//最終的に表示する値の代入
				pScore->indicateNum = pScore->terminateNum;

			}
		}
	}

	//最後に表示する値を返す
	return pScore->indicateNum;

}