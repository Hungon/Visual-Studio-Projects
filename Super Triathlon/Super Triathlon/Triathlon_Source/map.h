#ifndef __MAP_H__
#define __MAP_H__


//▼マクロの定義
#define MAP_IMAGE_MAX	3		//使用画像枚数
#define MAP_SIGN_MAX	2		//サイン数
#define STAGE_WIDTH		640		//ステージの画面の幅
#define STAGE_HEIGHT	480		//ステージの画面の高さ


//カメラの焦点
#define CAMERA_FOCUS_POSITION_X		304
#define CAMERA_FOCUS_POSITION_Y		200

//引数のキャラクターを画面の中心にセットする
#define CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(w)		( STAGE_WIDTH - w ) / 2
//引数のキャラクターを画面の中心にセットする
#define CHARACTER_CENTER_POSITION_Y_TO_STAGE_HEIGHT(h)		( STAGE_HEIGHT - h ) / 2


//音用
#define MAP_SOUND_EFFECT_MAX 3	//使用効果音の数


//▼ヘッダーインクルード
#include "graphics.h"
#include "animation.h"
#include "window.h"
#include "score.h"



//▼構造体の定義
typedef struct tagSTAGE{

	CHARACTER ch;			//キャラクター構造体
	ANIMATION ani;			//アニメーション構造体
	TIME ti;				//時間の構造体
	float movex;			//マップ移動先X
	float movey;			//マップ移動先Y
	bool scrolly;			//縦軸のスクロールフラグ
	bool scrollx;			//横軸のスクロールフラグ
	bool scroll_r;			//右スクロールフラグ
	bool scroll_l;			//左スクロールフラグ
	bool sign;				//スタート合図のフラグ
	float obj_popx;			//オブジェクト発生起点Ｘ座標
	float obj_endx;			//オブジェクト消失起点Ｘ座標
	int whole_w;			//ステージ全体の幅
	float wholeHeight;		//ステージ全体の高さ

} STAGE,*LPSTAGE;

//▼自作関数の定義
void InitMap(HWND hWnd);					//初期化
void UpdateMap();							//更新
void DrawMap();								//表示
void ReleaseMap();							//解放
//現在のステージの幅を取得
float GetStageWidth();
//現在のステージの高さを取得
float GetStageHeight();
float GetCameraX();								//カメラのＸ座標を返す関数
float GetCameraY();								//カメラのＹ座標を返す関数
void SetCamera(float cameraX,float cameraY);	//カメラの座標基点を画面の中心にする関数
//カメラの範囲を変更する
void ChangeCamera(float top , float bottom , float width , float height);
//カメラの矩形と判定を取る関数
bool CollisionCamera(CHARACTER* ch);
//スタートのサインを好きなタイミングで出せる関数
bool StartSign(bool flag);
//競技中を取得
bool GetRacing();
//ステージのクリア時間を取得する
TIME* GetStageCompleteTime();
//ステージの時間を操作する
void ControlStageTime(bool control);

#endif
