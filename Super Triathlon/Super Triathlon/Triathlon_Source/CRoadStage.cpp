#include	"CRoadStage.h"
#include	"CPlayerRunner.h"
#include	"play.h"
#include	"wipe.h"
#include	"game.h"
#include	<iostream>

using namespace std;


//�� �ÓI�����o�ϐ��̍Ē�`
HWND	CRoadStage::m_hWnd = NULL;
const int  CRoadStage::ROADSTAGE_IMAGE_MAX = 5;				//�g�p����
const int CRoadStage::ROADSTAGE_BACK_GROUND_MAX = 2;		//�w�i�̐�
//�X�e�[�W�̋���
const float  CRoadStage::ROADSTAGE_DISTANCE_EAZY	= 15000.0f;		//EAZY���[�h
const float  CRoadStage::ROADSTAGE_DISTANCE_NORMAL	= 20000.0f;		//NORMAL���[�h
const float  CRoadStage::ROADSTAGE_DISTANCE_HARD	= 30000.0f;		//HARD���[�h

//���H�̓����i�s�N�Z���j
const float  CRoadStage::ROADSTAGE_THE_WIDTH_OF_A_ROAD = 380.0f;	//��ʏ�[�̓���

//�e��I�u�W�F�N�g�̐ݒ�
const int  CRoadStage::ROADSTAGE_OBJECT_MAX = 4;		//������
//�I�u�W�F�N�g�����̐ݒ�
const int	CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_WIDTH = 20;					//��
const int	CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_HEIGHT = 120;				//����
const float	CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_INDICATE_SPACE_Y = 200.0f;	//�\���ԊuY
//�g�k��
const float  CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_DEFAULT_SCALE = 1.0f;
const int	CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_MAX = 3;	//�����̐�

//�X�e�[�W�̑S��������
float CRoadStage::m_RoadStageHeight = 0.0f;

//setting railings
 const int	CRoadStage::ROADSTAGE_IMAGE_RAILING_WIDTH = 60;
 const int	CRoadStage::ROADSTAGE_IMAGE_RAILING_HEIGHT = 130;
 const float	CRoadStage::ROADSTAGE_IMAGE_RAILING_DEFAULT_SCALE = 1.0f;
 const int	CRoadStage::ROADSTAGE_IMAGE_RAILING_MAX = 6;


/*
**********************************************************************************
�R���X�g���N�^
**********************************************************************************
*/
CRoadStage::CRoadStage(){}


/*
**********************************************************************************
�f�B�X�g���N�^
**********************************************************************************
*/
CRoadStage::~CRoadStage(){

}


//************************************************************************************************
//����X�e�[�W�ł̏�����
//************************************************************************************************
void    CRoadStage::InitRoadStage(){
	
	//�摜�f�[�^
	char	*lpImageTbl[ CRoadStage::ROADSTAGE_IMAGE_MAX ] = {
	
		{ "Image\\Road\\road.bmp" }					,		//���H
		{ "Image\\Road\\roadline.bmp" }				,		//���������є���
		{ "Image\\Road\\aroundRoad.bmp" }			,		//�H���摜
		{ "Image\\Road\\roadStageGoal.bmp" }		,		//�S�[��
		{ "Image\\Road\\Railing.bmp"},						//railing
	};

	//�Q�[���̓�Փx���擾
	tagGAMELEVEL getGameLevel = GetGameLevel();


	//�X�e�[�W�̋����������f�[�^�e�[�u��
	float stageLevelTbl[ LEVEL_MAX ] = {

		CRoadStage::ROADSTAGE_DISTANCE_EAZY			,		//EAZY
		CRoadStage::ROADSTAGE_DISTANCE_NORMAL		,		//NORMAL
		CRoadStage::ROADSTAGE_DISTANCE_HARD			,		//HARD
	};

	//�� �e�탁�����̊m��
	this->m_lpRoadStageImg = new int [ CRoadStage::ROADSTAGE_IMAGE_MAX ];			//�摜
	//�\���ݒ�p
	this->m_lpRoadBackGround = new STAGE [ CRoadStage::ROADSTAGE_BACK_GROUND_MAX ];	//�w�i
	this->m_lpRoadObject = new STAGE [ CRoadStage::ROADSTAGE_OBJECT_MAX ];			//�I�u�W�F�N�g
	this->m_lpCPlayerRunner = new CPlayerRunner;									//�v���C���[�N���X
	this->m_lpRailings = new CHARACTER[(CRoadStage::ROADSTAGE_IMAGE_RAILING_MAX * 2)]; //railings

	//����Փx�ɉ����ăX�e�[�W�̋��������肷��
	this->m_RoadStageBottom = stageLevelTbl[getGameLevel];				//����
	this->m_RoadStageTop = stageLevelTbl[getGameLevel] - STAGE_HEIGHT;	//���
	this->m_RoadStageWidth = STAGE_WIDTH;								//��
	this->m_RoadStageHeight= stageLevelTbl[getGameLevel];				//����

	//�� �J�����͈̔͂�ݒ�
	ChangeCamera( this->m_RoadStageTop , this->m_RoadStageBottom , 
	this->m_RoadStageWidth , this->m_RoadStageHeight );

	//�� �J�����̏œ_������
	SetCamera( 0 ,  this->m_RoadStageHeight- 
		( ( STAGE_HEIGHT - CAMERA_FOCUS_POSITION_Y ) + CAMERA_FOCUS_POSITION_Y ) ); 

	//�� �摜�̓ǂݍ���
	for( int i = 0 ; i < CRoadStage::ROADSTAGE_IMAGE_MAX ; i++ ){
		this->m_lpRoadStageImg[i] = CreateDDImage( lpImageTbl[i] , CRoadStage::m_hWnd );
	}


	//�� �\���ݒ�

	//�w�i�̐ݒ�
	//0�ŏ�����
	ZeroMemory( this->m_lpRoadBackGround , sizeof( STAGE ) * CRoadStage::ROADSTAGE_BACK_GROUND_MAX );
	//���H�摜
	this->m_lpRoadBackGround[0].ch.x = 0.0f;				//X���W
	this->m_lpRoadBackGround[0].ch.y = 0.0f;				//Y���W
	this->m_lpRoadBackGround[0].ch.w = 640;					//��
	this->m_lpRoadBackGround[0].ch.h = 480;					//����
	this->m_lpRoadBackGround[0].ch.flag = true;				//�\���t���O

	//�H���摜
	this->m_lpRoadBackGround[1].ch.x = 0.0f;				//X���W
	this->m_lpRoadBackGround[1].ch.y = -1440.0f;			//Y���W
	this->m_lpRoadBackGround[1].ch.w = 640;					//��
	this->m_lpRoadBackGround[1].ch.h = 1920;				//����
	this->m_lpRoadBackGround[1].ch.flag = true;				//�\���t���O

	//�I�u�W�F�N�g�̐ݒ�
	//0�ŏ�����
	ZeroMemory( this->m_lpRoadObject , sizeof( STAGE ) * CRoadStage::ROADSTAGE_OBJECT_MAX );
	//�Ԑ�
	for( int i = 0 ; i < CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_MAX ; i++ ){
		this->m_lpRoadObject[i].ch.x = 310.0f;											//X
		this->m_lpRoadObject[i].ch.y = this->m_RoadStageTop + 
			( CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_INDICATE_SPACE_Y * i );			//Y
		this->m_lpRoadObject[i].ch.w = CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_WIDTH;	//��
		this->m_lpRoadObject[i].ch.h = CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_HEIGHT;	//����
		this->m_lpRoadObject[i].ch.flag = true;											//�t���O
		this->m_lpRoadObject[i].ch.scale = CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_DEFAULT_SCALE;		//�g�k��
		this->m_lpRoadObject[i].ch.defaultScale = CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_DEFAULT_SCALE;//�����g�k�{��
	}

	//�S�[�����C���摜
	this->m_lpRoadObject[3].ch.x = 135.0f;				//X���W
	this->m_lpRoadObject[3].ch.y = -250.0f;				//Y���W
	this->m_lpRoadObject[3].ch.w = 370;					//��
	this->m_lpRoadObject[3].ch.h = 200;					//����
	this->m_lpRoadObject[3].ch.flag = true;				//�\���t���O
	this->m_lpRoadObject[3].ch.scale = 1.35f;			//�g�k��
	this->m_lpRoadObject[3].ch.defaultScale = 1.35f;	//�g�k�{�������l

	//�Ԑ��̃X�^�[�g���_�ł̊g�k�������߂�
	for( int i = 0 ; i < CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_MAX ; i++ ){
		FittingPositionToBackCameraAngle( &this->m_lpRoadObject[i].ch , CRoadStage::ROADSTAGE_THE_WIDTH_OF_A_ROAD / 640.0f );
	}

	//set railings
	ZeroMemory(&this->m_lpRailings[0], sizeof(CHARACTER) * (CRoadStage::ROADSTAGE_IMAGE_RAILING_MAX * 2));
	//top height
	float topH = CRoadStage::ROADSTAGE_IMAGE_RAILING_HEIGHT * CRoadStage::ROADSTAGE_IMAGE_RAILING_DEFAULT_SCALE;
	//for material
	int cnt = 0;
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < CRoadStage::ROADSTAGE_IMAGE_RAILING_MAX; i++) {
			this->m_lpRailings[cnt].w = CRoadStage::ROADSTAGE_IMAGE_RAILING_WIDTH;
			this->m_lpRailings[cnt].h = CRoadStage::ROADSTAGE_IMAGE_RAILING_HEIGHT;
			this->m_lpRailings[cnt].scale = CRoadStage::ROADSTAGE_IMAGE_RAILING_DEFAULT_SCALE;
			this->m_lpRailings[cnt].y = this->m_RoadStageTop + (i * topH);
			//local coordinateY
			float localY = this->m_lpRailings[cnt].y - this->m_RoadStageTop;
			//calculate railing's coordinateY into rate of coordinateY in stage's height
			float rateY = ((float)STAGE_HEIGHT - localY) / (float)(STAGE_HEIGHT - ROADSTAGE_SUBTLTY_POSITIONX_FOR_ROAD_WIDTH);
			//constrain move
			float margin = ROADSTAGE_MARGIN_WIDTH_FOR_TOP_AND_BOTTOM_IN_ROAD * rateY;
			//coordinateX
			float posX[2] = { ((float)STAGE_WIDTH - margin) , margin - (ROADSTAGE_SUBTLTY_POSITIONX_FOR_ROAD_WIDTH / 1.7)};
			//subtract strict move area from stage's width
			this->m_lpRailings[cnt].x = posX[j];
			this->m_lpRailings[cnt].flag = true;
			this->m_lpRailings[cnt].defaultScale = CRoadStage::ROADSTAGE_IMAGE_RAILING_DEFAULT_SCALE;
			this->m_lpRailings[cnt].picy = j * CRoadStage::ROADSTAGE_IMAGE_RAILING_HEIGHT;
			cnt++;
		}
	}
}

//************************************************************************************************
//����X�e�[�W�̍X�V
//************************************************************************************************
void    CRoadStage::UpdateRoadStage(){

	//�� �����i�[�̏����擾
	//�v���C���[
	RUNNER* pPrunner = this->m_lpCPlayerRunner->GetRunner();

	//�J�����̍��W���擾
	float comeCameraY = GetCameraY();
	

	//���@���H�w�i���Ӊ摜���L�����N�^�[�̈ړ��ɍ��킹��
	//�X�N���[��������
	this->m_lpRoadBackGround[1].ch.y -= pPrunner->aggregateSpeed;
	
	//Y���W���O�܂ŒB�����猳�̏ꏊ�ɍĔz�u����
	if( this->m_lpRoadBackGround[1].ch.y >= 0.0f ){

		//Y���W�����ɖ߂�
		this->m_lpRoadBackGround[1].ch.y = -1440.0f;
	}

	//�� ��ʊO�ɏo���Ԑ��͏�֔z�u
	for( int i = 0 ; i < CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_MAX ; i++ ){
		
		//���@��O�ɗ���قǂɑ傫�������鏈��
		//�Ԑ��̕���ύX����
		FittingPositionToBackCameraAngle( &this->m_lpRoadObject[i].ch , ( CRoadStage::ROADSTAGE_THE_WIDTH_OF_A_ROAD / 640.0f ) / 2 );

		//��ʊO�ɏo�����ɍĔz�u
		if( this->m_lpRoadObject[i].ch.y - ( this->m_lpRoadObject[i].ch.h * 
			( ( this->m_lpRoadObject[i].ch.scale - ROADSTAGE_OBJECT_WHITE_LINE_DEFAULT_SCALE ) / 2 ) ) >=
			(comeCameraY + STAGE_HEIGHT) ){
			//Y���W
			this->m_lpRoadObject[i].ch.y = (float)( comeCameraY - this->m_lpRoadObject[i].ch.h );
			//�g�k�����f�t�H���g��
			this->m_lpRoadObject[i].ch.scale = ROADSTAGE_OBJECT_WHITE_LINE_DEFAULT_SCALE;
		}
	}


	//���@��O�ɗ���قǂɑ傫���S�[�����g�傷��
	FittingPositionToBackCameraAngle( &this->m_lpRoadObject[3].ch , 0.7f );

	//fitting railings' size according to present coordinateY in local coordinateY
	int cnt = 0;
	for (int j = 0; j < 2; j++){
		for (int i = 0; i < CRoadStage::ROADSTAGE_IMAGE_RAILING_MAX; i++) {
			if (this->m_lpRailings[cnt].flag) {
				//local coordinateY
				float localY = this->m_lpRailings[cnt].y - this->m_RoadStageTop;
				//calculate railing's coordinateY into rate of coordinateY in stage's height
				float rateY = ((float)STAGE_HEIGHT - localY) / (float)(STAGE_HEIGHT - ROADSTAGE_SUBTLTY_POSITIONX_FOR_ROAD_WIDTH);
				//constrain move
				float margin = ROADSTAGE_MARGIN_WIDTH_FOR_TOP_AND_BOTTOM_IN_ROAD * rateY;
				//coordinateX
				float posX[2] = { ((float)STAGE_WIDTH - margin) , margin - (ROADSTAGE_SUBTLTY_POSITIONX_FOR_ROAD_WIDTH / 1.7)};
				//subtract strict move area from stage's width
				this->m_lpRailings[cnt].x = posX[j];
				//top height
				float topH = this->m_lpRailings[cnt].h * this->m_lpRailings[cnt].scale;
				FittingPositionToBackCameraAngle(&this->m_lpRailings[cnt], (CRoadStage::ROADSTAGE_THE_WIDTH_OF_A_ROAD / 640.0f) / 2);
				//rearrange coordinateY as a railing gets off from stage's bottom
				if (this->m_lpRailings[cnt].y >= (comeCameraY + (float)STAGE_HEIGHT)) {
					//runner's facing position
					this->m_lpRailings[cnt].y = comeCameraY - topH;
					this->m_lpRailings[cnt].scale = CRoadStage::ROADSTAGE_IMAGE_RAILING_DEFAULT_SCALE;
					cnt++;
					continue;
				}
				cnt++;
			}
		}
	}
	//�� �����i�[�̈ړ��ʂ���ɉ�ʂ̉��ӂƏ�ӂ��X�V
	//����
	this->m_RoadStageBottom += pPrunner->aggregateSpeed;
	//���
	this->m_RoadStageTop = this->m_RoadStageBottom - STAGE_HEIGHT;

	//���J�����͈̔͂�ύX
	ChangeCamera( this->m_RoadStageTop , this->m_RoadStageBottom , 
	this->m_RoadStageWidth , this->m_RoadStageHeight );


	//�� �S�[���ݒ�
	//�v���C���[���S�[���n�_�܂ŒB������A
	//���U���g��ʂɑJ��
	if( pPrunner->ch.y + pPrunner->ch.h * pPrunner->ch.scale < 
		this->m_lpRoadObject[3].ch.y + this->m_lpRoadObject[3].ch.h * this->m_lpRoadObject[3].ch.scale ){
		//�^�C�}�[��OFF�ɂ���
		ControlStageTime(false);
		//���U���g��
		CreateWipe( SCENE_RESULT , CHANGE_SCENE , WIPE_CONTRACTION );
	}
	
}


//************************************************************************************************
//����X�e�[�W�̕\��
//************************************************************************************************
void    CRoadStage::DrawRoadStage(){


	//�J�����̍��W���擾
	float comeCameraY = GetCameraY();

	char mes[32] = {0};

/*
	wsprintf( &mes[0] , "����%d" , (int)this->m_RoadStageBottom );
	DDTextOut( &mes[0] , 500 , 85 );
*/

	//�� �w�i�摜
	//����X�e�[�W�u���H���Ӂv�̔w�i��\��
	if( this->m_lpRoadBackGround[1].ch.flag ){
	
		DDBlt(
			(int)this->m_lpRoadBackGround[1].ch.x ,
			(int)this->m_lpRoadBackGround[1].ch.y ,
			this->m_lpRoadBackGround[1].ch.w ,
			this->m_lpRoadBackGround[1].ch.h ,
			this->m_lpRoadStageImg[2] ,
			this->m_lpRoadBackGround[1].ch.picx ,
			this->m_lpRoadBackGround[1].ch.picy );

	}

	//����X�e�[�W�u���H�v�̔w�i��\��
	if( this->m_lpRoadBackGround[0].ch.flag ){
	
		DDBlt(
			(int)this->m_lpRoadBackGround[0].ch.x ,
			(int)this->m_lpRoadBackGround[0].ch.y ,
			this->m_lpRoadBackGround[0].ch.w ,
			this->m_lpRoadBackGround[0].ch.h ,
			this->m_lpRoadStageImg[0] ,
			this->m_lpRoadBackGround[0].ch.picx ,
			this->m_lpRoadBackGround[0].ch.picy );

	}

	//�������摜
	for( int i = 0 ; i < CRoadStage::ROADSTAGE_OBJECT_WHITE_LINE_MAX ; i++ ){

		if( this->m_lpRoadObject[i].ch.flag ){
		
			DDScaleBlt(
				this->m_lpRoadObject[i].ch.x ,
				this->m_lpRoadObject[i].ch.y - comeCameraY ,
				this->m_lpRoadObject[i].ch.w ,
				this->m_lpRoadObject[i].ch.h ,
				this->m_lpRoadStageImg[1] ,
				this->m_lpRoadObject[i].ch.picx ,
				this->m_lpRoadObject[i].ch.picy ,
				this->m_lpRoadObject[i].ch.scale );

		}

	}

	//������X�e�[�W�u�S�[���v�̉摜��\��
	if( this->m_lpRoadObject[3].ch.flag ){
	
		DDScaleBlt(
			this->m_lpRoadObject[3].ch.x ,
			this->m_lpRoadObject[3].ch.y - comeCameraY ,
			this->m_lpRoadObject[3].ch.w ,
			this->m_lpRoadObject[3].ch.h ,
			this->m_lpRoadStageImg[3] ,
			this->m_lpRoadObject[3].ch.picx ,
			this->m_lpRoadObject[3].ch.picy ,
			this->m_lpRoadObject[3].ch.scale );

	}

	//draw railings
	for (int i = 0; i < (CRoadStage::ROADSTAGE_IMAGE_RAILING_MAX * 2); i++) {
		if (this->m_lpRailings[i].flag) {
			DDScaleBltSelectIndicate(
				this->m_lpRailings[i].x,
				this->m_lpRailings[i].y - comeCameraY,
				this->m_lpRailings[i].w,
				this->m_lpRailings[i].h,
				this->m_lpRoadStageImg[4],
				this->m_lpRailings[i].picx,
				this->m_lpRailings[i].picy,
				this->m_lpRailings[i].scale,
				DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP);
		}
	}
	
}

//***************************************************************************************************
//���
//***************************************************************************************************
void    CRoadStage::ReleaseRoadStage(){

	//�摜�̉��
	if( this->m_lpRoadStageImg != NULL ){
		for( int i = 0 ; i < CRoadStage::ROADSTAGE_IMAGE_MAX ; i++ ){
			ReleaseDDImage( this->m_lpRoadStageImg[i] );
		}
	}

	//�������̉��
	delete []this->m_lpRoadStageImg;		//�摜
	this->m_lpRoadStageImg = NULL;
	
	//�������̉��
	if( this->m_lpRoadBackGround != NULL ){
		delete []this->m_lpRoadBackGround;		//�w�i
		this->m_lpRoadBackGround = NULL;
	}
	if( this->m_lpRoadObject != NULL ){
		delete []this->m_lpRoadObject;			//�I�u�W�F�N�g
		this->m_lpRoadObject = NULL;
	}
	if( this->m_lpCPlayerRunner != NULL ){
		delete this->m_lpCPlayerRunner;			//�v���C���[�N���X
		this->m_lpCPlayerRunner = NULL;
	}
	delete[]this->m_lpRailings;				//railings
	this->m_lpRailings = NULL;
}
