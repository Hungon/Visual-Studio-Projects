#include	"CRoadEnemy.h"
#include	"CPlayerRunner.h"
#include	"CCompetitorManager.h"
#include	"CRunnerManager.h"
#include	"map.h"
#include	"sound.h"
#include	"play.h"
#include	<iostream>

using namespace std;


//�� �ÓI�����o�ϐ��̍Ē�`
const int CObstacle::ROADENEMY_OBSTACLE_MAX = 12;			//��Q���̏o������

//��ʃX�N���[���ɍ��킹�Ċg�傷���Q���̉�ʉ��[�ł̍ŏI�I��X���W�̂���
const float CObstacle::OBSTACLE_FITTING_SCROL_POSITION_X = 120.0f;

//�����ɕ��ׂ�ő吔
const int	CObstacle::OBSTACLE_HURDLE_LINE_MAX = 5;
//�����ɕ��ׂ�ۂ̔z��̎��
const int	CObstacle::OBSTACLE_HURDLE_LINE_ARRANGE_MAX = 5;

//�z��̎��
const char	CObstacle::OBSTACLE_HURDLE_ARRANGE_TYPE_STRAIGHT_LINE = 0;			//���꒼���ɓ����ǂ�
const char	CObstacle::OBSTACLE_HURDLE_ARRANGE_TYPE_STRAIGHT_RANDOM = 1;		//���꒼���Ƀ����_���ɔz�u
const char	CObstacle::OBSTACLE_HURDLE_KIND_OF_ARRANGE = 2;						//��ސ�

//�n�[�h���̔z�u�n�_���W
const float CObstacle::OBSTACLE_HURDLE_POSITION_START_X	= 130.0f;		//X
//�n�[�h���̊Ԋu
const float CObstacle::OBSTACLE_HURDLE_SPACE_X = 15.0f;					//X

//�e���Q���̕\���ݒ�
//��
const int CObstacle::ROADENEMY_OBSTACLE_HURDLE_SIZE_WIDTH	= 64;
const int CObstacle::ROADENEMY_OBSTACLE_ROCK_SIZE_WIDTH		= 64;
//����
const int CObstacle::ROADENEMY_OBSTACLE_HURDLE_SIZE_HEIGHT	= 32;
const int CObstacle::ROADENEMY_OBSTACLE_ROCK_SIZE_HEIGHT	= 64;
//�g�k�{��
const float CObstacle::ROADENEMY_OBSTACLE_HURDLE_SIZE_SCALE	= 1.0f;
const float CObstacle::ROADENEMY_OBSTACLE_ROCK_SIZE_SCALE	= 1.0f;
//�ړ���Y��Βl
const float CObstacle::ROADENEMY_OBSTACLE_HURDLE_ABSOLUTE_MOVE_Y = 1.0f;
const float CObstacle::ROADENEMY_OBSTACLE_ROCK_ABSOLUTE_MOVE_Y	 = 1.5f;
//�ړ���X��Βl
const float CObstacle::ROADENEMY_OBSTACLE_HURDLE_ABSOLUTE_MOVE_X = 0.5f;
const float CObstacle::ROADENEMY_OBSTACLE_ROCK_ABSOLUTE_MOVE_X	 = 0.5f;

//�e���Q���̃A�j���[�V�����ݒ�
//�t���[����
const int CObstacle::ROADENEMY_OBSTACLE_HURDLE_ANIMATION_FRAM = 5;
const int CObstacle::ROADENEMY_OBSTACLE_ROCK_ANIMATION_FRAM = 5;
//�R�}��
const int CObstacle::ROADENEMY_OBSTACLE_HURDLE_ANIMATION_CNT_MAX = 4;
const int CObstacle::ROADENEMY_OBSTACLE_ROCK_ANIMATION_CNT_MAX = 4;
//��Q���̃A�j���[�V�����t���O
const BYTE CObstacle::ROADENEMY_ANIMATION_FLAG_NORMAL = 0x00;	//�ʏ���
const BYTE CObstacle::ROADENEMY_ANIMATION_FLAG_BREAK  = 0x01;	//�j��t���O


//�Փ˂���ʂ�����
const char CObstacle::OBSTACLE_NOT_BUMP		=	-1;			//�Փ˂Ȃ�
const char CObstacle::OBSTACLE_BUMP_TOP		=	0x00;		//��
const char CObstacle::OBSTACLE_BUMP_BOTTOM	=	0x01;		//��
const char CObstacle::OBSTACLE_BUMP_LEFT	=	0x02;		//��
const char CObstacle::OBSTACLE_BUMP_RIGHT	=	0x03;		//�E


/*
*******************************************************************************************
�f�t�H���g�R���X�g���N�^
*******************************************************************************************
*/
CObstacle::CObstacle(){

}

/*
*******************************************************************************************
�R���X�g���N�^
*******************************************************************************************
*/
CObstacle::CObstacle( int max ){

	//���񐶐�����ő吔���擾
	this->m_ObstacleCreateMax = max;
	this->m_CreatedObstacleY = 999999.9f;

}

/*
*******************************************************************************************
�f�B�X�g���N�^
*******************************************************************************************
*/
CObstacle::~CObstacle(){

}

//*******************************************************************************************
//������
//*******************************************************************************************
void   CObstacle::InitRoadEnemy( CCompetitorManager	*lpCCompetitorManager , CPlayerRunner *lpCPlayerRunner ){

	//�Q�[����Փx���擾
	tagGAMELEVEL	gameLevel = GetGameLevel();

	//�e��n�[�h���̐����m��
	//��F�� �� �� �� �Ԃ̏�
	int	CreateRateTbl[ LEVEL_MAX ][ RUNNER_EFFECT_KIND_OF_TYPE ] = {
	
		{ 80 , 30 , 30 } ,			//EASY
		{ 80 , 15 , 10 } ,			//NORMAL
		{ 80 ,  5 ,  5 } ,			//HARD
	};
	//�e��n�[�h���̐�������ő吔
	//��F�� �� �� �� �΂̏�
	int	CreateCntMaxTbl[ LEVEL_MAX ][ RUNNER_EFFECT_KIND_OF_TYPE ] = {
	
		{ 3 , 3 , 2 } ,			//EASY
		{ 2 , 4 , 1 } ,			//NORMAL
		{ 1 , 5 , 1 } ,			//HARD
	};

	//��Փx�ɉ����ď�Q���̕\���Ԋu��ύX
	int	CreateSpaceYTbl[ LEVEL_MAX ] = { 400 , 380 , 360 };

	//�\���Ԋu����
	this->m_CreateObstacleSpaceY = CreateSpaceYTbl[ gameLevel ];

	//�� �e�탁�����̊m��
	//��Q���������\����
	this->m_lpRoadObstacle = new OBSTACLE [ this->m_ObstacleCreateMax ];
	//�e��0�ŏ�����
	ZeroMemory( this->m_lpRoadObstacle , sizeof( OBSTACLE ) * this->m_ObstacleCreateMax );
	//�e���Q���J�E���g�p
	this->m_lpObstacleCnt = new int [ ROADENEMY_KIND_OF_OBSTACLE ];
	//�e���Q�������J�E���g�p
	this->m_lpCreationOBstacleCnt = new int[ROADENEMY_KIND_OF_OBSTACLE];
	//�e��n�[�h���̏o���m��
	this->m_lpCreateRateToHurdle = new int [ RUNNER_EFFECT_KIND_OF_TYPE ];
	//�e��n�[�h���̃J�E���g�p
	this->m_lpCreateHurdleCnt = new int [ ROADENEMY_OBSTACLE_HURDLE_KIND_OF_COLOR ];
	//�e��n�[�h���̍ő吶����
	this->m_lpCreateHurdleCntMax = new int [ ROADENEMY_OBSTACLE_HURDLE_KIND_OF_COLOR ];
		
	//�� �e��n�[�h���ɑ΂����ݒ�
	//�e��n�[�h���̏o���m��
	for( int i = 0 ; i < RUNNER_EFFECT_KIND_OF_TYPE ; i++ ){
		this->m_lpCreateRateToHurdle[i] = CreateRateTbl[ gameLevel ][i];
	}
	//�e��n�[�h���̐����ő吔
	for( int i = 0 ; i < ROADENEMY_OBSTACLE_HURDLE_KIND_OF_COLOR ; i++ ){
		this->m_lpCreateHurdleCntMax[i] = CreateCntMaxTbl[ gameLevel ][i];
	}
	//�e���Q���J�E���g�p
	ZeroMemory( this->m_lpObstacleCnt , sizeof( int ) * ROADENEMY_KIND_OF_OBSTACLE );
	//�e�퐶��������Q���J�E���g�p
	ZeroMemory(this->m_lpCreationOBstacleCnt, sizeof(int) * ROADENEMY_KIND_OF_OBSTACLE);
	//�e��n�[�h���̃J�E���g�p
	ZeroMemory( this->m_lpCreateHurdleCnt , sizeof( int ) * ROADENEMY_OBSTACLE_HURDLE_KIND_OF_COLOR );

	//�e�탉���i�[�Ɗ֘A�t��
	//�v���C���[
	this->m_lpCPlayerRunner = lpCPlayerRunner;
	//�����ҊǗ��N���X
	this->m_lpCCompetitorManager = lpCCompetitorManager;

}

//*******************************************************************************************
//�X�V
//*******************************************************************************************
void   CObstacle::UpdateRoadEnemy(){


	//�v���C���[�̏����擾
	RUNNER	*lpRunner = this->m_lpCPlayerRunner->GetRunner();

	//�����_���̒l
	int obstacleType = RandomNum( ROADENEMY_OBSTACLE_TYPE_ROCK , ROADENEMY_OBSTACLE_TYPE_HURDLE );

	//��Q���J�E���g�p
	int	obstacleCnt[ ROADENEMY_KIND_OF_OBSTACLE ] = {0};

	//�e��n�[�h���J�E���g�p
	int	hurdleCnt[ ROADENEMY_OBSTACLE_HURDLE_KIND_OF_COLOR ] = {0};

	//�ݒ肵���\���Ԋu���ɏ�Q���𐶐�����
	if( ( this->m_CreatedObstacleY - this->m_CreateObstacleSpaceY ) > lpRunner->ch.y ){
		//�����������W���X�V
		this->m_CreatedObstacleY = lpRunner->ch.y;
		//�o���������ޔԍ��������_���œn��
		this->CreateObstacle( obstacleType );
	}

	//��Q���̍ő�o�����܂ŉ�
	for( int i = 0 ; i < this->m_ObstacleCreateMax ; i++ ){

		//��ʊO�ɏo����Q���̃t���O��false��
		if( CollisionCamera( &this->m_lpRoadObstacle[i].ch ) == false ){

			//�����t���O
			this->m_lpRoadObstacle[i].ch.flag = false;
			
			//��ޔԍ��������Ȃ���
			this->m_lpRoadObstacle[i].bitType = ROADENEMY_OBSTACLE_BIT_TYPE_VACANT;
			continue;

		}

		//false�Ȃ玟�̗v�f��
		if( this->m_lpRoadObstacle[i].ch.flag == false ) continue;
				
		//�e���Q���̐����J�E���g
		obstacleCnt[ this->m_lpRoadObstacle[i].ch.type ]++;
		//�e��n�[�h���̐����J�E���g
		if( this->m_lpRoadObstacle[i].ch.type == ROADENEMY_OBSTACLE_TYPE_HURDLE ) hurdleCnt[ this->m_lpRoadObstacle[i].colorType ]++;

		//���s���̍X�V�̕���
		switch( this->m_lpRoadObstacle[i].ch.type ){
			case ROADENEMY_OBSTACLE_TYPE_HURDLE :
				this->UpdateActionObstacleHurdle( i );
				break;
			case ROADENEMY_OBSTACLE_TYPE_ROCK :
				this->UpdateActionObstacleRock( i );
				break;
		}
		continue;
	
	}

	//��Q���̐����X�V
	for( int i = 0 ; i < ROADENEMY_KIND_OF_OBSTACLE ; i++ ){ this->m_lpObstacleCnt[i] = obstacleCnt[i]; }
	//�e��n�[�h���̐�
	for( int i = 0 ; i < ROADENEMY_OBSTACLE_HURDLE_KIND_OF_COLOR ; i++ ){ this->m_lpCreateHurdleCnt[i] = hurdleCnt[i]; }


}

//*******************************************************************************************
//�\��
//*******************************************************************************************
void   CObstacle::DrawRoadEnemy(){

	//�J�����̍��W���擾
	float px = GetCameraX();				//X
	float py = GetCameraY();				//Y

	char mes[255] = {0};

	char *lpHurdle[4] = {
		"��" ,
		"��" ,
		"��" ,
		"��" ,
	};
	char *lpObstacle[2] = {
		"�n�[�h��" ,
		"���" ,
	};
	TPOINT<int> pos = { 450 , 460 };
	
	//����Q��
	//�o�������鐔�������[�v����
	for( int i = 0 ; i < this->m_ObstacleCreateMax ; i++ ){

		//�����t���O��true�Ȃ�
		if( this->m_lpRoadObstacle[i].ch.flag ){

			//�g�k�\��
			DDScaleBlt(
				this->m_lpRoadObstacle[i].ch.x - px ,
				this->m_lpRoadObstacle[i].ch.y - py ,
				this->m_lpRoadObstacle[i].ch.w ,
				this->m_lpRoadObstacle[i].ch.h ,
				this->m_lpRoadEnemyImg[this->m_lpRoadObstacle[i].ch.bitnum] ,
				this->m_lpRoadObstacle[i].ch.picx ,
				this->m_lpRoadObstacle[i].ch.picy ,
				this->m_lpRoadObstacle[i].ch.scale);
		
		}
	
	}

/*
	//��Q���̍ő吔
	wsprintf( &mes[0] , "��Q���̍ő吔%d" , this->m_ObstacleCreateMax );
	DDTextOut( &mes[0] , pos.x , pos.y );

	//�o�����̊e��n�[�h���̐�
	for( int i = 0 ; i < ROADENEMY_OBSTACLE_HURDLE_KIND_OF_COLOR ; i++ ){
		wsprintf( &mes[0] , "�n�[�h��%s�̐�%d" , lpHurdle[i] , this->m_lpCreateHurdleCnt[i] );
		DDTextOut( &mes[0] , pos.x  , ( pos.y - 20 ) - ( i * 20) );
	}
	//��Q���̐�
	for( int i = 0 ; i < ROADENEMY_KIND_OF_OBSTACLE ; i++ ){
		wsprintf( &mes[0] , "%s�̐�%d" , lpObstacle[i] , this->m_lpObstacleCnt[i] );
		DDTextOut( &mes[0] , pos.x  , ( pos.y - 100 ) - ( i * 20) );
	}

*/

}

//*******************************************************************************************
//���
//*******************************************************************************************
void   CObstacle::ReleaseRoadEnemy(){

	
	//�������̉��
	if( this->m_lpRoadEnemyImg != NULL ){
		delete []this->m_lpRoadEnemyImg;		//�摜
		this->m_lpRoadEnemyImg = NULL;
	}
	if( this->m_lpRoadEnemyEffectSound != NULL ){
		delete []this->m_lpRoadEnemyEffectSound;//���ʉ�
		this->m_lpRoadEnemyEffectSound = NULL;
	}

	if( this->m_lpRoadObstacle != NULL ){
		delete []this->m_lpRoadObstacle;		//��Q��
		this->m_lpRoadObstacle = NULL;
	}
	if( this->m_lpCreateRateToHurdle != NULL ){
		delete []this->m_lpCreateRateToHurdle;	//�e��n�[�h���̏o���m��
		this->m_lpCreateRateToHurdle = NULL;
	}
	if( this->m_lpCreateHurdleCnt != NULL ){
		delete []this->m_lpCreateHurdleCnt;		//�e��n�[�h���̃J�E���g�p
		this->m_lpCreateHurdleCnt = NULL;
	}
	if( this->m_lpCreateHurdleCntMax != NULL ){
		delete []this->m_lpCreateHurdleCntMax;	//�e��n�[�h���̍ő吶����
		this->m_lpCreateHurdleCntMax = NULL;
	}
	if( this->m_lpObstacleCnt != NULL ){
		delete []this->m_lpObstacleCnt;			//�e���Q���J�E���g�p
		this->m_lpObstacleCnt = NULL;
	}
	delete []this->m_lpCreationOBstacleCnt;		//�e���Q���������J�E���g�p
	this->m_lpCreationOBstacleCnt = NULL;
}

//*******************************************************************************************
//��Q���̐���
//*******************************************************************************************
void   CObstacle::CreateObstacle( int type ){

	//�v���C���[�̌��ʂ𔭊�������Q���̗v�f���擾
	RUNNER	*lpRunner = this->m_lpCPlayerRunner->GetRunner();

	//�����҂̍ő吔���擾
	int	compeMax = this->m_lpCCompetitorManager->GetCompetitorMax();


	//���꒼���ɔz�u����ۂ̃����_���l���擾����
	this->m_ObstacleRandomArrangeMax = RandomNum( ( CObstacle::OBSTACLE_HURDLE_LINE_ARRANGE_MAX - 1 ) , 0 );

	//����̃n�[�h���̔z��̎�ނ̗v�f
	this->m_ObstacleInitArrangeRandomType = RandomNum( CObstacle::OBSTACLE_HURDLE_ARRANGE_TYPE_STRAIGHT_RANDOM , 
		CObstacle::OBSTACLE_HURDLE_ARRANGE_TYPE_STRAIGHT_LINE );

	//��Q���̃J�E���g�����Z�b�g
	this->m_lpCreationOBstacleCnt[0] = 0;				//�n�[�h��
	this->m_lpCreationOBstacleCnt[1] = 0;				//���

	//�ő�\�����܂Ń��[�v����
	for( int i = 0 ; i < this->m_ObstacleCreateMax  ; i++ ){

		//�����t���O��false�Ȃ�
		if( this->m_lpRoadObstacle[i].ch.flag == false ){
			//�L�����N�^�[�Ɍ��ʂ𔭊������v�f���Ȃ�A�L�����N�^�[���擾������Q���̗v�f�����Z�b�g����
			//�v���C���[
			if( i == lpRunner->obstacleBumpNow ) lpRunner->obstacleBumpNow = -1;
			//������
			for( int j = 0 ; j < compeMax ; j++ ){
				if( i == *( this->m_lpCCompetitorManager->GetCompetitorManagerObstacleMaterial() + j ) ){
					this->m_lpCCompetitorManager->SetCompetitorManagerObstacleMaterial( -1 , j );
				}
			}

			//���@���ʂ̐ݒ�
			this->m_lpRoadObstacle[i].ch.type = type;										//��ޔԍ�
			this->m_lpRoadObstacle[i].ch.flag = true;										//�����t���O
					
			//���@�e���Q���̏�����
			switch( this->m_lpRoadObstacle[i].ch.type ){
				case ROADENEMY_OBSTACLE_TYPE_HURDLE :
					//���������Ȃ�return
					if( this->InitObstacleHurdle( i ) ) return;
					break;
				case ROADENEMY_OBSTACLE_TYPE_ROCK :
					if( this->InitObstacleRock( i ) ) return;
					break;
			}
		}
	}
}


//********************************************************************************************
//��Q��1�u�n�[�h���v�̍s���̏�����
//********************************************************************************************
bool	CObstacle::InitObstacleHurdle( int material ){

	//�v���C���[�̗̑͂ɉ�������������
	RUNNER	*lpRunner = this->m_lpCPlayerRunner->GetRunner();

	//�o��������n�[�h���̎�ނ��܂Ƃ߂��f�[�^�e�[�u��
	//�ԂƗ΂���D��I�ɏo�₷������z�u
	tagHURDLECOLORTYPE hurdleTypeTbl[ RUNNER_EFFECT_KIND_OF_TYPE ] = {

		ROADENEMY_OBSTACLE_HURDLE_COLOR_BLUE	,		//��
		ROADENEMY_OBSTACLE_HURDLE_COLOR_GREEN ,			//��
		ROADENEMY_OBSTACLE_HURDLE_COLOR_RED	,			//��	
	};

	//�e��n�[�h���̐����m��
	//�����Y�����Ȃ���΍����n�[�h�����o��
	int hurdleType = ROADENEMY_OBSTACLE_HURDLE_COLOR_BLACK;

	//�e��n�[�h�����o���m���̉ϒl
	int variableType[ RUNNER_EFFECT_KIND_OF_TYPE ] = {0};

	//�v���C���[�̗̑͂������Ă�����A�Ԃ̃n�[�h���Ɨ΂̃n�[�h�����o���m�����グ��
	switch( lpRunner->ch.hp ){
		case 1 :
			variableType[2] = 10;
			variableType[1] = 60;
			break;
		case 2 :
			variableType[2] = 5;
			variableType[1] = 40;
			break;

	}

	//���������n�[�h���̎�ނ��e��ɐݒ肵���m���ɉ����Č���
	for( int i = 0 ; i < RUNNER_EFFECT_KIND_OF_TYPE ; i++ ){
		//��������^�C�v�������邩���ׂ�
		if( GetLikelihood( this->m_lpCreateRateToHurdle[i] + variableType[i] ) ) hurdleType = hurdleTypeTbl[i]; continue;
	}

	//���@�e��ݒ�
	this->m_lpRoadObstacle[material].ch.w = CObstacle::ROADENEMY_OBSTACLE_HURDLE_SIZE_WIDTH;			//��
	this->m_lpRoadObstacle[material].ch.h = CObstacle::ROADENEMY_OBSTACLE_HURDLE_SIZE_HEIGHT;			//����
	this->m_lpRoadObstacle[material].ch.bitnum = ROADENEMY_OBSTACLE_TYPE_HURDLE;						//�\���v�f�ԍ�
	this->m_lpRoadObstacle[material].ch.scale = CObstacle::ROADENEMY_OBSTACLE_HURDLE_SIZE_SCALE;		//�g�k�{��
	this->m_lpRoadObstacle[material].ch.defaultScale = CObstacle::ROADENEMY_OBSTACLE_HURDLE_SIZE_SCALE;	//�g�k�{�������l
	this->m_lpRoadObstacle[material].bitType = ROADENEMY_OBSTACLE_BIT_TYPE_HURDLE;						//�L�����N�^�[�p��ޔԍ�

	//�z�񂪏I��������true���󂯎���Đ����I������
	bool ret = false;

	//�z�u�����̕���
	switch( this->m_ObstacleInitArrangeRandomType ){
		//�߂�l�ɔz�u�����̃t���O���󂯎��
		case CObstacle::OBSTACLE_HURDLE_ARRANGE_TYPE_STRAIGHT_LINE :
			ret = this->InitArrangeHurdleStraightLine( material );
			break;
		case CObstacle::OBSTACLE_HURDLE_ARRANGE_TYPE_STRAIGHT_RANDOM :
			ret = this->InitArrangeHurdleRandomStraightLine( material );
			break;
	}
		

	//��Q���̏o�����W���擾
	this->m_lpRoadObstacle[material].ch.sx = this->m_lpRoadObstacle[material].ch.x;									//X

	//���@���x����
	this->m_lpRoadObstacle[material].ch.speed = CObstacle::ROADENEMY_OBSTACLE_HURDLE_ABSOLUTE_MOVE_Y;//���x�{��
	
	//���@�A�j���[�V�����ݒ�
	this->m_lpRoadObstacle[material].ani.fram = CObstacle::ROADENEMY_OBSTACLE_HURDLE_ANIMATION_FRAM;	//�t���[����
	this->m_lpRoadObstacle[material].ani.max = CObstacle::ROADENEMY_OBSTACLE_HURDLE_ANIMATION_CNT_MAX;	//�ő�R�}��

	//�o���������ނ������_���őI��
	this->m_lpRoadObstacle[material].colorType = hurdleType;

	//�\�������WY�Ɏ�ނɉ��������W����
	this->m_lpRoadObstacle[material].ch.picy = this->m_lpRoadObstacle[material].colorType * this->m_lpRoadObstacle[material].ch.h;
	this->m_lpRoadObstacle[material].ch.picx = 0;

	//�A�j���[�V�������̕\����Y���W���擾
	this->m_lpRoadObstacle[material].ch.direction = this->m_lpRoadObstacle[material].colorType;

	//�ݒ萔�܂ŏ��������s��
	return ret;

}


//********************************************************************************************
//��Q��1�u�n�[�h���v�z�u�̏�����
//���꒼���ɔz�u����
//********************************************************************************************
bool	CObstacle::InitArrangeHurdleStraightLine( int material ){

	//�J�������W���擾
	float comeCameraX = GetCameraX();						//X
	float comeCameraY = GetCameraY();						//Y

	//�o�����W
	//�n�[�h���͓��H�̓����ɂ����o��������
	char posX = this->m_lpCreationOBstacleCnt[0];			//X

	//�\����Y
	char posY = 3;

	//�o�������ĉ�ʃX�N���[���ɍ��킹�����W�ύX�p�f�[�^�e�[�u��
	float fitScrolX[ 1 ][ CObstacle::OBSTACLE_HURDLE_LINE_MAX ] = {
		
		{ CObstacle::OBSTACLE_FITTING_SCROL_POSITION_X , 
		CObstacle::OBSTACLE_FITTING_SCROL_POSITION_X - 50.0f ,
		CObstacle::OBSTACLE_FITTING_SCROL_POSITION_X ,
		CObstacle::OBSTACLE_FITTING_SCROL_POSITION_X - 50.0f , 
		CObstacle::OBSTACLE_FITTING_SCROL_POSITION_X } ,
	};

	//�o���Ԋu
	float spaceX[ 1 ][ CObstacle::OBSTACLE_HURDLE_LINE_MAX ] = {
	
		{ 0 , CObstacle::OBSTACLE_HURDLE_SPACE_X ,										//������荶��
		CObstacle::OBSTACLE_HURDLE_SPACE_X ,											//����
		CObstacle::OBSTACLE_HURDLE_SPACE_X , CObstacle::OBSTACLE_HURDLE_SPACE_X } ,		//�������E��
	
	};
	

	//�� �z�u
	this->m_lpRoadObstacle[material].ch.x = CObstacle::OBSTACLE_HURDLE_POSITION_START_X  + 
		(float)( ( this->m_lpRoadObstacle[material].ch.w * this->m_lpRoadObstacle[material].ch.scale + spaceX[0][posX] ) * posX );	//�\����X
	this->m_lpRoadObstacle[material].ch.y = (float)( comeCameraY - this->m_lpRoadObstacle[material].ch.h * posY );					//�\����Y

	//��Q���̉�ʈʒu�ɉ�����X���̉ϒl���擾
	this->m_lpRoadObstacle[material].fitPos.fX = fitScrolX[0][posX];

	//�o�������J�E���g�A�b�v
	this->m_lpCreationOBstacleCnt[0]++;

	//���꒼���ɕ��ׂ���return
	if (this->m_lpCreationOBstacleCnt[0] >= CObstacle::OBSTACLE_HURDLE_LINE_MAX) return true;


	//�ݒ萔�܂ŏ��������s��
	return false;

}

/**************************************************************************************************
�n�[�h���̔z�u�̏�����
���꒼���Ƀ����_���ŕ��ׂ�
***************************************************************************************************
*/
bool	CObstacle::InitArrangeHurdleRandomStraightLine( int material ){
	
	//�J�������W���擾
	float comeCameraX = GetCameraX();						//X
	float comeCameraY = GetCameraY();						//Y

	//�o�����W
	//�����_���Ŕz�u�����߂�
	char posXTbl[ CObstacle::OBSTACLE_HURDLE_LINE_ARRANGE_MAX ][ 4 ] = {

		{ 0 , 2 , 4 } ,		 //�E�[�A�����A���[�̔z�u
		{ 1 , 2 , 3 } ,		 //�[�����󂯂��z�u
		{ 1 , 2 , 3 , 4 } ,  //���[�����󂯂��z�u
		{ 0 , 1 , 2 , 3 } ,	 //�E�[�����󂯂��z�u
		{ 0 , 1 , 3 , 4 } ,  //�^�񒆂����󂯂��z�u
	
	};

	//�o����
	char popMaxTbl[ CObstacle::OBSTACLE_HURDLE_LINE_ARRANGE_MAX ] = { 3 , 3 , 4 , 4 , 4 };

	//�o���Ԋu
	float spaceX[ CObstacle::OBSTACLE_HURDLE_LINE_MAX ] = {
	
		 0 , CObstacle::OBSTACLE_HURDLE_SPACE_X ,										//������荶��
		CObstacle::OBSTACLE_HURDLE_SPACE_X ,											//����
		CObstacle::OBSTACLE_HURDLE_SPACE_X , CObstacle::OBSTACLE_HURDLE_SPACE_X			//�������E��
	
	};

	//�\����Y
	char posY = 3;

	//�o�������ĉ�ʃX�N���[���ɍ��킹�����W�ύX�p�f�[�^�e�[�u��
	float fitScrolX[ 1 ][ CObstacle::OBSTACLE_HURDLE_LINE_MAX ] = {
		
		{ CObstacle::OBSTACLE_FITTING_SCROL_POSITION_X , 
		CObstacle::OBSTACLE_FITTING_SCROL_POSITION_X - 50.0f ,
		CObstacle::OBSTACLE_FITTING_SCROL_POSITION_X ,
		CObstacle::OBSTACLE_FITTING_SCROL_POSITION_X - 50.0f ,
		CObstacle::OBSTACLE_FITTING_SCROL_POSITION_X } ,
	};

	//�� �z�u
	this->m_lpRoadObstacle[material].ch.x = CObstacle::OBSTACLE_HURDLE_POSITION_START_X  + 
		(float)( ( this->m_lpRoadObstacle[material].ch.w * this->m_lpRoadObstacle[material].ch.scale + 
		spaceX[ posXTbl[ this->m_ObstacleRandomArrangeMax ][this->m_lpCreationOBstacleCnt[0]] ] ) *
		posXTbl[ this->m_ObstacleRandomArrangeMax ][this->m_lpCreationOBstacleCnt[0]] );							//�\����X

	this->m_lpRoadObstacle[material].ch.y = (float)( comeCameraY - this->m_lpRoadObstacle[material].ch.h * posY );	//�\����Y

	//��Q���̉�ʈʒu�ɉ�����X���̉ϒl���擾
	this->m_lpRoadObstacle[material].fitPos.fX = fitScrolX[ 0 ][ posXTbl [ this->m_ObstacleRandomArrangeMax ]
	[this->m_lpCreationOBstacleCnt[0]] ];

	//�o�������J�E���g�A�b�v
	this->m_lpCreationOBstacleCnt[0]++;

	//���̐�����ׂ���return
	if (this->m_lpCreationOBstacleCnt[0] >= popMaxTbl[this->m_ObstacleRandomArrangeMax]) return true;

	//�ݒ萔�܂ŏ��������s��
	return false;

}

//********************************************************************************************
//��Q��1�u�n�[�h���v�̍s���̍X�V
//********************************************************************************************
void   CObstacle::UpdateActionObstacleHurdle( int material ){

	//�J�����̍��W���擾
	float comeCameraY = GetCameraY();

	//��Q���̃��[�J�����W�����߂�
	float localCameraY = this->m_lpRoadObstacle[material].ch.y - comeCameraY;


	//�j��t���O�������Ă�����A�j���[�V����������
	if( this->m_lpRoadObstacle[material].ani.type == CObstacle::ROADENEMY_ANIMATION_FLAG_BREAK ){

		//�j��t���O���A���A�Ō�̃A�j���[�V�����܂ŃR�}��������t���O������
		if( UpdateAnimation( &this->m_lpRoadObstacle[material].ch , &this->m_lpRoadObstacle[material].ani ) == false ){

			//�����t���O������
			this->m_lpRoadObstacle[material].ch.flag = false;

			//�j��t���O��ʏ�ɖ߂�
			this->m_lpRoadObstacle[material].ani.type &= ~CObstacle::ROADENEMY_ANIMATION_FLAG_BREAK;
			return;
		
		}
	}

	
	//���@��O�ɗ���قǂɑ傫�������鏈��
	FittingPositionToBackCameraAngle( &this->m_lpRoadObstacle[material].ch , 380.0f / 640.0f );

	//��Q���̃��[�J�����W���c��ʃT�C�Y�́A
	//�ǂ̊����Ɉʒu���Ă��邩���v�Z
	float localRateY = localCameraY / 480.0f;


	//���ړ�����

	//�z�u�̒����iposX = 2�j�Ɉʒu����_�ɍ��E�ɉ����ĉ�ʃX�N���[���ɍ��킹�Ĉʒu�����炵�Ă���
	//��ʒ������獶�ɔz�u����p�x
	if( this->m_lpRoadObstacle[material].ch.sx < ( 320 - this->m_lpRoadObstacle[material].ch.w / 2 ) ){

		//�n�_���W�����Ɍ��݂�Y���W�ɑ΂��Ĉʒu�����炷
		this->m_lpRoadObstacle[material].ch.x = this->m_lpRoadObstacle[material].ch.sx - ( this->m_lpRoadObstacle[material].fitPos.fX * localRateY );
	}

	//�������E�ɔz�u���ꂽ�ꍇ
	if( this->m_lpRoadObstacle[material].ch.sx > ( 320 + this->m_lpRoadObstacle[material].ch.w / 2 ) ){

		//�n�_���W�����Ɍ��݂�Y���W�ɑ΂��Ĉʒu�����炷
		this->m_lpRoadObstacle[material].ch.x = this->m_lpRoadObstacle[material].ch.sx + ( this->m_lpRoadObstacle[material].fitPos.fX * localRateY );	
	}

}


//********************************************************************************************
//��Q��2�u��΁v�̍s���̏�����
//********************************************************************************************
bool	CObstacle::InitObstacleRock( int material ){

	//�ړ���X������
	float direction[2] = {
		( -1 * CObstacle::ROADENEMY_OBSTACLE_ROCK_ABSOLUTE_MOVE_X ) , CObstacle::ROADENEMY_OBSTACLE_ROCK_ABSOLUTE_MOVE_X
	};
	//�\�����W
	char posX = RandomNum( 9 , 0 );		//X
	char posY = RandomNum( 5 , 1 );		//Y

	//���@�e��ݒ�
	this->m_lpRoadObstacle[material].ch.w = CObstacle::ROADENEMY_OBSTACLE_ROCK_SIZE_WIDTH;			//��
	this->m_lpRoadObstacle[material].ch.h = CObstacle::ROADENEMY_OBSTACLE_ROCK_SIZE_HEIGHT;			//����
	this->m_lpRoadObstacle[material].ch.x = (float)( GetCameraX() + this->m_lpRoadObstacle[material].ch.w * posX );				//�\����X
	this->m_lpRoadObstacle[material].ch.y = (float)( GetCameraY() - this->m_lpRoadObstacle[material].ch.h * posY );				//�\����Y
	this->m_lpRoadObstacle[material].ch.bitnum = ROADENEMY_OBSTACLE_TYPE_ROCK;						//�\���v�f�ԍ�
	this->m_lpRoadObstacle[material].ch.scale = CObstacle::ROADENEMY_OBSTACLE_ROCK_SIZE_SCALE;		//���݂̊g�k�{��
	this->m_lpRoadObstacle[material].ch.defaultScale = CObstacle::ROADENEMY_OBSTACLE_ROCK_SIZE_SCALE;//�g�k�{�������l
	this->m_lpRoadObstacle[material].bitType = ROADENEMY_OBSTACLE_BIT_TYPE_ROCK;					//�L�����N�^�[�p��ޔԍ�
	
	//�������������J�E���g
	this->m_lpCreationOBstacleCnt[1]++;

	//���@���x����
	this->m_lpRoadObstacle[material].ch.mx = direction[ RandomNum( 1 , 0 ) ];						//X�ړ���
	this->m_lpRoadObstacle[material].ch.my = CObstacle::ROADENEMY_OBSTACLE_ROCK_ABSOLUTE_MOVE_Y;	//Y�ړ���
	this->m_lpRoadObstacle[material].ch.speed = 1.0f;												//���x�{��
	
	//���@�A�j���[�V�����ݒ�
	this->m_lpRoadObstacle[material].ani.fram = CObstacle::ROADENEMY_OBSTACLE_ROCK_ANIMATION_FRAM;		//�t���[����
	this->m_lpRoadObstacle[material].ani.max = CObstacle::ROADENEMY_OBSTACLE_ROCK_ANIMATION_CNT_MAX;	//�ő�R�}��

	//�₪�]���鉹
	PlaySound( this->m_lpRoadEnemyEffectSound[0] , 0 , false );

	//�\����
	this->m_lpRoadObstacle[material].ch.picx = 0;

	//��΂����̐���������Ă�����return
	if (this->m_lpCreationOBstacleCnt[1] >= (this->m_ObstacleCreateMax - (CObstacle::OBSTACLE_HURDLE_LINE_MAX + 2))) return true;

	//�ݒ萔�܂ŏ��������s��
	return false;

}

//********************************************************************************************
//��Q��2�u��΁v�̍s���̍X�V
//********************************************************************************************
void   CObstacle::UpdateActionObstacleRock( int material ){

	//�J�����̍��W���擾
	float comeCameraY = GetCameraY();

	//��Q���̃��[�J�����W�����߂�
	float localCameraY = this->m_lpRoadObstacle[material].ch.y - comeCameraY;

	//�A�j���[�V�����̃f�[�^�e�[�u��
	char obstacleAnimationTbl[2] = {
	
		CObstacle::ROADENEMY_ANIMATION_FLAG_NORMAL ,		//�ʏ펞
		CObstacle::ROADENEMY_ANIMATION_FLAG_BREAK  ,		//�j��
	
	};

	//�j��t���O�������Ă�����A�j���[�V������ω�������
	if( this->m_lpRoadObstacle[material].ani.type == obstacleAnimationTbl[this->m_lpRoadObstacle[material].ani.type] ){

		//�\�������W��ς���
		this->m_lpRoadObstacle[material].ch.direction = obstacleAnimationTbl[this->m_lpRoadObstacle[material].ani.type];

		//�j��t���O���A���A�Ō�̃A�j���[�V�����܂ŃR�}��������t���O������
		if( UpdateAnimation( &this->m_lpRoadObstacle[material].ch , &this->m_lpRoadObstacle[material].ani ) == false && 
			this->m_lpRoadObstacle[material].ani.type == obstacleAnimationTbl[1] ){

			//�����t���O������
			this->m_lpRoadObstacle[material].ch.flag = false;

			//�j��t���O��ʏ�ɖ߂�
			this->m_lpRoadObstacle[material].ani.type &= ~obstacleAnimationTbl[1];
			return;
		
		}
	}

	//���@��O�ɗ���قǂɑ傫�������鏈��
	FittingPositionToBackCameraAngle( &this->m_lpRoadObstacle[material].ch , 380.0f / 640.0f );

	//���@�ړ��ʂ��X�V
	this->m_lpRoadObstacle[material].ch.x += this->m_lpRoadObstacle[material].ch.mx * this->m_lpRoadObstacle[material].ch.speed;			//X
	this->m_lpRoadObstacle[material].ch.y += this->m_lpRoadObstacle[material].ch.my * this->m_lpRoadObstacle[material].ch.speed;			//Y
}

/*
***********************************************************************************************
�����̃L�����N�^�[�\���̂̍��G�͈͂Ŕ�������
����   : �L�����N�^�[�\����
�߂�l : ��Q���̎�ޔԍ���Ԃ�
***********************************************************************************************
*/
BYTE	CObstacle::BroadenCollisionSquareObstacle( CHARACTER* pch ){

	//�߂�l�p
	BYTE ret = ROADENEMY_OBSTACLE_BIT_TYPE_VACANT;			//��Q���Ȃ�

	//��Q���̐������܂킷
	for( int i = 0 ; i < this->m_ObstacleCreateMax ; i++ ){

		//�����̃L�����N�^�[���L���Ă�����G�͈͂Ɣ�������
		if( BroadenCollisionSquare( &this->m_lpRoadObstacle[i].ch , pch ) ){

			//�Փ˂����Q���̎�ނ̃r�b�g�ڂ�Ԃ�
			ret |= this->m_lpRoadObstacle[i].bitType;
			
			break;
		}
	}

	//�ŏI�I�Ȕ����Ԃ�
	return ret;

}


//************************************************************************************************
//��Q���Ɖ��̓����蔻������@AttempCollisionSquare
//************************************************************************************************
BYTE	CObstacle::AttemptCollisionSquare( CHARACTER* pch1 ){


	//��Q���̂ǂ̖ʂƓ�����������Ԃ�
	BYTE bump = CObstacle::OBSTACLE_NOT_BUMP;

	//�L�����N�^�[��1�t���[�����̈ړ��ʂ����߂Ĉړ���̍��W�����߂�
	float moveX = pch1->x + ( pch1->mx * pch1->speed );								//X
	float moveY = pch1->y + ( pch1->my * pch1->speed );								//Y

	//���������g�k�{���ʂ�Ɍv�Z
	//�L�����N�^�[
	float charaRectW = pch1->w * pch1->scale;										//��
	float charaRectH = pch1->h * pch1->scale;										//����


	//����ʏ�Ɏc���Ă����Q���Ɣ�������
	for( int i = 0 ; i < this->m_ObstacleCreateMax ; i++ ){

		if( this->m_lpRoadObstacle[i].ch.flag ){

			//��Q���̕��������v�Z
			float obstaRectW = this->m_lpRoadObstacle[i].ch.w * this->m_lpRoadObstacle[i].ch.scale;//��
			float obstaRectH = this->m_lpRoadObstacle[i].ch.h * this->m_lpRoadObstacle[i].ch.scale;//����

			//pch1�̍��[�Ə�Q���̉E�[�̋�`�Ŕ���
			if( moveX < this->m_lpRoadObstacle[i].ch.x + obstaRectW &&
				moveX > this->m_lpRoadObstacle[i].ch.x &&
				moveY < this->m_lpRoadObstacle[i].ch.y + obstaRectH &&
				moveY + charaRectH > this->m_lpRoadObstacle[i].ch.y ){
				
				//�߂�l����
				bump = CObstacle::OBSTACLE_BUMP_RIGHT;											//�E
				break;

			}				

			//pch1�̏�[�Ə�Q���̉��[�̋�`�Ŕ���
			if( moveX < this->m_lpRoadObstacle[i].ch.x + obstaRectW &&
				moveX + charaRectW > this->m_lpRoadObstacle[i].ch.x &&
				moveY < this->m_lpRoadObstacle[i].ch.y + obstaRectH &&
				moveY > this->m_lpRoadObstacle[i].ch.y ){
				
				//�߂�l����
				bump = CObstacle::OBSTACLE_BUMP_BOTTOM;										//��
				break;
			}

			//pch1�̉E�[�Ə�Q���̍��[�̋�`�Ŕ���
			if( moveX + charaRectW < this->m_lpRoadObstacle[i].ch.x + obstaRectW &&
				moveX + charaRectW > this->m_lpRoadObstacle[i].ch.x &&
				moveY < this->m_lpRoadObstacle[i].ch.y + obstaRectH &&
				moveY + charaRectH > this->m_lpRoadObstacle[i].ch.y ){
				
				//�߂�l����
				bump = CObstacle::OBSTACLE_BUMP_LEFT;											//��
				break;
			}

			//pch1�̉��[�Ə�Q���̏�[�̋�`�Ŕ���
			if( moveX < this->m_lpRoadObstacle[i].ch.x + obstaRectW &&
				moveX + charaRectW > this->m_lpRoadObstacle[i].ch.x &&
				moveY + charaRectH < this->m_lpRoadObstacle[i].ch.y + obstaRectH &&
				moveY + charaRectH > this->m_lpRoadObstacle[i].ch.y ){

				//�߂�l����
				bump = CObstacle::OBSTACLE_BUMP_TOP;											//��
				break;
			}
		}
	}

	//�ŏI�I�ɂǂ��̖ʂƐڐG���邩��Ԃ�
	return bump;

}


//************************************************************************************************
//�摜�̗v�f��ݒ肷��
//************************************************************************************************
void	CObstacle::SetRoadEnemyImage( int* lpImage , int max ){

	//�����̍ő吔�����摜�̗v�f���m�ۂ���
	this->m_lpRoadEnemyImg = new int [ max ];

	//�����̍ő吔�����摜�̗v�f��ݒ肷��
	for( int i = 0 ; i < max ; i++ ){ this->m_lpRoadEnemyImg[i] = lpImage[i]; }
	
}

/*
****************************************************************************************************
���ʉ��̗v�f��ݒ肷��
****************************************************************************************************
*/
void	CObstacle::SetRoadEnemyEffectSound( int *lpSound , int max ){

	//�����̐������������m��
	this->m_lpRoadEnemyEffectSound = new int [ max ];
	//���ʉ��̗v�f�𐶐�
	for( int i = 0 ; i < max ; i++ ){ this->m_lpRoadEnemyEffectSound[i] = lpSound[i]; }
}

//*******************************************************************************************
//�����蔻��
//*******************************************************************************************
char	CObstacle::CollisionRoadEnemy(RUNNER* pRunner) {

	//�߂�l�p
	char ret = RUNNER_NOT_BUMP;				//�Փ˂Ȃ�

	//��Q���̐�������
	for (int i = 0; i < this->m_ObstacleCreateMax; i++) {

		//�����t���O��true�A���A�j��t���O�������Ă��Ȃ������画������
		if (this->m_lpRoadObstacle[i].ch.flag &&
			this->m_lpRoadObstacle[i].ani.type == CObstacle::ROADENEMY_ANIMATION_FLAG_NORMAL) {

			//�����̃L�����N�^�[�ƏՓ˂����Q���Ƃ̔���������A�����i�[�̋�`�ɋ򂢍��܂��Ĕ��肷��
			//���̍ۂɁA�����i�[�̑O���̏Փ�
			if (this->m_lpCPlayerRunner->CollisionRunner(&pRunner->ch, &this->m_lpRoadObstacle[i].ch)) {

				//initialize obstacles when player bump to obstacle
				switch (this->m_lpRoadObstacle[i].ch.type) {
				case ROADENEMY_OBSTACLE_TYPE_HURDLE:
					ret = this->CollisionInitializeHurdle(pRunner, i);				//initialize hurdle
					if(ret != RUNNNER_BUMP_TO_HURDLE) return ret;					//for breaking hurdle process
					break;
				case ROADENEMY_OBSTACLE_TYPE_ROCK:
					ret = this->CollisionInitializeRock(pRunner);					//initialize rock
					break;
				}
				//�L�����N�^�[���U�������G��ԂȂ�j��t���O�𗧂Ă�
				if (pRunner->actionF == RUNNER_ACTION_ATTACK ||
					pRunner->effectType == RUNNER_EFFECT_TYPE_ABSOLUTE ||
					pRunner->effectType == RUNNER_EFFECT_TYPE_SPEED_AND_ABSOLUTE) {
					//���G��ԂȂ�J�E���g���Ȃ�
					if (pRunner->effectType != RUNNER_EFFECT_TYPE_ABSOLUTE ||
						pRunner->effectType != RUNNER_EFFECT_TYPE_SPEED_AND_ABSOLUTE) {
						//�j�󐔂��J�E���g
						pRunner->defeatCnt[this->m_lpRoadObstacle[i].ch.type]++;
					}
					PlaySound(this->m_lpRoadEnemyEffectSound[1], 0, false);			//destroyed obstacle sound
					//�j��t���O�𗧂Ă�
					this->m_lpRoadObstacle[i].ani.type |= CObstacle::ROADENEMY_ANIMATION_FLAG_BREAK;
					//�A�j���[�V�����ݒ�
					this->m_lpRoadObstacle[i].ani.time = 0;		//����
					this->m_lpRoadObstacle[i].ani.cnt = 0;		//�R�}��
					//�t���O�͏Փ˂��Ă��Ȃ���ԂƂ���
					ret = RUNNER_NOT_BUMP;
					continue;
				}
				break;
			}
		}
	}
	//�ŏI�I�ɓ������Ă��邩�����Ȃ�����Ԃ�
	return ret;
}

/*
initialize process for hurdle when player bump to hurdle
*/
char	CObstacle::CollisionInitializeHurdle( RUNNER *pRunner , int material) {

	//��x�ڐG�����n�[�h���Ƃ͔�������Ȃ�
	if (pRunner->obstacleBumpNow == material) return RUNNER_NOT_BUMP;

	//���ʃt���O�f�[�^�e�[�u��
	BYTE bumpTbl[RUNNER_EFFECT_KIND_OF_TYPE] = {

		RUNNER_EFFECT_FLAG_ABSOLUTE				,		//���G���
		RUNNER_EFFECT_FLAG_SPEED_UP 			,		//���x�㏸
		RUNNER_EFFECT_FLAG_HEALING				,		//��

	};
	//�o��������n�[�h���̎�ނ��܂Ƃ߂��f�[�^�e�[�u��
	tagHURDLECOLORTYPE hurdleTypeTbl[RUNNER_EFFECT_KIND_OF_TYPE] = {

		ROADENEMY_OBSTACLE_HURDLE_COLOR_RED	,			//��
		ROADENEMY_OBSTACLE_HURDLE_COLOR_BLUE	,		//��
		ROADENEMY_OBSTACLE_HURDLE_COLOR_GREEN ,			//��
	};

	//�߂�l�p
	BYTE	ret = RUNNNER_BUMP_TO_HURDLE;

	//��Q���̗v�f�����擾
	pRunner->obstacleBumpNow = material;
	
	//��Q���@ �n�[�h��
	//��Q�����n�[�h���A�L�����N�^�[�̍s�����W�����v���i�A�j���[�V�����j�ł����
	//�j�󏈗����s��Ȃ��œ����蔻��Ō��ʃt���O��Ԃ�
	if (pRunner->actionF == RUNNER_ACTION_JUMP ||
		pRunner->actionF == RUNNER_ACTION_JUMP_AND_ATTACK) {
		//���̃n�[�h���Ȃ画�����炸��return����
		if (this->m_lpRoadObstacle[material].colorType == ROADENEMY_OBSTACLE_HURDLE_COLOR_BLACK) return RUNNER_NOT_BUMP;
		//�n�[�h���̐F�̎�ނ�����
		for (int j = 0; j < RUNNER_EFFECT_KIND_OF_TYPE; j++) {
			//�n�[�h���̐F�ɉ����Č��ʂ𕪊򂳂���
			if (this->m_lpRoadObstacle[material].colorType == hurdleTypeTbl[j]) {
				//���ʂ̎�ނ�Ԃ�
				ret = bumpTbl[j];
				break;
			}
		}
	}
	return ret;
}

/*
initialize process for rock when player bump to hurdle
*/
char	CObstacle::CollisionInitializeRock(RUNNER *pRunner) {
	return RUNNNER_BUMP_TO_ROCK;
}