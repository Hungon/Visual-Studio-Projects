#include	"CompetitorRunner.h"
#include	"CPlayerRunner.h"
#include	"CEffectManager.h"
#include	"CRoadEnemyManager.h"
#include	"CRoadEnemy.h"
#include	"CRoadStage.h"
#include	"animation.h"
#include	"map.h"
#include	"sound.h"
#include	"play.h"
#include	<iostream>
#include	<math.h>

using namespace std;

//�A�N�V�����̎�ސ�
const int	CCompetitor::COMPETITOR_KIND_OF_EFFECT = 2;
//���W�擾�p
TPOINT<float>	CCompetitor::m_CompetitorPos = {0};
//�\���Ԋu
const int	CCompetitor::COMPETITOR_INDICATE_SPACE_X = 30;
//��ʊO�ɏo���ۂ̐������W
const float	CCompetitor::COMPETITOR_CONSTRAIN_FRONT_POSITION_Y = 500.0f;		//�O��
const float	CCompetitor::COMPETITOR_CONSTRAIN_BACK_POSITION_Y = 200.0f;			//���

//��щz������Q���̗v�f�������i�擾�p�j
int CCompetitor::m_getJumpedMaterial = -1;


/*
************************************************************************************************
�R���X�g���N�^
************************************************************************************************
*/
CCompetitor::CCompetitor(){

	this->m_CompetitorRunnerImg = NULL;
	this->m_CompetitorShadowImg = NULL;
	this->m_lpActionRateToHurdle = NULL;
	this->m_lpActionRateToRock = NULL;
	this->m_VariableSpeedNow = 0.0f;
	this->m_GetVariableSpeed = 0.0f;
	this->m_ConstarinSpeedMax = 0.0f;
	this->m_lpCRoadEnemy = NULL;

}


/*
************************************************************************************************
�f�B�X�g���N�^
************************************************************************************************
*/
CCompetitor::~CCompetitor(){

	//�������̉��[
	if( this->m_lpActionRateToHurdle != NULL ){
		delete []this->m_lpActionRateToHurdle;	//�n�[�h���ɑ΂���A�N�V�����m��
		this->m_lpActionRateToHurdle = NULL;
	}
	if( this->m_lpActionRateToRock != NULL ){
		delete []this->m_lpActionRateToRock;	//��΂ɑ΂���A�N�V�����m��	
		this->m_lpActionRateToRock = NULL;
	}
	if( this->m_lpCPlayerRunner != NULL ){
		delete this->m_lpCPlayerRunner;			//�v���C���[
		this->m_lpCPlayerRunner = NULL;
	}

}

//************************************************************************************************
//�����҂̏�����
//************************************************************************************************
void   CCompetitor::InitRunner( int runnerNum , CRoadEnemyManager	*lpRoadEnemy , float roadDistance ){

	//�e�탁�����̊m��
	this->m_lpCPlayerRunner = new CPlayerRunner;				//�v���C���[�N���X
	//�A�N�V�����̐��������������m��
	this->m_lpActionRateToHurdle = new int [ COMPETITOR_RUNNER_KIND_OF_ACTION ];		//�n�[�h��
	this->m_lpActionRateToRock = new int [ COMPETITOR_RUNNER_KIND_OF_ACTION ];			//���

	//���W�擾�p�ɐ錾
	TPOINT<float> pos = {0};

	//�Q�[����Փx���擾
	tagGAMELEVEL gameLevel = GetGameLevel();

	//�v���C���[�̏��擾
	RUNNER	*lpPlayer = this->m_lpCPlayerRunner->GetRunner();

	//�L�����N�^�[�P�l�̕\���T�C�Y
	int charaSizeW = (int)( RUNNER_DEFAULT_IMAGE_WIDTH * RUNNER_DEFAULT_IMAGE_SCALE );

	//���D��x�������A�N�V�����قǁA���̂ق��ɔz�u����
	//�Q�[����Փx�ɉ����Ĕ����m����ω�
	//�@ �n�[�h��
	int hurdlePerTbl[LEVEL_MAX][COMPETITOR_RUNNER_KIND_OF_ACTION] = {

		//��v�f �F �W�����v �� �U�� �� �ړ����
		{ 80 , 30 , 60 } ,						//EASY
		
		{ 90 , 20 , 50 } ,						//NORMAL

		{ 100 , 0 , 0 } ,						//HARD	
		
	};
	//�A ���
	int rockPerTbl[LEVEL_MAX][COMPETITOR_RUNNER_KIND_OF_ACTION] = {

		//��v�f �F �W�����v �� �U�� �� �ړ����
		{ 0 , 80 , 60 } ,						//EASY
		
		{ 0 , 90 , 50 } ,						//NORMAL

		{ 0 , 100 , 40 } ,						//HARD	
		
	};

	//���݂̎��g�̈ʒu�ƃv���C���[�̈ʒu�ɉ��������x�̕ω�
	float	variableSpeedTbl[ LEVEL_MAX ] = { 1.0f , 1.3f , 1.6f };

	//���x����
	float	constrainSpeedTbl[ LEVEL_MAX ] = { -4.5f , -5.0f , -6.0f };

	//�����҂̕\����X���W�f�[�^
	float	PosXTbl[10] = {

		( lpPlayer->ch.x + charaSizeW ) + CCompetitor::COMPETITOR_INDICATE_SPACE_X ,				 //1�l��
		( lpPlayer->ch.x + ( charaSizeW * 2 ) ) + ( CCompetitor::COMPETITOR_INDICATE_SPACE_X * 2 ) , //2�l��
		( lpPlayer->ch.x + ( charaSizeW * 3 ) ) + ( CCompetitor::COMPETITOR_INDICATE_SPACE_X * 3 ) , //3�l��
		( lpPlayer->ch.x - charaSizeW ) - CCompetitor::COMPETITOR_INDICATE_SPACE_X ,				 //4�l��
		( lpPlayer->ch.x - ( charaSizeW * 2 ) ) - ( CCompetitor::COMPETITOR_INDICATE_SPACE_X * 2 ) , //5�l��
		( lpPlayer->ch.x - ( charaSizeW * 3 ) ) - ( CCompetitor::COMPETITOR_INDICATE_SPACE_X * 3 ) , //6�l��
		( lpPlayer->ch.x + ( charaSizeW * 4 ) ) + ( CCompetitor::COMPETITOR_INDICATE_SPACE_X * 4 ) , //7�l��
		( lpPlayer->ch.x + ( charaSizeW * 5 ) ) + ( CCompetitor::COMPETITOR_INDICATE_SPACE_X * 5 ) , //8�l��
		( lpPlayer->ch.x - ( charaSizeW * 4 ) ) - ( CCompetitor::COMPETITOR_INDICATE_SPACE_X * 4 ) , //9�l��
		( lpPlayer->ch.x - ( charaSizeW * 5 ) ) - ( CCompetitor::COMPETITOR_INDICATE_SPACE_X * 5 ) , //10�l��

	};
	
		
	//�� �e�A�N�V���������m��
	//��Q���Ǘ��N���X
	this->m_lpCRoadEnemy = lpRoadEnemy;	
	
	//�� �\���ݒ�
	//�e�l����
	this->m_RunnerDefault.ch.x = PosXTbl[runnerNum];										//�\����X
	this->m_RunnerDefault.ch.y = roadDistance - CAMERA_FOCUS_POSITION_Y;					//�\����Y
	this->m_RunnerDefault.ch.mx = 0.0f;														//X�ړ���
	this->m_RunnerDefault.ch.flag = true;													//�����t���O
	//�擾�p���W
	pos.x = this->m_RunnerDefault.ch.x;
	pos.y = this->m_RunnerDefault.ch.y;
	this->m_CompetitorPos = pos;
	//���x�ω��l
	this->m_GetVariableSpeed = variableSpeedTbl[ gameLevel ];
	//���x����
	this->m_ConstarinSpeedMax = constrainSpeedTbl[ gameLevel ];
	
	//�� �A�N�V�������x����			
	//�@ �n�[�h��
	//�W�����v
	this->m_lpActionRateToHurdle[0] = hurdlePerTbl[gameLevel][0];
	//�U��
	this->m_lpActionRateToHurdle[1] = hurdlePerTbl[gameLevel][1];
	//�ړ����
	this->m_lpActionRateToHurdle[2] = hurdlePerTbl[gameLevel][2];

	//�A ���
	//�W�����v
	this->m_lpActionRateToRock[0] = rockPerTbl[gameLevel][0];
	//�U��
	this->m_lpActionRateToRock[1] = rockPerTbl[gameLevel][1];
	//�ړ����
	this->m_lpActionRateToRock[2] = rockPerTbl[gameLevel][2];



	//�A�j���[�V�������̐ݒ�
	SetAnimation( &this->m_RunnerDefault.ch , &this->m_RunnerDefault.ani ,
		this->m_RunnerDefault.ani.sx , this->m_RunnerDefault.ani.sy ,
		this->m_RunnerDefault.ani.max , this->m_RunnerDefault.ani.fram , 
		this->m_RunnerDefault.ani.type , true);

}

//************************************************************************************************
//�����҂̍X�V
//************************************************************************************************
void   CCompetitor::UpdateRunner( int runner , CEffectManager *lpCEffect ){

	//�v���C���[�����i�[�̏����擾����
	RUNNER *prunner = this->m_lpCPlayerRunner->GetRunner();

	//�����蔻������Ȃ���`����
	//��A�E�A���A���̏�
	TSAFETYRECT<int> safetyRect = { 60 , 10 , 60 , 10 };

	//�擾���W�p
	TPOINT<float> Pos = { this->m_RunnerDefault.ch.x , this->m_RunnerDefault.ch.y };

	//�Փˎ��Ɏ擾������ʃf�[�^�e�[�u��
	BYTE bumpTbl[ CCompetitor::COMPETITOR_KIND_OF_EFFECT ] = {

		RUNNER_EFFECT_FLAG_ABSOLUTE			,		//���G���
		RUNNER_EFFECT_FLAG_SPEED_UP 			,		//���x�㏸
	
	};

	//���ʃf�[�^�e�[�u��
	BYTE effectTbl[ CCompetitor::COMPETITOR_KIND_OF_EFFECT ] = {

		RUNNER_EFFECT_TYPE_ABSOLUTE			,		//���G���
		RUNNER_EFFECT_TYPE_SPEED_UP			,		//���x�㏸
	
	};

	//�J�����̍��W���擾
	float comeCameraY = GetCameraY();

	//�� �����҂̃A�N�V�����t���O�e�[�u��
	BYTE actionTbl[COMPETITOR_RUNNER_KIND_OF_ACTION] = {

		RUNNER_ACTION_JUMP				,	//�W�����v�t���O
		RUNNER_ACTION_ATTACK			,	//�U���t���O
		RUNNER_ACTION_LINE_MOVE			,	//���ړ��t���O

	};
	
	//�� ���x�̊Ǘ�
	this->ManageCompetitorRunnerSpeed();

	//�� �����i�[���m�̔���i�ʒu�C���j
	CheckOverlapCharacter( &prunner->ch , &this->m_RunnerDefault.ch , safetyRect );
	
	//�� ��Q���ɑ΂��ẴA�N�V�����ݒ�
	this->SetActionToObstacle();

	//�� �A�N�V�����s���̃t���O�������Ă�����X�V����
	for( int i = 0 ; i < COMPETITOR_RUNNER_KIND_OF_ACTION ; i++ ){
	
		//�Ή������A�N�V�����̍X�V����
		if( this->m_RunnerDefault.actionF == actionTbl[i] ){

			//�s���̕���
			switch( i ){
				case 0:
					this->UpdateCompetitorRunnerEvadeHardol();
					break;
				case 1:
					this->UpdateCompetitorRunnerAttack();
					break;
				case 2:
					this->UpdateCompetitorRunnerLineMove();
					break;
			}
			break;
		}
	}

	//�� ��Q���Ƃ̓����蔻��
	//�����t���O��true�Ȃ�
	if( this->m_RunnerDefault.ch.flag ){

		//��Q���Ɣ��������ăt���O���󂯎��
		this->m_RunnerDefault.bumpType = this->m_lpCRoadEnemy->CollisionRoadEnemyManager( &this->m_RunnerDefault );
				
		//�� �������ʂ̕���
		//�ʏ�̏ՓˁA�Փ˂��Ă��Ȃ��Ȃ珈�������Ȃ�
		if (this->m_RunnerDefault.bumpType != RUNNER_NOT_BUMP ||
			this->m_RunnerDefault.bumpType != RUNNNER_BUMP_TO_HURDLE ||
			this->m_RunnerDefault.bumpType != RUNNNER_BUMP_TO_ROCK) {
			//�^�C�v�̐�������
			for (int i = 0; i < (RUNNER_EFFECT_KIND_OF_TYPE - 1); i++) {
				//�擾�����Փ˃t���O�̔ԍ������ɕ���
				if (this->m_RunnerDefault.bumpType == bumpTbl[i]) {
					//�Փ˂�����Q���̗v�f�����X�V
					this->m_getJumpedMaterial = this->m_RunnerDefault.obstacleBumpNow;
					//�� �G�t�F�N�g�̐���
					lpCEffect->CreateEffectManaager(&this->m_RunnerDefault, runner);
					//�� ���ʂ̕���
					switch (i) {
					case 0:
						CRunner::RunnerEffectAbsolute();
						break;
					case 1:
						CRunner::RunnerEffectSpeedUp();
						break;
					}
					//�Փ˃t���O���Ȃ���
					this->m_RunnerDefault.bumpType = RUNNER_NOT_BUMP;
					break;
				}
			}
		}
		//�� ���ʂɏՓ˂����珈��
		if (this->m_RunnerDefault.bumpType == RUNNNER_BUMP_TO_HURDLE ||
			this->m_RunnerDefault.bumpType == RUNNNER_BUMP_TO_ROCK) {
			//���ʉ�
			PlaySound( this->m_CompetitorEffectSound , 0 , false );
			//�� ���x�㏸���ʂ𖳌���
			CRunner::RepealRunnerEffectSpeedUp();		
			//�G�t�F�N�g�̔�\��
			lpCEffect->RepealEffectManager( 0 , runner );
			//�Փ˃t���O���Ȃ���
			this->m_RunnerDefault.bumpType = RUNNER_NOT_BUMP; 
			//�����t���O
			this->m_RunnerDefault.ch.flag = false;
		}
	}

	//�� ���G���̏���
	//���G���Ȃ玞�Ԃ𑪂�A���̎��ԂɂȂ����疳�G��Ԃ𖳌���
	if( CRunner::RepealRunnerEffectAbsolute() == false){
		//�G�t�F�N�g�̔�\��
		lpCEffect->RepealEffectManager( 1 , runner );
	}

	//�� �G�t�F�N�g�̍X�V
	lpCEffect->UpdateEffectManager( &this->m_RunnerDefault , runner );

	//�� �v���C���[�������
	AvoidEnemy( &this->m_RunnerDefault.ch , this->m_RunnerDefault.avoidtime );	

	//�� ����J�����ɍ��킹�ăL�����N�^�[���g�k��
	FittingPositionToBackCameraAngle( &this->m_RunnerDefault.ch , 380.0f / 640.0f );
	
	//�� �A�j���[�V�����̍X�V
	this->ChangeCompetitorRunnerAnimation();
	
	//�� �ړ��͈͂̐���
	this->ManageConstrainCompetitorRunnerMove();

	//�� ���݂̍��W����
	this->m_CompetitorPos = Pos;

}


//************************************************************************************************
//�����҂̕\��
//************************************************************************************************
void   CCompetitor::DrawRunner(){

	//�J�����̍��W���擾����
	float comeCameraX = GetCameraX();		//X���W
	float comeCameraY = GetCameraY();		//Y���W

	char mes[32] = {0};

	int x = 480 , y = 0;
/*
	wsprintf( &mes[0] , "C�̃^�C�v%d" , this->m_RunnerDefault.effectType );
	DDTextOut( &mes[0] , x, y );

	wsprintf( &mes[0] , "C�̏Փ˃^�C�v%d" , this->m_RunnerDefault.bumpType );
	DDTextOut( &mes[0] , x , y + 20 );

	sprintf( &mes[0] , "C�̑��x%f" , (this->m_RunnerDefault.ch.speed * this->m_RunnerDefault.ch.my) * 10 );
	DDTextOut( &mes[0] , x , y + 40 );
	
	wsprintf( &mes[0] , "C��Y���W%d" , (int)this->m_RunnerDefault.ch.y );
	DDTextOut( &mes[0] , x , y + 60 );

	wsprintf( &mes[0] , "C�̖��G����%d" , this->m_RunnerDefault.absoluteTime );
	DDTextOut( &mes[0] , x , y + 80 );

*/

	//�����҂̕\��
	if( this->m_RunnerDefault.ch.time % 2 == 0 ){
		
		//�e
		DDAlphaBlt(
			(int)( ( this->m_RunnerDefault.ch.x - 5.0f ) - comeCameraX ) ,
			(int)( ( this->m_RunnerDefault.ch.y + this->m_RunnerDefault.ch.h - 
			( m_ShadowDefault.ch.h / 2 ) ) - comeCameraY ) ,
			m_ShadowDefault.ch.w ,
			m_ShadowDefault.ch.h ,
			this->m_CompetitorShadowImg ,
			m_ShadowDefault.ch.picx ,
			m_ShadowDefault.ch.picy ,
			m_ShadowDefault.ch.alpha
			);


		//�����Ҏ��g�̕\��
		DDScaleBlt(
			this->m_RunnerDefault.ch.x  - comeCameraX ,
			this->m_RunnerDefault.ch.y  - comeCameraY ,
			this->m_RunnerDefault.ch.w ,
			this->m_RunnerDefault.ch.h ,
			this->m_CompetitorRunnerImg ,
			this->m_RunnerDefault.ch.picx ,
			this->m_RunnerDefault.ch.picy ,
			this->m_RunnerDefault.ch.scale);

	}

}

//************************************************************************************************
//�����҂̉��
//************************************************************************************************
void   CCompetitor::ReleaseRunner(){

	//�������̉��
	if( this->m_lpActionRateToHurdle != NULL ){
		delete []this->m_lpActionRateToHurdle;	//�n�[�h���ɑ΂���A�N�V�����m��
		this->m_lpActionRateToHurdle = NULL;
	}
	if( this->m_lpActionRateToRock != NULL ){
		delete []this->m_lpActionRateToRock;	//��΂ɑ΂���A�N�V�����m��	
		this->m_lpActionRateToRock = NULL;
	}

}

//************************************************************************************************
//�����҂̑��x���Ǘ�����
//************************************************************************************************
void   CCompetitor::ManageCompetitorRunnerSpeed(){

	//���x�ɏ������^�C�v����
	tagRUNNERANIMATIONSPEED speedType[4] = {

		RUNNER_SPEED_SLOW	,		//�x��
		RUNNER_SPEED_NORMAL ,		//����
		RUNNER_SPEED_DECENT ,		//��������
		RUNNER_SPEED_RAPID	,		//����	
	
	};

	//���݂̑��x���v���X�V
	this->m_RunnerDefault.aggregateSpeed = ( this->m_RunnerDefault.ch.my * this->m_RunnerDefault.ch.speed ) + 
		this->m_VariableSpeedNow;
	
	//���x�̏��
	if( fabs( this->m_RunnerDefault.aggregateSpeed ) >= fabs( this->m_ConstarinSpeedMax ) ){
		//�������
		this->m_RunnerDefault.aggregateSpeed = this->m_ConstarinSpeedMax;
	}

	//���x�̉���
	if( fabs( this->m_RunnerDefault.aggregateSpeed ) <= fabs( RUNNER_CONSTRAIN_MINIMUM_SPEED ) ){
		//��������
		this->m_RunnerDefault.aggregateSpeed = RUNNER_CONSTRAIN_MINIMUM_SPEED;
	}

	//�����x���x�����X�V
	//���������璲�ׂČ��ݑ��x���������烋�[�v�𔲂��āA�A�j���[�V�������X�V
	//�ʏ펞�̂ݔ��f
	if( this->m_RunnerDefault.ani.type == RUNNER_ANIMATION_ROUND ){
		for( int i = 3 ; i >= 0 ; i-- ){
			
			//���݂̑��x�����̒l�������Ă�����
			if( fabs( this->m_RunnerDefault.ch.my ) * this->m_RunnerDefault.ch.speed > 
				i * RUNNER_SPEED_ANIMATION_RATE ){
				
				//���x���x���̃}�N������
				this->m_RunnerDefault.ani.fram = speedType[i];
				break;
			}
		}
	}
	//�����t���O��true�Ȃ�ړ��ʂ����Z����
	if( this->m_RunnerDefault.ch.flag ){
		this->m_RunnerDefault.ch.y += this->m_RunnerDefault.aggregateSpeed;	//Y
	}

}


//************************************************************************************************
//�����҂̈ړ��͈͂𐧌䂷��
//************************************************************************************************
void   CCompetitor::ManageConstrainCompetitorRunnerMove(){


	//�J�����̍��W���擾
	float comeCameraY = GetCameraY();
	//�v���C���[�̏����擾
	RUNNER	*lpPlayer = this->m_lpCPlayerRunner->GetRunner();

	//���ړ��͈͂𐧌�
	//��ʏ�[
	//�������������Ȃ��悤�ɐ�����݂���
	if( this->m_RunnerDefault.ch.y < ( comeCameraY - CCompetitor::COMPETITOR_CONSTRAIN_FRONT_POSITION_Y ) ){

		//��ʏ�[���琧�����W�܂�
		this->m_RunnerDefault.ch.y = ( comeCameraY - CCompetitor::COMPETITOR_CONSTRAIN_FRONT_POSITION_Y );
	}
	
	//calculate competitor's coordinateY in local coordinateY
	float localY = this->m_RunnerDefault.ch.y - comeCameraY;
	//calculate railing's coordinateY into rate of coordinateY in stage's height
	float rateY = ((float)STAGE_HEIGHT - localY) / (float)(STAGE_HEIGHT - ROADSTAGE_SUBTLTY_POSITIONX_FOR_ROAD_WIDTH);
	//constrain move
	float margin = ROADSTAGE_MARGIN_WIDTH_FOR_TOP_AND_BOTTOM_IN_ROAD * rateY;
	//��ʍ��[
	if (this->m_RunnerDefault.ch.x < margin) this->m_RunnerDefault.ch.x = margin;

	//��ʉE�[
	if (this->m_RunnerDefault.ch.x + (this->m_RunnerDefault.ch.w * this->m_RunnerDefault.ch.scale) >
		(STAGE_WIDTH - margin)) {
		this->m_RunnerDefault.ch.x = (STAGE_WIDTH - margin) - (this->m_RunnerDefault.ch.w * this->m_RunnerDefault.ch.scale);
	}
		
	//��ʉ��[
	//���������ꂷ���Ȃ��悤�ɐ�����݂���
	if( this->m_RunnerDefault.ch.y + ( this->m_RunnerDefault.ch.h * 
		this->m_RunnerDefault.ch.scale ) > ( comeCameraY + STAGE_HEIGHT ) + CCompetitor::COMPETITOR_CONSTRAIN_BACK_POSITION_Y ){
		
		//��ʉ��[���琧�����W�܂�
		this->m_RunnerDefault.ch.y = 
		( ( comeCameraY + STAGE_HEIGHT ) + CCompetitor::COMPETITOR_CONSTRAIN_BACK_POSITION_Y ) - 
		( this->m_RunnerDefault.ch.h * this->m_RunnerDefault.ch.scale );
		//�v���C���[�̌��݂̑��x��菭�����x���グ��
		this->m_VariableSpeedNow = -1 * this->m_GetVariableSpeed;
	}

	//�ǂ��グ���Ƀv���C���[�t�߂�Y���\���B������A���x�ω��l�����Z�b�g����
	if( this->m_RunnerDefault.ch.y <= lpPlayer->ch.y ) this->m_VariableSpeedNow = 0;

}



/**********************************************************************************************************************
��Q���̎�ނɉ����ēK�؂ȍs�����m���Ŕ��f����
**************************************************************************************************************************/
void   CCompetitor::SetActionToObstacle(){


	//�� ��Q���̃r�b�g�ڂ̎�ނ��܂Ƃ߂��f�[�^�e�[�u��
	BYTE obstacleTbl[ ROADENEMY_KIND_OF_OBSTACLE ] = {

		ROADENEMY_OBSTACLE_BIT_TYPE_HURDLE	,			//�n�[�h��
		ROADENEMY_OBSTACLE_BIT_TYPE_ROCK	,			//���
	
	};
	
	//�� �e��Q���ɑ΂���A�N�V���������m�����܂Ƃ߂��f�[�^�e�[�u��
	int actionPerTbl[ROADENEMY_KIND_OF_OBSTACLE][COMPETITOR_RUNNER_KIND_OF_ACTION] = {

		//�@ �n�[�h��
		//��F�U�� �� �ړ���� �� �W�����v
		{ this->m_lpActionRateToHurdle[1] , this->m_lpActionRateToHurdle[2] , this->m_lpActionRateToHurdle[0] } ,

		//�A ���
		//��F�W�����v �� �ړ���� �� �U��
		{ this->m_lpActionRateToRock[0] , this->m_lpActionRateToRock[2] , this->m_lpActionRateToRock[1] } ,
	
	};

	
	//�� �����҂̃A�N�V�����t���O�e�[�u��
	tagCOMPETITORACTIONTYPE actionTbl[ROADENEMY_KIND_OF_OBSTACLE][COMPETITOR_RUNNER_KIND_OF_ACTION] = {
	
		//�n�[�h��
		{ COMPETITOR_RUNNER_ACTION_TYPE_ATTACK			,	//�U���t���O
		  COMPETITOR_RUNNER_ACTION_TYPE_EVADE			,	//����ړ��t���O
  		  COMPETITOR_RUNNER_ACTION_TYPE_JUMP			,	//�W�����v�t���O
		},

		//���
		{ COMPETITOR_RUNNER_ACTION_TYPE_JUMP			,	//�W�����v�t���O
		  COMPETITOR_RUNNER_ACTION_TYPE_EVADE			,	//����ړ��t���O
  		  COMPETITOR_RUNNER_ACTION_TYPE_ATTACK			,	//�U���t���O
		},

	};
					
	//�� ��Q�������G
	//���������Ă��Ȃ�������
	if( this->m_RunnerDefault.getType == ROADENEMY_OBSTACLE_BIT_TYPE_VACANT ){
	
		//��Q���̎�ނ��擾����
		this->m_RunnerDefault.getType |= 
			this->m_lpCRoadEnemy->BroadenCollisionSquareToObstacle( &this->m_RunnerDefault.ch );

		//�A�N�V�����̎�ނ����Z�b�g
		this->m_RunnerDefault.actionType = COMPETITOR_RUNNER_ACTION_TYPE_VACANT;
	
	}

	//��Q���̎�ނ�����
	for( int i = 0 ; i < ROADENEMY_KIND_OF_OBSTACLE ; i++ ){

		//�擾������Q���̎�ނɉ����ď����𕪊򂳂���
		//��Q���������Ă����珈������		
		if( this->m_RunnerDefault.actionF == RUNNER_ACTION_ROUND && 
			this->m_RunnerDefault.getType == obstacleTbl[i] ){
				
			//�Փ˃t���O�ɂ���
			this->m_RunnerDefault.avoidF |= RUNNER_BUMP_ATTENTION;

			//��Q���ɑ΂��čs���̐��x���m���Ŏ擾
			//�A�N�V�����^�C�v����
			for( int j = 0 ; j < COMPETITOR_RUNNER_KIND_OF_ACTION ; j++ ){


				//�����Ɋm����n���āA�ǂ̃A�N�V�������s���������߂�
				this->m_RunnerDefault.actionType = 
					this->ConmpetitorAttemptDoAction( actionPerTbl[i][j] , actionTbl[i][j] );

				//�����̃A�N�V�������擾���Ă����烋�[�v�𔲂���
				if( this->m_RunnerDefault.actionType != COMPETITOR_RUNNER_ACTION_TYPE_VACANT )	break;
			
			}

		//�����A�N�V�����^�C�v���擾�ł��Ȃ������ꍇ�́Areturn���ĉ������Ȃ�
		if( this->m_RunnerDefault.actionType == COMPETITOR_RUNNER_ACTION_TYPE_VACANT ) return;

		//�Ή�������Q�����������̂Ń��[�v�𔲂���
		break;	
		
		}
		
	}
		
	//�Փ˃t���O�������Ă��āA�Ȃ����A����t���O���łȂ���΍s���̐ݒ�
	if( this->m_RunnerDefault.avoidF == RUNNER_BUMP_ATTENTION && 
		this->m_RunnerDefault.avoidF != RUNNER_WHILE_AVOID ){

		//�Փ˃t���O���Ȃ���
		this->m_RunnerDefault.avoidF &= ~RUNNER_BUMP_ATTENTION;

		//�� �ݒ�̕���
		switch( this->m_RunnerDefault.actionType ){
			case 0:
				this->SetCompetitorRunnerEvadeHardol();
				break;
			case 1:
				this->SetCompetitorRunnerAttack();
				break;
			case 2:
				this->SetCompetitorRunnerLineMove();
				break;
		}
	}

}


//*************************************************************************************************
//�����҂̍s���P�̏������u�O���ɏ�Q��������Ή��Ɉړ��v
//*************************************************************************************************
void   CCompetitor::SetCompetitorRunnerLineMove() {

	//get current global coordinateY
	float gY = GetCameraY();

	//����t���O��ON�ɂ���
	this->m_RunnerDefault.avoidF |= RUNNER_WHILE_AVOID;

	//���ړ��t���O��ON
	this->m_RunnerDefault.actionF |= RUNNER_ACTION_LINE_MOVE;

	//����ړ��������O�Ƀ��Z�b�g
	this->m_RunnerDefault.avoidDistance = 0;

	//�v���C���[�̉E�[�̍��W�����߂�
	float rectRightPos = this->m_RunnerDefault.ch.x + (this->m_RunnerDefault.ch.w * this->m_RunnerDefault.ch.scale);

	//�����_���l
	//�ړ��������Ƃ肠���������_����
	int selectRoot = RandomNum(1, 0);

	//���v���C���[�̍��W����ʂɋ߂��Ȃ甽�΂̈ړ������A�ړ�����w�肷��
	//���[�Ȃ�
	if (this->m_RunnerDefault.ch.x >= 0 &&
		this->m_RunnerDefault.ch.x <= this->m_RunnerDefault.ch.w) {
		//�ړ������A�ړ���̓v���X�̍��W��
		selectRoot = 0;
	}

	//calculate competitor's coordinateY in local coordinateY
	float localY = this->m_RunnerDefault.ch.y - gY;
	//calculate competitor's coordinateY into rate of coordinateY in stage's height
	float rateY = ((float)STAGE_HEIGHT - localY) / (float)STAGE_HEIGHT;
	//constrain move
	float margin = 125.0f * rateY;
	//�E�[�Ȃ�
	if (rectRightPos <= (STAGE_WIDTH - margin) &&
		rectRightPos >= ((STAGE_WIDTH - this->m_RunnerDefault.ch.w) - margin)) {
		//�ړ������A�ړ���̓v���X�̍��W��
		selectRoot = 1;
	}
	//�ړ����������߂�
	int distance[2] = {
		(int)(this->m_RunnerDefault.ch.w * this->m_RunnerDefault.ch.scale) * 3 ,			//�E����
		(int)(this->m_RunnerDefault.ch.w * this->m_RunnerDefault.ch.scale) * -3			//������
	};
	//����܂ł̈ړ��������󂯎��
	this->m_RunnerDefault.designateDistance = abs(distance[selectRoot]);
	//�ړ�����W�����߂�
	float moveEndX[2] = {
		this->m_RunnerDefault.ch.x + distance[selectRoot] ,					//�ړ�����WX
		this->m_RunnerDefault.ch.x + distance[selectRoot]					//�ړ�����WX
	};
	//�ړ��ʂ̃f�[�^�e�[�u��
	float lineMove[2] = {
		RUNNER_DEFAULT_MOVE_RIGHT ,			//�E�ړ�
		RUNNER_DEFAULT_MOVE_LEFT  ,			//���ړ�
	};
	//�ړ�����W����肷��
	//���g�̕��������̋������ړ���Ƃ���
	this->m_RunnerDefault.ch.ex = moveEndX[selectRoot];
	//�ړ��ʂ�ݒ�
	this->m_RunnerDefault.ch.mx = lineMove[selectRoot];

}


//*************************************************************************************************
//�����҂̍s���Q�̏������u�Ώۂ��n�[�h���Ȃ�W�����v���ĉ������v
//*************************************************************************************************
void   CCompetitor::SetCompetitorRunnerEvadeHardol(){

	
	//���n���Ȃ�W�����v
	if( this->m_RunnerDefault.actionF == RUNNER_ACTION_ROUND ){

		//���݂̃A�j���[�V�����̃R�}�������Z�b�g
		this->m_RunnerDefault.ani.cnt = 0;

		//���Ԃ����Z�b�g
		this->m_RunnerDefault.ani.time = 0;

		//�ő�R�}�����Z�b�g
		this->m_RunnerDefault.ani.max = RUNNER_ANIMATION_TYPE_JUMP_CNT_MAX;

		//�t���[�������Z�b�g
		this->m_RunnerDefault.ani.fram = RUNNER_ANIMATION_TYPE_JUMP_FRAM;


		//����t���O��ON�ɂ���
		this->m_RunnerDefault.avoidF |= RUNNER_WHILE_AVOID;

		//�W�����v�t���O�𗧂Ă�
		this->m_RunnerDefault.actionF |= RUNNER_ACTION_JUMP;

		//�A�j���[�V�����^�C�v���W�����v���ɕω�������
		this->m_RunnerDefault.ani.type |= RUNNER_ANIMATION_JUMP;		//�W�����v�̃A�j���[�V����
		
	
	}

}


//*************************************************************************************************
//�����҂̍s���R�̏������u�Ώۂ��U������v
//*************************************************************************************************
void   CCompetitor::SetCompetitorRunnerAttack(){

	
	//���n���Ȃ�U��
	if( this->m_RunnerDefault.actionF == RUNNER_ACTION_ROUND ){

		//���݂̃A�j���[�V�����̃R�}�������Z�b�g
		this->m_RunnerDefault.ani.cnt = 0;

		//���Ԃ����Z�b�g
		this->m_RunnerDefault.ani.time = 0;

		//�ő�R�}�����Z�b�g
		this->m_RunnerDefault.ani.max = RUNNER_ANIMATION_TYPE_ATTACK_CNT_MAX;

		//�t���[�������Z�b�g
		this->m_RunnerDefault.ani.fram = RUNNER_ANIMATION_TYPE_ATTACK_FRAM;


		//�U���t���O��
		this->m_RunnerDefault.actionF |= RUNNER_ACTION_ATTACK;

		//�A�j���[�V�����^�C�v���U���̓����
		this->m_RunnerDefault.ani.type |= RUNNER_ANIMATION_ATTACK;
	
	}

}


//*************************************************************************************************
//�����҂̍s���P�u�O���ɏ�Q��������Ή��Ɉړ��v
//*************************************************************************************************
void   CCompetitor::UpdateCompetitorRunnerLineMove(){


	//�ړ���ɒB������ړ����~�߂�
	if( this->m_RunnerDefault.avoidDistance >= this->m_RunnerDefault.designateDistance ){

		//�ړ��ʂ��O�ɂ���
		this->m_RunnerDefault.ch.mx = 0.0f;
			
		//���ړ��t���O��OFF
		this->m_RunnerDefault.actionF &= ~RUNNER_ACTION_LINE_MOVE;
		
		//����s���t���O��OFF�ɂ���
		this->m_RunnerDefault.avoidF &= ~RUNNER_WHILE_AVOID;

		//�擾������Q���̎�ނ����Z�b�g
		this->m_RunnerDefault.getType &= ~this->m_RunnerDefault.getType;

		//�X�V�I��
		return;
	}

	//�ړ���ɒB������܂ňړ����X�V
	if( this->m_RunnerDefault.avoidDistance <= this->m_RunnerDefault.designateDistance ){

		//�ړ��ʂ��X�V
		this->m_RunnerDefault.ch.x += this->m_RunnerDefault.ch.mx;

		//����ړ��������X�V
		this->m_RunnerDefault.avoidDistance += (int)( fabs( this->m_RunnerDefault.ch.mx ) );

	}

	//�L�����N�^�[�̉E�[�̍��W�����߂�
	float rightPosX = this->m_RunnerDefault.ch.x + ( this->m_RunnerDefault.ch.w * this->m_RunnerDefault.ch.scale );

	//�ړ��悪��ʒ[���z����Ȃ�ړ��ʂ𔽓]������
	//�E�[
	if( rightPosX >= STAGE_WIDTH ){

		//�ړ��ʂ𔽓]
		this->m_RunnerDefault.ch.mx = RUNNER_DEFAULT_MOVE_LEFT;
	}
	//���[
	if( this->m_RunnerDefault.ch.x <= 0 ){

		//�ړ��ʂ𔽓]
		this->m_RunnerDefault.ch.mx = RUNNER_DEFAULT_MOVE_RIGHT;
	}

}


//*************************************************************************************************
//�����҂̍s���Q�̍X�V�u�Ώۂ��n�[�h���Ȃ�W�����v���ĉ������v
//*************************************************************************************************
void   CCompetitor::UpdateCompetitorRunnerEvadeHardol(){
	
	//�ڒn��Ԃɖ߂��Ă�����W�����v�t���O���Ȃ���
	if( this->m_RunnerDefault.ani.type == RUNNER_ACTION_ROUND ){

		//�t���O��OFF
		this->m_RunnerDefault.actionF &= ~RUNNER_ACTION_JUMP;

		//����s���t���O��OFF�ɂ���
		this->m_RunnerDefault.avoidF &= ~RUNNER_WHILE_AVOID;

		//�擾������Q���̎�ނ����Z�b�g
		this->m_RunnerDefault.getType &= ~this->m_RunnerDefault.getType;

	}

}


//*************************************************************************************************
//�����҂̍s���R�̍X�V�u�Ώۂ��U������v
//*************************************************************************************************
void   CCompetitor::UpdateCompetitorRunnerAttack(){

	//�ڒn��Ԃɖ߂��Ă�����U���t���O���Ȃ���
	if( this->m_RunnerDefault.ani.type == RUNNER_ACTION_ROUND ){

		//�U���t���O��OFF
		this->m_RunnerDefault.actionF &= ~RUNNER_ACTION_ATTACK;

		//�擾������Q���̎�ނ����Z�b�g
		this->m_RunnerDefault.getType &= ~this->m_RunnerDefault.getType;
		
	}

}


//***********************************************************************************************
//�����̃L�����N�^�[�\���̂̍��G�͈͂𔻒�����
//***********************************************************************************************
bool	CCompetitor::BroadenCollisionSquareCompetitor( CHARACTER* pch ){


	//�߂�l�p
	bool ret = false;

	//�����̃L�����N�^�[�̍��G�͈͋�`�ƁA���g�̋�`��
	//��������
	if( BroadenCollisionSquare( &this->m_RunnerDefault.ch , pch ) ){


		//�Փ˂������Ƃ�Ԃ�
		ret = true;
	}

	//�ŏI�I�Ȕ����Ԃ�
	return ret;

}


//************************************************************************************************
//�����҃����i�[�̃A�j���[�V�������X�V
//************************************************************************************************
void   CCompetitor::ChangeCompetitorRunnerAnimation(){

	//�摜�f�[�^�e�[�u���iY���W�ŕω��j
	char animationTbl[ RUNNER_ANIMATION_MAX ] = {

		RUNNER_ANIMATION_ROUND	,		//�ڒn���
		RUNNER_ANIMATION_JUMP	,		//�W�����v��
		RUNNER_ANIMATION_ATTACK	,		//�U����
	
	};

	//���݂̃A�j���[�V�����̃^�C�v�ԍ��ɉ����ĉ摜��ω�������
	for( int i = 0 ; i < RUNNER_ANIMATION_MAX ; i++ ){

		//�A�j���[�V�����ɉ����ĉ摜����
		if( animationTbl[i] == this->m_RunnerDefault.ani.type ){

			//�\����Y���W�𔽉f����
			this->m_RunnerDefault.ch.direction = animationTbl[i];

			//�� �����҂̃A�j���[�V�����X�V
			if( UpdateAnimation( &this->m_RunnerDefault.ch , &this->m_RunnerDefault.ani ) == false ){

				//���݂̃A�j���[�V�����̃r�b�g�ڂ�0�ɂ���
				this->m_RunnerDefault.ani.type &= ~animationTbl[i];
				//�ʏ�̃A�j���[�V�����ɕύX
				this->m_RunnerDefault.ani.max = RUNNER_ANIMATION_TYPE_NORMAL_CNT_MAX;	//�ő�R�}��
				this->m_RunnerDefault.ani.fram = RUNNER_ANIMATION_TYPE_NORMAL_FRAM;		//�t���[����

			
			}

			break;

		}

	}

}

/*****************************************************************************************************
�����̒l�Ŋm���ōs�������߂�
�����P�F�m���@1�`100
�����Q�F�s�������A�N�V�����̎�ޔԍ�

�߂�l�F�A�N�V�����̎�ޔԍ�
�����s������-1��Ԃ�
******************************************************************************************************/
char	CCompetitor::ConmpetitorAttemptDoAction( char likelihood , BYTE actionType ){
	
	//�߂�l�p
	char ret = COMPETITOR_RUNNER_ACTION_TYPE_VACANT;

	//�����̒l�Ŋm�����v��
	if( GetLikelihood( likelihood ) ){
			
		//�m�����I�������̂ň����̒l�̃A�N�V�����̎�ޔԍ���Ԃ�
		ret = actionType;
	}
	
	return ret;

}



/*
//*************************************************************************************************
//�����҂̍s���Q�i����s���j
//*************************************************************************************************
void   CCompetitor::UpdateCompetitorRunnerAvoid(){

	//�Փ˂���ʂ�����
	BYTE bumpAction = OBSTACLE_NOT_BUMP;		//�����������ĂȂ����	
	
	//��Q���Ɖ��̔��������āA�ڐG����ʂɉ����Ĉړ��ʂ�ω�������
	//�Փ˖ʂ��󂯎��
	bumpAction = AttemptCollisionSquare( &this->m_RunnerDefault.ch );

	switch(bumpAction){
		
		case OBSTACLE_NOT_BUMP:

			break;

		case OBSTACLE_BUMP_TOP:

			this->m_RunnerDefault.ch.my = -5.0f;
			break;
			
		case OBSTACLE_BUMP_BOTTOM:

			this->m_RunnerDefault.ch.my = 5.0f;
			break;
			
		case OBSTACLE_BUMP_LEFT:

			this->m_RunnerDefault.ch.mx = -5.0f;
			break;
			
		case OBSTACLE_BUMP_RIGHT:

			this->m_RunnerDefault.ch.mx = 5.0f;
			break;
	}

}

*/
