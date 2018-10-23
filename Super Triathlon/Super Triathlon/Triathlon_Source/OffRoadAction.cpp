#include	"OffRoadAction.h"
#include	"play.h"
#include	"input.h"
#include	"sound.h"
#include	<iostream>

//名前空間
using namespace std;


//▼ 静的メンバ変数の定義
const int COffRoadAction::OFFROADACTION_IMAGE_MAX = 2;			//使用枚数
const int COffRoadAction::OFFROADACTION_EFFECT_SOUND_MAX = 2;	//効果音使用数

//ウィンドウハンドル
HWND	COffRoadAction::m_hWnd = NULL;
//各種初期値
//①アクションコマンドのフレーム
//表示座標（ローカル座標）
const float COffRoadAction::OFFROADACTION_FRAM_DEFAULT_POSITION_X = 180.0f;			//表示座標X
const float COffRoadAction::OFFROADACTION_FRAM_DEFAULT_POSITION_Y = 380.0f;			//表示座標Y
//画像サイズ
const int COffRoadAction::OFFROADACTION_FRAM_IMAGE_WIDTH = 160;		//幅
const int COffRoadAction::OFFROADACTION_FRAM_IMAGE_HEIGHT = 64;		//高さ
//拡縮倍率
const float COffRoadAction::OFFROADACTION_FRAM_DEFAULT_SCALE = 1.0f;

//枠の設定
//サイズ
TPOINT<int>		COffRoadAction::m_FramSize = {0};
//表示座標
TPOINT<float>	COffRoadAction::m_indicateFramPos = {0};

//②アクションコマンド画像
//各種初期値
//表示座標（ローカル座標）
const float COffRoadAction::OFFROADACTION_COMMAND_DEFAULT_POSITION_X = 
COffRoadAction::OFFROADACTION_FRAM_DEFAULT_POSITION_X + 5.0f;			//表示座標X
const float COffRoadAction::OFFROADACTION_COMMAND_DEFAULT_POSITION_Y =
COffRoadAction::OFFROADACTION_FRAM_DEFAULT_POSITION_Y + 10.0f;			//表示座標Y

//画像サイズ
const int COffRoadAction::OFFROADACTION_COMMAND_IMAGE_WIDTH = 40;		//幅
const int COffRoadAction::OFFROADACTION_COMMAND_IMAGE_HEIGHT = 40;		//高さ
//拡縮倍率
const float COffRoadAction::OFFROADACTION_COMMAND_DEFAULT_SCALE = 1.0f;
//成否に応じた画像の種類
const CORRECTINPUTTYPE COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_NORMAL	= 0;
const CORRECTINPUTTYPE COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_EROR	= 1;
const CORRECTINPUTTYPE COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_SUCCESS = 2;
//成否に応じた種類数
const int COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_MAX = 3;


//アクションコマンド表示間隔
const int COffRoadAction::OFFROADACTION_COMMAND_INTERVAL_TIME_EASY_MODE		= 55;		//Easy時
const int COffRoadAction::OFFROADACTION_COMMAND_INTERVAL_TIME_NORMAL_MODE	= 40;		//Normal時
const int COffRoadAction::OFFROADACTION_COMMAND_INTERVAL_TIME_HARD_MODE		= 31;		//Hard時

//アクションコマンド表示数
const int COffRoadAction::OFFROADACTION_COMMAND_CREATION_MAX_EASY_MODE		= 4;		//Easy時
const int COffRoadAction::OFFROADACTION_COMMAND_CREATION_MAX_NORMAL_MODE	= 6;		//Normal時
const int COffRoadAction::OFFROADACTION_COMMAND_CREATION_MAX_HARD_MODE		= 8;		//Hard時

//コマンド生成中フラグ
bool	COffRoadAction::m_CommandCreateFlag = false;

/*
************************************************************************************************
コンストラクタ
************************************************************************************************
*/
COffRoadAction::COffRoadAction(){

	this->m_actionInterval = 0;
	this->m_commandMax = 0;
	this->m_commandCnt = 0;
	this->m_CommandTypeNow = OFFROADACTION_COMMAND_TYPE_VACANT;
	this->m_hWnd = NULL;
	this->m_lpCOffRoadActionImg = NULL;
	this->m_lpCommandType = NULL;
	this->m_lpCorrectInput = NULL;
	this->m_CommandMaterialNow = -1;

}

/*
************************************************************************************************
ディストラクタ
************************************************************************************************
*/
COffRoadAction::~COffRoadAction() {
	//入力の成否
	delete[] this->m_lpCorrectInput;
	this->m_lpCorrectInput = NULL;
}

//************************************************************************************************
//アクションコマンドでの初期化
//************************************************************************************************
void COffRoadAction::InitOffRoadAction(){
	
	//ゲーム難易度を取得
	tagGAMELEVEL	gameLevel = GetGameLevel();

	//コマンド表示間隔データテーブル
	int	commandIntervalTbl[LEVEL_MAX] = {
		
		COffRoadAction::OFFROADACTION_COMMAND_INTERVAL_TIME_EASY_MODE ,			//EASY
		COffRoadAction::OFFROADACTION_COMMAND_INTERVAL_TIME_NORMAL_MODE ,		//NORMAL
		COffRoadAction::OFFROADACTION_COMMAND_INTERVAL_TIME_HARD_MODE ,			//HARD
	};

	//コマンド表示間隔データテーブル
	int	commandCreationTbl[LEVEL_MAX] = {
		
		COffRoadAction::OFFROADACTION_COMMAND_CREATION_MAX_EASY_MODE ,			//EASY
		COffRoadAction::OFFROADACTION_COMMAND_CREATION_MAX_NORMAL_MODE ,		//NORMAL
		COffRoadAction::OFFROADACTION_COMMAND_CREATION_MAX_HARD_MODE ,			//HARD
	};

	//▼ 読み込む画像のデータテーブル
	char	*imageTbl[ COffRoadAction::OFFROADACTION_IMAGE_MAX ] = {
	
		{"Image\\OffRoad\\ActionCommandFram.bmp"} ,			//アクションコマンドのフレーム
		{"Image\\OffRoad\\ActionCommand.bmp"} ,				//アクションコマンド
	};

	//読み込む効果音のデータテーブル
	char	*seTbl[ COffRoadAction::OFFROADACTION_EFFECT_SOUND_MAX ] = {

		{ "SE\\cursor.wav"} ,		//コマンド成功音
		{ "SE\\cancel.wav"} ,		//コマンド失敗音
	};

	//▼ コマンドアクションの難易度に応じた数だけ配列を確保する
	//コマンドタイプ
	this->m_lpCommandType = new tagCOMMANDACTIONTYPE[ commandCreationTbl[gameLevel] ];

	//入力の成否
	this->m_lpCorrectInput = new CORRECTINPUTTYPE[ commandCreationTbl[gameLevel] ];
	for (int i = 0; i < commandCreationTbl[gameLevel]; i++) { this->m_lpCorrectInput[i] = COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_NORMAL; }

	//▼ メモリの確保
	//今回使用する画像の枚数分だけメモリを確保する
	this->m_lpCOffRoadActionImg = new int[ COffRoadAction::OFFROADACTION_IMAGE_MAX ];

	//CHARACTER構造体の要素を確保する
	//①アクションコマンドのフレーム
	this->m_lpOffRoadActionFram = new CHARACTER;
	//0で初期化
	ZeroMemory( this->m_lpOffRoadActionFram , sizeof( CHARACTER ) );

	//②アクションコマンド
	this->m_lpOffRoadActionCommand = new CHARACTER[ commandCreationTbl[gameLevel] ];
	//0で初期化
	ZeroMemory( this->m_lpOffRoadActionCommand , sizeof( CHARACTER ) * commandCreationTbl[gameLevel] );
	
	//効果音のメモリを確保
	this->m_lpCOffRoadActionSe = new int [ COffRoadAction::OFFROADACTION_EFFECT_SOUND_MAX ];

	//▼ 表示設定
	//①アクションコマンドのフレーム
	this->m_lpOffRoadActionFram[0].x = COffRoadAction::OFFROADACTION_FRAM_DEFAULT_POSITION_X;			//表示座標X
	this->m_lpOffRoadActionFram[0].y = COffRoadAction::OFFROADACTION_FRAM_DEFAULT_POSITION_Y;			//表示座標Y
	this->m_lpOffRoadActionFram[0].w = COffRoadAction::OFFROADACTION_FRAM_IMAGE_WIDTH;					//幅
	this->m_lpOffRoadActionFram[0].h = COffRoadAction::OFFROADACTION_FRAM_IMAGE_HEIGHT;					//高さ
	this->m_lpOffRoadActionFram[0].flag = false;														//表示フラグ
	//拡縮倍率をゲームレベルに応じて変更させる
	//コマンドの数に応じた長さを求める
	float commandWidth = float(COffRoadAction::OFFROADACTION_COMMAND_IMAGE_WIDTH * commandCreationTbl[gameLevel]);
	this->m_lpOffRoadActionFram[0].scale = float(commandWidth / COffRoadAction::OFFROADACTION_FRAM_IMAGE_WIDTH + 0.1f);

	//枠の設定取得用
	//座標
	this->m_indicateFramPos.x = COffRoadAction::OFFROADACTION_FRAM_DEFAULT_POSITION_X;
	this->m_indicateFramPos.y = COffRoadAction::OFFROADACTION_FRAM_DEFAULT_POSITION_Y;
	//サイズ
	this->m_FramSize.x = COffRoadAction::OFFROADACTION_FRAM_IMAGE_WIDTH;
	this->m_FramSize.y = COffRoadAction::OFFROADACTION_FRAM_IMAGE_HEIGHT;

	//②アクションコマンド
	//扱うコマンドの最大数
	this->m_commandMax = commandCreationTbl[gameLevel];

	for( int i = 0 ; i < this->m_commandMax ; i++ ){
		this->m_lpOffRoadActionCommand[i].w = COffRoadAction::OFFROADACTION_COMMAND_IMAGE_WIDTH;			//幅
		this->m_lpOffRoadActionCommand[i].h = COffRoadAction::OFFROADACTION_COMMAND_IMAGE_HEIGHT;			//高さ
		this->m_lpOffRoadActionCommand[i].scale = COffRoadAction::OFFROADACTION_COMMAND_DEFAULT_SCALE;		//拡縮倍率
		this->m_lpOffRoadActionCommand[i].flag = false;														//表示フラグ
	}

	//コマンド表示間隔
	this->m_actionInterval = commandIntervalTbl[gameLevel];

	//▼ 画像の読み込み
	for( int i = 0 ; i < COffRoadAction::OFFROADACTION_IMAGE_MAX ; i++ ){

		this->m_lpCOffRoadActionImg[i] = CreateDDImage( imageTbl[i] , COffRoadAction::m_hWnd );

	}

	//▼ 効果音の読み込み
	for( int i = 0 ; i < COffRoadAction::OFFROADACTION_EFFECT_SOUND_MAX ; i++ ){

		this->m_lpCOffRoadActionSe[i] = CreateSound( seTbl[i] );
	}


}

//************************************************************************************************
//アクションコマンドの更新
//戻り値 ＝ アクション成功の成否
//************************************************************************************************
bool COffRoadAction::UpdateOffRoadAction() {

	//アクション失敗フラグ
	bool actionEror = false;

	//コマンド成否に応じた種類データテーブル
	CORRECTINPUTTYPE	correctTypeTbl[COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_MAX] = {

		COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_NORMAL	,
		COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_EROR		,
		COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_SUCCESS	,
	};

	//▼ コマンドの成否に応じた更新
	//コマンド表示中なら処理
	if (this->m_lpOffRoadActionFram[0].flag) {
		for (int j = 0; j < COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_MAX; j++) {
			for (int i = this->m_CommandMaterialNow; i < this->m_commandMax; i++) {

				//コマンド成否に応じて画像の差し替え
				if (this->m_lpCorrectInput[i] == correctTypeTbl[j]) {

					//1回でも入力にミスしていたらアクション成功フラグをfalseに
					if (this->m_lpCorrectInput[i] == COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_EROR) actionEror = true;
					//表示元座標を変更
					this->m_lpOffRoadActionCommand[i].picy = correctTypeTbl[j] * this->m_lpOffRoadActionCommand[i].h;
					continue;
				}
			}
		}
	}
	//アクション失敗フラグ
	return actionEror;
}


//************************************************************************************************
//アクションコマンドの表示
//************************************************************************************************
void COffRoadAction::DrawOffRoadAction(){


	//アクションコマンドフレームの表示
	if( this->m_lpOffRoadActionFram[0].flag ){

		DDScaleOnlyWidthBlt( 
			this->m_lpOffRoadActionFram[0].x ,
			this->m_lpOffRoadActionFram[0].y ,
			this->m_lpOffRoadActionFram[0].w ,
			this->m_lpOffRoadActionFram[0].h ,
			this->m_lpCOffRoadActionImg[0] ,
			this->m_lpOffRoadActionFram[0].picx ,
			this->m_lpOffRoadActionFram[0].picy ,
			this->m_lpOffRoadActionFram[0].scale
			);
	}

	
	//アクションコマンドの表示
	for( int i = 0 ; i < this->m_commandMax ; i++ ){
		if( this->m_lpOffRoadActionCommand[i].flag ){

			DDScaleBlt( 
				this->m_lpOffRoadActionCommand[i].x ,
				this->m_lpOffRoadActionCommand[i].y ,
				this->m_lpOffRoadActionCommand[i].w ,
				this->m_lpOffRoadActionCommand[i].h ,
				this->m_lpCOffRoadActionImg[1] ,
				this->m_lpOffRoadActionCommand[i].picx ,
				this->m_lpOffRoadActionCommand[i].picy ,
				this->m_lpOffRoadActionCommand[i].scale
				);
		}
	}
	
}

//***************************************************************************************************
//解放
//***************************************************************************************************
void COffRoadAction::ReleaseOffRoadAction(){

	//画像
	//未解放なら解放処理
	if( this->m_lpCOffRoadActionImg != NULL ){
		for( int i = 0 ; i < COffRoadAction::OFFROADACTION_IMAGE_MAX ; i++ ){
			ReleaseDDImage( this->m_lpCOffRoadActionImg[i] );
		}
	}
	delete []this->m_lpCOffRoadActionImg;
	this->m_lpCOffRoadActionImg = NULL;
	
	//効果音
	//未解放なら解放処理
	if( this->m_lpCOffRoadActionSe != NULL ){
		for( int i = 0 ; i < COffRoadAction::OFFROADACTION_EFFECT_SOUND_MAX ; i++ ){
			ReleaseSound( this->m_lpCOffRoadActionSe[i] );
		}
	}
	delete []this->m_lpCOffRoadActionSe;
	this->m_lpCOffRoadActionSe = NULL;

	//CHARACTER構造体の解放
	//①アクションコマンドのフレーム
	delete this->m_lpOffRoadActionFram;
	this->m_lpOffRoadActionFram = NULL;

	//②アクションコマンド
	delete[] this->m_lpOffRoadActionCommand;
	this->m_lpOffRoadActionCommand = NULL;

	//アクションコマンドの種類番号を扱う配列
	delete[] this->m_lpCommandType;
	this->m_lpCommandType = NULL;

}

/*
***************************************************************************************************
水平回転アクションコマンドの初期化
***************************************************************************************************
*/
void COffRoadAction::InitLevelRotateActionCommand(){

	//アクションコマンドの種類番号データテーブル
	tagCOMMANDACTIONTYPE	commandTbl[ OFFROADACTION_COMMAND_KIND_OF_ACTION ]
	[ COffRoadAction::OFFROADACTION_COMMAND_CREATION_MAX_HARD_MODE] = {

		//右１回転
		{ OFFROADACTION_COMMAND_TYPE_UP		,		//上
		OFFROADACTION_COMMAND_TYPE_RIGHT	,		//右
		OFFROADACTION_COMMAND_TYPE_DOWN		,		//下
		OFFROADACTION_COMMAND_TYPE_LEFT		,		//左
		OFFROADACTION_COMMAND_TYPE_UP		,		//上
		OFFROADACTION_COMMAND_TYPE_LEFT		,		//左
		OFFROADACTION_COMMAND_TYPE_DOWN		,		//下
		OFFROADACTION_COMMAND_TYPE_RIGHT    }		//右
		,
		//左１回転
		{ OFFROADACTION_COMMAND_TYPE_UP		,		//上
		OFFROADACTION_COMMAND_TYPE_LEFT		,		//左
		OFFROADACTION_COMMAND_TYPE_DOWN		,		//下
		OFFROADACTION_COMMAND_TYPE_RIGHT	,		//右
		OFFROADACTION_COMMAND_TYPE_UP		,		//上
		OFFROADACTION_COMMAND_TYPE_RIGHT	,		//右
		OFFROADACTION_COMMAND_TYPE_DOWN		,		//下
		OFFROADACTION_COMMAND_TYPE_LEFT		}		//左
		,
		//右半回転
		{ OFFROADACTION_COMMAND_TYPE_UP		,		//上
		OFFROADACTION_COMMAND_TYPE_RIGHT	,		//右
		OFFROADACTION_COMMAND_TYPE_UP		,		//上
		OFFROADACTION_COMMAND_TYPE_LEFT		,		//左
		OFFROADACTION_COMMAND_TYPE_UP		,		//上
		OFFROADACTION_COMMAND_TYPE_RIGHT	,		//右
		OFFROADACTION_COMMAND_TYPE_VACANT	,
		OFFROADACTION_COMMAND_TYPE_VACANT	}
		,
		//左半回転
		{ OFFROADACTION_COMMAND_TYPE_UP		,		//上
		OFFROADACTION_COMMAND_TYPE_LEFT		,		//左
		OFFROADACTION_COMMAND_TYPE_UP		,		//上
		OFFROADACTION_COMMAND_TYPE_RIGHT	,		//右
		OFFROADACTION_COMMAND_TYPE_UP		,		//上
		OFFROADACTION_COMMAND_TYPE_LEFT		,		//左
		OFFROADACTION_COMMAND_TYPE_VACANT	,
		OFFROADACTION_COMMAND_TYPE_VACANT	}

	};

	//ランダムの値を取得
	int ran = RandomNum( 3 , 0 );

	//生成最大数までループ
	for( int i = 0 ; i < this->m_commandMax ; i++ ){

		//コマンドなしならbreak
		if( commandTbl[ran][i] == OFFROADACTION_COMMAND_TYPE_VACANT ) break;

		//扱うコマンド数
		this->m_commandCnt = ( i + 1 );

		//表示座標X
		this->m_lpOffRoadActionCommand[i].x = COffRoadAction::OFFROADACTION_COMMAND_DEFAULT_POSITION_X + 
			( this->m_lpOffRoadActionCommand[i].w * this->m_lpOffRoadActionCommand[i].scale ) * i;
		//表示座標Y
		this->m_lpOffRoadActionCommand[i].y = COffRoadAction::OFFROADACTION_COMMAND_DEFAULT_POSITION_Y;
		//表示元座標X
		this->m_lpOffRoadActionCommand[i].picx = ( commandTbl[ran][i] - OFFROADACTION_COMMAND_TYPE_LEFT ) * 
			COffRoadAction::OFFROADACTION_COMMAND_IMAGE_WIDTH;				
		//表示するコマンドタイプ
		this->m_lpCommandType[i] = commandTbl[ran][i];														//種類番号
	}

}


/*
***************************************************************************************************
アクションコマンドの生成
***************************************************************************************************
*/
bool COffRoadAction::CreateActionCommand( bool actionFlag ){

	//引数がtrueなら枠を表示
	if( actionFlag && this->m_lpOffRoadActionFram[0].flag == false ){
		//枠の表示
		this->m_lpOffRoadActionFram[0].flag = true;
		//コマンドの初期化
		//水平アクション
		this->InitLevelRotateActionCommand();
	}


	//枠の表示から時間経過でコマンドを表示していく
	if( this->m_lpOffRoadActionFram[0].flag ){

		//今回扱うコマンド数だけループ
		for( int i = 0 ; i < this->m_commandCnt ; i++ ){

			//表示済みなら次の要素へ
			if( this->m_lpOffRoadActionCommand[i].flag ) continue;

			//時間経過
			this->m_lpOffRoadActionCommand[i].time++;

			//設定したフレーム毎に表示
			if (this->m_lpOffRoadActionCommand[i].time % (this->m_actionInterval * (i + 1)) == 0) {
				//subsutitute draw command flag for static variable
				this->m_CommandCreateFlag = true;
				//コマンドの表示
				this->m_lpOffRoadActionCommand[i].flag = true;
				//現在表示される要素を代入
				this->m_CommandMaterialNow = i;
				//経過時間のリセット
				this->m_lpOffRoadActionCommand[i].time = 0;
				//ビット目を取得
				this->m_CommandTypeNow = this->m_lpCommandType[i];
				//時間経過後のコマンドはエラー表示にする
				if (i > 0 && i < this->m_commandCnt) {					//配列外回避処理
					//don't process when player's input was success and eror
					if (this->m_lpCorrectInput[i - 1] != COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_SUCCESS &&
						this->m_lpCorrectInput[i - 1] != COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_EROR) {
						this->m_lpCorrectInput[i - 1] = COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_EROR;
						PlaySound(this->m_lpCOffRoadActionSe[1], 0, false);
						//表示元座標を変更
						this->m_lpOffRoadActionCommand[i - 1].picy = COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_EROR * this->m_lpOffRoadActionCommand[i - 1].h;
					}
				}
			}
		}
		//最後のアクションコマンド表示後、設定した表示間隔経過後、非表示にする
		if( this->m_lpOffRoadActionCommand[ ( this->m_commandCnt - 1 ) ].flag ){
			//時間経過
			this->m_lpOffRoadActionCommand[ ( this->m_commandCnt - 1 ) ].time++;

			//時間経過で非表示にし、経過時間をリセット
			if( this->m_lpOffRoadActionCommand[ ( this->m_commandCnt - 1 ) ].time % 
				this->m_actionInterval == 0 ){
				//フレームを非表示に
				this->m_lpOffRoadActionFram[0].flag = false;
				//表示したコマンドの要素をリセット
				this->m_CommandMaterialNow = -1;
				//コマンド生成中フラグを取得用に代入
				this->m_CommandCreateFlag = false;
				//経過時間をリセット
				this->m_lpOffRoadActionCommand[ ( this->m_commandCnt - 1 ) ].time = 0;
				//表示中のアクションコマンドの種類番号をリセット
				this->m_CommandTypeNow = OFFROADACTION_COMMAND_TYPE_VACANT;
				//表示されたコマンドの数だけ非表示にし、表示元座標を通常に
				for( int i = 0 ; i < this->m_commandCnt ; i++ ){ 
					this->m_lpOffRoadActionCommand[i].flag = false;
					//コマンドの成否をリセット
					this->m_lpCorrectInput[i] = COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_NORMAL;
				}
			}
		}

	}

	//枠表示中はコマンド生成中フラグを返す
	return this->m_lpOffRoadActionFram[0].flag;

}

/*
************************************************************************************************
アクションコマンドの入力状況を扱う
戻り値
成功 ： 入力したキーコードに対応したビット目
失敗 ： -1
************************************************************************************************
*/
BYTE COffRoadAction::InputActionCommand(){
	
	//コマンドの種類番号データテーブル
	tagCOMMANDACTIONTYPE	commandKindTbl1[ OFFROADACTION_COMMAND_KIND_OF_TYPE ] = {

		OFFROADACTION_COMMAND_TYPE_UP		,		//上
		OFFROADACTION_COMMAND_TYPE_RIGHT	,		//右
		OFFROADACTION_COMMAND_TYPE_DOWN		,		//下
		OFFROADACTION_COMMAND_TYPE_LEFT		,		//左
	};

	//ゲームパッド用
	BYTE	commandKindTbl2[OFFROADACTION_COMMAND_KIND_OF_TYPE] = {
		LEVER_UP,
		LEVER_RIGHT,
		LEVER_DOWN,
		LEVER_LEFT,
	};

	//コマンドのビット目のデータテーブル
	BYTE	commandBitTbl[ OFFROADACTION_COMMAND_KIND_OF_TYPE ] = {

		OFFROADACTION_COMMAND_BIT_TYPE_UP		,		//上
		OFFROADACTION_COMMAND_BIT_TYPE_RIGHT	,		//右
		OFFROADACTION_COMMAND_BIT_TYPE_DOWN		,		//下
		OFFROADACTION_COMMAND_BIT_TYPE_LEFT		,		//左
	};

	//戻り値用
	BYTE	ret = OFFROADACTION_COMMAND_BIT_TYPE_VACANT;

	//入力は1回のみ
	if( this->m_lpCorrectInput[ this->m_CommandMaterialNow ] == COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_EROR ||
		this->m_lpCorrectInput[ this->m_CommandMaterialNow ] == COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_SUCCESS ){
		return OFFROADACTION_COMMAND_BIT_TYPE_VACANT;
	}
	//入力状況を調べる
	for( int i = 0 ; i < OFFROADACTION_COMMAND_KIND_OF_TYPE ; i++ ){

		//入力を取得
		if (JustKey(commandKindTbl1[i]) || JustLever(commandKindTbl2[i])) {

			//対応したコマンドの種類番号（コード番号）を調べる
			if (this->m_CommandTypeNow == commandKindTbl1[i] || this->m_CommandTypeNow == commandKindTbl2[i]) {

				//効果音
				PlaySound(this->m_lpCOffRoadActionSe[0], 0, false);

				//コマンドの成否を代入
				this->m_lpCorrectInput[this->m_CommandMaterialNow] = COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_SUCCESS;

				//対応したコマンドのビット目を戻り値に代入
				ret = commandBitTbl[i];
				break;
			}
			else {
				//異なるコマンド
				//効果音
				PlaySound(this->m_lpCOffRoadActionSe[1], 0, false);
				//コマンドの成否を代入
				this->m_lpCorrectInput[this->m_CommandMaterialNow] = COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_EROR;
				return ret;
			}
		}
	}

	return ret;

}