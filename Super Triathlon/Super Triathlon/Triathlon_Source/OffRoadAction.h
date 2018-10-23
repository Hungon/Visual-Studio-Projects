#ifndef OFFROADACTION_H__
#define OFFROADACTION_H__


//▼ ヘッダーのインクルード
#include	"graphics.h"
#include	"CCharacter.h"
#include	<windows.h>

//▼ 列挙型
//コマンドの種類
enum	tagCOMMANDACTIONTYPE{

	OFFROADACTION_COMMAND_TYPE_VACANT = -1 ,	//種類なし
	OFFROADACTION_COMMAND_TYPE_LEFT = 37 ,		//左入力
	OFFROADACTION_COMMAND_TYPE_UP		,		//上入力
	OFFROADACTION_COMMAND_TYPE_RIGHT	,		//右入力
	OFFROADACTION_COMMAND_TYPE_DOWN		,		//下入力
	//コマンドの種類数
	OFFROADACTION_COMMAND_KIND_OF_TYPE = 4,
};

//コマンドのビット目
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_VACANT		 = 0x00;		//種類なし
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_LEFT			 = 0x01;		//左入力
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_UP			 = 0x02;		//上入力
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_RIGHT		 = 0x04;		//右入力
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_DOWN			 = 0x08;		//下入力
//アクション派生用フラグ
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_UP_RIGHT		 = 0x06;		//上入力と右入力
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_UP_LEFT		 = 0x03;		//上入力と左入力
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_DOWN	 = 0x0C;		//右入力と下入力
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_UP_RIGHT_DOWN = 0x0E;		//上入力、右入力と下入力
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_UP_LEFT_DOWN  = 0x0B;		//上入力と左、下入力
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_UP_RIGHT_LEFT = 0x07;		//上入力、右入力と左入力
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_ALL_MOVE_KEY	 = 0x0F;		//移動キー全て
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_HALF_RIGHT_ROTATE = 0x10;	//上から時計回り180°
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_HALF_LEFT_ROTATE	 = 0x20;	//上から反時計回り180°
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_ROTATE	 = 0x40;		//上から時計回りの入力
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_LEFT_ROTATE	 = 0x80;		//上から反時計回りの入力

//アクション派生用フラグの種類
static const BYTE	OFFROADACTION_COMMAND_BIT_KIND_OF_TYPE		 = 8;
//アクション実行用フラグ
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_HALF_ROTATE_ACTION	 = 0x16;		//上から時計回り180度回転のアクションフラグ
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_LEFT_HALF_ROTATE_ACTION	 = 0x23;		//上から反時計回り180度回転のアクションフラグ
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_ROTATE_ACTION		 = 0x1F;		//上から時計回りの入力時のアクションフラグ
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_LEFT_ROTATE_ACTION		 = 0x2F;		//上から反時計回りの入力時のアクションフラグ
static const BYTE	OFFROADACTION_COMMAND_KIND_OF_ACTION					 = 4;			//アクションの数

//各コマンド成功時の得点
static const int	OFFROADACTION_COMMAND_TYPE_RIGHT_HALF_ROTATE_ACTION_POINT   = 100;		//上から時計回り180度回転のアクションフラグ
static const int	OFFROADACTION_COMMAND_TYPE_LEFT_HALF_ROTATE_ACTION_POINT    = 100;		//上から反時計回り180度回転のアクションフラグ
static const int	OFFROADACTION_COMMAND_TYPE_RIGHT_ROTATE_ACTION_POINT	    = 200;		//上から時計回りの入力時のアクションフラグ
static const int	OFFROADACTION_COMMAND_TYPE_LEFT_ROTATE_ACTION_POINT			= 200;		//上から反時計回りの入力時のアクションフラグ
//once success point
static const int	OFFROADACTION_SUCCESS_ACTION_BONUS_POINT = 10;

//コマンド入力の成否
typedef unsigned char CORRECTINPUTTYPE;

//▼オフロードプレイヤークラスの定義
class COffRoadAction{

private:

	//画像の要素を扱う
	int		*m_lpCOffRoadActionImg;
	//効果音の要素を扱う
	int		*m_lpCOffRoadActionSe;

	//画像設定用
	CHARACTER		*m_lpOffRoadActionFram;		//アクションコマンドのフレーム用
	CHARACTER		*m_lpOffRoadActionCommand;	//アクションコマンド用

	//アクションコマンド表示間隔
	int		m_actionInterval;

	//扱うアクションコマンドの最大数
	int		m_commandMax;

	//扱うアクションコマンドカウント
	int		m_commandCnt;

	//現在表示中のアクションコマンドの種類番号を扱う
	tagCOMMANDACTIONTYPE	m_CommandTypeNow;

	//扱うアクションタイプの配列を扱う
	tagCOMMANDACTIONTYPE	*m_lpCommandType;

	//アクションコマンドフレームの表示座標取得用
	static TPOINT<float>	m_indicateFramPos;

	//アクションコマンドのサイズ取得用
	static TPOINT<int>		m_FramSize;

	//アクション生成中フラグ
	static bool m_CommandCreateFlag;

	//現在表示中のアクションコマンドの入力の成否
	CORRECTINPUTTYPE	*m_lpCorrectInput;

	//現在表示したコマンドの要素
	int		m_CommandMaterialNow;

protected:



public:

	//▼ 静的メンバ変数の定義
	static const int OFFROADACTION_IMAGE_MAX;				//画像使用枚数
	static const int OFFROADACTION_EFFECT_SOUND_MAX;		//効果音使用数
	

	//ウィンドウハンドル
	static HWND	m_hWnd;

	//①アクションコマンドのフレーム
	//各種初期値
	//表示座標（ローカル座標）
	static const float OFFROADACTION_FRAM_DEFAULT_POSITION_X;			//表示座標X
	static const float OFFROADACTION_FRAM_DEFAULT_POSITION_Y;			//表示座標Y
	//画像サイズ
	static const int OFFROADACTION_FRAM_IMAGE_WIDTH;		//幅
	static const int OFFROADACTION_FRAM_IMAGE_HEIGHT;		//高さ
	//拡縮倍率
	static const float OFFROADACTION_FRAM_DEFAULT_SCALE;

	//②アクションコマンド画像
	//各種初期値
	//表示座標（ローカル座標）
	static const float OFFROADACTION_COMMAND_DEFAULT_POSITION_X;			//表示座標X
	static const float OFFROADACTION_COMMAND_DEFAULT_POSITION_Y;			//表示座標Y
	//画像サイズ
	static const int OFFROADACTION_COMMAND_IMAGE_WIDTH;			//幅
	static const int OFFROADACTION_COMMAND_IMAGE_HEIGHT;		//高さ
	//拡縮倍率
	static const float OFFROADACTION_COMMAND_DEFAULT_SCALE;

	//成否に応じた画像の種類
	static const CORRECTINPUTTYPE OFFROADACTION_COMMAND_IMAGE_VARIATION_NORMAL;
	static const CORRECTINPUTTYPE OFFROADACTION_COMMAND_IMAGE_VARIATION_EROR;
	static const CORRECTINPUTTYPE OFFROADACTION_COMMAND_IMAGE_VARIATION_SUCCESS;

	//成否に応じた種類数
	static const int OFFROADACTION_COMMAND_IMAGE_VARIATION_MAX;

	//アクションコマンド表示間隔
	static const int OFFROADACTION_COMMAND_INTERVAL_TIME_EASY_MODE;		//Easy時
	static const int OFFROADACTION_COMMAND_INTERVAL_TIME_NORMAL_MODE;	//Normal時
	static const int OFFROADACTION_COMMAND_INTERVAL_TIME_HARD_MODE;		//Hard時

	//アクションコマンド表示数
	static const int OFFROADACTION_COMMAND_CREATION_MAX_EASY_MODE;		//Easy時
	static const int OFFROADACTION_COMMAND_CREATION_MAX_NORMAL_MODE;	//Normal時
	static const int OFFROADACTION_COMMAND_CREATION_MAX_HARD_MODE;		//Hard時

	//コンストラクタ
	COffRoadAction();
	//ディストラクタ
	~COffRoadAction();
	
	//▼ 自作メンバ関数の定義
	//アクションの初期化
	void InitOffRoadAction();
	//アクションの更新
	bool UpdateOffRoadAction();
	//アクションの表示
	void DrawOffRoadAction();
	//解放
	void ReleaseOffRoadAction();
	/*
	水平回転アクションコマンドの初期化
	*/
	void InitLevelRotateActionCommand();

	/*
	アクションコマンドの生成
	戻り値 ＝ コマンド生成中ならtrue
	*/
	bool CreateActionCommand( bool actionFlag );
		
	/*
	************************************************************************************************
	アクションコマンドの入力状況を扱う
	戻り値
	成功 ： 入力したキーコードに対応したビット目
	失敗 ： -1
	************************************************************************************************
	*/
	BYTE InputActionCommand();


	//各種取得用の設定
	//表示座標
	void	SetFramIndicatePosition( TPOINT<float> pos ){ this->m_indicateFramPos = pos; }
	//サイズ
	void	SetFramSize( TPOINT<int> size ){ this->m_FramSize = size; }

	//取得用
	//座標
	TPOINT<float>	GetFramIndicatePosition() const { return this->m_indicateFramPos ; } 
	//サイズ
	TPOINT<int>		GetFramSize() const { return this->m_FramSize; } 
	//コマンド生成中を取得
	bool	GetCommandActionCretaeFlag() const { return this->m_CommandCreateFlag; }

};


#endif
