#ifndef __WIPE_H__
#define __WIPE_H__

//▼マクロの定義
//画面遷移
//列挙型
enum tagWIPE_TRANSITION{

	NOT_TRANSITION = -1 ,	//シーン変更しない時
	CHANGE_SCENE ,			//シーン切り替え
	CHANGE_STAGE ,			//ステージ切り替え
	RESTART_RACE ,			//スタートから始める
	TO_TITLE	 ,			//タイトルへ
	TO_SELECT	 ,			//ステージセレクトへ

};

//画像
//ワイプ
#define WIPE_IMAGE_MAX		4		//画像最大枚数
#define WIPE_MAX			6		//ワイプ使用枚数
//ロード中イメージ
#define LOAD_IMAGE_MAX		1		//最大枚数

//ワイプ演出時の可変値
//透過演出
#define WIPE_DEFAULT_ALPHA_ADD		50.0f		//加算値
#define WIPE_DEFAULT_ALPHA_SUB		-50.0f		//減算値
//拡縮演出
#define WIPE_DEFAULT_SCALE_SPEED		0.020f	//加算値
#define WIPE_DEFAULT_CONTRACTION_SPEED	-0.020f	//減算値
//stay time
const int WIPE_DEFAULT_STAY_TIME = 100;

//ワイプ演出の列挙型
enum tagWIPE_DIRECT{

	WIPE_FEEDOUT	 ,	//透過演出
	WIPE_CONTRACTION ,	//画面中心へ収縮する演出
	WIPE_DIRECT_MAX  ,	//演出の数

};
//▼ヘッダーをインクルード
#include "graphics.h"
#include "animation.h"
#include "game.h"

//▼構造体を定義
typedef struct tagWIPE{

	CHARACTER ch;				//キャラクター構造体
	ANIMATION ani;				//アニメーション構造体
	tagGAMETRASITION next;		//次のシーン番号
	tagWIPE_TRANSITION type;	//ワイプ種類
	int stay;					//ワイプをとどめる時間
	float alpha;				//アルファ値
	float add_alpha;			//アルファ加算値
	float sub_alpha;			//アルファ減算値
	float contraction_speed;	//縮小速度
	float scale_speed;			//拡大速度

}WIPE,*LPWIPE;

//▼プロトタイプ宣言
void InitWipe(HWND hWnd);//初期化
bool UpdateWipe();		//戻り値に生存フラグを返す
void DrawWipe();		//表示
void ReleaseWipe();		//解放
//ワイプ生成
void CreateWipe(tagGAMETRASITION next , tagWIPE_TRANSITION type , tagWIPE_DIRECT direct);
//ワイプ処理（透過演出）
void DirectWipe0();
//ワイプ処理（収縮演出）
void DirectWipe1();


#endif