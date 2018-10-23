#include	"OffRoadScoreManager.h"
#include	"OffRoadScore.h"
#include	"graphics.h"
#include	"OffRoadRacer.h"
#include	"OffRoadAction.h"
#include	"input.h"
#include	<iostream>
#include	<stdio.h>

//���O���
using namespace	std;


//�� �ÓI�����o�ϐ��̒�`
const int COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_MAX = 2;		//�摜�g�p����

//�X�R�A�̃T�C�Y
const int COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_WIDTH = 160;	//��
const int COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_HEIGHT = 50;	//����
//Chain
const int COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_CHAIN_WIDTH = 96;//Width
//EXCELLENT�̃T�C�Y
const int	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_EXCELLENT_WIDTH = 288;	//��
//�g�k�{��
const float COffRoadScoreManager::OFFROADSCOREMANAGER_DEFAULT_SCORE_IMAGE_SCALE = 1.0f;	
//�X�R�A�̈ړ���
const float	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_MOVE_X = -5.0f;	//X
const float	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_MOVE_Y = 5.0f;	//Y
//�X�R�A�̑ҋ@����
const int	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_STAY_TIME_MAX = 30;

//�A�A�N�V�����|�C���g�\���p
//�X�R�A�̃T�C�Y
const int COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_IMAGE_WIDTH = 40;	//��
const int COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_IMAGE_HEIGHT = 50;	//����
//�g�k�{��
const float COffRoadScoreManager::OFFROADSCOREMANAGER_DEFAULT_SCORE_NUMBER_IMAGE_SCALE = 0.5f;
//�X�R�A�̎�ސ�
const char	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_KIND_OF_TYPE = 1;
//���o�����������X�R�A�̑���
const char	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_DIRECTION_MAX = 1;
//���Z�E���Z���o���̃t���[����
const int	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_DEFAULT_DIRECTION_INTERVAL_FRAM = 20;


//�X�R�A�̎��
//SUCCEED
const char	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_SUCCEED = 0;
//SCORE
const char	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_SCORE = 1;
//GOOD
const char	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_GOOD = 2;
//COOL
const char	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_COOL = 3;
//EXCELLENT
const char	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_EXCELLENT = 4;
//�X�R�A�̎�ސ�
const char	COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_KIND_OF_TYPE = 6;

//�E�B���h�E�n���h��
HWND	COffRoadScoreManager::m_hWnd = NULL;


/*
************************************************************************************************
�R���X�g���N�^
************************************************************************************************
*/
COffRoadScoreManager::COffRoadScoreManager(){

	//�N���X
	this->m_lpOffRoadScoreImage = NULL;
	this->m_lpOffRoadScoreNumber = NULL;
	this->m_lpCOffRoadRacer = NULL;
	this->m_lpCOffRoadAction = NULL;
	//�摜�̗v�f
	this->m_lpOffRoadScoreImg = NULL;
	//�����摜���o�p�\����
	this->m_lpScoreDirect = NULL;
	//Private�����o
	this->m_ScoreArriveCoordinate.x = 0.0f;
	this->m_ScoreArriveCoordinate.y = 0.0f;
	this->m_ScoreLeaveCoordinate.x = 0.0f;
	this->m_ScoreLeaveCoordinate.y = 0.0f;
	this->m_ScoreStayTime = 0;
	this->m_DisplayNumber = 0;
	this->m_CommandActionOld = OFFROADACTION_COMMAND_BIT_TYPE_VACANT;
	this->m_ValueTypeNext = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_SUCCEED;

}

/*
************************************************************************************************
�f�B�X�g���N�^
************************************************************************************************
*/
COffRoadScoreManager::~COffRoadScoreManager(){
	
}

//************************************************************************************************
//�X�R�A�̏�����
//************************************************************************************************
void COffRoadScoreManager::InitScoreManager(){

	//�� �ǂݍ��މ摜�f�[�^�e�[�u��
	char	*lpImgTbl[ COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_MAX ] = {

		{ "Image\\OffRoad\\Number.bmp" } ,						//�����摜
		{ "Image\\OffRoad\\ValueImage.bmp" } ,					//Succeed�̉摜

	};

	//�� �g�p����摜�̃��������m��
	this->m_lpOffRoadScoreImg = new int [ COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_MAX ];

	//�� �g�p����N���X�̃��������m��
	//�X�R�A�C���[�W
	this->m_lpOffRoadScoreImage = new COffRoadScore [ COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_KIND_OF_TYPE ];
	//�����X�R�A
	this->m_lpOffRoadScoreNumber = new COffRoadScore [ COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_KIND_OF_TYPE ];
	this->m_lpCOffRoadRacer = new COffRoadRacer;
	this->m_lpCOffRoadAction = new COffRoadAction;
	//�����摜���o�p�̍\����
	this->m_lpScoreDirect = new SCOREDIRECT[ COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_DIRECTION_MAX ];
	//0�ŏ�����
	ZeroMemory( this->m_lpScoreDirect , sizeof( SCOREDIRECT ) * COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_DIRECTION_MAX );

	//�� �摜�ݒ�
	//�@ �����摜
	this->m_lpOffRoadScoreNumber[0].m_Isize.x = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_IMAGE_WIDTH;			//��
	this->m_lpOffRoadScoreNumber[0].m_Isize.y = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_IMAGE_HEIGHT;			//����
	this->m_lpOffRoadScoreNumber[0].m_scale = COffRoadScoreManager::OFFROADSCOREMANAGER_DEFAULT_SCORE_NUMBER_IMAGE_SCALE;
	this->m_lpOffRoadScoreNumber[0].m_existFlag = true;
	//�����摜���o���̐ݒ�
	//���Z�E���Z���o���̉��Z���Z�Ԋu
	this->m_lpScoreDirect[0].intervalFram = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_DEFAULT_DIRECTION_INTERVAL_FRAM;

	//�A�N�V�����R�}���h�̘g�̐ݒ���擾
	//���W
	TPOINT<float> pos = this->m_lpCOffRoadAction->GetFramIndicatePosition();
	//�T�C�Y
	TPOINT<int>	size = this->m_lpCOffRoadAction->GetFramSize();


	//�e��C���[�W�摜�̐ݒ�
	for( int i = 0 ; i < COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_KIND_OF_TYPE ; i++ ){
	
		this->m_lpOffRoadScoreImage[i].m_Fpos.x = pos.x + size.x;														//�\�����WX
		this->m_lpOffRoadScoreImage[i].m_Fpos.y = pos.y - COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_HEIGHT;	//�\�����WY
		this->m_lpOffRoadScoreImage[i].m_Isize.x = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_WIDTH;			//��
		this->m_lpOffRoadScoreImage[i].m_Isize.y = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_HEIGHT;		//����
		this->m_lpOffRoadScoreImage[i].m_scale = COffRoadScoreManager::OFFROADSCOREMANAGER_DEFAULT_SCORE_IMAGE_SCALE;	//�g�k�{��
		this->m_lpOffRoadScoreImage[i].m_Fmove.x = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_MOVE_X;				//�ړ���X
		this->m_lpOffRoadScoreImage[i].m_Fmove.y = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_MOVE_Y;				//�ړ���Y
		this->m_lpOffRoadScoreImage[i].m_Ipic.y = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_HEIGHT * i;		//�\����Y		
	}
	//SCORE�摜�̕\������W��ݒ�
	//�v���C���[�̃A�N�V�����|�C���g���W���擾
	TPOINT<float> pointPos = this->m_lpCOffRoadRacer[0].GetActionPointIndicate();
	//�����摜�̕��T�C�Y
	float numberSize = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_NUMBER_IMAGE_WIDTH * 
		COffRoadScoreManager::OFFROADSCOREMANAGER_DEFAULT_SCORE_NUMBER_IMAGE_SCALE;
	//SCORE�摜�̕��T�C�Y
	float scoreSize = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_WIDTH * 
		COffRoadScoreManager::OFFROADSCOREMANAGER_DEFAULT_SCORE_IMAGE_SCALE;
	//�\������W�̐ݒ�
	this->m_lpOffRoadScoreImage[1].m_Fpos.x = pointPos.x - ( scoreSize - numberSize ) / 2;				//X
	this->m_lpOffRoadScoreImage[1].m_Fpos.y = pointPos.y - 
	( this->m_lpOffRoadScoreImage[1].m_Isize.y * this->m_lpOffRoadScoreImage[1].m_scale );			//Y
	//�\���t���O
	this->m_lpOffRoadScoreImage[1].m_existFlag = true;

	//Set Chain image
	//Get player's position
	TPOINT<float> consecutivePos = this->m_lpCOffRoadRacer->GetOffRoadRacerIndicateConsecutiveAction();
	//substitute width for variable number
	this->m_lpOffRoadScoreImage[5].m_Isize.x = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_CHAIN_WIDTH;
	//Drawing position
	//position Y
	this->m_lpOffRoadScoreImage[5].m_Fpos.y = consecutivePos.y - ((COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_CHAIN_WIDTH *
		COffRoadScoreManager::OFFROADSCOREMANAGER_DEFAULT_SCORE_IMAGE_SCALE) / 2);
	//Chain's high size of width
	float chainWidth = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_CHAIN_WIDTH * COffRoadScoreManager::OFFROADSCOREMANAGER_DEFAULT_SCORE_IMAGE_SCALE;
	//position X
	this->m_lpOffRoadScoreImage[5].m_Fpos.x = consecutivePos.x - (chainWidth - numberSize) / 2;
	//Drawing flag
	this->m_lpOffRoadScoreImage[5].m_existFlag = true;

	//EXCELLENT�̉摜������
	this->m_lpOffRoadScoreImage[4].m_Isize.x = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_EXCELLENT_WIDTH;			//��

	//�\���o�����W
	this->m_ScoreLeaveCoordinate.x = pos.x + size.x;
	this->m_ScoreLeaveCoordinate.y = pos.y - COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_HEIGHT;
	//�\���������W
	this->m_ScoreArriveCoordinate.x = pos.x;																//X
	this->m_ScoreArriveCoordinate.y = pos.y - COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_HEIGHT;	//Y

	//�摜�ԍ��̐ݒ�
	for( int i = 0 ; i < COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_MAX ; i++ ){
		this->m_lpOffRoadScoreImg[i] = CreateDDImage( lpImgTbl[i] , COffRoadScoreManager::m_hWnd );
	}
	//�e��C���[�W�摜�̐ݒ�
	for( int i = 0 ; i < COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_KIND_OF_TYPE ; i++ ){
		this->m_lpOffRoadScoreImage[i].SetCharaImg( this->m_lpOffRoadScoreImg[1] );
	}

}

//************************************************************************************************
//�X�R�A�̍X�V
//************************************************************************************************
void COffRoadScoreManager::UpdateScoreManagerSucceedAction(){

	//�A�N�V�������s�p�t���O�f�[�^�e�[�u��
	BYTE	doActionTbl[ OFFROADACTION_COMMAND_KIND_OF_ACTION ] = { 

		OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_HALF_ROTATE_ACTION	 ,		//�ォ�玞�v���180�x��]�̃A�N�V�����t���O
		OFFROADACTION_COMMAND_BIT_TYPE_LEFT_HALF_ROTATE_ACTION	 ,		//�ォ�甽���v���180�x��]�̃A�N�V�����t���O
		OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_ROTATE_ACTION		 ,		//�ォ�玞�v���̓��͎��̃A�N�V�����t���O
		OFFROADACTION_COMMAND_BIT_TYPE_LEFT_ROTATE_ACTION		 ,		//�ォ�甽���v���̓��͎��̃A�N�V�����t���O

	};

	//�\������]���摜�̃f�[�^�e�[�u��
	char	valueImageTbl[ COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_KIND_OF_TYPE - 1] = {

		//SUCCEED
		COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_SUCCEED	,
		//GOOD
		COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_GOOD		,
		//COOL
		COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_COOL		,
		//EXCELLENT
		COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_EXCELLENT	,

	};

	//�R�}���h�������Ȃ珈��
	if( this->m_lpCOffRoadAction[0].GetCommandActionCretaeFlag() ){
		//�]���摜�̐��������[�v
		for( int j = this->m_ValueTypeNext ; j < (COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_KIND_OF_TYPE - 1); j++ ){

			//SCORE�̉摜�Ȃ玟�̗v�f��
			if( j == COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_SCORE ) continue;

			//���\���]���摜�̂ݕ\��
			if( this->m_lpOffRoadScoreImage[j].m_existFlag ) continue;

			//�A�N�V�����R�}���h�̐��������[�v
			for( int i = 0 ; i < OFFROADACTION_COMMAND_KIND_OF_ACTION ; i++ ){

				//�R�}���h���͂ɐ������A�O��̃R�}���h�A�N�V�����ƈقȂ�Ε]���摜��\��
				if( this->m_lpCOffRoadRacer[0].GetOffRoadRacerCommandInfo() == doActionTbl[i] && 
					this->m_CommandActionOld != doActionTbl[i] ){

					//����̃A�N�V�����r�b�g�ڂ��擾
					this->m_CommandActionOld = doActionTbl[i];

					//���̕]���摜�̗v�f���擾
					this->m_ValueTypeNext = ( j + 1 );

					//�\���t���O��true��
					this->m_lpOffRoadScoreImage[j].m_existFlag = true;
					break;
				}
			}
		}
	} else {
		//�]���ς݉摜�����Z�b�g
		this->m_ValueTypeNext = COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_SUCCEED;
		//����̃A�N�V�����r�b�g�ڂ��擾
		this->m_CommandActionOld = OFFROADACTION_COMMAND_TYPE_VACANT;
	}

	//�]���摜�̐��������[�v
	for (int i = 0; i < (COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_KIND_OF_TYPE - 1); i++) {

		//SCORE�̉摜�Ȃ玟�̗v�f��
		if (i == COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_TYPE_SCORE) continue;

		//�\���t���O�Ȃ�X�R�A�̈ړ�
		if (this->m_lpOffRoadScoreImage[i].m_existFlag) {

			//�\������W�ɓ�������܂ňړ��ʂ����Z
			if (this->m_lpOffRoadScoreImage[i].m_Fpos.x >= this->m_ScoreArriveCoordinate.x) {
				//�ړ��ʂ����W�ɉ��Z
				this->m_lpOffRoadScoreImage[i].m_Fpos.x += this->m_lpOffRoadScoreImage[i].m_Fmove.x;		//X
			}
			//�\������W�ɓ�����������W�������l�ɖ߂�
			if (this->m_lpOffRoadScoreImage[i].m_Fpos.x <= this->m_ScoreArriveCoordinate.x) {

				//���Ԍo��
				this->m_ScoreStayTime++;
			}

			//�o�ߎ��Ԃ��ݒ肵�����ԂɒB��������W�����ɖ߂��āA�\���t���O��false
			if (this->m_ScoreStayTime > COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_STAY_TIME_MAX) {
				//�o�ߎ��Ԃ����Z�b�g
				this->m_ScoreStayTime = 0;
				this->m_lpOffRoadScoreImage[i].m_Fpos.x = this->m_ScoreLeaveCoordinate.x;
				this->m_lpOffRoadScoreImage[i].m_existFlag = false;
			}
		}
	}

}


//************************************************************************************************
//�X�R�A�̕\��
//************************************************************************************************
void COffRoadScoreManager::DrawScoreManager(){

	//�e��C���[�W�摜�̕\��
	for( int i = 0 ; i < COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_KIND_OF_TYPE ; i++ ){

		//�g�k�\��
		this->m_lpOffRoadScoreImage[i].DrawChara();
	}
}

/*
***************************************************************************************************
�����摜�\��
***************************************************************************************************
*/
void	COffRoadScoreManager::DrawScoreNumber(TPOINT<float> pos, int number,
	int digit, tagSCOREMANAGERCOLORTYPE color, float addScale) {

	//�\�����鐔�l���󂯎��
	int num = number;
	//�\�����鐔�l
	int rest = 0;
	//�\�����鐔�l�̕\����
	int numD = 0;
	//���J�E���g�p
	int digitCnt = 0;

	//�n���ꂽ�l�����̒l�Ȃ琳�̒l�ɂ���
	if (num < 0) num *= -1;

	//�\���������l���󂯎��
	this->m_lpScoreDirect[0].terminateNum = num;
	//�� ���Z���Z���o
	num = this->GraduallyNumber(&this->m_lpScoreDirect[0]);

	//�n���ꂽ�����̐��l�̊e���̐��l�𒲂ׂ�
	do {

		//�]��̐��l�����߂錅�̐��l
		rest = num % 10;
		//�n���ꂽ�l�𖈉�10�Ŋ����Ă���
		num /= 10;
		//�\�����鐔�̕\����������
		numD = this->m_lpOffRoadScoreNumber[0].m_Isize.x * rest;

		//���l�摜�̕\��
		DDScaleBlt(pos.x -
			(this->m_lpOffRoadScoreNumber[0].m_Isize.x * this->m_lpOffRoadScoreNumber[0].m_scale) * digitCnt,
			pos.y,
			this->m_lpOffRoadScoreNumber[0].m_Isize.x,
			this->m_lpOffRoadScoreNumber[0].m_Isize.y,
			this->m_lpOffRoadScoreImg[0],
			this->m_lpOffRoadScoreNumber[0].m_Ipic.x + numD,
			this->m_lpOffRoadScoreNumber[0].m_Ipic.y + (this->m_lpOffRoadScoreNumber[0].m_Isize.y * color),
			( this->m_lpOffRoadScoreNumber[0].m_scale + addScale) );

		//�v�Z���閈�ɃJ�E���g
		digitCnt++;

		//�n���ꂽ�l��0�����ɂȂ����甲����
	} while (num > 0 || digitCnt <= digit);

}


//***************************************************************************************************
//���
//***************************************************************************************************
void COffRoadScoreManager::ReleaseScoreManager(){

	//�摜�̉��
	//������Ȃ���
	if( this->m_lpOffRoadScoreImg != NULL ){
		for( int i = 0 ; i < COffRoadScoreManager::OFFROADSCOREMANAGER_SCORE_IMAGE_MAX ; i++ ){
			ReleaseDDImage( this->m_lpOffRoadScoreImg[i] );
		}
	}
	
	//�e�탁�����̉��
	//�N���X
	delete []this->m_lpOffRoadScoreImage;	//�X�R�A
	this->m_lpOffRoadScoreImage = NULL;

	delete []this->m_lpOffRoadScoreNumber;	//�����X�R�A
	this->m_lpOffRoadScoreNumber = NULL;
	
	delete this->m_lpCOffRoadRacer;			//���C�_�[
	this->m_lpCOffRoadRacer = NULL;

	delete this->m_lpCOffRoadAction;		//�A�N�V����
	this->m_lpCOffRoadAction = NULL;

	//�X�R�A���o�p�\����
	delete []this->m_lpScoreDirect;
	this->m_lpScoreDirect = NULL;

	//�摜�v�f
	delete []this->m_lpOffRoadScoreImg;
	this->m_lpOffRoadScoreImg = NULL;

}


/*
************************************************************************************************
�ڕW�̐��l�܂ŏ��X�ɉ��Z�i���Z�j����
************************************************************************************************
*/
int COffRoadScoreManager::GraduallyNumber( SCOREDIRECT* pScore  ){

	//�����l�̏���l�E�����l�̐ݒ�
	//������0
	if( pScore->terminateNum < 0 ) pScore->terminateNum = 0;
	
	//�����1������
	if( pScore->terminateNum > 100000000 ) pScore->terminateNum = 99999999;

	//���\������l�ƕ\���������l��������珈�����s��
	//�������͉�]�̏������܂������Ă�������s����
	if( pScore->terminateNum != pScore->indicateNum ){

		//�\������l���\���������l��菬���������瑝������
		if( pScore->indicateNum <= pScore->terminateNum ){

			//�\������l�ƕ\���������l�̍��������߂�
			pScore->restNum = pScore->terminateNum - pScore->indicateNum;

			//�t���[�����ɉ��Z����
			pScore->indicateNum += pScore->restNum / pScore->intervalFram;
			
			//�\���������l���\������l���獷�����������l�t�߂ɂȂ�����
			//�\���������l��������
			//�������́A�uZ�L�[���@�{�^���v����������l����
			if( pScore->restNum <= pScore->intervalFram || JustKey('Z') || JustButton(BUTTON_1) ){

				//�ŏI�I�ɕ\������l�̑��
				pScore->indicateNum = pScore->terminateNum;
			}
		}

		//�\������l���\���������l���傫�������猸������
		if( pScore->indicateNum >= pScore->terminateNum ){

			//�\������l�ƕ\���������l�̍��������߂�
			pScore->restNum = pScore->indicateNum - pScore->terminateNum;

			//�t���[�����Ɍ��Z����
			pScore->indicateNum -= pScore->restNum / pScore->intervalFram;

			//�\���������l���\������l���獷�����������l�t�߂ɂȂ�����
			//�\����������l��������
			if( pScore->restNum <= pScore->intervalFram || JustKey('Z') || JustButton(BUTTON_1) ){

				//�ŏI�I�ɕ\������l�̑��
				pScore->indicateNum = pScore->terminateNum;

			}
		}
	}

	//�Ō�ɕ\������l��Ԃ�
	return pScore->indicateNum;

}