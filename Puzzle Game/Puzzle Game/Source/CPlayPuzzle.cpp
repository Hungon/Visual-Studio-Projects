#include	<iostream>
#include	"CPlayPuzzle.h"
#include	"CSelectButton.h"
#include	"MemoryLeakCheck.h"
#include	"input.h"
#include	"CWipeManager.h"

using namespace std;

//define static members
HWND	CPlayPuzzle::m_hWnd = NULL;
//using kind of image
const int	CPlayPuzzle::PLAY_KIND_OF_IMAGE = 1;
//default setting
const TPOINT<int>	CPlayPuzzle::PLAY_IMAGE_BUTTON_NEXT_SIZE = { 112, 48 };
const TPOINT<float>	CPlayPuzzle::PLAY_IMAGE_BUTTON_NEXT_COORDINATE = { 0.0f, 350.0f };


CPlayPuzzle::~CPlayPuzzle() {
	delete[]this->m_lpImage;			//for image
	this->m_lpImage = NULL;
	delete[]this->m_lpCButton;			//for button
	this->m_lpCButton = NULL;
}

/*
************************************************************************************
プレイの初期化
************************************************************************************
*/
SCENE_NUM CPlayPuzzle::Init() {

	//using image
	char	*lpImage[CPlayPuzzle::PLAY_KIND_OF_IMAGE] = {
		"Image\\Bg\\PlayButton.bmp",
	};
	//allot memory
	this->m_lpImage = new int[CPlayPuzzle::PLAY_KIND_OF_IMAGE];
	this->m_lpCButton = new CSelectButton[PLAYBUTTON_KIND_OF_BUTTON];			//for button

	//load image's material
	for (int i = 0; i < CPlayPuzzle::PLAY_KIND_OF_IMAGE; i++) { this->m_lpImage[i] = CreateDDImage(lpImage[i], CPlayPuzzle::m_hWnd); }

	//initialize image's indication for button in play
	TPOINT<float> buttonPos[PLAYBUTTON_KIND_OF_BUTTON] = {
		{ CHARACTER_CENTER_POSITION_X(CPlayPuzzle::PLAY_IMAGE_BUTTON_NEXT_SIZE.x),
		CPlayPuzzle::PLAY_IMAGE_BUTTON_NEXT_COORDINATE.y },									//for next of button
	};
	this->m_lpCButton[PLAYBUTTON_NEXT].InitChara(buttonPos[0], CPlayPuzzle::PLAY_IMAGE_BUTTON_NEXT_SIZE);
	this->m_lpCButton[PLAYBUTTON_NEXT].SetCharaImg(this->m_lpImage[0]);

	//ステージ
	CStageManager::InitStageManager();
	//ピース
	CPieceManager::InitPieceManager();
	//Score
	CScoreManager::InitScoreManager();
	//risdon
	CRisdonManager::InitManagerCharacter();

	//初期化に失敗したらSCENE_ERRORを返す
	return SCENE_UPDATE;

}


/*
************************************************************************************
プレイの更新
************************************************************************************
*/
SCENE_NUM CPlayPuzzle::Update() {
	
	//make little blank time
	if (this->m_startintPlayCount < 20) {
		this->m_startintPlayCount++;
	}
	else {
		this->m_NextFlag = this->CPieceManager::UpdatePieceManager();			//check flag of complete
	}
	
	//when player completed stage, draw the image of next
	if (this->m_NextFlag == false) {
		//ステージ
		CStageManager::UpdateStageManager();
		//Score
		CScoreManager::UpdateScoreManager();
		//if current count time limit 0, transition to game over
		if (CScoreManager::GetScoreNumber(SCOREMANAGER_SCORENUMBER_COUNT_DOWN_TIME) <= 0) {
			CWipeManager::CreateWipeManager(GAMESCENE_GAMEOVER, WIPEMANAGER_FEEDOUT);
			return SCENE_RELEASE;
		}
	}
	else {
		this->m_lpCButton[PLAYBUTTON_NEXT].m_existFlag = true;					//draw next image
		//when drawing button, check mouse's coordinate with button's coordinate
		if (this->m_lpCButton[PLAYBUTTON_NEXT].CheckMouseAction()) {
			CWipeManager::CreateWipeManager(GAMESCENE_SELECT_MODE, WIPEMANAGER_FEEDOUT);
			return SCENE_RELEASE;
		}
	}

	CRisdonManager::UpdateManagerCharacter();			//risdon

	return SCENE_UPDATE;
}



/*
************************************************************************************
プレイの表示
************************************************************************************
*/
void CPlayPuzzle::Draw(){

	//ステージ
	CStageManager::DrawStageManager();
	//ピース
	CPieceManager::DrawPieceManager();
	//Score
	CScoreManager::DrawScoreManager();
	//button
	m_lpCButton[PLAYBUTTON_NEXT].DrawChara(DRAWING_TYPE_DDBLT, DDSCALE_STARTING_POINT_NOTHING);
	//risdon
	CRisdonManager::DrawManagerCharacter();
}

/*
************************************************************************************
プレイの解放
************************************************************************************
*/
SCENE_NUM CPlayPuzzle::Release() {

	//ステージ
	CStageManager::ReleaseStageManager();
	//ピース
	CPieceManager::ReleasePieceManager();
	//Score
	CScoreManager::ReleaseScoreManager();
	//button
	this->m_lpCButton[PLAYBUTTON_NEXT].ReleaseChara();
	//risdon
	CRisdonManager::ReleaseManagerCharacter();
	//each image
	for (int i = 0; i < CPlayPuzzle::PLAY_KIND_OF_IMAGE; i++) { ReleaseDDImage(this->m_lpImage[i]); }

	//基本的にSCENE_ENDを返す
	return SCENE_END;

}
