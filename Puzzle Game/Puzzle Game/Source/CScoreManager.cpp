#include	"CScoreManager.h"
#include	"CPieceManager.h"
#include	"CScore.h"
#include	"graphics.h"
#include	"input.h"
#include	<iostream>
#include	<stdio.h>

//名前空間
using namespace	std;

//▼ 静的メンバ変数の定義
const int CScoreManager::SCOREMANAGER_SCORE_IMAGE_MAX = 1;		//画像使用枚数
//ウィンドウハンドル
HWND	CScoreManager::m_hWnd = NULL;
//for get present drawing number score
int		*CScoreManager::m_lpPresentNumber = NULL;


/*
************************************************************************************************
ディストラクタ
************************************************************************************************
*/
CScoreManager::~CScoreManager() {
	delete []this->m_lpPresentNumber;
	this->m_lpPresentNumber = NULL;
	delete []this->m_lpCScoreNumber;
	this->m_lpCScoreNumber = NULL;
	delete []this->m_lpScoreManagerImg;
	this->m_lpScoreManagerImg = NULL;
	delete this->m_lpCPieceManager;
	this->m_lpCPieceManager = NULL;
}

//************************************************************************************************
//スコアの初期化
//************************************************************************************************
void CScoreManager::InitScoreManager() {

	//using image data
	char	*lpImage[CScoreManager::SCOREMANAGER_SCORE_IMAGE_MAX] = {
		{ "Image\\Ch\\Number.bmp" } ,						//number's image
	};
	//allot memory
	this->m_lpCScoreNumber = new CScore[SCOREMANAGER_KIND_OF_SCORENUMBER];				//for CScore
	this->m_lpScoreManagerImg = new int[CScoreManager::SCOREMANAGER_SCORE_IMAGE_MAX];	//for image
	this->m_lpPresentNumber = new int[SCOREMANAGER_KIND_OF_SCORENUMBER];				//for get number
	this->m_lpCPieceManager = new CPieceManager;										//for get flag of complete
	//create material of image
	for (int i = 0; i < CScoreManager::SCOREMANAGER_SCORE_IMAGE_MAX; i++) { this->m_lpScoreManagerImg[i] = CreateDDImage(lpImage[i], CScoreManager::m_hWnd); }
	//set number's image
	this->m_lpCScoreNumber[SCOREMANAGER_SCORENUMBER_COUNT_DOWN_TIME].SetCharaImg(this->m_lpScoreManagerImg[0]);
	
	//initialize number's image
	this->m_lpCScoreNumber[SCOREMANAGER_SCORENUMBER_COUNT_DOWN_TIME].InitChara();

	this->m_lpCScoreNumber[SCOREMANAGER_SCORENUMBER_COUNT_DOWN_TIME].m_existFlag = true;
	this->m_lpCScoreNumber[SCOREMANAGER_SCORENUMBER_COUNT_DOWN_TIME].m_indicateNum = 60;
	this->m_lpCScoreNumber[SCOREMANAGER_SCORENUMBER_COUNT_DOWN_TIME].m_digit = 3;
	this->m_lpCScoreNumber[SCOREMANAGER_SCORENUMBER_COUNT_DOWN_TIME].m_scoreColor = SCORE_COLOR_LIGHT_BLUE;
	this->m_lpCScoreNumber[SCOREMANAGER_SCORENUMBER_COUNT_DOWN_TIME].m_scale = 0.75f;
	this->m_lpCScoreNumber[SCOREMANAGER_SCORENUMBER_COUNT_DOWN_TIME].m_Fpos = { 200.0f, 20.0f };
}

//************************************************************************************************
//スコアの更新
//************************************************************************************************
void CScoreManager::UpdateScoreManager() {
	
	if (this->m_lpCPieceManager->GetCompletePuzzle() == false) {
		//count milliseond for draw second until complete
		this->m_millisecond++;
		if (this->m_millisecond > 99) {
			//reset count
			this->m_millisecond = 0;
			//subtract one second from indicated second
			this->m_lpCScoreNumber[SCOREMANAGER_SCORENUMBER_COUNT_DOWN_TIME].m_indicateNum--;
		}
		//set drawing preset number score for ger present number
		this->m_lpPresentNumber[SCOREMANAGER_SCORENUMBER_COUNT_DOWN_TIME] = this->m_lpCScoreNumber[SCOREMANAGER_SCORENUMBER_COUNT_DOWN_TIME].m_indicateNum;
	}
}

//************************************************************************************************
//スコアの表示
//************************************************************************************************
void CScoreManager::DrawScoreManager() {

	//draw number's image
	if (this->m_lpCScoreNumber[SCOREMANAGER_SCORENUMBER_COUNT_DOWN_TIME].m_existFlag) {
		this->m_lpCScoreNumber[SCOREMANAGER_SCORENUMBER_COUNT_DOWN_TIME].DrawNumber(
			this->m_lpCScoreNumber[SCOREMANAGER_SCORENUMBER_COUNT_DOWN_TIME].m_Fpos,
			this->m_lpCScoreNumber[SCOREMANAGER_SCORENUMBER_COUNT_DOWN_TIME].m_indicateNum,
			this->m_lpCScoreNumber[SCOREMANAGER_SCORENUMBER_COUNT_DOWN_TIME].m_digit,
			this->m_lpCScoreNumber[SCOREMANAGER_SCORENUMBER_COUNT_DOWN_TIME].m_scoreColor, 
			this->m_lpCScoreNumber[SCOREMANAGER_SCORENUMBER_COUNT_DOWN_TIME].m_scale);
	}
}

//***************************************************************************************************
//解放
//***************************************************************************************************
void CScoreManager::ReleaseScoreManager() {
	for (int i = 0; i < CScoreManager::SCOREMANAGER_SCORE_IMAGE_MAX; i++) {
		ReleaseDDImage(this->m_lpScoreManagerImg[i]);
	}
}
