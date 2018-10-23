#include	"CRoadEnemyManager.h"
#include	"CRunnerManager.h"
#include	"CRoadEnemy.h"
#include	"sound.h"
#include	"play.h"

//▼ 静的メンバ変数の再定義
const int	CRoadEnemyManager::ROADENEMYMANAGER_IMAGE_MAX = 2;			//使用画像枚数
const int	CRoadEnemyManager::ROADENEMYMANAGER_EFFECT_SOUND_MAX = 2;	//使用効果音数
const HWND	CRoadEnemyManager::m_hWnd = NULL;


/*
*************************************************************************************************
コンストラクタ
*************************************************************************************************
*/
CRoadEnemyManager::CRoadEnemyManager(){
	

}

/*
*************************************************************************************************
ディストラクタ
*************************************************************************************************
*/
CRoadEnemyManager::~CRoadEnemyManager(){

}


/*
*************************************************************************************************
初期化
*************************************************************************************************
*/
void	CRoadEnemyManager::InitRoadEnemyManager( CRunnerManager *lpRunnerManager ){

	//▼ 使用する画像
	char	*lpImageTbl[ CRoadEnemyManager::ROADENEMYMANAGER_IMAGE_MAX ] = {
		{ "Image\\Road\\hurdle.bmp" }	,
		{ "Image\\Road\\stone.bmp" }	,
	};
	//▼ 使用する効果音
	char	*lpSoundTbl[CRoadEnemyManager::ROADENEMYMANAGER_EFFECT_SOUND_MAX] = {
		{ "SE\\rattling.wav" },
		{ "SE\\Damage.wav" },
	};

	//▼ 難易度に応じた発生させる障害物の最大数のデータテーブル
	int obstacleMaxTbl[LEVEL_MAX] = { 8 , 10 , CObstacle::ROADENEMY_OBSTACLE_MAX };

	//ゲーム難易度を取得する
	tagGAMELEVEL getGameLevel = GetGameLevel();

	//今回生成する障害物の最大数を代入
	this->m_ObstacleCreateMax = obstacleMaxTbl[ getGameLevel ];

	//▼ 使用するメモリの確保
	//引数に生成可能最大数を渡す
	this->m_lpCObstacle = new CObstacle( this->m_ObstacleCreateMax );			//障害物クラス
	
	//画像の要素
	this->m_lpRoadEnemyManagerImg = new int [ CRoadEnemyManager::ROADENEMYMANAGER_IMAGE_MAX ];

	//効果音の要素
	this->m_lpRoadEnemyManagerEffectSound = new int [ CRoadEnemyManager::ROADENEMYMANAGER_EFFECT_SOUND_MAX ];

	//画像の要素を生成する
	for( int i = 0 ; i < CRoadEnemyManager::ROADENEMYMANAGER_IMAGE_MAX ; i++ ){
		//画像の要素を生成
		this->m_lpRoadEnemyManagerImg[i] = CreateDDImage( lpImageTbl[i] , CRoadEnemyManager::m_hWnd );

	}
	//生成した画像の要素を渡す
	this->m_lpCObstacle->SetRoadEnemyImage( this->m_lpRoadEnemyManagerImg , CRoadEnemyManager::ROADENEMYMANAGER_IMAGE_MAX );

	//効果音の要素を設定する
	for( int i = 0 ; i < CRoadEnemyManager::ROADENEMYMANAGER_EFFECT_SOUND_MAX ; i++ ){
		this->m_lpRoadEnemyManagerEffectSound[i] = CreateSound( lpSoundTbl[i] );
	}
	//生成した効果音の要素を渡す
	this->m_lpCObstacle->SetRoadEnemyEffectSound( this->m_lpRoadEnemyManagerEffectSound , CRoadEnemyManager::ROADENEMYMANAGER_EFFECT_SOUND_MAX );

	//▼ 障害物の初期化
	//引数に各走者のアドレスを渡す
	this->m_lpCObstacle->InitRoadEnemy( lpRunnerManager->GetCCompetitorManager() ,
		lpRunnerManager->GetCPlayerRunner() );

}

/*
*************************************************************************************************
更新
*************************************************************************************************
*/
void	CRoadEnemyManager::UpdateRoadEnemyManager(){

	//障害物の更新
	this->m_lpCObstacle->UpdateRoadEnemy();
}

/*
*************************************************************************************************
表示
*************************************************************************************************
*/
void	CRoadEnemyManager::DrawRoadEnemyManager(){

	//障害物の表示
	this->m_lpCObstacle->DrawRoadEnemy();

}

/*
*************************************************************************************************
解放
*************************************************************************************************
*/
void	CRoadEnemyManager::ReleaseRoadEnemyManager(){

	//画像の解放
	if( this->m_lpRoadEnemyManagerImg != NULL ){
		for( int i = 0 ; i < CRoadEnemyManager::ROADENEMYMANAGER_IMAGE_MAX ; i++ ){
			ReleaseDDImage( this->m_lpRoadEnemyManagerImg[i] );
		}
	}
	//効果音の解放
	if( this->m_lpRoadEnemyManagerEffectSound != NULL ){
		for( int i = 0 ; i < CRoadEnemyManager::ROADENEMYMANAGER_EFFECT_SOUND_MAX ; i++ ){
			ReleaseSound( this->m_lpRoadEnemyManagerEffectSound[i] );
		}
	}

	//障害物の解放
	this->m_lpCObstacle->ReleaseRoadEnemy();

	//メモリの解放
	delete []this->m_lpRoadEnemyManagerImg;			//画像の要素
	this->m_lpRoadEnemyManagerImg = NULL;
	delete []this->m_lpRoadEnemyManagerEffectSound;	//効果音の要素
	this->m_lpRoadEnemyManagerEffectSound = NULL;

	if( this->m_lpCObstacle != NULL ){
		delete this->m_lpCObstacle;					//障害物クラス
		this->m_lpCObstacle = NULL;
	}
	


}

/*
*************************************************************************************************
引数のランナーと重なりの判定を取る
*************************************************************************************************
*/
char	CRoadEnemyManager::CollisionRoadEnemyManager( RUNNER *lpRunner ){

	//障害物クラスの障害物と判定を取る
	char	ret = this->m_lpCObstacle->CollisionRoadEnemy( lpRunner );
	//戻り値に今回の効果の種類を返す
	return ret;
}


/*
*************************************************************************************************
引数のキャラクター構造体の索敵範囲で判定を取る
引数   : キャラクター構造体
戻り値 : 障害物の種類番号を返す
***********************************************************************************************
*/
BYTE	CRoadEnemyManager::BroadenCollisionSquareToObstacle( CHARACTER *lpCh ){

	//引数の索敵範囲に障害物があるか調べ、その種類を返す
	BYTE ret = this->m_lpCObstacle->BroadenCollisionSquareObstacle( lpCh );
	//戻り値に障害物の種類番号を返す
	return ret;
}


