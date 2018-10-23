#ifndef  __SWIMER_H__
#define  __SWIMER_H__



//▼ヘッダーファイルの読み込み
#include	"graphics.h"
#include	"animation.h"
#include	<windows.h>

//▼マクロの定義
//画像設定用
#define SWIMER_IMAGE_MAX			5		//使用枚数
#define SWIMER_ANIMATION_TYPE		7		//画像タイプの種類
#define SWIMER_ATTACK_TYPE_MAX		4		//攻撃タイプ分岐点の数

//通常時のサイズ
const int SWIMER_DEFAULT_IMAGE_WIDTH = 104;
const int SWIMER_DEFAULT_IMAGE_HEIGHT = 40;

//回転攻撃と突進攻撃用画像サイズ
const int SWIMER_ATTACK_IMAGE_HEIGHT = 96;


//▼ アニメーション設定
const int SWIMER_TYPE_NORMAL_ANIMATION_MAX			= 3;			//通常時
//各種攻撃タイプに応じた最大コマ数
const int SWIMER_TYPE_CIRCLE_ATTACK_ANIMATION_MAX	= 69;			//回転攻撃
const int SWIMER_TYPE_BUMP_ATTACK_ANIMATION_MAX		= 6;			//突進攻撃

//フレーム数
const int SWIMER_TYPE_NORMAL_ANIMATION_FRAM			= 20;			//通常時
//各種攻撃タイプに応じたフレーム数
const int SWIMER_TYPE_CIRCLE_ATTACK_ANIMATION_FRAM	= 2;			//回転攻撃
const int SWIMER_TYPE_BUMP_ATTACK_ANIMATION_FRAM	= 2;			//突進攻撃


//拡縮率
#define SWIMER_DEFAULT_SCALE		1.0f

//ステータス用
#define SWIMER_STATUS				3		//使用要素数
#define SWIMER_SCORE				1		//使用要素数

//攻撃ゲージ
#define	SWIMER_ATTACK_MAX			3		//最大攻撃回数

//ステータス可変値
//速度の初期値
#define SWIMER_DEFAULT_SPEEDX_R			-2.0f	//右スクロール時のX移動量
#define SWIMER_DEFAULT_SPEEDX_L			2.0f	//左スクロール時のX移動量
#define SWIMER_DEFAULT_SPEEDY			5.0f	//Y移動量
#define SWIMER_DEFAULT_SPEEDMAX_RATE	3.5f	//速度倍率上限
#define SWIMER_DEFAULT_SPEEDMIN_RATE	0.5f	//速度倍率下限
#define SWIMER_DEFAULT_SPEED_RATE		1.0f	//速度倍率

//敵回避時間
#define SWIMER_DEFAULT_AVOID_TIME	100

//音用
#define SWIMER_SOUND_EFFECT_MAX		2	//効果音使用数


//▼ アニメーション分岐のビット目
//攻撃ビット目
const BYTE	SWIMER_ANIMATION_BIT_TYPE_NORMAL_ATTACK		= 0x02;		//バタフライ時
const BYTE	SWIMER_ANIMATION_BIT_TYPE_CIRCLE_ATTACK		= 0x04;		//回転時
const BYTE	SWIMER_ANIMATION_BIT_TYPE_BUMP_ATTACK		= 0x08;		//突進時

//右向き
const BYTE	SWIMER_ANIMATION_TYPE_DIRECTION_RIGHT			= 0x00;		//右向き
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_NORMAL_ATTACK		= 0x02;		//バタフライ時
//アイテム効果の分岐
//通常時
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_SPEED_UP		= 0x10;						//速度上昇時
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_ABSOLUTE		= 0x20;						//無敵時
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_SPEED_DOWN	= 0x40;						//速度減少時
//バタフライ時
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_NORMAL_ATTACK_SPEED_UP	= 0x12;			//速度上昇時
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_NORMAL_ATTACK_ABSOLUTE	= 0x22;			//無敵時
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_NORMAL_ATTACK_SPEED_DOWN	= 0x42;			//速度減少時
//左向き
//アイテム効果
//通常時
const BYTE	SWIMER_ANIMATION_TYPE_DIRECTION_LEFT			= 0x01;		//左向き
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_NORMAL_ATTACK		= 0x03;		//バタフライ
//アイテム効果
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_SPEED_UP		= 0x11;						//速度上昇時
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_ABSOLUTE		= 0x21;						//無敵時
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_SPEED_DOWN		= 0x41;						//速度減少時
//バタフライ時
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_NORMAL_ATTACK_SPEED_UP		= 0x13;			//速度上昇時
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_NORMAL_ATTACK_ABSOLUTE		= 0x23;			//無敵時
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_NORMAL_ATTACK_SPEED_DOWN	= 0x43;			//速度減少時


//アニメーションの種類
const int	SWIMER_ANIMATION_KIND_OF_TYPE	= 16;


//▼ サイズ変更攻撃
//右向き
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_CIRCLE_ATTACK		= 0x06;		//回転時
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_BUMP_ATTACK		= 0x0E;		//突進時
//回転時
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_CIRCLE_ATTACK_SPEED_UP	= 0x16;			//速度上昇時
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_CIRCLE_ATTACK_ABSOLUTE	= 0x26;			//無敵時
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_CIRCLE_ATTACK_SPEED_DOWN	= 0x46;			//速度減少時
//突進
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_BUMP_ATTACK_SPEED_UP		= 0x1E;			//速度上昇時
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_BUMP_ATTACK_ABSOLUTE		= 0x2E;			//無敵時
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_BUMP_ATTACK_SPEED_DOWN	= 0x4E;			//速度減少時

//左向き
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_CIRCLE_ATTACK		= 0x08;		//回転
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_BUMP_ATTACK		= 0x0F;		//突進
//回転時
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_CIRCLE_ATTACK_SPEED_UP		= 0x18;			//速度上昇時
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_CIRCLE_ATTACK_ABSOLUTE		= 0x28;			//無敵時
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_CIRCLE_ATTACK_SPEED_DOWN	= 0x48;			//速度減少時
//突進
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_BUMP_ATTACK_SPEED_UP		= 0x1F;			//速度上昇時
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_BUMP_ATTACK_ABSOLUTE		= 0x2F;			//無敵時
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_BUMP_ATTACK_SPEED_DOWN		= 0x4F;			//速度減少時

//攻撃時の表示元座標Y
const int	SWIMER_ANIMATION_TYPE_CIRCLE_ATTACK_PIC_Y	=	640;		//回転
const int	SWIMER_ANIMATION_TYPE_BUMP_ATTACK_PIC_Y		=	1408;		//突進


//攻撃タイプ時のアニメーション
#define SWIMER_ATTACK_ABSOLUTE	2		//無敵時
#define SWIMER_ATTACK_SPEEDSTAR 4		//速度上昇
#define SWIMER_ATTACK_SNAIL		6		//速度減少


//列挙型の定義
//プレイヤーに作用するアイテムの効果
enum tagSWIMERITEMEFFECTTYPE {

	SWIMER_ITEM_EFFECT_TYPE_VACANT = -1,	//なし
	SWIMER_ITEM_EFFECT_TYPE_SPEEDSTAR,		//速度上昇
	SWIMER_ITEM_EFFECT_TYPE_ABSOLUTE,		//無敵状態
	SWIMER_ITEM_EFFECT_TYPE_BIG,			//拡大
	SWIMER_ITEM_EFFECT_TYPE_SNAIL,			//鈍足
	SWIMER_ITEM_EFFECT_TYPE_SMALL,			//縮小
	SWIMER_KIND_OF_ITEM_EFFECT,				//効果の種類
};

//攻撃分岐用
enum tagSWIMERATTACKTYPE{

	SWIMER_ATTACK_NORMAL = -1	,		//通常時
	SWIMER_ATTACK_WAVE_ATTACK	,		//バタフライ時
	SWIMER_ATTACK_CIRCLE_ATTACK ,		//回転時
	SWIMER_ATTACK_BUMP_ATTACK	,		//突進時

};

//速度に応じたアニメーションを扱う
enum tagSWIMERANIMATIONSPEED{

	SWIMER_SPEED_SLOW	= 40 ,		//遅い	
	SWIMER_SPEED_NORMAL = 20 ,		//普通
	SWIMER_SPEED_DECENT = 15 ,		//結構速い
	SWIMER_SPEED_RAPID	= 10 ,		//速い
};


//▼構造体の定義

//SWIMER構造体
typedef struct tagSWIMER{

	CHARACTER ch;							//キャラクターの基本情報
	ANIMATION ani;							//アニメーション情報
	bool	attack;							//攻撃中フラグ
	bool	able_attack;					//攻撃可能フラグ
	bool	attack_charge;					//攻撃チャージフラグ
	int		attack_type;					//攻撃のタイプ
	tagSWIMERITEMEFFECTTYPE	itemEffectType;	//効果アイテムの種類
	float	pos_x;							//位置変更X座標
	float	pos_y;							//位置変更Y座標
	float	speedmax;						//最大加速倍数
	float	speedmin;						//最小加速倍数
	int		speedlevel;						//速度レベル
	int		avoid_time;						//敵回避時間
	int		defeat_cnt;						//敵を倒した数をカウント
	int		chain_max;						//最大連続討伐数
	int		charge;							//溜め時間

} SWIMER , *LPSWIMER;


//▼自作関数の定義

//初期化を行う関数
void    InitSwimer( HWND hWnd );
//更新を行う関数
void    UpdateSwimer();
//表示する関数
void    DrawSwimer();
//解放を行う関数
void    ReleaseSwimer();

//アニメーションを更新(通常サイズ時)
void	ChangeSwimerNormalAnimation();

//アニメーションを更新(サイズ変更時)
void	ChangeSwimerAttackAnimation();

//移動の更新
void    UpdateSwimerMove();

//速度の更新
void    UpdateSwimerSpeed();

//攻撃の更新
void    UpdateSwimerAttack();

//移動範囲を制御する
void    StrictSwimerMove();

//プレイヤーのポインタを返す関数
SWIMER* GetSwimer();

//プレイヤーの付加効果を消す
void    OrdinarySwimer();

//攻撃ゲージの回復
void    SwimerChargeAttack();

//攻撃ゲージを消費して攻撃を行う
bool	SwimerGoAttack();

//アクションタイプ０番「バタフライ」の初期化
void    InitSwimerActionType0();

//アクションタイプ1番「回転」の初期化
void    InitSwimerActionType1();

//アクションタイプ2番「ベジェ」の初期化
void    InitSwimerActionType2();

//アクションタイプ０番「バタフライ」
void    SwimerActionType0();

//アクションタイプ1番「回転」
void    SwimerActionType1();

//ベジェ曲線の動き
void    SwimerActionType2();

//効果１「速度２倍」
void    SwimerEffect0();

//効果２「無敵化」
void    SwimerEffect1();

//効果３「速度半減」
void    SwimerEffect2();

//効果４「巨大化」
void    SwimerEffect3();

//効果５「縮小」
void    SwimerEffect4();

//アイテム効果のビット目を設定する
void	SetItemEffect( BYTE item );


#endif