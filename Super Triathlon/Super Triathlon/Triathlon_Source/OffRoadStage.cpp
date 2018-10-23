#include	"OffRoadStage.h"
#include	"OffRoadRacer.h"
#include	"map.h"
#include	"play.h"
#include	"wipe.h"
#include	<stdio.h>
#include	"input.h"


//▼ 静的メンバ変数の定義
const int COffRoadStage::OFFROADSTAGE_IMAGE_MAX = 5;							//使用枚数

//背景のサイズ
//地面
const int COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GROUND_WIDTH = 640;			//幅
const int COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GROUND_HEIGHT = 480;			//高さ
//ゴール地点
const int COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GOAL_POINT_WIDTH = 320;		//幅
const int COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GOAL_POINT_HEIGHT = 40;		//高さ

//背景の種類数
const int COffRoadStage::OFFROADSTAGE_BACK_IMAGE_KIND_OF_TYPE = 2;

//ジャンプポイントのサイズ
const int COffRoadStage::OFFROADSTAGE_JUMP_POINT_IMAGE_WIDTH = 128;			//幅
const int COffRoadStage::OFFROADSTAGE_JUMP_POINT_IMAGE_HEIGHT = 64;			//高さ
//拡縮倍率初期値
const float COffRoadStage::OFFROADSTAGE_JUMP_POINT_DEFAULT_SCALE = 1.0f;
//表示Ｘの間隔
const int COffRoadStage::OFFROADSTAGE_JUMP_POINT_INDICATE_X_SPACE = 100;
//表示Ｘの配置最大数
const int COffRoadStage::OFFROADSTAGE_JUMP_POINT_INDICATE_X_POSITION_MAX = 3;
//配置最大数
const int COffRoadStage::OFFROADSTAGE_JUMP_POINT_POSITION_MAX = 1;

//ジャンプ地点の情報取得用
CHARACTER* COffRoadStage::m_lpOffRoadGetJumpPoint = NULL;

//ウィンドウハンドル
HWND	COffRoadStage::m_hWnd = NULL;

//ステージの距離
const int COffRoadStage::OFFROADSTAGE_DISTANCE_EAZY		= 20000;	//EAZYモード
const int COffRoadStage::OFFROADSTAGE_DISTANCE_NORMAL	= 30000;	//NORMALモード
const int COffRoadStage::OFFROADSTAGE_DISTANCE_HARD		= 40000;	//HARDモード

//ステージの全長を扱う
float COffRoadStage::m_wholeDistance = 0;

//今回のステージの画面サイズ
const int COffRoadStage::OFFROADSTAGE_SCREEN_WIDTH	= 640;		//幅
const int COffRoadStage::OFFROADSTAGE_SCREEN_HEIGHT = 480;		//高さ

//setting brief map
 const int COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_WIDTH = 32;						//for back ground
 const int COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_HEIGHT = 420;
 const float COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_INDICATE_X = 598.0f;
 const float COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_INDICATE_Y = 30.0f;
 const float COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_RACER_DEFAULT_SCALE = 0.5f;		//for minimum racer
 const int COffRoadStage::OFFROADSTAGE_MINIMAP_KIND_OF_IMAGE = 2;						//kind of image

																		/*
************************************************************************************************
ディストラクタ
************************************************************************************************
*/
COffRoadStage::~COffRoadStage() {

}

//************************************************************************************************
//陸上ステージでの初期化
//************************************************************************************************
void COffRoadStage::InitOffRoadStage() {

	//難易度を取得
	tagGAMELEVEL gameLevel = GetGameLevel();

	//ステージの全長データテーブル
	int	distanceTbl[LEVEL_MAX] = {
		COffRoadStage::OFFROADSTAGE_DISTANCE_EAZY ,		//Easy
		COffRoadStage::OFFROADSTAGE_DISTANCE_NORMAL ,	//Normal
		COffRoadStage::OFFROADSTAGE_DISTANCE_HARD ,		//Hard
	};

	//ジャンプ台の生成間隔
	int	CreationFramTbl[LEVEL_MAX] = { 200 , 150 , 70 };

	//難易度に応じて距離を決定
	this->m_distance = (float)distanceTbl[gameLevel];
	//for get
	this->m_wholeDistance = (float)distanceTbl[gameLevel];

	//ジャンプ台の生成間隔
	this->m_CreatrionFram = CreationFramTbl[gameLevel];

	//▼ 読み込む画像のデータテーブル
	char	*imageTbl[COffRoadStage::OFFROADSTAGE_IMAGE_MAX] = {
		{"Image\\OffRoad\\StageBackImage.bmp"} ,
		{"Image\\OffRoad\\JumpPoint.bmp"} ,
		{"Image\\OffRoad\\GoalLine.bmp"} ,
		{"Image\\Road\\Minimap.bmp"},
		{"Image\\OffRoad\\cyclist.bmp"},
	};

	//▼ 画像の設定
	//今回使用する画像の枚数分だけメモリを確保する
	this->m_lpCOffRoadImg = new int[COffRoadStage::OFFROADSTAGE_IMAGE_MAX];

	//CHARACTER構造体の要素を確保する
	//背景
	this->m_lpOffRoad = new CHARACTER[COffRoadStage::OFFROADSTAGE_BACK_IMAGE_KIND_OF_TYPE];
	//0で初期化
	ZeroMemory(this->m_lpOffRoad, sizeof(CHARACTER) * COffRoadStage::OFFROADSTAGE_BACK_IMAGE_KIND_OF_TYPE);
	//ジャンプ台
	this->m_lpOffRoadJumpPoint = new CHARACTER[COffRoadStage::OFFROADSTAGE_JUMP_POINT_POSITION_MAX];
	//0で初期化
	ZeroMemory(this->m_lpOffRoadJumpPoint, sizeof(CHARACTER) * COffRoadStage::OFFROADSTAGE_JUMP_POINT_POSITION_MAX);
	//ジャンプ台取得用
	this->m_lpOffRoadGetJumpPoint = new CHARACTER[COffRoadStage::OFFROADSTAGE_JUMP_POINT_POSITION_MAX];
	//0で初期化
	ZeroMemory(this->m_lpOffRoadGetJumpPoint, sizeof(CHARACTER) * COffRoadStage::OFFROADSTAGE_JUMP_POINT_POSITION_MAX);
	//for brief map
	this->m_lpMinimap = new CHARACTER[COffRoadStage::OFFROADSTAGE_MINIMAP_KIND_OF_IMAGE];
	ZeroMemory(&this->m_lpMinimap[0], (sizeof(CHARACTER) * COffRoadStage::OFFROADSTAGE_MINIMAP_KIND_OF_IMAGE));
	//ライダーのクラス
	this->m_lpCOffRoadRacer = new COffRoadRacer;

	//▼ 表示設定
	//背景「地面」
	this->m_lpOffRoad[0].w = COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GROUND_WIDTH;					//幅
	this->m_lpOffRoad[0].h = COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GROUND_HEIGHT;					//高さ
	this->m_lpOffRoad[0].flag = true;																//表示フラグ

	//ゴール地点
	this->m_lpOffRoad[1].x = (COffRoadStage::OFFROADSTAGE_SCREEN_WIDTH -
		COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GOAL_POINT_WIDTH) / 2;								//表示先X
	this->m_lpOffRoad[1].y = 0.0f;																	//表示先Y
	this->m_lpOffRoad[1].w = COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GOAL_POINT_WIDTH;				//幅
	this->m_lpOffRoad[1].h = COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GOAL_POINT_HEIGHT;				//高さ
	this->m_lpOffRoad[1].flag = true;																//表示フラグ

	//ジャンプ地点
	for (int i = 0; i < COffRoadStage::OFFROADSTAGE_JUMP_POINT_POSITION_MAX; i++) {

		this->m_lpOffRoadJumpPoint[i].w = COffRoadStage::OFFROADSTAGE_JUMP_POINT_IMAGE_WIDTH;		//幅
		this->m_lpOffRoadJumpPoint[i].h = COffRoadStage::OFFROADSTAGE_JUMP_POINT_IMAGE_HEIGHT;		//高さ
		this->m_lpOffRoadJumpPoint[i].scale = COffRoadStage::OFFROADSTAGE_JUMP_POINT_DEFAULT_SCALE;	//拡縮倍率
	}

	//initialize brief map
	this->m_lpMinimap[0].x = COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_INDICATE_X;
	this->m_lpMinimap[0].y = COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_INDICATE_Y;
	this->m_lpMinimap[0].w = COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_WIDTH;
	this->m_lpMinimap[0].h = COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_HEIGHT;
	this->m_lpMinimap[0].scale = 1.0f;
	this->m_lpMinimap[0].flag = true;
	//for a minimum cyclist
	//top width
	float cyclistW = this->m_lpCOffRoadRacer->OFFROADRACER_IMAGE_WIDTH * COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_RACER_DEFAULT_SCALE;
	this->m_lpMinimap[1].y = COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_INDICATE_Y + COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_HEIGHT;
	this->m_lpMinimap[1].w = this->m_lpCOffRoadRacer->OFFROADRACER_IMAGE_WIDTH;
	this->m_lpMinimap[1].h = this->m_lpCOffRoadRacer->OFFROADRACER_IMAGE_HEIGHT;
	this->m_lpMinimap[1].scale = COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_RACER_DEFAULT_SCALE;
	this->m_lpMinimap[1].x = this->m_lpMinimap[0].x + ((this->m_lpMinimap[0].w - cyclistW) / 2);
	this->m_lpMinimap[1].flag = true;

	//▼ 画像の読み込み
	for (int i = 0; i < COffRoadStage::OFFROADSTAGE_IMAGE_MAX; i++) {

		this->m_lpCOffRoadImg[i] = CreateDDImage(imageTbl[i], COffRoadStage::m_hWnd);
	}

	//画面の範囲を変更
	ChangeCamera((float)(this->m_distance - COffRoadStage::OFFROADSTAGE_SCREEN_HEIGHT),
		(float)(this->m_distance),
		(float)(COffRoadStage::OFFROADSTAGE_SCREEN_WIDTH),
		(float)(this->m_distance));
}

//************************************************************************************************
//陸上ステージの更新
//************************************************************************************************
void COffRoadStage::UpdateOffRoadStage() {

	//ライダーの速度を取得
	float racerSpeed = this->m_lpCOffRoadRacer[0].GetOffRoadRacerSpeed();
	//ライダーの座標を取得
	TPOINT<float>	riderPos = this->m_lpCOffRoadRacer[0].GetRiderPosition();

	//全長をライダーの移動量で減算
	this->m_distance += racerSpeed;

	//ライダーの速度を基に画面の範囲を変更
	ChangeCamera((float)(this->m_distance - COffRoadStage::OFFROADSTAGE_SCREEN_HEIGHT),
		(float)(this->m_distance),
		(float)(COffRoadStage::OFFROADSTAGE_SCREEN_WIDTH),
		(float)(this->m_distance));


	//▼ ライダーの座標がゴール地点に到達したら、リザルトへ
	if (this->m_lpCOffRoadRacer->GetRacerActionFlag() == false &&
		riderPos.y < (this->m_lpOffRoad[1].y + this->m_lpOffRoad[1].h)) {
		//タイマーをOFFにする
		ControlStageTime(false);
		//リザルトへ
		CreateWipe(SCENE_RESULT, CHANGE_SCENE, WIPE_CONTRACTION);
	}

	//ジャンプ地点生成	ジャンプ台未生成かつ、プレイヤーがアクション中でなければ時間経過で生成
	if (this->m_lpOffRoadJumpPoint[0].flag == false &&
		this->m_lpCOffRoadRacer->GetRacerActionFlag() == false) {
		this->m_lpOffRoadJumpPoint[0].time++;
		// ▼ジャンプ地点の生成
		if (this->m_lpOffRoadJumpPoint[0].time >= this->m_CreatrionFram) {
			this->CreateJumpPoint();
			this->m_lpOffRoadJumpPoint[0].time = 0;
		}
	}

	//画面外に出たら生存フラグをfalse
	for (int i = 0; i < COffRoadStage::OFFROADSTAGE_JUMP_POINT_POSITION_MAX; i++) {
		if (this->m_lpOffRoadJumpPoint[i].flag) {
			//出現するジャンプ台の情報を設定
			this->SetJumpPointInfo(&this->m_lpOffRoadJumpPoint[i], i);
			this->m_lpOffRoadJumpPoint[i].flag = CollisionCamera(&this->m_lpOffRoadJumpPoint[i]);
		}
	}

	//indicate present cyclist's coordinateY in whole, that is brief map
	float indicateY = (riderPos.y / this->m_wholeDistance);
	float topH = this->m_lpMinimap[1].h * this->m_lpMinimap[1].scale;
	this->m_lpMinimap[1].y = ((this->m_lpMinimap[0].h * indicateY) - this->m_lpMinimap[0].y) + topH;
}


//************************************************************************************************
//陸上ステージの表示
//************************************************************************************************
void COffRoadStage::DrawOffRoadStage() {

	//カメラの座標
	float	pY = GetCameraY();

	//背景の表示
	if (this->m_lpOffRoad[0].flag) {

		DDBlt(
			(int)this->m_lpOffRoad[0].x,
			(int)this->m_lpOffRoad[0].y,
			this->m_lpOffRoad[0].w,
			this->m_lpOffRoad[0].h,
			this->m_lpCOffRoadImg[0],
			this->m_lpOffRoad[0].picx,
			this->m_lpOffRoad[0].picy
			);
	}

	//ジャンプ地点の表示
	for (int i = 0; i < COffRoadStage::OFFROADSTAGE_JUMP_POINT_POSITION_MAX; i++) {

		if (this->m_lpOffRoadJumpPoint[i].flag) {

			DDBlt(
				(int)this->m_lpOffRoadJumpPoint[i].x,
				(int)this->m_lpOffRoadJumpPoint[i].y - (int)pY,
				this->m_lpOffRoadJumpPoint[i].w,
				this->m_lpOffRoadJumpPoint[i].h,
				this->m_lpCOffRoadImg[1],
				this->m_lpOffRoadJumpPoint[i].picx,
				this->m_lpOffRoadJumpPoint[i].picy
				);

		}
	}

	//ゴール地点の表示
	if (this->m_lpOffRoad[1].flag) {

		DDBlt(
			(int)this->m_lpOffRoad[1].x,
			(int)(this->m_lpOffRoad[1].y - pY),
			this->m_lpOffRoad[1].w,
			this->m_lpOffRoad[1].h,
			this->m_lpCOffRoadImg[2],
			this->m_lpOffRoad[1].picx,
			this->m_lpOffRoad[1].picy
			);
	}
	//draw brief map
	//material of image
	int image[COffRoadStage::OFFROADSTAGE_MINIMAP_KIND_OF_IMAGE] = { this->m_lpCOffRoadImg[3], this->m_lpCOffRoadImg[4] };
	for (int i = 0; i < COffRoadStage::OFFROADSTAGE_MINIMAP_KIND_OF_IMAGE; i++) {
		if (this->m_lpMinimap[i].flag) {
			DDScaleBltSelectIndicate(
				this->m_lpMinimap[i].x,
				this->m_lpMinimap[i].y,
				this->m_lpMinimap[i].w,
				this->m_lpMinimap[i].h,
				image[i],
				this->m_lpMinimap[i].picx,
				this->m_lpMinimap[i].picy,
				this->m_lpMinimap[i].scale,
				DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP);
		}
	}
}

//***************************************************************************************************
//解放
//***************************************************************************************************
void COffRoadStage::ReleaseOffRoadStage() {

	//未解放なら解放処理
	if (this->m_lpCOffRoadImg != NULL) {
		for (int i = 0; i < COffRoadStage::OFFROADSTAGE_IMAGE_MAX; i++) {
			ReleaseDDImage(this->m_lpCOffRoadImg[i]);
		}
	}
	delete[]this->m_lpCOffRoadImg;
	this->m_lpCOffRoadImg = NULL;

	//CHARACTER構造体の解放
	//背景
	delete[]this->m_lpOffRoad;
	this->m_lpOffRoad = NULL;
	//ジャンプ台
	delete[]this->m_lpOffRoadJumpPoint;
	this->m_lpOffRoadJumpPoint = NULL;
	//ジャンプ台取得用
	delete this->m_lpOffRoadGetJumpPoint;
	this->m_lpOffRoadGetJumpPoint = NULL;

	//ライダーのクラスの解放
	delete this->m_lpCOffRoadRacer;
	this->m_lpCOffRoadRacer = NULL;

	//for brief map
	delete[]this->m_lpMinimap;
	this->m_lpMinimap = NULL;

}

/*
***************************************************************************************************
ジャンプ地点の生成
***************************************************************************************************
*/
void COffRoadStage::CreateJumpPoint() {

	//カメラの座標
	float	pY = GetCameraY();

	//表示Ｘ座標データテーブル
	float	posXTbl[COffRoadStage::OFFROADSTAGE_JUMP_POINT_INDICATE_X_POSITION_MAX] = {

		//左側
		((COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GROUND_WIDTH - COffRoadStage::OFFROADSTAGE_JUMP_POINT_IMAGE_WIDTH) / 2) -
			COffRoadStage::OFFROADSTAGE_JUMP_POINT_INDICATE_X_SPACE ,
		//中央
		(COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GROUND_WIDTH - COffRoadStage::OFFROADSTAGE_JUMP_POINT_IMAGE_WIDTH) / 2 ,
		//右側
		((COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GROUND_WIDTH - COffRoadStage::OFFROADSTAGE_JUMP_POINT_IMAGE_WIDTH) / 2) +
		COffRoadStage::OFFROADSTAGE_JUMP_POINT_INDICATE_X_SPACE

	};

	//ランダムの値
	int ran = RandomNum(COffRoadStage::OFFROADSTAGE_JUMP_POINT_INDICATE_X_POSITION_MAX, 0);

	//配置最大数までループ
	for (int i = 0; i < COffRoadStage::OFFROADSTAGE_JUMP_POINT_POSITION_MAX; i++) {
		if (this->m_lpOffRoadJumpPoint[i].flag == false) {


			//表示設定
			this->m_lpOffRoadJumpPoint[i].x = posXTbl[ran];
			this->m_lpOffRoadJumpPoint[i].y = pY - this->m_lpOffRoadJumpPoint[i].h;
			this->m_lpOffRoadJumpPoint[i].flag = true;

			break;

		}
	}

}

/*
***************************************************************************************************
ジャンプ地点とプレイヤーの重なりを調べる
***************************************************************************************************
*/
bool	COffRoadStage::CollisionJumpPoint(CHARACTER *pch) {

	//戻り値用
	bool ret = false;

	//表示最大数までループ
	for (int i = 0; i < COffRoadStage::OFFROADSTAGE_JUMP_POINT_POSITION_MAX; i++) {

		//ジャンプ台が生成されていいなかったら次の要素へ
		if (this->m_lpOffRoadGetJumpPoint[i].flag == false) continue;

		//引数と判定を取る
		if (CollisionCharacter(pch, &this->m_lpOffRoadGetJumpPoint[i])) {

			ret = true;
			break;
		}

	}

	return ret;

}
