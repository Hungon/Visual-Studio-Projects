#include "map.h"
#include "swimer.h"
#include "seaenemy.h"
#include "input.h"
#include <stdio.h>


//▼　グローバル変数の宣言
//Normal number image
SCOREVALUE g_NumberNormal[SCORE_NUMBER_TYPE_NORMAL_KIND_OF_TYPE] = { 0 };
//Speed number image
SCOREVALUE g_NumberSpeed[SCORE_NUMBER_TYPE_SPEED_KIND_OF_TYPE] = { 0 };
//SCOREVALUE型変数「評価画像を扱う」
SCOREVALUE g_value[SCORE_VALUE_MAX] = {0};
//ranking image
SCOREVALUE g_NumberRanking = { 0 };

//画像番号を扱う
int g_ScoreImg[SCORE_IMAGE_MAX] = {0};

//スコア画像の表示済みフラグ
char	g_ScoreValueFlag = NO_VALUE;

//************************************************************************************************
//初期化
//************************************************************************************************
void InitScore(HWND hWnd) {

	//画像データ
	char	*lpImageTbl[SCORE_IMAGE_MAX] = {
		{ "Image\\NormalNumber.bmp" },				//normal number
		{ "Image\\SpeedNumber.bmp" },				//speed number
		{ "Image\\Scorevalue.bmp"},					//value image
		{ "Image\\Road\\Ranking.bmp"},				//ranking image
	};

	//▼　画像の読み込み
	for (int i = 0; i < SCORE_IMAGE_MAX; i++) { g_ScoreImg[i] = CreateDDImage(lpImageTbl[i], hWnd); }

	//スコアをリセット
	ZeroMemory(&g_NumberNormal[0], sizeof(SCOREVALUE) * SCORE_NUMBER_TYPE_NORMAL_KIND_OF_TYPE);		//normal
	ZeroMemory(&g_NumberSpeed[0], sizeof(SCOREVALUE) * SCORE_NUMBER_TYPE_SPEED_KIND_OF_TYPE);		//speed
	//評価をリセット
	ZeroMemory(&g_value[0], sizeof(SCOREVALUE) * SCORE_VALUE_MAX);
	ZeroMemory(&g_NumberRanking, sizeof(SCOREVALUE));												//ranking

	//initialize each number image
	//normal number
	//each origin indication X
	int srcXTbl1[SCORE_NUMBER_TYPE_NORMAL_KIND_OF_TYPE] = {
		0,													//number
		SCORE_NUMBER_AND_PUNCTUAL_IMAGE_WIDTH * 10,			//[:]
	};
	for (int i = 0; i < SCORE_NUMBER_TYPE_NORMAL_KIND_OF_TYPE; i++) {
		g_NumberNormal[i].ch.h = SCORE_NUMBER_AND_PUNCTUAL_IMAGE_HEIGHT;				//高さ
		g_NumberNormal[i].ch.w = SCORE_NUMBER_AND_PUNCTUAL_IMAGE_WIDTH;					//幅
		g_NumberNormal[i].ch.picx = srcXTbl1[i];										//表示元Ｘ
		g_NumberNormal[i].ch.picy = 0;													//表示元Ｙ
	}
	//speed number
	//each width
	int widthTbl1[SCORE_NUMBER_TYPE_SPEED_KIND_OF_TYPE] = {
		SCORE_NUMBER_AND_PUNCTUAL_IMAGE_WIDTH,				//number image
		SCORE_NUMBER_AND_PUNCTUAL_IMAGE_WIDTH * 2,			//km image
		SCORE_NUMBER_AND_PUNCTUAL_IMAGE_WIDTH,				//[/]image
		SCORE_NUMBER_AND_PUNCTUAL_IMAGE_WIDTH,				//[.]image
	};
	//each origin indication X
	int srcXTbl2[SCORE_NUMBER_TYPE_SPEED_KIND_OF_TYPE] = {
		0,													//number
		SCORE_NUMBER_AND_PUNCTUAL_IMAGE_WIDTH * 10,			//km
		(SCORE_NUMBER_AND_PUNCTUAL_IMAGE_WIDTH * 10) +		//[/]
		(SCORE_NUMBER_AND_PUNCTUAL_IMAGE_WIDTH * 2),		
		(SCORE_NUMBER_AND_PUNCTUAL_IMAGE_WIDTH * 10) +		//[.]
		(SCORE_NUMBER_AND_PUNCTUAL_IMAGE_WIDTH * 3),
	};
	for (int i = 0; i < SCORE_NUMBER_TYPE_SPEED_KIND_OF_TYPE; i++) {
		g_NumberSpeed[i].ch.h = SCORE_NUMBER_AND_PUNCTUAL_IMAGE_HEIGHT;	//高さ
		g_NumberSpeed[i].ch.w = widthTbl1[i];							//幅
		g_NumberSpeed[i].ch.picx = srcXTbl2[i];							//表示元Ｘ
		g_NumberSpeed[i].ch.picy = 0;									//表示元Ｙ
	}
	
	//▼　評価画像の設定
	//画像サイズ
	int	widthTbl2[SCORE_VALUE_MAX] = {
		SCORE_VALUE_IMAGE_WIDTH_COMMON,							//CHAIN
		SCORE_VALUE_IMAGE_WIDTH_COMMON,							//GOOD
		SCORE_VALUE_IMAGE_WIDTH_COMMON,							//COOL
		SCORE_VALUE_IMAGE_WIDTH_EXCELLENT						//EXCELLENT
	};
	for (int i = 0; i < SCORE_VALUE_MAX; i++) {
		g_value[i].ch.x = SCORE_VALUE_COMMON_STAY_INDICATION_X;		//待機座標X
		g_value[i].ch.h = SCORE_VALUE_IMAGE_HEIGHT_COMMON;			//高さ
		g_value[i].ch.w = widthTbl2[i];								//幅
		g_value[i].ch.picy = 0;										//表示元Ｙ
		g_value[i].ch.scale = SCORE_VALUE_IMAGE_SCALE_COMMON;		//拡縮率
		g_value[i].ch.flag = false;									//生存フラグ
		g_value[i].ch.picy = (SCORE_VALUE_IMAGE_HEIGHT_COMMON * i);	//表示元Y
		g_value[i].ch.mx = SCORE_VALUE_DEFAULT_SPEED_X;				//移動量X
	}

	//ranking image in Road
	g_NumberRanking.ch.h = SCORE_NUMBER_RANKING_IMAGE_HEIGHT;
	g_NumberRanking.ch.w = SCORE_NUMBER_RANKING_IMAGE_WIDTH;

}


//************************************************************************************************
//更新
//************************************************************************************************
void UpdateValueation() {

	//評価画像最大枚数だけループ
	for (int i = SCORE_GOOD; i <= SCORE_EXCELLENT; i++) {
		//表示済みなら次の要素へ
		if (g_ScoreValueFlag == i) continue;
		if (g_value[i].ch.flag) {
			//指定の座標に達したら移動量を０
			if (g_value[i].ch.x <= g_value[i].score_x) {
				g_value[i].ch.x = g_value[i].score_x;
				g_value[i].ch.time++;
				//設定したフレーム経過後消去
				if (g_value[i].ch.time >= SCORE_VALUE_STAY_TIME) {
					g_value[i].ch.time = 0;
					g_value[i].ch.flag = false;
					//スコア表示済みフラグに対応した要素番号を代入する
					g_ScoreValueFlag = i;
					break;
				}
			}
			//スコアの移動量の加算
			g_value[i].ch.x -= g_value[i].ch.mx;	//X座標
		}
	}
}


//************************************************************************************************
//画像でスコアを表示する関数
//************************************************************************************************
void DrawScore(SCOREVALUE* pscore) {

	//扱うスコア画像のデータテーブル
	int numberImg[2] = {

		g_ScoreImg[0] ,		//時間などの各種項目の画像
		g_ScoreImg[1] ,		//速度用の画像
	};

	//計算した回数をカウント(桁を扱う)
	int digit = 0;

	//求まった値の余りを扱う
	int rest = 0;

	//表示する桁の表示元を扱う
	int num_d = 0;

	//引数の値を受け取る
	float num = pscore->num;

	//渡された値が負の値なら正の値にする
	if (num < 0) {
		num *= -1;
	}

	//▼スコアの色を変更する
	//表示元Ｙ座標のデータテーブル
	int scoreSrcTbl[SCORE_COLOR_VALUATION] = {

		0						,								//黒
		SCORE_NUMBER_AND_PUNCTUAL_IMAGE_HEIGHT		,			//赤
		SCORE_NUMBER_AND_PUNCTUAL_IMAGE_HEIGHT * 2	,			//青
		SCORE_NUMBER_AND_PUNCTUAL_IMAGE_HEIGHT * 3	,			//黄
		SCORE_NUMBER_AND_PUNCTUAL_IMAGE_HEIGHT * 4	,			//緑
	};

	//スコア色変更
	int scoreColorSrc = scoreSrcTbl[pscore->scoreColor];

	//▼斜体フラグ
	int scoreItalic[2] = {
		0 ,											//vertical
		SCORE_NUMBER_AND_PUNCTUAL_IMAGE_HEIGHT ,	//italic
	};

	//小数を受け取る
	float dicimals_num = num - (int)num;

	//小数点第二位まで整数にする
	dicimals_num *= 100;

	//演出終了なら値を代入する
	if (pscore->directEnd) num = pscore->num;

	//徐々に加算減算フラグなら
	if (pscore->gradually && pscore->directEnd == false) {

		num = (float)GraduallyNumber(pscore);
	}

	//更新フラグが立ったスコアなら、表示したい値以上になったら更新フラグが立ったスコアだけスコアの色を変更する
	if (pscore->renealF == RECORD_RENEWAL_FLAG && pscore->directEnd == true) {

		scoreColorSrc = scoreSrcTbl[pscore->renewalColor];
	}

	//更新フラグなしなら、即座に色を反映させる
	if (pscore->renealF == RECORD_NOT_RENEWAL) {

		scoreColorSrc = scoreSrcTbl[pscore->scoreColor];
	}

	//▼整数の値まで表示
	//渡された引数の数値の各桁の数値を調べる
	do {

		//余りの数値が求める桁の数値
		rest = (int)num % 10;
		//渡された値を毎回10で割っていく
		num /= 10;
		//表示する数の表示元を扱う
		num_d = g_NumberNormal[0].ch.w * rest;

		//数値画像の表示
		DDScaleBlt(pscore->score_x - ((g_NumberNormal[0].ch.w * pscore->score_scale) * digit),
			pscore->score_y,
			g_NumberNormal[0].ch.w,
			g_NumberNormal[0].ch.h,
			numberImg[pscore->ch.type],
			g_NumberNormal[0].ch.picx + num_d,
			g_NumberNormal[0].ch.picy + scoreColorSrc + scoreItalic[pscore->italic],
			pscore->score_scale);

		//計算する毎にカウント
		digit++;

		//渡された値が0未満になったら抜ける
	} while ((int)num > 0 || digit < pscore->digit);


	//▼小数の値を表示
	if (pscore->dicimals_flag || pscore->ch.type == SCORE_NUMBER_TYPE_SPEED) {

		//ドット画像の表示
		DDScaleBlt(pscore->score_x + (g_NumberNormal[1].ch.w * pscore->score_scale),
			pscore->score_y,
			g_NumberSpeed[3].ch.w,
			g_NumberSpeed[3].ch.h,
			numberImg[pscore->ch.type],
			g_NumberSpeed[3].ch.picx,
			g_NumberSpeed[3].ch.picy + scoreColorSrc + scoreItalic[pscore->italic],
			pscore->score_scale);

		//渡された引数の数値の各桁の数値を調べる
		do {

			//余りの数値が求める桁の数値
			rest = (int)dicimals_num % 10;

			//渡された値を毎回10で割っていく
			dicimals_num /= 10;

			//表示する数の表示元を扱う
			num_d = g_NumberNormal[0].ch.w * rest;

			//数値画像の表示
			DDScaleBlt(pscore->score_x + ((g_NumberNormal[0].ch.w * pscore->score_scale) * digit),
				pscore->score_y,
				g_NumberNormal[0].ch.w,
				g_NumberNormal[0].ch.h,
				numberImg[pscore->ch.type],
				g_NumberNormal[0].ch.picx + num_d,
				g_NumberNormal[0].ch.picy + scoreColorSrc + scoreItalic[pscore->italic],
				pscore->score_scale);

			//桁表示が引数の設定した値になったらループを抜ける
			if (digit == pscore->digit) break;

			//計算する毎にカウント
			digit++;

			//計算しきったら抜ける
		} while ((int)dicimals_num > 0);

	}


	//速度記号の表示
	if (pscore->kiloFlag) {

		//スラッシュを表示
		DDScaleBlt(pscore->score_x + (g_NumberNormal[0].ch.w * pscore->score_scale) * digit,
			pscore->score_y,
			g_NumberSpeed[2].ch.w,
			g_NumberSpeed[2].ch.h,
			numberImg[pscore->ch.type],
			g_NumberSpeed[2].ch.picx,
			g_NumberSpeed[2].ch.picy + scoreSrcTbl[pscore->scoreColor] + scoreItalic[pscore->italic],
			pscore->score_scale);


		//「Km」を表示
		DDScaleBlt(pscore->score_x + (g_NumberNormal[0].ch.w * pscore->score_scale) * digit,
			pscore->score_y,
			g_NumberSpeed[1].ch.w,
			g_NumberSpeed[1].ch.h,
			numberImg[pscore->ch.type],
			g_NumberSpeed[1].ch.picx,
			g_NumberSpeed[1].ch.picy + scoreSrcTbl[pscore->scoreColor] + scoreItalic[pscore->italic],
			pscore->score_scale);
	}

	//▼　評価画像の表示
	for (int i = 0; i < SCORE_VALUE_MAX; i++) {
		//表示フラグ
		if (g_value[i].ch.flag) {
			//拡縮で表示
			DDScaleBltSelectIndicate(
				g_value[i].ch.x,
				g_value[i].ch.y,
				g_value[i].ch.w,
				g_value[i].ch.h,
				g_ScoreImg[2],
				g_value[i].ch.picx,
				g_value[i].ch.picy,
				g_value[i].ch.scale,
				DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP);
		}
	}
}

//************************************************************************************************
//解放
//************************************************************************************************
void ReleaseScore() {

	//使用した枚数だけ解放
	for (int i = 0; i < SCORE_IMAGE_MAX; i++) {
		ReleaseDDImage(g_ScoreImg[i]);
	}
}

//************************************************************************************************
//指定した座標に経過時間を表示する
//************************************************************************************************
void DrawTime(SCOREVALUE* ps) {

	//計算した回数をカウント(桁を扱う)
	int digit = 0;
	//求まった値の余りを扱う
	int rest = 0;
	//表示する桁の表示元を扱う
	int num_d = 0;

	//▼扱うスコア画像のデータテーブル
	int numberImg[2] = {

		g_ScoreImg[0] ,		//時間などの各種項目の画像
		g_ScoreImg[1] ,		//速度用の画像
	};

	//▼スコアの色を変更する
	//表示元Ｙ座標のデータテーブル
	int scoreSrcTbl[SCORE_COLOR_VALUATION] = {

		0						,			//黒
		g_NumberNormal[0].ch.h		,			//赤
		g_NumberNormal[0].ch.h * 2	,			//青
		g_NumberNormal[0].ch.h * 3	,			//黄
		g_NumberNormal[0].ch.h * 4	,			//緑

	};

	//反映させるスコア色
	int scoreColorSrc = scoreSrcTbl[ps->scoreColor];

	//時間計測のフラグが経っていたら時間計測
	if (ps->ti.cntFlag) {

		//ミリ秒経過
		ps->ti.millisecond++;

		//少数の値が60以上になったら繰り上げる
		if (ps->ti.millisecond >= 60) {

			//フレーム数をリセット
			ps->ti.millisecond = 0;

			//秒をカウント
			ps->ti.second++;
		}

		//秒が60以上になったら繰り上げる
		if (ps->ti.second >= 60) {

			//秒をリセット
			ps->ti.second = 0;

			//分をカウント
			ps->ti.minutes++;
		}

		//分が60以上になったら繰り上げる
		if (ps->ti.minutes >= 60) {

			//分をリセット
			ps->ti.minutes = 0;

			//時をカウント
			ps->ti.hour++;
		}
	}

	//表示する値
	int time[3] = {

		ps->ti.millisecond		,		//ミリ秒
		ps->ti.second			,		//秒
		ps->ti.minutes			,		//分

	};
	//時間経過フラグならミリ秒の値表示を99までにする
	if (ps->ti.cntFlag) time[0] *= 1.67;

	//表示先を扱う
	float numX = ps->score_x;

	//画像の幅を扱う
	float numW = g_NumberNormal[0].ch.w * ps->score_scale;

	//セミコロンの位置を取得
	float punctual = ps->spaceX + ps->spaceX / 2;

	//更新フラグが立ったスコアなら、表示したい値以上になったら更新フラグが立ったスコアだけスコアの色を変更する
	if (ps->renealF == RECORD_RENEWAL_FLAG && ps->directEnd == true) {

		scoreColorSrc = scoreSrcTbl[ps->renewalColor];
	}

	//更新フラグなしなら、即座に色を反映させる
	if (ps->renealF == RECORD_NOT_RENEWAL) {

		scoreColorSrc = scoreSrcTbl[ps->scoreColor];
	}


	//▼時間の表示
	for (int i = 0; i < 3; i++) {

		//演出終了なら値を各単位に代入する
		if (ps->directEnd) time[i] = time[i];

		//徐々に加算・減算フラグなら
		if (ps->gradually && ps->directEnd == false) {

			//値を受け取る
			ps->num = (float)time[i];

			//徐々に加算・減算する値を返す関数
			time[i] = GraduallyNumber(ps);

		}

		//渡された引数の数値の各桁の数値を調べる
		do {
			//余りの数値が求める桁の数値
			rest = time[i] % 10;

			//渡された値を毎回10で割っていく
			time[i] /= 10;

			//表示する数の表示元を扱う
			num_d = g_NumberNormal[0].ch.w * rest;

			//数値画像の表示
			DDScaleBlt((numX - (numW * digit)),
				ps->score_y,
				g_NumberNormal[0].ch.w,
				g_NumberNormal[0].ch.h,
				numberImg[ps->ch.type],
				g_NumberNormal[0].ch.picx + num_d,
				g_NumberNormal[0].ch.picy + scoreColorSrc,
				ps->score_scale);

			//計算する毎にカウント
			digit++;

			//渡された値が0未満になったら抜ける
		} while (ps->digit > digit || time[i] > 0);

		//分まで表示したらreturn
		if (i == 2)	return;

		//次に表示する座標をずらす
		numX = (numX - numW * digit) - ps->spaceX;

		//桁をリセット
		digit = 0;

		//セミコロンで区切る
		DDScaleBlt(
			numX + punctual,
			ps->score_y,
			g_NumberNormal[1].ch.w,
			g_NumberNormal[1].ch.h,
			g_ScoreImg[0],
			g_NumberNormal[1].ch.picx,
			g_NumberNormal[1].ch.picy + scoreColorSrc,
			ps->score_scale);

	}

}

//************************************************************************************************
//スコアに応じて画像を表示する
//************************************************************************************************
int DrawValueation(float x, float y, int count) {

	//現在表示中の要素を扱う
	int ret = NO_VALUE;

	//CHAINの表示先座標を設定
	g_value[SCORE_CHAIN].ch.x = x + 15.0f;
	g_value[SCORE_CHAIN].ch.y = y + ((g_value[SCORE_CHAIN].ch.h * g_value[SCORE_CHAIN].ch.scale) / 2);

	//0になったら非表示
	if (count == 0) {
		g_value[SCORE_CHAIN].ch.flag = false;					//image chain
		//スコア画像の表示済みフラグをリセット
		g_ScoreValueFlag = NO_VALUE;
	}

	//敵を倒したら表示
	if (count >= 1) {
		if (g_value[SCORE_CHAIN].ch.flag == false) {
			g_value[SCORE_CHAIN].ch.flag = true;				//image chain
		}

		//敵を倒した数に応じて画像を表示
		//5の倍数で表示
		for (int i = SCORE_GOOD; i <= SCORE_EXCELLENT; i++) {
			//既に表示中、もしくは表示済みなら次の要素へ
			if (g_value[i].ch.flag || g_ScoreValueFlag >= i) continue;
			//表示
			if (g_value[i].ch.flag == false &&
				count >= (i * 5)) {

				g_value[i].score_x = x - 
					(SCORE_VALUE_IMAGE_WIDTH_COMMON * SCORE_VALUE_IMAGE_SCALE_COMMON);		//表示先X
				g_value[i].score_y = y;												//表示先Y
				g_value[i].ch.flag = true;													//生存フラグ
				g_value[i].ch.x = SCORE_VALUE_COMMON_STAY_INDICATION_X;						//待機X座標
				g_value[i].ch.y = y +
					(g_value[SCORE_CHAIN].ch.h * g_value[SCORE_CHAIN].ch.scale);			//Y座標
				ret = i;																	//表示中の画像要素番号
				break;
			}
		}
	}
	//最終的に表示中の画像の要素番号を返す
	return ret;
}


//************************************************************************************************
//目標の数値まで徐々に加算（減算）する
//************************************************************************************************
int GraduallyNumber( SCOREVALUE* pscore ){

	//加算形式の演出分岐
	void (*pgraduallyDirect[2])( SCOREVALUE* ) = {
	
		GraduallyDirectNormal	,			//フレーム毎に加算・減算する演出
		GraduallyDirectRolling	,			//指定フレーム経過中、回転の演出した後、表示
	};
	
	//▼表示する値と表示したい値が違ったら処理を行う
	//もしくは回転の処理がまだ続いていたら実行する
	if( pscore->directEnd == false ){
		//演出の分岐
		pgraduallyDirect[pscore->graduallyDirect]( pscore );

	}

	//▼数値の上下下限値の設定
	//下限は0
	if( (int)pscore->num < 0 ){
		pscore->num = 0.0f;
	}
	//上限は1億未満
	if( (int)pscore->num > 100000000 ){
		pscore->num = 99999999.0f;
	}

	//最後に表示する値を返す
	return (int)pscore->numD;

}


//************************************************************************************************
//加算・減算の演出１「徐々に加算・減算した後、表示」
//************************************************************************************************
void GraduallyDirectNormal(SCOREVALUE* pscore) {

	//表示する値が表示したい値より小さかったら増加する
	if (pscore->numD <= pscore->num) {

		//表示する値と表示したい値の差分を求める
		pscore->sub = pscore->num - pscore->numD;

		//フレーム毎に加算する
		pscore->numD += (pscore->sub + (pscore->interval / 10)) / pscore->interval;

		//表示したい値が表示する値から差分を引いた値付近になったら
		//表示したい値を代入する
		//もしくは、「Zキーか①ボタン」を押したら値を代入
		if (pscore->numD >= (pscore->num - (pscore->sub + pscore->interval) / pscore->interval) ||
			pscore->numD >= (pscore->num / 2) &&
			JustKey('Z') || JustButton(BUTTON_1)) {

			//最終的に表示する値の代入
			pscore->numD = pscore->num;
			//演出終了フラグ
			pscore->directEnd = true;
		}
	}

	//表示する値が表示したい値より大きかったら減少する
	if (pscore->numD >= pscore->num) {

		//表示する値と表示したい値の差分を求める
		pscore->sub = pscore->numD - pscore->num;

		//フレーム毎に減算する
		pscore->numD -= (pscore->sub + (pscore->interval / 10)) / pscore->interval;

		//表示したい値が表示する値から差分を引いた値付近になったら
		//表示したいを値を代入する
		if (pscore->numD <= (pscore->num + (pscore->sub + pscore->interval) / pscore->interval) ||
			pscore->numD >= (pscore->num / 2) &&
			JustKey('Z') || JustButton(BUTTON_1)) {

			//最終的に表示する値の代入
			pscore->numD = pscore->num;
			//演出終了フラグ
			pscore->directEnd = true;

		}
	}
}

//************************************************************************************************
//加算・減算の演出２「指定したフレームだけ回転後、値を表示」
//************************************************************************************************
void GraduallyDirectRolling(SCOREVALUE* pscore) {

	//フレーム経過
	pscore->ch.time++;

	//表示する上限の数値を扱う
	int unsigned terminateNum = 1;
	//上限の桁数を計算
	for (int i = 0; i < pscore->digit; i++) { terminateNum *= 10; }
	//毎フレーム加算する値を求める
	int add = ((terminateNum - 1) / 9);

	//指定したフレームに経過するまでスコアの表示を回転状態にする
	if (pscore->ch.time < pscore->rollTime) {
		//表示する値を加算する
		pscore->numD += add;
		//桁の位まで表示する
		if (pscore->numD >= terminateNum) pscore->numD = 0.0f;
	}
	//経過フレームが指定フレームに達したら、値を代入して表示する
	//もしくは、決定キーが押されたら代入
	if (pscore->ch.time >= 20) {				//rolling process at least 20 fram
		if (pscore->ch.time >= pscore->rollTime ||
			JustKey('Z') || JustButton(BUTTON_1)) {
			//表示する値に代入する
			pscore->numD = pscore->num;
			//演出終了フラグ
			pscore->directEnd = true;
		}
	}
}

/*
draw player's ranking in road stage
*/
void	DrawRanking(float x, float y, float scale, int ranking, tagSCORERANKINGCOLOR color) {

	//indication way is scaleBlt
	DDScaleBltSelectIndicate(
		x,
		y,
		g_NumberRanking.ch.w,
		g_NumberRanking.ch.h,
		g_ScoreImg[3],
		g_NumberRanking.ch.picx + (SCORE_NUMBER_RANKING_IMAGE_WIDTH * color),
		g_NumberRanking.ch.picy + (SCORE_NUMBER_RANKING_IMAGE_HEIGHT * (ranking - 1)),
		scale,
		DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP);
}