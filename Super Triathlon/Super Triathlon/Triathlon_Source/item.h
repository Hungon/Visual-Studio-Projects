#ifndef __ITEM_H__
#define __ITEM_H__




//▼マクロの定義
//各アイテムの表示元X座標
#define ITEM_1		0			//速度上昇
#define ITEM_2		32			//無敵
#define ITEM_3		64			//拡大
#define ITEM_4		96			//速度減少
#define ITEM_5		128			//縮小

//▼画像
#define ITEM_IMAGE_MAX		2				//使用枚数


#define ITEM_MAX			10				//最大アイテム表示数

#define ITEM_APPEAR_RANDOM_MAX		49		//アイテムを発生乱数の上限
#define ITEM_APPEAR_RANDOM_MIN		0		//アイテムの発生乱数の下限

//音
#define ITEM_SOUND_EFFECT_MAX	3	//効果音の使用数


//▼　列挙型
//各種アイテムの種類番号を扱う
enum tagITEMTYPE{

	ITEM_VACANT = -1			,			//アイテムを持っていない状態
	ITEM_TYPE_SPEED_UP			,			//速度上昇
	ITEM_TYPE_ABSOLUTE			,			//無敵
	ITEM_TYPE_BIG				,			//拡大
	ITEM_KIND_STRENGTH			,			//強化アイテムの種類数
	ITEM_KIND_BOUND = 10		,			//強化アイテムと弱体化アイテムの境界となる要素数
	ITEM_TYPE_SPEED_DOWN = 10	,			//速度減少
	ITEM_TYPE_SMALL				,			//縮小
	ITEM_KIND_WEAK = 2			,			//弱体化アイテムの種類数
	ITEM_KIND = 5				,			//アイテムの種類数
};

//▼ヘッダーファイルの読み込み
#include "graphics.h"
#include "animation.h"
#include "swimer.h"


//▼構造体を定義
typedef struct tagITEM{
	
	CHARACTER ch;		//アイテムの基本情報
	float pos_x;		//アイテムの出現座標X
	float pos_y;		//アイテムの出現座標Y
	int type;			//アイテムの種類番号
	int time;			//各アイテムの効果時間
	int e_time;			//効果時間
	int displayX;		//取得済みのアイテムを表示X座標
	int displayY;		//取得済みのアイテムを表示Y座標
	int get[ITEM_KIND];	//種類毎の取得数
	bool among;			//効果中を扱う
	bool have;			//取得を扱う
	float speed_up;		//速度上昇
	float speed_down;	//速度減少

}ITEM,*LPITEM;


//関数の定義
void InitItem(HWND hWnd);		//初期化
void UpdateItem();				//更新
void DrawItem();				//表示
void ReleaseItem();				//解放

//▼自作関数の定義

//アイテムを出現させる関数
void CreateItem(float x,float y,int r);
//アイテムとの判定を取る関数
bool CollisionItem(SWIMER* ch);
//アイテム使用時の処理
void UseItem();
//アイテムの効果を消失させるかの判定を行う
void CheckResetEffect();
//アイテムの情報を取得する
ITEM* GetItem();


#endif

