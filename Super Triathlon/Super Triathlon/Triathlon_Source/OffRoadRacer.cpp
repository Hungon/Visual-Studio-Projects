#include	"OffRoadRacer.h"
#include	"input.h"
#include	"gadget.h"
#include	"map.h"
#include	"OffRoadStage.h"
#include	"play.h"
#include	"sound.h"
#include	<stdio.h>
#include	<iostream>

//���O���
using namespace std;

//�� �ÓI�����o�ϐ��̒�`
const int COffRoadRacer::OFFROADRACER_IMAGE_MAX = 1;		//�g�p����
const int COffRoadRacer::OFFROADRACER_EFFECT_SOUND_MAX = 2;	//���ʉ�
//�E�B���h�E�n���h��
HWND	COffRoadRacer::m_hWnd = NULL;
//�e�평���l
//�\�����W�i���[�J�����W�j
const float COffRoadRacer::OFFFROADRACER_DEFAULT_POSITION_X = 304.0f;			//�\�����WX
const float COffRoadRacer::OFFFROADRACER_DEFAULT_POSITION_Y = 350.0f;			//�\�����WY
//�ړ���
const float COffRoadRacer::OFFFROADRACER_DEFAULT_MOVE_UP = -3.0f;		//�O��
const float COffRoadRacer::OFFFROADRACER_DEFAULT_MOVE_RIGHT = 3.0f;		//�E
const float COffRoadRacer::OFFFROADRACER_DEFAULT_MOVE_BACK = 3.0f;		//���
const float COffRoadRacer::OFFFROADRACER_DEFAULT_MOVE_LEFT = -3.0f;		//��
//���x�{��
const float	COffRoadRacer::OFFROADRACER_DEFAULT_SPEED_RATE = 1.0f;
//���x���v
float COffRoadRacer::m_AggregateSpeed = 0.0f;
//�R�}���h�A�N�V�����������p
BYTE  COffRoadRacer::m_CommandSuccess = OFFROADACTION_COMMAND_BIT_TYPE_VACANT;
//�摜�T�C�Y
const int COffRoadRacer::OFFROADRACER_IMAGE_WIDTH = 32;		//��
const int COffRoadRacer::OFFROADRACER_IMAGE_HEIGHT = 64;	//����
//�g�k�{��
const float COffRoadRacer::OFFFROADRACER_DEFAULT_SCALE = 1.0f;
//�A�N�V�������̃v���C���[�̍ő�g�k�{��
const float COffRoadRacer::OFFROADRACER_ACTION_SCALE_MAX = 5.0f;

//�g�k�{���̕ϐ��l
//���Z�l
const float	COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_ADD_EASY	= 0.01f;
const float	COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_ADD_NORMAL	= 0.02f;
const float	COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_ADD_HARD	= 0.03f;
//���Z�l
const float	COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_SUB_EASY	= 0.05f;
const float	COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_SUB_NORMAL	= 0.06f;
const float	COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_SUB_HARD	= 0.07f;
//�A�j���[�V�����ݒ�
//�ʏ펞
const int COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL_CNT_MAX	= 2;	//�ő�R�}��
const int COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL_FRAM	= 5;	//�t���[��

//��]�A�j���[�V������
const int COffRoadRacer::OFFROADRACER_ANIMATION_ROTATE_IMAGE_WIDTH  = 64;	//��
const int COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_ROTATE_CNT_MAX = 19;	//�ő�R�}��
const int COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_ROTATE_FRAM	= 2;	//�t���[��

//����]�A�j���[�V������
const int COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_CNT_MAX	= 5;	//�ő�R�}��
const int COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_FRAM		= 2;	//�t���[��

//�A�j���[�V�������̃r�b�g��
//��]�A�j���[�V����
const BYTE COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL			= 0x00;	//�ʏ펞
const BYTE COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_ROTATE		= 0x01;	//�E��]
const BYTE COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_ROTATE		= 0x02;	//����]
const BYTE COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_HALF_ROTATE = 0x04;	//�E����]
const BYTE COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_HALF_ROTATE	= 0x08;	//������]
//�A�j���[�V�����̎��
const int COffRoadRacer::OFFROADRACER_ANIMATION_KIND_OF_TYPE = 5;

//�A�j���[�V�������̕\�������WY
const int COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL_PICY			= 0;	//�ʏ펞
const int COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_HALF_ROTATE_PICY = 1;	//�E����]
const int COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_HALF_ROTATE_PICY	= 2;	//������]
const int COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_ROTATE_PICY		= 1;	//�E1��]
const int COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_ROTATE_PICY		= 2;	//��1��]

//�A�N�V�����R�}���h�ő���͐�
const int COffRoadRacer::OFFROADRACER_COMMAND_INPUT_MAX = 4;

//�A�N�V�����|�C���g�\������W
TPOINT<float>	COffRoadRacer::m_IndicateActionPoint = { 310.0f , 50.0f };
//���v�A�N�V�����|�C���g
int		COffRoadRacer::m_ActionPoint = 0;
//�A�N�V�����|�C���g�̕\������
const int	COffRoadRacer::OFFROADRACER_ACTION_POINT_DIGIT = 0;

//���C�_�[�̍��W�擾�p
TPOINT<float>	COffRoadRacer::m_RiderPosition = {0};

//���������A�N�V�����̐����������߂�
int		COffRoadRacer::m_SuccessActionCount[ OFFROADACTION_COMMAND_KIND_OF_TYPE ] = {0};
//�A���ŃA�N�V��������������������
int		COffRoadRacer::m_SucceedActoinConsecutive = 0;
int		COffRoadRacer::m_SucceedActoinConsecutiveMax = 0;
//�A���Ő��������A�N�V�������̕\������W
TPOINT<float>	COffRoadRacer::m_IndicateActionConsecutive = { 460.0f , 50.0f };
//process for scale of number score image
//default add scale rate
const float COffRoadRacer::OFFROADRACER_CONSECUTIVE_ACTION_ADD_SCALE_RATE = 0.015f;
//add scale max
const float COffRoadRacer::OFFROADRACER_CONSECUTIVE_ACTION_ADD_SCALE_RATE_MAX = 0.3f;
//scale up consecutive success count
const int COffRoadRacer::OFFROADRACER_CONSECUTIVE_ACTION_SCALE_UP_NUMBER_SUCCESS_COUNT = 3;

//process for scale of total number score image
//default add scale rate
const float COffRoadRacer::OFFROADRACER_TOTAL_ACTION_POINT_ADD_SCALE_RATE = 0.015f;
//add scale max
const float COffRoadRacer::OFFROADRACER_TOTAL_ACTION_POINT_ADD_SCALE_RATE_MAX = 0.3f;
//starting point of scale up
const int COffRoadRacer::OFFROADRACER_TOTAL_ACTION_POINT_STARTING_POINT_SCALE_UP = 1000;
//limit time for scale up of score image
const int COffRoadRacer::OFFROADRACER_LIMIT_TIME_FOR_SCALE_UP_SCORE_IMAGE = 130;

//�A�N�V�����R�}���h�������̑��x�ω��l
const float COffRoadRacer::OFFROADRACER_SUCCEED_ACTION_VARIABLE_SPEED_RATE = 0.10f;

//�A�N�V��������������
bool COffRoadRacer::m_actionFlag = false;

//blank time that before player can input
const int COffRoadRacer::OFFROADRACER_BLANK_TIME_FOR_INPUT = 10;

/*
************************************************************************************************
�f�B�X�g���N�^
************************************************************************************************
*/
COffRoadRacer::~COffRoadRacer(){
	
}

//************************************************************************************************
//���C�_�[�̏�����
//************************************************************************************************
void COffRoadRacer::InitOffRoadRacer() {

	//��Փx���擾
	tagGAMELEVEL	gameLevel = GetGameLevel();

	//�Q�[�����x���ˑ��ݒ�
	//�g�k�{�����Z�l
	float	scaleRateAddTbl[LEVEL_MAX] = {
		COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_ADD_EASY		,
		COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_ADD_NORMAL		,
		COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_ADD_HARD		,
	};
	//�g�k�{�����Z�l
	float	scaleRateSubTbl[LEVEL_MAX] = {
		COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_SUB_EASY		,
		COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_SUB_NORMAL		,
		COffRoadRacer::OFFROADRACER_VARIABLE_SCALE_RATE_SUB_HARD		,
	};

	//�� �ǂݍ��މ摜�̃f�[�^�e�[�u��
	char	*imageTbl[COffRoadRacer::OFFROADRACER_IMAGE_MAX] = {

		{"Image\\OffRoad\\cyclist.bmp"} ,
	};
	//�� �ǂݍ��ތ��ʉ��̃f�[�^�e�[�u��
	char	*lpSeTbl[COffRoadRacer::OFFROADRACER_EFFECT_SOUND_MAX] = {

		{"SE\\strongitem.wav"} ,
		{"SE\\jump.wav"} ,

	};

	//�� �摜�̐ݒ�
	//����g�p����摜�̖������������������m�ۂ���
	this->m_lpCOffRoadRacerImg = new int[COffRoadRacer::OFFROADRACER_IMAGE_MAX];
	//���ʉ�
	this->m_lpOffRoadRacerSe = new int[COffRoadRacer::OFFROADRACER_EFFECT_SOUND_MAX];
	//�X�e�[�W�̃N���X
	this->m_lpCOffRoadStage = new COffRoadStage;
	//�X�R�A�Ǘ��N���X
	this->m_lpScoreManager = new COffRoadScoreManager[OFFROADRACER_KIND_OF_SCORE];
	//add scale rate to score image for recer's action
	this->m_lpScoreScaleAdd = new float[OFFROADRACER_KIND_OF_SCORE];
	ZeroMemory(this->m_lpScoreScaleAdd, sizeof(float) * OFFROADRACER_KIND_OF_SCORE);
	//count time to scale up of score image
	this->m_lpScoreScaleUpCountTime = new int[OFFROADRACER_KIND_OF_SCORE];
	ZeroMemory(this->m_lpScoreScaleUpCountTime, sizeof(int) * OFFROADRACER_KIND_OF_SCORE);
	//after acale up number
	this->m_lpNumberAfterScaleUp = new int[OFFROADRACER_KIND_OF_SCORE];
	ZeroMemory(this->m_lpNumberAfterScaleUp, sizeof(int) * OFFROADRACER_KIND_OF_SCORE);

	//�A�N�V�����̐������̃��Z�b�g
	ZeroMemory(this->m_SuccessActionCount, sizeof(int) * OFFROADACTION_COMMAND_KIND_OF_ACTION);

	//���v�A�N�V�����|�C���g�����Z�b�g
	this->m_ActionPoint = 0;

	//�A���ŃA�N�V�������������������Z�b�g
	this->m_SucceedActoinConsecutive = 0;
	//�A���ŃA�N�V�������������ő吔�����Z�b�g
	this->m_SucceedActoinConsecutiveMax = 0;

	//�� �\���ݒ�
	//�@���C�_�[
	this->m_OffRoadRacerCh.x = COffRoadRacer::OFFFROADRACER_DEFAULT_POSITION_X;										//�\�����WX
	this->m_OffRoadRacerCh.y = this->m_lpCOffRoadStage[0].GetOffRoadStageDistance() -
		(this->m_lpCOffRoadStage[0].OFFROADSTAGE_SCREEN_HEIGHT - COffRoadRacer::OFFFROADRACER_DEFAULT_POSITION_Y);//�\�����WY
	this->m_OffRoadRacerCh.w = COffRoadRacer::OFFROADRACER_IMAGE_WIDTH;												//��
	this->m_OffRoadRacerCh.h = COffRoadRacer::OFFROADRACER_IMAGE_HEIGHT;											//����
	this->m_OffRoadRacerCh.flag = true;																				//�\���t���O
	this->m_OffRoadRacerCh.my = COffRoadRacer::OFFFROADRACER_DEFAULT_MOVE_UP;										//�ړ���Y
	this->m_OffRoadRacerCh.speed = COffRoadRacer::OFFROADRACER_DEFAULT_SPEED_RATE;									//���x�{��
	this->m_AggregateSpeed = this->m_OffRoadRacerCh.my * this->m_OffRoadRacerCh.speed;								//���x���v�l
	this->m_OffRoadRacerCh.scale = COffRoadRacer::OFFFROADRACER_DEFAULT_SCALE;

	//�g�k�{���ϐ��l
	//���Z�l
	this->m_actionScaleAdd = scaleRateAddTbl[gameLevel];
	//���Z�l
	this->m_actionScaleSub = scaleRateSubTbl[gameLevel];

	//���W�擾�p
	this->m_RiderPosition.x = this->m_OffRoadRacerCh.x;			//X
	this->m_RiderPosition.y = this->m_OffRoadRacerCh.y;			//Y

	//�� �摜�̓ǂݍ���
	for (int i = 0; i < COffRoadRacer::OFFROADRACER_IMAGE_MAX; i++) {

		this->m_lpCOffRoadRacerImg[i] = CreateDDImage(imageTbl[i], COffRoadRacer::m_hWnd);
	}
	//�� ���ʉ��̓ǂݍ���
	for (int i = 0; i < COffRoadRacer::OFFROADRACER_EFFECT_SOUND_MAX; i++) {

		this->m_lpOffRoadRacerSe[i] = CreateSound(lpSeTbl[i]);
	}

	//�J�������W�̐ݒ�
	SetCamera(0.0f, this->m_OffRoadRacerCh.y - (this->m_OffRoadRacerCh.y - COffRoadRacer::OFFFROADRACER_DEFAULT_POSITION_Y));


	// ���A�j���[�V�����ݒ�
	this->m_OffRoadRacerAni.max = COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL_CNT_MAX;		//�R�}��
	this->m_OffRoadRacerAni.fram = COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL_FRAM;			//�t���[��
	this->m_OffRoadRacerAni.type = COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL;				//�^�C�v

	//�A�j���[�V�������̐ݒ�
	SetAnimation(&this->m_OffRoadRacerCh, &this->m_OffRoadRacerAni, this->m_OffRoadRacerAni.sx, this->m_OffRoadRacerAni.sy,
		this->m_OffRoadRacerAni.max, this->m_OffRoadRacerAni.fram, this->m_OffRoadRacerAni.type, true);

	//�A�N�V�����R�}���h�p
	this->m_CommandSuccess = OFFROADACTION_COMMAND_TYPE_VACANT;
	this->m_DidCommandAction = OFFROADACTION_COMMAND_TYPE_VACANT;

	//�� �A�N�V�����R�}���h�̏�����
	this->InitOffRoadAction();

	//�� �X�R�A�̏�����
	for (int i = 0; i < OFFROADRACER_KIND_OF_SCORE; i++) { this->m_lpScoreManager[i].InitScoreManager(); }

}

//************************************************************************************************
//���C�_�[�̍X�V
//************************************************************************************************
void COffRoadRacer::UpdateOffRoadRacer(){

	//�J�����̍��W���擾
	float pY = GetCameraY();

	//�� �W�����v�n�_�Ƃ̔��� 
	bool jumpPoint = this->m_lpCOffRoadStage[0].CollisionJumpPoint( &this->m_OffRoadRacerCh );

	//�W�����v��
	if( jumpPoint && this->m_actionFlag == false ) PlaySound( this->m_lpOffRoadRacerSe[1] , 1 , false );

	//�� �A�N�V�����R�}���h�̐���
	//�W�����v�n�_�Ɣ�������A�d�Ȃ��Ă����珈��
	if (this->CreateActionCommand(jumpPoint)) {		//when indicate command, get flag of action

		//when count time more than default blank time, player can input
		if (this->m_OffRoadRacerCh.time > COffRoadRacer::OFFROADRACER_BLANK_TIME_FOR_INPUT) {
			this->m_actionFlag = true;
		}
		else {
			//cout time for player can input
			this->m_OffRoadRacerCh.time++;
		}
		//�� �A�N�V�����R�}���h�̓��͏�
		//�߂�l�ɑΉ������R�}���h�̃r�b�g�ڂ��󂯎��
		if (this->m_actionFlag && this->GetCommandActionCretaeFlag()) {			//when player is doing action and draw command, execute proccess
			this->SetOffRoadRacerCommandInfo(this->InputActionCommand());
			//�� �A�N�V�����R�}���h�̍X�V
			if (this->UpdateOffRoadAction()) this->m_SucceedActoinConsecutive = 0;	//reset consecutive action point
		}
		//�ݒ肵���{���܂ŉ��Z
		if( this->m_OffRoadRacerCh.scale < COffRoadRacer::OFFROADRACER_ACTION_SCALE_MAX ){
			//�{���ɉ��Z
			this->m_OffRoadRacerCh.scale += this->m_actionScaleAdd;
		}
	} 
	else 
	{ 	
		//�� �ړ�
		this->MoveOffRoadRacer();
		//�{�������Z
		this->m_OffRoadRacerCh.scale -= this->m_actionScaleSub;
		//�k�������͓��{�܂�
		if( this->m_OffRoadRacerCh.scale <= COffRoadRacer::OFFFROADRACER_DEFAULT_SCALE ){ 
			//�A�N�V�����t���O
			this->m_actionFlag = false;
			//���������R�}���h�̃r�b�g�ڂ����Z�b�g
			this->m_CommandSuccess = OFFROADACTION_COMMAND_BIT_TYPE_VACANT; 
			//�v���C���[�𓙔{�ɂ���
			this->m_OffRoadRacerCh.scale = COffRoadRacer::OFFFROADRACER_DEFAULT_SCALE;
			this->m_OffRoadRacerCh.time = 0;				//reset count time
		}
	}

	//�� �ړ��̐���
	this->ConstrainOffRoadRacerMove();

	//���x���v�̐ݒ�
	this->SetOffRoadRacerSpeed( (this->m_OffRoadRacerCh.my * this->m_OffRoadRacerCh.speed) - 
		( this->m_SucceedActoinConsecutive * COffRoadRacer::OFFROADRACER_SUCCEED_ACTION_VARIABLE_SPEED_RATE) );

	//�ړ��ʂ̍X�V
	this->m_OffRoadRacerCh.x += this->m_OffRoadRacerCh.mx;			//�ړ���X
	this->m_OffRoadRacerCh.y += this->m_AggregateSpeed;				//�ړ���Y

	//�ړ��ʂ̃��Z�b�g
	this->m_OffRoadRacerCh.mx = 0.0f;								//�ړ���X

	//�� ���C�_�[�̍��W���擾�p�ɑ��
	this->m_RiderPosition.x = this->m_OffRoadRacerCh.x;
	this->m_RiderPosition.y = this->m_OffRoadRacerCh.y;

	//�� �L�����N�^�[�̈ړ��ʂ𑬓x���[�^�[�ɔ��f������
	SetStatus( this->m_AggregateSpeed );

	//�� �J�������W�̐ݒ�
	SetCamera( 0.0f , this->m_OffRoadRacerCh.y - ( this->m_OffRoadRacerCh.y - pY ) );

	//�� �A�N�V�����ݒ�
	//�ʏ�A�j���[�V�����ŁA���s�����A�N�V�����R�}���h�̃r�b�g�ڂŌ��݂̃r�b�g�ڂ��������A�A�N�V�����ݒ�
	if( this->m_OffRoadRacerAni.type == COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL &&
		this->m_DidCommandAction != this->m_CommandSuccess ){

		this->SetRotateActionOffRoadRacer();
	}

	//�� �A�j���[�V�������̍X�V
	this->ChangeAnimationOffRoadRacer();

	//�� �A�N�V�����̍X�V
	this->UpdateRotateActionOffRoadRacer();
	
	//�� �X�R�A�̍X�V
	// Only Sccess Image
	this->m_lpScoreManager[0].UpdateScoreManagerSucceedAction();

	//Starting point of scale up for number image
	this->RacerStartingPointScaleUpScoreNumber();

}


//************************************************************************************************
//���C�_�[�̕\��
//************************************************************************************************
void COffRoadRacer::DrawOffRoadRacer() {

	//�J�����̍��W���擾
	float	pY = GetCameraY();

	//���C�_�[�̕\��
	if( this->m_OffRoadRacerCh.flag ){

		DDScaleBlt( 
			this->m_OffRoadRacerCh.x ,
			this->m_OffRoadRacerCh.y - pY ,
			this->m_OffRoadRacerCh.w ,
			this->m_OffRoadRacerCh.h ,
			this->m_lpCOffRoadRacerImg[0] ,
			this->m_OffRoadRacerCh.picx ,
			this->m_OffRoadRacerCh.picy ,
			this->m_OffRoadRacerCh.scale
			);
	}

	//�R�}���h�̕\��
	this->DrawOffRoadAction();

	//Draw Succeed image and consecutive action image
	for (int i = 0; i < OFFROADRACER_KIND_OF_SCORE; i++) {
		this->m_lpScoreManager[i].DrawScoreManager();
	}

	//�l�������A�N�V�����|�C���g�̕\��
	this->m_lpScoreManager[0].DrawScoreNumber( this->m_IndicateActionPoint , 
		this->m_ActionPoint , 
		COffRoadRacer::OFFROADRACER_ACTION_POINT_DIGIT , SCOREMANAGER_COLOR_BLACK , this->m_lpScoreScaleAdd[0] );

	//�A���Ő��������A�N�V��������\��
	this->m_lpScoreManager[1].DrawScoreNumber(this->m_IndicateActionConsecutive,
		this->m_SucceedActoinConsecutive,
		COffRoadRacer::OFFROADRACER_ACTION_POINT_DIGIT , SCOREMANAGER_COLOR_RED , this->m_lpScoreScaleAdd[1]);
	
}

//***************************************************************************************************
//���
//***************************************************************************************************
void COffRoadRacer::ReleaseOffRoadRacer(){

	//�摜
	//������Ȃ�������
	if( this->m_lpCOffRoadRacerImg != NULL ){
		for( int i = 0 ; i < COffRoadRacer::OFFROADRACER_IMAGE_MAX ; i++ ){
			ReleaseDDImage( this->m_lpCOffRoadRacerImg[i] );

		}
	}
	delete []this->m_lpCOffRoadRacerImg;
	this->m_lpCOffRoadRacerImg = NULL;

	//���ʉ�
	//������Ȃ�������
	if( this->m_lpOffRoadRacerSe != NULL ){
		for( int i = 0 ; i < COffRoadRacer::OFFROADRACER_EFFECT_SOUND_MAX ; i++ ){
			ReleaseSound( this->m_lpOffRoadRacerSe[i] );

		}
	}
	delete []this->m_lpOffRoadRacerSe;
	this->m_lpOffRoadRacerSe = NULL;

	//�X�R�A�̉��
	for (int i = 0; i < OFFROADRACER_KIND_OF_SCORE; i++) { this->m_lpScoreManager[i].ReleaseScoreManager(); }
	
	//�X�e�[�W�N���X�̉��
	delete this->m_lpCOffRoadStage;
	this->m_lpCOffRoadStage = NULL;

	//Delete ScoreManager class
	delete []this->m_lpScoreManager;
	this->m_lpScoreManager = NULL;

	//Delete add scale for score
	delete[]this->m_lpScoreScaleAdd;
	this->m_lpScoreScaleAdd = NULL;

	//delete count time to scale up of score image
	delete[]this->m_lpScoreScaleUpCountTime;
	this->m_lpScoreScaleUpCountTime = NULL;
	//after scale up number
	delete[]this->m_lpNumberAfterScaleUp;
	this->m_lpNumberAfterScaleUp = NULL;

	//�R�}���h�̉��
	this->ReleaseOffRoadAction();
}


/*
************************************************************************************
����
************************************************************************************
*/
void COffRoadRacer::MoveOffRoadRacer(){

	//�� ���C�_�[�̑���
	//�����
	if( KeepKey( VK_UP ) || KeepLever(LEVER_UP)){ this->m_OffRoadRacerCh.y += COffRoadRacer::OFFFROADRACER_DEFAULT_MOVE_UP; }
	//�E����
	if( KeepKey( VK_RIGHT ) || KeepLever(LEVER_RIGHT)){
		this->m_OffRoadRacerCh.mx = COffRoadRacer::OFFFROADRACER_DEFAULT_MOVE_RIGHT + 
			abs(this->m_SucceedActoinConsecutive * COffRoadRacer::OFFROADRACER_SUCCEED_ACTION_VARIABLE_SPEED_RATE);
	}
	//������
	if( KeepKey( VK_DOWN ) || KeepLever(LEVER_DOWN)){ this->m_OffRoadRacerCh.y += COffRoadRacer::OFFFROADRACER_DEFAULT_MOVE_BACK; }
	//������
	if( KeepKey( VK_LEFT ) || KeepLever(LEVER_LEFT)){
		this->m_OffRoadRacerCh.mx = COffRoadRacer::OFFFROADRACER_DEFAULT_MOVE_LEFT -
			abs(this->m_SucceedActoinConsecutive * COffRoadRacer::OFFROADRACER_SUCCEED_ACTION_VARIABLE_SPEED_RATE); 
	}

}


//************************************************************************************************
//�ړ��͈͂𐧌�����
//************************************************************************************************
void COffRoadRacer::ConstrainOffRoadRacerMove(){


	//�J�����̍��W���擾
	float comeCameraY = GetCameraY();

	//���ړ��͈͂𐧌�
	//��ʍ��[
	if( this->m_OffRoadRacerCh.x < 0 ){
	
		this->m_OffRoadRacerCh.x = 0.0f;
	}

	//��ʉE�[
	if( this->m_OffRoadRacerCh.x + ( this->m_OffRoadRacerCh.w * this->m_OffRoadRacerCh.scale ) >
		STAGE_WIDTH ){
		
		this->m_OffRoadRacerCh.x = STAGE_WIDTH - ( this->m_OffRoadRacerCh.w * this->m_OffRoadRacerCh.scale );
	}

	//��ʏ�[
	if( this->m_OffRoadRacerCh.y < comeCameraY ){
		
		this->m_OffRoadRacerCh.y = comeCameraY;
	}

	//��ʉ��[
	if( this->m_OffRoadRacerCh.y + ( this->m_OffRoadRacerCh.h * this->m_OffRoadRacerCh.scale ) > 
		comeCameraY + STAGE_HEIGHT ){
		
			this->m_OffRoadRacerCh.y = comeCameraY + STAGE_HEIGHT - 
			( this->m_OffRoadRacerCh.h * this->m_OffRoadRacerCh.scale );
	}

}

/*
************************************************************************************************
���C�_�[�̃A�j���[�V�����X�V
************************************************************************************************
*/
void COffRoadRacer::ChangeAnimationOffRoadRacer(){

	//�摜�f�[�^�e�[�u���iY���W�ŕω��j
	char animationTbl[ COffRoadRacer::OFFROADRACER_ANIMATION_KIND_OF_TYPE ] = {

		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL			,		//�ʏ펞
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_ROTATE		,		//�E��]
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_ROTATE		,		//����]
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_HALF_ROTATE,		//�E����]
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_HALF_ROTATE	,		//������]
	
	};

	//�e�摜�̕\�������W�������f�[�^�e�[�u��
	int animationPicYTbl[ COffRoadRacer::OFFROADRACER_ANIMATION_KIND_OF_TYPE ] = {

		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL_PICY				,	//�ʏ펞
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_HALF_ROTATE_PICY	,	//�E����]
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_HALF_ROTATE_PICY	,	//������]
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_ROTATE_PICY		,	//�E��]
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_ROTATE_PICY			,	//����]

	};
	
	//�A�N�V�������s�p�t���O�f�[�^�e�[�u��
	BYTE	doActionTbl[ OFFROADACTION_COMMAND_KIND_OF_ACTION ] = { 

		OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_HALF_ROTATE_ACTION	 ,		//�ォ�玞�v���180�x��]�̃A�N�V�����t���O
		OFFROADACTION_COMMAND_BIT_TYPE_LEFT_HALF_ROTATE_ACTION	 ,		//�ォ�甽���v���180�x��]�̃A�N�V�����t���O
		OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_ROTATE_ACTION		 ,		//�ォ�玞�v���̓��͎��̃A�N�V�����t���O
		OFFROADACTION_COMMAND_BIT_TYPE_LEFT_ROTATE_ACTION		 ,		//�ォ�甽���v���̓��͎��̃A�N�V�����t���O

	};

	//���݂̃A�j���[�V�����̃^�C�v�ԍ��ɉ����ĉ摜��ω�������
	for( int i = 0 ; i < COffRoadRacer::OFFROADRACER_ANIMATION_KIND_OF_TYPE ; i++ ){

		//�A�j���[�V�����ɉ����ĉ摜����
		if( animationTbl[i] == this->m_OffRoadRacerAni.type ){

			//�\����Y���W�𔽉f����
			this->m_OffRoadRacerCh.direction = animationPicYTbl[i];

			//�� �v���C���[�̃A�j���[�V�����X�V
			//�ʏ펞�ƂP��]�A�N�V����
			if( this->m_OffRoadRacerAni.type == COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL || 
				this->m_OffRoadRacerAni.type == COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_ROTATE ||
				this->m_OffRoadRacerAni.type == COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_ROTATE ){

				//���Z���A�j���[�V����
				if( UpdateAnimation( &this->m_OffRoadRacerCh , &this->m_OffRoadRacerAni ) == false ){

					//���݂̃A�j���[�V�����̃r�b�g�ڂ�0�ɂ���
					this->m_OffRoadRacerAni.type &= ~animationTbl[i];
					break;
				}
			
			}
			//����]�A�N�V�������̃A�j���[�V�����X�V
			if( this->m_OffRoadRacerAni.type == COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_HALF_ROTATE ||
				this->m_OffRoadRacerAni.type == COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_HALF_ROTATE ){
			
				//���Z�A�j���[�V������A���Z�ŃA�j���[�V����
				if( UpdateReverseAnimation( &this->m_OffRoadRacerCh , &this->m_OffRoadRacerAni ) == false ){

					//���݂̃A�j���[�V�����̃r�b�g�ڂ�0�ɂ���
					this->m_OffRoadRacerAni.type &= ~animationTbl[i];
					break;
				}
			}
			
		}

	}

}

/*
************************************************************************************************************
���C�_�[�̉�]�A�N�V�����̐ݒ�
*************************************************************************************************************
*/
void COffRoadRacer::SetRotateActionOffRoadRacer(){

	//��]�A�j���[�V�����̃f�[�^�e�[�u��
	BYTE	rotateAction[ OFFROADACTION_COMMAND_KIND_OF_ACTION ] = {

		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_HALF_ROTATE,		//�E����]
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_HALF_ROTATE	,		//������]
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_RIGHT_ROTATE		,		//�E��]
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_LEFT_ROTATE		,		//����]
	
	};

	//�A�N�V�������s�p�t���O�f�[�^�e�[�u��
	BYTE	doActionTbl[ OFFROADACTION_COMMAND_KIND_OF_ACTION ] = { 

		OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_HALF_ROTATE_ACTION	 ,		//�ォ�玞�v���180�x��]�̃A�N�V�����t���O
		OFFROADACTION_COMMAND_BIT_TYPE_LEFT_HALF_ROTATE_ACTION	 ,		//�ォ�甽���v���180�x��]�̃A�N�V�����t���O
		OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_ROTATE_ACTION		 ,		//�ォ�玞�v���̓��͎��̃A�N�V�����t���O
		OFFROADACTION_COMMAND_BIT_TYPE_LEFT_ROTATE_ACTION		 ,		//�ォ�甽���v���̓��͎��̃A�N�V�����t���O

	};

	//�A�j���[�V�����R�}��
	int		animationCntTbl[ OFFROADACTION_COMMAND_KIND_OF_ACTION ] = {

		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_CNT_MAX  ,	//����]
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_CNT_MAX  ,	//����]
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_ROTATE_CNT_MAX		,	//1��]
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_ROTATE_CNT_MAX		,	//1��]
	};

	//�t���[����
	int		framTbl[ OFFROADACTION_COMMAND_KIND_OF_ACTION ] = {
		
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_FRAM		,	//����]
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_FRAM		,	//����]
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_ROTATE_FRAM			,	//1��]
		COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_ROTATE_FRAM			,	//1��]
	};

	//�e�R�}���h�������̓��_
	int		actionPointTbl[ OFFROADACTION_COMMAND_KIND_OF_ACTION ] = {
	
		OFFROADACTION_COMMAND_TYPE_RIGHT_HALF_ROTATE_ACTION_POINT    ,		//�ォ�玞�v���180�x��]�̃A�N�V�����t���O
		OFFROADACTION_COMMAND_TYPE_LEFT_HALF_ROTATE_ACTION_POINT     ,		//�ォ�甽���v���180�x��]�̃A�N�V�����t���O
		OFFROADACTION_COMMAND_TYPE_RIGHT_ROTATE_ACTION_POINT	     ,		//�ォ�玞�v���̓��͎��̃A�N�V�����t���O
		OFFROADACTION_COMMAND_TYPE_LEFT_ROTATE_ACTION_POINT			 ,		//�ォ�甽���v���̓��͎��̃A�N�V�����t���O
	
	};

	//�R�}���h���͂ɐ���
	for( int i = 0 ; i < OFFROADACTION_COMMAND_KIND_OF_ACTION ; i++ ){
		if( this->m_CommandSuccess == doActionTbl[i] ){

			//���ʉ�
			PlaySound( this->m_lpOffRoadRacerSe[0] , 0 , false );

			//�A�N�V�������������J�E���g
			this->m_SuccessActionCount[i]++;

			//�A���������J�E���g
			this->m_SucceedActoinConsecutive++;

			//�A���Ő��������ő�l���X�V
			if (this->m_SucceedActoinConsecutive > this->m_SucceedActoinConsecutiveMax) this->m_SucceedActoinConsecutiveMax = this->m_SucceedActoinConsecutive;

			//�^�C�v�ύX
			this->m_OffRoadRacerAni.type |= rotateAction[i];

			//�摜�̕���ύX
			this->m_OffRoadRacerCh.w = COffRoadRacer::OFFROADRACER_ANIMATION_ROTATE_IMAGE_WIDTH;

			//�A�j���̃R�}�����Z�b�g
			this->m_OffRoadRacerAni.cnt= 0;
			//���Ԃ����Z�b�g
			this->m_OffRoadRacerAni.time = 0;
			//�ő�R�}����ύX
			this->m_OffRoadRacerAni.max = animationCntTbl[i];
			//�t���[����ύX
			this->m_OffRoadRacerAni.fram = framTbl[i];
			
			//�|�C���g���Z
			this->m_ActionPoint += actionPointTbl[i] + ( this->m_SucceedActoinConsecutive * OFFROADACTION_SUCCESS_ACTION_BONUS_POINT);
		
			break;

		}
	}

}

/*
************************************************************************************************************
���C�_�[�̉�]�A�N�V�����̍X�V
*************************************************************************************************************
*/
void COffRoadRacer::UpdateRotateActionOffRoadRacer(){


	//�A�j���[�V�����^�C�v���ʏ�ɖ߂��Ă�����
	if( this->m_OffRoadRacerAni.type == COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL ){

		//�摜�̕������ɖ߂�
		this->m_OffRoadRacerCh.w = COffRoadRacer::OFFROADRACER_IMAGE_WIDTH;

		//���ݎ��s�����A�N�V�����R�}���h�̃r�b�g�ڂ��擾
		this->m_DidCommandAction = this->m_CommandSuccess;

		//�A�j���̃R�}�����Z�b�g
		this->m_OffRoadRacerAni.cnt= 0;
		//���Ԃ����Z�b�g
		this->m_OffRoadRacerAni.time = 0;
		//�ő�R�}����ύX
		this->m_OffRoadRacerAni.max = COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL_CNT_MAX;
		//�t���[����ύX
		this->m_OffRoadRacerAni.fram = COffRoadRacer::OFFROADRACER_ANIMATION_TYPE_NORMAL_FRAM;

		
	}

}


/*
*************************************************************************************************************
���C�_�[�̓��͏��擾�p
*************************************************************************************************************
*/
void	COffRoadRacer::SetOffRoadRacerCommandInfo( BYTE CommandSuccess ){ 

	//�����̃r�b�g�ڂ��R�}���h���s�Ȃ牽��������������return
	if (CommandSuccess == OFFROADACTION_COMMAND_BIT_TYPE_VACANT) return;
	
	//once success bonus point
	this->m_ActionPoint += (this->m_SucceedActoinConsecutive * OFFROADACTION_SUCCESS_ACTION_BONUS_POINT);
	
	//�R�}���h�̃r�b�g�ڂ̃f�[�^�e�[�u��
	BYTE	commandBitTbl[ OFFROADACTION_COMMAND_KIND_OF_ACTION ] = {

		OFFROADACTION_COMMAND_BIT_TYPE_RIGHT	,		//�E
		OFFROADACTION_COMMAND_BIT_TYPE_LEFT		,		//��
		OFFROADACTION_COMMAND_BIT_TYPE_LEFT		,		//��
		OFFROADACTION_COMMAND_BIT_TYPE_RIGHT	,		//�E
	};

	//�A�N�V�����h���p�t���O�f�[�^�e�[�u��
	BYTE	technicalTbl[ OFFROADACTION_COMMAND_KIND_OF_ACTION ] = {
		
		OFFROADACTION_COMMAND_BIT_TYPE_UP					,		//��
		OFFROADACTION_COMMAND_BIT_TYPE_UP					,		//��
		OFFROADACTION_COMMAND_BIT_TYPE_UP_RIGHT_DOWN		,		//����́A�E���͂Ɖ�����
		OFFROADACTION_COMMAND_BIT_TYPE_UP_LEFT_DOWN			,		//����͂ƍ��A������

	};

	//�A�N�V���������p�t���O
	BYTE	doActionTbl[ OFFROADACTION_COMMAND_KIND_OF_ACTION ] = { 

		OFFROADACTION_COMMAND_BIT_TYPE_HALF_RIGHT_ROTATE	,		//�ォ�玞�v����180�x
		OFFROADACTION_COMMAND_BIT_TYPE_HALF_LEFT_ROTATE		,		//�ォ�甽���v����180�x
		OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_ROTATE			,		//�ォ�玞�v���̓���
		OFFROADACTION_COMMAND_BIT_TYPE_LEFT_ROTATE			,		//�ォ�甽���v���̓���
	};
	
	//���݂̃r�b�g�ڂƈ����̃r�b�g�ڂ𒲂ׂ�
	//�����̃r�b�g��
	for( int j = 0 ; j < OFFROADACTION_COMMAND_KIND_OF_ACTION ; j++ ){

		//�����̃r�b�g�ڂ��f�[�^�e�[�u����j�ԖڂɂȂ���΃A�N�V�����͕s�����ŁA���̗v�f��
		if( CommandSuccess != commandBitTbl[j] ) continue; 
		
		//�h���p�r�b�g��
		for( int i = j ; i < OFFROADACTION_COMMAND_KIND_OF_ACTION ; i++ ){

			//���݂̃r�b�g�ڂƈ����̃r�b�g�ڂƂŁA�Ή������h���A�N�V�����𒲂ׂ�
			if( this->m_CommandSuccess == technicalTbl[i] ){

				//�����p�t���O����
				this->m_CommandSuccess |= doActionTbl[i];
				this->m_CommandSuccess |= CommandSuccess;
				return;
			}
		}
	}

	//���݂̃A�N�V����������]�ŁA���̃r�b�g�ڂ��E�����Ȃ甽�΂̃r�b�g�ڂ�����
	//�E����] �� ������]�A�N�V����
	if( OFFROADACTION_COMMAND_BIT_TYPE_LEFT == CommandSuccess &&
		OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_HALF_ROTATE_ACTION == this->m_CommandSuccess ){

			//������]�����̂��߂ɁA���݂̓��͏������Z�b�g
			this->m_CommandSuccess = OFFROADACTION_COMMAND_BIT_TYPE_VACANT;
			//���s�A�N�V�����p�r�b�g�ڂ���
			this->m_CommandSuccess |= OFFROADACTION_COMMAND_BIT_TYPE_LEFT_HALF_ROTATE_ACTION;
			return;
	}
	//������] �� �E����]�A�N�V����
	if( OFFROADACTION_COMMAND_BIT_TYPE_RIGHT == CommandSuccess &&
		OFFROADACTION_COMMAND_BIT_TYPE_LEFT_HALF_ROTATE_ACTION == this->m_CommandSuccess ){

			//������]�����̂��߂ɁA���݂̓��͏������Z�b�g
			this->m_CommandSuccess = OFFROADACTION_COMMAND_BIT_TYPE_VACANT;
			//���s�A�N�V�����p�r�b�g�ڂ���
			this->m_CommandSuccess |= OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_HALF_ROTATE_ACTION;
			return;
	}
	//�P��]�A�N�V�����Ȃ�A�N�V�������s�r�b�g�ڂ����Z�b�g
	if( this->m_CommandSuccess == OFFROADACTION_COMMAND_BIT_TYPE_LEFT_ROTATE_ACTION || 
		this->m_CommandSuccess == OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_ROTATE_ACTION ){
	
		//�R�}���h�����Z�b�g
		this->m_CommandSuccess = OFFROADACTION_COMMAND_BIT_TYPE_VACANT;
	}
	//�Ō�Ɉ����̒l����
	this->m_CommandSuccess |= CommandSuccess;

}


/*
Starting point of scale up for number image
*/
void	COffRoadRacer::RacerStartingPointScaleUpScoreNumber() {

	//starting point of scale up
	int	startingpoint[OFFROADRACER_KIND_OF_SCORE] = {
		COffRoadRacer::OFFROADRACER_TOTAL_ACTION_POINT_STARTING_POINT_SCALE_UP,
		COffRoadRacer::OFFROADRACER_CONSECUTIVE_ACTION_SCALE_UP_NUMBER_SUCCESS_COUNT,
	};
	//each other action point
	int actionpoint[OFFROADRACER_KIND_OF_SCORE] = {
		this->m_ActionPoint,
		this->m_SucceedActoinConsecutive,
	};
	//add scale
	float addscale[OFFROADRACER_KIND_OF_SCORE] = {
		COffRoadRacer::OFFROADRACER_TOTAL_ACTION_POINT_ADD_SCALE_RATE,
		COffRoadRacer::OFFROADRACER_CONSECUTIVE_ACTION_ADD_SCALE_RATE,
	};
	//limit add scale
	float limitAddscale[OFFROADRACER_KIND_OF_SCORE] = {
		COffRoadRacer::OFFROADRACER_TOTAL_ACTION_POINT_ADD_SCALE_RATE_MAX,
		COffRoadRacer::OFFROADRACER_CONSECUTIVE_ACTION_ADD_SCALE_RATE_MAX,
	};


	//Process for scale of number score image
	//loop by racer's kind of score
	for (int i = 0; i < OFFROADRACER_KIND_OF_SCORE; i++) {
		// subsutitute consecutive action for local variable
		int consecutive = actionpoint[i];
		// if consecutive is zero during scale up process, initialize global scale rate and reset count time
		// then continue
		if (consecutive == 0 && this->m_lpScoreScaleAdd[i] > 0) {
			this->m_lpScoreScaleAdd[i] = 0; this->m_lpScoreScaleUpCountTime[i] = 0; continue;
		}
		//count time more than limit time
		if (this->m_lpScoreScaleUpCountTime[i] >= COffRoadRacer::OFFROADRACER_LIMIT_TIME_FOR_SCALE_UP_SCORE_IMAGE) {
			//subtract add scale rate from global variable
			this->m_lpScoreScaleAdd[i] -= addscale[i];
			//limit to subtract scale rate
			if (this->m_lpScoreScaleAdd[i] <= 0.0f) {
				//subsutitute scale up number for
				this->m_lpNumberAfterScaleUp[i] = consecutive;
				this->m_lpScoreScaleAdd[i] = 0.0f;			//reset scale
				this->m_lpScoreScaleUpCountTime[i] = 0;		//reset time
			}
			continue;										//don't need to add scale rate because came to limit scale rate
		}
		//case after scale up number to process
		if ((this->m_lpNumberAfterScaleUp[i] + startingpoint[i]) <= consecutive) {
			//limit time for scale up process
			if (this->m_lpScoreScaleAdd[i] == limitAddscale[i]) {
				//count time for scale up
				this->m_lpScoreScaleUpCountTime[i]++;
				//count time less than limit time
				if (this->m_lpScoreScaleUpCountTime[i] < COffRoadRacer::OFFROADRACER_LIMIT_TIME_FOR_SCALE_UP_SCORE_IMAGE)  continue;
			}
			//rest for total point
			int rest = 0;
			//case total point round off fractions less than one thousand
			if (i == 0) rest = actionpoint[0] % startingpoint[0]; consecutive -= rest;
			//scale up process
			if (consecutive % startingpoint[i] == 0 &&
				consecutive >= startingpoint[i]) {
				//add scale rate to global variabal
				this->m_lpScoreScaleAdd[i] += addscale[i];
			}
			//limit scale
			if (this->m_lpScoreScaleAdd[i] >= limitAddscale[i]) {
				//subsutitute scale max for
				this->m_lpScoreScaleAdd[i] = limitAddscale[i];
			}
		}
	}
}