#include	"CCompetitorManager.h"
#include	"CompetitorRunner.h"
#include	"sound.h"
#include	"CEffectManager.h"


//▼ 静的メンバ変数の再定義
const int	CCompetitorManager::COMPETITORMANAGER_IMAGE_MAX = 2;		//使用画像枚数
const int	CCompetitorManager::COMPETITORMANAGER_EFFECT_SOUND_MAX = 1;	//使用効果音数
const HWND	CCompetitorManager::m_hWnd = NULL;
//各競争者の座標を扱う
TPOINT<float>	*CCompetitorManager::m_lpCompetitorPos = {0};
//各障害物の効果を発揮した障害物の要素
int		*CCompetitorManager::m_lpCompetitorBumpObstacleNow = NULL;
//競争者の最大数
char	CCompetitorManager::m_CompetitorMax = 0;

//競争者クラス
CCompetitor	*CCompetitorManager::m_lpCCompetitorRunner = NULL;

/*
*************************************************************************************************
コンストラクタ
*************************************************************************************************
*/
CCompetitorManager::CCompetitorManager(){
	
	this->m_lpCCompetitorRunner = NULL;
	this->m_lpCEffect = NULL;
	this->m_CompetitorMax = 0;
	this->m_lpCompetitorEffectSound = NULL;
	this->m_lpCompetitorImg = NULL;

}

/*
*************************************************************************************************
ディストラクタ
*************************************************************************************************
*/
CCompetitorManager::~CCompetitorManager(){

	//メモリの解放
	if( this->m_lpCCompetitorRunner != NULL ){
		delete []this->m_lpCCompetitorRunner;		//競争者のクラス
		this->m_lpCCompetitorRunner = NULL;
	}
	if( this->m_lpCEffect != NULL ){
		delete this->m_lpCEffect;					//エフェクト管理クラス
		this->m_lpCEffect = NULL;
	}
	if( this->m_lpCompetitorPos != NULL ){
		delete []this->m_lpCompetitorPos;			//各競争者の座標
		this->m_lpCompetitorPos = NULL;
	}
	if( this->m_lpCompetitorBumpObstacleNow != NULL ){
		delete []this->m_lpCompetitorBumpObstacleNow;//障害物の要素
		this->m_lpCompetitorBumpObstacleNow = NULL;
	}


}


/*
*************************************************************************************************
初期化
*************************************************************************************************
*/
void	CCompetitorManager::InitCompetitorManager( int runnerMax , CRoadEnemyManager *lpRoadEnemy , float roadDistance ){


	//▼ 使用する画像
	char	*lpImageTbl[ CCompetitorManager::COMPETITORMANAGER_IMAGE_MAX ] = {
		{ "Image\\Road\\CompetitorRunner.bmp" }	,
		{ "Image\\Road\\Shadow.bmp" }	,
	};
	//▼ 使用する効果音
	char	*lpSoundTbl[ CCompetitorManager::COMPETITORMANAGER_EFFECT_SOUND_MAX ] = {
		{ "SE\\attack.wav" }
	};

	//競争者の数
	this->m_CompetitorMax = runnerMax;

	//▼ 使用するメモリの確保
	this->m_lpCCompetitorRunner = new CCompetitor[ this->m_CompetitorMax ];			//競争者クラス
	this->m_lpCEffect = new CEffectManager;										//エフェクト管理クラス

	//画像の要素
	this->m_lpCompetitorImg = new int [ CCompetitorManager::COMPETITORMANAGER_IMAGE_MAX ];

	//取得座標用
	this->m_lpCompetitorPos = new TPOINT<float> [ this->m_CompetitorMax ];

	//障害物の要素取得用
	this->m_lpCompetitorBumpObstacleNow = new int [ this->m_CompetitorMax ];

	//効果音の要素
	this->m_lpCompetitorEffectSound = new int [ CCompetitorManager::COMPETITORMANAGER_EFFECT_SOUND_MAX ];

	//画像の要素を生成する
	for( int i = 0 ; i < CCompetitorManager::COMPETITORMANAGER_IMAGE_MAX ; i++ ){
		//画像の要素を生成
		this->m_lpCompetitorImg[i] = CreateDDImage( lpImageTbl[i] , CCompetitorManager::m_hWnd );
	}
	//効果音の要素を設定する
	for( int i = 0 ; i < CCompetitorManager::COMPETITORMANAGER_EFFECT_SOUND_MAX ; i++ ){
		this->m_lpCompetitorEffectSound[i] = CreateSound( lpSoundTbl[i] );
	}

	//▼ 画像の設定
	for( int i = 0 ; i < this->m_CompetitorMax ; i++ ){
		//競争者自身の画像要素
		this->m_lpCCompetitorRunner[i].SetCompetitorImage( this->m_lpCompetitorImg[0] );
		//競争者の影
		this->m_lpCCompetitorRunner[i].SetCompetitorShadowImage( this->m_lpCompetitorImg[1] );
		//効果音
		this->m_lpCCompetitorRunner[i].SetCompetitorEffectSound( this->m_lpCompetitorEffectSound[0] );
		//各種競争者の初期化
		this->m_lpCCompetitorRunner[i].InitRunner( i , lpRoadEnemy , roadDistance );
		//座標取得用
		this->m_lpCompetitorPos[i] = this->m_lpCCompetitorRunner[i].GetOneCompetitorPosition();
		//障害物の要素
		this->m_lpCompetitorBumpObstacleNow[i] = this->m_lpCCompetitorRunner[i].GetCompetitorJumpedObstacleMaterial();
	}

	//▼ エフェクトの初期化
	this->m_lpCEffect->InitEffectManager( this->m_CompetitorMax );

}

/*
*************************************************************************************************
更新
*************************************************************************************************
*/
void	CCompetitorManager::UpdateCompetitorManager( ){

	//各種競争者の更新
	for( int i = 0 ; i < this->m_CompetitorMax ; i++ ){
		//競争者自身の更新
		this->m_lpCCompetitorRunner[i].UpdateRunner( i , this->m_lpCEffect );
		//座標を取得
		this->m_lpCompetitorPos[i] = this->m_lpCCompetitorRunner[i].GetOneCompetitorPosition();
		//障害物の要素を取得する
		this->m_lpCompetitorBumpObstacleNow[i] = this->m_lpCCompetitorRunner[i].GetCompetitorJumpedObstacleMaterial();
	
	}

}

/*
*************************************************************************************************
表示
*************************************************************************************************
*/
void	CCompetitorManager::DrawCompetitorManager(){

	//各種競争者の表示
	for( int i = 0 ; i < this->m_CompetitorMax ; i++ ){
		//競争者自身
		this->m_lpCCompetitorRunner[i].DrawRunner();
		//エフェクト
		this->m_lpCEffect->DrawEffectManager( i );
	}

}

/*
*************************************************************************************************
解放
*************************************************************************************************
*/
void	CCompetitorManager::ReleaseCompetitorManager(){

	//画像の解放
	if( this->m_lpCompetitorImg != NULL ){
		for( int i = 0 ; i < CCompetitorManager::COMPETITORMANAGER_IMAGE_MAX ; i++ ){
			ReleaseDDImage( this->m_lpCompetitorImg[i] );
		}
	}
	//効果音の解放
	if( this->m_lpCompetitorEffectSound != NULL ){
		for( int i = 0 ; i < CCompetitorManager::COMPETITORMANAGER_EFFECT_SOUND_MAX ; i++ ){
			ReleaseSound( this->m_lpCompetitorEffectSound[i] );
		}
	}
	//各種競争者の解放
	for( int i = 0 ; i < this->m_CompetitorMax ; i++ ){
		//競争者自身
		this->m_lpCCompetitorRunner[i].ReleaseRunner();
	}
	//エフェクト
	this->m_lpCEffect->ReleaseEffectManager();

	//メモリの解放
	delete []this->m_lpCompetitorImg;			//画像の要素
	this->m_lpCompetitorImg = NULL;
	delete []this->m_lpCompetitorEffectSound;	//効果音の要素
	this->m_lpCompetitorEffectSound = NULL;


}

//障害物の要素
void	CCompetitorManager::SetCompetitorManagerObstacleMaterial( int material , int runner ){

	this->m_lpCCompetitorRunner[runner].SetCompetitorJumpedObstacleMaterial( material );
}
