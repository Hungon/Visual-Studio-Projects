#include	"CEffectManager.h"
#include	"RunnerEffect.h"


//�� �ÓI�����o�ϐ��̍Ē�`
//�E�B���h�E�n���h��
HWND CEffectManager::m_hWnd = NULL;
//�摜�̍ő喇��
const int CEffectManager::EFFECTMANAGER_PICTURE_MAX = 2;
//�G�t�F�N�g�̍ő唭����
const int CEffectManager::EFFECTMANAGER_CREATE_EFFECT_MAX = 3;


/*
*************************************************************************************************
�R���X�g���N�^
*************************************************************************************************
*/
CEffectManager::CEffectManager(){
	

}

/*
*************************************************************************************************
�f�B�X�g���N�^
*************************************************************************************************
*/
CEffectManager::~CEffectManager(){

	//�������̉��
	if( this->m_lpCEffect != NULL ){
		delete []this->m_lpCEffect;					//�G�t�F�N�g�N���X
		this->m_lpCEffect = NULL;
	}

}


/*
*************************************************************************************************
������
*************************************************************************************************
*/
void	CEffectManager::InitEffectManager( int runnerMax ){

	//�� �g�p����摜
	char	*lpImageTbl[ CEffectManager::EFFECTMANAGER_PICTURE_MAX ] = {
		{ "Image\\Road\\RunnerEffect.bmp" }			,
		{ "Image\\Road\\RunnerEffectPower.bmp" }	,
	};

	//�G�t�F�N�g�������҂̑���
	this->m_EffectUserMax = runnerMax;

	//�� �e�탁�����̊m��
	//�G�t�F�N�g�̑����������������m�ۂ���
	this->m_lpCEffect = new CRunnerEffect[ this->m_EffectUserMax ];
	//�摜�̗v�f
	this->m_lpEffectManagerImg = new int[ CEffectManager::EFFECTMANAGER_PICTURE_MAX ];
	

	//�� �摜�̗v�f�̐���
	for( int i = 0 ; i < CEffectManager::EFFECTMANAGER_PICTURE_MAX ; i++ ){
		//�摜�v�f�̐���		
		this->m_lpEffectManagerImg[i] = CreateDDImage( lpImageTbl[i] , CEffectManager::m_hWnd );
	}
	//�e��G�t�F�N�g�N���X�ւ̐ݒ�
	for( int i = 0 ; i < this->m_EffectUserMax ; i++ ){
		//�摜�̗v�f��n��
		this->m_lpCEffect[i].SetRunnerEffectImage( this->m_lpEffectManagerImg , CEffectManager::EFFECTMANAGER_PICTURE_MAX );
		//������
		this->m_lpCEffect[i].InitRunnerEffect( CEffectManager::EFFECTMANAGER_CREATE_EFFECT_MAX );

	}

}

/*
*************************************************************************************************
�X�V
*************************************************************************************************
*/
void	CEffectManager::UpdateEffectManager( RUNNER *lpRunner , int runner ){

	//�e��G�t�F�N�g�N���X�̍X�V
	this->m_lpCEffect[runner].UpdateRunnerEffect(lpRunner->ch.x,
		lpRunner->ch.y, lpRunner->ch.w, lpRunner->ch.h, lpRunner->absoluteTime);

}

/*
*************************************************************************************************
�\��
*************************************************************************************************
*/
void	CEffectManager::DrawEffectManager( int runner ){

	//�e��G�t�F�N�g�N���X�̕\��
	this->m_lpCEffect[runner].DrawRunnerEffect();

}

/*
*************************************************************************************************
���
*************************************************************************************************
*/
void	CEffectManager::ReleaseEffectManager(){

	//�e��G�t�F�N�g�N���X�̉��
	for( int i = 0 ; i < this->m_EffectUserMax ; i++ ){
		//���
		this->m_lpCEffect[i].ReleaseRunnerEffect();
	}

	//�摜�̉��
	//������Ȃ�
	if( this->m_lpEffectManagerImg != NULL ){
		for( int i = 0 ; i < CEffectManager::EFFECTMANAGER_PICTURE_MAX ; i++ ){
			ReleaseDDImage( this->m_lpEffectManagerImg[i] );
		}
	}
	//�������̊J��
	delete []this->m_lpEffectManagerImg;		//�摜�̗v�f
	this->m_lpEffectManagerImg = NULL;

}
/*
*********************************************************************************
�����������L�����N�^�[�p�ɃG�t�F�N�g�𐶐�����
*********************************************************************************
*/
void	CEffectManager::CreateEffectManaager( RUNNER *lpRunner , int runner ){

	//�����̗v�f�̃L�����N�^�[�̃G�t�F�N�g�𐶐�
	this->m_lpCEffect[runner].CreateRunnerEffect( lpRunner->ch.x , lpRunner->ch.y , ( lpRunner->bumpType - 1 ) );
}

/*
*********************************************************************************
���ʂ𖳌��ɂ���G�t�F�N�g�̊Ǘ�
*********************************************************************************
*/
void	CEffectManager::RepealEffectManager( unsigned char type , int runner ){

	//�����̗v�f�̃L�����N�^�[�̃G�t�F�N�g�𖳌��ɂ���
	this->m_lpCEffect[runner].GetRunnerRepealEffectType( type );
}