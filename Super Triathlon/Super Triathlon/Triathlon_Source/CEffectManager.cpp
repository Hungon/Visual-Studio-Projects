#include	"CEffectManager.h"
#include	"RunnerEffect.h"


//▼ 静的メンバ変数の再定義
//ウィンドウハンドル
HWND CEffectManager::m_hWnd = NULL;
//画像の最大枚数
const int CEffectManager::EFFECTMANAGER_PICTURE_MAX = 2;
//エフェクトの最大発生数
const int CEffectManager::EFFECTMANAGER_CREATE_EFFECT_MAX = 3;


/*
*************************************************************************************************
コンストラクタ
*************************************************************************************************
*/
CEffectManager::CEffectManager(){
	

}

/*
*************************************************************************************************
ディストラクタ
*************************************************************************************************
*/
CEffectManager::~CEffectManager(){

	//メモリの解放
	if( this->m_lpCEffect != NULL ){
		delete []this->m_lpCEffect;					//エフェクトクラス
		this->m_lpCEffect = NULL;
	}

}


/*
*************************************************************************************************
初期化
*************************************************************************************************
*/
void	CEffectManager::InitEffectManager( int runnerMax ){

	//▼ 使用する画像
	char	*lpImageTbl[ CEffectManager::EFFECTMANAGER_PICTURE_MAX ] = {
		{ "Image\\Road\\RunnerEffect.bmp" }			,
		{ "Image\\Road\\RunnerEffectPower.bmp" }	,
	};

	//エフェクトを扱う者の総数
	this->m_EffectUserMax = runnerMax;

	//▼ 各種メモリの確保
	//エフェクトの総数だけメモリを確保する
	this->m_lpCEffect = new CRunnerEffect[ this->m_EffectUserMax ];
	//画像の要素
	this->m_lpEffectManagerImg = new int[ CEffectManager::EFFECTMANAGER_PICTURE_MAX ];
	

	//▼ 画像の要素の生成
	for( int i = 0 ; i < CEffectManager::EFFECTMANAGER_PICTURE_MAX ; i++ ){
		//画像要素の生成		
		this->m_lpEffectManagerImg[i] = CreateDDImage( lpImageTbl[i] , CEffectManager::m_hWnd );
	}
	//各種エフェクトクラスへの設定
	for( int i = 0 ; i < this->m_EffectUserMax ; i++ ){
		//画像の要素を渡す
		this->m_lpCEffect[i].SetRunnerEffectImage( this->m_lpEffectManagerImg , CEffectManager::EFFECTMANAGER_PICTURE_MAX );
		//初期化
		this->m_lpCEffect[i].InitRunnerEffect( CEffectManager::EFFECTMANAGER_CREATE_EFFECT_MAX );

	}

}

/*
*************************************************************************************************
更新
*************************************************************************************************
*/
void	CEffectManager::UpdateEffectManager( RUNNER *lpRunner , int runner ){

	//各種エフェクトクラスの更新
	this->m_lpCEffect[runner].UpdateRunnerEffect(lpRunner->ch.x,
		lpRunner->ch.y, lpRunner->ch.w, lpRunner->ch.h, lpRunner->absoluteTime);

}

/*
*************************************************************************************************
表示
*************************************************************************************************
*/
void	CEffectManager::DrawEffectManager( int runner ){

	//各種エフェクトクラスの表示
	this->m_lpCEffect[runner].DrawRunnerEffect();

}

/*
*************************************************************************************************
解放
*************************************************************************************************
*/
void	CEffectManager::ReleaseEffectManager(){

	//各種エフェクトクラスの解放
	for( int i = 0 ; i < this->m_EffectUserMax ; i++ ){
		//解放
		this->m_lpCEffect[i].ReleaseRunnerEffect();
	}

	//画像の解放
	//未解放なら
	if( this->m_lpEffectManagerImg != NULL ){
		for( int i = 0 ; i < CEffectManager::EFFECTMANAGER_PICTURE_MAX ; i++ ){
			ReleaseDDImage( this->m_lpEffectManagerImg[i] );
		}
	}
	//メモリの開放
	delete []this->m_lpEffectManagerImg;		//画像の要素
	this->m_lpEffectManagerImg = NULL;

}
/*
*********************************************************************************
生成したいキャラクター用にエフェクトを生成する
*********************************************************************************
*/
void	CEffectManager::CreateEffectManaager( RUNNER *lpRunner , int runner ){

	//引数の要素のキャラクターのエフェクトを生成
	this->m_lpCEffect[runner].CreateRunnerEffect( lpRunner->ch.x , lpRunner->ch.y , ( lpRunner->bumpType - 1 ) );
}

/*
*********************************************************************************
効果を無効にするエフェクトの管理
*********************************************************************************
*/
void	CEffectManager::RepealEffectManager( unsigned char type , int runner ){

	//引数の要素のキャラクターのエフェクトを無効にする
	this->m_lpCEffect[runner].GetRunnerRepealEffectType( type );
}