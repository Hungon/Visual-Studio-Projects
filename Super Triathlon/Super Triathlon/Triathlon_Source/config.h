#ifndef __CONFIG_H__
#define __CONFIG_H__

//マクロの定義
#define ACTION_MAX 3	//行動の数
#define ATTACK 0		//攻撃
#define JUMP 1		//ジャンプ
#define DUSH 2		//ダッシュ
#define BUTTON_MAX 10	//コントローラーのボタン数

#define CONFIG_IMG_MAX	1				//画像の使用枚数
#define CONFIG_SOUND_EFFECT_MAX	3		//音の使用数


#include "graphics.h"


//プロトタイプ宣言
void InitConfig( HWND hWnd );	//初期化
void UpdateConfig();		//更新
void DrawConfig();	//表示
void ReleaseConfig();		//解放
int GetConfig(int action);		//使用キーの変更

#endif