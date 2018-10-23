#ifndef __SEASTAGE_H__
#define __SEASTAGE_H__


//▼マクロの定義
//画像
#define SEA_MAP_IMAGE_MAX	9			//使用画像枚数
const int SEASTAGE_BACKGROUND_MAX = 3;	//背景の数
#define SEA_OBJECT_MAX	1				//オブジェクトの数
#define SEA_SIGN_MAX	1				//サイン表示の数
//効果音
#define SEA_EFFECT_SOUND_MAX	3		//使用効果音の数
//ミニマップ画像枚数
#define SEA_MINIMAP_KIMD_OF_IMAGE	4
//indication setting
//back ground
const int SEA_MINIMAP_IMAGE_BACK_GROUND_WIDTH = 500;
const int SEA_MINIMAP_IMAGE_BACK_GROUND_HEIGHT = 40;
const float SEA_MINIMAP_IMAGE_BACK_GROUND_INDICATE_X = 135.0f;
const float SEA_MINIMAP_IMAGE_BACK_GROUND_INDICATE_Y = 425.0f;
//minimum swimer
const float	SEA_MINIMAP_IMAGE_SWIMER_SCALE_RATE = 0.5f;
//flags
const int	SEA_MINIMAP_IMAGE_FLAG_WIDTH = 40;
const int	SEA_MINIMAP_IMAGE_FLAG_HEIGHT = 40;

//▼ヘッダーインクルード
#include "map.h"

//▼自作関数の定義
//海ステージの初期化
void InitSea(HWND hWnd);
//海の更新
void UpdateSea();
//道路の更新
void UpdateRoad();
//海ステージの表示
void DrawSea();
//海ステージの解放
void ReleaseSea();
//海ステージの情報を取得できる
STAGE* GetSeaStage();

#endif
