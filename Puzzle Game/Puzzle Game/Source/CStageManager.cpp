#include	<iostream>
#include	"CStageManager.h"
#include	"MemoryLeakCheck.h"
#include	"graphics.h"
#include	"input.h"

using namespace std;

//�� �ÓI�����o�ϐ��̍Ē�`
//�E�B���h�E�n���h��
HWND CStageManager::m_hWnd = NULL;
//�摜�̎g�p����
const int	CStageManager::CSTAGEMANAGER_IMAGE_MAX = 2;

//�摜�T�C�Y
//�@�w�i
const int	CStageManager::CSTAGEMANAGER_BACK_IMAGE_WIDTH = 500;		//��
const int	CStageManager::CSTAGEMANAGER_BACK_IMAGE_HEIGHT = 300;		//����

/*
************************************************************************************
�X�e�[�W�̃R���X�g���N�^
************************************************************************************
*/
CStageManager::CStageManager() {}


/*
************************************************************************************
�X�e�[�W�̃f�B�X�g���N�^
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
�X�e�[�W�̏�����
************************************************************************************
*/
void CStageManager::InitStageManager() {


	//�� �����摜�̃f�[�^�e�[�u��
	char	*lpBackImage[CStageManager::CSTAGEMANAGER_IMAGE_MAX] = {

		"Image\\Bg\\StageBack.bmp" ,
		"Image\\Bg\\Stage1.bmp" ,
	};

	//�� �N���X�̊m��
	this->m_lpCStage = new CStage[CStageManager::CSTAGEMANAGER_IMAGE_MAX];
	//�摜�̗v�f���m��
	this->m_lpStageImg = new int[CStageManager::CSTAGEMANAGER_IMAGE_MAX];

	//�� �摜�̓ǂݍ���
	for (int i = 0; i < CStageManager::CSTAGEMANAGER_IMAGE_MAX; i++) {
		//�w�i�摜
		this->m_lpStageImg[i] = CreateDDImage(lpBackImage[i], CStageManager::m_hWnd);
		//�摜��ݒ�
		this->m_lpCStage[i].SetCharaImg(this->m_lpStageImg[i]);
	}

	//�� �\���ݒ�
	//�@�w�i
	this->m_lpCStage[1].m_Isize.x = CStageManager::CSTAGEMANAGER_BACK_IMAGE_WIDTH;		//��
	this->m_lpCStage[1].m_Isize.y = CStageManager::CSTAGEMANAGER_BACK_IMAGE_HEIGHT;		//����
	//�\����
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
�X�e�[�W�̍X�V
************************************************************************************
*/
void CStageManager::UpdateStageManager() {}



/*
************************************************************************************
�X�e�[�W�̕\��
************************************************************************************
*/
void CStageManager::DrawStageManager() {

	//�ǂݍ��񂾃X�e�[�W�̕\��
	for (int i = 0; i < CStageManager::CSTAGEMANAGER_IMAGE_MAX; i++) {
		this->m_lpCStage[i].DrawChara(DRAWING_TYPE_DDBLT, DDSCALE_STARTING_POINT_NOTHING);
	}
}

/*
************************************************************************************
�X�e�[�W�̉��
************************************************************************************
*/
void CStageManager::ReleaseStageManager() {
	for (int i = 0; i < CStageManager::CSTAGEMANAGER_IMAGE_MAX; i++) {
		ReleaseDDImage(this->m_lpStageImg[i]);
	}
}
