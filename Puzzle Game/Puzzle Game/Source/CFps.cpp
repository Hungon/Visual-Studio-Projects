#include "CFps.h"
#include <stdio.h>
#include "window.h"

//▼ 静的メンバ変数の再定義
//処理速度を扱う
const double CFps::FPS_CONSTRAIN_TIME = 1000.0f / 60.0f;


CFps::CFps(){

	this->m_Wait = timeGetTime();			//set fps in this time
	this->m_Old = 0;			//前回処理を行った時間
	this->m_Show = 0;			//表示するFPS
	this->m_Cnt = 0;			//処理回数	
}

CFps::~CFps() {}

void CFps::DrawFps( int x , int y ){

	double now = 0.0;			//現在の経過時間用
	char mes[256] = {0};

	//処理回数を加算	
	this->m_Cnt++;

	//現在の時間を取得
	now = ( double )timeGetTime();

	//１秒間経過したか確認
	if( now - this->m_Old >= 1000 ){

		//前回処理を行った時間を更新
		this->m_Old = now;
		//ウィンドウに表示する値を更新
		this->m_Show = this->m_Cnt;
		//１秒間の処理回数をリセット
		this->m_Cnt = 0;
	
	}
	//現在のＦＰＳを画面に出力
	wsprintf( &mes[0] , "FPS = %d" , this->m_Show );
	DDTextOut( &mes[0] , x , y );
}

bool CFps::WaitFps() {

	double now = 0.0;			//現在の経過時間用

	//時間を取得
	now = (double)timeGetTime();

	//1000/60ミリ秒間ごとにtrueを返す
	if (now - this->m_Wait >= CFps::FPS_CONSTRAIN_TIME) {

		//前回処理を行った時間を更新
		do {
			this->m_Wait += CFps::FPS_CONSTRAIN_TIME;	//this->m_WaitにCFps::FPS_CONSTRAIN_TIMEの値を加算し続ける

		} while (now - this->m_Wait >= CFps::FPS_CONSTRAIN_TIME);

		//CFps::FPS_CONSTRAIN_TIMEミリ秒経過したので「true」を返す
		return true;

	}
	return false;
}
