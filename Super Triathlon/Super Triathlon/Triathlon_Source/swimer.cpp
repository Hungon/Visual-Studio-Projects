//
#pragma warning ( disable : 4996 )

#include "swimer.h"
#include "gadget.h"
#include "input.h"
#include "seaenemy.h"
#include "item.h"
#include "map.h"
#include "seastage.h"
#include "play.h"
#include "sound.h"
#include "talk.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//▼グローバル関数
SWIMER		g_swimer = {0};
SWIMER		g_SwimerStatus[SWIMER_STATUS] = {0};
SCOREVALUE	g_swimerScore[SWIMER_SCORE] = {0};

//コントロールのフラグ
bool g_ristrictControl = true;

//画像の要素を扱う
int g_swimerImg[SWIMER_IMAGE_MAX] = {0};

//効果音の要素を扱う
int g_swimerSound[SWIMER_SOUND_EFFECT_MAX] = {0};

//攻撃溜め時間
int	g_AttackChargeTime = 0;

//現在の攻撃ビットタイプを扱う
BYTE	g_SwimerAttackBitTypeNow = SWIMER_ANIMATION_TYPE_DIRECTION_RIGHT;

//************************************************************************************************
//初期化を行う関数
//************************************************************************************************
void InitSwimer(HWND hWnd){

	//▼難易度による変更
	//難易度によって攻撃回復速度を変更する
	int attackrecoverTbl[LEVEL_MAX] = {
		5 , 45 , 55 ,
	};

	//プレイヤーの初期化
	ZeroMemory(&g_swimer, sizeof(SWIMER));
	//ステータスの初期化
	ZeroMemory(&g_SwimerStatus[0], sizeof(SWIMER) * SWIMER_STATUS);
	//スコアの初期化
	ZeroMemory(&g_swimerScore[0], sizeof(SCOREVALUE) * SWIMER_SCORE);
	//操作制限
	g_ristrictControl = true;
	
	//▼画像の全ステージ共通の初期設定
	//プレイヤー（スイマー）の画像を読み込む
	g_swimerImg[0] = CreateDDImage("Image\\Sea\\swimer.bmp" , hWnd);	
	//攻撃メーター
	g_swimerImg[1] = CreateDDImage("Image\\Sea\\attack.bmp" , hWnd);


	//▼効果音
	//スピードチャージ上昇音
	g_swimerSound[0] = CreateSound("SE\\speedcharge.wav");
	//スピードチャージ減少音
	g_swimerSound[1] = CreateSound("SE\\chargedown.wav");


	//▼ステータス情報
	//敵討伐数		
	g_swimer.defeat_cnt = 0;							//討伐数
	g_swimer.chain_max = 0;								//CHAIN最大数
	g_swimerScore[0].score_x = 450.0f;					//表示先X
	g_swimerScore[0].score_y = 50.0f;					//表示先Y
	g_swimerScore[0].score_scale = 0.5f;				//拡縮率
	g_swimerScore[0].dicimals_flag = false;				//小数表示フラグ
	g_swimerScore[0].ch.type = SCORE_NUMBER_TYPE_NORMAL;	//スコア画像の種類
	g_swimerScore[0].scoreColor = SCORE_YELOW;			//スコアの色
	g_swimerScore[0].digit = 3;							//桁数
	//敵を倒した数の表示
	g_swimerScore[0].ch.flag = true;


	//攻撃メーター
	//１個目
	g_SwimerStatus[0].ch.x = 50;			//表示先X
	g_SwimerStatus[0].ch.y = 30;			//表示先Y
	g_SwimerStatus[0].ch.h = 64;			//表示高さ
	g_SwimerStatus[0].ch.w = 64;			//表示幅
	g_SwimerStatus[0].ch.scale = 0.5f;		//拡縮倍率
	g_SwimerStatus[0].able_attack = true;	//攻撃可能フラグ
	g_SwimerStatus[0].attack_charge = true;//攻撃チャージフラグ
	g_AttackChargeTime = attackrecoverTbl[GetGameLevel()];//攻撃溜め時間
	
	//2個目
	g_SwimerStatus[1].ch.x = g_SwimerStatus[0].ch.x + 
		g_SwimerStatus[0].ch.w * g_SwimerStatus[0].ch.scale;	//表示先X
	g_SwimerStatus[1].ch.y = g_SwimerStatus[0].ch.y;			//表示先Y
	g_SwimerStatus[1].ch.h = 64;								//表示高さ
	g_SwimerStatus[1].ch.w = 64;								//表示幅
	g_SwimerStatus[1].ch.scale = 0.1f;							//拡縮倍率
	g_SwimerStatus[1].able_attack = false;						//攻撃可能フラグ
	
	//3個目
	g_SwimerStatus[2].ch.x = g_SwimerStatus[1].ch.x + 
		g_SwimerStatus[0].ch.w * g_SwimerStatus[0].ch.scale;	//表示先X
	g_SwimerStatus[2].ch.y = g_SwimerStatus[0].ch.y;			//表示先Y
	g_SwimerStatus[2].ch.h = 64;								//表示高さ
	g_SwimerStatus[2].ch.w = 64;								//表示幅
	g_SwimerStatus[2].ch.scale = 0.1f;							//拡縮倍率
	g_SwimerStatus[2].able_attack = false;						//攻撃可能フラグ



	//▼プレイヤー「スイマー」の初期設定
	g_swimer.ch.x = 200.0f;								//表示するＸ座標
	g_swimer.ch.y = 240.0f;								//表示するＹ座標
	g_swimer.ch.w = SWIMER_DEFAULT_IMAGE_WIDTH;			//表示する画像の幅
	g_swimer.ch.h = SWIMER_DEFAULT_IMAGE_HEIGHT;		//表示する画像の高さ
	g_swimer.ch.picx = 0;								//表示元X
	g_swimer.ch.picy = 0;								//表示元Y
	g_swimer.ch.scale = SWIMER_DEFAULT_SCALE;			//拡縮の倍率
	g_swimer.ch.speed = SWIMER_DEFAULT_SPEED_RATE;		//速度倍率
	g_swimer.ch.defaultSpeed = SWIMER_DEFAULT_SPEED_RATE;//速度倍率初期値
	g_swimer.ch.mx = SWIMER_DEFAULT_SPEEDX_L;			//移動量Ｘ
	g_swimer.ch.default_mx = SWIMER_DEFAULT_SPEEDX_L;	//移動量の初期設定
	g_swimer.ch.my = SWIMER_DEFAULT_SPEEDY;				//移動量Y
	g_swimer.ch.default_my = g_swimer.ch.my;			//移動量Y初期値
	g_swimer.speedmax = SWIMER_DEFAULT_SPEEDMAX_RATE;	//最大加速倍数
	g_swimer.speedmin = SWIMER_DEFAULT_SPEEDMIN_RATE;	//最小加速倍数
	g_swimer.ch.flag = true;							//生存フラグ
	g_swimer.attack = false;							//攻撃中フラグ
	g_swimer.avoid_time = SWIMER_DEFAULT_AVOID_TIME;	//敵に当たった際の回避時間
	g_swimer.speedlevel = SWIMER_SPEED_NORMAL;			//プレイヤーの速度レベル
	g_swimer.attack_type = SWIMER_ATTACK_NORMAL;		//攻撃のタイプ
	g_swimer.ch.direction = D_RIGHT;					//方向
	g_swimer.ch.direction_now = g_swimer.ch.direction;	//現在の方向
	g_swimer.itemEffectType = SWIMER_ITEM_EFFECT_TYPE_VACANT;	//アイテム効果のタイプ

	//位置設定
	g_swimer.pos_x = 200.0f;							//X座標
	g_swimer.pos_y = 240.0f;							//Y座標

	//▼アニメーション設定
	g_swimer.ani.fram = SWIMER_TYPE_NORMAL_ANIMATION_FRAM;		//フレーム数
	g_swimer.ani.max = SWIMER_TYPE_NORMAL_ANIMATION_MAX;		//最大コマ数
	g_swimer.ani.type = SWIMER_ANIMATION_TYPE_DIRECTION_RIGHT;	//泳ぐアニメーション
	
	//プレイヤーのアニメーション情報を設定
	SetAnimation(&g_swimer.ch , &g_swimer.ani ,
				g_swimer.ani.sx , g_swimer.ani.sy ,
				g_swimer.ani.max , g_swimer.ani.fram , SWIMER_ANIMATION_TYPE_DIRECTION_RIGHT, true);
	
}

//************************************************************************************************
//更新を行う関数
//************************************************************************************************
void UpdateSwimer(){

	//アイテム情報を取得
	ITEM* pi = GetItem();
	
	//ステージの情報を取得
	STAGE* pst = GetSeaStage();
	
	//敵を倒した数を更新する
	g_swimerScore[0].num = (float)g_swimer.defeat_cnt;

	//▼ 縦スクロール時の変更
	if( pst->scrolly ){
		
		//移動量をマイナスの値に
		g_swimer.ch.mx = SWIMER_DEFAULT_SPEEDX_R;

		//現在の移動量を変更
		g_swimer.ch.default_mx = g_swimer.ch.mx;
		
		//プレイヤー点滅
		g_swimer.ch.flag = false;
		
		//操作不能
		g_ristrictControl = false;
		
		//敵を倒したスコアを非表示
		g_swimerScore[0].ch.flag = false;
	
	}

	//▼ 右スクロール時の変更
	if( pst->scroll_r && g_swimer.ch.direction == D_RIGHT ){
	
		//操作可能
		g_ristrictControl = true;
		
		//カメラ位置を左へ移動させる（プレイヤーは画面右に）
		g_swimer.pos_x = 500.0f;
		
		//向きを左へ
		g_swimer.ch.direction = D_LEFT;
		
		//現在の方向を取得
		g_swimer.ch.direction_now = g_swimer.ch.direction;		

		//敵を倒したスコアを表示
		g_swimerScore[0].ch.flag = true;
	
	}

	//▼ 移動の更新
	UpdateSwimerMove();
	
	//▼ 攻撃ゲージの回復
	SwimerChargeAttack();

	//▼ 対応したキー入力の挙動
	//操作制御フラグ
	if( g_ristrictControl ){
				
		//▼ 攻撃の更新
		UpdateSwimerAttack();
		
		//▼　速度の更新
		UpdateSwimerSpeed();

		//▼ アイテムの使用
		//アイテム取得済みなら、Cボタンでアイテム使用
		if( JustButton(BUTTON_3) || JustKey('C') && g_swimer.attack == false ) UseItem();

	}

	//▼ 生存フラグなら敵との当たり判定
	if( g_swimer.ch.flag )	CollisionEnemy( &g_swimer.ch );

	//▼ 通常のアニメーション更新
	ChangeSwimerNormalAnimation();

	//▼ 敵を一定時間回避する（ダメージ時の処理）
	AvoidEnemy(&g_swimer.ch , g_swimer.avoid_time);

	//▼ アイテムとの当たり判定
	CollisionItem( &g_swimer );

	//▼ カメラの座標をプレイヤーが中心より、やや左の位置に指定する
	SetCamera(g_swimer.ch.x - g_swimer.pos_x + g_swimer.ch.w ,
		g_swimer.ch.y - g_swimer.pos_y);

	//▼ 移動範囲を制限する
	StrictSwimerMove();

}

//************************************************************************************************
//表示を行う関数
//************************************************************************************************
void DrawSwimer() {

	//カメラの座標を取得
	float px = GetCameraX();
	float py = GetCameraY();

	//敵とぶつかっていたら点滅させる
	if (g_swimer.ch.time % 2 == 0) {

		//キャラクターの画像
		DDScaleBlt(
			g_swimer.ch.x - px,					//X座標
			g_swimer.ch.y - py,					//Y座標
			g_swimer.ch.w,							//幅
			g_swimer.ch.h,							//高さ
			g_swimerImg[0],						//対応した画像番号
			g_swimer.ch.picx,						//表示元Ｘ座標
			g_swimer.ch.picy,						//表示元Ｙ座標
			g_swimer.ch.scale);					//拡縮倍率
	}

	//攻撃メーターの画像
	for (int i = 0; i < SWIMER_ATTACK_MAX; i++) {
		DDScaleBlt(
			g_SwimerStatus[i].ch.x,				//X座標
			g_SwimerStatus[i].ch.y,				//Y座標
			g_SwimerStatus[i].ch.w,				//幅
			g_SwimerStatus[i].ch.h,				//高さ
			g_swimerImg[1],						//対応した画像番号
			g_SwimerStatus[i].ch.picx,			//表示元Ｘ座標
			g_SwimerStatus[i].ch.picy,			//表示元Ｙ座標
			g_SwimerStatus[i].ch.scale);			//拡縮倍率
	}

	//敵を倒した数を表示する
	if (g_swimerScore[0].ch.flag) DrawScore(&g_swimerScore[0]);

	//評価画像の表示
	DrawValueation(g_swimerScore[0].score_x +
		(SCORE_NUMBER_AND_PUNCTUAL_IMAGE_WIDTH * g_swimerScore[0].score_scale),//X座標 
			g_swimerScore[0].score_y,											//Y座標
			g_swimer.defeat_cnt);												//敵討伐数
	
}

//************************************************************************************************
//解放を行う関数
//************************************************************************************************
void ReleaseSwimer(){

	//使用した枚数だけループで回して解放を行う
	for (int i = 0; i < SWIMER_IMAGE_MAX; i++) {
		ReleaseDDImage(g_swimerImg[i]);
	}
	//効果音
	for (int i = 0; i < SWIMER_SOUND_EFFECT_MAX; i++) {
		ReleaseSound(g_swimerSound[i]);
	}
}

//************************************************************************************************
//アニメーションを更新(通常サイズ時の更新)
//************************************************************************************************
void ChangeSwimerNormalAnimation(){

	//画像データテーブル（Y座標で変化）
	BYTE	 animationTbl[ SWIMER_ANIMATION_KIND_OF_TYPE ] = {

		//▼ アニメーション分岐のビット目
		SWIMER_ANIMATION_TYPE_DIRECTION_RIGHT			 ,					//右向き
		SWIMER_ANIMATION_TYPE_DIRECTION_LEFT			 ,					//左向き
		
		SWIMER_ANIMATION_BIT_TYPE_NORMAL_ATTACK		 ,						//バタフライ時
		SWIMER_ANIMATION_TYPE_D_LEFT_NORMAL_ATTACK		 ,					//バタフライ

		SWIMER_ANIMATION_TYPE_D_RIGHT_ABSOLUTE		 ,						//無敵時
		SWIMER_ANIMATION_TYPE_D_LEFT_ABSOLUTE		 ,						//無敵時
		SWIMER_ANIMATION_TYPE_D_RIGHT_NORMAL_ATTACK_ABSOLUTE	 ,			//無敵時
		SWIMER_ANIMATION_TYPE_D_LEFT_NORMAL_ATTACK_ABSOLUTE		 ,			//無敵時
		
		SWIMER_ANIMATION_TYPE_D_RIGHT_SPEED_UP		 ,						//速度上昇時
		SWIMER_ANIMATION_TYPE_D_LEFT_SPEED_UP		 ,						//速度上昇時
		SWIMER_ANIMATION_TYPE_D_RIGHT_NORMAL_ATTACK_SPEED_UP	 ,			//速度上昇時
		SWIMER_ANIMATION_TYPE_D_LEFT_NORMAL_ATTACK_SPEED_UP		 ,			//速度上昇時

		SWIMER_ANIMATION_TYPE_D_RIGHT_SPEED_DOWN	 ,						//速度減少時
		SWIMER_ANIMATION_TYPE_D_LEFT_SPEED_DOWN		 ,						//速度減少時
		SWIMER_ANIMATION_TYPE_D_RIGHT_NORMAL_ATTACK_SPEED_DOWN	 ,			//速度減少時
		SWIMER_ANIMATION_TYPE_D_LEFT_NORMAL_ATTACK_SPEED_DOWN	 ,			//速度減少時

	};

	//現在のアニメーションのタイプ番号に応じて画像を変化させる
	for( int i = 0 ; i < SWIMER_ANIMATION_KIND_OF_TYPE ; i++ ){

		//アニメーションに応じて画像分岐
		if( animationTbl[i] == g_swimer.ani.type ){

			//表示元Y座標を反映する
			g_swimer.ani.sy = ( i * g_swimer.ch.h );

			//▼ 競争者のアニメーション更新
			if( UpdateAnimation( &g_swimer.ch , &g_swimer.ani ) == false ){

				//現在の攻撃アニメーションのビット目を0にする
				g_swimer.ani.type &= ~g_SwimerAttackBitTypeNow;		
				//攻撃タイプを通常に戻す
				g_swimer.attack_type = SWIMER_ATTACK_NORMAL;	
				//現在の攻撃ビット目を通常に
				g_SwimerAttackBitTypeNow = SWIMER_ANIMATION_TYPE_DIRECTION_RIGHT;
				//攻撃フラグをfalseに
				g_swimer.attack = false;

			}
			break;
		}

	}
}


//************************************************************************************************
//アニメーションを更新(攻撃サイズ時の更新)
//************************************************************************************************
void	ChangeSwimerAttackAnimation() {

	//使用するアニメーション分岐
	bool(*lpAnimation[2])(CHARACTER*, ANIMATION*) = {
		UpdateAnimation,				//回転攻撃時
		UpdateReverseAnimation,			//突進攻撃時
	};

	//画像データテーブル（Y座標で変化）
	BYTE	 animationTbl[SWIMER_ANIMATION_KIND_OF_TYPE] = {

		//右向き
		SWIMER_ANIMATION_TYPE_D_RIGHT_CIRCLE_ATTACK		 ,		//回転時
		SWIMER_ANIMATION_TYPE_D_LEFT_CIRCLE_ATTACK		 ,		//回転

		SWIMER_ANIMATION_TYPE_D_RIGHT_CIRCLE_ATTACK_ABSOLUTE	 ,			//無敵時
		SWIMER_ANIMATION_TYPE_D_LEFT_CIRCLE_ATTACK_ABSOLUTE		 ,			//無敵時
		SWIMER_ANIMATION_TYPE_D_RIGHT_CIRCLE_ATTACK_SPEED_UP	 ,			//速度上昇時
		SWIMER_ANIMATION_TYPE_D_LEFT_CIRCLE_ATTACK_SPEED_UP		 ,			//速度上昇時
		SWIMER_ANIMATION_TYPE_D_RIGHT_CIRCLE_ATTACK_SPEED_DOWN	 ,			//速度減少時
		SWIMER_ANIMATION_TYPE_D_LEFT_CIRCLE_ATTACK_SPEED_DOWN	 ,			//速度減少時
		//突進
		SWIMER_ANIMATION_TYPE_D_RIGHT_BUMP_ATTACK		 ,		//突進時
		SWIMER_ANIMATION_TYPE_D_LEFT_BUMP_ATTACK		 ,		//突進

		SWIMER_ANIMATION_TYPE_D_RIGHT_BUMP_ATTACK_ABSOLUTE		 ,			//無敵時
		SWIMER_ANIMATION_TYPE_D_LEFT_BUMP_ATTACK_ABSOLUTE		 ,			//無敵時
		SWIMER_ANIMATION_TYPE_D_RIGHT_BUMP_ATTACK_SPEED_UP		 ,			//速度上昇時
		SWIMER_ANIMATION_TYPE_D_LEFT_BUMP_ATTACK_SPEED_UP		 ,			//速度上昇時
		SWIMER_ANIMATION_TYPE_D_RIGHT_BUMP_ATTACK_SPEED_DOWN	 ,			//速度減少時
		SWIMER_ANIMATION_TYPE_D_LEFT_BUMP_ATTACK_SPEED_DOWN		 ,			//速度減少時

	};

	//画像表示元Y
	int	picY = 0;
	//ループの始めの要素
	int start = 0;
	//アニメーション更新関数の分岐用
	int diverge;
	//現在の攻撃タイプに応じて表示元Ｙ座標を変更する
	//回転攻撃
	if (g_SwimerAttackBitTypeNow == SWIMER_ANIMATION_TYPE_D_RIGHT_CIRCLE_ATTACK) {
		start = 0;
		picY = SWIMER_ANIMATION_TYPE_CIRCLE_ATTACK_PIC_Y;
	}
	//突進攻撃
	if (g_SwimerAttackBitTypeNow == SWIMER_ANIMATION_TYPE_D_RIGHT_BUMP_ATTACK) {
		start = 8;
		picY = SWIMER_ANIMATION_TYPE_BUMP_ATTACK_PIC_Y;
	}

	//現在のアニメーションのタイプ番号に応じて画像を変化させる
	for (int i = start; i < SWIMER_ANIMATION_KIND_OF_TYPE; i++) {
		//アニメーションに応じて画像分岐
		if (animationTbl[i] == g_swimer.ani.type) {
			//アニメーション更新分岐用の要素数
			if (start < 8) { diverge = 0; }	//回転
			else {
				diverge = 1;				//突進
			}
			//表示元Y座標を反映する
			g_swimer.ani.sy = picY + ((i - start) * g_swimer.ch.h);
			//▼ 競争者のアニメーション更新
			if (lpAnimation[diverge](&g_swimer.ch, &g_swimer.ani) == false) {
				//現在の攻撃アニメーションのビット目を0にする
				g_swimer.ani.type &= ~g_SwimerAttackBitTypeNow;
				//現在の攻撃ビット目を通常に
				g_SwimerAttackBitTypeNow = SWIMER_ANIMATION_TYPE_DIRECTION_RIGHT;
				//フレーム数を初期値に
				g_swimer.ani.fram = SWIMER_TYPE_NORMAL_ANIMATION_FRAM;
				//最大コマ数を初期値に
				g_swimer.ani.max = SWIMER_TYPE_NORMAL_ANIMATION_MAX;
				//攻撃タイプを通常に戻す
				g_swimer.attack_type = SWIMER_ATTACK_NORMAL;
				//表示する高さを変更
				g_swimer.ch.h = SWIMER_DEFAULT_IMAGE_HEIGHT;
				//攻撃フラグをfalseに
				g_swimer.attack = false;
				//速度倍率を最高倍率にする
				g_swimer.ch.speed = SWIMER_DEFAULT_SPEEDMAX_RATE;
			}
			break;
		}
	}
}


//*************************************************************************************************
//スイマーの移動を更新する
//*************************************************************************************************
void UpdateSwimerMove(){

	//「↑」の入力「初期速度は5.0」
	if(KeepLever(LEVER_UP) || KeepKey(VK_UP)) g_swimer.ch.y -= g_swimer.ch.my;
	//「↓」の入力
	if(KeepLever(LEVER_DOWN) || KeepKey(VK_DOWN)) g_swimer.ch.y += g_swimer.ch.my;
}

//*************************************************************************************************
//スイマーの速度を更新する
//*************************************************************************************************
void UpdateSwimerSpeed(){

	//速度に準じたタイプ分け
	tagSWIMERANIMATIONSPEED speedType[4] = {

		SWIMER_SPEED_SLOW ,			//遅い
		SWIMER_SPEED_NORMAL ,		//普通
		SWIMER_SPEED_DECENT ,		//少し速い
		SWIMER_SPEED_RAPID ,		//速い	
	
	};
	//プレイヤーの方向に応じた入力するキー変更テーブル
	BYTE	inputSpeedUpTbl[2] = { VK_RIGHT , VK_LEFT };		//速度上昇
	BYTE	inputSpeedDownTbl[2] = { VK_LEFT , VK_RIGHT };		//速度減少

	//▼ 速度
	//Ｂボタンか、右or左キーで加速（方向に応じて変化）
	if(KeepButton(BUTTON_2) || KeepKey(inputSpeedUpTbl[g_swimer.ch.direction_now])){
		//速度倍率上昇
		g_swimer.charge++;
		//速度が上昇中なら音を鳴らす
		if(g_swimer.ch.speed < g_swimer.speedmax && g_swimer.charge % 50 == 0){
			//効果音
			PlaySound(g_swimerSound[0] , 0 , false);
		}
		//速度に倍率を加算
		g_swimer.ch.speed += 0.0001f * g_swimer.charge;
	}

	//Bボタンか、右or左キーが離されたらチャージの値をリセット
	if(JustUpButton(BUTTON_2) || JustUpKey(inputSpeedUpTbl[g_swimer.ch.direction_now])){
		//速度倍率をリセット
		g_swimer.charge = 0;
	}
	//Yボタンか、右or左キーが押されていたら徐々に減速
	if(KeepButton(BUTTON_4) || KeepKey(inputSpeedDownTbl[g_swimer.ch.direction_now])){
		//速度倍率減少
		g_swimer.charge++;
		//減少中なら音を鳴らす
		if(g_swimer.ch.speed > g_swimer.speedmin && g_swimer.charge % 50 == 0){
			//効果音
			PlaySound(g_swimerSound[1] , 0 , false);
		}
		//速度に倍率の値を減算
		g_swimer.ch.speed -= 0.0002f * g_swimer.charge;
	}
	//Yボタンか、右or左キーが離されたらチャージの値をリセット
	if(JustUpButton(BUTTON_4) || JustUpKey(inputSpeedDownTbl[g_swimer.ch.direction_now])){
		//速度倍率をリセット
		g_swimer.charge = 0;
	}
	//倍率の上限値は999まで
	g_swimer.charge %= 1000;

	//速度の上限
	if(g_swimer.ch.speed >= g_swimer.speedmax) g_swimer.ch.speed = g_swimer.speedmax;
	//速度の下限
	if(g_swimer.ch.speed <= g_swimer.speedmin) g_swimer.ch.speed = g_swimer.speedmin;

	//▼速度レベルを更新（アニメーション変化）
	//高い順から調べて現在速度が上回ったらループを抜けて、アニメーションを更新
	for( int i = 3 ; i >= 0 ; i-- ){
		
		//現在の速度が一定の値を上回っていたら
		if(fabs( g_swimer.ch.mx ) * g_swimer.ch.speed > i * 2){
			
			//速度レベルのマクロを代入
			g_swimer.speedlevel = speedType[i];
			break;
		}
	}

	//攻撃中じゃなかったら反映させる
	if( g_swimer.attack == false ){

		//プレイヤーのアニメーション速度を変化
		g_swimer.ani.fram = g_swimer.speedlevel;
	}

	//移動量を座標に加算して反映させる
	//X移動は生存フラグが生きている時だけ行う
	if( g_swimer.ch.flag ){
		
		//移動量*加速度を座標に加算
		g_swimer.ch.x += g_swimer.ch.mx * g_swimer.ch.speed;
	}	

	//キャラクターの移動量を速度メーターに反映させる
	SetStatus( g_swimer.ch.mx * g_swimer.ch.speed );					//X移動

}

//*************************************************************************************************
//スイマーの攻撃を更新する
//*************************************************************************************************
void UpdateSwimerAttack(){

	//攻撃の分岐点
	//通常　→　バタフライ　→　回転　→　突進と変化させていく
	tagSWIMERATTACKTYPE actiontype[SWIMER_ATTACK_TYPE_MAX] = {
		
		SWIMER_ATTACK_NORMAL ,			//通常の状態
		SWIMER_ATTACK_WAVE_ATTACK ,		//バタフライの状態
		SWIMER_ATTACK_CIRCLE_ATTACK ,	//回転の状態
		SWIMER_ATTACK_BUMP_ATTACK ,		//突進の状態
	
	};

	//攻撃タイプの初期化の分岐
	void (*pAttackInit[3])() = {

		InitSwimerActionType0 ,		//バタフライ
		InitSwimerActionType1 ,		//回転
		InitSwimerActionType2 ,		//突進
	
	};
	
	//攻撃タイプの分岐
	void (*pAttackType[3])() = {
		
		SwimerActionType0 ,			//バタフライ
		SwimerActionType1 ,			//回転
		SwimerActionType2 ,			//突進
	
	};

	//Aボタンか、Aキーで攻撃
	if( JustButton(BUTTON_1) || JustKey('A') && g_swimer.itemEffectType != SWIMER_ITEM_EFFECT_TYPE_ABSOLUTE ){
		//攻撃可能なら実行
		if( SwimerGoAttack() ){
			
			//攻撃タイプのループ
			for( int j = 0 ; j <= SWIMER_ATTACK_TYPE_MAX ; j++ ){
				
				//攻撃タイプがベジェタイプなら代入せずにbreak
				if( g_swimer.attack_type == SWIMER_ATTACK_BUMP_ATTACK ) break;

				//現在のタイプと比較して異なる次の要素に
				if( g_swimer.attack_type != actiontype[j] )	continue;
		
				//現在の攻撃タイプに応じて攻撃パターンを変化させる
				if( g_swimer.attack_type == actiontype[j] ){
					
					//派生させる要素を取得
					g_swimer.attack_type = actiontype[j] + 1;
					
					//攻撃分岐の初期化
					pAttackInit[j]();
					break;
				}
			}
		}
	}

	//▼ 攻撃の分岐
	//攻撃フラグがtrueなら分岐
	if( g_swimer.attack ) pAttackType[g_swimer.attack_type]();

}


//**************************************************************************************************
//スイマーの移動範囲を制限する
//**************************************************************************************************
void StrictSwimerMove(){

	
	//右画面からはみ出ないように制御
	if(g_swimer.ch.x > GetCameraX() + 640 - ((float)g_swimer.ch.w * g_swimer.ch.scale)){
		
		//右画面端にぴったりついた座標	
		g_swimer.ch.x = GetCameraX() + 640 - ((float)g_swimer.ch.w * g_swimer.ch.scale);
	}

	//下画面から、はみでないように制御
	if(g_swimer.ch.y > (float)GetCameraY() + 480 - ((float)g_swimer.ch.h * g_swimer.ch.scale)){
		
		//下画面端にぴったりついた座標
		g_swimer.ch.y = (float)GetCameraY() + 480 - ((float)g_swimer.ch.h * g_swimer.ch.scale);
	}

	//左画面からはみ出ないように制御
	if(g_swimer.ch.x < (float)GetCameraX()){

		//左画面端にぴったりくっついた座標
		g_swimer.ch.x = (float)GetCameraX();
	}

	//上画面から、はみでないように制御
	if(g_swimer.ch.y < (float)GetCameraY()){
	
		//上画面端にぴったりくっついた座標
		g_swimer.ch.y = (float)GetCameraY();
	}


}



//************************************************************************************************
//プレイヤーのポインタを返す関数
//************************************************************************************************
SWIMER* GetSwimer(){ return &g_swimer; }

//************************************************************************************************
//プレイヤーの付加効果を消す
//************************************************************************************************
void	OrdinarySwimer(){

	//プレイヤー「スイマー」の初期設定
	g_swimer.ch.w = SWIMER_DEFAULT_IMAGE_WIDTH;			//表示する画像の幅
	g_swimer.ch.h = SWIMER_DEFAULT_IMAGE_HEIGHT;		//表示する画像の高さ
	g_swimer.ch.speed = SWIMER_DEFAULT_SPEED_RATE;		//速度倍数
	g_swimer.ch.mx = g_swimer.ch.default_mx;			//初期速度
	g_swimer.ch.my = g_swimer.ch.default_my;			//初期速度
	g_swimer.ch.time = 0;								//時間
	g_swimer.avoid_time = SWIMER_DEFAULT_AVOID_TIME;	//敵に当たった際の回避時間
	g_swimer.speedmax = SWIMER_DEFAULT_SPEEDMAX_RATE;	//最高速度倍数
	g_swimer.speedmin = SWIMER_DEFAULT_SPEEDMIN_RATE;	//最低速度倍数
	g_swimer.ch.scale = SWIMER_DEFAULT_SCALE;			//拡縮倍率
	g_swimer.attack_type = SWIMER_ATTACK_NORMAL;		//攻撃タイプ
	g_swimer.attack = false;							//攻撃中フラグ
	g_swimer.itemEffectType = SWIMER_ITEM_EFFECT_TYPE_VACANT;//現在のアイテムの効果番号

	//プレイヤー「スイマー」のアニメーション設定
	g_swimer.speedlevel = SWIMER_SPEED_NORMAL;					//フレーム数
	g_swimer.ch.direction = g_swimer.ch.direction_now;			//画像を通常へ変更
	g_swimer.ani.type = SWIMER_ANIMATION_TYPE_DIRECTION_RIGHT;	//泳ぐアニメーション

	//攻撃変更用アニメーションタイプを取得
	g_swimer.ani.fram = SWIMER_TYPE_NORMAL_ANIMATION_FRAM;		//フレーム数
	g_swimer.ani.max = SWIMER_TYPE_NORMAL_ANIMATION_MAX;		//最大コマ数

}


//**********************************************************************************************************
//攻撃ゲージを扱う
//**********************************************************************************************************
void	SwimerChargeAttack(){


	//各攻撃ゲージが攻撃不可状態なら、チャージする
	for( int i = 0 ; i < SWIMER_ATTACK_MAX ; i++ ){
		if( g_SwimerStatus[i].able_attack ) continue;
		g_SwimerStatus[i].attack_charge = true; 
		break;
	}

	//攻撃チャージ中なら、若い要素から順にチャージしていく
	for( int i = 0 ; i < SWIMER_ATTACK_MAX ; i++ ){
		//既に攻撃可能なら次の要素へ
		if( g_SwimerStatus[i].able_attack ) continue;
		//溜めフラグ中なら
		if( g_SwimerStatus[i].attack_charge ){
			
			//時間を計測
			g_SwimerStatus[i].ch.time ++;
						
			//一定時間経過で画像を拡大する
			if( g_SwimerStatus[i].ch.time > g_AttackChargeTime ){
				
				//経過時間をリセット
				g_SwimerStatus[i].ch.time = 0;
				
				//画像を拡大
				g_SwimerStatus[i].ch.scale += 0.1f;
				
				//拡縮率が一定の値まで達っしたら攻撃可能にし、溜めフラグをfalseに
				if( g_SwimerStatus[i].ch.scale >= 0.5f ){
					g_SwimerStatus[i].ch.scale = 0.5f;
					g_SwimerStatus[i].able_attack = true;
					g_SwimerStatus[i].attack_charge = false;
					//次の要素へ
					continue;
				}
			//1度に回復する要素は１つ
			break;
				
			}
		}
	}

}


/*
***************************************************************************************************
攻撃ゲージを消費して攻撃を行う
***************************************************************************************************
*/
bool	SwimerGoAttack(){

	bool Go = false;

	//最大攻撃回数の要素数だけループを回す
	//若い要素から攻撃ゲージを消費していく
	for( int i = ( SWIMER_ATTACK_MAX - 1 ) ; i >= 0 ; i-- ){
		
		//攻撃可能フラグが立っていなかったら次の要素に
		if( g_SwimerStatus[i].able_attack == false ) continue;

		//攻撃可能フラグをfalseに
		g_SwimerStatus[i].able_attack = false;

		//攻撃フラグの画像を縮小
		g_SwimerStatus[i].ch.scale = 0.1f;

		Go = true;
		break;
	}
	return Go;

}
//**********************************************************************************************************
//プレイヤーアクションタイプ０番「バタフライの動き」の初期化
//**********************************************************************************************************
void InitSwimerActionType0(){

	//コマ数をリセット
	g_swimer.ani.cnt = 0;
	//経過時間
	g_swimer.ani.time = 0;
	g_swimer.attack = true;												//攻撃フラグ
	
	//アニメーション設定
	g_swimer.ani.type |= SWIMER_ANIMATION_BIT_TYPE_NORMAL_ATTACK;		//タイプを攻撃に
	g_swimer.ani.fram = SWIMER_TYPE_NORMAL_ANIMATION_FRAM;				//フレーム数
	g_swimer.ani.max = SWIMER_TYPE_NORMAL_ANIMATION_MAX;				//最大コマ数
	//現在の攻撃ビット目を取得
	g_SwimerAttackBitTypeNow |= SWIMER_ANIMATION_BIT_TYPE_NORMAL_ATTACK;

}

//**********************************************************************************************************
//プレイヤーアクションタイプ１番「回転の動き」の初期化
//**********************************************************************************************************
void InitSwimerActionType1(){

	//コマ数をリセット
	g_swimer.ani.cnt = 0;
	//角度リセット
	g_swimer.ch.cnt = 0;
	//経過時間
	g_swimer.ani.time = 0;
	g_swimer.attack = true;							//攻撃フラグ
	g_swimer.ch.h = SWIMER_ATTACK_IMAGE_HEIGHT;		//表示する高さを変更
	//アニメーション設定
	g_swimer.ani.type |= SWIMER_ANIMATION_BIT_TYPE_CIRCLE_ATTACK;		//タイプを回転攻撃に
	g_swimer.ani.fram = SWIMER_TYPE_CIRCLE_ATTACK_ANIMATION_FRAM;		//フレーム数
	g_swimer.ani.max = SWIMER_TYPE_CIRCLE_ATTACK_ANIMATION_MAX;			//最大コマ数
	//現在の攻撃ビット目を取得
	g_SwimerAttackBitTypeNow |= SWIMER_ANIMATION_BIT_TYPE_CIRCLE_ATTACK;
	//出発座標を取得
	g_swimer.ch.sx = g_swimer.ch.x - ((g_swimer.ch.w * g_swimer.ch.scale) / 2);				//X座標
	g_swimer.ch.sy = g_swimer.ch.y;				//Y座標

}

//**********************************************************************************************************
//プレイヤーアクションタイプ２番「ベジェの動き」の初期化
//**********************************************************************************************************
void InitSwimerActionType2(){

	//コマ数をリセット
	g_swimer.ani.cnt = 0;
	//経過時間
	g_swimer.ani.time = 0;
	
	//時間経過をリセット
	g_swimer.ch.millisecond = 0;
	g_swimer.attack = true;						//攻撃フラグ
	
	//出発座標を取得
	g_swimer.ch.sx = g_swimer.ch.x;				//X座標
	g_swimer.ch.sy = g_swimer.ch.y;				//Y座標
	
	g_swimer.ch.h = SWIMER_ATTACK_IMAGE_HEIGHT;		//表示する高さを変更

	//アニメーション設定
	g_swimer.ani.type |= SWIMER_ANIMATION_BIT_TYPE_BUMP_ATTACK;		//タイプを回転攻撃に
	g_swimer.ani.fram = SWIMER_TYPE_BUMP_ATTACK_ANIMATION_FRAM;		//フレーム数
	g_swimer.ani.max = SWIMER_TYPE_BUMP_ATTACK_ANIMATION_MAX;		//最大コマ数
	//現在の攻撃ビット目を取得
	g_SwimerAttackBitTypeNow |= SWIMER_ANIMATION_BIT_TYPE_BUMP_ATTACK;

}

//**********************************************************************************************************
//プレイヤーアクションタイプ０番「バタフライの動き」
//**********************************************************************************************************
void SwimerActionType0(){
	
	//攻撃タイプを代入
	g_swimer.attack_type = SWIMER_ATTACK_WAVE_ATTACK;		
}

//**********************************************************************************************************
//プレイヤーアクションタイプ１番「回転の動き」
//**********************************************************************************************************
void SwimerActionType1(){

	//攻撃タイプを代入
	g_swimer.attack_type = SWIMER_ATTACK_CIRCLE_ATTACK;
	//角度の加算
	g_swimer.ch.cnt += 3;
	CenterCharacter(&g_swimer.ch, (g_swimer.ch.w * g_swimer.ch.scale), g_swimer.ch.cnt);
	//360°まで
	g_swimer.ch.cnt %= 360;
	//▼ アニメーション更新
	ChangeSwimerAttackAnimation();		
}

//**********************************************************************************************************
//プレイヤーアクションタイプ２番「ベジェ曲線の動き」
//**********************************************************************************************************
void SwimerActionType2(){

	//海ステージ情報を取得
	STAGE* pst = GetSeaStage();
	//セットするベジェ曲線の要素数
	int setbezier = 4;
	//攻撃タイプを代入
	g_swimer.attack_type = SWIMER_ATTACK_BUMP_ATTACK;
	
	//頂点
	POINT pos[4] = {
		{0 , 0} ,			//始点
		{800 , 300} ,		//中間点１
		{800 , -200} ,		//中間点３
		{0 , 0} ,			//終点
	};

	//右スクロール時
	if( pst->scroll_r ){
		//頂点Ｘを変更
		pos[1].x = -800;
		pos[2].x = -800;
	}

	//ベジェ曲線の頂点に自身の始点座標を加算
	for( int i = 0 ; i < setbezier ; i++ ){
		pos[i].x += (int)g_swimer.ch.sx;		//X座標
		pos[i].y += (int)g_swimer.ch.sy;		//Y座標
	}
	//時間の経過
	g_swimer.ch.millisecond += 0.01f;
	//ベジェ曲線関数の呼び出し
	SetBezier( &g_swimer.ch , &pos[0] , setbezier - 1 , g_swimer.ch.millisecond );
	//▼ アニメーション更新
	ChangeSwimerAttackAnimation();

}

//************************************************************************************************
//効果１「速度２倍」
//************************************************************************************************
void SwimerEffect0(){


	g_swimer.attack_type = SWIMER_ATTACK_NORMAL;				//攻撃タイプ
	g_swimer.attack = false;									//攻撃中フラグ
	//プレイヤー「スイマー」のアニメーション設定
	g_swimer.speedlevel = SWIMER_SPEED_NORMAL;					//フレーム数
	g_swimer.ch.direction = g_swimer.ch.direction_now;			//画像を通常へ変更
	g_swimer.ani.type = SWIMER_ANIMATION_TYPE_DIRECTION_RIGHT;	//泳ぐアニメーション
	//攻撃変更用アニメーションタイプを取得
	g_swimer.ani.fram = SWIMER_TYPE_NORMAL_ANIMATION_FRAM;		//フレーム数
	g_swimer.ani.max = SWIMER_TYPE_NORMAL_ANIMATION_MAX;		//最大コマ数

	//入手直後に変化
	g_swimer.ani.time = g_swimer.ani.fram;

	//ビット目を代入
	g_swimer.ani.type |= SWIMER_ANIMATION_TYPE_D_RIGHT_SPEED_UP;

	//現在のアイテムの効果番号を取得
	g_swimer.itemEffectType = SWIMER_ITEM_EFFECT_TYPE_SPEEDSTAR;

	//速度倍率を初期値の2倍
	//X移動量
	g_swimer.ch.mx = (float)( (int)g_swimer.ch.default_mx << 1 );
	//Y移動量
	g_swimer.ch.my = (float)( (int)g_swimer.ch.default_my << 1 ); 
	
	//上限は2倍まで
	//X移動量
	if( g_swimer.ch.mx > (float)( (int)g_swimer.ch.default_mx << 1 ) ){
		g_swimer.ch.mx = (float)( (int)g_swimer.ch.default_mx << 1 );
	}
	//Y移動量
	if( g_swimer.ch.my > (float)( (int)g_swimer.ch.default_my << 1 ) ){
		g_swimer.ch.my = (float)( (int)g_swimer.ch.default_my << 1 );
	}
	
	//動作を通常より２倍速
	g_swimer.speedlevel = SWIMER_SPEED_RAPID;
}

//************************************************************************************************
//効果２「無敵化」
//************************************************************************************************
void SwimerEffect1(){

	g_swimer.attack_type = SWIMER_ATTACK_NORMAL;				//攻撃タイプ
	g_swimer.attack = false;									//攻撃中フラグ
	//プレイヤー「スイマー」のアニメーション設定
	g_swimer.speedlevel = SWIMER_SPEED_NORMAL;					//フレーム数
	g_swimer.ch.direction = g_swimer.ch.direction_now;			//画像を通常へ変更
	g_swimer.ani.type = SWIMER_ANIMATION_TYPE_DIRECTION_RIGHT;	//泳ぐアニメーション
	//攻撃変更用アニメーションタイプを取得
	g_swimer.ani.fram = SWIMER_TYPE_NORMAL_ANIMATION_FRAM;		//フレーム数
	g_swimer.ani.max = SWIMER_TYPE_NORMAL_ANIMATION_MAX;		//最大コマ数

	//入手直後に変化
	g_swimer.ani.time = g_swimer.ani.fram;
	//タイプを無敵状態に
	g_swimer.ani.type |= SWIMER_ANIMATION_TYPE_D_RIGHT_ABSOLUTE;
	//現在のアイテムの効果番号を取得
	g_swimer.itemEffectType = SWIMER_ITEM_EFFECT_TYPE_ABSOLUTE;
	//プレイヤーの敵回避時間をリセット
	g_swimer.ch.time = 0;
	//生存フラグ
	g_swimer.ch.flag = true;
}

//************************************************************************************************
//効果３「速度半減」
//************************************************************************************************
void SwimerEffect2(){

	//プレイヤーの状態を通常に
	OrdinarySwimer();

	//入手直後に変化
	g_swimer.ani.time = g_swimer.ani.fram;

	//ビット目を代入
	g_swimer.ani.type |= SWIMER_ANIMATION_TYPE_D_RIGHT_SPEED_DOWN;

	//現在のアイテムの効果番号を取得
	g_swimer.itemEffectType = SWIMER_ITEM_EFFECT_TYPE_SNAIL;

	//速度倍率を初期値の半分
	//X移動量
	g_swimer.ch.mx = (float)((int)g_swimer.ch.default_mx >> 1);
	
	//Y移動量
	g_swimer.ch.my = (float)((int)g_swimer.ch.default_my >> 1); 
	
	//上限は2倍まで
	//X移動量
	if(g_swimer.ch.mx > (float)((int)g_swimer.ch.default_mx >> 1)){
		g_swimer.ch.mx = (float)((int)g_swimer.ch.default_mx >> 1);
	}
	
	//Y移動量
	if(g_swimer.ch.my > (float)((int)g_swimer.ch.default_my >> 1)){
		g_swimer.ch.my = (float)((int)g_swimer.ch.default_my >> 1);
	}
	
	//動作を通常より0.5倍速
	g_swimer.speedlevel = SWIMER_SPEED_SLOW;
}

//************************************************************************************************
//効果４「拡大」
//************************************************************************************************
void SwimerEffect3(){
	//画像サイズを1.25倍にする
	g_swimer.ch.scale = 1.25f;

}

//************************************************************************************************
//効果５「縮小」
//************************************************************************************************
void SwimerEffect4(){
	//画像をサイズを3/4にする
	g_swimer.ch.scale = 0.75f;
}