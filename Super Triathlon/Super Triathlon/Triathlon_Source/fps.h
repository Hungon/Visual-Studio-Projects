#ifndef __FPS_H__
#define __FPS_H__


//▼ヘッダーファイルの読み込み
#include "graphics.h"


//▼自作関数を定義

//ＦＰＳを表示する関数
void DrawFps(int x,int y);
//ゲーム内の処理を1000/60ミリ秒に制限する関数
bool WaitFps();

#endif
