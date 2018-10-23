#include	"CTalkManager.h"
#include	"CTalk.h"

//define static members
//set default value
//set bloon for talking
const TPOINT<int> CTalkManager::TALKMANAGER_IMAGE_FRAM_DEAFULT_SIZE = { 256, 180 };
HWND	CTalkManager::m_hWnd = NULL;

const int		CTalkManager::TALKMANAGER_KIND_OF_IMAGE = 1;				//kind of image
const int		CTalkManager::TALKMANAGER_FONT_DEFAULT_SIZE = 15;
const int		CTalkManager::TALKMANAGER_FONT_DEFAULT_SPATIAL_TIME = 10;
const int		CTalkManager::TALKMANAGER_FONT_LINE_SPACE = 5;
const COLORREF	CTalkManager::TALKMANAGER_FONT_DEFAULT_COLOR = RGB(0, 0, 0);

 CTalkManager::CTalkManager() {}
 
 CTalkManager::~CTalkManager() {
	 delete this->m_lpImage;			//for image
	 this->m_lpImage = NULL;
	 delete this->m_lpCTalk;			//for talk
	 this->m_lpCTalk = NULL;
 }

 void	CTalkManager::InitializeTalkManager() {
 
	 //kind of image
	 char	*lpImage[CTalkManager::TALKMANAGER_KIND_OF_IMAGE] = {
		 "Image\\Bg\\Baloon.bmp",			//baloon of talking
	 };

	 //allot memory
	 this->m_lpCTalk = new CTalk;			//for talk
	 this->m_lpImage = new int[CTalkManager::TALKMANAGER_KIND_OF_IMAGE];

	 //load material of image
	 for (int i = 0; i < CTalkManager::TALKMANAGER_KIND_OF_IMAGE; i++) {
		 this->m_lpImage[i] = CreateDDImage(lpImage[i], CTalkManager::m_hWnd);	//load image
		 this->m_lpCTalk[i].SetCharaImg(this->m_lpImage[0]);					//set material of image
	 }
 
	 //initializer for talking process
	 this->m_lpCTalk[0].InitializeTalk(
		 CTalkManager::TALKMANAGER_FONT_DEFAULT_SIZE,
		 CTalkManager::TALKMANAGER_FONT_DEFAULT_SPATIAL_TIME,
		 CTalkManager::TALKMANAGER_FONT_LINE_SPACE,
		 CTalkManager::TALKMANAGER_FONT_DEFAULT_COLOR);

	 //intializer for baloon of talking
	 TPOINT<float> pos = { 150.0f,150.0f };
	 this->m_lpCTalk[0].InitChara(
		 pos, CTalkManager::TALKMANAGER_IMAGE_FRAM_DEAFULT_SIZE);
	 this->m_lpCTalk[0].m_existFlag = true;
 }

void	CTalkManager::UpdateTalkManager() {
	//each files to talk
	char	*lpFile[1] = {
		"Data\\Talk\\talk1.txt",
	};

	this->m_lpCTalk[0].UpdateTalk();		//update talking
	
	this->m_lpCTalk[0].CreateTalk(			//create talking
		lpFile[0], 400.0f, 300.0f);

	this->m_lpCTalk[0].UpdateChara();		//update baloon
}

void	CTalkManager::DrawTalkManager() {
	//draw baloon of talking
	this->m_lpCTalk[0].DrawChara(DRAWING_TYPE_DDBLT, DDSCALE_STARTING_POINT_NOTHING);
	this->m_lpCTalk[0].DrawTalk();			//draw talking
}

void	CTalkManager::ReleaseTalkManager() {
	for (int i = 0; i < CTalkManager::TALKMANAGER_KIND_OF_IMAGE; i++) {
		ReleaseDDImage(this->m_lpImage[i]);
	}
}