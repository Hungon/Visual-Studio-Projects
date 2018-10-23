#include	"CRunnerManager.h"
#include	"CCompetitorManager.h"
#include	"CPlayerRunner.h"
#include	"CRoadEnemyManager.h"
#include	"CEffectManager.h"
#include	"CRoadStage.h"
#include	"play.h"

//▼ 静的メンバ変数の再定義
//ウィンドウハンドル
const HWND	CRunnerManager::m_hWnd = NULL;
//画像の要素数
const int CRunnerManager::RUNNERMANAGER_IMAGE_MAX = 2;

//競争者の最大数
const int CRunnerManager::RUNNERMANAGER_COMPETITOR_MAX = 9;

//プレイヤーの順位
const float CRunnerManager::RUNNERMANAGER_RANKING_INDICATE_X	= 15.0f;		//表示先X
const float CRunnerManager::RUNNERMANAGER_RANKING_INDICATE_Y	= 320.0f;		//幅表示先Y
const int CRunnerManager::RUNNERMANAGER_RANKING_IMAGE_WIDTH		= 64;			//幅
const int CRunnerManager::RUNNERMANAGER_RANKING_IMAGE_HEIGHT	= 64;			//高さ
const float CRunnerManager::RUNNERMANAGER_RANKING_DEFAULT_SCALE = 1.0f;			//拡縮倍率
int CRunnerManager::m_PlayerRanking = 0;
//ミニマップ
const int CRunnerManager::RUNNERMANAGER_MINIMAP_IMAGE_MAX = 2;					//画像の数
const float CRunnerManager::RUNNERMANAGER_MINIMAP_INDICATE_X = 600.0f;			//表示先X
const float CRunnerManager::RUNNERMANAGER_MINIMAP_INDICATE_Y = 20.0f;			//表示先Y
const int CRunnerManager::RUNNERMANAGER_MINIMAP_IMAGE_WDITH = 32;				//幅
const int CRunnerManager::RUNNERMANAGER_MINIMAP_IMAGE_HEIGHT = 420;				//高さ

//ミニマップ上のプレイヤー
const int CRunnerManager::RUNNERMANAGER_MINIMAP_PLAYER_IMAGE_WDITH = 32;		//幅
const int CRunnerManager::RUNNERMANAGER_MINIMAP_PLAYER_IMAGE_HEIGHT = 96;		//高さ
const float CRunnerManager::RUNNERMANAGER_MINIMAP_PLAYER_DEFAULT_SCALE = 0.35f;	//拡縮倍率

//走者クラス
CPlayerRunner	*CRunnerManager::m_lpCPlayerRunner = NULL;
CCompetitorManager	*CRunnerManager::m_lpCCompetitorManager = NULL;


/*
*************************************************************************************************
コンストラクタ
*************************************************************************************************
*/
CRunnerManager::CRunnerManager(){
	

}

/*
*************************************************************************************************
ディストラクタ
*************************************************************************************************
*/
CRunnerManager::~CRunnerManager(){

	//メモリの解放
	if( this->m_lpCPlayerRunner != NULL ){
		delete this->m_lpCPlayerRunner;			//プレイヤークラス
		this->m_lpCPlayerRunner = NULL;
	}
	if( this->m_lpCCompetitorManager != NULL ){
		delete this->m_lpCCompetitorManager;	//競争者管理クラス
		this->m_lpCCompetitorManager = NULL;
	}
	if( this->m_lpCEffectManager != NULL ){
		delete this->m_lpCEffectManager;		//エフェクト管理クラス
		this->m_lpCEffectManager = NULL;
	}
	if( this->m_lpCRoadStage != NULL ){
		delete this->m_lpCRoadStage;			//ステージクラス
		this->m_lpCRoadStage = NULL;
	}
	if( this->m_lpMiniMap != NULL ){
		delete []this->m_lpMiniMap;				//ミニマップ
		this->m_lpMiniMap = NULL;
	}
}


/*
*************************************************************************************************
初期化
*************************************************************************************************
*/
void	CRunnerManager::InitRunnerManager( CRoadEnemyManager	*lpRoadEnemy ){

	//難易度を取得
	tagGAMELEVEL	gameLevel = GetGameLevel();

	//画像データ
	char	*lpImageTbl[ CRunnerManager::RUNNERMANAGER_IMAGE_MAX ] = {
		{ "Image\\Road\\MiniMap.bmp" }		,		//ミニマップ
		{ "Image\\Road\\PlayerRunner.bmp" }		,	//プレイヤー
	};
	//競争者の数
	char	CompetitorMaxTbl[ LEVEL_MAX ] = { 3 , 5 , CRunnerManager::RUNNERMANAGER_COMPETITOR_MAX };

	//各種メモリの確保
	this->m_lpCCompetitorManager = new CCompetitorManager;		//競争者管理クラス
	this->m_lpCPlayerRunner = new CPlayerRunner;				//プレイヤークラス
	this->m_lpCEffectManager = new CEffectManager;				//エフェクト管理クラス
	this->m_lpCRoadStage = new CRoadStage;						//ステージクラス
	this->m_lpRunnerManagerImg = new int [ CRunnerManager::RUNNERMANAGER_IMAGE_MAX ];	//画像
	this->m_lpMiniMap = new CHARACTER [ CRunnerManager::RUNNERMANAGER_MINIMAP_IMAGE_MAX ];//ミニマップ

	//今回のステージの全長を取得
	this->m_StageWholeDistance = this->m_lpCRoadStage->GetRoadStageDistance();

	//競争者の最大数
	this->m_CompetitorMax = CompetitorMaxTbl[ gameLevel ];

	//▼ プレイヤー順位表示用
	this->m_RunnerRanking.x = RUNNERMANAGER_RANKING_INDICATE_X;			//表示先X
	this->m_RunnerRanking.y = RUNNERMANAGER_RANKING_INDICATE_Y;			//表示先Y
	this->m_RunnerRanking.w = RUNNERMANAGER_RANKING_IMAGE_WIDTH;		//幅
	this->m_RunnerRanking.h = RUNNERMANAGER_RANKING_IMAGE_HEIGHT;		//高さ
	this->m_RunnerRanking.scale = RUNNERMANAGER_RANKING_DEFAULT_SCALE;	//拡縮倍率

	//ミニマップ用
	//0で初期化
	ZeroMemory( this->m_lpMiniMap , sizeof( CHARACTER ) * CRunnerManager::RUNNERMANAGER_MINIMAP_IMAGE_MAX );
	//ミニ道路画像
	this->m_lpMiniMap[0].x = CRunnerManager::RUNNERMANAGER_MINIMAP_INDICATE_X;	//表示先X
	this->m_lpMiniMap[0].y = CRunnerManager::RUNNERMANAGER_MINIMAP_INDICATE_Y;	//表示先Y
	this->m_lpMiniMap[0].w = CRunnerManager::RUNNERMANAGER_MINIMAP_IMAGE_WDITH;	//幅
	this->m_lpMiniMap[0].h = CRunnerManager::RUNNERMANAGER_MINIMAP_IMAGE_HEIGHT;//高さ

	//ミニマップ上のプレイヤー
	this->m_lpMiniMap[1].x = this->m_lpMiniMap[0].x;										//表示先X
	this->m_lpMiniMap[1].y = ( this->m_lpMiniMap[0].y + this->m_lpMiniMap[0].h );			//表示先Y
	this->m_lpMiniMap[1].w = CRunnerManager::RUNNERMANAGER_MINIMAP_PLAYER_IMAGE_WDITH;		//幅
	this->m_lpMiniMap[1].h = CRunnerManager::RUNNERMANAGER_MINIMAP_PLAYER_IMAGE_HEIGHT;		//高さ
	this->m_lpMiniMap[1].scale = CRunnerManager::RUNNERMANAGER_MINIMAP_PLAYER_DEFAULT_SCALE;//拡縮倍率



	//画像の読み込み
	for( int i = 0 ; i < CRunnerManager::RUNNERMANAGER_IMAGE_MAX ; i++ ){
		m_lpRunnerManagerImg[i] = CreateDDImage( lpImageTbl[i] , CRunnerManager::m_hWnd );
	}

	//▼ プレイヤーランナーの初期化
	//引数に人数と敵のアドレスを渡す
	this->m_lpCPlayerRunner->InitRunner( 1 , lpRoadEnemy , this->m_StageWholeDistance );

	//プレイヤーのエフェクトの初期化
	//引数に人数 
	this->m_lpCEffectManager->InitEffectManager( 1 );

	//▼ 競争者の初期化
	this->m_lpCCompetitorManager->InitCompetitorManager( this->m_CompetitorMax , lpRoadEnemy , this->m_StageWholeDistance );

	//treat current player's ranking
	this->m_PlayerRanking = 1;
}

/*
*************************************************************************************************
更新
*************************************************************************************************
*/
void	CRunnerManager::UpdateRunnerManager(){

	//ランキングカウント用
	int rankingCnt = 1;

	//各種競争者の座標を取得
	TPOINT<float> *lpCompetitorPos = this->m_lpCCompetitorManager->GetCompetitorManagerPosition();

	//プレイヤーの情報を取得
	RUNNER	*lpRunner = this->m_lpCPlayerRunner->GetRunner();

	//▼ プレイヤーランナーの更新
	//プレイヤーにエフェクトのアドレスを渡す
	this->m_lpCPlayerRunner->UpdateRunner( 0 , this->m_lpCEffectManager );

	//▼ 競争者の更新
	this->m_lpCCompetitorManager->UpdateCompetitorManager();

	//プレイヤーの順位を決定
	for( int i = 0 ; i < this->m_CompetitorMax ; i++ ){ if( lpRunner->ch.y > lpCompetitorPos[i].y ) rankingCnt++; }
	//現在の順位を代入
	this->m_PlayerRanking = rankingCnt;

	//プレイヤーのY座標がステージの全長のどのくらいの割合に位置しているかでミニマップ上のプレイヤーの座標を更新する
	this->m_lpMiniMap[1].y = this->m_lpMiniMap[0].h * 
		( lpRunner->ch.y / this->m_StageWholeDistance ) - this->m_lpMiniMap[0].y;


}

/*
*************************************************************************************************
表示
*************************************************************************************************
*/
void	CRunnerManager::DrawRunnerManager(){

	//▼ プレイヤーランナーの表示
	this->m_lpCPlayerRunner->DrawRunner();

	//プレイヤーのエフェクトを表示
	this->m_lpCEffectManager->DrawEffectManager( 0 );

	//▼ 競争者の表示
	this->m_lpCCompetitorManager->DrawCompetitorManager();

	//表示元座標の更新
	this->m_RunnerRanking.picy = ( this->m_PlayerRanking - 1 ) * this->m_RunnerRanking.h;

	//▼ プレイヤーの順位を表示
	DrawRanking(this->m_RunnerRanking.x,
		this->m_RunnerRanking.y,
		this->m_RunnerRanking.scale,
		this->m_PlayerRanking,
		SCORE_RANING_COLOR_BLACK);

	//▼ ミニマップの表示
	//背景
	DDBlt(
		(int)this->m_lpMiniMap[0].x ,
		(int)this->m_lpMiniMap[0].y ,
		this->m_lpMiniMap[0].w ,
		this->m_lpMiniMap[0].h ,
		m_lpRunnerManagerImg[0] ,
		this->m_lpMiniMap[0].picx ,
		this->m_lpMiniMap[0].picy 
		);
	//プレイヤー
	DDScaleBlt(
		this->m_lpMiniMap[1].x ,
		this->m_lpMiniMap[1].y ,
		this->m_lpMiniMap[1].w ,
		this->m_lpMiniMap[1].h ,
		m_lpRunnerManagerImg[1] ,
		this->m_lpMiniMap[1].picx ,
		this->m_lpMiniMap[1].picy ,
		this->m_lpMiniMap[1].scale
		);


}

/*
*************************************************************************************************
解放
*************************************************************************************************
*/
void	CRunnerManager::ReleaseRunnerManager(){

	//画像の解放
	if(  this->m_lpRunnerManagerImg != NULL ){
		for( int i = 0 ; i < CRunnerManager::RUNNERMANAGER_IMAGE_MAX ; i++ ){
			ReleaseDDImage( this->m_lpRunnerManagerImg[i] );
		}
	}
	//▼プレイヤーランナーの解放
	this->m_lpCPlayerRunner->ReleaseRunner();
	//プレイヤーのエフェクトの解放
	this->m_lpCEffectManager->ReleaseEffectManager();

	//▼競争者の解放
	this->m_lpCCompetitorManager->ReleaseCompetitorManager();

	//メモリの解放
	delete []this->m_lpRunnerManagerImg;	//画像
	this->m_lpRunnerManagerImg = NULL;

	//メモリの解放
	if( this->m_lpCPlayerRunner != NULL ){
		delete this->m_lpCPlayerRunner;			//プレイヤークラス
		this->m_lpCPlayerRunner = NULL;
	}
	if( this->m_lpCCompetitorManager != NULL ){
		delete this->m_lpCCompetitorManager;	//競争者管理クラス
		this->m_lpCCompetitorManager = NULL;
	}
	if( this->m_lpCEffectManager != NULL ){
		delete this->m_lpCEffectManager;		//エフェクト管理クラス
		this->m_lpCEffectManager = NULL;
	}
	if( this->m_lpCRoadStage != NULL ){
		delete this->m_lpCRoadStage;			//ステージクラス
		this->m_lpCRoadStage = NULL;
	}
	if( this->m_lpMiniMap != NULL ){
		delete []this->m_lpMiniMap;				//ミニマップ
		this->m_lpMiniMap = NULL;
	}

}

