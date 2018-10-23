#ifndef __GARAPHICS_H__
#define __GARAPHICS_H__

//▼マクロの定義
#define BPP 16				//カラーモード１６bIT
#define MODE_FULLSCREEN 0	//フルスクリーンモード
#define MODE_WINDOW 1		//ウィンドウモード
#define IMAGE_MAX	50		//画像の枚数
#define LOCK_BACK	-1		//バックサーフェイスのロック指定

//フォントの字体
#define FONT_MIN	"ＭＳ 明朝"
#define FONT_GOS	"ＭＳ ゴシック"

//DDScaleBltSelectIndicate
//Drawing starting point flag
#define DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP	0
#define DDSCALEBLT_SELECT_STARTING_POINT_CENTER		1

//▼ヘッダーファイルのインクルード
#include <windows.h>
#include <ddraw.h>
#include "character.h"

//▼構造体の定義
typedef struct tagIMAGESURFACE{

	LPDIRECTDRAWSURFACE7 surface;	//画像を書き込むオフスクリーンサーフェイス
	char pass[32];					//ファイルパス
	bool flag;						//使用フラグ

}IMAGESURFACE,*LPIMAGESURFACE;


//▼関数のプロトタイプ宣言
bool InitDDFullScreen(HWND hWnd);		//フルスクリーンの初期化
void ReleaseDirectDraw();				//解放
void UpdatePrimary(HWND hWnd);			//最終出力
void FillBackSurface();					//バックサーフェイスの塗りつぶし
LPDIRECTDRAWSURFACE7 GetBackSurface();	//バックサーフェイスの取得

//ウィンドウモードでの初期化
bool InitDDWindow(HWND hWnd);

//スクリーンのモードを変更する
void ChangeDisplayMode(HWND hWnd);

//画像を読み込む
int CreateDDImage(char* pass,HWND hWnd);

//画像を表示する
void DDBlt(int dst_x,int dst_y,int w,int h,int id,int src_x,int src_y);

//文字表示
void DDTextOut(char* mes,int x,int y);

//フォントでの文字表示
void DDTextOutEx(char* mes ,int x, int y,int size,COLORREF color,bool Italic);

//フォントの文字体を変更する
void ChangeFont(char* mes);

//クリッピングを行う
void Clipping(RECT* dst,RECT* src);	

//画像の拡縮を行う
void DDScaleBlt(float dst_x,float dst_y,int w,int h,int id,int src_x,int src_y,float pow);

//画像の拡縮を行える「表示先座標変更可能」
void DDScaleBltSelectIndicate( float dst_x , float dst_y , int w , int h , int id , int src_x , int src_y , float pow , char dstPointFlag );

//画像の幅だけの拡縮を行える「ロック、アンロックなし」
void DDScaleOnlyWidthBlt( float dst_x , float dst_y , int w , int h , int id , 
						 int src_x , int src_y , float pow );
//拡縮後クリッピング
void ClippingEx(RECT* pdst,RECT* psrc,float pow);

//サーフェイスロストの対策
void RestoreSurface(HWND hWnd);

//現在のピクセルフォーマットを取得し表示する
void DrawBpp(int x,int y);

//色合成
DWORD DDRGB( DWORD r, DWORD g, DWORD b);

//指定の画像を解放する
void ReleaseDDImage(int id);

//特殊描画練習
void DDRectBlt(int id,int dst_x,int dst_y,DWORD w,DWORD h,DWORD color);		

//画像を指定の色に変更
void DDChangeColor(int id,DWORD src_color,DWORD dst_color);

//色を指定して描画
void DDBltEx(int dst_x,int dst_y,DWORD w,DWORD h,int id,int src_x,int src_y);

//拡縮を行える描画関数
void DDScaleBltEx(int dst_x,int dst_y,DWORD w,DWORD h,int id,int src_x,int src_y,float pow);

//半透明の描画を行える関数「ロック、アンロック有り」
void DDAlphaBltEx(int dst_x,int dst_y,DWORD w,DWORD h,int id,int src_x,int src_y);

//半透明の描画「ロック、アンロックなし」
void DDAlphaBlt(int dst_x,int dst_y,DWORD w,DWORD h,int id,int src_x,int src_y,float alpha);

//空のサーフェイスを生成
int CreateDDOffScreen(int w,int h);

//指定サーフェイスの塗りつぶし
void FillImage(int id,DWORD r,DWORD g,DWORD b);

//回転の描画（ロック、アンロック有り）
void DDRotateBltEx(int dst_x ,int dst_y,int w,int h,int id,int src_x,int src_y,int angle);

//色との判定を取る関数（ロック、アンロック有り）
bool ColisionDDRGB(CHARACTER* ch,int id,DWORD dst_color,DWORD src_color);

//線の描画
void DDLine();

//多角形の描画
void DDPolygon(POINT *p,int size);

//円の描画
void DDCircle(float cx, float cy,int len);

#endif

