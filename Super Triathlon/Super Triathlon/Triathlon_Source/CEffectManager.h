#ifndef		CEFFECTMANAGER_H__
#define		CEFFECTMANAGER_H__


//ヘッダーのインクルード
#include	"graphics.h"
#include	"CRunner.h"

//▼ 前方宣言
class CRunnerEffect;

//▼ ランナーエフェクトクラス
class CEffectManager{

private:

	//画像の要素数
	int		*m_lpEffectManagerImg;

	//エフェクトクラス
	CRunnerEffect	*m_lpCEffect;

	//エフェクトを扱う者の総数
	int		m_EffectUserMax;


protected:

public:

	//▼ 静的メンバの定義
	//ウィンドウハンドル
	static HWND m_hWnd;
	//画像の最大枚数
	static const int EFFECTMANAGER_PICTURE_MAX;
	//エフェクトの最大発生数
	static const int EFFECTMANAGER_CREATE_EFFECT_MAX;


	//▼ 自作関数の定義
	//コンストラクタ
	CEffectManager();
	//ディストラクタ
	~CEffectManager();
	//初期化
	void	InitEffectManager( int runnerMax );	
	//更新
	void	UpdateEffectManager( RUNNER *lpRunner , int runner );					
	//表示
	void	DrawEffectManager( int runner );							
	//解放
	void	ReleaseEffectManager();	
	//生成したいキャラクター用にエフェクトを生成する
	void	CreateEffectManaager( RUNNER *lpRunner , int runner );
	//効果を無効にするエフェクトの管理
	void	RepealEffectManager( unsigned char type , int runner );


};


#endif