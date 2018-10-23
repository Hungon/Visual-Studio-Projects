#include "fps.h"
#include <stdio.h>
#include "window.h"


//▼グローバル変数
double g_fps = 1000.0f / 60.0f;			//計算結果を実数で扱う
double g_wait = (double)timeGetTime();	//プログラムを開いた際の初期時間を取得
int g_cnt = 0;							//経過回数計算用
double g_old = 0.0f;						//前回の経過回数用
int g_show = 0;							//表示用

//************************************************************************************************
//Fpsの表示
//************************************************************************************************
void DrawFps(int x, int y) {

	double now = 0.0;			//現在の経過時間用
	char mes[256] = { 0 };

	//処理回数を加算	
	g_cnt++;

	//現在の時間を取得
	now = (double)timeGetTime();

	//１秒間経過した確認
	if (now - g_old >= 1000) {

		//前回処理を行った時間を更新
		g_old = now;
		//ウィンドウに表示する値を更新
		g_show = g_cnt;
		//１秒間の処理回数をリセット
		g_cnt = 0;

	}
	//現在のＦＰＳを画面に出力
	wsprintf(&mes[0], "FPS = %d", g_show);
	DDTextOut(&mes[0], x, y);
}

//************************************************************************************************
//WaitFps関数
//************************************************************************************************
bool WaitFps() {

	double now = 0.0;			//現在の経過時間用

	//時間を取得
	now = (double)timeGetTime();

	//1000/60ミリ秒間ごとにtrueを返す
	if (now - g_wait >= g_fps) {
		//前回処理を行った時間を更新
		do {
			g_wait += g_fps;	//g_waitにg_fpsの値を加算し続ける

		} while (now - g_wait >= g_fps);
		//g_fpsミリ秒経過したので「true」を返す
		return true;
	}
	return false;
}
