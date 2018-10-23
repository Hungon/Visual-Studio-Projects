#include "seaenemy.h"
#include "map.h"
#include "input.h"
#include "swimer.h"
#include "item.h"
#include "effect.h"
#include "play.h"
#include "sound.h"
#include "seastage.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//▼グローバル変数
ENEMY g_enemy[ENEMY_MAX] = {0};			//座標、移動量、サイズ、生存フラグを扱う構造体型の変数
int g_enemyImg[ENEMY_IMG] = {0};		//ビットマップオブジェクト、メモリデバイスコンテキスト
//現在出現中エネミーの要素を扱う
int g_enemy_type_now = 0;
//直前に出現したエネミーの要素を扱う
int g_enemy_type_old = 0;
//効果音
int g_en_sound[ENEMY_SOUND_EFFECT_MAX] = {0};
//出現させる敵の数
int g_enemyMax = ENEMY_MAX;


//************************************************************************************************
//初期化
//************************************************************************************************
void InitEnemy(HWND hWnd){

	//ゲームの難易度を取得
	tagGAMELEVEL getGameLevel = GetGameLevel();
	
	//海ステージの情報を取得する
	STAGE* pst = GetSeaStage();

	//海ステージの幅を取得
	float getStageWidth = GetStageWidth();

	//難易度に応じて敵の出現間隔を変更する「クラゲ、サメ、魚群、マンボー、エイ」の順番
	int enemyframTbl[LEVEL_MAX][KIND_ENEMY_SEA] = {
		
		{ 100 , 100 , 100 , 0 , 0 } ,			//EASY
		{ 90 , 100 , 110 , 110 , 100 } ,		//NORMAL
		{ 80 , 90 , 100 , 100 , 90 } ,			//HARD
	};

	//難易度に応じて敵の出現する数を変更する
	int enemyappearTbl[LEVEL_MAX][KIND_ENEMY_SEA] = {

		{ 2 , 4 , 3 , 0 , 0 } ,			//EASY
		{ 2 , 4 , 3 , 3 , 3 } ,			//NORMAL
		{ 3 , 5 , 4 , 4 , 4 } ,			//HARD
	};

	//難易度に応じて出現させる敵の最大数を変更する
	int enemymaxTbl[LEVEL_MAX] = {
		5 , 7 , ENEMY_MAX ,
	};

	//出現させる最大数を代入
	g_enemyMax = enemymaxTbl[getGameLevel];

	//エネミーの初期化
	ZeroMemory(g_enemy, sizeof(ENEMY) * ENEMY_MAX);
	//現在出現中エネミーを初期化
	g_enemy_type_now = 0;
	//直前に出現したエネミーを初期化
	g_enemy_type_old = 0;

	//各エネミーの設定
	for (int i = 0;i < g_enemyMax ; i++ ){
	
		g_enemy[i].ch.direction = ENEMY_DIRECTION_LEFT;			//方向「左」
		g_enemy[i].ch.scale = 1.0f;								//拡縮倍率
		//出現座標
		g_enemy[i].popx = 640.0f;								//X
		g_enemy[i].popy = 480.0f;								//Y
	}

	//▼各エネミーの出現・消失起点Ｘ座標
	//左スクロール時
	//クラゲ　要素０
	g_enemy[JELLYFISH].appear_start_posx = 500.0f;									//出現起点座標Ｘ
	g_enemy[JELLYFISH].appear_end_posx = (float)(getStageWidth / 5) * 2;			//消失起点座標Ｘ
	g_enemy[JELLYFISH].appear_type = JELLYFISH;										//出現タイプ
	g_enemy[JELLYFISH].appear = enemyappearTbl[getGameLevel][JELLYFISH];			//出現数
	g_enemy[JELLYFISH].appear_fram = enemyframTbl[getGameLevel][JELLYFISH];			//出現フレーム間隔
	
	//サメ　要素１
	g_enemy[SHARK].appear_start_posx = g_enemy[JELLYFISH].appear_end_posx + 500.0f;	//出現起点座標Ｘ
	g_enemy[SHARK].appear_end_posx = (float)(getStageWidth / 5) * 4;				//消失起点座標Ｘ
	g_enemy[SHARK].appear_type = SHARK;												//出現タイプ
	g_enemy[SHARK].appear = enemyappearTbl[getGameLevel][SHARK];					//出現数
	g_enemy[SHARK].appear_fram = enemyframTbl[getGameLevel][SHARK];					//出現フレーム間隔
	
	//魚群　要素２
	g_enemy[SHOAL].appear_start_posx = g_enemy[SHARK].appear_end_posx + 500.0f;		//出現起点座標Ｘ
	g_enemy[SHOAL].appear_end_posx = (float)getStageWidth - 1000.0f;				//消失起点座標Ｘ
	g_enemy[SHOAL].appear_type = SHOAL;												//出現タイプ
	g_enemy[SHOAL].appear = enemyappearTbl[getGameLevel][SHOAL];					//出現数
	g_enemy[SHOAL].appear_fram = enemyframTbl[getGameLevel][SHOAL];					//出現フレーム間隔
	
	//マンボー
	g_enemy[SUNFISH].appear_type = SUNFISH;											//出現タイプ
	g_enemy[SUNFISH].appear = enemyappearTbl[getGameLevel][SUNFISH];				//出現数
	g_enemy[SUNFISH].appear_fram = enemyframTbl[getGameLevel][SUNFISH];				//出現フレーム間隔
	
	//エイ
	g_enemy[RAY].appear_type = RAY;													//出現タイプ
	g_enemy[RAY].appear = enemyappearTbl[getGameLevel][RAY];						//出現数
	g_enemy[RAY].appear_fram = enemyframTbl[getGameLevel][RAY];						//出現フレーム間隔


	//▼出現・消失するＸ座標起点を変更
	//右スクロール時
	//EASYモード時
	if(getGameLevel == EASY_MODE){
		//魚群
		g_enemy[SHOAL].appear_start_posx_r = (float)(getStageWidth / 3) * 1 - 500.0f;//始点
		g_enemy[SHOAL].appear_end_posx_r = 640.0f;//終点
		//サメ
		g_enemy[SHARK].appear_start_posx_r = (float)(getStageWidth / 3) * 2 - 500.0f;//始点
		g_enemy[SHARK].appear_end_posx_r = g_enemy[SHOAL].appear_start_posx_r + 250.0f;//終点
		//クラゲ
		g_enemy[JELLYFISH].appear_start_posx_r = (float)getStageWidth - 500.0f;	//始点
		g_enemy[JELLYFISH].appear_end_posx_r = g_enemy[SHARK].appear_start_posx_r + 250.0f;//終点
	}
	//NORMALモード時
	if(getGameLevel == NORMAL_MODE){
		//エイ
		g_enemy[RAY].appear_start_posx_r = (float)(getStageWidth / 3) * 1 - 500.0f;//始点
		g_enemy[RAY].appear_end_posx_r = 640.0f;//終点
		//マンボー
		g_enemy[SUNFISH].appear_start_posx_r = (float)(getStageWidth / 3) * 2 - 500.0f;//始点
		g_enemy[SUNFISH].appear_end_posx_r = g_enemy[RAY].appear_start_posx_r + 250.0f;//終点
		//魚群
		g_enemy[SHOAL].appear_start_posx_r = (float)getStageWidth - 500.0f;	//始点
		g_enemy[SHOAL].appear_end_posx_r = g_enemy[SUNFISH].appear_start_posx_r + 250.0f;//終点
	}
	//HARDモード時
	if(getGameLevel == HARD_MODE){
		//エイ
		g_enemy[RAY].appear_start_posx_r = (float)(getStageWidth / 5) * 1 - 500.0f;//始点
		g_enemy[RAY].appear_end_posx_r = 640.0f;//終点
		//マンボー
		g_enemy[SUNFISH].appear_start_posx_r = (float)(getStageWidth / 5) * 2 - 500.0f;//始点
		g_enemy[SUNFISH].appear_end_posx_r = g_enemy[RAY].appear_start_posx_r + 250.0f;//終点
		//魚群
		// AppearStartPos
		// appearStartPosX
		g_enemy[SHOAL].appear_start_posx_r = (float)(getStageWidth / 5) * 3 - 500.0f;	//始点
		g_enemy[SHOAL].appear_end_posx_r = g_enemy[SUNFISH].appear_start_posx_r + 250.0f;//終点
		//サメ
		g_enemy[SHARK].appear_start_posx_r = (float)(getStageWidth / 5) * 4 - 500.0f;	//始点
		g_enemy[SHARK].appear_end_posx_r = g_enemy[SHOAL].appear_start_posx_r + 250.0f;//終点
		//クラゲ
		g_enemy[JELLYFISH].appear_start_posx_r = (float)getStageWidth - 500.0f;	//始点
		g_enemy[JELLYFISH].appear_end_posx_r = g_enemy[SHARK].appear_start_posx_r + 250.0f;//終点
	}

	//▼画像の読み込み
	//くらげの画像を読み込む
	g_enemyImg[0] = CreateDDImage("Image\\Sea\\jellyfish.bmp" , hWnd);
	//サメの画像を読み込む
	g_enemyImg[1] = CreateDDImage("Image\\Sea\\shark.bmp" , hWnd);
	//魚群の画像を読み込む
	g_enemyImg[2] = CreateDDImage("Image\\Sea\\shoal.bmp" , hWnd);
	//マンボーの画像を読み込む
	g_enemyImg[3] = CreateDDImage("Image\\Sea\\sunfish.bmp" , hWnd);
	//エイの画像を読み込む
	g_enemyImg[4] = CreateDDImage("Image\\Sea\\ray.bmp" , hWnd);


	//▼効果音の読み込み
	g_en_sound[0] = CreateSound("SE\\attack.wav");

}

//************************************************************************************************
//更新
//************************************************************************************************
void UpdateEnemy(){

	//ゲームレベルを取得
	tagGAMELEVEL gameLevel = GetGameLevel();

	//関数ポインタに敵の行動をまとめた関数を代入する
	void(*ptype[ENEMY_TYPE_SEA])(ENEMY*) = {

		ActionType0 ,
		ActionType1 ,
		ActionType2 ,
		ActionType3 ,
		ActionType4 ,
	};

	//easy normal
	//座標を基点にした敵の出現
	if( gameLevel == EASY_MODE || gameLevel == NORMAL_MODE ) SetCreationEnemyByPosition();

	//hard
	//敵がランダムで出現
	if (gameLevel == HARD_MODE) SetCreationEnemyByRandom();

	//出現させる敵の数だけループを回す
	for (int i = 0; i < g_enemyMax; i++) {

		//エネミーの生存フラグがtrueなら動かす
		if (g_enemy[i].ch.flag == true) {

			//種類によって異なる行動
			ptype[g_enemy[i].action_type](&g_enemy[i]);

			//座標を移動量に加算
			g_enemy[i].ch.x += g_enemy[i].ch.mx;
			g_enemy[i].ch.y += g_enemy[i].ch.my;
		}//生存フラグ＝trueの処理はここまで	

		//i番との敵とカメラの判定
		if (CollisionCamera(&g_enemy[i].ch) == false) {

			//当たっていなかったら敵を消す
			g_enemy[i].ch.flag = false;
			continue;
		}
	}//for文の終わり
}

//************************************************************************************************
//表示
//************************************************************************************************
void DrawEnemy(){

	//カメラ座標を取得する関数ポインタを宣言
	float px = GetCameraX();						
	float py = GetCameraY();

	//出現させる敵の数だけループを回す
	for (int i = 0; i < g_enemyMax; i++) {

		//エネミーの生存フラグがtrueなら表示
		if (g_enemy[i].ch.flag == true) {

			//エネミーの画像
			DDScaleBlt(
				g_enemy[i].ch.x - px,					//X座標
				g_enemy[i].ch.y - py,					//Y座標
				g_enemy[i].ch.w,						//幅
				g_enemy[i].ch.h,						//高さ
				g_enemyImg[g_enemy[i].ch.bitnum],		//対応したメモリデバイスコンテキスト
				g_enemy[i].ch.picx,					//表示元Ｘ座標
				g_enemy[i].ch.picy,					//表示元Ｙ座標
				g_enemy[i].ch.scale);					//拡縮倍率
		}//g_enemy[i] = trueの時の処理はここまで
	}//for文の処理はここまで
	
}

//************************************************************************************************
//解放
//************************************************************************************************
void ReleaseEnemy(){

	//使用した画像枚数だけ解放を行う
	for (int i = 0; i < ENEMY_IMG; i++) {
		ReleaseDDImage(g_enemyImg[i]);
	}
	//効果音
	for (int i = 0; i < ENEMY_SOUND_EFFECT_MAX; i++) {
		ReleaseSound(g_en_sound[i]);
	}
}

//************************************************************************************************
//敵を指定された座標に出現させる関数
//************************************************************************************************
void CreateEnemy(int appear,int type,int action){

	//プレイヤーの情報を取得する
	SWIMER* pch = GetSwimer();
	//ステージの情報を取得
	STAGE*	lpStage = GetSeaStage();
	//カメラ座標を取得する関数ポインタを宣言
	float px = GetCameraX();
	float py = GetCameraY();

	//タイプ毎の初期化
	void (*pInitAction[KIND_ENEMY_SEA])(ENEMY*) = {
		InitEnemyAction0 ,
		InitEnemyAction1 ,
		InitEnemyAction2 ,
		InitEnemyAction3 ,
		InitEnemyAction4 ,
	};


	//各エネミーのデータテーブル「幅、高さ、DDBltの要素数」
	int typeTbl[KIND_ENEMY_SEA][3] = {

		{32 , 32 , 0} ,			//種類番号０のエネミー「クラゲ」
		{64 , 32 , 1} ,			//種類番号１のエネミー「サメ」
		{64 , 32 , 2} ,			//種類番号２のエネミー「魚群」
		{96 , 128 , 3} ,		//種類番号３のエネミー「マンボー」
		{160 , 96 , 4} ,		//種類番号４のエネミー「エイ」	
	};

	//▼難易度設定
	//難易度に応じて速度を変更する「列要素＝クラゲ、サメ、魚群、マンボー、エイ」の順
	float enemyspeedTbl[LEVEL_MAX][KIND_ENEMY_SEA] = {

		{ 1.0f , -2.0f , -2.0f , 2.0f , 2.0f } ,	//EASY
		{ 1.5f , -3.0f , -3.0f , 3.0f , 3.0f } ,	//NORMAL
		{ 2.0f , -4.0f , -4.0f , 4.0f , 4.0f } ,	//HARD
	
	};

	//最大出現数まで処理
	for(int i = 0 ; i < g_enemyMax ; i++ ){

		//エネミーの生存フラグがfalseなら処理を行う
		if(g_enemy[i].ch.flag == false){
			
			//種類番号によって異なる値を設定
			g_enemy[i].ch.w = typeTbl[type][0];							//幅
			g_enemy[i].ch.h = typeTbl[type][1];							//高さ
			g_enemy[i].ch.bitnum = typeTbl[type][2];					//BitBlt時に扱う要素数
			g_enemy[i].ch.scale = 1.0f;									//拡縮倍率
			g_enemy[i].ch.speed = enemyspeedTbl[GetGameLevel()][type];	//速度
			
			//出現する種類の設定
			g_enemy[i].type = type;
		
			//現在のアクションタイプを設定
			g_enemy[i].action_type = action;

			//出現座標制限（画面下端）
			g_enemy[i].popy = (py + STAGE_HEIGHT);				//Y

			//左スクロール時
			if (lpStage->scroll_l) {
				g_enemy[i].ch.picx = 0;								//表示元座標Y
				g_enemy[i].ch.direction = ENEMY_DIRECTION_LEFT;		//移動方向
				//出現座標
				g_enemy[i].popx = (px + STAGE_WIDTH);				//X
			}
			//右スクロール時の設定
			if (lpStage->scroll_r) {
				g_enemy[i].ch.picx = g_enemy[i].ch.w;								//表示元座標Y
				g_enemy[i].ch.direction = ENEMY_DIRECTION_RIGHT;					//移動方向
				//出現座標
				g_enemy[i].popx = px + ( 0.0f - ( g_enemy[i].ch.w * g_enemy[i].ch.scale ));	//X
			}

			//種類毎による初期化
			pInitAction[g_enemy[i].action_type](&g_enemy[i]);
					
			//生存フラグをON
			g_enemy[i].ch.flag = true;

			//現在のアクションタイプで生成する数が指定した数に達っしたら、生成完了
			if( g_enemy[action].appearCnt == appear ){
				//生成した数をリセット
				g_enemy[action].appearCnt = 0;
				//発射フラグをON
				g_enemy[action].launch = true;
				return;
			}
		}
	}//for文の終わり
}

//************************************************************************************************
//敵の行動の初期化関数「その場で円を描く動き」
//************************************************************************************************
void InitEnemyAction0(ENEMY* pen){
	
	//難易度に応じて角度を変更する「アクションタイプ0番」のみ
	float enemyangleTbl[LEVEL_MAX] = {
		45.0f , 55.0f , 65.0f ,
	};
	//角度を代入
	pen->ch.angle = (int)enemyangleTbl[GetGameLevel()];
	//Ｘ座標
	pen->ch.x = pen->popx;
	//Ｙ座標
	pen->ch.y = (float)RandomNum((int)( pen->popy - ( pen->ch.h * pen->ch.scale )) , (int)(pen->popy - STAGE_HEIGHT));
	//現在のアクションタイプで生成した数をカウント
	g_enemy[ENEMY_ACTION_CIRCLE].appearCnt++;
	
}

//************************************************************************************************
//敵の行動の初期化関数「サメ」のアクション「一直線に動く」
//************************************************************************************************
void InitEnemyAction1(ENEMY* pen){
	
	//出現させる数が要素数に達した場合はreturn
	if(g_enemy[ENEMY_ACTION_STRAIGHT].appearCnt > g_enemy[ENEMY_ACTION_STRAIGHT].appear) return;

	//整列させるＹ座標
	int setTbl[4][8] = {

		{10 , 52 , 94 , 136 , 178 , 220 , 262 , 304} ,
		{136 , 178 , 220 , 262 , 304 , 346 , 388 , 430} ,
		{10 , 52 , 94 , 262 , 304 , 346 , 388 , 430} ,
		{10 , 52 , 94 , 136 , 178 , 346 , 388 , 430} ,
	
	};

	//配列させるＹ座標の行要素番号を取得
	int pos = RandomNum(3,0);

	//整列させる行要素数を指定
	pen->ch.y = ( ( pen->popy - STAGE_HEIGHT ) + (float)setTbl[pos][g_enemy[ENEMY_ACTION_STRAIGHT].appearCnt] );
	//Ｘ座標をプレイヤーの移動に合わせて、画面端で待機
	pen->ch.x = pen->popx;
	//生成した数をカウント
	g_enemy[ENEMY_ACTION_STRAIGHT].appearCnt++;
}

//************************************************************************************************
//敵の行動の初期化関数「魚群」のアクション「上下に波打つ動き」
//************************************************************************************************
void InitEnemyAction2(ENEMY* pen){
	
	//Ｘ座標
	pen->ch.x = pen->popx;	
	//Ｙ座標
	pen->ch.y = (float)RandomNum( (int)( pen->popy - (pen->ch.h * pen->ch.scale) ), (int)(pen->popy - STAGE_HEIGHT) );
	//現在のアクションタイプで生成した数をカウント
	g_enemy[ENEMY_ACTION_WAVE].appearCnt++;

}

//************************************************************************************************
//敵の行動の初期化関数「マンボー」のアクション「エネミーの座標を原点にプレイヤーの座標まで一直線に動く」
//************************************************************************************************
void InitEnemyAction3(ENEMY* pen){
	
	//プレイヤー情報の取得
	SWIMER* pch = GetSwimer();

	//Ｘ座標
	pen->ch.x = pen->popx;
	//Ｙ座標
	pen->ch.y = (float)RandomNum( (int) ( pen->popy - (pen->ch.h * pen->ch.scale)), (int)( pen->popy - STAGE_HEIGHT ) );
	//現在のアクションタイプで生成した数をカウント
	g_enemy[ENEMY_ACTION_APPROACH].appearCnt++;
	//プレイヤーの座標を取得
	pen->targetx = pch->ch.x;
	pen->targety = pch->ch.y;
}

//************************************************************************************************
//敵の行動の初期化関数「エイ」のアクション「エネミーの座標を原点にプレイヤーの座標を追跡する」
//************************************************************************************************
void InitEnemyAction4(ENEMY* pen){

	//Ｘ座標
	pen->ch.x = pen->popx;
	//Ｙ座標
	pen->ch.y = (float)RandomNum((int)(pen->popy - (pen->ch.h * pen->ch.scale)), (int)(pen->popy - STAGE_HEIGHT));
	//現在のアクションタイプで生成した数をカウント
	g_enemy[ENEMY_ACTION_CHASE].appearCnt++;
}

//************************************************************************************************
//敵の行動を変化させる関数「クラゲ」のアクション「その場で円を描く動き」
//************************************************************************************************
void ActionType0(ENEMY* pen){
	
	//回転
	pen->circleCnt++;
	//その場で円を描きながら動く動作
	pen->ch.mx = (float)cos(pen->circleCnt * 3.14 / pen->ch.angle) *
									(pen->ch.speed * pen->ch.direction);		//Ｘ移動

	pen->ch.my = (float)sin(pen->circleCnt * 3.14 / pen->ch.angle) * pen->ch.speed;//Ｙ移動
	//カウント制限1000まで
	if(pen->circleCnt > 1000) pen->circleCnt = 0;
}
	
//************************************************************************************************
//敵の行動を変化させる関数「サメ」のアクション「一直線に動く」
//************************************************************************************************
void ActionType1(ENEMY* pen){

	//整列した数が5対以上になったら行動
	if (g_enemy[ENEMY_ACTION_STRAIGHT].launch) {
		//時間経過
		g_enemy[ENEMY_ACTION_STRAIGHT].stayTime++;
		//進行方向へ直進する動作
		pen->ch.mx = ( pen->ch.speed * pen->ch.direction );		//X移動
		pen->ch.my = 0.0f;										//Y移動
	}

	//1500フレーム以上になったら時間をリセットし、発射フラグをfalseにする
	if (g_enemy[ENEMY_ACTION_STRAIGHT].stayTime >= 1500) {
		g_enemy[ENEMY_ACTION_STRAIGHT].stayTime = 0;
		//発射フラグ
		g_enemy[ENEMY_ACTION_STRAIGHT].launch = false;
	}
}

//************************************************************************************************
//敵の行動を変化させる関数「魚群」のアクション「上下に波打つ動き」
//************************************************************************************************
void ActionType2(ENEMY* pen){

	pen->waveCnt += 2;
	//上下に波打ちながら画面左方向へ移動する動作
	pen->ch.mx = ( pen->ch.speed * pen->ch.direction);							//X移動
	pen->ch.my = (float)sin(pen->waveCnt * 3.14 / 180.0 ) * pen->ch.speed;		//Y移動
	//カウント制限1000まで
	pen->waveCnt %= 1000;

}

//************************************************************************************************
//敵の行動を変化させる関数「エネミーの座標を原点にプレイヤーの座標まで一直線に動く」
//************************************************************************************************
void ActionType3(ENEMY* pen){
	
	//プレイヤーの情報を取得
	SWIMER* pch = GetSwimer();
	//海マップの情報を取得する
	STAGE* pmap = GetSeaStage();

	//▼処理は横スクロール時のみ
	if(pmap->scrollx == true){

		//▼左スクロール時
		if(pmap->scroll_l == true){
		
			//プレイヤーの中心Ｘ座標を超えたらそのまま画面左上へ直進させる
			if(pen->ch.x + (pen->ch.w / 2) <= pch->ch.x + (pch->ch.w * pch->ch.scale)){
				pen->ch.mx = -4.0f;
				pen->ch.my = -4.0f;
			}

			//エネミーの座標がプレイヤーの前方に位置していたら
			//CreateEnemy内で取得したプレイヤーの中心座標を元に
			//エネミーを原点（始線）とし、直角三角形の底辺と高さと斜辺を求める
			if(pen->ch.x + (pen->ch.w / 2) >= pch->ch.x + (pch->ch.w * pch->ch.scale)){
				
				//底辺
				float a = ( pen->targetx + ( pch->ch.w * pch->ch.scale ) / 2 ) - 
							( pen->ch.x + ( pen->ch.w / 2 ) );
				//高さ
				float b = ( pen->targety + ( pch->ch.h * pch->ch.scale ) / 2 ) - 
							( pen->ch.y + (pen->ch.h / 2 ) );
				//斜辺
				float c = sqrtf( (a * a) + (b * b) );
				//cos値を求める
				pen->ch.mx = (a / c) * pen->ch.speed;
				//tan値を求める
				pen->ch.my = (b / c) * pen->ch.speed;
			}
		}
		//▼右スクロール時での処理
		if(pmap->scroll_r == true){
			//プレイヤーの中心Ｘ座標を超えたらそのまま画面左上へ直進させる
			if(pen->ch.x + (pen->ch.w / 2) >= pch->ch.x){
				pen->ch.mx = 4.0f;
				pen->ch.my = -4.0f;
			}
			//エネミーの座標がプレイヤーの前方に位置していたら
			//CreateEnemy内で取得したプレイヤーの中心座標を元に
			//エネミーを原点（始線）とし、直角三角形の底辺と高さと斜辺を求める
			if(pen->ch.x + (pen->ch.w / 2) <= pch->ch.x){
				//底辺
				float a = (pen->targetx + (pch->ch.w * pch->ch.scale) / 2) - 
							(pen->ch.x + (pen->ch.w / 2));
				//高さ
				float b = (pen->targety + (pch->ch.h * pch->ch.scale) / 2) - 
							(pen->ch.y + (pen->ch.h / 2));
				//斜辺
				float c = sqrtf((a * a) + (b * b));
				//cos値を求める
				pen->ch.mx = (a / c) * pen->ch.speed;
				//tan値を求める
				pen->ch.my = (b / c) * pen->ch.speed;
			}
		}
	}
}

//************************************************************************************************
//敵の行動を変化させる関数「エネミーの座標を原点にプレイヤーの座標を追跡する」
//************************************************************************************************
void ActionType4(ENEMY* pen){
	
	//プレイヤーの情報を取得
	SWIMER* pch = GetSwimer();
	//マップの情報を取得する
	STAGE* pmap = GetSeaStage();
	
	//▼横スクロールの時のみ処理
	if(pmap->scrollx == true){

		//▼左スクロール時の処理
		if(pmap->scroll_l == true){
		
			//プレイヤーの右端Ｘ座標を超えたらそのまま画面左上へ直進させる
			if(pen->ch.x + (pen->ch.w / 2) <= pch->ch.x + (pch->ch.w * pch->ch.scale)){
				pen->ch.mx = -4.0f;
				pen->ch.my = -4.0f;
			}

			//エネミーの中心座標がプレイヤーの右端を越えていなかったら
			//エネミーを原点（始線）とし、直角三角形の底辺と高さと斜辺を求める
			if(pen->ch.x + (pen->ch.w / 2) >= pch->ch.x + (pch->ch.w * pch->ch.scale)){

				//底辺
				float a = (pch->ch.x + (pch->ch.w * pch->ch.scale) / 2) - 
							(pen->ch.x + (pen->ch.w / 2));
				//高さ
				float b = (pch->ch.y + (pch->ch.h * pch->ch.scale) / 2) - 
							(pen->ch.y + (pen->ch.h / 2));
				//斜辺
				float c = sqrtf((a * a) + (b * b));
				//cos値を求める
				pen->ch.mx = (a / c) * pen->ch.speed;
				//sin値を求める
				pen->ch.my = (b / c) * pen->ch.speed;
			}
		}
		
		//▼右スクロール時の処理
		if(pmap->scroll_r == true){
		
			//プレイヤーの左端Ｘ座標を超えたらそのまま画面左上へ直進させる
			if(pen->ch.x + (pen->ch.w / 2) >= pch->ch.x){
				pen->ch.mx = 4.0f;
				pen->ch.my = 4.0f;
			}

			//エネミーの中心座標がプレイヤーの左端を越えていなかったら
			//エネミーを原点（始線）とし、直角三角形の底辺と高さと斜辺を求める
			if(pen->ch.x + (pen->ch.w / 2) <= pch->ch.x){

				//底辺
				float a = (pch->ch.x + (pch->ch.w * pch->ch.scale) / 2) - 
							(pen->ch.x + (pen->ch.w / 2));
				//高さ
				float b = (pch->ch.y + (pch->ch.h * pch->ch.scale) / 2) - 
							(pen->ch.y + (pen->ch.h / 2));
				//斜辺
				float c = sqrtf((a * a) + (b * b));
				//cos値を求める
				pen->ch.mx = (a / c) * pen->ch.speed;
				//sin値を求める
				pen->ch.my = (b / c) * pen->ch.speed;
			}
		}
	}
}

//************************************************************************************************
//当たり判定
//************************************************************************************************
bool CollisionEnemy(CHARACTER* ch){

	//戻り値用
	bool bump = false;

	SWIMER* pch = GetSwimer();		//プレイヤーのポインタを取得
	ITEM* pi = GetItem();			//アイテムの情報を取得

	//引数でキャラクターと生きている敵で判定
	for (int i = 0; i < g_enemyMax; i++) {

		//引数の敵が生きていたら
		if (g_enemy[i].ch.flag) {

			//引数のキャラクターとi番の敵で判定
			if (CollisionCharacter(ch, &g_enemy[i].ch)) {

				//敵のダメージエフェクトを表示
				CreateEffect(g_enemy[i].ch.x, g_enemy[i].ch.y);
				//効果音
				PlaySound(g_en_sound[0], 0, false);
				//プレイヤーが攻撃中or無敵状態なら敵を消す
				if (pch->attack || pch->itemEffectType == SWIMER_ITEM_EFFECT_TYPE_ABSOLUTE) {
					//当たった敵を消す	
					g_enemy[i].ch.flag = false;
					//倒した数を種類毎にカウント
					g_enemy[0].defeat[g_enemy[i].type]++;
					//CHAIN数をカウント
					pch->defeat_cnt++;
					//最大討伐数を更新
					if (pch->defeat_cnt > pch->chain_max) pch->chain_max = pch->defeat_cnt;
					//敵がいた場所の少し前方にアイテムを出現させる
					CreateItem((float)g_enemy[i].ch.x + pi->pos_x,		//X座標
						(float)g_enemy[i].ch.y,							//Y座標
						RandomNum(ITEM_APPEAR_RANDOM_MAX,
							ITEM_APPEAR_RANDOM_MIN));				//ランダムで種類決定			
					//敵に当たったらtrueを返す
					bump = true;
					//次の敵と判定
					continue;
				}

				//プレイヤーの生存フラグをfalseに
				pch->ch.flag = false;
				//CHAINを消失
				pch->defeat_cnt = 0;

				//敵に当たったらtrueを返す
				bump = true;
				break;
			}
		}
	}
	//当たっている敵がいなかったらfalseを返す
	return bump;
}

//************************************************************************************************
//敵の情報を返す
//************************************************************************************************
ENEMY* GetEnemy(){ return &g_enemy[0]; }


/*
設定した座標を規定にした敵の出現
*/
void	SetCreationEnemyByPosition() {


	//プレイヤーの情報を取得
	SWIMER* pch = GetSwimer();
	//マップの情報を取得
	STAGE* pst = GetSeaStage();

	//デフォルトの敵のアクションパターン
	int enemy_actionTbl[KIND_ENEMY_SEA] = {

		ENEMY_ACTION_CIRCLE ,			//クラゲ
		ENEMY_ACTION_STRAIGHT ,			//サメ
		ENEMY_ACTION_WAVE ,				//魚群
		ENEMY_ACTION_APPROACH ,			//マンボー
		ENEMY_ACTION_CHASE ,			//エイ
	};

	//▼　現在のプレイヤーの座標を基にエネミーの種類を決定する
	//敵の種類だけループ
	for (int i = 0; i < KIND_ENEMY_SEA; i++) {

		//▼　左スクロール時
		if (pst->scroll_l) {
			//プレイヤーの現在のＸ座標を基に出現させる敵を決定
			if (pch->ch.x >= g_enemy[i].appear_start_posx &&
				pch->ch.x <= g_enemy[i].appear_end_posx) {
				//出現間隔を計る
				g_enemy[0].ch.time++;
				//アクションタイプを代入
				g_enemy_type_now = enemy_actionTbl[i];
				//▼　敵の発生条件		
				//敵を各エネミーに設定されたフレーム毎に生成
				if (g_enemy[0].ch.time % g_enemy[i].appear_fram == 0) {
					g_enemy[0].ch.time = 0;
					//出現する数、出現する種類、アクションタイプを渡す
					CreateEnemy(g_enemy[i].appear, g_enemy[i].appear_type,
						g_enemy_type_now);
					break;
				}
			}
		}

		//▼　右スクロール時
		if (pst->scroll_r) {
			//プレイヤーの現在のＸ座標を基に出現させる敵を決定
			if (pch->ch.x <= g_enemy[i].appear_start_posx_r &&
				pch->ch.x >= g_enemy[i].appear_end_posx_r) {
				//出現間隔を計る
				g_enemy[0].ch.time++;
				//アクションタイプを代入
				g_enemy_type_now = enemy_actionTbl[i];
				//▼　敵の発生条件		
				//敵を各エネミーに設定されたフレーム毎に生成
				if (g_enemy[0].ch.time % g_enemy[i].appear_fram == 0) {
					g_enemy[0].ch.time = 0;
					//出現する数、出現する種類、アクションタイプを渡す
					CreateEnemy(g_enemy[i].appear, g_enemy[i].appear_type,
						g_enemy_type_now);
					break;
				}
			}
		}
	}
}

/*
敵の出現がランダム
*/
void	SetCreationEnemyByRandom() {

	//デフォルトの敵のアクションパターン
	int enemy_actionTbl[KIND_ENEMY_SEA] = {

		ENEMY_ACTION_CIRCLE ,			//クラゲ
		ENEMY_ACTION_STRAIGHT ,			//サメ
		ENEMY_ACTION_WAVE ,				//魚群
		ENEMY_ACTION_APPROACH ,			//マンボー
		ENEMY_ACTION_CHASE ,			//エイ
	};

	//ランダムの値
	int	random = RandomNum(ENEMY_ACTION_CHASE, ENEMY_ACTION_CIRCLE);

	//▼　現在のプレイヤーの座標を基にエネミーの種類を決定する
	//出現間隔を計る
	g_enemy[0].ch.time++;
	//アクションタイプを代入
	g_enemy_type_now = enemy_actionTbl[random];
	//▼　敵の発生条件		
	//敵を各エネミーに設定されたフレーム毎に生成
	if (g_enemy[0].ch.time % g_enemy[random].appear_fram == 0) {
		g_enemy[0].ch.time = 0;
		//出現する数、出現する種類、アクションタイプを渡す
		CreateEnemy(g_enemy[random].appear, g_enemy[random].appear_type,
			g_enemy_type_now);
		
	}
}