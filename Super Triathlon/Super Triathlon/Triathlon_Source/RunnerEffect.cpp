#include	"RunnerEffect.h"
#include	"map.h"
#include	"CPlayerRunner.h"

// ▼静的メンバの再定義
//エフェクトタイプ毎の画像設定
//リング型
const int CRunnerEffect::RUNNER_EFFECT_TYPE_RING_IMAGE_HEIGHT	= 96;					//高さ
const int CRunnerEffect::RUNNER_EFFECT_TYPE_RING_IMAGE_WIDTH	= 96;					//幅
const float CRunnerEffect::RUNNER_EFFECT_TYPE_RING_IMAGE_SCALE	= 1.0F;					//拡大率
const float CRunnerEffect::RUNNER_EFFECT_TYPE_RING_IMAGE_CONTRACTION_SPEED = -0.05f;	//拡大率の速度

																			//ボール型
const int CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_IMAGE_HEIGHT	= 32;		//高さ
const int CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_IMAGE_WIDTH	= 32;		//幅
const float CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_IMAGE_SCALE	= 1.0F;		//拡大率
//アニメーション設定
const int CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_ANIMATION_MAX = 4;			//最大コマ数
const int CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_ANIMATION_FRAM = 5;		//フレーム数

//************************************************************************************************
//コンストラクタ
//************************************************************************************************
CRunnerEffect::CRunnerEffect(){


}

//************************************************************************************************
//ディストラクタ
//************************************************************************************************
CRunnerEffect::~CRunnerEffect(){

	//使用したメモリの解放
	delete []this->m_lpEffectAni;
	this->m_lpEffectAni = NULL;
	
	delete []this->m_lpPowerAni;
	this->m_lpPowerAni = NULL;
	
	delete []this->m_lpEffectCh;
	this->m_lpEffectCh = NULL;
	
	delete []this->m_lpPowerCh;
	this->m_lpPowerCh = NULL;

	delete []this->m_lpRunnerEffectImg;
	this->m_lpRunnerEffectImg = NULL;

	delete this->m_lpCPlayer;
	this->m_lpCPlayer = NULL;

}



/************************************************************************************************
プレイヤーエフェクトの初期化
*************************************************************************************************
*/
void CRunnerEffect::InitRunnerEffect( int effectMax ){

	//引数の最大数だけメモリの確保
	//① 効果発生時の画像
	this->m_lpEffectAni = new ANIMATION [ effectMax ];		//アニメーション
	this->m_lpEffectCh = new CHARACTER [ effectMax ];		//画像設定用

	//② 効果中の画像は種類の数だけ確保する
	this->m_lpPowerAni = new ANIMATION[ RUNNER_EFFECT_SIGN_KIND_OF_TYPE ];
	this->m_lpPowerCh = new CHARACTER[ RUNNER_EFFECT_SIGN_KIND_OF_TYPE ];

	//allow plaayer class
	this->m_lpCPlayer = new CPlayerRunner;

	//発生させるエフェクトの最大数を代入
	this->m_CreateEffectMax = effectMax;
			
	//▼ 画像設定
	for( int i = 0 ; i < this->m_CreateEffectMax ; i++ ){
	
		//▼ 効果発生時のエフェクト
		this->m_lpEffectCh[i].x = 0.0f;			//表示元座標X
		this->m_lpEffectCh[i].y = 0.0f;			//表示元座標Y
		this->m_lpEffectCh[i].w = CRunnerEffect::RUNNER_EFFECT_TYPE_RING_IMAGE_WIDTH;			//幅
		this->m_lpEffectCh[i].h = CRunnerEffect::RUNNER_EFFECT_TYPE_RING_IMAGE_HEIGHT;			//高さ
		this->m_lpEffectCh[i].picx = 0;			//表示元X
		this->m_lpEffectCh[i].picy = 0;			//表示元Y
		this->m_lpEffectCh[i].flag = false;		//表示フラグ
		this->m_lpEffectCh[i].scale = CRunnerEffect::RUNNER_EFFECT_TYPE_RING_IMAGE_SCALE;		//拡縮率

	}
		
	//▼ 効果中のエフェクト
	for( int i = 0 ; i < RUNNER_EFFECT_SIGN_KIND_OF_TYPE ; i++ ){
	
		this->m_lpPowerCh[i].x = 0.0f;			//表示元座標X
		this->m_lpPowerCh[i].y = 0.0f;			//表示元座標Y
		this->m_lpPowerCh[i].w = CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_IMAGE_WIDTH;			//幅
		this->m_lpPowerCh[i].h = CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_IMAGE_HEIGHT;			//高さ
		this->m_lpPowerCh[i].picx = 0;			//表示元X
		this->m_lpPowerCh[i].picy = 0;			//表示元Y
		this->m_lpPowerCh[i].flag = false;		//表示フラグ
		this->m_lpPowerCh[i].scale = CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_IMAGE_SCALE;		//拡縮率

		//アニメーション設定
		this->m_lpPowerAni[i].cnt = 0;			//コマ数
		this->m_lpPowerAni[i].time = 0;			//時間
		this->m_lpPowerAni[i].sx = 0;			//表示元X
		this->m_lpPowerAni[i].sy = 0;			//表示元Y
		this->m_lpPowerAni[i].max = CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_ANIMATION_MAX;		//コマの最大枚数
		this->m_lpPowerAni[i].fram = CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_ANIMATION_FRAM;		//コマのフレーム数
	}

}

/*
***********************************************************************************************
エフェクト生成
*************************************************************************************************
*/
void CRunnerEffect::CreateRunnerEffect( float x , float y , char type ){

	//種類をまとめたデータテーブル
	tagRUNNEREFFECTKIND effectTypeTbl[ RUNNER_EFFECT_SIGN_KIND_OF_TYPE ] = {
		RUNNER_EFFECT_TYPE_BLUE		 ,
		RUNNER_EFFECT_TYPE_RED		 ,
	};

	//出現させるエフェクトの数だけ回す
	for( int i = 0 ; i < this->m_CreateEffectMax ; i++ ){

		//生存フラグがfalseなら
		if( this->m_lpEffectCh[i].flag == false ){

			//引数のタイプを生成
			this->m_lpEffectCh[i].x = x;				//X座標
			this->m_lpEffectCh[i].y = y;				//Y座標
			this->m_lpEffectCh[i].type = type;			//種類番号を代入
			this->m_lpEffectCh[i].flag = true;			//生存フラグ
			this->m_lpEffectCh[i].scale = CRunnerEffect::RUNNER_EFFECT_TYPE_RING_IMAGE_SCALE;	//拡縮率
			this->m_lpEffectCh[i].picy = type * CRunnerEffect::RUNNER_EFFECT_TYPE_RING_IMAGE_HEIGHT;

			//回復の要素ならここでreturnする
			if( type == RUNNER_EFFECT_TYPE_GREEN ) return;
			
			//効果中画像の設定
			for( int j = 0 ; j < RUNNER_EFFECT_SIGN_KIND_OF_TYPE ; j++ ){

				//生存フラグがfalseかつ
				//取得したタイプ番号なら
				if( this->m_lpPowerCh[j].flag == false &&
					this->m_lpEffectCh[i].type == effectTypeTbl[j] ){

					//引数のタイプを生成
					this->m_lpPowerCh[j].x = x + CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_IMAGE_WIDTH;	//X座標
					this->m_lpPowerCh[j].y = y + ( effectTypeTbl[j] * 
						CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_IMAGE_HEIGHT);							//Y座標
					this->m_lpPowerCh[j].type = type;													//種類番号を代入
					this->m_lpPowerCh[j].flag = true;													//生存フラグ
					this->m_lpPowerCh[j].scale = CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_IMAGE_SCALE;	//拡縮率
					this->m_lpPowerCh[j].time = 0;
					//アニメーションの設定
					this->m_lpPowerAni[j].cnt = 0;			//コマ数
					this->m_lpPowerAni[j].time = 0;			//経過時間
					//種類に応じて表示元座標を変更
					this->m_lpPowerCh[j].direction= type;
					break;
				}
			}
			return;
		}
	}
}


//************************************************************************************************
//更新
//************************************************************************************************
void CRunnerEffect::UpdateRunnerEffect(float x, float y, int w, int h, int invincibleTime) {

	//▼ エフェクト画像の表示
	for (int i = 0; i < this->m_CreateEffectMax; i++) {
		//生存フラグがtrueなら
		if (this->m_lpEffectCh[i].flag)
		{
			//座標を更新
			this->m_lpEffectCh[i].x = x - w;
			this->m_lpEffectCh[i].y = y;
			//縮小して表示フラグをfalseに
			this->m_lpEffectCh[i].scale += CRunnerEffect::RUNNER_EFFECT_TYPE_RING_IMAGE_CONTRACTION_SPEED;
			//0以下になったら表示フラグをfalseに
			if (this->m_lpEffectCh[i].scale <= 0) this->m_lpEffectCh[i].flag = false;
		}
	}

	//▼ エフェクト効果中の更新
	for (int i = 0; i < RUNNER_EFFECT_SIGN_KIND_OF_TYPE; i++) {
		//生存フラグがtrueなら更新
		if (this->m_lpPowerCh[i].flag) {
			//signal of repealing time for player's invincible
			if(this->m_lpPowerCh[i].type == RUNNER_EFFECT_TYPE_RED){
				if (invincibleTime < (RUNNER_ABSOLUTE_EFFECT_TIME >> 2)) {			//absolute time less than 125
					//count for flickers of invincible effect
					this->m_lpPowerCh[i].time++;
				}
			}
			//座標を更新
			this->m_lpPowerCh[i].x = x + this->m_lpPowerCh[i].w;			//X座標
			this->m_lpPowerCh[i].y = y +
				(this->m_lpPowerCh[i].type * this->m_lpPowerCh[i].h);		//Y座標
			//effect ball moves around the character
			this->m_lpPowerCh[i].sx = x;			//subsutitute character's coordinates for decide center coordinateX and Y 
			this->m_lpPowerCh[i].sy = y;
			//rotating angle
			this->m_lpPowerCh[i].cnt++;
			//rotate process
			CenterCharacter(&this->m_lpPowerCh[i], 25, this->m_lpPowerCh[i].cnt++);
			this->m_lpPowerCh[i].cnt %= 360;
			//アニメーション更新
			UpdateAnimation(&this->m_lpPowerCh[i], &this->m_lpPowerAni[i]);
		}
	}

}


//************************************************************************************************
//表示
//************************************************************************************************
void CRunnerEffect::DrawRunnerEffect(){

	//カメラの座標を取得
	float comeCameraY = GetCameraY();

	//エフェクト発生時画像の表示
	for( int i = 0 ; i < this->m_CreateEffectMax ; i++ ){

		//生存フラグtrueなら
		if( this->m_lpEffectCh[i].flag ){

			//効果発生時のエフェクト
			DDScaleBlt(						
				this->m_lpEffectCh[i].x ,									//表示座標X
				this->m_lpEffectCh[i].y - comeCameraY ,					//表示座標Y
				this->m_lpEffectCh[i].w ,									//幅
				this->m_lpEffectCh[i].h ,									//高さ
				this->m_lpRunnerEffectImg[0] ,							//画像の番号
				this->m_lpEffectCh[i].picx , this->m_lpEffectCh[i].picy , 	//表示元座標X・Ｙ
				this->m_lpEffectCh[i].scale );							//拡縮率
		}
	}

	//エフェクト効果中画像の表示
	for (int i = 0; i < RUNNER_EFFECT_SIGN_KIND_OF_TYPE; i++) {

		//生存フラグtrueなら
		if (this->m_lpPowerCh[i].flag) {
			if (this->m_lpPowerCh[i].time % 2 == 0) { //if present effect is invincible, count time get less than 125 to flickers

				//効果発生中のエフェクト
				DDScaleBlt(
					this->m_lpPowerCh[i].x,									//表示座標X
					this->m_lpPowerCh[i].y - comeCameraY,					//表示座標Y
					this->m_lpPowerCh[i].w,									//幅
					this->m_lpPowerCh[i].h,									//高さ
					this->m_lpRunnerEffectImg[1],							//画像の番号
					this->m_lpPowerCh[i].picx, this->m_lpPowerCh[i].picy,	//表示元座標X・Ｙ
					this->m_lpPowerCh[i].scale);							//拡縮率
			}
		}
	}

}


/*
**************************************************************************************************
エフェクトの解放
**************************************************************************************************
*/
void	CRunnerEffect::ReleaseRunnerEffect(){

	//使用したメモリの解放
	if( this->m_lpEffectAni != NULL ){
		delete []this->m_lpEffectAni;
		this->m_lpEffectAni = NULL;
	}
	if( this->m_lpPowerAni != NULL ){
		delete []this->m_lpPowerAni;
		this->m_lpPowerAni = NULL;
	}
	if( this->m_lpEffectCh != NULL ){
		delete []this->m_lpEffectCh;
		this->m_lpEffectCh = NULL;
	}
	if( this->m_lpPowerCh != NULL ){
		delete []this->m_lpPowerCh;
		this->m_lpPowerCh = NULL;
	}
	if( this->m_lpRunnerEffectImg != NULL ){
		delete []this->m_lpRunnerEffectImg;
		this->m_lpRunnerEffectImg = NULL;
	}

}


//************************************************************************************************
//画像の要素を設定する
//************************************************************************************************
void	CRunnerEffect::SetRunnerEffectImage( int* lpimage , int max ){

	//引数の最大数だけ画像の要素を確保する
	this->m_lpRunnerEffectImg = new int [ max ];

	//引数の最大数だけ画像の要素を設定する
	for( int i = 0 ; i < max ; i++ ){ this->m_lpRunnerEffectImg[i] = lpimage[i]; }
	
}
