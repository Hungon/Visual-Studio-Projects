#include    "opening.h"
#include    "game.h"
#include    "input.h"
#include    "sound.h"
#include    "wipe.h"
#include    "score.h"
#include    "play.h"
#include    "result.h"
#include	"SelectStage.h"
#include	"map.h"
#include	 <stdio.h>


//▼グローバル変数
int g_openingImg[ OPENING_IMAGE_MAX ] = {0};
//タイトルロゴ用
CHARACTER	g_OpTitleLogo[ OPENING_TITLE_LOGO_KIND_OF_IMAGE ] = {0};
//各種表示設定用
BUTTON g_Opbutton[ OPENING_BUTTON_KIND_OF_IMAGE ] = {0};
//矢印の表示設定用
CHARACTER g_cursor1 = {0};

//オープニング内のシーンを扱う
//現在
BYTE	g_OpSceneFlagNow = OPENING_SCENE_TITLE_LOGO_DIRECTION;		//始めはタイトルロゴ演出
//次
BYTE	g_OpSceneFlagNext = OPENING_SCENE_TITLE_LOGO_DIRECTION;		//始めはタイトルロゴ演出


//音の読み込み
int g_op_sound[ OPENING_SOUND_EFFECT_MAX ] = {0};	//カーソル音

//************************************************************************************************
//▼初期化
//************************************************************************************************
void InitOpening(HWND hWnd) {

	//カウント用
	int cnt = 0;

	//get current language
	tagGAMELANGUAGE language = GetGameLanguage();

	//画像データ
	char	*lpImageTbl[OPENING_IMAGE_MAX] = {
		{ "Image\\title.bmp" }				,		//背景
		{ "Image\\GameTitle.bmp" }			,		//ゲームタイトル
		{ "Image\\OpeningButton.bmp" }		,		//各種ボタン
		{ "Image\\cursor.bmp" }		,				//カーソル
		{ "Image\\EventImage.bmp" }			,		//各種ステージイメージ画像
		{ "Image\\menu.bmp" }				,		//ボタン画像の背景

	};

	//各種ステージイメージ画像の画像サイズデータ
	//列要素：高さ、幅
	int		StageImageTbl[STAGE_MAX][2] = {
		{ 64 , 192 }		,			//海
		{ 128 , 110 }		,			//道路
		{ 128 , 192 }		,			//オフロード
	};

	//ステージ
	tagSELECTSTAGE stageTbl[STAGE_MAX] = { STAGE_SEA , STAGE_ROAD , STAGE_OFF_ROAD };

	//▼ クリア済ステージをリセット
	for (int i = 0; i < STAGE_MAX; i++) {
		SetCompleteStage(SELECTSTAGE_STAGE_NOT_COMPLETE, stageTbl[i]);
	}

	//▼画像の読み込み
	for (int i = 0; i < OPENING_IMAGE_MAX; i++) { g_openingImg[i] = CreateDDImage(lpImageTbl[i], hWnd); }

	//▼音データの読み込み
	//カーソル音
	g_op_sound[0] = CreateSound("SE\\cursor.wav");
	//決定音
	g_op_sound[1] = CreateSound("SE\\ok.wav");
	//キャンセル音
	g_op_sound[2] = CreateSound("SE\\cancel.wav");


	//タイトル画面での曲
	CreateStreaming("BGM\\title.wav");

	//▼ タイトルロゴ表示設定
	g_OpTitleLogo[0].picy = 0;								//case current language is Japanese
	//change title logo image accoriding to current language
	if (language == GAME_LANGUAGE_ENGLISH) g_OpTitleLogo[0].picy = (OPENING_IMAGE_TITLE_LOGO_HEIGHT * 4);
	//ゲームタイトル
	g_OpTitleLogo[0].x = OPENING_TITLE_LOGO_BIGIN_INDICATE_X;			//表示先X
	g_OpTitleLogo[0].y = OPENING_TITLE_LOGO_BIGIN_INDICATE_Y;			//表示先Y
	g_OpTitleLogo[0].w = OPENING_IMAGE_TITLE_LOGO_WIDTH;				//幅
	g_OpTitleLogo[0].h = OPENING_IMAGE_TITLE_LOGO_HEIGHT;				//高さ
	g_OpTitleLogo[0].flag = true;										//表示フラグ
	g_OpTitleLogo[0].my = OPENING_TITLE_LOGO_MOVE_Y;					///移動量Y
	g_OpTitleLogo[0].millisecond = 0;									//経過時間
	//最終的な位置
	g_OpTitleLogo[0].ey = OPENING_TITLE_LOGO_TERMINATE_INDICATE_Y;		//終点Y
	g_OpTitleLogo[0].ex = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(OPENING_IMAGE_TITLE_LOGO_WIDTH);//X
	//タイトルロゴの背後の図形
	for (int i = 4; i < OPENING_TITLE_LOGO_KIND_OF_IMAGE; i++) {
		//要素カウント
		cnt++;
		//左から延びる図形
		g_OpTitleLogo[i].x = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH
			(OPENING_IMAGE_TITLE_LOGO_WIDTH);								//表示先X
		g_OpTitleLogo[i].y = OPENING_TITLE_LOGO_TERMINATE_INDICATE_Y;		//表示先Y
		g_OpTitleLogo[i].w = OPENING_IMAGE_TITLE_LOGO_WIDTH;				//幅
		g_OpTitleLogo[i].h = OPENING_IMAGE_TITLE_LOGO_HEIGHT;				//高さ
		g_OpTitleLogo[i].picy = (OPENING_IMAGE_TITLE_LOGO_HEIGHT * cnt);	//表示元Y
		g_OpTitleLogo[i].flag = false;										//表示フラグ
	}
	//右から伸びる図形は幅は0で時間経過で伸ばしていく
	g_OpTitleLogo[6].w = 0;


	//各種ステージイメージ画像
	for (int i = 1; i < (1 + OPENING_TITLE_STAGE_IMAGING_KIND_OF_IMAGE); i++) {
		g_OpTitleLogo[i].x = OPENING_STAGE_IMAGE_CHARACTER_INDICATE_X;				//表示先X
		g_OpTitleLogo[i].y = OPENING_STAGE_IMAGE_CHARACTER_INDICATE_Y;				//表示先Y
		g_OpTitleLogo[i].ey = OPENING_STAGE_IMAGE_CHARACTER_INDICATE_Y;				//最終的な表示先Y
		g_OpTitleLogo[i].w = StageImageTbl[i - 1][1];								//幅
		g_OpTitleLogo[i].h = StageImageTbl[i - 1][0];								//高さ
		g_OpTitleLogo[i].flag = false;												//表示フラグ
		g_OpTitleLogo[i].mx = (OPENING_STAGE_IMAGE_CHARACTER_MOVE_X + (i - 1));	//移動量X
		g_OpTitleLogo[i].scale = OPENING_STAGE_IMAGE_CHARACTER_SCALE;				//拡大率
	}
	//最終的な位置
	g_OpTitleLogo[1].ex = OPENING_STAGE_IMAGE_CHARACTER_TERMINATE_INDICATE_X;
	g_OpTitleLogo[2].ex = g_OpTitleLogo[1].ex + StageImageTbl[0][1] +
		OPENING_STAGE_IMAGE_CHARACTER_SPACE_X;
	g_OpTitleLogo[3].ex = g_OpTitleLogo[2].ex + StageImageTbl[1][1] +
		OPENING_STAGE_IMAGE_CHARACTER_SPACE_X;
	//表示元Y
	g_OpTitleLogo[1].picy = 0;
	g_OpTitleLogo[2].picy = StageImageTbl[0][0];
	g_OpTitleLogo[3].picy = StageImageTbl[0][0] + StageImageTbl[1][0];

	//オープニング内のシーンを扱う
	//現在
	g_OpSceneFlagNow = OPENING_SCENE_TITLE_LOGO_DIRECTION;		//始めはタイトルロゴ演出
	//次
	g_OpSceneFlagNext = OPENING_SCENE_TITLE_LOGO_DIRECTION;		//始めはタイトルロゴ演出

	//▼ 各種ボタン画像設定
	for (int i = 0; i < OPENING_BUTTON_KIND_OF_IMAGE; i++) {
		g_Opbutton[i].ch.w = OPENING_BUTTON_IMAGE_WIDTH;				//幅
		g_Opbutton[i].ch.h = OPENING_BUTTON_IMAGE_HEIGHT;				//高さ
		g_Opbutton[i].ch.picy = (i * OPENING_BUTTON_IMAGE_HEIGHT);		//表示元Y
		g_Opbutton[i].ch.flag = false;									//表示フラグ

	}
	// [Start]
	g_Opbutton[0].ch.x = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(OPENING_BUTTON_IMAGE_WIDTH);
	g_Opbutton[0].ch.y = OPENING_BUTTON_BEGIN_IMAGE_INDICATE_Y;
	g_Opbutton[0].ch.flag = false;
	//「OPTION」
	g_Opbutton[1].ch.x = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(OPENING_BUTTON_IMAGE_WIDTH);
	g_Opbutton[1].ch.y = g_Opbutton[0].ch.y + OPENING_BUTTON_IMAGE_INDICATE_SPACE_Y;
	g_Opbutton[1].ch.flag = false;

	//難易度
	//EASY
	g_Opbutton[2].ch.x = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(OPENING_BUTTON_IMAGE_WIDTH);
	g_Opbutton[2].ch.y = OPENING_BUTTON_LEVEL_IMAGE_INDICATE_Y;
	//NORMAL
	g_Opbutton[3].ch.x = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(OPENING_BUTTON_IMAGE_WIDTH);
	g_Opbutton[3].ch.y = g_Opbutton[2].ch.y + OPENING_BUTTON_IMAGE_INDICATE_SPACE_Y;
	//HARD
	g_Opbutton[4].ch.x = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(OPENING_BUTTON_IMAGE_WIDTH);
	g_Opbutton[4].ch.y = g_Opbutton[3].ch.y + OPENING_BUTTON_IMAGE_INDICATE_SPACE_Y;

	//ボタン画像の背景
	g_Opbutton[5].ch.x = g_Opbutton[0].ch.x - OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN;
	g_Opbutton[5].ch.y = g_Opbutton[0].ch.y - OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN;
	g_Opbutton[5].ch.w = g_Opbutton[0].ch.w + (int)(OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN * 2);
	g_Opbutton[5].ch.h = (int)(((g_Opbutton[1].ch.y + g_Opbutton[1].ch.h + OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN)
		- (g_Opbutton[0].ch.y - OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN)));
	g_Opbutton[5].ch.picy = 0;
	g_Opbutton[5].ch.alpha = 100.0f;
	g_Opbutton[5].ch.flag = false;

	//カーソル
	g_cursor1.x = g_Opbutton[0].ch.x;
	g_cursor1.y = g_Opbutton[0].ch.y;
	g_cursor1.w = g_Opbutton[0].ch.w;
	g_cursor1.h = OPENING_BUTTON_IMAGE_HEIGHT;
	g_cursor1.my = OPENING_BUTTON_IMAGE_INDICATE_SPACE_Y;
	g_cursor1.flag = false;
}

//************************************************************************************************
//更新
//************************************************************************************************
void UpdateOpening() {


	//▼ 各種オープニング内の遷移
	if (g_OpSceneFlagNow == g_OpSceneFlagNext) {
		switch (g_OpSceneFlagNow) {
		case OPENING_SCENE_TITLE_LOGO_DIRECTION:
			//▼ タイトルロゴ演出の更新
			UpdateTitleLogoDirect();
			break;
		case OPENING_SCENE_SELECT_START_FUNCTION:
			//▼ ボタン設定1（Start , Option）
			SetFirstButton();
			break;
		case OPENING_SCENE_SELECT_LEVEL_FUNCTION:
			//▼ ボタン設定2 レベル設定
			SetSecondButton();
			break;
		}
	}

	//現在の遷移フラグと次の遷移フラグが違ったら時間経過
	if (g_OpSceneFlagNow != g_OpSceneFlagNext) g_OpTitleLogo[0].time++;
	//一定時間経過で次のシーンを代入
	if (g_OpTitleLogo[0].time >= OPENING_CURSOR_DONT_ACCEPT_CONTROL_TIME) {
		g_OpSceneFlagNow = g_OpSceneFlagNext;
		g_OpTitleLogo[0].time = 0;
	}

	//曲のストリーミング再生
	PlayStreaming(true);
}

//************************************************************************************************
//表示
//************************************************************************************************
void DrawOpening() {

	//オープニング画像の表示
	DDBlt(
		0, 0,						//表示座標XＹ
		640, 480,					//幅、高さ
		g_openingImg[0],			//対応した画像番号
		0, 0);						//表示元座標X・Ｙ

	//タイトルロゴの背後の図形表示
	for (int i = 4; i < OPENING_TITLE_LOGO_KIND_OF_IMAGE; i++) {
		if (g_OpTitleLogo[i].flag) {
			DDBlt(
				(int)g_OpTitleLogo[i].x,
				(int)g_OpTitleLogo[i].y,
				g_OpTitleLogo[i].w,
				g_OpTitleLogo[i].h,
				g_openingImg[1],
				g_OpTitleLogo[i].picx,
				g_OpTitleLogo[i].picy);
		}
	}

	//タイトルロゴの表示
	if (g_OpTitleLogo[0].flag) {
		DDBlt(
			(int)g_OpTitleLogo[0].x,
			(int)g_OpTitleLogo[0].y,
			g_OpTitleLogo[0].w,
			g_OpTitleLogo[0].h,
			g_openingImg[1],
			g_OpTitleLogo[0].picx,
			g_OpTitleLogo[0].picy);
	}

	//各種ステージイメージ画像の表示
	for (int i = 1; i < (1 + OPENING_TITLE_STAGE_IMAGING_KIND_OF_IMAGE); i++) {
		if (g_OpTitleLogo[i].flag) {
			DDScaleBltSelectIndicate(
				g_OpTitleLogo[i].x,
				g_OpTitleLogo[i].y,
				g_OpTitleLogo[i].w,
				g_OpTitleLogo[i].h,
				g_openingImg[4],
				g_OpTitleLogo[i].picx,
				g_OpTitleLogo[i].picy,
				g_OpTitleLogo[i].scale,
				0);
		}
	}

	//各種ボタン画像の背景
	if (g_Opbutton[5].ch.flag) {
		DDAlphaBlt(
			(int)g_Opbutton[5].ch.x,
			(int)g_Opbutton[5].ch.y,
			g_Opbutton[5].ch.w,
			g_Opbutton[5].ch.h,
			g_openingImg[5],
			g_Opbutton[5].ch.picx,
			g_Opbutton[5].ch.picy,
			g_Opbutton[5].ch.alpha);
	}


	//各種ボタン画像の表示
	for (int i = 0; i < (OPENING_BUTTON_KIND_OF_IMAGE - 1); i++) {
		//「Start」[Option]　各種レベル
		if (g_Opbutton[i].ch.flag) {
			DDBlt(
				(int)g_Opbutton[i].ch.x,
				(int)g_Opbutton[i].ch.y,
				g_Opbutton[i].ch.w,
				g_Opbutton[i].ch.h,
				g_openingImg[2],
				g_Opbutton[i].ch.picx,
				g_Opbutton[i].ch.picy);
		}
	}

	//カーソル
	if (g_cursor1.flag) {
		DDAlphaBlt((int)g_cursor1.x,
			(int)g_cursor1.y,
			g_cursor1.w,
			g_cursor1.h,
			g_openingImg[3],
			g_cursor1.picx,
			g_cursor1.picy,
			120.0f);
	}
}

//************************************************************************************************
//解放
//************************************************************************************************
void ReleaseOpening() {


	//画像の解放
	for (int i = 0; i < OPENING_IMAGE_MAX; i++) {
		ReleaseDDImage(g_openingImg[i]);
	}
	//効果音の解放
	for (int i = 0; i < OPENING_SOUND_EFFECT_MAX; i++) {
		ReleaseSound(g_op_sound[i]);
	}
	//曲の解放
	ReleaseStreaming();
}


/*
**************************************************************************************************
タイトルロゴ演出の更新
**************************************************************************************************
*/
void	UpdateTitleLogoDirect() {

	//タイトルロゴの曲線を描く座標
	POINT pos[OPENING_TITLE_LOGO_SET_BEZIER] = {
		{ OPENING_TITLE_LOGO_BIGIN_INDICATE_X ,
		  OPENING_TITLE_LOGO_BIGIN_INDICATE_Y } ,
		{ 100 , -100 } ,
		{ 150 , 50 } ,
		{ CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(OPENING_IMAGE_TITLE_LOGO_WIDTH) ,
		OPENING_TITLE_LOGO_TERMINATE_INDICATE_Y }
	};

	//タイトルロゴの移動量の更新
	if (g_OpTitleLogo[0].y < g_OpTitleLogo[0].ey) {
		//ミリ秒の経過
		g_OpTitleLogo[0].millisecond += 0.01;
		//曲線を描く
		SetBezier(&g_OpTitleLogo[0], &pos[0],
			(OPENING_TITLE_LOGO_SET_BEZIER - 1), g_OpTitleLogo[0].millisecond);
	}
	//表示先に到達したらイメージ画像の表示
	if (g_OpTitleLogo[0].y >= g_OpTitleLogo[0].ey) {
		//各種ステージイメージ画像とタイトル背景の画像を表示
		for (int i = 1; i < OPENING_TITLE_LOGO_KIND_OF_IMAGE; i++) { g_OpTitleLogo[i].flag = true; }
	}

	//タイトルの背後の図形
	//左から表示する図形
	if (g_OpTitleLogo[6].flag) g_OpTitleLogo[6].w += OPENING_TITLE_LOGO_FIGURE_DRAWING_SPEED;
	if (g_OpTitleLogo[6].w > OPENING_IMAGE_TITLE_LOGO_WIDTH) g_OpTitleLogo[6].w = OPENING_IMAGE_TITLE_LOGO_WIDTH;
	//右から表示する図形に重ねる長方形
	if (g_OpTitleLogo[5].flag) g_OpTitleLogo[5].w -= OPENING_TITLE_LOGO_FIGURE_DRAWING_SPEED;
	if (g_OpTitleLogo[5].w <= 0) g_OpTitleLogo[5].w = 0;


	//▼ 各種イメージ画像の移動量を更新
	for (int i = 1; i < (1 + OPENING_TITLE_STAGE_IMAGING_KIND_OF_IMAGE); i++) {
		//移動量の加算
		if (g_OpTitleLogo[i].flag && g_OpTitleLogo[i].x > g_OpTitleLogo[i].ex) g_OpTitleLogo[i].x += g_OpTitleLogo[i].mx;
		//移動先
		if (g_OpTitleLogo[i].x < g_OpTitleLogo[i].ex) {
			g_OpTitleLogo[i].x = g_OpTitleLogo[i].ex;
			//スイマーの画像が終着地点に到達したら遷移フラグを変更
			if (g_OpTitleLogo[1].x <= g_OpTitleLogo[1].ex) {
				g_OpSceneFlagNext = OPENING_SCENE_SELECT_START_FUNCTION;
				return;
			}
		}
	}

	// ▼「決定キー」で演出を飛ばす
	if (JustKey('Z') || JustButton(BUTTON_1)) {
		for (int i = 0; i < OPENING_TITLE_LOGO_KIND_OF_IMAGE; i++) { g_OpTitleLogo[i].flag = true; }
		for (int i = 0; i < (1 + OPENING_TITLE_STAGE_IMAGING_KIND_OF_IMAGE); i++) {
			//表示先座標
			g_OpTitleLogo[i].x = g_OpTitleLogo[i].ex;	//X
			g_OpTitleLogo[i].y = g_OpTitleLogo[i].ey;	//Y
		}
		g_OpTitleLogo[6].w = OPENING_IMAGE_TITLE_LOGO_WIDTH;
		g_OpTitleLogo[5].w = 0;
		g_OpSceneFlagNext = OPENING_SCENE_SELECT_START_FUNCTION;
	}

}

/*
**************************************************************************************************
ボタン設定1（Start , Option）
**************************************************************************************************
*/
void	SetFirstButton() {

	//背景のサイズを合わせる
	g_Opbutton[5].ch.h = (int)(((g_Opbutton[1].ch.y + g_Opbutton[1].ch.h + OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN)
		- (g_Opbutton[0].ch.y - OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN)));
	//座標
	g_Opbutton[5].ch.y = g_Opbutton[0].ch.y - OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN;
	g_Opbutton[5].ch.x = g_Opbutton[0].ch.x - OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN;
	//ボタン画像の表示フラグ
	//START OPTION表示
	for (int i = 0; i < 2; i++) { g_Opbutton[i].ch.flag = true; }
	g_Opbutton[5].ch.flag = true;				//BackImage
	//カーソル
	g_cursor1.flag = true;

	//▼カーソル移動
	//START OPTION選択時の処理
	//下入力
	if (JustKey(VK_DOWN) || JustLever(LEVER_DOWN)) {
		//カーソル音
		PlaySound(g_op_sound[0], 0, false);
		//ボタンの間隔だけ動かす
		g_cursor1.y += g_cursor1.my;
		//下の項目まで来たら上の項目へ
		if (g_cursor1.y > g_Opbutton[1].ch.y) g_cursor1.y = g_Opbutton[0].ch.y;
	}
	//上入力
	if (JustKey(VK_UP) || JustLever(LEVER_UP)) {
		//カーソル音
		PlaySound(g_op_sound[0], 0, false);
		//ボタンの間隔だけ動かす
		g_cursor1.y -= g_cursor1.my;
		//上の項目まで来たら下の項目へ
		if (g_cursor1.y < g_Opbutton[0].ch.y) g_cursor1.y = g_Opbutton[1].ch.y;
	}

	//▼キー入力
	//決定キーで各画面へ遷移
	//レベル設定
	if (g_cursor1.y == g_Opbutton[0].ch.y) {
		//カーソルの幅を項目の幅に合わせる
		g_cursor1.w = g_Opbutton[0].ch.w;
		//決定
		if (JustUpKey('Z') || JustUpButton(BUTTON_1)) {
			//決定音
			PlaySound(g_op_sound[1], 0, false);
			//START OPTION消去
			for (int i = 0; i < 2; i++) { g_Opbutton[i].ch.flag = false; }
			//カーソルを上の項目へ
			g_cursor1.y = g_Opbutton[2].ch.y;
			g_cursor1.x = g_Opbutton[2].ch.x;
			//遷移フラグ
			g_OpSceneFlagNext = OPENING_SCENE_SELECT_LEVEL_FUNCTION;
			return;
		}
	}
	//オプション画面へ遷移
	if (g_cursor1.y == g_Opbutton[1].ch.y) {
		//カーソルの幅を項目の幅に合わせる
		g_cursor1.w = g_Opbutton[1].ch.w;
		//決定
		if (JustKey('Z') || JustButton(BUTTON_1)) {
			//決定音
			PlaySound(g_op_sound[1], 0, false);
			//オプション画面へ
			CreateWipe(SCENE_OPTION, CHANGE_SCENE, WIPE_CONTRACTION);
			return;
		}
	}

}

/*
**************************************************************************************************
ボタン設定2レベル設定
**************************************************************************************************
*/
void	SetSecondButton() {

	//難易度データ
	tagGAMELEVEL	gameLevelTbl[LEVEL_MAX] = {
		EASY_MODE		,
		NORMAL_MODE		,
		HARD_MODE		,
	};

	//各種難易度表示
	for (int i = 2; i < OPENING_BUTTON_KIND_OF_IMAGE; i++) { g_Opbutton[i].ch.flag = true; }

	//背景のサイズを合わせる
	g_Opbutton[5].ch.h = (int)(((g_Opbutton[4].ch.y + g_Opbutton[4].ch.h + OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN)
		- (g_Opbutton[2].ch.y - OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN)));
	//座標
	g_Opbutton[5].ch.y = g_Opbutton[2].ch.y - OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN;
	g_Opbutton[5].ch.x = g_Opbutton[2].ch.x - OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN;

	//Level選択時の処理
	//下入力
	if (JustKey(VK_DOWN) || JustLever(LEVER_DOWN)) {
		//カーソル音
		PlaySound(g_op_sound[0], 0, false);
		g_cursor1.y += g_cursor1.my;
		//下の項目の座標まで来たら上に戻す
		if (g_cursor1.y > g_Opbutton[4].ch.y) g_cursor1.y = g_Opbutton[2].ch.y;

	}
	//上入力
	if (JustKey(VK_UP) || JustLever(LEVER_UP)) {
		//カーソル音
		PlaySound(g_op_sound[0], 0, false);
		g_cursor1.y -= g_cursor1.my;
		//上の項目の座標まで来たら下に戻す
		if (g_cursor1.y < g_Opbutton[2].ch.y) g_cursor1.y = g_Opbutton[4].ch.y;

	}
	//▼ レベル設定
	for (int i = 2; i < LEVEL_MAX + 2; i++) {
		//カーソル移動
		if (g_cursor1.y == g_Opbutton[i].ch.y) {
			//幅を合わせる
			g_cursor1.w = g_Opbutton[i].ch.w;
			//決定キー時
			if (JustKey('Z') || JustButton(BUTTON_1)) {
				//決定音
				PlaySound(g_op_sound[1], 0, false);
				//難易度を代入
				SetGameLevel(gameLevelTbl[i - 2]);
				//ステージ選択画面へ遷移
				CreateWipe(SCENE_SELECTSTAGE, CHANGE_SCENE, WIPE_CONTRACTION);
				return;
			}
		}
	}
	//キャンセル時
	if (JustKey('X') || JustButton(BUTTON_2)) {
		//キャンセル音
		PlaySound(g_op_sound[2], 0, false);
		//難易度設定消去
		for (int i = 2; i < (OPENING_BUTTON_KIND_OF_IMAGE - 1); i++) { g_Opbutton[i].ch.flag = false; }
		//カーソルの座標をSTARTに合わせる
		g_cursor1.y = g_Opbutton[0].ch.y;
		g_cursor1.x = g_Opbutton[0].ch.x;
		//遷移フラグ
		g_OpSceneFlagNext = OPENING_SCENE_SELECT_START_FUNCTION;
		return;
	}

}
