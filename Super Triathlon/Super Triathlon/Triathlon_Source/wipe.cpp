#include	"wipe.h"
#include	"play.h"
#include	"map.h"
#include	"opening.h"
#include	"menu.h"
#include	"SelectStage.h"
#include	<stdio.h>
#include	<math.h>

//▼グローバル変数宣言
//ワイプ画像を扱う
int g_wipeImg[WIPE_IMAGE_MAX] = {0};

//ワイプ画像設定用
WIPE g_wipe[WIPE_MAX] = {0};

//ロード中イメージ設定用
int g_loadImg[LOAD_IMAGE_MAX] = {0};

//ロード中イメージ設定用
WIPE g_load[LOAD_IMAGE_MAX] = {0};

//ワイプ演出を扱う
tagWIPE_DIRECT g_directwipe = WIPE_FEEDOUT;

//シーン切り替えフラグ
bool g_wipescene = false;

//************************************************************************************************
//初期化
//************************************************************************************************
void InitWipe(HWND hWnd){
	
	//▼画像の読み込み
	//ワイプ画像（背景黒）透過演出用
	g_wipeImg[0] = CreateDDImage("Image\\wipe.bmp" , hWnd);

	//ワイプ画像（背景黒）真ん中に収縮する演出用
	g_wipeImg[1] = CreateDDImage("Image\\wipe_circle.bmp" , hWnd);
	g_wipeImg[2] = CreateDDImage("Image\\wipe_half1.bmp" , hWnd);
	g_wipeImg[3] = CreateDDImage("Image\\wipe_half2.bmp" , hWnd);

	//読み込みイメージ画像「NowLoading」
	g_loadImg[0] = CreateDDImage("Image\\loading.bmp" , hWnd);
	
	//画面を覆った状態の滞在時間
	g_wipe[0].stay = WIPE_DEFAULT_STAY_TIME;

	//▼ロードイメージの初期化
	g_load[0].ch.h = 24;			//高さ
	g_load[0].ch.w = 150;			//幅
	g_load[0].ch.x = 10.0f;			//X座標
	g_load[0].ch.y = 440.0f;		//Y座標
	g_load[0].ch.picx = 0;			//表示元X
	g_load[0].ch.picy = 0;			//表示元Y

	//ロードイメージのアニメーション設定の初期化
	g_load[0].ani.cnt = 0;		//現在のコマ数
	g_load[0].ani.fram = 10;	//フレーム数
	g_load[0].ani.max = 5;		//最大コマ数
	g_load[0].ani.time = 0;		//経過時間
	g_load[0].ani.type = 0;		//種類番号

	//アニメーションを設定
	SetAnimation(&g_load[0].ch , &g_load[0].ani ,
				g_load[0].ani.sx , g_load[0].ani.sy ,
				g_load[0].ani.max , g_load[0].ani.fram ,
				g_load[0].ani.type , true);

}

//************************************************************************************************
//更新
//************************************************************************************************
bool UpdateWipe(){

	//ワイプ処理の分岐
	void (*pdirectwipe[WIPE_DIRECT_MAX])() = {
		
		DirectWipe0 ,		//透過演出
		DirectWipe1 ,		//収縮演出
	};

	//ステージクリアを調べる用
	int complete = 0;

	//ワイプの処理
	if(g_wipe[0].ch.flag){
		
		//ワイプ演出の分岐
		pdirectwipe[g_directwipe]();
		
		//シーン切り替えフラグが立ったら画面遷移
		if( g_wipescene ){

			//ステージクリアを調べる
			for( int i = 0 ; i < STAGE_MAX ; i++ ){
				if( *( GetCompleteStage() + i ) == SELECTSTAGE_STAGE_COMPLETE ) complete++;
			}

			//▼ 次のシーンがステージ選択シーンで、全ステージをクリア済なら次のシーンを
			//エンディングへ変更する
			if( g_wipe[0].next == SCENE_SELECTSTAGE &&  complete == 3 ) g_wipe[0].next = SCENE_ENDING;

			//ワイプの種類をもとに場面変更
			switch( g_wipe[0].type ){
			
				//シーン切り替え
				case CHANGE_SCENE:
					ChangeScene( g_wipe[0].next );
					break;
				
				//ステージ切り替え
				case CHANGE_STAGE:
					break;
				
				//レースを始めからスタート
				case RESTART_RACE:
					RestartRace();
					break;

				//レースを抜けてタイトルへ
				case TO_TITLE:
					ToTitle();
					break;
				//ステージセレクトへ
				case TO_SELECT:
					ToSelectStage();
					break;
			}
			//変更後、種類を-1に
			g_wipe[0].type = NOT_TRANSITION;
		}
		//ロードイメージ画像のアニメーション
		UpdateAnimation( &g_load[0].ch , &g_load[0].ani );

	}
	//最後にワイプの生存フラグを返す
	return g_wipe[0].ch.flag;

}


//************************************************************************************************
//ワイプ処理（透過演出）
//************************************************************************************************
void DirectWipe0(){
	
	//ロードイメージ未表示で、ワイプのアルファ値が1より大きいなら暗転処理
	if( g_load[0].ch.flag == false && 
		g_wipe[0].alpha > 1.0f && g_wipescene == false ) g_wipe[0].alpha += g_wipe[0].sub_alpha;

	//ワイプで完全に暗転したら、そのままワイプをとどめて、画面切り替え
	if( g_wipe[0].alpha <= 1.0f && g_wipe[0].ch.time < g_wipe[0].stay ){
	
		//ロードイメージ画像を表示
		g_load[0].ch.flag = true;
		//時間を計測
		g_wipe[0].ch.time++;
		//アルファ値は「1」
		g_wipe[0].alpha= 1.0f;
		//画面切り替えフラグをtrueに
		g_wipescene = true;
		return;
	}

	//画面遷移したら
	if( g_wipescene ){
		//指定した滞在フレーム数を超えたら、画面切り替え、暗転から少しずつ透過でゲーム画面を表示
		if( g_wipe[0].ch.time >= g_wipe[0].stay && g_wipe[0].alpha < 255.0f ){
			//徐々に透過していく
			g_wipe[0].alpha += g_wipe[0].add_alpha;
			//ロードイメージ画像を消去
			g_load[0].ch.flag = false;
		}

		//透過値が「255以上」かつ、ロードイメージ画像が消えたらワイプ消去
		if( g_wipe[0].alpha >= 255.0f && g_load[0].ch.flag == false ){

			//経過時間リセット
			g_wipe[0].ch.time = 0;

			//ワイプ発生フラグをfalseに
			g_wipe[0].ch.flag = false;
			
		}
	}

}


//************************************************************************************************
//ワイプ処理（拡縮演出）
//************************************************************************************************
void DirectWipe1(){


	//画面の対角線を求める
	float line_d = sqrtf( (STAGE_WIDTH * STAGE_WIDTH) + (STAGE_HEIGHT * STAGE_HEIGHT) );

	//○と余白の差を求める
	float margin = (float)g_wipe[1].ch.w - 700.0f;
	
	//○の拡縮を反映した直径を求める
	float circle_d = (float)(g_wipe[1].ch.w * g_wipe[1].ch.scale) - (margin * g_wipe[1].ch.scale);

	//拡大・縮小後の幅・高さを求める
	//○
	float scale_cw = (float)(g_wipe[1].ch.w * g_wipe[1].ch.scale);
	float scale_ch = (float)(g_wipe[1].ch.h * g_wipe[1].ch.scale);
	//上
	float scale_th = (float)(g_wipe[2].ch.h * g_wipe[2].ch.scale);	//高さ
	//左
	float scale_lw = (float)(g_wipe[3].ch.w * g_wipe[3].ch.scale);	//幅
	//下
	float scale_bh = (float)(g_wipe[4].ch.h * g_wipe[4].ch.scale);	//高さ
	//右
	float scale_rw = (float)(g_wipe[5].ch.w * g_wipe[5].ch.scale);	//幅

	//中心を求める
	//○
	float center_cx = (float)(g_wipe[1].ch.w / 2);
	float center_cy = (float)(g_wipe[1].ch.h / 2);
	//上
	float center_ty = (float)(g_wipe[2].ch.h / 2);
	//左
	float center_lx = (float)(g_wipe[3].ch.w / 2);
	//下
	float center_by = (float)(g_wipe[4].ch.h / 2);
	//右
	float center_rx = (float)(g_wipe[5].ch.w / 2);

	//各ワイプの座標を算出する
	//○の座標
	float wipe_cx = g_wipe[1].ch.x + center_cx - (scale_cw / 2);	//X
	float wipe_cy = g_wipe[1].ch.y + center_cy - (scale_ch / 2);	//Y
	//上の座標
	float wipe_ty = g_wipe[2].ch.y + center_ty - (float)(scale_th / 2);	//Y
	//左の座標
	float wipe_lx = g_wipe[3].ch.x + center_lx - (scale_lw / 2);	//X
	//下の座標
	float wipe_by = g_wipe[4].ch.y + center_by - (scale_bh / 2);	//Y
	//右の座標
	float wipe_rx = g_wipe[5].ch.x + center_rx - (scale_rw / 2);	//X

	//求まった幅高さを基に上下左右のワイプの座標を修正する
	g_wipe[2].ch.y = (wipe_cy - scale_th) + ((scale_ch - circle_d) / 2);		//上
	g_wipe[3].ch.x = (wipe_cx - scale_lw) + ((scale_cw - circle_d) / 2);		//左
	g_wipe[4].ch.y = (wipe_cy + scale_ch) - ((scale_ch - circle_d) / 2);		//下
	g_wipe[5].ch.x = (wipe_cx + scale_cw) - ((scale_cw - circle_d) / 2);		//右

	//中心を丸くくりぬいた画像が見えなくなるまで処理
	if(g_wipe[1].ch.scale > 0 && g_wipescene == false){
		g_wipe[1].ch.scale += g_wipe[1].contraction_speed;
	}
	//○が無くなったら暗転の状態を維持してシーン切り替え
	if(g_wipe[1].ch.scale <= 0){
		//○画像の拡縮倍率を0に
		g_wipe[1].ch.scale = 0.0f;
		//シーン切り替えフラグをtrueに
		g_wipescene = true;
		//ロード中イメージの表示
		g_load[0].ch.flag = true;
		//時間経過
		g_wipe[0].ch.time++;
	}
	//指定した滞在フレームを超えたらシーン表示
	if(g_wipe[0].ch.time > g_wipe[0].stay){
		//ロード中イメージを非表示
		g_load[0].ch.flag = false;
		//拡大
		g_wipe[1].ch.scale += g_wipe[1].scale_speed;
	}
	//○が画面を外まで広がりきったら表示フラグを時間をリセット
	if(circle_d > line_d){
		g_wipe[0].ch.flag = false;	//表示フラグ
		g_wipe[0].ch.time = 0;		//時間をリセット
		
	}

}

//************************************************************************************************
//表示
//************************************************************************************************
void DrawWipe(){

	//拡大処理用画像要素
	int imageTbl[5] = {
		g_wipeImg[1],
		g_wipeImg[2],
		g_wipeImg[3],
		g_wipeImg[2],
		g_wipeImg[3]
	};

	//▼　透過処理
	//ワイプ発生中フラグがtrueなら表示
	if( g_wipe[0].ch.flag && g_directwipe == WIPE_FEEDOUT ){

		//ワイプ画像の表示
		DDAlphaBlt(						
			(int)g_wipe[0].ch.x ,
			(int)g_wipe[0].ch.y ,						
			g_wipe[0].ch.w ,
			g_wipe[0].ch.h ,				
			g_wipeImg[0] ,		
			g_wipe[0].ch.picx ,
			g_wipe[0].ch.picy ,
			g_wipe[0].alpha);		
	}

	//▼　拡縮処理
	//ワイプ発生中フラグがtrueなら表示
	if(g_wipe[0].ch.flag == true && g_directwipe == WIPE_CONTRACTION){
		for (int i = 1; i < WIPE_MAX; i++) {
			DDScaleBltSelectIndicate(
				(float)g_wipe[i].ch.x,
				(float)g_wipe[i].ch.y,
				g_wipe[i].ch.w,
				g_wipe[i].ch.h,
				imageTbl[i - 1],
				g_wipe[i].ch.picx,
				g_wipe[i].ch.picy,
				g_wipe[i].ch.scale,
				1);
		}
	}
	
	//▼ロード中イメージ画像「NowLoading」
	//ロードイメージ画像の生存フラグがtrueなら表示
	if(g_load[0].ch.flag == true){

		//ロードイメージ画像の表示
		DDBlt(						
			(int)g_load[0].ch.x,
			(int)g_load[0].ch.y,						
			g_load[0].ch.w,
			g_load[0].ch.h,				
			g_loadImg[0],		
			g_load[0].ch.picx,
			g_load[0].ch.picy);		
	}
}

//************************************************************************************************
//解放
//************************************************************************************************
void ReleaseWipe(){
	
	int i = 0;

	//ワイプ画像の解放
	for(i = 0;i < WIPE_IMAGE_MAX;i++){
		ReleaseDDImage(g_wipeImg[i]);
	}
	//ロード中イメージ画像の解放
	for(i = 0;i < LOAD_IMAGE_MAX;i++){
		ReleaseDDImage(g_loadImg[i]);
	}
}

//************************************************************************************************
//生成
//************************************************************************************************
void CreateWipe( tagGAMETRASITION next , tagWIPE_TRANSITION type , tagWIPE_DIRECT direct ){


	//シーン切り替えフラグをfalse
	g_wipescene = false;
	//ワイプ演出を取得
	g_directwipe = direct;
	//時間をリセット
	g_wipe[0].ch.time = 0;
	//次のシーンを取得
	g_wipe[0].next = next;

	//▼　透過演出
	//未発生ならワイプ生成
	if(g_wipe[0].ch.flag == false && direct == WIPE_FEEDOUT){
		
		//ワイプ1（黒背景「透過処理用」）の設定
		g_wipe[0].ch.x = 0.0f;		//X座標
		g_wipe[0].ch.y = 0.0f;		//Y座標
		g_wipe[0].ch.w = 640;		//幅
		g_wipe[0].ch.h = 480;		//高さ
		g_wipe[0].ch.picx = 0;		//表示元X
		g_wipe[0].ch.picy = 0;		//表示元Y
		g_wipe[0].ch.flag = true;	//ワイプの生存フラグ
		g_wipe[0].type = type;		//種類
		g_wipe[0].alpha = 255.0f;	//アルファ値
		g_wipe[0].add_alpha = WIPE_DEFAULT_ALPHA_ADD;//アルファ加算値
		g_wipe[0].sub_alpha = WIPE_DEFAULT_ALPHA_SUB;//アルファ減算値
		
	}

	//▼　収縮演出
	//未発生ならワイプ発生
	if(g_wipe[0].ch.flag == false && direct == WIPE_CONTRACTION){

		//ワイプ2（黒背景「画面中心へ収縮する演出用」）

		////画面中央を丸くくりぬいた画像
		g_wipe[1].ch.w = 1500;
		g_wipe[1].ch.h = 1500;
		g_wipe[1].ch.x = (float)((STAGE_WIDTH - g_wipe[1].ch.w) / 2);
		g_wipe[1].ch.y = (float)((STAGE_HEIGHT - g_wipe[1].ch.h) / 2);
		g_wipe[1].ch.picx = 0;
		g_wipe[1].ch.picy = 0;
		//ワイプの生存フラグをtrueに
		g_wipe[0].ch.flag = true;
		//遷移の種類
		g_wipe[0].type = type;
		//拡縮倍率
		g_wipe[1].ch.scale = 1.0f;
		//縮小速度
		g_wipe[1].contraction_speed = WIPE_DEFAULT_CONTRACTION_SPEED;
		//拡大速度
		g_wipe[1].scale_speed = WIPE_DEFAULT_SCALE_SPEED;

		//上から拡縮「横長の長方形」
		g_wipe[2].ch.x = 0.0f;
		g_wipe[2].ch.y = -240.0f;
		g_wipe[2].ch.w = 640;
		g_wipe[2].ch.h = 260;
		g_wipe[2].ch.picx = 0;
		g_wipe[2].ch.picy = 0;
		//拡縮倍率
		g_wipe[2].ch.scale = 1.0f;

		//左から拡縮「縦長の長方形」
		g_wipe[3].ch.x = -320.0f;
		g_wipe[3].ch.y = 0.0f;
		g_wipe[3].ch.w = 340;
		g_wipe[3].ch.h = 480;
		g_wipe[3].ch.picx = 0;
		g_wipe[3].ch.picy = 0;
		//拡縮倍率
		g_wipe[3].ch.scale = 1.0f;


		//下から拡縮「横長の長方形」
		g_wipe[4].ch.x = 0.0f;
		g_wipe[4].ch.y = 480.0f;
		g_wipe[4].ch.w = 640;
		g_wipe[4].ch.h = 260;
		g_wipe[4].ch.picx = 0;
		g_wipe[4].ch.picy = 0;
		//拡縮倍率
		g_wipe[4].ch.scale = 1.0f;


		//右から拡縮「横長の長方形」
		g_wipe[5].ch.x = 640.0f;
		g_wipe[5].ch.y = 0.0f;
		g_wipe[5].ch.w = 340;
		g_wipe[5].ch.h = 480;
		g_wipe[5].ch.picx = 0;
		g_wipe[5].ch.picy = 0;
		//拡縮倍率
		g_wipe[5].ch.scale = 1.0f;


	}
}
