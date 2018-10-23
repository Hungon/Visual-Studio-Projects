#ifndef __EFFECT_H__
#define __EFFECT_H__

//▼マクロの定義
#define EFFECT_MAX 10		//エフェクトの種類数

//▼ヘッダーをインクルード
#include "graphics.h"
#include "animation.h"


//▼構造体の定義
typedef struct tagEFFECT{

	CHARACTER ef;	
	ANIMATION ani;
	int type;
	
} EFFECT,*LPEFFECT;


//▼関数の定義
void InitEffect(HWND hWnd);		//初期化
void UpdateEffect();			//更新
void DrawEffect();		//表示
void ReleaseEffect();			//解放
void CreateEffect(float x,float y);		//エフェクトを指定の座標に発生させる
//ステージ変更時のエフェクトの更新
void ResetEffect();

#endif
