#include	"CPlayerManager.h"
#include	"CPlayer.h"
#include	"CPieceManager.h"
#include	"graphics.h"
#include	"input.h"
#include	"MemoryLeakCheck.h"
#include	<iostream>
#include	<math.h>

//名前空間
using namespace	std;


//▼ 静的メンバ変数の再定義
//ウィンドウハンドル
HWND CPlayerManager::m_hWnd = NULL;
//画像の使用枚数
const int CPlayerManager::CPLAYERMANAGER_IMAGE_MAX = 1;
//プレイヤーの座標を扱う
TPOINT<float>	CPlayerManager::m_ArrowPosition = {0};
//プレイヤーのサイズ
const TPOINT<int> CPlayerManager::CPLAYERMANAGER_ARROW_SIZE = {46 , 46};
//プレイヤーの拡縮倍率
const float	CPlayerManager::CPLAYERMANAGER_DEFAULT_SCALE = 1.0f;



/*
************************************************************************************
プレイヤー管理のディストラクタ
************************************************************************************
*/
CPlayerManager::~CPlayerManager(){

	//解放全般
	this->ReleaseChara();
}

/*
************************************************************************************
プレイヤー管理の初期化
************************************************************************************
*/
void CPlayerManager::InitChara(){

	
	//▼ クラスのメモリ確保
	this->m_lpCPieceManager = new CPieceManager;		//ピース管理用
	this->m_lpCPlayer = new CPlayer;					//プレイヤー

	//画像の要素を確保
	this->m_lpPlayerImg = new int[ CPlayerManager::CPLAYERMANAGER_IMAGE_MAX ];


	//▼ 各種表示設定
	//並んだピースの一番左端のピースの座標を取得
	TPOINT<float> pos = this->m_lpCPieceManager->GetPiecePosition();

	//ピースのサイズを取得
	TPOINT<int>	size = this->m_lpCPieceManager->GetPieceSize();

	//各種値の設定
	this->m_lpCPlayer[0].m_Fpos = pos;							//表示座標
	this->m_lpCPlayer[0].m_Fmove.x = (float)size.x;				//移動量X
	this->m_lpCPlayer[0].m_Fmove.y = (float)size.y;				//移動量Y
	this->m_lpCPlayer[0].m_Isize = CPlayerManager::CPLAYERMANAGER_ARROW_SIZE;//サイズ
	this->m_lpCPlayer[0].m_existFlag = true;					//生存フラグ

	//プレイヤーの大きさをピースのサイズに合わせる
	float sizefit = (int)( sqrtf( ( this->m_lpCPlayer[0].m_Isize.x * this->m_lpCPlayer[0].m_Isize.x ) +
		( this->m_lpCPlayer[0].m_Isize.y * this->m_lpCPlayer[0].m_Isize.y ) ) ) / 
		(int)( sqrtf( ( size.x * size.x ) ) + ( size.y * size.y ) );
	
	sizefit += (int)( sqrtf( ( this->m_lpCPlayer[0].m_Isize.x * this->m_lpCPlayer[0].m_Isize.x ) +
		( this->m_lpCPlayer[0].m_Isize.y * this->m_lpCPlayer[0].m_Isize.y ) ) ) % 
		(int)( sqrtf( ( size.x * size.x ) ) + ( size.y * size.y ) );
	
	//求まった倍率を代入
	this->m_lpCPlayer[0].m_scale = sizefit * 0.01;

	//▼ 画像の読み込み
	//背景画像
	this->m_lpPlayerImg[0] = CreateDDImage( "Image\\Ch\\Arrow.bmp" , CPlayerManager::m_hWnd );

	//画像の要素番号を渡す
	this->m_lpCPlayer[0].SetCharaImg( this->m_lpPlayerImg[0] );

}

/*
************************************************************************************
プレイヤー管理の更新
************************************************************************************
*/
void CPlayerManager::UpdateChara(){

	//矢印の更新
	this->m_lpCPlayer[0].UpdateChara();

	//矢印の移動の更新
	this->MoveArrow();

	//矢印の移動制限
	this->ConstrainArrowMove();

	//矢印の座標更新
	this->m_ArrowPosition = this->m_lpCPlayer[0].m_Fpos;
	
}



/*
************************************************************************************
プレイヤー管理の表示
************************************************************************************
*/
void CPlayerManager::DrawChara(){


	//矢印の表示
	this->m_lpCPlayer[0].DrawChara();

}

/*
************************************************************************************
解放全般
************************************************************************************
*/
void CPlayerManager::ReleaseChara(){

	//画像の解放
	this->ReleaseImagePlayerManager();
	//クラスの解放
	this->ReleaseClassPlayerManager();

}

/*
************************************************************************************
画像の解放
************************************************************************************
*/
void CPlayerManager::ReleaseImagePlayerManager(){


	//初期化されていなかったら解放する
	if( m_lpPlayerImg != NULL ){
		//画像の解放
		for( int i = 0 ; i < CPlayerManager::CPLAYERMANAGER_IMAGE_MAX ; i++ ){
			ReleaseDDImage( this->m_lpPlayerImg[i] );
		}
	}

	//画像の要素
	if( this->m_lpPlayerImg != NULL){
		delete []this->m_lpPlayerImg;
		this->m_lpPlayerImg = NULL;
	}

}

/*
************************************************************************************
クラスの解放
************************************************************************************
*/
void CPlayerManager::ReleaseClassPlayerManager(){

	delete	this->m_lpCPieceManager;
	this->m_lpCPieceManager = NULL;		//ピース管理クラス
	
	delete	this->m_lpCPlayer;			//プレイヤークラス
	this->m_lpCPlayer = NULL;

}

/*
************************************************************************************
矢印の操作
************************************************************************************
*/
void CPlayerManager::MoveArrow(){

	//▼ 矢印の操作
	//上入力
	if( JustKey( VK_UP ) ){ this->m_lpCPlayer[0].m_Fpos.y -= this->m_lpCPlayer[0].m_Fmove.y; }
	//右入力
	if( JustKey( VK_RIGHT ) ){ this->m_lpCPlayer[0].m_Fpos.x += this->m_lpCPlayer[0].m_Fmove.x; }
	//下入力
	if( JustKey( VK_DOWN ) ){ this->m_lpCPlayer[0].m_Fpos.y += this->m_lpCPlayer[0].m_Fmove.y; }
	//左入力
	if( JustKey( VK_LEFT ) ){ this->m_lpCPlayer[0].m_Fpos.x -= this->m_lpCPlayer[0].m_Fmove.x; }


}


/*
************************************************************************************
矢印の移動制限
************************************************************************************
*/
void CPlayerManager::ConstrainArrowMove(){

	//並んだピースの一番左端のピースの座標を取得
	TPOINT<float> pos = this->m_lpCPieceManager->GetPiecePosition();

	//ピースのサイズを取得
	TPOINT<int>	size = this->m_lpCPieceManager->GetPieceSize();
	
	//ピースの全体サイズを取得
	TPOINT<int>	wholeSize = this->m_lpCPieceManager->GetWholePieceSize();

	//▼ 移動制限
	//配置したチップステージのサイズだけ制限する
	//上
	if( this->m_lpCPlayer[0].m_Fpos.y < pos.y ){
		this->m_lpCPlayer[0].m_Fpos.y = ( pos.y + wholeSize.y ) - size.y;
	}
	//右
	if( this->m_lpCPlayer[0].m_Fpos.x + ( this->m_lpCPlayer[0].m_Isize.x * this->m_lpCPlayer[0].m_scale ) > 
		pos.x + wholeSize.x ){
			this->m_lpCPlayer[0].m_Fpos.x = pos.x;
	}
	//下
	if( this->m_lpCPlayer[0].m_Fpos.y + ( this->m_lpCPlayer[0].m_Isize.y * this->m_lpCPlayer[0].m_scale ) > 
		pos.y + wholeSize.y ){
			this->m_lpCPlayer[0].m_Fpos.y = pos.y;
	}
	//左
	if( this->m_lpCPlayer[0].m_Fpos.x < pos.x ){
		this->m_lpCPlayer[0].m_Fpos.x = ( pos.x + wholeSize.x ) - size.x;
	}

}

