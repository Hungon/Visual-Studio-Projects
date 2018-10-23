#include "config.h"
#include "game.h"
#include "input.h"
#include "wipe.h"
#include "sound.h"

//画像読み込み用
int g_configImg[CONFIG_IMG_MAX] = {0};
//効果音の読み込み
int g_co_sound[CONFIG_SOUND_EFFECT_MAX] = {0};

//矢印の基本情報
CHARACTER g_cursor = {0};

int g_config[ACTION_MAX] = {
	BUTTON_4 ,		//初期設定のボタン
	BUTTON_3 ,		
	BUTTON_6 ,
};


//*************************************************************************************************
//初期化　InitConfig
//*************************************************************************************************
void InitConfig( HWND hWnd ){

	//▼画像の読み込み
	g_configImg[0] = CreateDDImage("Image\\cursor.bmp" , hWnd );	//絵


	//▼効果音の読み込み
	//カーソル音
	g_co_sound[0] = CreateSound("SE\\cursor.wav");
	//決定音
	g_co_sound[1] = CreateSound("SE\\enter.wav");

	//▼曲の読み込み
	CreateStreaming("BGM\\option.wav");

	//カーソルの初期化
	g_cursor.x = 50.0f;	//x座標	
	g_cursor.y = 100.0f;	//y座標
	g_cursor.w = 32;	//幅
	g_cursor.h = 32;	//高さ
	g_cursor.scale = 1.0f;	//拡縮倍率

}

//*************************************************************************************************
//更新　UpdateConfig
//*************************************************************************************************
void UpdateConfig(){

	int i = 0;

	//曲の再生
	PlayStreaming(true);
	
	if((int)g_cursor.y == 400 && 
	  JustKey('Z') || JustButton(BUTTON_1)){
		//決定音
		PlaySound(g_co_sound[1],0,false);
		//シーンオープニングへ
		CreateWipe(SCENE_OPENING , CHANGE_SCENE , WIPE_FEEDOUT);
	}

	//オープニングへシーン変更
	//全てのボタンの押した瞬間を調べる
	for(i = 0;i < BUTTON_MAX;i++){
		if(JustButton(0x01<<i)){
			//決定音
			PlaySound(g_co_sound[1],0,false);
			//各ボタン設定の座標
			if((int)g_cursor.y == 100){
				g_config[ATTACK] = 0x01<<i;
			}
			if((int)g_cursor.y == 200){
				g_config[JUMP] = 0x01<<i;
			}
			if((int)g_cursor.y == 300){
				g_config[DUSH] = 0x01<<i;
			}

		}	//カーソルの値が２００ならジャンプ、３００ならダッシュ
	}


	//-------------------------------------------------------------
	//矢印の移動(X座標が100、Y座標は100～400の間で100ずつ変化する)
	//-------------------------------------------------------------

	//キー「↑」か、十字キー「↑」で矢印移動
	if( JustKey( VK_UP ) || JustLever( LEVER_UP ) ){
		g_cursor.y -= 100;
		//カーソル音
		PlaySound(g_co_sound[0],0,false);
		//下限を下回ったら、一番下の項目に合わせる
		if( g_cursor.y < 100 ){
			g_cursor.y = 400.0f;
		}
	}

	//キー「↓」か、十字キー「↓」で矢印移動
	if( JustKey( VK_DOWN ) || JustLever( LEVER_DOWN ) ){
		g_cursor.y += 100;
		//カーソル音
		PlaySound(g_co_sound[0],0,false);
		//上限を上回ったら、一番上の項目に合わせる
		if( g_cursor.y > 400 ){
			g_cursor.y = 100.0f;
		}
	}
	
}

//*************************************************************************************************
//表示　DrawConfig
//*************************************************************************************************
void DrawConfig(){

	char mes[32] = {0};

	//背景を白で塗りつぶし
	DDBlt(0 , 0 , 640 , 480 , NULL , 0 , 0 );
	
	//X100 Y100　の座標に”攻撃ボタン / 決定ボタン”と表示
	DDTextOut("攻撃ボタン / 決定ボタン",100 , 100 );
	
	//X100 Y200　の座標に”ジャンプボタン”と表示
	DDTextOut("ジャンプボタン",100 , 200 );

	//X100 Y300　の座標に”ジャンプボタン”と表示
	DDTextOut("ダッシュボタン" ,100 , 300 );

	//X100 Y400　の座標に”ジャンプボタン”と表示
	DDTextOut("タイトルに戻る",100 , 400 );

	//矢印の表示
	DDBlt((int)g_cursor.x , (int)g_cursor.y , g_cursor.w , g_cursor.h , g_configImg[0] , 0 , 0);

	//その他の文字カーソル表示
	//攻撃の使用キーを１６進数で表示
	wsprintf(&mes[0] , "0x%d" , g_config[ATTACK]);
	DDTextOut(&mes[0] , 300 , 100);
	wsprintf(&mes[0] , "0x%d" , g_config[JUMP]);
	DDTextOut(&mes[0] , 300 , 200);
	wsprintf(&mes[0] , "0x%d" , g_config[DUSH]);
	DDTextOut(&mes[0] , 300 , 300);
}



//*************************************************************************************************
//解放　ReleaseConfig
//*************************************************************************************************
void ReleaseConfig(){
	
	int i = 0;
	
	//画像
	for(i = 0;i < CONFIG_IMG_MAX;i++){
		//矢印の絵を解放
		ReleaseDDImage(g_configImg[i]);
	}
	//音
	for(i = 0;i < CONFIG_SOUND_EFFECT_MAX;i++){
		ReleaseSound(g_co_sound[i]);
	}
	//曲の解放
	ReleaseStreaming();
}

//************************************************************************************************
//使用キーの取得
//************************************************************************************************
int GetConfig(int action){
	return g_config[action];		//引数の行動で使うボタンを設定
}
