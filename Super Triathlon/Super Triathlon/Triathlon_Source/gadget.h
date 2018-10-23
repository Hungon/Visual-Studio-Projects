#ifndef __GADGET_H__
#define __GADGET_H__


//▼マクロの定義
//画像用
#define GADGET_IMAGE_MAX	3		//使用枚数
#define GADGET_SET_MAX		3		//設定用の要素数

//速度に準じた画像差し替え用
#define GADGET_SPEED_SCORE_ITALIC	100.0f		//100.0Km以上なら斜体にする

//音用

//ステータス表示用
#define GADGET_SCORE_MAX	1		//使用要素数

//▼ヘッダーをインクルード
#include "graphics.h"
#include "animation.h"

//▼構造体の定義
typedef struct tagGADGET{
	CHARACTER ch;
	ANIMATION ani;
	float indicator;	//速度メーターの針の角度を扱う
	float coordinate;	//速度メーターを現在のプレイヤーの移動量に対応させる
}GADGET , *LPGADGET;


//▼関数宣言
//初期化
void InitGadget(HWND hWnd);
//更新
void UpdateGadget();
//表示
void DrawGadget();
//解放
void ReleaseGadget();
//ガジェットの情報を渡す
GADGET* GetGadget();
//プレイヤーのステータスを反映させる
void SetStatus(float status);


#endif