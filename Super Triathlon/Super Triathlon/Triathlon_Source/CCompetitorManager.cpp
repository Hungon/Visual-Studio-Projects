#include	"CCompetitorManager.h"
#include	"CompetitorRunner.h"
#include	"sound.h"
#include	"CEffectManager.h"


//�� �ÓI�����o�ϐ��̍Ē�`
const int	CCompetitorManager::COMPETITORMANAGER_IMAGE_MAX = 2;		//�g�p�摜����
const int	CCompetitorManager::COMPETITORMANAGER_EFFECT_SOUND_MAX = 1;	//�g�p���ʉ���
const HWND	CCompetitorManager::m_hWnd = NULL;
//�e�����҂̍��W������
TPOINT<float>	*CCompetitorManager::m_lpCompetitorPos = {0};
//�e��Q���̌��ʂ𔭊�������Q���̗v�f
int		*CCompetitorManager::m_lpCompetitorBumpObstacleNow = NULL;
//�����҂̍ő吔
char	CCompetitorManager::m_CompetitorMax = 0;

//�����҃N���X
CCompetitor	*CCompetitorManager::m_lpCCompetitorRunner = NULL;

/*
*************************************************************************************************
�R���X�g���N�^
*************************************************************************************************
*/
CCompetitorManager::CCompetitorManager(){
	
	this->m_lpCCompetitorRunner = NULL;
	this->m_lpCEffect = NULL;
	this->m_CompetitorMax = 0;
	this->m_lpCompetitorEffectSound = NULL;
	this->m_lpCompetitorImg = NULL;

}

/*
*************************************************************************************************
�f�B�X�g���N�^
*************************************************************************************************
*/
CCompetitorManager::~CCompetitorManager(){

	//�������̉��
	if( this->m_lpCCompetitorRunner != NULL ){
		delete []this->m_lpCCompetitorRunner;		//�����҂̃N���X
		this->m_lpCCompetitorRunner = NULL;
	}
	if( this->m_lpCEffect != NULL ){
		delete this->m_lpCEffect;					//�G�t�F�N�g�Ǘ��N���X
		this->m_lpCEffect = NULL;
	}
	if( this->m_lpCompetitorPos != NULL ){
		delete []this->m_lpCompetitorPos;			//�e�����҂̍��W
		this->m_lpCompetitorPos = NULL;
	}
	if( this->m_lpCompetitorBumpObstacleNow != NULL ){
		delete []this->m_lpCompetitorBumpObstacleNow;//��Q���̗v�f
		this->m_lpCompetitorBumpObstacleNow = NULL;
	}


}


/*
*************************************************************************************************
������
*************************************************************************************************
*/
void	CCompetitorManager::InitCompetitorManager( int runnerMax , CRoadEnemyManager *lpRoadEnemy , float roadDistance ){


	//�� �g�p����摜
	char	*lpImageTbl[ CCompetitorManager::COMPETITORMANAGER_IMAGE_MAX ] = {
		{ "Image\\Road\\CompetitorRunner.bmp" }	,
		{ "Image\\Road\\Shadow.bmp" }	,
	};
	//�� �g�p������ʉ�
	char	*lpSoundTbl[ CCompetitorManager::COMPETITORMANAGER_EFFECT_SOUND_MAX ] = {
		{ "SE\\attack.wav" }
	};

	//�����҂̐�
	this->m_CompetitorMax = runnerMax;

	//�� �g�p���郁�����̊m��
	this->m_lpCCompetitorRunner = new CCompetitor[ this->m_CompetitorMax ];			//�����҃N���X
	this->m_lpCEffect = new CEffectManager;										//�G�t�F�N�g�Ǘ��N���X

	//�摜�̗v�f
	this->m_lpCompetitorImg = new int [ CCompetitorManager::COMPETITORMANAGER_IMAGE_MAX ];

	//�擾���W�p
	this->m_lpCompetitorPos = new TPOINT<float> [ this->m_CompetitorMax ];

	//��Q���̗v�f�擾�p
	this->m_lpCompetitorBumpObstacleNow = new int [ this->m_CompetitorMax ];

	//���ʉ��̗v�f
	this->m_lpCompetitorEffectSound = new int [ CCompetitorManager::COMPETITORMANAGER_EFFECT_SOUND_MAX ];

	//�摜�̗v�f�𐶐�����
	for( int i = 0 ; i < CCompetitorManager::COMPETITORMANAGER_IMAGE_MAX ; i++ ){
		//�摜�̗v�f�𐶐�
		this->m_lpCompetitorImg[i] = CreateDDImage( lpImageTbl[i] , CCompetitorManager::m_hWnd );
	}
	//���ʉ��̗v�f��ݒ肷��
	for( int i = 0 ; i < CCompetitorManager::COMPETITORMANAGER_EFFECT_SOUND_MAX ; i++ ){
		this->m_lpCompetitorEffectSound[i] = CreateSound( lpSoundTbl[i] );
	}

	//�� �摜�̐ݒ�
	for( int i = 0 ; i < this->m_CompetitorMax ; i++ ){
		//�����Ҏ��g�̉摜�v�f
		this->m_lpCCompetitorRunner[i].SetCompetitorImage( this->m_lpCompetitorImg[0] );
		//�����҂̉e
		this->m_lpCCompetitorRunner[i].SetCompetitorShadowImage( this->m_lpCompetitorImg[1] );
		//���ʉ�
		this->m_lpCCompetitorRunner[i].SetCompetitorEffectSound( this->m_lpCompetitorEffectSound[0] );
		//�e�틣���҂̏�����
		this->m_lpCCompetitorRunner[i].InitRunner( i , lpRoadEnemy , roadDistance );
		//���W�擾�p
		this->m_lpCompetitorPos[i] = this->m_lpCCompetitorRunner[i].GetOneCompetitorPosition();
		//��Q���̗v�f
		this->m_lpCompetitorBumpObstacleNow[i] = this->m_lpCCompetitorRunner[i].GetCompetitorJumpedObstacleMaterial();
	}

	//�� �G�t�F�N�g�̏�����
	this->m_lpCEffect->InitEffectManager( this->m_CompetitorMax );

}

/*
*************************************************************************************************
�X�V
*************************************************************************************************
*/
void	CCompetitorManager::UpdateCompetitorManager( ){

	//�e�틣���҂̍X�V
	for( int i = 0 ; i < this->m_CompetitorMax ; i++ ){
		//�����Ҏ��g�̍X�V
		this->m_lpCCompetitorRunner[i].UpdateRunner( i , this->m_lpCEffect );
		//���W���擾
		this->m_lpCompetitorPos[i] = this->m_lpCCompetitorRunner[i].GetOneCompetitorPosition();
		//��Q���̗v�f���擾����
		this->m_lpCompetitorBumpObstacleNow[i] = this->m_lpCCompetitorRunner[i].GetCompetitorJumpedObstacleMaterial();
	
	}

}

/*
*************************************************************************************************
�\��
*************************************************************************************************
*/
void	CCompetitorManager::DrawCompetitorManager(){

	//�e�틣���҂̕\��
	for( int i = 0 ; i < this->m_CompetitorMax ; i++ ){
		//�����Ҏ��g
		this->m_lpCCompetitorRunner[i].DrawRunner();
		//�G�t�F�N�g
		this->m_lpCEffect->DrawEffectManager( i );
	}

}

/*
*************************************************************************************************
���
*************************************************************************************************
*/
void	CCompetitorManager::ReleaseCompetitorManager(){

	//�摜�̉��
	if( this->m_lpCompetitorImg != NULL ){
		for( int i = 0 ; i < CCompetitorManager::COMPETITORMANAGER_IMAGE_MAX ; i++ ){
			ReleaseDDImage( this->m_lpCompetitorImg[i] );
		}
	}
	//���ʉ��̉��
	if( this->m_lpCompetitorEffectSound != NULL ){
		for( int i = 0 ; i < CCompetitorManager::COMPETITORMANAGER_EFFECT_SOUND_MAX ; i++ ){
			ReleaseSound( this->m_lpCompetitorEffectSound[i] );
		}
	}
	//�e�틣���҂̉��
	for( int i = 0 ; i < this->m_CompetitorMax ; i++ ){
		//�����Ҏ��g
		this->m_lpCCompetitorRunner[i].ReleaseRunner();
	}
	//�G�t�F�N�g
	this->m_lpCEffect->ReleaseEffectManager();

	//�������̉��
	delete []this->m_lpCompetitorImg;			//�摜�̗v�f
	this->m_lpCompetitorImg = NULL;
	delete []this->m_lpCompetitorEffectSound;	//���ʉ��̗v�f
	this->m_lpCompetitorEffectSound = NULL;


}

//��Q���̗v�f
void	CCompetitorManager::SetCompetitorManagerObstacleMaterial( int material , int runner ){

	this->m_lpCCompetitorRunner[runner].SetCompetitorJumpedObstacleMaterial( material );
}
