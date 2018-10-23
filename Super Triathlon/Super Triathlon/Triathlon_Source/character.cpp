#include "character.h"
#include "map.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;


//▼　グローバル変数の宣言

//************************************************************************************************
//当たり判定　CollisionCharacter
//************************************************************************************************
bool CollisionCharacter(CHARACTER* pch1 , CHARACTER* pch2){


	//2つのキャラクターの重なりを調べる
	if((pch1->x - fabs(pch1->mx)) < pch2->x + (float)(pch2->w * pch2->scale) &&
	   (pch1->x + (float)(pch1->w * pch1->scale)) + fabs(pch1->mx) > pch2->x &&
 	   (pch1->y + fabs(pch1->my)) < pch2->y + (float)(pch2->h  * pch2->scale) &&
	   (pch1->y + (float)(pch1->h  * pch1->scale)) - fabs(pch1->my) > pch2->y ){		
		//重なっていたらtrueを返す
		return true;
	}
	//重なっていなかったらfalseを返す
	return false;
}


//************************************************************************************************
//一定時間の間だけ、敵との判定を取らない
//************************************************************************************************
bool AvoidEnemy(CHARACTER* pch , int avoidtime){

	//敵を回避中かどうかを返す変数
	bool ret = false;

	//生存フラグがfalseなら、時間を計測
	//速度をデフォルトにリセット
	if(pch->flag == false){
		//経過時間
		pch->time++;
		//停止状態にする
		pch->speed = 0;
		//回避中であることを返す
		ret = true;
	}

	//経過時間が引数のフレーム数を超えたらtrueに、経過時間をリセットする
	if(pch->time > avoidtime){
		//生存フラグ
		pch->flag = true;
		//時間をリセット
		pch->time = 0;
		//速度倍率を元に戻す
		pch->speed = pch->defaultSpeed;
		//回避フラグを消す
		ret = false;
	}

	//最終的に回避中かどうかを返す
	return ret;

}


//***********************************************************************************************
//引数の中心座標を求める
//***********************************************************************************************
void CenterCharacter(CHARACTER* ch , int len , int angle){

	//回転中心を算出
	int cx = (int)ch->sx + (ch->w >> 1);
	int cy = (int)ch->sy + (ch->h >> 1);

	//回転後の座標を求める
	ch->x = (float)cos(angle * 3.14 / 180.0) * len + cx - (ch->w >> 1);
	ch->y = (float)sin(angle * 3.14 / 180.0) * len + cy - (ch->h >> 1);
}


//************************************************************************************************
//多次元ベジェ曲線（引数のベジェの値だけ曲線を描く）
//************************************************************************************************
void SetBezier(CHARACTER* ch , POINT* pos , int bezier , float time){
	
	int i = 0;

	//▼X座標
	//始点座標を求める
	float sx = pos[0].x * (float)(pow((1 - time) , bezier));
	//中間座標
	float cx = 0.0f;
	//終点座標を求める
	float ex = pos[bezier].x * (float)(pow(time , bezier));			
	
	//▼Y座標
	//始点座標を求める
	float sy = pos[0].y * (float)(pow((1 - time) , bezier));
	//中間座標
	float cy = 0.0f;
	//終点座標を求める
	float ey = pos[bezier].y * (float)(pow(time , bezier));					

	//始点を代入
	ch->x = sx;			//X
	ch->y = sy;			//Y

	//中心座標を求める
	for(i = 1;i < bezier;i++){
		//X座標
		ch->x += pos[i].x * bezier * (float)(pow((1 - time) , (bezier - i)))
			* (float)(pow(time , i));
		//Y座標
		ch->y += pos[i].y * bezier * (float)(pow((1 - time) , (bezier - i)))
			* (float)(pow(time , i));
	}
	//終点を最後に加算
	ch->x += ex;		//X
	ch->y += ey;		//Y
}


//************************************************************************************************
//渡された２つの値から、範囲内でランダムの値を返す
//************************************************************************************************
int RandomNum(int max,int min){

	int r = 0;
	
	//min～maxの範囲で乱数を取得
	r = rand() % ( max - min + 1 ) + min;
	//渡された値の範囲内のランダムの値を返す
	return r;
}


//************************************************************************************************
//キャラクター同士の矩形が重なっていたら、方向に応じて座標を修正する（重ならないようにする）
//************************************************************************************************
BYTE CheckOverlapCharacter( CHARACTER* pch1 , CHARACTER* pch2 , TSAFETYRECT<int> safetyRect ){

	//戻り値用
	BYTE	ret = CHARACTER_NOT_BUMP;

	//幅高さを拡縮倍率通りに計算
	//キャラクター1
	float chara1RectW = pch1->w * pch1->scale;			//幅
	float chara1RectH = pch1->h * pch1->scale;			//高さ

	//キャラクター２
	float chara2RectW = pch2->w * pch2->scale;			//幅
	float chara2RectH = pch2->h * pch2->scale;			//高さ

	//pch1の左端とpch2の右端の矩形で判定
	if( ( pch1->x + safetyRect.left ) < pch2->x + chara2RectW &&
		( pch1->x + safetyRect.left ) > pch2->x &&
		( pch1->y + safetyRect.up ) < pch2->y + chara2RectH &&
		( pch1->y + chara1RectH ) - safetyRect.down > pch2->y ){

		//pch2をpch1の左端にくっつけた位置に修正する
		pch2->x = ( pch1->x - chara2RectW ) + safetyRect.left;

		//衝突した面を返す
		ret = CHARACTER_BUMP_LEFT;
	}				
/*
	//pch1の上端とpch2の下端の矩形で判定
	if( ( pch1->x + chara1RectW ) - safetyRect.right > pch2->x &&
		( pch1->x + safetyRect.left ) < pch2->x + chara2RectW &&
		( pch1->y + safetyRect.up ) > pch2->y &&
		( pch1->y + safetyRect.up ) < pch2->y + chara2RectH ){
		

		//pch2をpch1の上端にくっつけた位置に修正する
		pch2->y = ( pch1->y - chara2RectH ) + safetyRect.up;

		//衝突した面を返す
		ret = CHARACTER_BUMP_TOP;
	}

*/
	//pch1の右端とpch2の左端の矩形で判定
	if( ( pch1->x + chara1RectW ) - safetyRect.right < pch2->x + chara2RectW &&
		( pch1->x + chara1RectW ) - safetyRect.right > pch2->x &&
		( pch1->y + safetyRect.up ) < pch2->y + chara2RectH &&
		( pch1->y + chara1RectH ) - safetyRect.down > pch2->y ){

		//pch2をpch1の右端にくっつけた位置に修正する
		pch2->x = ( pch1->x + chara1RectW ) - safetyRect.right;

		//衝突した面を返す
		ret = CHARACTER_BUMP_RIGHT;
	}

	//pch1の下端とpch2の上端の矩形で判定
	if( ( pch1->x + chara1RectW ) - safetyRect.right > pch2->x &&
		( pch1->x + safetyRect.left ) < pch2->x + chara2RectW &&
		( pch1->y + chara1RectH ) - safetyRect.down > pch2->y &&
		( pch1->y + chara1RectH ) - safetyRect.down < pch2->y + chara2RectH ){

		//pch2をpch1の下端にくっつけた位置に修正する
		pch2->y = ( pch1->y + chara1RectH ) - safetyRect.down;

		//衝突した面を返す
		ret = CHARACTER_BUMP_BOTTOM;
	}

	//衝突した面を返す
	return ret;

}


//************************************************************************************************
//引数のキャラクターの索敵範囲と引数のキャラクターの矩形で判定を取る　BroadenCollisionSquare
//引数１　＝　自身の矩形で判定を取るキャラクター
//引数２　＝　自身の周りに判定を広げるキャラクター
//************************************************************************************************
bool BroadenCollisionSquare( CHARACTER* pch1 , CHARACTER* pch2 ){


	//引数２の中心座標を求める
	float pch2CenterX = pch2->x + ( ( pch2->w * pch2->scale ) / 2 );		//X座標
	float pch2CenterY = pch2->y + ( ( pch2->h * pch2->scale ) / 2 );		//Y座標

	//索敵範囲を計算用に変換する
	int pch2RectW = pch2->SearchSquareWidth / 2;							//幅

	//引数1のキャラクターの矩形と引数2のキャラクターが広げる索敵範囲とで判定を取る
	//索敵範囲を広げるキャラクターは中心座標を基点とする
	if( pch1->x < pch2CenterX + (float)pch2RectW &&
	   pch1->x + (float)( pch1->w * pch1->scale ) > pch2CenterX - (float)pch2RectW &&
	   pch1->y < pch2CenterY + (float)pch2->SearchSquareFront &&
	   pch1->y + (float)( pch1->h  * pch1->scale ) > pch2->y ){		
	 
		  //重なっていたらtrueを返す
		  return true;
	}

	//重なっていなかったらfalseを返す
	return false;

}

//************************************************************************************************
//後方カメラ視点に合わせて引数のキャラクターの拡縮率を適した値に変更する
//************************************************************************************************
void FittingPositionToBackCameraAngle( CHARACTER* pch , float scaleMax ){

	//カメラの座標を取得
	float comeCameraY = GetCameraY();

	//pch2のローカル座標を求める
	float localCameraY = pch->y - comeCameraY;

	//▼　手前に来るほどに大きく見せる処理

	//pch2のローカル座標が縦画面サイズの、
	//どの割合に位置しているかを計算
	float localRateY = localCameraY / 480.0f;

	//画面上端から出てから拡縮率に加算していく
	if( pch->y + ( pch->h * pch->scale ) >= comeCameraY ){
		//現在の位置に応じて対応する拡縮率を代入
		pch->scale = pch->defaultScale + ( scaleMax * localRateY );
	}
	//上限値
	if( pch->scale >= pch->defaultScale + scaleMax ){
		//画面上端の道路の道幅と画面下端の道幅の差分倍率を等倍に加算
		pch->scale = pch->defaultScale + scaleMax;
	}

}


/**************************************************************************************************
引数の値の確率でtrueを取得する
引数１　：　取得したい確率の整数（1～100まで）
******************************************************************************************************/
bool GetLikelihood( char num ){

	//ランダムの値を取得する
	char compairNum = RandomNum( 100 , 1 );

	//戻り値用
	bool ret = false;

	//取得したランダムの値が引数以下ならtrueを返す
	if( compairNum <= num ){

		ret = true;
	}

	//最終的な結果を返す
	return ret;
}
/*
Inventor replaces material with Host's material, that Host make space for inventor
return is A replaed value with B, that matrix of materials
*/
int*	ReplaceValue(int inventor, int replaceMaterial, int* lpHost, int hostMax) {
	
	int replace = 0;
	//count
	int next = 0;
	//next item
	next = (replaceMaterial + 1);
	//subsutitute value for matrix that starting point of replace process
	lpHost[replaceMaterial] = inventor;
	for (int j = replaceMaterial; j < hostMax; j++) {		//loop to hostMax
		replace = lpHost[j];
		if (next < hostMax) {
			//replace material with lpHost[]			
			lpHost[j] = lpHost[next];
			lpHost[next] = replace;
		}
		next = (j + 2);						//this is subsutitution material
	}
	return lpHost;
}