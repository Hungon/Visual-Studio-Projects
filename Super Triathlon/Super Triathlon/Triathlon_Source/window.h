#ifndef __WINDOW_H__
#define __WINDOW_H__		//多重定義防止用


//▼マクロの定義
#define SCREEN_WIDTH	640		//ウィンドのの幅
#define SCREEN_HEIGHT	480		//ウィンドウの高さ
#define SCREEN_X		350		//ウィンドウ表示位置X
#define SCREEN_Y		120		//ウィンドウ表示位置Y

//▼ヘッダーファイルの読み込み
#include <windows.h>


//▼プロトタイプ宣言
//マウスの操作を扱う
LRESULT CALLBACK WndProc( HWND hWnd , UINT message , WPARAM wParam , LPARAM lParam );
int GetMouseX();							//現在のマウス座標Ｘを取得する関数
int GetMouseY();							//現在のマウス座標Ｙを取得する関数
bool GetDownL();							//左クリックが押されたら「true」を返す関数


//▼自作関数の定義
HWND InitWindow(char* title,int x,int y);		//ウィンドウの作成関数


#endif