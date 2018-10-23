#ifndef RUNNER_EFFECT_H__
#define RUNNER_EFFECT_H__


enum tagRUNNEREFFECTKIND {
	RUNNER_EFFECT_TYPE_BLUE,			//速度上昇
	RUNNER_EFFECT_TYPE_RED,				//無敵
	RUNNER_EFFECT_TYPE_GREEN,			//回復
	RUNNEREFFECT_KIND_OF_TYPE,			//効果の種類
	RUNNER_EFFECT_SIGN_KIND_OF_TYPE = 2,	//効果中を知らせる種類
};

//ヘッダーのインクルード
#include	"graphics.h"
#include	"animation.h"


//▼ 前方宣言
class CPlayerRunner;

//▼ ランナーエフェクトクラス
class CRunnerEffect{

private:

	//効果発生時のエフェクト用
	CHARACTER	*m_lpEffectCh;
	ANIMATION	*m_lpEffectAni;
	//効果中のエフェクト用
	CHARACTER	*m_lpPowerCh;
	ANIMATION	*m_lpPowerAni;
	//画像の要素数
	int		*m_lpRunnerEffectImg;
	//発生するエフェクトの最大数
	int		m_CreateEffectMax;
	//PlayerRunner's class
	CPlayerRunner	*m_lpCPlayer;

protected:

public:

	//▼ 静的メンバの定義

	//エフェクトタイプ毎の画像設定
	//リング型
	static const int RUNNER_EFFECT_TYPE_RING_IMAGE_HEIGHT;		//高さ
	static const int RUNNER_EFFECT_TYPE_RING_IMAGE_WIDTH;		//幅
	static const float RUNNER_EFFECT_TYPE_RING_IMAGE_SCALE;		//拡大率
	//拡大率の速度
	static const float RUNNER_EFFECT_TYPE_RING_IMAGE_CONTRACTION_SPEED;

	//ボール型
	static const int RUNNER_EFFECT_TYPE_BALL_IMAGE_HEIGHT;		//高さ
	static const int RUNNER_EFFECT_TYPE_BALL_IMAGE_WIDTH;		//幅
	static const float RUNNER_EFFECT_TYPE_BALL_IMAGE_SCALE;		//拡大率
	//アニメーション設定
	static const int RUNNER_EFFECT_TYPE_BALL_ANIMATION_MAX;		//最大コマ数
	static const int RUNNER_EFFECT_TYPE_BALL_ANIMATION_FRAM;	//フレーム数
	
																//▼ 自作関数の定義
	//コンストラクタ
	CRunnerEffect();
	//ディストラクタ
	~CRunnerEffect();
	//初期化
	void InitRunnerEffect( int effectMax );	
	//更新
	void UpdateRunnerEffect(float x, float y, int w, int h, int invincibleTime);
	//表示
	void DrawRunnerEffect();							
	//解放
	void ReleaseRunnerEffect();				
	/************************************************************************************************
	エフェクト生成
	*************************************************************************************************
	*/
	void CreateRunnerEffect( float x , float y , char type );
	/*
	***************************************************************************************************
	無効にするエフェクトタイプを受け取る
	***************************************************************************************************
	*/
	void GetRunnerRepealEffectType( const unsigned char type ){	this->m_lpPowerCh[type].flag = false;	}

	//画像の要素を設定する
	void	SetRunnerEffectImage( int *lpimage , int max );

};


#endif