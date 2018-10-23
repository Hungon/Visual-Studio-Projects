#include "seastage.h"
#include "swimer.h"
#include "play.h"
#include "sound.h"
#include "item.h"
#include "seaenemy.h"
#include "wipe.h"
#include <stdio.h>

//▼グローバル変数
//▼画像の要素
int g_seamapImg[SEA_MAP_IMAGE_MAX] = {0};

//▼効果音
int g_seaSound[SEA_EFFECT_SOUND_MAX] = {0};

//海画像の情報を扱う
STAGE g_seaBack[SEASTAGE_BACKGROUND_MAX] = {0};
//オブジェクトの情報を扱う
STAGE g_seaObject[SEA_OBJECT_MAX] = {0};
//サイン表示を扱う
STAGE g_seaSign[SEA_SIGN_MAX] = {0};
//ミニマップ
STAGE g_SeaMiniMap[SEA_MINIMAP_KIMD_OF_IMAGE] = { 0 };

//▼ステージの範囲を扱う
float g_seaStageW = 0.0f;			//幅
float g_seaStageH = 0.0f;			//高さ
float g_seaStageTop = 0.0f;			//ステージの上辺
float g_seaStageBottom = 480.0f;	//ステージの下辺


//************************************************************************************************
//海のステージの初期化
//************************************************************************************************
void InitSea(HWND hWnd){

	//難易度設定
	//設定した難易度に応じて分岐する
	int sealevelTbl[LEVEL_MAX] = {
		15 , 25 , 35 ,
	};
	//使用する画像
	char	*lpImage[SEA_MAP_IMAGE_MAX] = {
		{ "Image\\Sea\\sea.bmp" },					//ocean
		{ "Image\\Sea\\upheaval.bmp" },				//upheaval
		{ "Image\\Sea\\bubble.bmp" },				//bubble
		{ "Image\\Sea\\caution.bmp" },				//caution image
		{ "Image\\Sea\\Tide.bmp" },					//tide
		{ "Image\\Sea\\ground.bmp" },				//sea's ground
		{ "Image\\Sea\\Minimap.bmp" },				//brief map
		{ "Image\\Sea\\swimer.bmp"},				//swimer image for brief map
		{ "Image\\Sea\\Flag.bmp"},					//the half way's point and goal's point
	};
	//▼画像の読み込み
	for (int i = 0; i < SEA_MAP_IMAGE_MAX; i++) { g_seamapImg[i] = CreateDDImage(lpImage[i], hWnd); }
	
	//▼効果音の読み込み
	//ゴール音
	g_seaSound[0] = CreateSound("SE\\goal.wav");
	//CAUTION警告音
	g_seaSound[1] = CreateSound("SE\\caution.wav");
	//海流の音
	g_seaSound[2] = CreateSound("SE\\stream.wav");

	//海画像の情報をリセット
	ZeroMemory(&g_seaBack[0], sizeof(STAGE) * SEASTAGE_BACKGROUND_MAX);
	//オブジェクトの情報をリセット
	ZeroMemory(&g_seaObject[0], sizeof(STAGE) * SEA_OBJECT_MAX);
	//サイン情報のリセット
	ZeroMemory(&g_seaSign[0], sizeof(STAGE) * SEA_SIGN_MAX);
	//ミニマップ
	ZeroMemory(&g_SeaMiniMap[0], sizeof(STAGE) * SEA_MINIMAP_KIMD_OF_IMAGE);

	//海の背景の設定
	//スクロール設定
	g_seaBack[0].scrollx = true;	//始めは横スクロール
	g_seaBack[0].scroll_l = true;	//左スクロールフラグ
	g_seaBack[0].scroll_r = false;	//右スクロールフラグ
	//背景の設定
	for (int i = 0; i < SEASTAGE_BACKGROUND_MAX; i++) {
		g_seaBack[i].ch.w = 640;
		g_seaBack[i].ch.h = 480;
	}
	g_seaBack[0].ch.flag = true;				//海
	g_seaBack[1].ch.flag = true;				//海底

	//initialize brief map that is indication setting
	//back ground image
	g_SeaMiniMap[0].ch.w = SEA_MINIMAP_IMAGE_BACK_GROUND_WIDTH;
	g_SeaMiniMap[0].ch.h = SEA_MINIMAP_IMAGE_BACK_GROUND_HEIGHT;
	g_SeaMiniMap[0].ch.x = SEA_MINIMAP_IMAGE_BACK_GROUND_INDICATE_X;
	g_SeaMiniMap[0].ch.y = SEA_MINIMAP_IMAGE_BACK_GROUND_INDICATE_Y;
	g_SeaMiniMap[0].ch.flag = true;
	g_SeaMiniMap[0].ch.alpha = 120.0f;
	//minimum player
	float swimerTopHeight = SWIMER_DEFAULT_IMAGE_HEIGHT * SEA_MINIMAP_IMAGE_SWIMER_SCALE_RATE;
	float swimerTopWidth = SWIMER_DEFAULT_IMAGE_WIDTH * SEA_MINIMAP_IMAGE_SWIMER_SCALE_RATE;
	g_SeaMiniMap[1].ch.w = SWIMER_DEFAULT_IMAGE_WIDTH;
	g_SeaMiniMap[1].ch.h = SWIMER_DEFAULT_IMAGE_HEIGHT;
	g_SeaMiniMap[1].ch.scale = SEA_MINIMAP_IMAGE_SWIMER_SCALE_RATE;
	g_SeaMiniMap[1].ch.x = g_SeaMiniMap[0].ch.x - swimerTopWidth;
	g_SeaMiniMap[1].ch.y = g_SeaMiniMap[0].ch.y + (g_SeaMiniMap[0].ch.h - swimerTopHeight);
	g_SeaMiniMap[1].ch.flag = true;
	float flagPosX[2] = { 
		(SEA_MINIMAP_IMAGE_BACK_GROUND_INDICATE_X + SEA_MINIMAP_IMAGE_BACK_GROUND_WIDTH) - SEA_MINIMAP_IMAGE_FLAG_WIDTH,
		SEA_MINIMAP_IMAGE_BACK_GROUND_INDICATE_X,
	};
	for (int i = 2; i < SEA_MINIMAP_KIMD_OF_IMAGE; i++) {		//setting of flag. these are goal and the half point
		g_SeaMiniMap[i].ch.h = SEA_MINIMAP_IMAGE_FLAG_HEIGHT;
		g_SeaMiniMap[i].ch.w = SEA_MINIMAP_IMAGE_FLAG_WIDTH;
		g_SeaMiniMap[i].ch.picy = SEA_MINIMAP_IMAGE_FLAG_HEIGHT * (i - 2);
		g_SeaMiniMap[i].ch.x = flagPosX[i - 2];
		g_SeaMiniMap[i].ch.y = SEA_MINIMAP_IMAGE_BACK_GROUND_INDICATE_Y;
	}
	//starting drawing is flag of the half point
	g_SeaMiniMap[2].ch.flag = true;

	//▼ステージの範囲の設定
	//高さ
	g_seaStageH = 7 * STAGE_HEIGHT;
	//幅
	g_seaStageW = (float)( sealevelTbl[GetGameLevel()] * STAGE_WIDTH );
	//上辺
	g_seaStageTop = 0;
	//下辺
	g_seaStageBottom = 480;
	
	//海底から突き出た巨岩の設定
	g_seaObject[0].ch.x = 640.0f - 256.0f;
	g_seaObject[0].ch.y = 480.0f - 320;		//表示先Ｙ座標
	g_seaObject[0].ch.h = 320;			//高さ
	g_seaObject[0].ch.w = 256;			//幅
	g_seaObject[0].ch.scale = 1.0f;	//拡縮率
	g_seaObject[0].ch.flag = true;	//生存フラグ
	g_seaObject[0].obj_popx = 320.0f;	//発生起点Ｘ座標（この値の倍数の座標に発生）
	g_seaObject[0].obj_endx = g_seaStageW - 640.0f;//消失起点Ｘ座標
	g_seaObject[0].movex = 640.0f;				//出現先Ｘ座標

	//「CAUTIOM」の設定
	g_seaSign[0].ch.x = 200;		//X座標
	g_seaSign[0].ch.y = 100;		//Y座標
	g_seaSign[0].ch.h = 48;			//高さ
	g_seaSign[0].ch.w = 250;		//幅
	g_seaSign[0].ch.flag = false;
	//「CAUTION」のアニメーション設定
	g_seaSign[0].ani.fram = 15;		//フレーム数
	g_seaSign[0].ani.max = 4;			//最大表示数
	//アニメーションの初期化
	SetAnimation(&g_seaSign[0].ch , &g_seaSign[0].ani ,
				g_seaSign[0].ani.sx , g_seaSign[0].ani.sy ,
				g_seaSign[0].ani.max ,
				g_seaSign[0].ani.fram ,
				0 , true);	

	//カメラの座標を設定する
	ChangeCamera( (float)g_seaStageTop , (float)g_seaStageBottom ,
				 (float)g_seaStageW , (float)g_seaStageH);
}

//************************************************************************************************
//海ステージの更新
//************************************************************************************************
void UpdateSea(){

	//プレイヤーの情報を取得
	SWIMER* pch = GetSwimer();
	//アイテムの情報を取得
	ITEM* pit = GetItem();
	//エネミーの情報を取得
	ENEMY* pen = GetEnemy();

	//カメラの座標を取得
	float comeCameraX = GetCameraX();		//X座標
	float comeCameraY = GetCameraY();		//Y座標

	//▼ゴールの設定
	//→スクロールに切り替わってから、設定したＸ座標まで到達したらリザルト画面へ
	if(g_seaBack[0].scroll_r && pch->ch.x < (STAGE_WIDTH >> 1)){
		//タイマーをOFFにする
		ControlStageTime(false);
		//効果音
		PlaySound( g_seaSound[0] , 0 , false );
		//シーン切り替え
		CreateWipe( SCENE_RESULT , CHANGE_SCENE , WIPE_CONTRACTION );
	}

	
	//▼オブジェクトの出現
	//海底から突起した岩
	//Ｙ座標を更新する
	g_seaObject[0].ch.y = comeCameraY + (STAGE_HEIGHT - g_seaObject[0].ch.h);
	//プレイヤーの向きによって出現先Ｘ座標を変更
	//右向き
	if(pch->ch.direction == D_RIGHT){
		g_seaObject[0].movex = 640.0f;
	}
	//左向き
	if(pch->ch.direction == D_LEFT){
		g_seaObject[0].movex = -320.0f;
	}
	//プレイヤーのＸ座標を基に、オブジェクトに割り当てた出現座標Ｘの値で剰余算し、
	//余りが０なら出現させる
	if(g_seaObject[0].ch.flag == false && (int)pch->ch.x % (int)g_seaObject[0].obj_popx == 0 &&
		pch->ch.x < g_seaObject[0].obj_endx){	
		//オブジェクトのX座標を更新
		g_seaObject[0].ch.x = comeCameraX + g_seaObject[0].movex;
		//表示フラグをtrueにする
		g_seaObject[0].ch.flag = true;
	}
	//カメラの枠から出たらfalseに
	if(g_seaObject[0].ch.flag) g_seaObject[0].ch.flag = CollisionCamera(&g_seaObject[0].ch);

	//強制下スクロール前に注意喚起の画像をアニメーションで表示
	if(pch->ch.x > g_seaStageW - STAGE_WIDTH * 2 &&
		pch->ch.y < STAGE_HEIGHT){
			
		//警告音
		if(g_seaSign[0].ch.flag == false){
			PlaySound(g_seaSound[1] , 0 , false);
		}

		//生存フラグをtrueにする
		g_seaSign[0].ch.flag = true;
		
		//「CAUTION」画像の表示フラグがtrueなら
		if(g_seaSign[0].ch.flag == true){	
			//アニメーション更新
			UpdateAnimation(&g_seaSign[0].ch , &g_seaSign[0].ani);
		}
	}
	
	//プレイヤーの座標が既定の座標に達したら背景を上下に整列し、スクロールフラグをtrueにする
	if(pch->ch.x > g_seaStageW - STAGE_WIDTH &&
		pch->ch.y < STAGE_HEIGHT){
		
		//横スクロールフラグをfalseにする
		g_seaBack[0].scrollx = false;
		//縦スクロールフラグをtrueにする
		g_seaBack[0].scrolly = true;
		//「CAUTION」を非表示に
		g_seaSign[0].ch.flag = false;
		//draw tide
		g_seaBack[2].ch.flag = true;
	}

	//縦スクロールフラグ（下移動）がtrueなら指定の座標まで下へ強制スクロール
	if(g_seaBack[0].scrolly == true && pch->ch.y < (g_seaStageH - STAGE_HEIGHT)){
		
		//プレイヤーのY座標が設定した縦の面の最底面の上端のY座標まで到達するまで
		//生存フラグをfalseにし移動を制限し、下へ強制移動
		//海流の音
		PlaySound(g_seaSound[2] , 0 , true);
		//▼プレイヤーの処理
		pch->ch.speed = 0.0f;	//速度倍率を無効に		
		pch->ch.my = 5;			//下へ移動
		//少しずつ右にずらしていく
		if(pch->ch.x < comeCameraX + 500){
			pch->ch.x += 2;		//右方向へ
		}
		//プレイヤーのY座標を加算して、画面下端の値を設定
		g_seaStageBottom += (int)pch->ch.my;
		//下辺を基に上辺を更新
		g_seaStageTop = g_seaStageBottom - STAGE_HEIGHT;
	}
	
	//プレイヤーの座標が指定の座標を越したら、縦スクロールフラグをfalseにし、
	//プレイヤーの移動方向を右から左へ変更する（左移動）
	if(g_seaBack[0].scrolly == true && pch->ch.y >= (g_seaStageH - STAGE_HEIGHT)){
		
		//海流の音停止
		StopSound(g_seaSound[2]);
		//don't draw tide
		g_seaBack[2].ch.flag = false;
		//縦スクロールフラグをfalseにする
		g_seaBack[0].scrolly = false;
		//横スクロールフラグをtrueにする
		g_seaBack[0].scrollx = true;
		//左スクロールから右スクロールへフラグを切り替える
		g_seaBack[0].scroll_l = false;	//左スクロールフラグ
		g_seaBack[0].scroll_r = true;	//右スクロールフラグ
		//change coordinateX of brief map and minimum swimer
		g_SeaMiniMap[0].ch.x = STAGE_WIDTH - (g_SeaMiniMap[0].ch.w + SEA_MINIMAP_IMAGE_BACK_GROUND_INDICATE_X);
		//change origin coordinateY of swimer
		g_SeaMiniMap[1].ch.picy = SWIMER_DEFAULT_IMAGE_HEIGHT;		//change facing direction
		//don't draw the half point
		g_SeaMiniMap[2].ch.flag = false;
		//draw goal's flag
		g_SeaMiniMap[3].ch.flag = true;
		//subsutitute coordinateX of the brief map for coordinateY of goal's variable
		g_SeaMiniMap[3].ch.x = g_SeaMiniMap[0].ch.x;
	}

	//カメラの座標を更新する
	ChangeCamera( g_seaStageTop , g_seaStageBottom ,
				g_seaStageW , g_seaStageH );

	//プレイヤーのX座標がステージの全長のどのくらいの割合に位置しているかでミニマップ上のプレイヤーの座標を更新する
	float topW = g_SeaMiniMap[1].ch.w * g_SeaMiniMap[1].ch.scale;
	g_SeaMiniMap[1].ch.x = (g_SeaMiniMap[0].ch.w * (pch->ch.x / g_seaStageW) + g_SeaMiniMap[0].ch.x) - topW;
}

//************************************************************************************************
//海ステージの表示
//************************************************************************************************
void DrawSea(){
		

	//カメラの座標を取得
	float comeCameraX = GetCameraX();		//X座標
	float comeCameraY = GetCameraY();		//Y座標

	//海の背景を表示
	//海、地面、潮流
	int backImg[SEASTAGE_BACKGROUND_MAX] = { g_seamapImg[0], g_seamapImg[5], g_seamapImg[4] };
	for (int i = 0; i < SEASTAGE_BACKGROUND_MAX; i++) {
		if (g_seaBack[i].ch.flag) {
			DDBlt(
				(int)g_seaBack[i].ch.x,
				(int)g_seaBack[i].ch.y,
				g_seaBack[i].ch.w,
				g_seaBack[i].ch.h,
				backImg[i],
				g_seaBack[i].ch.picx,
				g_seaBack[i].ch.picy);
		}
	}
	//隆起した岩の画像
	//生存フラグがtrueなら表示
	if(g_seaObject[0].ch.flag == true){
		DDScaleBlt(
			g_seaObject[0].ch.x - comeCameraX ,
			g_seaObject[0].ch.y - comeCameraY ,
			g_seaObject[0].ch.w ,
			g_seaObject[0].ch.h ,
			g_seamapImg[1] ,
			g_seaObject[0].ch.picx ,
			g_seaObject[0].ch.picy ,
			g_seaObject[0].ch.scale);
	}
	//「CAUTION」の画像
	//生存フラグがtrueなら表示
	if(g_seaSign[0].ch.flag == true){
		DDBlt(
			(int)g_seaSign[0].ch.x ,
			(int)g_seaSign[0].ch.y ,
			g_seaSign[0].ch.w ,
			g_seaSign[0].ch.h ,
			g_seamapImg[3] ,
			g_seaSign[0].ch.picx,
			g_seaSign[0].ch.picy);
	}
	//draw brief map and minimum swimer
	if (g_SeaMiniMap[0].ch.flag) {				//back ground
		DDAlphaBlt(
			g_SeaMiniMap[0].ch.x,
			g_SeaMiniMap[0].ch.y,
			g_SeaMiniMap[0].ch.w,
			g_SeaMiniMap[0].ch.h,
			g_seamapImg[6],
			g_SeaMiniMap[0].ch.picx,
			g_SeaMiniMap[0].ch.picy,
			g_SeaMiniMap[0].ch.alpha);
	}
	if (g_SeaMiniMap[1].ch.flag) {				//swimer
		DDScaleBltSelectIndicate(
			g_SeaMiniMap[1].ch.x,
			g_SeaMiniMap[1].ch.y,
			g_SeaMiniMap[1].ch.w,
			g_SeaMiniMap[1].ch.h,
			g_seamapImg[7],
			g_SeaMiniMap[1].ch.picx,
			g_SeaMiniMap[1].ch.picy,
			g_SeaMiniMap[1].ch.scale,
			DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP);
	}
	//draw flag
	for (int i = 2; i < SEA_MINIMAP_KIMD_OF_IMAGE; i++) {
		if (g_SeaMiniMap[i].ch.flag) {				//the half way's point and goal's point
			DDBlt(
				g_SeaMiniMap[i].ch.x,
				g_SeaMiniMap[i].ch.y,
				g_SeaMiniMap[i].ch.w,
				g_SeaMiniMap[i].ch.h,
				g_seamapImg[8],
				g_SeaMiniMap[i].ch.picx,
				g_SeaMiniMap[i].ch.picy);
		}
	}
}

//*************************************************************************************************
//解放
//*************************************************************************************************
void ReleaseSea(){
	
	//画像の解放
	for (int i = 0; i < SEA_MAP_IMAGE_MAX; i++) {
		ReleaseDDImage(g_seamapImg[i]);
	}
	//音の解放
	for (int i = 0; i < SEA_EFFECT_SOUND_MAX; i++) {
		ReleaseSound(g_seaSound[i]);
	}
}

//************************************************************************************************
//海ステージの情報を返す
//************************************************************************************************
STAGE* GetSeaStage() { return &g_seaBack[0]; }
