//▼ライブラリの追加
#pragma comment(lib,"ddraw.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"msimg32.lib")


//▼ヘッダーファイルのインクルード
#include "graphics.h"
#include "window.h"
#include <math.h>

//▼グローバル変数

//プライマリフェイスの作成用
LPDIRECTDRAW7	g_lpDD = {0};	
//バックサーフェイスの作成用				
LPDIRECTDRAWSURFACE7	g_lpDDSPrimary = {0};
//クライアント領域の作成用
LPDIRECTDRAWSURFACE7	g_lpDDSBack = {0};
//クリッパーオブジェクト
LPDIRECTDRAWCLIPPER g_lpDDClipper = NULL;
//フルスクリーンモードをウィンドウモードの切り替えを扱う
int g_displayModeNow = MODE_FULLSCREEN;			//次のモード
int g_displayModeOld = MODE_FULLSCREEN;				//直前のモード
//画像を扱うサーフェイス配列
IMAGESURFACE g_img[IMAGE_MAX] = {0};
//文字体を扱う
char* g_Font = FONT_MIN;


//*****************************************************************************************
//ウィンドウモードでの初期化　InitDDWindow
//*****************************************************************************************
bool InitDDWindow(HWND hWnd){
		
	//戻り値受け取り用
	BOOL ret = {0};
	//プライマリフェイスの使用
	DDSURFACEDESC2 ddsd = {0};
	//バックサーフェイスの使用
	DDSCAPS2	ddsCaps = {0};
	//クライアント領域の調整用
	RECT rect = {0};


	//この中ウィンドウモードの初期化の処理を実装
	//▼手順１　ウィンドウスタイルの設定
	SetWindowLong(hWnd,GWL_STYLE,WS_OVERLAPPEDWINDOW);
	SetWindowLong(hWnd,GWL_EXSTYLE,0);
	
	//▼手順２　DirectDrawオブジェクトの生成
	ret = DirectDrawCreateEx(NULL , (void**)&g_lpDD , IID_IDirectDraw7 , NULL);
	//エラーチェック
	if(ret != DD_OK){
		MessageBox(NULL ,"DDオブジェクトの生成に失敗\n" , "DirectDraw" ,MB_OK);
		return false;
	}

	//▼手順３　協調レベルの設定
	ret = g_lpDD->SetCooperativeLevel(hWnd,DDSCL_NORMAL);

	//エラーチェック
	if(ret != DD_OK){
		MessageBox(NULL ,"協調レベルの設定に失敗\n" , "DirectDraw" ,MB_OK);
		return false;
	}
	
	//▼手順４　プライマリーサーフェイスの設定
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	
	//ddsの中身をもとにサーフェイスを生成
	ret = g_lpDD->CreateSurface(&ddsd,&g_lpDDSPrimary,NULL);
	
	//▼手順５　バックサーフフェイスの生成
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = SCREEN_WIDTH;
	ddsd.dwHeight = SCREEN_HEIGHT;
	//ddsdをもとにサーフェイスを生成
	ret = g_lpDD->CreateSurface(&ddsd,&g_lpDDSBack,NULL);

	//クライアント領域の設定
	SetRect( &rect , 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT );
	AdjustWindowRectEx( &rect , WS_OVERLAPPEDWINDOW , FALSE , 0 );

	//一度だけウィンドウを左上に表示し、左上に残る黒い矩形を消す
	SetWindowPos(hWnd,HWND_TOP,0,0,rect.right - rect.left,rect.bottom - rect.top,SWP_SHOWWINDOW);

	//表示位置の設定（クライアント領域の反映）
	SetWindowPos(hWnd,HWND_TOP,SCREEN_X,SCREEN_Y,
				rect.right - rect.left,
				rect.bottom - rect.top,
				SWP_SHOWWINDOW);


	//▼手順６　クリッパーオブジェクトの生成
	ret = g_lpDD->CreateClipper(0,&g_lpDDClipper,NULL);
	//エラーチェック
	if(ret != DD_OK){
		MessageBox(NULL,"クリッパーオブジェクトの読み込みに失敗","DIRECT_DRAW",MB_OK);
	}
	//ウィンドウに設定
	ret = g_lpDDClipper->SetHWnd(0,hWnd);
	//エラーチェック
	if(ret != DD_OK){
		MessageBox(NULL,"ウィンドウ設定に失敗","DIRECT_DRAW",MB_OK);
	}
	//プライマリーサーフェイスに設定
	ret = g_lpDDSPrimary->SetClipper(g_lpDDClipper);
	//エラーチェック
	if(ret != DD_OK){
		MessageBox(NULL,"プライマリーサーフェイスの設定に失敗","DIRECT_DRAW",MB_OK);
	}
	
	//初期化に成功したら現在のモードをウィンドウに
	g_displayModeNow = MODE_WINDOW;

	//初期化に成功
	return true;
}

//****************************************************************************
//DirectDraw（フルスクリーン）初期化	InitDDFullScreen
//****************************************************************************

bool InitDDFullScreen(HWND hWnd){

	//プライマリフェイスの使用
	DDSURFACEDESC2 ddsd = {0};
	//バックサーフェイスの使用
	DDSCAPS2	ddsCaps = {0};
	//クライアント領域の調整用
	RECT rect = {0};
	//戻り値受け取り用
	BOOL ret = {0};


	//手順１▼　ウィンドウスタイルの変更
	SetWindowLong(hWnd , GWL_STYLE , WS_POPUP);
	SetWindowLong(hWnd , GWL_EXSTYLE , 0);


	//手順２▼　DirectDrawオブジェクトの生成
	ret = DirectDrawCreateEx(NULL , (void**)&g_lpDD , IID_IDirectDraw7 , NULL);
	//エラーチェック
	if(ret != DD_OK){
		MessageBox(NULL ,"DDオブジェクトの生成に失敗\n" , "DirectDraw" ,MB_OK);
		return false;
	}

	//手順３▼　協調レベルの設定
	ret = g_lpDD->SetCooperativeLevel(hWnd , DDSCL_EXCLUSIVE |
			DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT);

	//エラーチェック
	if(ret != DD_OK){
		MessageBox(NULL ,"協調レベルの設定に失敗\n" , "DirectDraw" ,MB_OK);
		return false;
	}

	//手順４▼　画面モードの設定
	ret = g_lpDD->SetDisplayMode( SCREEN_WIDTH , SCREEN_HEIGHT , BPP , 0 , 0 );
	//エラーチェック
	if(ret != DD_OK){
		MessageBox(NULL ,"画面モードの設定に失敗\n" , "DirectDraw" ,MB_OK);
		return false;
	}

	//手順５▼　プライマリサーフェイス（実画面）の生成
	ZeroMemory(&ddsd , sizeof(ddsd));		//クリア
	//サイズ設定
	ddsd.dwSize = sizeof(ddsd);
	//フラグ設定
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	//バックサーフェイス数の設定
	ddsd.dwBackBufferCount = 1; 
	//バックサーフェイスの作成
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
							DDSCAPS_COMPLEX | DDSCAPS_FLIP;

	//上記の設定をプライマリサーフェイスに作成
	ret = g_lpDD->CreateSurface(&ddsd , &g_lpDDSPrimary , NULL);
	//エラーチャック
	if(ret != DD_OK){
		MessageBox(NULL ,"プライマリサーフェイスの作成に失敗\n" , "DirectDraw" ,MB_OK);
		return false;
	}

	//手順６▼　バックサーフェイス（仮想画面）の生成
	ZeroMemory(&ddsCaps , sizeof(ddsCaps));

	//サーフェイスの種類をバックに設定
	ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;

	//プライマリサーフェイスの取得
	ret = g_lpDDSPrimary->GetAttachedSurface(&ddsCaps , &g_lpDDSBack);

	//エラーチェック
	if(ret != DD_OK){
		MessageBox(NULL ,"バックサーフェイスの取得に失敗\n" , "DirectDraw" ,MB_OK);
		return false;
	}
	
	//初期化に成功したら現在のモードをフルスクリーンモードへ
	g_displayModeNow = MODE_FULLSCREEN;

	return true;
}


//***********************************************************************************
//DD全般の解放（DDオブジェクトの含む全ての解放）
//***********************************************************************************

void ReleaseDirectDraw(){

	int i = 0;
	
	//▼全てのオフスクリーンサーフェイスを解放
	for(i = 0;i < IMAGE_MAX;i++){
		if(g_img[i].surface){
			//サーフェイスを持っているなら解放
			g_img[i].surface->Release();
			g_img[i].surface = NULL;
		}
	}	
	//▼クリッパーオブジェクトの解放
	if(g_lpDDClipper){
		g_lpDDClipper->Release();
		g_lpDDClipper = NULL;
	}
	//▼バックーサーフェイスの解放
	if(g_lpDDSBack){
		g_lpDDSBack->Release();
		g_lpDDSBack = NULL;
	}
	//▼プライマリサーフェイスの解放
	if(g_lpDDSPrimary){
		g_lpDDSPrimary->Release();
		g_lpDDSPrimary = NULL;
	}
	//▼DDオブジェクトの解放
	if(g_lpDD){
		g_lpDD->Release();
		g_lpDD = NULL;
	}
}



//***************************************************************************************
//プライマリサーフェイスの切り替え（最終出力）
//***************************************************************************************

void UpdatePrimary(HWND hWnd){

	RECT dst = {0};		//転送先の矩形
	RECT src = {0};		//転送もとの矩形
	POINT pos = {0};		//ウィンドウの座標

	//▼ウィンドウモードでの最終出力
	ClientToScreen(hWnd,&pos);
	SetRect( &src , 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT );
	SetRect( &dst , pos.x , pos.y , pos.x + SCREEN_WIDTH , pos.y + SCREEN_HEIGHT );

	//エラーチェック
	if(!g_lpDDSPrimary){
		MessageBox(NULL , "フリップに失敗" , "DirectDraw" , MB_OK);
	}
	//▼現在のモードがフルスクリーンモードなら
	if( g_displayModeNow == MODE_FULLSCREEN ){
		//プライマリサーフェイスのフリップ
		g_lpDDSPrimary->Flip( NULL , DDFLIP_WAIT );
	}else{
		//転送先、元の矩形を使ってバックサーフェイスを表示
		g_lpDDSPrimary->Blt( &dst , g_lpDDSBack , &src , DDBLT_WAIT , NULL );
	}
}


//****************************************************************************************
//バックサーフェイスの塗りつぶし
//****************************************************************************************

void FillBackSurface(){

	//塗りつぶし用
	DDBLTFX		ddbltfx = {0};

	//エラーチェック
	if(!g_lpDDSBack){
		MessageBox(NULL , "バックサーフェイスの塗りつぶしに失敗\n" , "DirectDraw" ,MB_OK);
		return;
	}

	//▼塗りつぶし
	//▼サイズ指定
	ddbltfx.dwSize = sizeof(ddbltfx);
	
	//▼画面を塗りつぶす
	ddbltfx.dwFillColor = DDRGB( 0 , 0 , 0 );

	//▼色を転送
	g_lpDDSBack->Blt( NULL , NULL , NULL , DDBLT_WAIT | DDBLT_COLORFILL , &ddbltfx );

}


//*****************************************************************************************
//バックサーフェイスの取得
//*****************************************************************************************

LPDIRECTDRAWSURFACE7 GetBackSurface(){
	//エラーチェック
	if(!g_lpDDSBack){
		MessageBox(NULL , "バックサーフェイス取得失敗\n" , "DirectDraw" , MB_OK);
		return NULL;
	}
	return g_lpDDSBack;
}


//*****************************************************************************************
//スクリーンモードを変更する
//*****************************************************************************************
void ChangeDisplayMode(HWND hWnd){

	int i = 0;
	char mes[32] = {0};
	bool (*pDisplayMode[2])(HWND) = {
		InitDDFullScreen,
		InitDDWindow,
	};

	//現在のモードを解放
	ReleaseDirectDraw();

	//直前のモードを取得
	g_displayModeOld = g_displayModeNow;
	//モード切り替え
	g_displayModeNow++;
	g_displayModeNow %= 2;

	//画面のモードを交互に変更する
	pDisplayMode[g_displayModeNow]( hWnd );

	//画像の再読み込み
	for(i = 0;i < IMAGE_MAX;i++){
		if(g_img[i].flag == true){
			//i番目のパスでtrueなら再読み込み
			CreateDDImage(&g_img[i].pass[0],hWnd);
		}
	}
	//現在のモードを表示
	if(g_displayModeNow == MODE_WINDOW){
		DDTextOut("ウィンドウモード",0,50);
	}
	if(g_displayModeNow == MODE_FULLSCREEN){
		DDTextOut("フルスクリーンモード",0,50);
	}
}

//*****************************************************************************************
//画像の読み込み
//*****************************************************************************************
int CreateDDImage(char* pass,HWND hWnd){

	HBITMAP hBit = NULL;		//画像用
	BITMAP bmp = {0};			//画像情報用
	DDSURFACEDESC2 ddsd = {0};	//画像書き込み用
	int i = 0;
	HDC hDC = NULL;
	HDC memDC = NULL;
	//カラーキー
	DDCOLORKEY ddck = {0};

	//カラーキーの設定（黒を透過色にする）※
	ZeroMemory(&ddck,sizeof(ddck));
	ddck.dwColorSpaceHighValue = DDRGB( 0 , 50 , 0 );
	ddck.dwColorSpaceLowValue = DDRGB( 0 , 50 , 0 );


	//▼手順１　ビットマップ情報の取得
	hBit = (HBITMAP)LoadImage(NULL,pass ,
					IMAGE_BITMAP ,
					0 , 0 ,
					LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	//エラーチェック
	if(hBit == NULL){
		MessageBox(NULL , pass , "読み込みに失敗" , MB_OK);
		return -1;
	}
	//ビットマップ情報の取得
	GetObject(hBit , sizeof(BITMAP) , &bmp);
	
	//▼手順２　オフスクリーンサーフェイスの生成
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);	
	ddsd.dwFlags = DDSD_CAPS |
			    DDSD_WIDTH |
			    DDSD_HEIGHT;
	//サーフェイスの幅を画像と同じ
	ddsd.dwWidth = bmp.bmWidth;
	//サーフェイスの高さを画像と同じに（負の値なら反転※）
	ddsd.dwHeight = bmp.bmHeight > 0 ?
				 bmp.bmHeight :
				 bmp.bmHeight * -1;
	//種類はオフスクリーンサーフェイス
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	//空の要素にサーフェイスを生成
	for(i = 0;i < IMAGE_MAX;i++){
		if(g_img[i].surface == NULL){
			//空のi番目にサーフェイスを生成
			g_lpDD->CreateSurface(
			&ddsd , &g_img[i].surface , NULL);
				
			//透過色を指定
			g_img[i].surface->SetColorKey(
			DDCKEY_SRCBLT , &ddck);

			//▼手順３　ビットマップの書き込み
			hDC = GetDC(hWnd);
			memDC = CreateCompatibleDC(hDC);
			ReleaseDC(hWnd , hDC);
			//hBitとmemDCを関連付け
			SelectObject(memDC , hBit);
			//オフスクリーンサーフェイスのHDCを取得
			g_img[i].surface->GetDC(&hDC);
			//サーフェイスに画像を表示
			BitBlt(hDC , 0 , 0 , ddsd.dwWidth , ddsd.dwHeight , memDC , 0 , 0 , SRCCOPY);
			//サーフェイスHDCの解放
			g_img[i].surface->ReleaseDC(hDC);
			//画像のmemDCを解放
			DeleteDC(memDC);
			//画像のhBitを解放
			DeleteObject(hBit);
			//i番目にファイルパスをコピー
			strcpy(&g_img[i].pass[0] , pass);
			//使用フラグをtrueにする
			g_img[i].flag = true;
		
			//読み込んだ要素番号を返す（手順１と手順２で記述したもの）
			return i;
		}
	} 
	return -1;	//読み込み成功
}

//*****************************************************************************************
//画像の表示
//*****************************************************************************************
void DDBlt(int dst_x,int dst_y,int w,int h,int id,int src_x,int src_y){

	RECT src = {0};
	RECT dst = {0};
	//転送元矩形の生成
	SetRect(&src , src_x , src_y , src_x + w , src_y + h);
	//転送元矩形の作成
	SetRect(&dst , dst_x , dst_y , dst_x + w , dst_y + h);
	
	//作成した矩形をクリッピング
	Clipping(&dst,&src);

	//バックサーフェイスにid番のサーフェイスを表示
	g_lpDDSBack->BltFast(dst.left,dst.top,
						g_img[id].surface,&src,DDBLTFAST_WAIT | 
						DDBLTFAST_SRCCOLORKEY);
} 

//*****************************************************************************************
//文字表示
//*****************************************************************************************
void DDTextOut(char* mes,int x,int y){

	HDC hDC = NULL;
	//バックサーフェイスのHDCを取得
	g_lpDDSBack->GetDC(&hDC);
	//引数を使ってTextOut
	TextOut(hDC,x,y,mes,lstrlen(mes));
	//バックサーフェイスのhDCを解放
	g_lpDDSBack->ReleaseDC(hDC);
}

//*****************************************************************************************
//フォントでの文字表示
//*****************************************************************************************
void DDTextOutEx(char* mes,int x,int y ,int size,COLORREF color,bool Italic){

	HDC hDC = NULL;
	COLORREF defColor = 0;		//変更前の色
	HFONT hFont = NULL;			//自作のフォント
	HFONT defFont = NULL;		//変更前のフォント
	
	//バックサーフェイスのHDCを取得
	g_lpDDSBack->GetDC(&hDC);

	//フォントの生成
	hFont = CreateFont(size , 0 , 0 , 0 , FW_HEAVY , Italic , 
		false , false , SHIFTJIS_CHARSET ,
		OUT_DEFAULT_PRECIS , CLIP_DEFAULT_PRECIS , DEFAULT_QUALITY ,
		FIXED_PITCH | FF_DONTCARE , g_Font);

	//フォントをバックサーフェイスに関連付け
	defFont = (HFONT)SelectObject(hDC , hFont);

	//背景の透過
	SetBkMode(hDC , TRANSPARENT);

	//文字色の変更（黒）
	defColor = SetTextColor(hDC , color);

	//引数を使ってTextOut
	TextOut(hDC , x , y , mes , lstrlen(mes));

	//フォントの解放
	DeleteObject(hFont);

	//フォントの初期化
	SelectObject(hDC , defFont);

	//背景の透過をリセット
	SetBkMode(hDC , OPAQUE);
	
	//バックサーフェイスのhDCを解放
	g_lpDDSBack->ReleaseDC(hDC);
}

//*****************************************************************************************
//フォントの文字体を変更する
//*****************************************************************************************
void ChangeFont(char* mes){
	g_Font = mes;
}

//*****************************************************************************************
//クリッピングを行う　Clipping
//*****************************************************************************************
void Clipping( RECT *dst , RECT *src ){
	
	//左端をはみ出した
	if( dst->left < 0 ){
		//転送元の左端をはみ出たぶんだけ引く
		src->left -= dst->left;
		//転送先の左端は0にする
		dst->left = 0;
	}
	//上端のクリッピング
	if( dst->top < 0 ){
		src->top -= dst->top;
		dst->top = 0;
	}
	//右端のクリッピング
	if( dst->right > SCREEN_WIDTH ){
		src->right -= ( dst->right - SCREEN_WIDTH );
		dst->right = SCREEN_WIDTH;
	}
	//下端のクリッピング
	if( dst->bottom > SCREEN_HEIGHT){
		src->bottom -= ( dst->bottom - SCREEN_HEIGHT );
		dst->bottom = SCREEN_HEIGHT;
	}
}

//*****************************************************************************************
//画像の拡縮を行える「ロック、アンロックなし」
//*****************************************************************************************
void DDScaleBlt( float dst_x , float dst_y , int w , int h , int id , int src_x , int src_y , float pow ){

	//転送元矩形
	RECT src = { 0 };
	//転送先矩形
	RECT dst = { 0 };

	//拡大・縮小後の幅・高さを求める
	float scale_w = (float)(w * pow);
	float scale_h = (float)(h * pow);
	//画像の中心座標を求める
	float dst_center_x = (float)(w / 2);	//X座標
	float dst_center_y = (float)(h / 2);	//Y座標

	//転送元矩形を作成
	SetRect( &src , src_x , src_y , src_x + w , src_y + h );
	//拡大・縮小後の転送先矩形を作成
	//求まった中心座標を基に、表示先Ｘ・Ｙ座標を修正する
	SetRect( &dst , (int)( ( dst_x + dst_center_x ) - ( scale_w / 2 ) ) ,
		(int)( ( dst_y + dst_center_y ) - ( scale_h / 2 ) ) ,
		(int)(dst_x + dst_center_x + scale_w / 2) , 
		(int)(dst_y + dst_center_y + scale_h / 2) ); 

	//作成した矩形をクリッピング
	ClippingEx(&dst,&src,pow);

	//描画
	g_lpDDSBack -> Blt(&dst, 								//転送先
			           	g_img[id].surface, 					//画像のサーフェイス
			            &src,								//転送元
				        DDBLT_WAIT | DDBLT_KEYSRC , NULL );	//転送マクロ			
}


//*****************************************************************************************
//画像の幅だけの拡縮を行える「ロック、アンロックなし」
//*****************************************************************************************
void DDScaleOnlyWidthBlt( float dst_x , float dst_y , int w , int h , 
						 int id , int src_x , int src_y , float pow ){

	//転送元矩形
	RECT src = { 0 };
	//転送先矩形
	RECT dst = { 0 };

	//拡大・縮小後の幅・高さを求める
	float scale_w = (float)( w * pow );
	float scaleH = (float)( h * 1.0f );
	
	//転送元矩形を作成
	SetRect( &src , src_x , src_y , src_x + w , src_y + h );
	
	//拡大・縮小後の転送先矩形を作成
	SetRect( &dst , (int)dst_x , (int)dst_y ,
		(int)( dst_x + scale_w ) , (int)( dst_y + scaleH ) ); 

	//作成した矩形をクリッピング
	ClippingEx( &dst , &src , pow );

	//描画
	g_lpDDSBack -> Blt(&dst, 								//転送先
			           	g_img[id].surface, 					//画像のサーフェイス
			            &src,								//転送元
				        DDBLT_WAIT | DDBLT_KEYSRC , NULL );	//転送マクロ			
}

//*****************************************************************************************
//画像の拡縮を行える「ロック、アンロックなし」
//*****************************************************************************************
void DDScaleBltSelectIndicate( float dst_x , float dst_y , int w , int h , int id , int src_x , int src_y , float pow , char dstPointFlag ){

	//転送元矩形
	RECT src = { 0 };
	//転送先矩形
	RECT dst = { 0 };

	//拡大・縮小後の幅・高さを求める
	float scale_w = (float)( w * pow );
	float scale_h = (float)( h * pow );
	//画像の中心座標を求める
	float dst_center_x = (float)(w / 2);	//X座標
	float dst_center_y = (float)(h / 2);	//Y座標

	//転送元矩形を作成
	SetRect( &src , src_x , src_y , src_x + w , src_y + h );

	//画面左上を基点に拡大・縮小をさせる処理
	switch( dstPointFlag ){
		//左上を基点
		case DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP:
			//拡大・縮小後の転送先矩形を作成
			SetRect( &dst , (int)dst_x , (int)dst_y ,
				(int)( dst_x + scale_w ) , (int)( dst_y + scale_h ) );
			break;
		//中心を基点
		case DDSCALEBLT_SELECT_STARTING_POINT_CENTER:
			//拡大・縮小後の転送先矩形を作成
			//求まった中心座標を基に、表示先Ｘ・Ｙ座標を修正する
			SetRect( &dst , (int)( ( dst_x + dst_center_x ) - ( scale_w / 2 ) ) ,
				(int)( ( dst_y + dst_center_y ) - ( scale_h / 2 ) ) ,
				(int)(dst_x + dst_center_x + scale_w / 2) , (int)(dst_y + dst_center_y + scale_h / 2) ); 
			break;
	}

	//作成した矩形をクリッピング
	ClippingEx(&dst,&src,pow);

	//描画
	g_lpDDSBack -> Blt(&dst, 								//転送先
			           	g_img[id].surface, 					//画像のサーフェイス
			            &src,								//転送元
				        DDBLT_WAIT | DDBLT_KEYSRC , NULL );	//転送マクロ			
}


//*****************************************************************************************
//拡縮後のクリッピング
//*****************************************************************************************
void ClippingEx(RECT *pdst,RECT *psrc,float pow){
	
	//左端のクリッピング
	if(pdst->left < 0){
		psrc->left -= (int)(pdst->left/pow);
		pdst->left = 0;
	}
	//上端のクリッピング
	if(pdst->top < 0){
		psrc->top -= (int)(pdst->top/pow);
		pdst->top = 0;
	}
	//右端のクリッピング
	if(pdst->right > SCREEN_WIDTH){
		psrc->right -= (int)((pdst->right - SCREEN_WIDTH) / pow);
		pdst->right = SCREEN_WIDTH;
	}
	//下端のクリッピング
	if(pdst->bottom > SCREEN_HEIGHT){
		psrc->bottom -= (int)((pdst->bottom - SCREEN_HEIGHT) / pow);
		pdst->bottom = SCREEN_HEIGHT;
	}
}

//*****************************************************************************************
//サーフェイスロスト対策
//*****************************************************************************************
void RestoreSurface(HWND hWnd){
	int i = 0;
	//プライマリーの修復
	if(g_lpDDSPrimary){
		//消失しているかを確認
		if(g_lpDDSPrimary->IsLost() == DDERR_SURFACELOST){
			//消失していたプライマリーを修復
			g_lpDDSPrimary->Restore();
		}	
	}
	//バックの修復
	if(g_lpDDSBack){
		//消失しているかを確認
		if(g_lpDDSBack->IsLost() == DDERR_SURFACELOST){
			//消失していたバックを修復
			g_lpDDSBack->Restore();
		}	
	}
	//オフスクリーン（画像）の修復
	for(i = 0;i < IMAGE_MAX;i++){
		if(g_img[i].surface){
			//消失していないかを確認
			if(g_img[i].surface->IsLost() == DDERR_SURFACELOST){
				//オフスクリーンを解放して再読み込み
				g_img[i].surface->Release();
				g_img[i].surface = NULL;
				CreateDDImage(&g_img[i].pass[0],hWnd);
			}
		}
	}
}

//*****************************************************************************************
//現在のピクセルフォーマットを表示
//*****************************************************************************************
void DrawBpp(int x,int y){
	
	char mes[32] = {0};
	DDPIXELFORMAT ddpf = {0};
	
	//各色の2進数確認用
	DWORD rgb = DDRGB(255,15,3);

	//ピクセルフォーマットを取得
	ZeroMemory(&ddpf,sizeof(ddpf));
	ddpf.dwSize = sizeof(ddpf);
	g_lpDDSBack->GetPixelFormat(&ddpf);
	
	//現在のピクセルフォーマットを表示
	wsprintf(&mes[0],"BPP = %d",ddpf.dwRGBBitCount);
	DDTextOut(&mes[0],x,y);
	
	//赤成分のマスクの値
	wsprintf(&mes[0],"赤 = %d",ddpf.dwRBitMask);
	DDTextOut(&mes[0],x+150,y);
	wsprintf(&mes[0],"緑 = %d",ddpf.dwGBitMask);
	DDTextOut(&mes[0],x+300,y);
	wsprintf(&mes[0],"青 = %d",ddpf.dwBBitMask);
	DDTextOut(&mes[0],x+450,y);

	//rgbが持つ色を３色に分解する
	wsprintf(&mes[0],"R = %d",rgb & ddpf.dwRBitMask);
	DDTextOut(&mes[0] , x , y + 20);
	wsprintf(&mes[0],"G = %d",rgb & ddpf.dwGBitMask);
	DDTextOut(&mes[0] , x , y + 40);
	wsprintf(&mes[0],"B = %d",rgb & ddpf.dwBBitMask);
	DDTextOut(&mes[0] , x , y + 60);


}

//*****************************************************************************************
//色の合成
//*****************************************************************************************
DWORD DDRGB( DWORD r , DWORD g , DWORD b ){
	 
	DWORD dwColor = 0;				//色合成用
	DDPIXELFORMAT ddpf = {0};		//ピクセルフォーマット
	BYTE RShift = 0;				//赤のシフト値
	BYTE RMask = 0;					//赤のマスク値
	BYTE GShift = 0;				//緑のシフト値
	BYTE GMask = 0;					//緑のマスク値
	BYTE BShift = 0;				//青のシフト値
	BYTE BMask = 0;					//青のマスク値
	
	//▼ピクセルフォーマットの取得
	ZeroMemory(&ddpf,sizeof(ddpf));
	ddpf.dwSize = sizeof(ddpf);
	g_lpDDSBack->GetPixelFormat(&ddpf);
	
	//▼赤の0ビット目が０ならループ
	while(!(ddpf.dwRBitMask & 0x01)){
		//０の数（シフト値）を加算
		RShift++;
		//次のビット目を調べるため、マスクを右にシフト
		ddpf.dwRBitMask = ddpf.dwRBitMask >> 1;
	}
	//赤の0ビット目が１ならループ
	while((ddpf.dwRBitMask & 0x01)){
		//１の数を加算
		RMask++;
		//次のビット目を調べるため、マスクを右に１つシフト
		ddpf.dwRBitMask = ddpf.dwRBitMask >> 1;
	}
	//マスク値、シフト値をもとにrをdwColorに追加
	dwColor = dwColor | ((r >> (8-RMask)) << RShift);
	
	//▼緑の0ビット目が０ならループ
	while(!(ddpf.dwGBitMask & 0x01)){
		//０の数（シフト値）を加算
		GShift++;
		//次のビット目を調べるため、マスクを右にシフト
		ddpf.dwGBitMask = ddpf.dwGBitMask >> 1;
	}
	//緑の0ビット目が１ならループ
	while((ddpf.dwGBitMask & 0x01)){
		//１の数を加算
		GMask++;
		//次のビット目を調べるため、マスクを右に１つシフト
		ddpf.dwGBitMask = ddpf.dwGBitMask >> 1;
	}
	//マスク値、シフト値をもとにgをdwColorに追加
	dwColor = dwColor | ((g >> (8-GMask)) << GShift);

	//▼青の0ビット目が０ならループ
	while(!(ddpf.dwBBitMask & 0x01)){
		//０の数（シフト値）を加算
		BShift++;
		//次のビット目を調べるため、マスクを右にシフト
		ddpf.dwBBitMask = ddpf.dwBBitMask >> 1;
	}
	//青の0ビット目が１ならループ
	while((ddpf.dwBBitMask & 0x01)){
		//１の数を加算
		BMask++;
		//次のビット目を調べるため、マスクを右に１つシフト
		ddpf.dwBBitMask = ddpf.dwBBitMask >> 1;
	}
	//マスク値、シフト値をもとにbをdwColorに追加
	dwColor = dwColor | ((b >> (8-BMask)) << BShift);


	//合成完了
	return dwColor;
}

//*****************************************************************************************
//指定の画像の解放
//*****************************************************************************************
void ReleaseDDImage(int id){

	//エラーチェック
	//引数の画像の要素がマクロの値の最大枚数より多いなら
	if(id >= IMAGE_MAX){
		MessageBox(NULL ,"画像の解放に失敗\n" , "ReleaseDDImage" ,MB_OK);
	}
	//指定の要素がサーフェイスを持つなら解放
	if( g_img[id].surface ){
		g_img[id].surface->Release();
		//id番のメンバも0で初期化
		ZeroMemory(&g_img[id] , sizeof(IMAGESURFACE));
	}
}

//*****************************************************************************************
//特殊描画練習
//*****************************************************************************************
void DDRectBlt(int id,int dst_x,int dst_y,DWORD w,DWORD h,DWORD color){

	DDSURFACEDESC2 ddsd = {0};
	DWORD* pd = NULL;	//32ビットモードでのポインタ
	WORD* pw = NULL;	//16ビットモードでのポインタ
	unsigned long i = 0,j = 0;
	//ロックするサーフェイスを扱う
	LPDIRECTDRAWSURFACE7 surface = NULL;
	//カラーキー
	DDCOLORKEY ddck = {0};
	//ピクセルフォーマット用	
	DDPIXELFORMAT ddpf = {0};

	//ピクセルフォーマットを取得
	ZeroMemory(&ddpf,sizeof(ddpf));
	ddpf.dwSize = sizeof(ddpf);
	g_lpDDSBack->GetPixelFormat(&ddpf);

	//引数idをもとにロック対象を指定
	if(id >= 0){
		//idが0以上なら対応する画像をロック
		surface = g_img[id].surface;
	}else{
		//idが０より小さいなら、バックサーフェイスをロック
		surface = g_lpDDSBack;
	}

	//ｄdsdに取得するサーフェイスの設定
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	//▼手順１　サーフェイスをロック
	surface->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL);
	
	//▼手順２　先頭ポインタをコピー
	pd = (DWORD*)ddsd.lpSurface;			//32ビットモード
	pw = (WORD*)ddsd.lpSurface;				//16ビットモード

	//▼手順３　色を操作
	//32ビットモードの場合 「ウィンドウモード」
	if(ddpf.dwRGBBitCount == 32){
		for(j = 0;j < h;j++){
			for(i = 0;i < w;i++){
				//座標が範囲外なら代入を飛ばす
				if(j + dst_y < 0 ||
					j + dst_y >= ddsd.dwHeight ||
					i + dst_x < 0 ||
					i + dst_x >= ddsd.dwWidth){
					continue;
				}
				//X:0、Y:0の位置に幅16高さ32の矩形
				*(pd+(j + dst_y) * (ddsd.lPitch/4) + (i + dst_x)) = color;
			}
		}
	}
	//16ビットモードの場合 「フルスクリーンモード」
	if(ddpf.dwRGBBitCount == 16){
		for(j = 0;j < h;j++){
			for(i = 0;i < w;i++){
				//X:0、Y:0の位置に幅16高さ32の矩形
				*(pw+(j + dst_y) * (ddsd.lPitch/2) + (i + dst_x)) = (unsigned short)color;
			}
		}
	}
	//▼手順４　サーフェイスのアンロック
	surface->Unlock(NULL);
}

//*****************************************************************************************
//色の変更
//*****************************************************************************************
void DDChangeColor( int id , DWORD src_color , DWORD dst_color ){

	DDSURFACEDESC2 ddsd = {0};
	DWORD* pd = NULL;			//32ビットモードでのポインタ
	WORD* pw = NULL;			//16ビットモードでのポインタ
	unsigned long i = 0,j = 0;
	//ロックするサーフェイスを扱う
	LPDIRECTDRAWSURFACE7 surface = NULL;
	//カラーキー
	DDCOLORKEY ddck = {0};
	//ピクセルフォーマット用	
	DDPIXELFORMAT ddpf = {0};

	//ピクセルフォーマットを取得
	ZeroMemory(&ddpf,sizeof(ddpf));
	ddpf.dwSize = sizeof(ddpf);
	g_lpDDSBack->GetPixelFormat(&ddpf);

	//引数idをもとにロック対象を指定
	if(id >= 0){
		//idが0以上なら対応する画像をロック
		surface = g_img[id].surface;
	}else{
		//idが０より小さいなら、バックサーフェイスをロック
		surface = g_lpDDSBack;
	}

	//ｄdsdに取得するサーフェイスの設定
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	//▼手順１　サーフェイスをロック
	surface->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL);
	
	//▼手順２　先頭ポインタをコピー
	pd = (DWORD*)ddsd.lpSurface;			//32ビットモード
	pw = (WORD*)ddsd.lpSurface;				//16ビットモード

	//▼手順３　ロックしたいサーフェイスの色を変える
	//32ビットモード
	if(ddpf.dwRGBBitCount == 32){
		for(j = 0;j < ddsd.dwHeight;j++){
			for(i = 0;i < ddsd.dwWidth;i++){
				//X:i、Y:jの座標がsrc_colorと同じ色か調べる
				if(*(pd + (j * (ddsd.lPitch / 4)) + i) == src_color){
					//色をdst_colorが持つ色に変更
					*(pd + (j * (ddsd.lPitch / 4)) + i) = dst_color;
				}
			}
		}
	}
	//16ビットモード
	if(ddpf.dwRGBBitCount == 16){
		for(j = 0;j < ddsd.dwHeight;j++){
			for(i = 0;i < ddsd.dwWidth;i++){
				//X:i、Y:jの座標がsrc_colorと同じ色か調べる
				if(*(pw + (j * (ddsd.lPitch / 2)) + i) == src_color){
					//色をdst_colorが持つ色に変更
					*(pw + (j * (ddsd.lPitch / 2)) + i) = (WORD)dst_color;
				}
			}
		}
	}
	//▼手順４　サーフェイスのアンロック
	surface->Unlock(NULL);
}

//*****************************************************************************************
//画像の表示（ロック、アンロック有り）
//*****************************************************************************************
void DDBltEx(int dst_x,int dst_y,DWORD w,DWORD h,int id,int src_x,int src_y){

	DDSURFACEDESC2 dst = {0};		//転送先のサーフェイス
	DDSURFACEDESC2 src = {0};		//転送元のサーフェイス
	DWORD* pdst32 = NULL;			//（32ビットモード）転送先の先頭ポインタ
	DWORD* psrc32 = NULL;			//（32ビットモード）転送元の先頭ポインタ
	WORD* pdst16 = NULL;			//（16ビットモード）転送先の先頭ポインタ
	WORD* psrc16 = NULL;			//（16ビットモード）転送先の先頭ポインタ
	//ループ用
	DWORD i = 0,j = 0;
	//カラーキー
	DDCOLORKEY ddck = {0};
	//ピクセルフォーマット用	
	DDPIXELFORMAT ddpf = {0};

	//ピクセルフォーマットを取得
	ZeroMemory(&ddpf,sizeof(ddpf));
	ddpf.dwSize = sizeof(ddpf);
	g_lpDDSBack->GetPixelFormat(&ddpf);

	//▼手順１　dst、srcの設定
	//dstに取得するサーフェイスの設定
	ZeroMemory(&dst,sizeof(dst));
	dst.dwSize = sizeof(dst);
	
	//srcに取得するサーフェイスの設定
	ZeroMemory(&src,sizeof(src));
	src.dwSize = sizeof(src);

	//▼手順２　dst、srcをもとに2つのサーフェイスをロック
	g_lpDDSBack->Lock(NULL,&dst,DDLOCK_WAIT,NULL);
	g_img[id].surface->Lock(NULL,&src,DDLOCK_WAIT,NULL);

	//▼手順３　ロックした２つのサーフェイスから先頭ポインタを取得
	pdst32 = (DWORD*)dst.lpSurface;		//（32ビット）転送先
	psrc32 = (DWORD*)src.lpSurface;		//（32ビット）転送元
	pdst16 = (WORD*)dst.lpSurface;		//（16ビット）転送先
	psrc16 = (WORD*)src.lpSurface;		//（16ビット）転送元

	//▼手順４　表示するサイズ分ループを回し、転送もとの色を転送先に代入
	//32ビットモード
	if(ddpf.dwRGBBitCount == 32){
		for(j = 0;j < h;j++){
			for(i = 0;i < w;i++){
				//転送先が範囲外なら飛ばす
				if((j + dst_y) < 0 ||
					(i + dst_x) < 0 ||
					(j + dst_y) >= dst.dwHeight ||
					(i + dst_x) >= dst.dwWidth){
						continue;
				}
				//転送元が範囲外なら処理を飛ばす
				if((j + src_y) < 0 ||
					(i + src_x) < 0 ||
					(j + src_y) >= src.dwHeight ||
					(i + src_x) >= src.dwWidth){
						continue;
				}
				//透過色なら代入を飛ばす
				if(*(psrc32 + ((src_y + j) * (src.lPitch / 4)) + (src_x + i)) == DDRGB(0,50,0)){
					continue;
				}
				//転送もとの画像を1ドットずつ転送
				*(pdst32 + ((dst_y + j) * (dst.lPitch / 4)) + (dst_x + i)) = 
					*(psrc32 + ((src_y + j) * (src.lPitch / 4)) + (src_x + i));
			}
		}
	}
	//16ビットモード
	if(ddpf.dwRGBBitCount == 16){
		for(j = 0;j < h;j++){
			for(i = 0;i < w;i++){
				
				//転送先が範囲外なら飛ばす
				if((j + dst_y) < 0 ||
					(i + dst_x) < 0 ||
					(j + dst_y) >= dst.dwHeight ||
					(i + dst_x) >= dst.dwWidth){
						continue;
				}
				//転送元が範囲外なら処理を飛ばす
				if((j + src_y) < 0 ||
					(i + src_x) < 0 ||
					(j + src_y) >= src.dwHeight ||
					(i + src_x) >= src.dwWidth){
						continue;
				}
				//透過色なら代入を飛ばす
				if(*(psrc16 + ((src_y + j) * (src.lPitch / 2)) + (src_x + i)) == DDRGB(0,50,0)){
					continue;
				}
				//転送もとの画像を1ドットずつ転送
				*(pdst16 + ((dst_y + j) * (dst.lPitch / 2)) + (dst_x + i)) = 
					*(psrc16 + ((src_y + j) * (src.lPitch / 2)) + (src_x + i));
			}
		}
	}
	//▼手順５　２つのサーフェイスをアンロック
	g_img[id].surface->Unlock(NULL);
	g_lpDDSBack->Unlock(NULL);
}

//*****************************************************************************************
//拡縮を行える描画関数「ロック、アンロック有り」
//*****************************************************************************************
void DDScaleBltEx(int dst_x,int dst_y,DWORD w,DWORD h,int id,int src_x,int src_y,float pow){

	DDSURFACEDESC2 dst = {0};		//転送先のサーフェイス
	DDSURFACEDESC2 src = {0};		//転送元のサーフェイス
	DWORD* pdst32 = NULL;			//（32ビットモード）転送先の先頭ポインタ
	DWORD* psrc32 = NULL;			//（32ビットモード）転送元の先頭ポインタ
	WORD* pdst16 = NULL;			//（16ビットモード）転送先の先頭ポインタ
	WORD* psrc16 = NULL;			//（16ビットモード）転送先の先頭ポインタ
	//ループ用
	DWORD i = 0,j = 0;
	//カラーキー
	DDCOLORKEY ddck = {0};
	//ピクセルフォーマット用	
	DDPIXELFORMAT ddpf = {0};
	
	//拡縮する転送元座標の値を扱う
	DWORD ssx = 0;
	DWORD ssy = 0;
	
	//拡縮する倍率を取得する
	DWORD scale_w = (DWORD)(w * pow);
	DWORD scale_h = (DWORD)(h * pow);

	//ピクセルフォーマットを取得
	ZeroMemory(&ddpf,sizeof(ddpf));
	ddpf.dwSize = sizeof(ddpf);
	g_lpDDSBack->GetPixelFormat(&ddpf);

	//▼手順１　dst、srcの設定
	//dstに取得するサーフェイスの設定
	ZeroMemory(&dst,sizeof(dst));
	dst.dwSize = sizeof(dst);
	
	//srcに取得するサーフェイスの設定
	ZeroMemory(&src,sizeof(src));
	src.dwSize = sizeof(src);

	//▼手順２　dst、srcをもとに2つのサーフェイスをロック
	g_lpDDSBack->Lock(NULL,&dst,DDLOCK_WAIT,NULL);
	g_img[id].surface->Lock(NULL,&src,DDLOCK_WAIT,NULL);

	//▼手順３　ロックした２つのサーフェイスから先頭ポインタを取得
	pdst32 = (DWORD*)dst.lpSurface;		//（32ビット）転送先
	psrc32 = (DWORD*)src.lpSurface;		//（32ビット）転送元
	pdst16 = (WORD*)dst.lpSurface;		//（16ビット）転送先
	psrc16 = (WORD*)src.lpSurface;		//（16ビット）転送元

	//▼手順４　表示するサイズ分ループを回し、転送もとの色を転送先に代入
	//32ビットモード
	if(ddpf.dwRGBBitCount == 32){
		for(j = 0;j < scale_h;j++){
			for(i = 0;i < scale_w;i++){
				//あらかじめ転送元に倍率を掛けた値を求めていく
				ssx = (DWORD)(i / pow) + src_x;
				ssy = (DWORD)(j / pow) + src_y;

				//転送先が範囲外なら飛ばす
				if((j + dst_y) < 0 ||
					(i + dst_x) < 0 ||
					(j + dst_y) >= dst.dwHeight ||
					(i + dst_x) >= dst.dwWidth){
						continue;
				}
				//転送元が範囲外なら処理を飛ばす
				if(ssx < 0 || ssy < 0 ||
					ssy >= src.dwHeight ||
					ssx >= src.dwWidth){
						continue;
				}
				//透過色なら代入を飛ばす
				if(*(psrc32 + (ssy * (src.lPitch / 4)) + ssx) == DDRGB(0,50,0)){
					continue;
				}
				//転送もとの画像を1ドットずつ転送
				*(pdst32 + ((dst_y + j) * (dst.lPitch / 4)) + (dst_x + i)) = 
					*(psrc32 + (ssy * (src.lPitch / 4)) + ssx);
			}
		}
	}
	//16ビットモード
	if(ddpf.dwRGBBitCount == 16){
		for(j = 0;j < scale_h;j++){
			for(i = 0;i < scale_w;i++){

				//あらかじめ転送元に倍率を掛けた値を求めていく
				ssx = (DWORD)(i / pow) + src_x;
				ssy = (DWORD)(j / pow) + src_y;

				//転送先が範囲外なら飛ばす
				if((j + dst_y) < 0 ||
					(i + dst_x) < 0 ||
					(j + dst_y) >= dst.dwHeight ||
					(i + dst_x) >= dst.dwWidth){
						continue;
				}
				//転送元が範囲外なら処理を飛ばす
				if(ssx < 0 || ssy < 0 ||
					ssy >= src.dwHeight ||
					ssx >= src.dwWidth){
						continue;
				}
				//透過色なら代入を飛ばす
				if(*(psrc16 + (ssy * (src.lPitch / 2)) + ssx) == DDRGB(0,50,0)){
					continue;
				}
				//転送もとの画像を1ドットずつ転送
				*(pdst16 + ((dst_y + j) * (dst.lPitch / 2)) + (dst_x + i)) = 
					*(psrc16 + (ssy * (src.lPitch / 2)) + ssx);
			}
		}
	}
	//▼手順５　２つのサーフェイスをアンロック
	g_img[id].surface->Unlock(NULL);
	g_lpDDSBack->Unlock(NULL);
}

//*****************************************************************************************
//半透明の描画「ロック、アンロック有り」
//*****************************************************************************************
void DDAlphaBltEx(int dst_x,int dst_y,DWORD w,DWORD h,int id,int src_x,int src_y){

	DDSURFACEDESC2 dst = {0};		//転送先のサーフェイス
	DDSURFACEDESC2 src = {0};		//転送元のサーフェイス

	DWORD* pdst32 = NULL;			//（32ビットモード）転送先の先頭ポインタ
	DWORD* psrc32 = NULL;			//（32ビットモード）転送元の先頭ポインタ
	WORD* pdst16 = NULL;			//（16ビットモード）転送先の先頭ポインタ
	WORD* psrc16 = NULL;			//（16ビットモード）転送先の先頭ポインタ

	DWORD dst_color = 0;			//転送先の色
	DWORD src_color = 0;			//転送もとの色
	DWORD dst_r = 0;				//転送先の赤の成分
	DWORD src_r = 0;				//転送もとの赤の成分
	DWORD dst_g = 0;				//転送先の緑の成分
	DWORD src_g = 0;				//転送もとの緑の成分
	DWORD dst_b = 0;				//転送先の青の成分
	DWORD src_b = 0;				//転送もとの青の成分

	//ループ用
	DWORD i = 0,j = 0;
	//カラーキー
	DDCOLORKEY ddck = {0};
	//ピクセルフォーマット用	
	DDPIXELFORMAT ddpf = {0};

	//ピクセルフォーマットを取得
	ZeroMemory( &ddpf , sizeof( ddpf ) );
	ddpf.dwSize = sizeof( ddpf );
	g_lpDDSBack->GetPixelFormat( &ddpf );

	//▼手順１　dst、srcの設定
	//dstに取得するサーフェイスの設定
	ZeroMemory( &dst , sizeof( dst ) );
	dst.dwSize = sizeof( dst );
	
	//srcに取得するサーフェイスの設定
	ZeroMemory( &src , sizeof( src ) );
	src.dwSize = sizeof( src );

	//▼手順２　dst、srcをもとに2つのサーフェイスをロック
	g_lpDDSBack->Lock( NULL , &dst , DDLOCK_WAIT , NULL );
	g_img[id].surface->Lock( NULL , &src , DDLOCK_WAIT , NULL );

	//▼手順３　ロックした２つのサーフェイスから先頭ポインタを取得
	pdst32 = (DWORD*)dst.lpSurface;		//（32ビット）転送先
	psrc32 = (DWORD*)src.lpSurface;		//（32ビット）転送元
	pdst16 = (WORD*)dst.lpSurface;		//（16ビット）転送先
	psrc16 = (WORD*)src.lpSurface;		//（16ビット）転送元

	//▼手順４　表示するサイズ分ループを回し、転送もとの色を転送先に代入
	//32ビットモード
	if(ddpf.dwRGBBitCount == 32){
		for(j = 0;j < h;j++){
			for(i = 0;i < w;i++){

				//転送先が範囲外なら飛ばす
				if((j + dst_y) < 0 ||
					(i + dst_x) < 0 ||
					(j + dst_y) >= dst.dwHeight ||
					(i + dst_x) >= dst.dwWidth){
						continue;
				}
				//転送元が範囲外なら処理を飛ばす
				if((j + src_y) < 0 ||
					(i + src_x) < 0 ||
					(j + src_y) >= src.dwHeight ||
					(i + src_x) >= src.dwWidth){
						continue;
				}
				//透過色なら代入を飛ばす
				if(*(psrc32 + ((src_y + j) * (src.lPitch / 4)) + (src_x + i)) == DDRGB(0,50,0)){
					continue;
				}

				//転送先の色を取得
				dst_color = (DWORD)( *( pdst32 ) + ( j + dst_y )
				*( dst.lPitch / 4 ) ) + ( i + dst_x );
				dst_r = dst_color & ddpf.dwRBitMask;
				//転送もとの色を取得
				src_color = (DWORD)( *( psrc32 ) + ( j + src_y )
				*( src.lPitch / 4 ) ) + ( i + src_x );
				src_r = src_color & ddpf.dwRBitMask;

				//▼赤の成分
				//中間色の作成
				dst_r = (DWORD)( ( dst_r * ( 1.0f - 0.5f ) )
				+ ( src_r * 0.5f ) ) & ddpf.dwRBitMask;

				//▼緑の成分
				dst_g = dst_color & ddpf.dwGBitMask;
				src_g = src_color & ddpf.dwGBitMask;
				//中間色の作成
				dst_g = (DWORD)( ( dst_g * ( 1.0f - 0.5f ) )
				+ ( src_g * 0.5f ) ) & ddpf.dwGBitMask;

				//▼青の成分
				dst_b = dst_color & ddpf.dwBBitMask;
				src_b = src_color & ddpf.dwBBitMask;
				//中間色の作成
				dst_b = (DWORD)( ( dst_b * ( 1.0f - 0.5f ) )
				+ ( src_b * 0.5f ) ) & ddpf.dwBBitMask;

				//中間色を転送先に代入
				*( pdst32 + ( ( j + dst_y ) * ( dst.lPitch / 4 ) ) + ( i + dst_x ) ) = 
					dst_r | dst_g | dst_b;
			}
		}
	}
	//16ビットモード
	if(ddpf.dwRGBBitCount == 16){
		for(j = 0;j < h;j++){
			for(i = 0;i < w;i++){
				//転送先が範囲外なら飛ばす
				if((j + dst_y) < 0 ||
					(i + dst_x) < 0 ||
					(j + dst_y) >= dst.dwHeight ||
					(i + dst_x) >= dst.dwWidth){
						continue;
				}
				//転送元が範囲外なら処理を飛ばす
				if((j + src_y) < 0 ||
					(i + src_x) < 0 ||
					(j + src_y) >= src.dwHeight ||
					(i + src_x) >= src.dwWidth){
						continue;
				}
				//透過色なら代入を飛ばす
				if(*(psrc16 + ((src_y + j) * (src.lPitch / 2)) + (src_x + i)) == DDRGB(0,50,0)){
					continue;
				}
				//転送先の色を取得
				dst_color = (WORD)(*(pdst16) + (j + dst_y)
				*(dst.lPitch / 2)) + (i + dst_x);
				dst_r = dst_color & ddpf.dwRBitMask;
				//転送もとの色を取得
				src_color = (WORD)(*(psrc16) + (j + src_y)
				*(src.lPitch / 2)) + (i + src_x);
				src_r = src_color & ddpf.dwRBitMask;

				//▼赤の成分
				//中間色の作成
				dst_r = (WORD)((dst_r * (1.0f - 0.5f))
				+ (src_r * 0.5f)) & ddpf.dwRBitMask;

				//▼緑の成分
				dst_g = dst_color & ddpf.dwGBitMask;
				src_g = src_color & ddpf.dwGBitMask;
				//中間色の作成
				dst_g = (WORD)((dst_g * (1.0f - 0.5f))
				+ (src_g * 0.5f)) & ddpf.dwGBitMask;

				//▼青の成分
				dst_b = dst_color & ddpf.dwBBitMask;
				src_b = src_color & ddpf.dwBBitMask;
				//中間色の作成
				dst_b = (WORD)((dst_b * (1.0f - 0.5f))
				+ (src_b * 0.5f)) & ddpf.dwBBitMask;

				//中間色を転送先に代入
				*(pdst16 + ((j + dst_y) * (dst.lPitch / 2)) + (i + dst_x)) = 
					(WORD)dst_r | (WORD)dst_g | (WORD)dst_b;
			}
		}
	}
	//▼手順５　２つのサーフェイスをアンロック
	g_img[id].surface->Unlock(NULL);
	g_lpDDSBack->Unlock(NULL);
}

//*****************************************************************************************
//透明表示（ロック・アンロック無し）
//*****************************************************************************************
void DDAlphaBlt(int dst_x,int dst_y,DWORD w,DWORD h,int id,int src_x,int src_y,float alpha){

	BLENDFUNCTION bf = {0};		//透明の設定
	HDC dsthDC = NULL;			//転送先のHDC
	HDC srchDC = NULL;			//転送元のHDC
	int dummy = 0;				//空のサーフェイス用
	RECT dst = {0};
	RECT src = {0};
	
	//▼手順１　空のサーフェイス生成
	dummy = CreateDDOffScreen(w , h);

	//AlphaBlendの設定
	bf.AlphaFormat = AC_SRC_OVER;
	bf.BlendFlags = NULL;
	bf.BlendOp = NULL;
	bf.SourceConstantAlpha = (int)( 255 * alpha );	//アルファ値の設定

	//▼手順２　空のサーフェイスにバックサーフェイスの描画
	SetRect( &dst , 0 , 0 , w , h );
	SetRect( &src , dst_x , dst_y , dst_x + w , dst_y + h );
	g_img[dummy].surface->Blt( &dst , g_lpDDSBack , &src , DDBLT_WAIT , NULL );

	//▼手順３　
	SetRect( &src , src_x , src_y , src_x + w , src_y + h );
	g_img[dummy].surface->Blt(
		&dst , g_img[id].surface ,
		&src , DDBLT_WAIT | DDBLT_KEYSRC , NULL);

	
	//転送元HDCの取得（空のサーフェイスに変更）
	g_img[dummy].surface->GetDC( &srchDC );
	//転送先HDCの取得
	g_lpDDSBack->GetDC( &dsthDC );
	
	//AlphaBlendの呼び出し透明の表示
	AlphaBlend( dsthDC , dst_x , dst_y , w , h , srchDC , 0 , 0 , w , h , bf );
	
	//転送元HDCの解放（空のサーフェイスに変更）
	g_img[dummy].surface->ReleaseDC( srchDC );
	//転送先のHDCを解放
	g_lpDDSBack->ReleaseDC( dsthDC );

	//▼手順５　空のサーフェイスを解放
	ReleaseDDImage( dummy );

}

//*****************************************************************************************
//空のサーフェイスを作る
//*****************************************************************************************
int CreateDDOffScreen(int w,int h){

	DDSURFACEDESC2 ddsd = {0};
	DDCOLORKEY ddck = {0};
	int i = 0;

	//生成するサーフェイスの設定
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = w;			//幅・高さは引数の値
	ddsd.dwHeight = h;

	//透過色の指定
	ZeroMemory( &ddck , sizeof( ddck ) );
	ddck.dwColorSpaceHighValue = DDRGB( 0 , 50 , 0 );
	ddck.dwColorSpaceLowValue = DDRGB( 0 , 50 , 0 );

	//g_imgの空の要素にサーフェイスを生成
	for(i = 0;i < IMAGE_MAX;i++){
		if(g_img[i].surface == NULL){
			//設定をもとにサーフェイスを生成
			g_lpDD->CreateSurface(&ddsd,&g_img[i].surface,0);
			//透過色の設定
			g_img[i].surface->SetColorKey(DDCKEY_SRCBLT,&ddck);
			//空のサーフェイスを塗りつぶし
			FillImage(i,0,0,0);
			//生成に成功
			return i;
		}
	}
	//生成に失敗
	return -1;
}

//*****************************************************************************************
//指定サーフェイスの塗りつぶし
//*****************************************************************************************
void FillImage(int id,DWORD r,DWORD g,DWORD b){

	DDBLTFX ddbltfx = {0};
	
	//塗りつぶしの設定
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = DDRGB(r,g,b);
	//指定のサーフェイスを塗りつぶす
	g_img[id].surface->Blt(NULL,NULL,NULL,DDBLT_WAIT | DDBLT_COLORFILL,&ddbltfx);
}

//*****************************************************************************************
//回転を行える描画（ロック、アンロック有り）
//*****************************************************************************************
void DDRotateBltEx(int dst_x ,int dst_y,int w,int h,int id,int src_x,int src_y,int angle){
	
	DDSURFACEDESC2 ddst = {0};		//転送先のサーフェイス
	DDSURFACEDESC2 dsrc = {0};		//転送元のサーフェイス
	DWORD* pdst32 = NULL;			//（32ビットモード）転送先の先頭ポインタ
	DWORD* psrc32 = NULL;			//（32ビットモード）転送元の先頭ポインタ
	WORD* pdst16 = NULL;			//（16ビットモード）転送先の先頭ポインタ
	WORD* psrc16 = NULL;			//（16ビットモード）転送先の先頭ポインタ
	//ループ用
	DWORD i = 0,j = 0;
	//カラーキー
	DDCOLORKEY ddck = {0};
	//ピクセルフォーマット用	
	DDPIXELFORMAT ddpf = {0};
	
	//空のサーフェイス用
	int dummy = 0;
	//大きいほうを1.5倍に
	int size = w > h ? (int)(w * 1.5f) : (int)(h * 1.5f);
	RECT src = {0};
	//回転の中心をサーフェイスの中心に
	int rotate_x = 0;			//回転後の転送元x座標
	int rotate_y = 0;			//回転後の転送y座標
	int cx = size / 2;			//回転の中心x座標
	int cy = size / 2;			//回転の中心y座標
	
	//空のサーフェイスを生成
	dummy = CreateDDOffScreen(size,size);
	//転送元矩形の作成
	SetRect(&src , src_x , src_y , src_x + w , src_y + h);
	//空のサーフェイスの中心に回転する画像を表示
	g_img[dummy].surface->BltFast((size / 2 - w / 2) , (size / 2 - h / 2),
	g_img[id].surface , &src , DDBLTFAST_WAIT);

	//ピクセルフォーマットを取得
	ZeroMemory(&ddpf,sizeof(ddpf));
	ddpf.dwSize = sizeof(ddpf);
	g_lpDDSBack->GetPixelFormat(&ddpf);

	//▼手順１　dst、srcの設定
	//dstに取得するサーフェイスの設定
	ZeroMemory(&ddst,sizeof(ddst));
	ddst.dwSize = sizeof(ddst);
	
	//srcに取得するサーフェイスの設定
	ZeroMemory(&dsrc,sizeof(dsrc));
	dsrc.dwSize = sizeof(dsrc);

	//▼手順２　dst、srcをもとに2つのサーフェイスをロック
	g_lpDDSBack->Lock(NULL,&ddst,DDLOCK_WAIT,NULL);
	g_img[dummy].surface->Lock(NULL,&dsrc,DDLOCK_WAIT,NULL);

	//▼手順３　ロックした２つのサーフェイスから先頭ポインタを取得
	pdst32 = (DWORD*)ddst.lpSurface;		//（32ビット）転送先
	psrc32 = (DWORD*)dsrc.lpSurface;		//（32ビット）転送元
	pdst16 = (WORD*)ddst.lpSurface;		//（16ビット）転送先
	psrc16 = (WORD*)dsrc.lpSurface;		//（16ビット）転送元

	//▼手順４　表示するサイズ分ループを回し、転送もとの色を転送先に代入
	//32ビットモード
	if(ddpf.dwRGBBitCount == 32){
		for(int j = 0;j < size;j++){
			for(int i = 0;i < size;i++){
				//転送先が範囲外なら飛ばす
				if((j + dst_y) < 0 ||
					(i + dst_x) < 0 ||
					(j + dst_y) >= (int)ddst.dwHeight ||
					(i + dst_x) >= (int)ddst.dwWidth){
						continue;
				}
				//転送元が範囲外なら処理を飛ばす
				if((j + src_y) < 0 ||
					(i + src_x) < 0 ||
					(j + src_y) >= (int)dsrc.dwHeight ||
					(i + src_x) >= (int)dsrc.dwWidth){
						continue;
				}
				//転送先座標から回転後の転送元を算出
				rotate_x = (int)( ( i - cx ) * cos( angle * 3.14 / 180.0 )
								- ( j - cy ) * sin( angle * 3.14 / 180.0 ) + cx );
				rotate_y = (int)( ( i - cx ) * sin( angle * 3.14 / 180.0 ) 
								+ ( j - cy ) * cos( angle * 3.14 / 180.0 ) + cy );
				//転送元が範囲外なら飛ばす
				if( rotate_y < 0 || rotate_y >= ( int )dsrc.dwHeight ||
					rotate_x < 0 || rotate_x >= ( int )dsrc.dwWidth ){
					continue;
				}

				//転送元が透過色なら飛ばす
				if( *( psrc32+(rotate_y)*(dsrc.lPitch/4)+(rotate_x) ) == DDRGB(0,50,0)){
					continue;
				}	
				//転送先に色を代入				
				*(pdst32+(j + (int)dst_y - (size / 2 - h / 2)) * (ddst.lPitch/4) + 
					(i + (int)dst_x - (size / 2 - w / 2))) = 
							*( psrc32 + (rotate_y) * (dsrc.lPitch / 4) + (rotate_x));
			}
		}
	}
	//16ビットモード
	if(ddpf.dwRGBBitCount == 16){
		for(j = 0;j < (WORD)size;j++){
			for(i = 0;i < (WORD)size;i++){
				
				//転送先が範囲外なら飛ばす
				if((j + dst_y) < 0 ||
					(i + dst_x) < 0 ||
					(j + dst_y) >= ddst.dwHeight ||
					(i + dst_x) >= ddst.dwWidth){
						continue;
				}
				//転送元が範囲外なら処理を飛ばす
				if((j + src_y) < 0 ||
					(i + src_x) < 0 ||
					(j + src_y) >= dsrc.dwHeight ||
					(i + src_x) >= dsrc.dwWidth){
						continue;
				}
				//転送先座標から回転後の転送元を算出
				rotate_x = (int)( ( i - cx ) * cos( angle * 3.14 / 180.0 )
								- ( j - cy ) * sin( angle * 3.14 / 180.0 ) + cx );
				rotate_y = (int)( ( i - cx ) * sin( angle * 3.14 / 180.0 ) 
								+ ( j - cy ) * cos( angle * 3.14 / 180.0 ) + cy );
				//転送元が範囲外なら飛ばす
				if( rotate_y < 0 || rotate_y >= ( int )dsrc.dwHeight ||
					rotate_x < 0 || rotate_x >= ( int )dsrc.dwWidth ){
					continue;
				}
				//透過色なら代入を飛ばす
				if(*(psrc16 + ((src_y + j) * (dsrc.lPitch / 2)) + (src_x + i)) == DDRGB(0,50,0)){
					continue;
				}
				//転送先に色を代入				
				*(pdst16+(j + (int)dst_y - (size / 2 - h / 2)) * (ddst.lPitch / 2) + 
					(i + (int)dst_x - (size / 2 - w / 2))) = 
							*( psrc16 + (rotate_y) * (dsrc.lPitch / 2) + (rotate_x));
			}
		}
	}
	//▼手順５　２つのサーフェイスをアンロック
	g_img[dummy].surface->Unlock(NULL);
	g_lpDDSBack->Unlock(NULL);

	//空のサーフェイスの開放
	ReleaseDDImage(dummy);
}

//*****************************************************************************************
//色との判定を取る関数（ロック、アンロック有り）
//*****************************************************************************************
bool ColisionDDRGB(CHARACTER* ch,int id,DWORD dst_color,DWORD src_color){

	DDSURFACEDESC2 ddst = {0};		//転送先のサーフェイス
	DDSURFACEDESC2 dsrc = {0};		//転送元のサーフェイス
	DWORD* pdst32 = NULL;			//（32ビットモード）転送先の先頭ポインタ
	DWORD* psrc32 = NULL;			//（32ビットモード）転送元の先頭ポインタ
	WORD* pdst16 = NULL;			//（16ビットモード）転送先の先頭ポインタ
	WORD* psrc16 = NULL;			//（16ビットモード）転送先の先頭ポインタ
	//ループ用
	int i = 0,j = 0;
	//カラーキー
	DDCOLORKEY ddck = {0};
	//ピクセルフォーマット用	
	DDPIXELFORMAT ddpf = {0};
	//戻り値用
	bool ret = false;
	//転送先の座標計算用
	int dx = 0,dy = 0;
	//転送元の座標計算用
	int sx = 0,sy = 0;
		
	//ピクセルフォーマットを取得
	ZeroMemory(&ddpf,sizeof(ddpf));
	ddpf.dwSize = sizeof(ddpf);
	g_lpDDSBack->GetPixelFormat(&ddpf);

	//▼手順１　dst、srcの設定
	//dstに取得するサーフェイスの設定
	ZeroMemory(&ddst,sizeof(ddst));
	ddst.dwSize = sizeof(ddst);
	
	//srcに取得するサーフェイスの設定
	ZeroMemory(&dsrc,sizeof(dsrc));
	dsrc.dwSize = sizeof(dsrc);

	//▼手順２　dst、srcをもとに2つのサーフェイスをロック
	g_lpDDSBack->Lock(NULL,&ddst,DDLOCK_WAIT,NULL);
	g_img[id].surface->Lock(NULL,&dsrc,DDLOCK_WAIT,NULL);

	//▼手順３　ロックした２つのサーフェイスから先頭ポインタを取得
	pdst32 = (DWORD*)ddst.lpSurface;		//（32ビット）転送先
	psrc32 = (DWORD*)dsrc.lpSurface;		//（32ビット）転送元
	pdst16 = (WORD*)ddst.lpSurface;			//（16ビット）転送先
	psrc16 = (WORD*)dsrc.lpSurface;			//（16ビット）転送元

	//▼手順４　表示するサイズ分ループを回し、転送もとの色を転送先に代入
	//32ビットモード
	if(ddpf.dwRGBBitCount == 32){
		for(j = 0;j < ch->h;j++){
			for(i = 0;i < ch->w;i++){
				//ループ内で使用する座標を計算
				dx = (int)(i + ch->x); 
				dy = (int)(j + ch->y); 
				sx = (int)(i + ch->picx);
				sy = (int)(j + ch->picy); 
				//転送先が範囲外なら飛ばす
				if(dy < 0 || dx < 0 ||
					dy >= (int)ddst.dwHeight || dx >= (int)ddst.dwWidth){
						continue;
				}
				//転送元が範囲外なら処理を飛ばす
				if(sy < 0 || sx < 0 ||
					sy >= (int)dsrc.dwHeight || sx >= (int)dsrc.dwWidth){
						continue;
				}
				//透過色なら代入を飛ばす
				if(*(psrc32 + (sy * (dsrc.lPitch / 4)) + sx) == DDRGB(0,50,0)){
					continue;
				}
				//判定元のキャラクターと引数で指定した色との判定
				if(*(psrc32 + (sy * (dsrc.lPitch / 4)) + sx) == src_color &&
					*(pdst32 + (dy * (ddst.lPitch / 4)) + dx) == dst_color){
					//当たったらtrueを返す
					ret = true;
					break;	//当たり判定が見つかったらループ終了
				}
			}
		}
	}	
	//▼手順５　２つのサーフェイスをアンロック
	g_img[id].surface->Unlock(NULL);
	g_lpDDSBack->Unlock(NULL);
	//当たっていなかったらfalseを返す
	return ret;
}

//***********************************************************************************************
//線の描画
//***********************************************************************************************
void DDLine(){
		HDC hDC = NULL;
		//バックサーフェイスのHDCを取得
		g_lpDDSBack->GetDC( &hDC );
		//始点設定
		MoveToEx( hDC , 0 , 0 , NULL );
		//始点から終点までを線でつなぐ
		LineTo( hDC , 320 , 240 );
		//バックサーフェイスのHDCを開放
		g_lpDDSBack->ReleaseDC( hDC );
}

//***********************************************************************************************
//多角形の描画
//***********************************************************************************************
void DDPolygon(POINT *p,int size){

	HDC hDC = NULL;
	//バックサーフェイスのＨＤＣを取得
	g_lpDDSBack->GetDC(&hDC);
	//引数の頂点で多角形の描画
	Polygon(hDC , p , size);
	//バックサーフェイスのＨＤＣを解放
	g_lpDDSBack->ReleaseDC(hDC);
}

//***********************************************************************************************
//円の描画
//***********************************************************************************************
void DDCircle(float cx,float cy,int len){

	HDC hDC = NULL;
	//バックサーフェイスのＨＤＣを取得
	g_lpDDSBack->GetDC(&hDC);
	//円を表示
	Ellipse(hDC ,(int)cx - (int)len ,(int)cy - (int)len ,(int)cx + (int)len ,(int)cy + (int)len);
	//バックサーフェイスのＨＤＣを解放
	g_lpDDSBack->ReleaseDC(hDC);
}
