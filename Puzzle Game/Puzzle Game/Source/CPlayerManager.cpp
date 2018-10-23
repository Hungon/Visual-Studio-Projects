#include	"CPlayerManager.h"
#include	"CPlayer.h"
#include	"CPieceManager.h"
#include	"graphics.h"
#include	"input.h"
#include	"MemoryLeakCheck.h"
#include	<iostream>
#include	<math.h>

//���O���
using namespace	std;


//�� �ÓI�����o�ϐ��̍Ē�`
//�E�B���h�E�n���h��
HWND CPlayerManager::m_hWnd = NULL;
//�摜�̎g�p����
const int CPlayerManager::CPLAYERMANAGER_IMAGE_MAX = 1;
//�v���C���[�̍��W������
TPOINT<float>	CPlayerManager::m_ArrowPosition = {0};
//�v���C���[�̃T�C�Y
const TPOINT<int> CPlayerManager::CPLAYERMANAGER_ARROW_SIZE = {46 , 46};
//�v���C���[�̊g�k�{��
const float	CPlayerManager::CPLAYERMANAGER_DEFAULT_SCALE = 1.0f;



/*
************************************************************************************
�v���C���[�Ǘ��̃f�B�X�g���N�^
************************************************************************************
*/
CPlayerManager::~CPlayerManager(){

	//����S��
	this->ReleaseChara();
}

/*
************************************************************************************
�v���C���[�Ǘ��̏�����
************************************************************************************
*/
void CPlayerManager::InitChara(){

	
	//�� �N���X�̃������m��
	this->m_lpCPieceManager = new CPieceManager;		//�s�[�X�Ǘ��p
	this->m_lpCPlayer = new CPlayer;					//�v���C���[

	//�摜�̗v�f���m��
	this->m_lpPlayerImg = new int[ CPlayerManager::CPLAYERMANAGER_IMAGE_MAX ];


	//�� �e��\���ݒ�
	//���񂾃s�[�X�̈�ԍ��[�̃s�[�X�̍��W���擾
	TPOINT<float> pos = this->m_lpCPieceManager->GetPiecePosition();

	//�s�[�X�̃T�C�Y���擾
	TPOINT<int>	size = this->m_lpCPieceManager->GetPieceSize();

	//�e��l�̐ݒ�
	this->m_lpCPlayer[0].m_Fpos = pos;							//�\�����W
	this->m_lpCPlayer[0].m_Fmove.x = (float)size.x;				//�ړ���X
	this->m_lpCPlayer[0].m_Fmove.y = (float)size.y;				//�ړ���Y
	this->m_lpCPlayer[0].m_Isize = CPlayerManager::CPLAYERMANAGER_ARROW_SIZE;//�T�C�Y
	this->m_lpCPlayer[0].m_existFlag = true;					//�����t���O

	//�v���C���[�̑傫�����s�[�X�̃T�C�Y�ɍ��킹��
	float sizefit = (int)( sqrtf( ( this->m_lpCPlayer[0].m_Isize.x * this->m_lpCPlayer[0].m_Isize.x ) +
		( this->m_lpCPlayer[0].m_Isize.y * this->m_lpCPlayer[0].m_Isize.y ) ) ) / 
		(int)( sqrtf( ( size.x * size.x ) ) + ( size.y * size.y ) );
	
	sizefit += (int)( sqrtf( ( this->m_lpCPlayer[0].m_Isize.x * this->m_lpCPlayer[0].m_Isize.x ) +
		( this->m_lpCPlayer[0].m_Isize.y * this->m_lpCPlayer[0].m_Isize.y ) ) ) % 
		(int)( sqrtf( ( size.x * size.x ) ) + ( size.y * size.y ) );
	
	//���܂����{������
	this->m_lpCPlayer[0].m_scale = sizefit * 0.01;

	//�� �摜�̓ǂݍ���
	//�w�i�摜
	this->m_lpPlayerImg[0] = CreateDDImage( "Image\\Ch\\Arrow.bmp" , CPlayerManager::m_hWnd );

	//�摜�̗v�f�ԍ���n��
	this->m_lpCPlayer[0].SetCharaImg( this->m_lpPlayerImg[0] );

}

/*
************************************************************************************
�v���C���[�Ǘ��̍X�V
************************************************************************************
*/
void CPlayerManager::UpdateChara(){

	//���̍X�V
	this->m_lpCPlayer[0].UpdateChara();

	//���̈ړ��̍X�V
	this->MoveArrow();

	//���̈ړ�����
	this->ConstrainArrowMove();

	//���̍��W�X�V
	this->m_ArrowPosition = this->m_lpCPlayer[0].m_Fpos;
	
}



/*
************************************************************************************
�v���C���[�Ǘ��̕\��
************************************************************************************
*/
void CPlayerManager::DrawChara(){


	//���̕\��
	this->m_lpCPlayer[0].DrawChara();

}

/*
************************************************************************************
����S��
************************************************************************************
*/
void CPlayerManager::ReleaseChara(){

	//�摜�̉��
	this->ReleaseImagePlayerManager();
	//�N���X�̉��
	this->ReleaseClassPlayerManager();

}

/*
************************************************************************************
�摜�̉��
************************************************************************************
*/
void CPlayerManager::ReleaseImagePlayerManager(){


	//����������Ă��Ȃ�������������
	if( m_lpPlayerImg != NULL ){
		//�摜�̉��
		for( int i = 0 ; i < CPlayerManager::CPLAYERMANAGER_IMAGE_MAX ; i++ ){
			ReleaseDDImage( this->m_lpPlayerImg[i] );
		}
	}

	//�摜�̗v�f
	if( this->m_lpPlayerImg != NULL){
		delete []this->m_lpPlayerImg;
		this->m_lpPlayerImg = NULL;
	}

}

/*
************************************************************************************
�N���X�̉��
************************************************************************************
*/
void CPlayerManager::ReleaseClassPlayerManager(){

	delete	this->m_lpCPieceManager;
	this->m_lpCPieceManager = NULL;		//�s�[�X�Ǘ��N���X
	
	delete	this->m_lpCPlayer;			//�v���C���[�N���X
	this->m_lpCPlayer = NULL;

}

/*
************************************************************************************
���̑���
************************************************************************************
*/
void CPlayerManager::MoveArrow(){

	//�� ���̑���
	//�����
	if( JustKey( VK_UP ) ){ this->m_lpCPlayer[0].m_Fpos.y -= this->m_lpCPlayer[0].m_Fmove.y; }
	//�E����
	if( JustKey( VK_RIGHT ) ){ this->m_lpCPlayer[0].m_Fpos.x += this->m_lpCPlayer[0].m_Fmove.x; }
	//������
	if( JustKey( VK_DOWN ) ){ this->m_lpCPlayer[0].m_Fpos.y += this->m_lpCPlayer[0].m_Fmove.y; }
	//������
	if( JustKey( VK_LEFT ) ){ this->m_lpCPlayer[0].m_Fpos.x -= this->m_lpCPlayer[0].m_Fmove.x; }


}


/*
************************************************************************************
���̈ړ�����
************************************************************************************
*/
void CPlayerManager::ConstrainArrowMove(){

	//���񂾃s�[�X�̈�ԍ��[�̃s�[�X�̍��W���擾
	TPOINT<float> pos = this->m_lpCPieceManager->GetPiecePosition();

	//�s�[�X�̃T�C�Y���擾
	TPOINT<int>	size = this->m_lpCPieceManager->GetPieceSize();
	
	//�s�[�X�̑S�̃T�C�Y���擾
	TPOINT<int>	wholeSize = this->m_lpCPieceManager->GetWholePieceSize();

	//�� �ړ�����
	//�z�u�����`�b�v�X�e�[�W�̃T�C�Y������������
	//��
	if( this->m_lpCPlayer[0].m_Fpos.y < pos.y ){
		this->m_lpCPlayer[0].m_Fpos.y = ( pos.y + wholeSize.y ) - size.y;
	}
	//�E
	if( this->m_lpCPlayer[0].m_Fpos.x + ( this->m_lpCPlayer[0].m_Isize.x * this->m_lpCPlayer[0].m_scale ) > 
		pos.x + wholeSize.x ){
			this->m_lpCPlayer[0].m_Fpos.x = pos.x;
	}
	//��
	if( this->m_lpCPlayer[0].m_Fpos.y + ( this->m_lpCPlayer[0].m_Isize.y * this->m_lpCPlayer[0].m_scale ) > 
		pos.y + wholeSize.y ){
			this->m_lpCPlayer[0].m_Fpos.y = pos.y;
	}
	//��
	if( this->m_lpCPlayer[0].m_Fpos.x < pos.x ){
		this->m_lpCPlayer[0].m_Fpos.x = ( pos.x + wholeSize.x ) - size.x;
	}

}

