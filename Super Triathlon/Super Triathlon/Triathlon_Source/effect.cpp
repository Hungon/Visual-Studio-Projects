#include "effect.h"
#include "map.h"
#include <stdio.h>



//▼グローバル変数
EFFECT g_effect[EFFECT_MAX] = {0};
int g_effectImg[1] = {0};

//************************************************************************************************
//▼初期化
//************************************************************************************************
void InitEffect(HWND hWnd){

	int i = 0;
	
	//エフェクト画像を読み込む
	g_effectImg[0] = CreateDDImage("Image\\Sea\\bubble.bmp",hWnd);
	
	//エフェクト画像の初期設定
	for(i = 0;i < EFFECT_MAX;i++){

		g_effect[i].ef.flag = false;
		g_effect[i].ef.h = 32;
		g_effect[i].ef.w = 32;
		g_effect[i].ef.scale = 1.0f;	//拡縮倍率

		//アニメーション設定
		g_effect[i].ani.fram = 10;		//フレーム数
		g_effect[i].ani.max = 5;		//最大コマ数


		SetAnimation(&g_effect[i].ef,&g_effect[i].ani,
			g_effect[i].ani.sx,g_effect[i].ani.sy,
			g_effect[i].ani.max,g_effect[i].ani.fram,
			g_effect[i].type,false);
	}
}

//************************************************************************************************
//更新
//************************************************************************************************
void UpdateEffect(){

	int i = 0;

	for(i = 0;i < EFFECT_MAX;i++){
		//エフェクトの生存フラグがtrueならアニメーション
		if(g_effect[i].ef.flag == true){	
			//エフェクトの生存フラグにアニメーションの更新フラグを代入
			g_effect[i].ef.flag = UpdateAnimation(&g_effect[i].ef,&g_effect[i].ani);
		}
	}
}

//************************************************************************************************
//表示
//************************************************************************************************
void DrawEffect(){

	int i = 0;

	float px = GetCameraX();						
	float py = GetCameraY();						

	for(i = 0;i < EFFECT_MAX;i++){
		//エフェクトの生存フラグがtrueなら表示
		if(g_effect[i].ef.flag == true){			
			//エフェクト画像
			DDBlt(
				(int)( g_effect[i].ef.x - px ) ,//X座標
				(int)( g_effect[i].ef.y - py ) ,//Y座標
				g_effect[i].ef.w,				//幅
				g_effect[i].ef.h,				//高さ
				g_effectImg[0],					//対応した画像の番号
				g_effect[i].ef.picx,			//表示元Ｘ座標
				g_effect[i].ef.picy);			//表示元Ｙ座標		
		}
	}
}

//************************************************************************************************
//解放
//************************************************************************************************
void ReleaseEffect(){
	ReleaseDDImage(g_effectImg[0]);
}

//************************************************************************************************
//エフェクトを発生させる関数
//************************************************************************************************
void CreateEffect(float x,float y){

	int i = 0;

	for(i = 0;i < EFFECT_MAX;i++){
		//エネミーの生存フラグがfalseなら処理を行う
		if(g_effect[i].ef.flag == false){
				
			//敵の座標を引数の値に設定
			g_effect[i].ef.x = x;
			g_effect[i].ef.y = y;
			//表示サイズ
			g_effect[i].ef.w = 32;
			g_effect[i].ef.h = 32;
			//生存フラグをtrueにする
			g_effect[i].ef.flag = true;
			//アニメーション設定
			SetAnimation(&g_effect[i].ef,&g_effect[i].ani,
				g_effect[i].ani.sx,g_effect[i].ani.sy,
				g_effect[i].ani.max,g_effect[i].ani.fram,
				g_effect[i].type,false);

			return;
		}
	}
}

//************************************************************************************************
//ステージ変更時のエフェクトのリセット
//************************************************************************************************
void ResetEffect(){
	int i = 0;
	//エフェクトの初期化
	for(i = 0;i < EFFECT_MAX;i++){
		ZeroMemory(&g_effect[i] , sizeof(EFFECT));
	}
}