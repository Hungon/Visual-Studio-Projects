#include    "map.h"
#include    "play.h"
#include    "sound.h"
#include    "menu.h"
#include	"SelectStage.h"
#include	<string.h>
#include	<math.h>
#include	<stdio.h>


//▼グローバル変数

//▼画像の要素
int g_mapImg[MAP_IMAGE_MAX] = {0};

//▼効果音
int g_map_sound[MAP_SOUND_EFFECT_MAX] = {0};

//ステージセレクト表示を扱う
CHARACTER g_select = {0};

//カメラ設定
float g_cameraX = 0;				//カメラＸ座標
float g_cameraY = 0;				//カメラＹ座標

//サイン表示を扱う
STAGE g_sign[MAP_SIGN_MAX] = {0};

//競技スタート中か扱う
bool g_start = false;

//時間表示
SCOREVALUE g_timeScore = {0};

//カメラの範囲を扱う
float g_stageW = 0;				//幅
float g_stageH = 0;				//高さ
float g_stageTop = 0;			//ステージの上辺
float g_stageBottom = 480.0f;	//ステージの下辺


//************************************************************************************************
//▼初期化
//************************************************************************************************
void InitMap(HWND hWnd){

	//現在のステージ番号を取得する
	tagSELECTSTAGE stageNum = GetStage();

	//画像データ
	char	*lpImageTbl[ MAP_IMAGE_MAX ] = {

		{"Image\\readygo.bmp"}		,
		{"Image\\countdown.bmp"}	,
		{"Image\\SelectStage.bmp"}	,

	};

	//各ステージの範囲を扱う
	g_stageW = 0.0f;			//幅
	g_stageH = 0.0f;			//高さ
	g_stageTop = 0.0f;			//ステージの上辺
	g_stageBottom = 480.0f;		//ステージの下辺
	//時間をリセット
	ZeroMemory(&g_timeScore, sizeof(SCOREVALUE));
	//ステージセレクト
	ZeroMemory(&g_select, sizeof(CHARACTER));
	//カメラの座標をリセット
	g_cameraX = 0;				//カメラＸ座標
	g_cameraY = 0;				//カメラＹ座標
	//サインの初期化
	ZeroMemory(g_sign, sizeof(STAGE) * MAP_SIGN_MAX);

	//▼画像の読み込み
	for( int i = 0 ; i < MAP_IMAGE_MAX ; i++ ){
		g_mapImg[i] = CreateDDImage( lpImageTbl[i] , hWnd );
	}

	//▼音の読み込み
	//効果音１
	g_map_sound[0] = CreateSound("SE\\countdown.wav");
	//効果音２
	g_map_sound[1] = CreateSound("SE\\startsign.wav");

	//▼各ステージの音楽ファイル
	char* backMusic[STAGE_MAX] = {
		"BGM\\sea.wav" ,
		"BGM\\road.wav" ,
		"BGM\\OffRoad.wav" ,
	};

	//読み込み
	CreateStreaming( backMusic[ stageNum ] );

	//▼マップの初期設定

	//競技スタート中をfalse
	g_start = false;

	//セレクト画像
	g_select.x = 200.0f;
	g_select.y = 100.0f;
	g_select.w = SELECTSTAGE_STAGE_TITLE_IMAGE_WIDTH;
	g_select.h = SELECTSTAGE_STAGE_TITLE_IMAGE_HEIGHT;
	g_select.picy = SELECTSTAGE_STAGE_TITLE_IMAGE_WIDTH * stageNum;
	g_select.flag = true;

	//スタート合図画像「readygo!!」
	g_sign[1].ch.x = 80;		//X座標
	g_sign[1].ch.y = 150;	//Y座標
	g_sign[1].ch.h = 176;	//高さ
	g_sign[1].ch.w = 496;	//幅
	g_sign[1].ch.flag = false;

	//「３，２，１」の画像
	g_sign[0].sign = false;
	g_sign[0].ch.x = 250;	//X座標
	g_sign[0].ch.y = 150;	//Y座標
	g_sign[0].ch.h = 144;	//高さ
	g_sign[0].ch.w = 144;	//幅
	g_sign[0].ch.flag = false;

	//「３，２，１」のアニメーション設定
	g_sign[0].ani.fram = 40;		//フレーム数
	g_sign[0].ani.max = 3;		//最大表示数
	
	SetAnimation(&g_sign[0].ch,&g_sign[0].ani ,
				g_sign[0].ani.sx,g_sign[0].ani.sy ,
				g_sign[0].ani.max ,
				g_sign[0].ani.fram ,
				0 , true);	

	//▼時間スコアの設定
	g_timeScore.digit = 2;								//2桁表示
	g_timeScore.score_x = 100.0f;						//表示X座標
	g_timeScore.score_y = 50.0f;						//表示Y座標
	g_timeScore.spaceX = 10.0f;							//表示間隔
	g_timeScore.ti.hour = 0;							//時
	g_timeScore.ti.minutes = 0;							//分
	g_timeScore.ti.second = 0;							//秒
	g_timeScore.ti.millisecond = 0;						//ミリ秒
	g_timeScore.score_scale = 0.25f;					//拡縮率
	g_timeScore.ch.type = SCORE_NUMBER_TYPE_NORMAL;		//スコア画像の種類

}

//************************************************************************************************
//▼更新
//************************************************************************************************
void UpdateMap(){

	int i = 0;

	//メニューフラグを受け取る
	bool menu = GetMenu();

	//プレイ開始未開始の処理「g_select == true」でステージ名を表示
	//メニューを開いていたら処理を更新しない
	if(g_start == false || menu == false){
			//時間経過
			g_select.time++;
		
		//フレーム経過でゲーム開始
		if(g_select.time > 100 && g_sign[0].sign == false && g_start == false){
			g_select.time = 0;
			g_select.flag = false;
			g_sign[0].sign = true;
		}

		//スタートサインを出す関数
		g_sign[0].sign = StartSign(g_sign[0].sign);
	}

	//メニューを開いていたらカウントを停止する
	if(menu){

		//カウント停止
		g_sign[0].sign = false;
		//時間停止
		g_timeScore.ti.cntFlag = false;
	}

	//メニューを開いていなければ
	if( menu == false && g_start == true ){

		//時間経過
		g_timeScore.ti.cntFlag = g_start;

		//レース開始
		Playing(g_start);
	}

	//曲を再生
	PlayStreaming(g_start);

}

//************************************************************************************************
//▼表示
//************************************************************************************************
void DrawMap(){

	//生存フラグがtrueなら表示
	if(g_sign[1].ch.flag == true){
		//スタートの合図「readygo!!」
		DDBlt(
			(int)g_sign[1].ch.x ,
			(int)g_sign[1].ch.y ,
			g_sign[1].ch.w ,
			g_sign[1].ch.h ,
			g_mapImg[0] ,
			g_sign[1].ch.picx ,
			g_sign[1].ch.picy);
	}
	//生存フラグがtrueなら表示
	if(g_sign[0].ch.flag == true){
		//スタートの合図「３，２，１」
		DDBlt(
			(int)g_sign[0].ch.x ,
			(int)g_sign[0].ch.y ,
			g_sign[0].ch.w ,
			g_sign[0].ch.h ,
			g_mapImg[1] ,
			g_sign[0].ch.picx ,
			g_sign[0].ch.picy);
	}

	//ステージセレクト画像
	if(g_select.flag){
		DDBlt(
			(int)g_select.x ,
			(int)g_select.y ,
			g_select.w ,
			g_select.h ,
			g_mapImg[2] ,
			g_select.picx ,
			g_select.picy);
	}

	//時間スコアの表示
	DrawTime( &g_timeScore );

}

//************************************************************************************************
//▼解放
//************************************************************************************************
void ReleaseMap(){

	int i = 0;
	//使用した枚数だけ解放
	for(i = 0;i < MAP_IMAGE_MAX;i++){
		ReleaseDDImage(g_mapImg[i]);
	}

	//効果音の解放
	for(i = 0;i < MAP_SOUND_EFFECT_MAX;i++){
		ReleaseSound(g_map_sound[i]);
	}
	//曲の解放
	ReleaseStreaming();
}

//************************************************************************************************
//スタート時のサインをflagを渡して表示する
//************************************************************************************************
bool StartSign(bool flag){

	bool ret = flag;

	//渡されたフラグがtrue、かつ、「ReadyGo!!」の表示カウントが未経過なら「3，2，1」を表示
	if(flag == true){
		if(g_sign[1].ch.time == 0){
			//カウントダウンの数字が繰り上がるたびに効果音を鳴らす
			if(g_sign[0].ani.time == 0){
				//効果音
				PlaySound(g_map_sound[0],0,false);
			}
			//「3,2,1」画像をアニメーション表示
			g_sign[0].ch.flag = true;
			//「3,2,1」表示後に消して、「ReadyGo!!」を表示
			if( g_sign[0].ch.flag == true && UpdateAnimation( &g_sign[0].ch,&g_sign[0].ani ) == false ){
				g_sign[0].ch.flag = false;
				//効果音
				PlaySound(g_map_sound[1],0,false);
				//「ReadyGo！！」表示
				g_sign[1].ch.flag = true;
			}
		}
		//カウントアップ
		if(g_sign[1].ch.flag == true){
			g_sign[1].ch.time++;		//経過時間
		}
		//フレーム数が１００を超えたら
		if(g_sign[1].ch.time > 100){
			//時間をリセット
			g_sign[1].ch.time = 0;
			//「ReadyGo!!」を消す
			g_sign[1].ch.flag = false;
			//戻り値
			ret = false;
			//プレイ開始
			g_start = true;
		}
	}
	return ret;
}

//***************************************************************************************************
//現在のステージの幅を渡す
//*************************************************************************************************
float GetStageWidth(){
	return g_stageW;
}

//***************************************************************************************************
//現在のステージの高さを渡す
//*************************************************************************************************
float GetStageHeight(){
	return g_stageH;
}


//************************************************************************************************
//競技中を取得
//************************************************************************************************
bool GetRacing(){
	return g_start;
}

//************************************************************************************************
//▼カメラの座標を指定の座標に変更する関数
//************************************************************************************************
void SetCamera( float cameraX , float cameraY ){
	

	//受け取った座標を基にカメラの座標の基点を画面の中心に変更する
	g_cameraX = cameraX;				//X座標
	g_cameraY = cameraY;				//Y座標

	//画面外（画面の左）を表示しないようにカメラの基点座標を修正
	if(g_cameraX <= 0){
		g_cameraX = 0;
	}
	//画面の右
	if( g_cameraX >= g_stageW - STAGE_WIDTH ){
		g_cameraX = (float)( g_stageW - STAGE_WIDTH );
	}
	//画面の上
	if( g_cameraY <= g_stageTop ){		
		g_cameraY = g_stageTop;
	}
	//画面の下
	if( g_cameraY >= g_stageBottom - STAGE_HEIGHT ){
		g_cameraY = g_stageBottom - STAGE_HEIGHT;
	}
}

//************************************************************************************************
//カメラの範囲を変更する
//************************************************************************************************
void ChangeCamera(float top , float bottom , float width , float height){
	
	//渡された値を代入
	g_stageTop = top;		//上辺
	g_stageBottom = bottom;	//下辺
	g_stageW = width;		//幅
	g_stageH = height;		//高さ

}

//************************************************************************************************
//▼カメラ範囲内矩形との当たり判定を取る関数
//************************************************************************************************
bool CollisionCamera(CHARACTER* ch){

	//カメラの判定を扱う変数
	CHARACTER rect = {0};
	
	//rectに画面サイズより大きい矩形を設定
	rect.x = g_cameraX - 320.0f;	//画面座標に合わせる
	rect.y = g_cameraY - 240.0f;	//画面座標に合わせる
	rect.w = 1280;					//幅
	rect.h = 960;					//高さ
	rect.scale = 1.0f;				//拡縮倍率

	//rectと引数で渡されたキャラクターで判定
	return CollisionCharacter( &rect , ch );
}

//************************************************************************************************
//▼カメラのＸ座標を返す関数
//************************************************************************************************
float GetCameraX(){	return g_cameraX; }

//************************************************************************************************
//▼カメラのＹ座標を返す関数
//************************************************************************************************
float GetCameraY(){	return g_cameraY; }

//************************************************************************************************
//ステージのクリア時間を返す
//************************************************************************************************
TIME* GetStageCompleteTime(){
	
	//ミリ秒を記録用に適した値に調整
	g_timeScore.ti.millisecond *= 1.67;
	return &g_timeScore.ti; 
}


//************************************************************************************************
//レースタイマーのON、OFFを扱う
//************************************************************************************************
void ControlStageTime(bool control){ g_timeScore.ti.cntFlag = control; }

