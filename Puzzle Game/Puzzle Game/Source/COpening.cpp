#include	<iostream>
#include	"COpening.h"
#include	"COpTitleLogo.h"
#include	"COpButton.h"
#include	"graphics.h"
#include	"input.h"
#include	"CWipeManager.h"
#include	"CTalkManager.h"

using namespace std;


//▼ 静的メンバ変数の再定義
//ウィンドウハンドル
HWND COpening::m_hWnd = NULL;
//画像の使用枚数
const char COpening::COPENING_IMAGE_MAX = 3;

/*
************************************************************************************
オープニングのコンストラクタ
************************************************************************************
*/
COpening::COpening() {}


/*
************************************************************************************
オープニングのディストラクタ
************************************************************************************
*/
COpening::~COpening() {
	delete[]this->m_lpImage;		//for material of image
	this->m_lpImage = NULL;
	delete this->m_lpCTitleLogo;	//for title logo
	this->m_lpCTitleLogo = NULL;
	delete this->m_lpCButton;		//for button
	this->m_lpCButton = NULL;
	delete this->m_lpCTalkManager;
	this->m_lpCTalkManager = NULL;
}



/*
************************************************************************************
オープニングの初期化
************************************************************************************
*/
SCENE_NUM COpening::Init() {

	//using image's pass
	char	*lpImage[COpening::COPENING_IMAGE_MAX] = {
		"Image\\Bg\\OpeningBack.bmp",			//back ground image
		"Image\\Bg\\TitleLogo.bmp",				//title logo
		"Image\\Bg\\TitleButton.bmp",			//title's button
	};

	//allot memory
	this->m_lpImage = new int[COpening::COPENING_IMAGE_MAX];
	this->m_lpCTitleLogo = new COpTitleLogo;					//for title logo
	this->m_lpCButton = new COpButton;							//for button
	this->m_lpCTalkManager = new CTalkManager;

	//load images
	for (int i = 0; i < COpening::COPENING_IMAGE_MAX; i++) {
		this->m_lpImage[i] = CreateDDImage(lpImage[i], COpening::m_hWnd);
	}
	//set material of image
	this->m_lpCTitleLogo->SetCharaImg(this->m_lpImage[1]);			//title logo
	this->m_lpCButton->SetCharaImg(this->m_lpImage[2]);				//button

	//intialize
	this->m_lpCTitleLogo->InitChara();						//for title logo
	this->m_lpCButton->InitChara(OPENINGBUTTON_START);		//for button

	this->m_lpCTalkManager[0].InitializeTalkManager();

	//初期化に失敗したらSCENE_ERRORを返す
	return SCENE_UPDATE;
}


/*
************************************************************************************
オープニングの更新
************************************************************************************
*/
SCENE_NUM COpening::Update(){
		
	//update title logo
	this->m_lpCTitleLogo->UpdateChara();
	//diverge scene from pressed button
	GAMESCENE_NUMBER gameScene = this->m_lpCButton->TransitionScene();
	if (gameScene != GAMESCENE_OPENING) {
		CWipeManager::CreateWipeManager(gameScene, WIPEMANAGER_FEEDOUT);		//next scene
		return SCENE_RELEASE;
	}
	
	this->m_lpCTalkManager[0].UpdateTalkManager();

	return SCENE_UPDATE;
}


/*
************************************************************************************
オープニングの解放
************************************************************************************
*/
SCENE_NUM COpening::Release(){

	//release
	this->m_lpCTitleLogo->ReleaseChara();			//title logo
	this->m_lpCButton->ReleaseChara();				//button
	//画像の解放
	for( int i = 0 ; i < COpening::COPENING_IMAGE_MAX ; i++ ){
		ReleaseDDImage( this->m_lpImage[i] );
	}
	//基本的にSCENE_ENDを返す
	return SCENE_END;
}


/*
************************************************************************************
オープニングの表示
************************************************************************************
*/
void COpening::Draw(){

	//オープニング画像の表示
	DDBlt(					
		0	,		//表示座標X
		0	,		//表示座標Ｙ
		640		,		//幅
		480		,		//高さ
		this->m_lpImage[0]		,		//画像の要素番号
		0	,		//表示元X
		0 );			//表示元座標Y
	
	//draw
	this->m_lpCTitleLogo->DrawChara();			//title logo
	this->m_lpCButton->DrawChara();				//button
	
	this->m_lpCTalkManager[0].DrawTalkManager();
}


