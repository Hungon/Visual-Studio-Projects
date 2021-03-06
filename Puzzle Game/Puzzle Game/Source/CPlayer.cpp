#include	"CPlayer.h"
#include	"graphics.h"
#include	"MemoryLeakCheck.h"
#include	<iostream>

using namespace std;


//▼ 静的メンバ変数の再定義


/*
************************************************************************************
プレイヤーのコンストラクタ
************************************************************************************
*/
CPlayer::CPlayer(){

}


/*
************************************************************************************
プレイヤーのディストラクタ
************************************************************************************
*/
CPlayer::~CPlayer(){


}


/*
************************************************************************************
プレイヤーの初期化
************************************************************************************
*/
void CPlayer::InitChara(){


}


/*
************************************************************************************
プレイヤーの更新
************************************************************************************
*/
void CPlayer::UpdateChara(){

}


/*
************************************************************************************
プレイヤーの表示
************************************************************************************
*/
void CPlayer::DrawChara(){

	TPOINT<int> pos = { 400 , 0 };
	char mes[25] = {0};

	//生存フラグがtrueなら
	if( this->m_existFlag ){
		DDScaleBltSelectIndicate(
			this->m_Fpos.x ,
			this->m_Fpos.y ,
			this->m_Isize.x ,
			this->m_Isize.y ,
			this->m_ImgNum ,
			this->m_Ipic.x ,
			this->m_Ipic.y ,
			this->m_scale  ,
			DDSCALE_STARTING_POINT_LEFT_TOP);

	}

	//矢印の座標
	sprintf(  mes , "ArrowPos.x = %f" , this->m_Fpos.x );
	DDTextOut( mes , pos.x , pos.y );
	sprintf(  mes , "ArrowPos.y = %f" , this->m_Fpos.y );
	DDTextOut( mes , pos.x , pos.y + 15 );


}


/*
************************************************************************************
プレイヤーの解放
************************************************************************************
*/
void CPlayer::ReleaseChara(){


}

