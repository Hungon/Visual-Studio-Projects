#ifndef __SCORE_H__
#define __SCORE_H__


//▼マクロの定義
//画像
#define SCORE_IMAGE_MAX		4				//扱う画像の枚数
//ナンバーの要素数
#define SCORE_NUMBER_MAX	6
//評価の要素数
#define SCORE_VALUE_MAX		4

//kind of number image
#define SCORE_NUMBER_TYPE_NORMAL	0					//normal
#define SCORE_NUMBER_TYPE_SPEED		1					//speed
#define SCORE_NUMBER_TYPE_RANKING	2					//ranking
#define SCORE_NUMBER_KIND_OF_TYPE	3					//kind

//default value for normal number image
//kind
const char SCORE_NUMBER_TYPE_NORMAL_KIND_OF_TYPE = 2;		//normal
const char SCORE_NUMBER_TYPE_SPEED_KIND_OF_TYPE = 4;		//speed
//initialize indication
const int SCORE_NUMBER_AND_PUNCTUAL_IMAGE_WIDTH = 64;	//width
const int SCORE_NUMBER_AND_PUNCTUAL_IMAGE_HEIGHT = 96;	//height

//スコア表示演出	
#define	SCORE_GRADUALLY_DIRECT_NORMAL	0	//通常の減算・加算した後、表示
#define	SCORE_GRADUALLY_DIRECT_ROLLING	1	//指定したフレーム数だけ回転後、表示


#define NO_VALUE		-1			//評価なし
#define SCORE_CHAIN		0			//CHAIN
#define SCORE_GOOD		1			//GOOD
#define SCORE_COOL		2			//COOL
#define SCORE_EXCELLENT 3			//EXCELLENT
//スコア画像の表示設定
const float SCORE_VALUE_COMMON_STAY_INDICATION_X = 700.0f;
const int	SCORE_VALUE_IMAGE_WIDTH_COMMON = 160;
const int	SCORE_VALUE_IMAGE_HEIGHT_COMMON = 96;
const int	SCORE_VALUE_IMAGE_WIDTH_EXCELLENT = 288;			//EXCELLENT
const float	SCORE_VALUE_IMAGE_SCALE_COMMON = 0.6f;
const float	SCORE_VALUE_DEFAULT_SPEED_X = 5.0f;
//スコア画像の滞在時間
const int SCORE_VALUE_STAY_TIME = 150;

//ranking image
const int SCORE_NUMBER_RANKING_IMAGE_WIDTH = 64;	//width
const int SCORE_NUMBER_RANKING_IMAGE_HEIGHT = 64;	//height


//▼列挙型
//スコアの色
enum tagSCORECOLOR{
	
	SCORE_BLACK				,		//黒
	SCORE_RED				,		//赤
	SCORE_BLUE				,		//青
	SCORE_YELOW				,		//黄
	SCORE_GREEN				,		//緑
	SCORE_COLOR_VALUATION	,		//色の種類
};

//score's color for ranking
enum tagSCORERANKINGCOLOR {
	SCORE_RANING_COLOR_BLACK,
	SCORE_RANING_COLOR_WHITE,
	SCORE_RANING_COLOR_RED,
	SCORE_RANING_COLOR_GREEN,
	SCORE_RANING_COLOR_BLUE,
	SCORE_RANING_COLOR_YELOW,
	SCORE_KIND_OF_RANKING_COLOR,
};

//スコアの文字体
enum tagSCOREDESIGN{

	SCORE_DESIGN_VIRTUALLY	,	//垂体
	SCORE_DESIGN_ITALIC		,	//斜体
};

//▼ヘッダーをインクルード
#include "graphics.h"
#include "record.h"


//スコア構造体の定義
typedef struct tagSCOREVALUE{

	CHARACTER ch;							//キャラクターを扱う構造体
	RECORD re;								//記録を扱う構造体
	TIME ti;								//時間を扱う構造体
	float num;								//表示したい値
	float numD;								//表示する値
	float sub;								//表示したい値と表示する値の差分
	int interval;							//フレームごとに表示する数値間隔
	int digit;								//表示する桁数
	float score_x;							//スコア用表示先X
	float score_y;							//スコア用表示先Y
	float score_posx;						//待機座標Ｘ
	float score_scale;						//スコア用拡縮率
	float spaceX;							//横表示間隔
	float spaceY;							//縦表示間隔
	bool dicimals_flag;						//小数表示フラグ
	bool kiloFlag;							//km表示フラグ
	bool gradually;							//徐々に値を増加するフラグ
	bool directEnd;							//演出終了フラグ
	char graduallyDirect;					//徐々に加算・減算する速度を扱う
	char scoreColor;						//表示するスコアの色
	char renewalColor;						//更新時のスコアの色
	tagSCOREDESIGN italic;					//斜体フラグ
	int rollTime;							//回転時間
	char renealF;							//表示用更新フラグ

}SCOREVALUE,*LPSCOREVALUE;


//▼プロトタイプ宣言
//初期化
void InitScore(HWND hWnd);

//更新
void UpdateValueation();

//画像でスコアを表示する
void DrawScore( SCOREVALUE* pscore );

//解放
void ReleaseScore();

//時間を表示する
void DrawTime( SCOREVALUE* ps );

//評価を表示する
int DrawValueation(float x , float y , int count);

//目標の値まで徐々に加算（減算）する
int GraduallyNumber( SCOREVALUE* pscore );

//スコア表示演出１「フレーム毎に値を加算・減算」
void GraduallyDirectNormal( SCOREVALUE* pscore );

//スコア表示演出２「指定フレームか回転後、表示」
void GraduallyDirectRolling( SCOREVALUE* pscore );
/*
draw player's ranking in road stage
*/
void	DrawRanking(float x, float y, float scale, int ranking, tagSCORERANKINGCOLOR color);

#endif