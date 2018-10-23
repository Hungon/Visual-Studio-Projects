#ifndef  __ENEMY_H__
#define  __ENEMY_H__


//▼マクロの定義
#define ENEMY_MAX		10			//表示するエネミーの最大数
#define ENEMY_IMG		5			//扱うエネミー画像の枚数
#define KIND_ENEMY_SEA	5			//エネミーの種類「海のステージ」
#define ENEMY_TYPE_SEA	5			//敵のアクションタイプ数

#define JELLYFISH		0				//クラゲ要素数
#define SHARK			1				//サメの要素数
#define SHOAL			2				//魚群の要素数
#define SUNFISH			3				//マンボーの要素数
#define RAY				4				//エイの要素数

#define ENEMY_ACTION_CIRCLE		0		//アクション0番「円を描く」
#define ENEMY_ACTION_STRAIGHT	1		//アクション1番「真っ直ぐ」
#define ENEMY_ACTION_WAVE		2		//アクション2番「波打つ」
#define ENEMY_ACTION_APPROACH	3		//アクション3番「一直線に接近」
#define ENEMY_ACTION_CHASE		4		//アクション4番「追跡」

//方向
const int	ENEMY_DIRECTION_RIGHT = -1;
const int	ENEMY_DIRECTION_LEFT = 1;

#define ENEMY_SOUND_EFFECT_MAX 1		//使用効果音

//▼ヘッダーファイルの読み込み
#include "graphics.h"


//▼構造体の宣言

//ENEMY構造体
typedef struct tagENEMY{

	CHARACTER ch;		               //キャラクターの基本情報を扱う構造体型変数
	float popx;			               //出現させるX座標
	float popy;			               //出現させるY座標
	int type;			               //エネミーの種類番号
	int action_type;	               //アクションタイプ
	int cnt;			               //動きを制御するカウント
	int defeat[5];		               //種類毎の倒した数（リザルト用）
	float targetx;		               //プレイヤーの座標を取得するＸ座標
	float targety;		               //プレイヤーの座標を取得するＹ座標
	bool launch;		               //発射フラグ
	int  stayTime;		               //滞在時間
	int appear;			               //出現数
	int	appearCnt;		               //出現数カウント用
	int appear_fram;	               //出現フレーム間隔
	int appear_type;	               //出現させる種類
	float appear_start_posx;	       //出現起点X座標
	float appear_end_posx;		       //消失起点X座標
	float appear_start_posx_r;	       //右スクロール時の出現起点座標Ｘ
	float appear_end_posx_r;           //右スクロール時の消失起点座標Ｘ
	//クラゲの挙動用
	int		circleCnt;				   //回転時のカウント
	//魚群の挙動用
	int		waveCnt;				   //波打つ動きのカウント

} ENEMY,*LPENEMY;



//▼自作関数を定義

//初期化を行う関数
void InitEnemy(HWND hWnd);
//更新を行う関数
void UpdateEnemy();
//表示を行う関数
void DrawEnemy();
//解放を行う関数
void ReleaseEnemy();
//指定された数、種類番号の敵を出現させる関数
void CreateEnemy(int appear,int type,int action);
//指定された種類に応じた行動の初期化
void InitEnemyAction0(ENEMY* pen);
void InitEnemyAction1(ENEMY* pen);
void InitEnemyAction2(ENEMY* pen);
void InitEnemyAction3(ENEMY* pen);
void InitEnemyAction4(ENEMY* pen);
//指定された種類番号によって敵の行動を変える関数
void ActionType0(ENEMY* pen);		//タイプ０番
void ActionType1(ENEMY* pen);		//タイプ１番
void ActionType2(ENEMY* pen);		//タイプ２番
void ActionType3(ENEMY* pen);		//タイプ３番
void ActionType4(ENEMY* pen);		//タイプ４番
//指定の敵と当たり判定を取る関数
bool CollisionEnemy(CHARACTER* ch);
//敵の情報を返す
ENEMY* GetEnemy();
/*
設定した座標を規定にした敵の出現
*/
void	SetCreationEnemyByPosition();
/*
敵の出現をランダム
*/
void	SetCreationEnemyByRandom();

#endif