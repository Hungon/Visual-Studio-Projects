#include	<iostream>
#include	"CStageManager.h"
#include	"MemoryLeakCheck.h"
#include	"graphics.h"
#include	"input.h"

using namespace std;

//▼ 静的メンバ変数の再定義
//ウィンドウハンドル
HWND CStageManager::m_hWnd = NULL;
//画像の使用枚数
const int	CStageManager::CSTAGEMANAGER_IMAGE_MAX = 2;

//画像サイズ
//①背景
const int	CStageManager::CSTAGEMANAGER_BACK_IMAGE_WIDTH = 500;		//幅
const int	CStageManager::CSTAGEMANAGER_BACK_IMAGE_HEIGHT = 300;		//高さ

/*
************************************************************************************
ステージのコンストラクタ
************************************************************************************
*/
CStageManager::CStageManager() {}


/*
************************************************************************************
ステージのディストラクタ
************************************************************************************
*/
CStageManager::~CStageManager() {
	delete[]this->m_lpCStage;
	this->m_lpCStage = NULL;
	delete[]this->m_lpStageImg;
	this->m_lpStageImg = NULL;
}




/*
************************************************************************************
ステージの初期化
************************************************************************************
*/
void CStageManager::InitStageManager() {


	//▼ 扱う画像のデータテーブル
	char	*lpBackImage[CStageManager::CSTAGEMANAGER_IMAGE_MAX] = {

		"Image\\Bg\\StageBack.bmp" ,
		"Image\\Bg\\Stage1.bmp" ,
	};

	//▼ クラスの確保
	this->m_lpCStage = new CStage[CStageManager::CSTAGEMANAGER_IMAGE_MAX];
	//画像の要素を確保
	this->m_lpStageImg = new int[CStageManager::CSTAGEMANAGER_IMAGE_MAX];

	//▼ 画像の読み込み
	for (int i = 0; i < CStageManager::CSTAGEMANAGER_IMAGE_MAX; i++) {
		//背景画像
		this->m_lpStageImg[i] = CreateDDImage(lpBackImage[i], CStageManager::m_hWnd);
		//画像を設定
		this->m_lpCStage[i].SetCharaImg(this->m_lpStageImg[i]);
	}

	//▼ 表示設定
	//①背景
	this->m_lpCStage[1].m_Isize.x = CStageManager::CSTAGEMANAGER_BACK_IMAGE_WIDTH;		//幅
	this->m_lpCStage[1].m_Isize.y = CStageManager::CSTAGEMANAGER_BACK_IMAGE_HEIGHT;		//高さ
	//表示先
	this->m_lpCStage[1].m_Fpos.x = (640 - this->m_lpCStage[1].m_Isize.x) / 2;			//X
	this->m_lpCStage[1].m_Fpos.y = (480 - this->m_lpCStage[1].m_Isize.y) / 2;			//Y
	this->m_lpCStage[1].m_existFlag = true;

	//back ground
	this->m_lpCStage[0].m_Isize.x = SCREEN_WIDTH;
	this->m_lpCStage[0].m_Isize.y = SCREEN_HEIGHT;
	this->m_lpCStage[0].m_existFlag = true;
}


/*
************************************************************************************
ステージの更新
************************************************************************************
*/
void CStageManager::UpdateStageManager() {}



/*
************************************************************************************
ステージの表示
************************************************************************************
*/
void CStageManager::DrawStageManager() {

	//読み込んだステージの表示
	for (int i = 0; i < CStageManager::CSTAGEMANAGER_IMAGE_MAX; i++) {
		this->m_lpCStage[i].DrawChara(DRAWING_TYPE_DDBLT, DDSCALE_STARTING_POINT_NOTHING);
	}
}

/*
************************************************************************************
ステージの解放
************************************************************************************
*/
void CStageManager::ReleaseStageManager() {
	for (int i = 0; i < CStageManager::CSTAGEMANAGER_IMAGE_MAX; i++) {
		ReleaseDDImage(this->m_lpStageImg[i]);
	}
}
