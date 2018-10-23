#include "gadget.h"
#include "seastage.h"
#include "swimer.h"
#include "game.h"
#include "input.h"
#include "score.h"
#include "sound.h"
#include "SelectStage.h"
#include "play.h"

//▼グローバル変数
//画像を扱う
int g_gadgetImg[GADGET_IMAGE_MAX] = {0};
//設定用
GADGET g_gadget[GADGET_SET_MAX] = {0};
//値表示用
SCOREVALUE g_gadgetScore[GADGET_SCORE_MAX] = {0};


//************************************************************************************************
//初期化
//************************************************************************************************
void InitGadget(HWND hWnd){

	//現在のステージを取得
	tagSELECTSTAGE stageNum = GetStage();

	//▼画像の読み込み
	//加速倍数をゲージで表示
	g_gadgetImg[0] = CreateDDImage("Image\\charge.bmp" , hWnd);
	//速度メーター
	g_gadgetImg[1] = CreateDDImage("Image\\speedindicator.bmp" , hWnd);
	//針
	g_gadgetImg[2] = CreateDDImage("Image\\index.bmp" , hWnd);

	//ガジェットの初期化
	ZeroMemory(&g_gadget[0], sizeof(GADGET) * GADGET_SET_MAX);
	//スコアの初期化
	ZeroMemory(&g_gadgetScore[0], sizeof(SCOREVALUE) * GADGET_SCORE_MAX);

	//▼各種設定
	//速度チャージ （海ステージのみ）
	if (stageNum == STAGE_SEA) {
		g_gadget[0].ch.x = 20.0f;			//表示先X
		g_gadget[0].ch.y = 460.0f;			//表示先Y
		g_gadget[0].ch.h = 16;				//表示高さ
		g_gadget[0].ch.w = 96;				//表示幅
		g_gadget[0].ch.flag = true;			//表示フラグ
	}
	//ガジェット情報「速度メーター」
	g_gadget[1].ch.h = 96;
	g_gadget[1].ch.w = 96;
	g_gadget[1].ch.x = 20.0f;
	g_gadget[1].ch.y = 364.0f;
	//表示フラグ
	g_gadget[1].ch.flag = true;

	//示針
	g_gadget[2].ch.x = g_gadget[1].ch.x + 13.0f;
	g_gadget[2].ch.y = g_gadget[1].ch.y + 15.0f;
	g_gadget[2].ch.h = 68;
	g_gadget[2].ch.w = 68;
	g_gadget[2].ani.fram = 10;
	g_gadget[2].ani.max = 55;
	//アニメーション設定
	SetAnimation(&g_gadget[2].ch , &g_gadget[2].ani ,
			g_gadget[2].ani.sx , g_gadget[2].ani.sy ,
			g_gadget[2].ani.max , g_gadget[2].ani.fram , 0 , true);

	//速度値
	g_gadgetScore[0].score_scale = 0.25f;					//スコア拡縮率
	g_gadgetScore[0].score_x = 20.0f;						//スコア表示先X
	g_gadgetScore[0].score_y = 400.0f;						//スコア表示先Y
	g_gadgetScore[0].ch.flag = true;						//表示フラグ
	g_gadgetScore[0].dicimals_flag = true;					//小数表示フラグ
	g_gadgetScore[0].kiloFlag = true;						//kmの表示フラグ
	g_gadgetScore[0].ch.type = SCORE_NUMBER_TYPE_SPEED;	//スコア画像の種類
	g_gadgetScore[0].digit = 2;							//桁数

}


//************************************************************************************************
//更新
//************************************************************************************************
void UpdateGadget(){
	
	//海ステージの情報を取得
	STAGE* pst = GetSeaStage();
	//プレイヤーの情報を取得
	SWIMER* pch = GetSwimer();

	
	//表示する速度を更新
	g_gadgetScore[0].num = g_gadget[1].coordinate * 10;

	//海ステージで縦スクロールでの変更
	if(pst->scrolly){

		//速度チャージを非表示
		g_gadget[0].ch.flag = false;

		//速度チャージの位置を反対に
		g_gadget[0].ch.x = 640.0f - (float)(g_gadget[1].ch.w + 20.0f);

		//速度メーターを非表示
		g_gadget[1].ch.flag = false;

		//速度メーターの位置を反対に
		g_gadget[1].ch.x = 640.0f - (float)(g_gadget[1].ch.w + 20.0f);

		//速度メーターの針の位置を反対に
		g_gadget[2].ch.x = g_gadget[1].ch.x + 13.0f;
		g_gadget[2].ch.y = g_gadget[1].ch.y + 15.0f;

		//速度を非表示
		g_gadgetScore[0].ch.flag = false;

		//速度表示の位置を反対に
		g_gadgetScore[0].score_x = 640.0f - (float)(g_gadget[1].ch.w + 20.0f);

	}

	//右スクロールでの変更
	if(pst->scroll_r){

		//速度チャージを表示
		g_gadget[0].ch.flag = true;

		//メーターを表示
		g_gadget[1].ch.flag = true;

		//速度を表示
		g_gadgetScore[0].ch.flag = true;
	}

	//取得したキャラクターの移動量を反映した値をもとに
	//現在の速度を算出する
	g_gadget[1].indicator = g_gadget[1].coordinate * 10;


	//▼　速度表示
	//左方向（－X移動）なら正の値に修正する
	if(g_gadget[1].indicator < 0){
		g_gadget[1].indicator *= -1;
	}

	//プレイヤーの現在の速度に応じてスコアの斜体フラグを切り替える
	//プレイヤーの速度が100km以上になったら文字を斜体にする
	if( g_gadget[1].indicator >= GADGET_SPEED_SCORE_ITALIC ){

		//斜体を扱うマクロを代入
		g_gadgetScore[0].italic = SCORE_DESIGN_ITALIC;

	}

	//設定した数値以下になったら通常の文字体に戻す
	if( g_gadget[1].indicator <= GADGET_SPEED_SCORE_ITALIC ){
	
		//垂体を扱うマクロを代入
		g_gadgetScore[0].italic = SCORE_DESIGN_VIRTUALLY;

	}

	//コマの変化は約５°時計回りに動いていく
	//現在の速度　*　270°角の最高速120ｋｍ　/　メモリの角度270°＝　1°に対する速度の変化量
	//速度の変化量　*　１コマで変化する角度　+　誤差（微調整）
	if(g_gadget[1].indicator >= g_gadget[2].ani.cnt * 120 / 270 * 5 + 0.62){

		//次のコマへ
		g_gadget[2].ani.cnt++;

	}else{

		//現在の速度がコマ*アニメーション変化する速度量を下回ったら前のコマへ
		g_gadget[2].ani.cnt--;
	}

	//速度メーターの最高表示速度120kmを超えたら針を繰り返し回転させる
	if(g_gadget[1].indicator >= 120){

		//表示するコマ数を加算
		g_gadget[2].ani.cnt++;
		
		//設定した最大コマ数付近に達したら
		if(g_gadget[2].ani.cnt >= g_gadget[2].ani.max - 1){
			
			//コマのカウントをリセット
			g_gadget[2].ani.cnt = 0;
		}
	}

	//表示元座標を更新
	g_gadget[2].ch.picx = g_gadget[2].ani.sx + ( g_gadget[2].ani.cnt * g_gadget[2].ch.w );
	g_gadget[2].ch.picy = g_gadget[2].ani.sy;

}

//************************************************************************************************
//表示
//************************************************************************************************
void DrawGadget(){

	//プレイヤーの情報を取得
	SWIMER* pch = GetSwimer();
	
	//生存フラグがtrueなら表示
	if(g_gadget[0].ch.flag){
		//速度チャージ値
		DDBlt(
			(int)g_gadget[0].ch.x ,									//X座標
			(int)g_gadget[0].ch.y ,									//Y座標
			(int)(g_gadget[0].ch.w / pch->speedmax * pch->ch.speed) ,//幅
			g_gadget[0].ch.h ,										//高さ
			g_gadgetImg[0] ,										//対応した画像番号
			g_gadget[0].ch.picx ,									//表示元Ｘ座標
			g_gadget[0].ch.picy);									//表示元Ｙ座標
	}
	
	//生存フラグがtrueなら表示
	if(g_gadget[1].ch.flag){
		//ガジェット画像「速度メーター」
		DDBlt(
			(int)g_gadget[1].ch.x ,			//X座標
			(int)g_gadget[1].ch.y ,			//Y座標
			g_gadget[1].ch.w ,				//幅
			g_gadget[1].ch.h ,				//高さ
			g_gadgetImg[1] ,				//対応した画像番号
			g_gadget[1].ch.picx ,			//表示元Ｘ座標
			g_gadget[1].ch.picy);			//表示元Ｙ座標

		//速度メーターの針
		DDBlt(
			(int)g_gadget[2].ch.x ,			//X座標
			(int)g_gadget[2].ch.y ,			//Y座標
			g_gadget[2].ch.w ,				//幅
			g_gadget[2].ch.h ,				//高さ
			g_gadgetImg[2] ,				//対応した画像番号
			g_gadget[2].ch.picx ,			//表示元Ｘ座標
			g_gadget[2].ch.picy);			//表示元Ｙ座標
	}

	//速度の表示
	if(g_gadgetScore[0].ch.flag){

		DrawScore( &g_gadgetScore[0] );
	}
}

//************************************************************************************************
//解放
//************************************************************************************************
void ReleaseGadget(){
	//画像の解放
	for(int i = 0;i < GADGET_IMAGE_MAX;i++){
		ReleaseDDImage(g_gadgetImg[i]);
	}
}

//************************************************************************************************
//ガジェットのポインタを返す関数
//************************************************************************************************
GADGET* GetGadget(){ return &g_gadget[1]; }

//************************************************************************************************
//プレイヤーのステータスをガジェットに反映させる
//************************************************************************************************
void SetStatus(float status){ g_gadget[1].coordinate = status; }
