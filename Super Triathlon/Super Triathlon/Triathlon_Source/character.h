#ifndef __CHARACTER_H__
#define __CHARACTER_H__


//▼マクロの定義
#define D_RIGHT 0x00		//右向き
#define D_LEFT	0x01		//左向き

//衝突する面を扱う
#define CHARACTER_NOT_BUMP		-1			//衝突なし
#define CHARACTER_BUMP_TOP		0x00		//上
#define CHARACTER_BUMP_BOTTOM	0x01		//下
#define CHARACTER_BUMP_LEFT		0x02		//左
#define CHARACTER_BUMP_RIGHT	0x03		//右


//▼ヘッダーをインクルード
#include <windows.h>

//▼構造体の定義
//衝突時の安全矩形の指定用
template < typename T >
struct TSAFETYRECT{
	T	up;
	T	right;
	T	down;
	T	left;
};


//キャラクターの座標を小数で扱う
typedef struct tagFPOSITION{

	float fX;			//X座標
	float fY;			//Y座標

} FPOSITION;

//１体のキャラクターを表現するデータをまとめた
//CHARACTER構造体
typedef struct tagCHARACTER{

	float x;				//Ｘ座標
	float y;				//Ｙ座標
	float sx;				//始点X座標
	float sy;				//始点Y座標
	float ex;				//終点X座標
	float ey;				//終点Y座標
	float mx;				//Ｘ移動量
	float my;				//Ｙ移動量
	float smx;				//始点Ｘ移動量
	float smy;				//始点Ｙ移動量
	float default_mx;		//初期Ｘ移動量
	float default_my;		//初期Ｙ移動量
	int w;					//幅
	int h;					//高さ
	float scale;			//拡縮の倍率
	float defaultScale;		//拡縮率の初期値
	float alpha;			//透過値
	int picx;				//表示元Ｘ座標
	int picy;				//表示元Ｙ座標
	bool flag;				//生存フラグ
	int direction;			//方向
	int direction_now;		//現在の方向
	int time;				//時間を扱う
	float millisecond;			//微細な時間を扱う
	int hp;					//体力
	float speed;			//速度
	float defaultSpeed;		//速度初期値
	int bitnum;				//描画時に扱う要素数
	int angle;				//角度
	int cnt;				//カウント
	int type;				//種類番号
	int SearchSquareFront;	//前方に広げる索敵範囲
	int SearchSquareWidth;	//広げる索敵範囲の幅


} CHARACTER,*LPCHARACTER;


//▼プロトタイプ宣言

//当たり判定を行う関数
bool CollisionCharacter(CHARACTER* pch1,CHARACTER* pch2);

//一定時間回避状態
bool AvoidEnemy(CHARACTER* pch , int avoidtime);

//多次元ベジェ曲線
void SetBezier(CHARACTER* ch , POINT* pos , int bezier , float time);

//キャラクターの中心座標を求める
void CenterCharacter(CHARACTER* ch , int len , int angle);

//渡された最大値と最小値を元に、ランダムの値を返す関数
int RandomNum(int max,int min);

//キャラクター同士の矩形で判定を取り、位置を修正する
BYTE CheckOverlapCharacter( CHARACTER *pch1 , CHARACTER *pch2, TSAFETYRECT<int>	safetyRect );

//引数のキャラクターの索敵範囲と引数のキャラクターの矩形で判定を取る　BroadenCollisionSquare
//引数１　＝　自身の矩形で判定を取るキャラクター
//引数２　＝　自身の周りに判定を広げるキャラクター
bool BroadenCollisionSquare( CHARACTER* pch1 , CHARACTER* pch2 );

//後方カメラ視点に合わせて引数のキャラクターの拡縮率を適した値に変更する
void FittingPositionToBackCameraAngle( CHARACTER* pch , float scaleMax );

/*
引数の値の確率でtrueを取得する
引数１　：　取得したい確率の整数（1～100まで）
*/
bool GetLikelihood( char num );
/*
Inventor replaces material with Host's material, that Host make space for inventor
return is A replaed value with B, that matrix of materials
*/
int*	ReplaceValue(int inventor, int replaceMaterial, int* lpHost, int hostMax);

#endif