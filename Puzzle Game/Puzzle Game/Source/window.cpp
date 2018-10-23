//▼ライブラリの追加
#pragma comment(lib,"winmm.lib")

//▼ヘッダインクルード
#include <windows.h>
#include <stdio.h.>
#include "window.h"
#include "resource.h"

//▼グローバル変数
int g_mouseX = 0;			//マウスＸ座標
int g_mouseY = 0;			//マウスＹ座標
bool g_downL = false;		//マウス左クリックフラグ

//***********************************************************************
//InitWindow関数
//[機能]		ウィンドウを作成し表示する
//[引数]		なし
//[戻り値]		ウィンドウハンドル
//***********************************************************************

//ウィンドウクラスの初期設定
HWND InitWindow(char* title,int x,int y){

	WNDCLASSEX wcex ={0};									//ウィンドウクラス設定用の構造体
	HINSTANCE hInstance = GetModuleHandle(NULL);			//インスタンスハンドル
	HWND hWnd = NULL;										//ウィンドウ受け取り
	RECT WindowRect = {								

	0,0,SCREEN_WIDTH,SCREEN_HEIGHT						//ウィンドウの大きさ

	};

	char classname[32] = "Window_Sample";					//ウィンドウクラス名

	//▼手順１　ウィンドウクラスの設定
	wcex.cbSize = sizeof(wcex);																								//自身のサイズ
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;	//ウィンドウクラスのスタイル
	wcex.lpfnWndProc = (WNDPROC)WndProc; 																		//ウィンドウプロシージャ
	wcex.cbClsExtra = 0;																															//拡張用のパラメーター
	wcex.cbWndExtra = 0;																															//拡張用のパラメーター
	wcex.hInstance = hInstance;																								//インスタンスハンドル

	//アイコンのハンドル
	wcex.hIcon = LoadIcon(
				hInstance ,
				MAKEINTRESOURCE(IDI_ICON1));	
	//カーソルのハンドル
	wcex.hCursor = (HCURSOR)LoadImage(
	NULL,
	IDC_ARROW,
	IMAGE_CURSOR,
	0,0,
	LR_DEFAULTSIZE|LR_SHARED);

	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		//バックカラー設定
	wcex.lpszMenuName = NULL;																																		//クラスのメニュー
	wcex.lpszClassName = &classname[0];																								//ウィンドウクラス名
	wcex.hIconSm = wcex.hIcon;																																	//スモールアイコンのハンドル


		//▼手順２　ウィンドウクラスの登録
		if(!RegisterClassEx(&wcex)){

			//戻り値が０なら登録失敗、returnで終了させる
			printf("ウィンドウクラスの登録失敗\n");

			return NULL;

		}

	//クライアント領域と枠のサイズを含めたウィンドウの幅と高さを求める
	AdjustWindowRectEx(&WindowRect,WS_OVERLAPPEDWINDOW,FALSE,NULL);


	//▼手順３　ウィンドウクラスを基にウィンドウを作成
	hWnd=CreateWindowEx(
		0,																//拡張ウィンドウのスタイル
		&classname[0],													//ウィンドウクラス名
		title,															//ウィンドウタイトル
		WS_OVERLAPPEDWINDOW,											//ウィンドウスタイル
		x,																//ウィンドウの左上X座標
		y,																//ウィンドウの左上Y座標
		WindowRect.right-WindowRect.left,								//ウィンドウの幅
		WindowRect.bottom-WindowRect.top,								//ウィンドウの高さ
		NULL,															//親ウィンドウのハンドル
		NULL,															//メニューのハンドル
		hInstance,														//インスタンスハンドル
		NULL															//拡張用のパラメーター

	);


		//ウィンドウ作成のエラーチェック
		if(hWnd == NULL){

			printf("ウィンドウ作成できず\n");

			return NULL;

		}


	//▼手順４　作成したウィンドウの表示
	ShowWindow(hWnd,SW_SHOW);


	//ウィンドウ領域の更新
	UpdateWindow(hWnd);

	return hWnd;

}

//************************************************************************************************
//マウスの感知
//************************************************************************************************
LRESULT CALLBACK WndProc( HWND hWnd , UINT message , WPARAM wParam , LPARAM lParam ){

	//座標表示用
	char mes[256] = {0};

	HDC hDC = NULL;

	//メッセージごとの処理
	switch( message ){

		//マウスの移動を感知
		case WM_MOUSEMOVE:

			//座標の取得
			g_mouseX = lParam & 0x0000FFFF;
			g_mouseY = lParam >> 16;

		break;

		//左クリックを押した時の処理
		case WM_LBUTTONDOWN:		

			//左クリックを押した瞬間は「true」
			g_downL = true;
			break;

		//右クリックが押された時の処理
		case WM_RBUTTONDOWN:		

			break;

		//左クリックを離した時の処理
		case WM_LBUTTONUP:

			//左クリックが離されている間は左クリックフラグを「false」
			g_downL = false;

			break;

		//右クリックを離した時の処理
		case WM_RBUTTONUP:

			break;

		
		default:
			//DefWindowProプロシージャに処理を任せる
			return DefWindowProc( hWnd , message , wParam , lParam );
	}
	return 0;
}

//************************************************************************************************
//マウスのX座標を取得
//************************************************************************************************
int GetMouseX() { return g_mouseX; }

//************************************************************************************************
//マウスのY座標を取得
//************************************************************************************************
int GetMouseY() { return g_mouseY; }

//************************************************************************************************
//左クリックフラグを返す関数
//************************************************************************************************
bool GetDownL() { return g_downL; }