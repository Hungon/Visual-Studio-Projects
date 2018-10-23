#include	<iostream>
#include	"COpening.h"
#include	"COpTitleLogo.h"
#include	"COpButton.h"
#include	"graphics.h"
#include	"input.h"
#include	"CWipeManager.h"
#include	"CTalkManager.h"

using namespace std;


//�� �ÓI�����o�ϐ��̍Ē�`
//�E�B���h�E�n���h��
HWND COpening::m_hWnd = NULL;
//�摜�̎g�p����
const char COpening::COPENING_IMAGE_MAX = 3;

/*
************************************************************************************
�I�[�v�j���O�̃R���X�g���N�^
************************************************************************************
*/
COpening::COpening() {}


/*
************************************************************************************
�I�[�v�j���O�̃f�B�X�g���N�^
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
�I�[�v�j���O�̏�����
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

	//�������Ɏ��s������SCENE_ERROR��Ԃ�
	return SCENE_UPDATE;
}


/*
************************************************************************************
�I�[�v�j���O�̍X�V
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
�I�[�v�j���O�̉��
************************************************************************************
*/
SCENE_NUM COpening::Release(){

	//release
	this->m_lpCTitleLogo->ReleaseChara();			//title logo
	this->m_lpCButton->ReleaseChara();				//button
	//�摜�̉��
	for( int i = 0 ; i < COpening::COPENING_IMAGE_MAX ; i++ ){
		ReleaseDDImage( this->m_lpImage[i] );
	}
	//��{�I��SCENE_END��Ԃ�
	return SCENE_END;
}


/*
************************************************************************************
�I�[�v�j���O�̕\��
************************************************************************************
*/
void COpening::Draw(){

	//�I�[�v�j���O�摜�̕\��
	DDBlt(					
		0	,		//�\�����WX
		0	,		//�\�����W�x
		640		,		//��
		480		,		//����
		this->m_lpImage[0]		,		//�摜�̗v�f�ԍ�
		0	,		//�\����X
		0 );			//�\�������WY
	
	//draw
	this->m_lpCTitleLogo->DrawChara();			//title logo
	this->m_lpCButton->DrawChara();				//button
	
	this->m_lpCTalkManager[0].DrawTalkManager();
}


