#include	"CRoadEnemyManager.h"
#include	"CRunnerManager.h"
#include	"CRoadEnemy.h"
#include	"sound.h"
#include	"play.h"

//�� �ÓI�����o�ϐ��̍Ē�`
const int	CRoadEnemyManager::ROADENEMYMANAGER_IMAGE_MAX = 2;			//�g�p�摜����
const int	CRoadEnemyManager::ROADENEMYMANAGER_EFFECT_SOUND_MAX = 2;	//�g�p���ʉ���
const HWND	CRoadEnemyManager::m_hWnd = NULL;


/*
*************************************************************************************************
�R���X�g���N�^
*************************************************************************************************
*/
CRoadEnemyManager::CRoadEnemyManager(){
	

}

/*
*************************************************************************************************
�f�B�X�g���N�^
*************************************************************************************************
*/
CRoadEnemyManager::~CRoadEnemyManager(){

}


/*
*************************************************************************************************
������
*************************************************************************************************
*/
void	CRoadEnemyManager::InitRoadEnemyManager( CRunnerManager *lpRunnerManager ){

	//�� �g�p����摜
	char	*lpImageTbl[ CRoadEnemyManager::ROADENEMYMANAGER_IMAGE_MAX ] = {
		{ "Image\\Road\\hurdle.bmp" }	,
		{ "Image\\Road\\stone.bmp" }	,
	};
	//�� �g�p������ʉ�
	char	*lpSoundTbl[CRoadEnemyManager::ROADENEMYMANAGER_EFFECT_SOUND_MAX] = {
		{ "SE\\rattling.wav" },
		{ "SE\\Damage.wav" },
	};

	//�� ��Փx�ɉ����������������Q���̍ő吔�̃f�[�^�e�[�u��
	int obstacleMaxTbl[LEVEL_MAX] = { 8 , 10 , CObstacle::ROADENEMY_OBSTACLE_MAX };

	//�Q�[����Փx���擾����
	tagGAMELEVEL getGameLevel = GetGameLevel();

	//���񐶐������Q���̍ő吔����
	this->m_ObstacleCreateMax = obstacleMaxTbl[ getGameLevel ];

	//�� �g�p���郁�����̊m��
	//�����ɐ����\�ő吔��n��
	this->m_lpCObstacle = new CObstacle( this->m_ObstacleCreateMax );			//��Q���N���X
	
	//�摜�̗v�f
	this->m_lpRoadEnemyManagerImg = new int [ CRoadEnemyManager::ROADENEMYMANAGER_IMAGE_MAX ];

	//���ʉ��̗v�f
	this->m_lpRoadEnemyManagerEffectSound = new int [ CRoadEnemyManager::ROADENEMYMANAGER_EFFECT_SOUND_MAX ];

	//�摜�̗v�f�𐶐�����
	for( int i = 0 ; i < CRoadEnemyManager::ROADENEMYMANAGER_IMAGE_MAX ; i++ ){
		//�摜�̗v�f�𐶐�
		this->m_lpRoadEnemyManagerImg[i] = CreateDDImage( lpImageTbl[i] , CRoadEnemyManager::m_hWnd );

	}
	//���������摜�̗v�f��n��
	this->m_lpCObstacle->SetRoadEnemyImage( this->m_lpRoadEnemyManagerImg , CRoadEnemyManager::ROADENEMYMANAGER_IMAGE_MAX );

	//���ʉ��̗v�f��ݒ肷��
	for( int i = 0 ; i < CRoadEnemyManager::ROADENEMYMANAGER_EFFECT_SOUND_MAX ; i++ ){
		this->m_lpRoadEnemyManagerEffectSound[i] = CreateSound( lpSoundTbl[i] );
	}
	//�����������ʉ��̗v�f��n��
	this->m_lpCObstacle->SetRoadEnemyEffectSound( this->m_lpRoadEnemyManagerEffectSound , CRoadEnemyManager::ROADENEMYMANAGER_EFFECT_SOUND_MAX );

	//�� ��Q���̏�����
	//�����Ɋe���҂̃A�h���X��n��
	this->m_lpCObstacle->InitRoadEnemy( lpRunnerManager->GetCCompetitorManager() ,
		lpRunnerManager->GetCPlayerRunner() );

}

/*
*************************************************************************************************
�X�V
*************************************************************************************************
*/
void	CRoadEnemyManager::UpdateRoadEnemyManager(){

	//��Q���̍X�V
	this->m_lpCObstacle->UpdateRoadEnemy();
}

/*
*************************************************************************************************
�\��
*************************************************************************************************
*/
void	CRoadEnemyManager::DrawRoadEnemyManager(){

	//��Q���̕\��
	this->m_lpCObstacle->DrawRoadEnemy();

}

/*
*************************************************************************************************
���
*************************************************************************************************
*/
void	CRoadEnemyManager::ReleaseRoadEnemyManager(){

	//�摜�̉��
	if( this->m_lpRoadEnemyManagerImg != NULL ){
		for( int i = 0 ; i < CRoadEnemyManager::ROADENEMYMANAGER_IMAGE_MAX ; i++ ){
			ReleaseDDImage( this->m_lpRoadEnemyManagerImg[i] );
		}
	}
	//���ʉ��̉��
	if( this->m_lpRoadEnemyManagerEffectSound != NULL ){
		for( int i = 0 ; i < CRoadEnemyManager::ROADENEMYMANAGER_EFFECT_SOUND_MAX ; i++ ){
			ReleaseSound( this->m_lpRoadEnemyManagerEffectSound[i] );
		}
	}

	//��Q���̉��
	this->m_lpCObstacle->ReleaseRoadEnemy();

	//�������̉��
	delete []this->m_lpRoadEnemyManagerImg;			//�摜�̗v�f
	this->m_lpRoadEnemyManagerImg = NULL;
	delete []this->m_lpRoadEnemyManagerEffectSound;	//���ʉ��̗v�f
	this->m_lpRoadEnemyManagerEffectSound = NULL;

	if( this->m_lpCObstacle != NULL ){
		delete this->m_lpCObstacle;					//��Q���N���X
		this->m_lpCObstacle = NULL;
	}
	


}

/*
*************************************************************************************************
�����̃����i�[�Əd�Ȃ�̔�������
*************************************************************************************************
*/
char	CRoadEnemyManager::CollisionRoadEnemyManager( RUNNER *lpRunner ){

	//��Q���N���X�̏�Q���Ɣ�������
	char	ret = this->m_lpCObstacle->CollisionRoadEnemy( lpRunner );
	//�߂�l�ɍ���̌��ʂ̎�ނ�Ԃ�
	return ret;
}


/*
*************************************************************************************************
�����̃L�����N�^�[�\���̂̍��G�͈͂Ŕ�������
����   : �L�����N�^�[�\����
�߂�l : ��Q���̎�ޔԍ���Ԃ�
***********************************************************************************************
*/
BYTE	CRoadEnemyManager::BroadenCollisionSquareToObstacle( CHARACTER *lpCh ){

	//�����̍��G�͈͂ɏ�Q�������邩���ׁA���̎�ނ�Ԃ�
	BYTE ret = this->m_lpCObstacle->BroadenCollisionSquareObstacle( lpCh );
	//�߂�l�ɏ�Q���̎�ޔԍ���Ԃ�
	return ret;
}


