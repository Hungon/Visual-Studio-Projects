#include	"CRunnerManager.h"
#include	"CCompetitorManager.h"
#include	"CPlayerRunner.h"
#include	"CRoadEnemyManager.h"
#include	"CEffectManager.h"
#include	"CRoadStage.h"
#include	"play.h"

//�� �ÓI�����o�ϐ��̍Ē�`
//�E�B���h�E�n���h��
const HWND	CRunnerManager::m_hWnd = NULL;
//�摜�̗v�f��
const int CRunnerManager::RUNNERMANAGER_IMAGE_MAX = 2;

//�����҂̍ő吔
const int CRunnerManager::RUNNERMANAGER_COMPETITOR_MAX = 9;

//�v���C���[�̏���
const float CRunnerManager::RUNNERMANAGER_RANKING_INDICATE_X	= 15.0f;		//�\����X
const float CRunnerManager::RUNNERMANAGER_RANKING_INDICATE_Y	= 320.0f;		//���\����Y
const int CRunnerManager::RUNNERMANAGER_RANKING_IMAGE_WIDTH		= 64;			//��
const int CRunnerManager::RUNNERMANAGER_RANKING_IMAGE_HEIGHT	= 64;			//����
const float CRunnerManager::RUNNERMANAGER_RANKING_DEFAULT_SCALE = 1.0f;			//�g�k�{��
int CRunnerManager::m_PlayerRanking = 0;
//�~�j�}�b�v
const int CRunnerManager::RUNNERMANAGER_MINIMAP_IMAGE_MAX = 2;					//�摜�̐�
const float CRunnerManager::RUNNERMANAGER_MINIMAP_INDICATE_X = 600.0f;			//�\����X
const float CRunnerManager::RUNNERMANAGER_MINIMAP_INDICATE_Y = 20.0f;			//�\����Y
const int CRunnerManager::RUNNERMANAGER_MINIMAP_IMAGE_WDITH = 32;				//��
const int CRunnerManager::RUNNERMANAGER_MINIMAP_IMAGE_HEIGHT = 420;				//����

//�~�j�}�b�v��̃v���C���[
const int CRunnerManager::RUNNERMANAGER_MINIMAP_PLAYER_IMAGE_WDITH = 32;		//��
const int CRunnerManager::RUNNERMANAGER_MINIMAP_PLAYER_IMAGE_HEIGHT = 96;		//����
const float CRunnerManager::RUNNERMANAGER_MINIMAP_PLAYER_DEFAULT_SCALE = 0.35f;	//�g�k�{��

//���҃N���X
CPlayerRunner	*CRunnerManager::m_lpCPlayerRunner = NULL;
CCompetitorManager	*CRunnerManager::m_lpCCompetitorManager = NULL;


/*
*************************************************************************************************
�R���X�g���N�^
*************************************************************************************************
*/
CRunnerManager::CRunnerManager(){
	

}

/*
*************************************************************************************************
�f�B�X�g���N�^
*************************************************************************************************
*/
CRunnerManager::~CRunnerManager(){

	//�������̉��
	if( this->m_lpCPlayerRunner != NULL ){
		delete this->m_lpCPlayerRunner;			//�v���C���[�N���X
		this->m_lpCPlayerRunner = NULL;
	}
	if( this->m_lpCCompetitorManager != NULL ){
		delete this->m_lpCCompetitorManager;	//�����ҊǗ��N���X
		this->m_lpCCompetitorManager = NULL;
	}
	if( this->m_lpCEffectManager != NULL ){
		delete this->m_lpCEffectManager;		//�G�t�F�N�g�Ǘ��N���X
		this->m_lpCEffectManager = NULL;
	}
	if( this->m_lpCRoadStage != NULL ){
		delete this->m_lpCRoadStage;			//�X�e�[�W�N���X
		this->m_lpCRoadStage = NULL;
	}
	if( this->m_lpMiniMap != NULL ){
		delete []this->m_lpMiniMap;				//�~�j�}�b�v
		this->m_lpMiniMap = NULL;
	}
}


/*
*************************************************************************************************
������
*************************************************************************************************
*/
void	CRunnerManager::InitRunnerManager( CRoadEnemyManager	*lpRoadEnemy ){

	//��Փx���擾
	tagGAMELEVEL	gameLevel = GetGameLevel();

	//�摜�f�[�^
	char	*lpImageTbl[ CRunnerManager::RUNNERMANAGER_IMAGE_MAX ] = {
		{ "Image\\Road\\MiniMap.bmp" }		,		//�~�j�}�b�v
		{ "Image\\Road\\PlayerRunner.bmp" }		,	//�v���C���[
	};
	//�����҂̐�
	char	CompetitorMaxTbl[ LEVEL_MAX ] = { 3 , 5 , CRunnerManager::RUNNERMANAGER_COMPETITOR_MAX };

	//�e�탁�����̊m��
	this->m_lpCCompetitorManager = new CCompetitorManager;		//�����ҊǗ��N���X
	this->m_lpCPlayerRunner = new CPlayerRunner;				//�v���C���[�N���X
	this->m_lpCEffectManager = new CEffectManager;				//�G�t�F�N�g�Ǘ��N���X
	this->m_lpCRoadStage = new CRoadStage;						//�X�e�[�W�N���X
	this->m_lpRunnerManagerImg = new int [ CRunnerManager::RUNNERMANAGER_IMAGE_MAX ];	//�摜
	this->m_lpMiniMap = new CHARACTER [ CRunnerManager::RUNNERMANAGER_MINIMAP_IMAGE_MAX ];//�~�j�}�b�v

	//����̃X�e�[�W�̑S�����擾
	this->m_StageWholeDistance = this->m_lpCRoadStage->GetRoadStageDistance();

	//�����҂̍ő吔
	this->m_CompetitorMax = CompetitorMaxTbl[ gameLevel ];

	//�� �v���C���[���ʕ\���p
	this->m_RunnerRanking.x = RUNNERMANAGER_RANKING_INDICATE_X;			//�\����X
	this->m_RunnerRanking.y = RUNNERMANAGER_RANKING_INDICATE_Y;			//�\����Y
	this->m_RunnerRanking.w = RUNNERMANAGER_RANKING_IMAGE_WIDTH;		//��
	this->m_RunnerRanking.h = RUNNERMANAGER_RANKING_IMAGE_HEIGHT;		//����
	this->m_RunnerRanking.scale = RUNNERMANAGER_RANKING_DEFAULT_SCALE;	//�g�k�{��

	//�~�j�}�b�v�p
	//0�ŏ�����
	ZeroMemory( this->m_lpMiniMap , sizeof( CHARACTER ) * CRunnerManager::RUNNERMANAGER_MINIMAP_IMAGE_MAX );
	//�~�j���H�摜
	this->m_lpMiniMap[0].x = CRunnerManager::RUNNERMANAGER_MINIMAP_INDICATE_X;	//�\����X
	this->m_lpMiniMap[0].y = CRunnerManager::RUNNERMANAGER_MINIMAP_INDICATE_Y;	//�\����Y
	this->m_lpMiniMap[0].w = CRunnerManager::RUNNERMANAGER_MINIMAP_IMAGE_WDITH;	//��
	this->m_lpMiniMap[0].h = CRunnerManager::RUNNERMANAGER_MINIMAP_IMAGE_HEIGHT;//����

	//�~�j�}�b�v��̃v���C���[
	this->m_lpMiniMap[1].x = this->m_lpMiniMap[0].x;										//�\����X
	this->m_lpMiniMap[1].y = ( this->m_lpMiniMap[0].y + this->m_lpMiniMap[0].h );			//�\����Y
	this->m_lpMiniMap[1].w = CRunnerManager::RUNNERMANAGER_MINIMAP_PLAYER_IMAGE_WDITH;		//��
	this->m_lpMiniMap[1].h = CRunnerManager::RUNNERMANAGER_MINIMAP_PLAYER_IMAGE_HEIGHT;		//����
	this->m_lpMiniMap[1].scale = CRunnerManager::RUNNERMANAGER_MINIMAP_PLAYER_DEFAULT_SCALE;//�g�k�{��



	//�摜�̓ǂݍ���
	for( int i = 0 ; i < CRunnerManager::RUNNERMANAGER_IMAGE_MAX ; i++ ){
		m_lpRunnerManagerImg[i] = CreateDDImage( lpImageTbl[i] , CRunnerManager::m_hWnd );
	}

	//�� �v���C���[�����i�[�̏�����
	//�����ɐl���ƓG�̃A�h���X��n��
	this->m_lpCPlayerRunner->InitRunner( 1 , lpRoadEnemy , this->m_StageWholeDistance );

	//�v���C���[�̃G�t�F�N�g�̏�����
	//�����ɐl�� 
	this->m_lpCEffectManager->InitEffectManager( 1 );

	//�� �����҂̏�����
	this->m_lpCCompetitorManager->InitCompetitorManager( this->m_CompetitorMax , lpRoadEnemy , this->m_StageWholeDistance );

	//treat current player's ranking
	this->m_PlayerRanking = 1;
}

/*
*************************************************************************************************
�X�V
*************************************************************************************************
*/
void	CRunnerManager::UpdateRunnerManager(){

	//�����L���O�J�E���g�p
	int rankingCnt = 1;

	//�e�틣���҂̍��W���擾
	TPOINT<float> *lpCompetitorPos = this->m_lpCCompetitorManager->GetCompetitorManagerPosition();

	//�v���C���[�̏����擾
	RUNNER	*lpRunner = this->m_lpCPlayerRunner->GetRunner();

	//�� �v���C���[�����i�[�̍X�V
	//�v���C���[�ɃG�t�F�N�g�̃A�h���X��n��
	this->m_lpCPlayerRunner->UpdateRunner( 0 , this->m_lpCEffectManager );

	//�� �����҂̍X�V
	this->m_lpCCompetitorManager->UpdateCompetitorManager();

	//�v���C���[�̏��ʂ�����
	for( int i = 0 ; i < this->m_CompetitorMax ; i++ ){ if( lpRunner->ch.y > lpCompetitorPos[i].y ) rankingCnt++; }
	//���݂̏��ʂ���
	this->m_PlayerRanking = rankingCnt;

	//�v���C���[��Y���W���X�e�[�W�̑S���̂ǂ̂��炢�̊����Ɉʒu���Ă��邩�Ń~�j�}�b�v��̃v���C���[�̍��W���X�V����
	this->m_lpMiniMap[1].y = this->m_lpMiniMap[0].h * 
		( lpRunner->ch.y / this->m_StageWholeDistance ) - this->m_lpMiniMap[0].y;


}

/*
*************************************************************************************************
�\��
*************************************************************************************************
*/
void	CRunnerManager::DrawRunnerManager(){

	//�� �v���C���[�����i�[�̕\��
	this->m_lpCPlayerRunner->DrawRunner();

	//�v���C���[�̃G�t�F�N�g��\��
	this->m_lpCEffectManager->DrawEffectManager( 0 );

	//�� �����҂̕\��
	this->m_lpCCompetitorManager->DrawCompetitorManager();

	//�\�������W�̍X�V
	this->m_RunnerRanking.picy = ( this->m_PlayerRanking - 1 ) * this->m_RunnerRanking.h;

	//�� �v���C���[�̏��ʂ�\��
	DrawRanking(this->m_RunnerRanking.x,
		this->m_RunnerRanking.y,
		this->m_RunnerRanking.scale,
		this->m_PlayerRanking,
		SCORE_RANING_COLOR_BLACK);

	//�� �~�j�}�b�v�̕\��
	//�w�i
	DDBlt(
		(int)this->m_lpMiniMap[0].x ,
		(int)this->m_lpMiniMap[0].y ,
		this->m_lpMiniMap[0].w ,
		this->m_lpMiniMap[0].h ,
		m_lpRunnerManagerImg[0] ,
		this->m_lpMiniMap[0].picx ,
		this->m_lpMiniMap[0].picy 
		);
	//�v���C���[
	DDScaleBlt(
		this->m_lpMiniMap[1].x ,
		this->m_lpMiniMap[1].y ,
		this->m_lpMiniMap[1].w ,
		this->m_lpMiniMap[1].h ,
		m_lpRunnerManagerImg[1] ,
		this->m_lpMiniMap[1].picx ,
		this->m_lpMiniMap[1].picy ,
		this->m_lpMiniMap[1].scale
		);


}

/*
*************************************************************************************************
���
*************************************************************************************************
*/
void	CRunnerManager::ReleaseRunnerManager(){

	//�摜�̉��
	if(  this->m_lpRunnerManagerImg != NULL ){
		for( int i = 0 ; i < CRunnerManager::RUNNERMANAGER_IMAGE_MAX ; i++ ){
			ReleaseDDImage( this->m_lpRunnerManagerImg[i] );
		}
	}
	//���v���C���[�����i�[�̉��
	this->m_lpCPlayerRunner->ReleaseRunner();
	//�v���C���[�̃G�t�F�N�g�̉��
	this->m_lpCEffectManager->ReleaseEffectManager();

	//�������҂̉��
	this->m_lpCCompetitorManager->ReleaseCompetitorManager();

	//�������̉��
	delete []this->m_lpRunnerManagerImg;	//�摜
	this->m_lpRunnerManagerImg = NULL;

	//�������̉��
	if( this->m_lpCPlayerRunner != NULL ){
		delete this->m_lpCPlayerRunner;			//�v���C���[�N���X
		this->m_lpCPlayerRunner = NULL;
	}
	if( this->m_lpCCompetitorManager != NULL ){
		delete this->m_lpCCompetitorManager;	//�����ҊǗ��N���X
		this->m_lpCCompetitorManager = NULL;
	}
	if( this->m_lpCEffectManager != NULL ){
		delete this->m_lpCEffectManager;		//�G�t�F�N�g�Ǘ��N���X
		this->m_lpCEffectManager = NULL;
	}
	if( this->m_lpCRoadStage != NULL ){
		delete this->m_lpCRoadStage;			//�X�e�[�W�N���X
		this->m_lpCRoadStage = NULL;
	}
	if( this->m_lpMiniMap != NULL ){
		delete []this->m_lpMiniMap;				//�~�j�}�b�v
		this->m_lpMiniMap = NULL;
	}

}

