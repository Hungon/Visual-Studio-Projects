#include	"CRunner.h"
#include	<iostream>

using namespace std;


/*
************************************************************************************
コンストラクタ
************************************************************************************
*/
CRunner::CRunner(){

	//▼各走者の共通した初期化
	//0の要素の初期化
	ZeroMemory( &this->m_RunnerDefault , sizeof( RUNNER ) );

	this->m_RunnerDefault.ch.w = RUNNER_DEFAULT_IMAGE_WIDTH;					//幅
	this->m_RunnerDefault.ch.h = RUNNER_DEFAULT_IMAGE_HEIGHT;					//高さ
	this->m_RunnerDefault.ch.my = RUNNER_DEFAULT_MOVE_Y;						//移動量Y
	this->m_RunnerDefault.ch.speed = RUNNER_DEFAULT_SPEED_RATE;					//速度倍率
	this->m_RunnerDefault.aggregateSpeed = this->m_RunnerDefault.ch.speed * 
		this->m_RunnerDefault.ch.my;											//速度合計
	this->m_RunnerDefault.ch.defaultSpeed = RUNNER_DEFAULT_SPEED_RATE;			//速度倍率初期値
	this->m_RunnerDefault.avoidtime = RUNNER_DEFAULT_AVOID_TIME;				//回避時間
	this->m_RunnerDefault.speed_max = RUNNER_DEFAULT_SPEEDMAX_RATE;				//速度倍率の最大値
	this->m_RunnerDefault.speed_min = RUNNER_DEFAULT_SPEEDMIN_RATE;				//速度倍率の最小値
	this->m_RunnerDefault.speedChargeMax = RUNNER_DEFAULT_CHARGEMAX_RATE;		//速度チャージ値の最大値
	this->m_RunnerDefault.ch.scale = RUNNER_DEFAULT_IMAGE_SCALE;				//拡縮倍率
	this->m_RunnerDefault.ch.defaultScale = RUNNER_DEFAULT_IMAGE_SCALE;			//拡縮倍率初期値
	this->m_RunnerDefault.ch.SearchSquareFront = RUNNER_DEFAULT_SEARCH_FRONT;	//前方に広げる矩形範囲
	this->m_RunnerDefault.ch.SearchSquareWidth = RUNNER_DEFAULT_SEARCH_WIDE;	//広げる矩形範囲の幅
	this->m_RunnerDefault.bumpType = RUNNER_NOT_BUMP;							//衝突フラグ
	this->m_RunnerDefault.obstacleBumpNow = -1;									//障害物の要素
	this->m_RunnerDefault.effectType = RUNNER_TYPE_NORMAL;						//エフェクトタイプ
	//アニメーション設定
	this->m_RunnerDefault.ani.max = RUNNER_ANIMATION_TYPE_NORMAL_CNT_MAX;					//最大コマ数
	this->m_RunnerDefault.ani.fram = RUNNER_ANIMATION_TYPE_NORMAL_FRAM;						//フレーム数

	//▼影の設定
	//0で初期化
	ZeroMemory( &this->m_ShadowDefault , sizeof( RUNNER ) );
	this->m_ShadowDefault.ch.w = 64;											//幅
	this->m_ShadowDefault.ch.h = 64;											//高さ
	this->m_ShadowDefault.ch.alpha = 120.0f;									//透過率
}

/*
************************************************************************************
ディストラクタ
************************************************************************************
*/
CRunner::~CRunner(){

}

/*************************************************************************************************
ハードルを飛び越えた際のランナーの強化効果１「速度上昇」
**************************************************************************************************
*/
void   CRunner::RunnerEffectSpeedUp(){

	//現在の効果タイプ
	this->m_RunnerDefault.effectType |= RUNNER_EFFECT_TYPE_SPEED_UP;

	//加速
	this->m_RunnerDefault.ch.speed += 0.1f;

	//速度の上限
	if( this->m_RunnerDefault.ch.speed >= this->m_RunnerDefault.speed_max ){
		this->m_RunnerDefault.ch.speed = this->m_RunnerDefault.speed_max;
	}

	cout << "SpeedUp" << endl;

}


/*************************************************************************************************
ハードルを飛び越えた際のランナーの強化効果２「無敵状態」
**************************************************************************************************
*/
void   CRunner::RunnerEffectAbsolute(){

	//現在の効果タイプ
	this->m_RunnerDefault.effectType |= RUNNER_EFFECT_TYPE_ABSOLUTE;
	
	//時間を代入
	this->m_RunnerDefault.absoluteTime = RUNNER_ABSOLUTE_EFFECT_TIME;

	cout << "Absolute" << endl;

}




/*************************************************************************************************
障害物にぶつかった際に効果中の強化を無効にする
速度上昇を無効
**************************************************************************************************
*/
void   CRunner::RepealRunnerEffectSpeedUp(){

	//タイプをなくす
	this->m_RunnerDefault.effectType &= ~RUNNER_EFFECT_TYPE_SPEED_UP;
	
	//速度を元に戻す
	this->m_RunnerDefault.ch.speed = RUNNER_DEFAULT_SPEED_RATE;

}


/*************************************************************************************************
時間経過で無敵状態を無効にする
**************************************************************************************************
*/
bool	CRunner::RepealRunnerEffectAbsolute() {

	//戻り値用
	bool ret = false;

	//無敵中、もしくは速度上昇＆無敵中なら時間を測る
	if (this->m_RunnerDefault.effectType == RUNNER_EFFECT_TYPE_ABSOLUTE ||
		this->m_RunnerDefault.effectType == RUNNER_EFFECT_TYPE_SPEED_AND_ABSOLUTE) {
		//時間経過
		this->m_RunnerDefault.absoluteTime--;
		ret = true;
	}
	//設定した時間経過後、無敵状態を無効に
	if (this->m_RunnerDefault.absoluteTime < 0) {
		//タイプをなくす
		this->m_RunnerDefault.effectType &= ~RUNNER_EFFECT_TYPE_ABSOLUTE;
		ret = false;
	}
	return ret;
}


/*
************************************************************************************************
当たり判定　CollisionRunner
ランナー前方の障害物との判定を取る際に、Y座標の食い込みを発生させて判定を取る
食い込ませるY座標はランナーの高さの半分のサイズ
************************************************************************************************
*/ 
bool	CRunner::CollisionRunner( CHARACTER* pch1 , CHARACTER* pch2 ){

	//ランナー前方の判定をを取る際の、Y座標の食い込みを扱う
	float attempt = (float)( pch1->h * pch1->scale ) / 2;

	//2つの重なりを調べる時に 、画像の矩形よりも、-方向のY座標を少し喰い込ませて判定を取る
	if( pch1->x < pch2->x + (float)pch2->w * pch2->scale &&
	    pch1->x + (float)pch1->w * pch1->scale > pch2->x &&
 	    ( pch1->y + attempt ) < pch2->y + (float)pch2->h  * pch2->scale &&
	    pch1->y + (float)pch1->h  * pch1->scale > pch2->y ){		
		//重なっていたらtrueを返す
		return true;
	}
	//重なっていなかったらfalseを返す
	return false;
}
