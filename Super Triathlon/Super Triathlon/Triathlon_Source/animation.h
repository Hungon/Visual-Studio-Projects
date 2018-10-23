#ifndef __ANIMATION_H__
#define __ANIMATION_H__


//▼ヘッダーのインクルード
#include "graphics.h"


//ANIMATION構造体の定義
typedef struct tagANIMATION{

	int sx;					//1コマ目のＸ
	int sy;					//１コマ目のＹ
	int max;				//最大コマ数
	int fram;				//フレーム数
	int time;				//経過時間
	int cnt;				//現在のコマ数
	int type;				//種類番号
	int type_now;			//現在の種類番号
	bool reverseF;			//反対アニメーションフラグ

}ANIMATION,*LPANIMATION;


//▼プロトタイプ宣言

//アニメーションしたいキャラクターに設定を行う
void SetAnimation(CHARACTER* ch,ANIMATION* ani,int sx,int sy,int max,int fram,int type,bool flag);
//アニメーションの更新を行う
bool UpdateAnimation(CHARACTER* ch,ANIMATION* ani);
//キャラクターのアニメーションを更新
bool UpdateReverseAnimation( CHARACTER* ch , ANIMATION* ani );


#endif
