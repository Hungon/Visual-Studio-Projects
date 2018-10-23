#include "item.h"
#include "swimer.h"
#include "seaenemy.h"
#include "map.h"
#include "input.h"
#include "play.h"
#include "sound.h"
#include "seastage.h"
#include <stdio.h>


//▼グローバル変数の宣言
//画像の要素を扱う
int g_itemImg[ITEM_IMAGE_MAX] = {0};
//アイテム設定用
ITEM g_item[ITEM_MAX] = {0};
//アイテム獲得時の入れ物の設定用
ITEM g_item_box[1] = {0};

//使用前ののタイプ
int g_type_old = ITEM_VACANT;
//現在の所持アイテムの要素番号を扱う
int g_get = ITEM_VACANT;
//効果中の要素番号を扱う
int g_among = ITEM_VACANT;
//効果音
int g_it_sound[ITEM_SOUND_EFFECT_MAX] = {0};
//難易度によってアイテム出現率を変更
int g_itemincidentTbl[LEVEL_MAX] = {1 , 2 , 3};

//************************************************************************************************
//初期化
//************************************************************************************************
void InitItem(HWND hWnd){

	
	//▼画像の読み込み
	//アイテムの画像
	g_itemImg[0] = CreateDDImage("Image\\Sea\\item.bmp" , hWnd);
	//アイテム獲得時の入れ物
	g_itemImg[1] = CreateDDImage("Image\\Sea\\itembox.bmp" , hWnd);

	//▼音の読み込み
	//アイテム獲得音
	g_it_sound[0] = CreateSound("SE\\itemget.wav");
	//強化アイテム使用時
	g_it_sound[1] = CreateSound("SE\\strongitem.wav");
	//弱体化アイテム取得時
	g_it_sound[2] = CreateSound("SE\\weakitem.wav");


	//アイテムの初期化
	ZeroMemory(&g_item[0], sizeof(ITEM) * ITEM_MAX);
	//使用済みのアイテムの番号を扱う値をリセット
	g_type_old = ITEM_VACANT;
	//現在のタイプをリセット
	g_among = ITEM_VACANT;
	//現在の所持アイテムの要素番号をリセット
	g_get = ITEM_VACANT;

	//アイテムの獲得時の入れ物用
	g_item_box[0].ch.x = 320.0f - 48.0f;
	g_item_box[0].ch.y = 0.0f;
	g_item_box[0].ch.w = 96;
	g_item_box[0].ch.h = 96;
	g_item_box[0].ch.alpha = 120.0f;
	g_item_box[0].ch.flag = true;

	//▼各アイテムの初期値を設定
	for (int i = 0; i < ITEM_MAX; i++) {

		g_item[i].ch.flag = false;								//生存フラグ
		g_item[i].ch.h = 32;									//高さ
		g_item[i].ch.w = 32;									//幅
		g_item[i].displayX = (int)(g_item_box[0].ch.x + 32.0f);	//表示X座標
		g_item[i].displayY = (int)(g_item_box[0].ch.y + 32.0f);	//表示Y座標
		g_item[i].ch.scale = 1.5f;								//拡縮倍率
		g_item[i].type = ITEM_VACANT;							//タイプ
	}


	//アイテムの出現率を難易度に応じて設定
	//EASYなら毎回
	//NORMALなら2回に1回の頻度
	//HARDなら3回に1回の頻度
	g_itemincidentTbl[GetGameLevel()];
	//アイテムの出現位置
	g_item[0].pos_x = 300.0f;		//X座標

}

//************************************************************************************************
//更新
//************************************************************************************************
void UpdateItem(){

	int i = 0;

	//海ステージの情報を取得
	STAGE* pst = GetSeaStage();

	//▼右スクロール時の変更
	if(pst->scroll_r){
		g_item[0].pos_x = -300.0f;	//出現Ｘ座標
	}
	
	//▼左スクロール時の変更
	if(pst->scroll_l){
		g_item[0].pos_x = 300.0f;	//出現Ｘ座標
	}

	//出現するアイテムの最大数だけ回す
	for(i = 0;i < ITEM_MAX;i++){
		
		//画面外なら生存フラグをfalseに
		if(CollisionCamera(&g_item[i].ch) == false){

			//アイテムのフラグをfalseに
			g_item[i].ch.flag = false;
			continue;
		}
	}

	//アイテムの効果を持続させるかを判定する関数
	CheckResetEffect();
}

//************************************************************************************************
//表示
//************************************************************************************************
void DrawItem(){

	int i = 0;

	//プレイヤーの情報を取得する
	SWIMER* pch = GetSwimer();

	char mes[25] = {0};

	//カメラの座標を取得する
	float px = GetCameraX();		//X座標					
	float py = GetCameraY();		//Y座標


	//出現するアイテムの最大数だけ回す
	for(i = 0;i < ITEM_MAX;i++){

		//アイテムの生存フラグがtrueなら表示
		if(g_item[i].ch.flag){

			//アイテムの画像
			DDBlt(
				(int)( g_item[i].ch.x - px ) ,		//X座標
				(int)( g_item[i].ch.y - py ) ,		//Y座標
				g_item[i].ch.w ,				//幅
				g_item[i].ch.h ,				//高さ
				g_itemImg[0] ,					//対応した画像番号
				g_item[i].ch.picx ,				//表示元Ｘ座標
				g_item[i].ch.picy);				//表示元Ｙ座標
		}
	}

	//入手済みなら入手したアイテム画像を左上に表示
	if(g_item[g_get].have){

		//アイテムの画像
		DDScaleBlt(
			(float)g_item[g_get].displayX ,	//X座標
			(float)g_item[g_get].displayY ,	//Y座標
			g_item[g_get].ch.w ,			//幅
			g_item[g_get].ch.h ,			//高さ
			g_itemImg[0] ,					//対応した画像番号
			g_item[g_get].ch.picx ,			//表示元Ｘ座標
			g_item[g_get].ch.picy ,			//表示元Ｙ座標
			g_item[g_get].ch.scale);		//拡縮率
	}

	//入手済みのアイテムを収める枠の画像
	if(g_item_box[0].ch.flag){
		
		//半透明で表示
		DDAlphaBlt(
			(int)g_item_box[0].ch.x ,
			(int)g_item_box[0].ch.y ,
			g_item_box[0].ch.w ,
			g_item_box[0].ch.h ,
			g_itemImg[1] ,
			g_item_box[0].ch.picx ,
			g_item_box[0].ch.picy ,
			g_item_box[0].ch.alpha);
	}

}


//************************************************************************************************
//解放
//************************************************************************************************
void ReleaseItem(){

	int i = 0;
	
	//画像
	for(i = 0;i < ITEM_IMAGE_MAX;i++){
		ReleaseDDImage(g_itemImg[i]);
	}
	//効果音
	for(i = 0;i < ITEM_SOUND_EFFECT_MAX;i++){
		ReleaseSound(g_it_sound[i]);
	}
}

//************************************************************************************************
//アイテムを出現させる関数
//************************************************************************************************
void CreateItem(float x, float y, int r) {

	int i = 0;
	//アイテムのデータテーブル「表示元X座標、効果時間、出現確率値の上値、出現確率値の下値」
	//↓強化
	int strengTbl[ITEM_KIND_STRENGTH][4] = {

		{ ITEM_1 , 1000 , 9 , 0 }   ,
		{ ITEM_2 , 1000 , 19 , 10 } ,
		{ ITEM_4 , 1000 , 29 , 20 } ,
	};
	//弱体
	int weakTbl[ITEM_KIND_WEAK][4] = {

		{ ITEM_3 , 500 , 39 , 30 } ,
		{ ITEM_5 , 500 , 49 , 40 } ,
	};

	int weak_type = 0;

	//アイテムを出現させるかを難易度に応じて判定
	//ランダムの値1～1000の範囲の値が、レベル毎に設定された値で
	//剰余算で割り切れたらアイテムを発生させる
	if (RandomNum(1000, 1) % g_itemincidentTbl[GetGameLevel()] == 0) {

		//出現するアイテムの最大数だけ回す
		for (i = 0; i < ITEM_MAX; i++) {

			//アイテムの生存フラグがfalseなら値を代入
			if (g_item[i].ch.flag == false && g_item[i].have == false) {

				//出現座標を決定
				g_item[i].ch.x = x;		//Ｘ座標
				g_item[i].ch.y = y;		//Ｙ座標

				//▼ 出現させるアイテムを渡された値を元に分岐させる		
				//速度上昇
				if (r >= strengTbl[0][3] && r <= strengTbl[0][2]) {
					//種類番号を取得
					g_item[i].type = ITEM_TYPE_SPEED_UP;
				}
				//無敵
				if (r >= strengTbl[1][3] && r <= strengTbl[1][2]) {
					//種類番号を取得
					g_item[i].type = ITEM_TYPE_ABSOLUTE;
				}
				//拡大
				if (r >= strengTbl[2][3] && r <= strengTbl[2][2]) {
					//種類番号を取得
					g_item[i].type = ITEM_TYPE_BIG;
				}
				//減速
				if (r >= weakTbl[0][3] && r <= weakTbl[0][2]) {
					//種類番号を取得
					g_item[i].type = ITEM_TYPE_SPEED_DOWN;
				}
				//縮小
				if (r >= weakTbl[1][3] && r <= weakTbl[1][2]) {
					//種類番号を取得
					g_item[i].type = ITEM_TYPE_SMALL;
				}
				//フラグをＯＮ
				g_item[i].ch.flag = true;
				//0以上かつ、10より小さいなら「強化アイテム」
				if (g_item[i].type >= 0 && g_item[i].type < ITEM_KIND_BOUND) {
					//効果時間を代入
					g_item[i].ch.time = strengTbl[g_item[i].type][1];
					//表示元X座標
					g_item[i].ch.picy = strengTbl[g_item[i].type][0];
				}
				//渡された種類番号が弱体化を扱う１０以上、１１以下の値なら
				//対応した要素番号に変える「弱体化アイテム」
				if (g_item[i].type >= ITEM_KIND_BOUND &&
					g_item[i].type < (ITEM_KIND_BOUND + ITEM_KIND_WEAK)) {
					//配列の要素に値を合わせる
					weak_type = (g_item[i].type - ITEM_KIND_BOUND);
					//種類番号に応じた値を代入
					//効果時間を代入
					g_item[i].ch.time = weakTbl[weak_type][1];
					//表示元X座標
					g_item[i].ch.picy = weakTbl[weak_type][0];
				}
				return;
			}
		}
	}
}

//************************************************************************************************
//アイテムとの判定を取る関数
//************************************************************************************************
bool CollisionItem(SWIMER* pch){

	int i = 0;
	int weak_type = 0;

	//戻り値用
	bool ret = false;

	//アイテムの効果を扱う関数ポインタ
	void (*pweak[ITEM_KIND_WEAK])() = {	
		
		SwimerEffect2 ,			//減速
		SwimerEffect4 ,			//縮小
	};

	//引数でキャラクターと出現したアイテムとで判定
	for( i = 0 ; i < ITEM_MAX ; i++ ){

		//引数のアイテムが出現したら
		if(g_item[i].ch.flag == true){
			
			//引数のキャラクターとi番のアイテムで判定
			if( CollisionCharacter( &pch->ch , &g_item[i].ch ) ){
		
				//当たったアイテムを消す	
				g_item[i].ch.flag = false;
			
				//取得したアイテムを種類毎にカウント
				g_item[0].get[g_item[i].type]++;
		
				//当たったアイテムの種類番号が0番以上、9番以下なら処理（強化アイテム）
				if( g_item[i].type >= 0 && g_item[i].type < ITEM_KIND_BOUND ){

					//アイテム入手済みフラグON
					g_item[i].have = true;

					//表示する要素に値を代入（所持用）※弱体アイテムは非表示
					g_get = i;

					//獲得音
					PlaySound(g_it_sound[0] , 0 , false);

					//アイテム取得を返す
					ret = true;

					break;
				}

				//当たったアイテムの種類番号が10番以上（弱体化アイテム）なら
				//対応した要素番号に変換する
				//弱体を扱う値内かを調べる
				if( g_item[i].type >= ITEM_KIND_BOUND && 
					g_item[i].type < ( ITEM_KIND_BOUND + ITEM_KIND_WEAK ) ){

					//入手済みフラグOFF
					g_item[i].have = false;	
				
					//弱体を扱う要素に現在の種類番号－強化と弱体の境界の値を代入する
					weak_type = ( g_item[i].type - ITEM_KIND_BOUND );
					
					//適用される効果の要素番号を取得
					g_among = i;
					
					//効果中フラグをtrueにする
					g_item[i].among = true;
					
					//獲得音
					PlaySound(g_it_sound[2],0,false);
					
					//効果中かつ、同じ種類なら効果時間を延長
					if(g_item[i].among == true && g_type_old == g_item[i].type){
					
						//時間加算
						g_item[i].e_time += g_item[i].ch.time;
					}

					//タイプが異なるなら、対応した効果時間を代入
					if(g_type_old != g_item[i].type){

						//時間を代入
						g_item[i].e_time = g_item[i].ch.time;

					}
				
					//種類番号ごとに効果を分岐
					pweak[weak_type]();

					//使用済みのアイテム番号を取得
					g_type_old = g_item[i].type;

					//アイテムに当たったらtrueを返す
					ret = true;

					break;
				}
			}
		}
	}
	//当たっているアイテムがなかったらfalseを返す
	return ret;
}

//************************************************************************************************
//アイテム使用時の処理
//************************************************************************************************
void UseItem(){

	SWIMER* pch = GetSwimer();		//プレイヤーのポインタを取得
	
	//アイテムの効果を扱う関数ポインタ
	void (*pstreng[ITEM_KIND_STRENGTH])() = {	
		
		SwimerEffect0 ,				//速度上昇
		SwimerEffect1 ,				//無敵
		SwimerEffect3 ,				//拡大
	};
	
	//範囲外ならreturnする
	if(g_item[g_get].type < 0 || g_item[g_get].type >= ITEM_KIND_STRENGTH ) return;

	//強化アイテムの要素の範囲内かを調べて
	//処理を分岐させる
	if(g_item[g_get].type >= 0 && g_item[g_get].type < ITEM_KIND_STRENGTH){
	
		//現在所持しているアイテムの効果を発揮
		if(g_item[g_get].have == true){

			//効果中の要素番号を取得
			g_among = g_get;

			//効果中フラグをON
			g_item[g_get].among = true;

			//パワーアップ音
			PlaySound(g_it_sound[1],0,false);

			//効果中かつ、同じ種類なら効果時間を延長
			if(g_item[g_get].among == true && g_type_old == g_item[g_get].type){
				
				//時間を延長
				g_item[g_get].e_time += g_item[g_get].ch.time;
			}

			//タイプが異なるなら、対応した効果時間を代入
			if(g_type_old != g_item[g_get].type){

				//時間を代入する
				g_item[g_get].e_time = g_item[g_get].ch.time;
				
			}
			
			//効果の分岐
			pstreng[g_item[g_get].type]();
			
			//使用済みのアイテム番号を取得
			g_type_old = g_item[g_get].type;
			
			//入手済みフラグOFF
			g_item[g_get].have = false;	
			
			//所持している要素をリセット
			g_get = ITEM_VACANT;
		}
	}
}

//************************************************************************************************
//経過時間によってアイテムの効果を消失させる関数
//************************************************************************************************
void CheckResetEffect(){
	
	//アイテムの効果中なら時間を計測する
	if(g_item[g_among].among == true){

		//時間経過
		g_item[g_among].e_time--;
	}

	//経過時間とアイテム毎の効果時間を比べて、
	//経過時間が超えていたら、プレイヤーの状態をリセットし、時間もリセット
	if(g_item[g_among].e_time < 0){

		//現在のタイプを初期値に
		g_item[g_among].type = ITEM_VACANT;

		//現在のタイプの要素番号をリセット
		g_among = ITEM_VACANT;

		//時間をリセット
		g_item[g_among].e_time = 0;

		//効果中フラグをOFF
		g_item[g_among].among = false;

		//プレイヤーの付加効果を消す
		OrdinarySwimer();
	}

}

//************************************************************************************************
//アイテムの情報を取得する
//************************************************************************************************
ITEM* GetItem(){ return g_item; }


