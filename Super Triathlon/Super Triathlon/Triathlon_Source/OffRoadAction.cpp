#include	"OffRoadAction.h"
#include	"play.h"
#include	"input.h"
#include	"sound.h"
#include	<iostream>

//���O���
using namespace std;


//�� �ÓI�����o�ϐ��̒�`
const int COffRoadAction::OFFROADACTION_IMAGE_MAX = 2;			//�g�p����
const int COffRoadAction::OFFROADACTION_EFFECT_SOUND_MAX = 2;	//���ʉ��g�p��

//�E�B���h�E�n���h��
HWND	COffRoadAction::m_hWnd = NULL;
//�e�평���l
//�@�A�N�V�����R�}���h�̃t���[��
//�\�����W�i���[�J�����W�j
const float COffRoadAction::OFFROADACTION_FRAM_DEFAULT_POSITION_X = 180.0f;			//�\�����WX
const float COffRoadAction::OFFROADACTION_FRAM_DEFAULT_POSITION_Y = 380.0f;			//�\�����WY
//�摜�T�C�Y
const int COffRoadAction::OFFROADACTION_FRAM_IMAGE_WIDTH = 160;		//��
const int COffRoadAction::OFFROADACTION_FRAM_IMAGE_HEIGHT = 64;		//����
//�g�k�{��
const float COffRoadAction::OFFROADACTION_FRAM_DEFAULT_SCALE = 1.0f;

//�g�̐ݒ�
//�T�C�Y
TPOINT<int>		COffRoadAction::m_FramSize = {0};
//�\�����W
TPOINT<float>	COffRoadAction::m_indicateFramPos = {0};

//�A�A�N�V�����R�}���h�摜
//�e�평���l
//�\�����W�i���[�J�����W�j
const float COffRoadAction::OFFROADACTION_COMMAND_DEFAULT_POSITION_X = 
COffRoadAction::OFFROADACTION_FRAM_DEFAULT_POSITION_X + 5.0f;			//�\�����WX
const float COffRoadAction::OFFROADACTION_COMMAND_DEFAULT_POSITION_Y =
COffRoadAction::OFFROADACTION_FRAM_DEFAULT_POSITION_Y + 10.0f;			//�\�����WY

//�摜�T�C�Y
const int COffRoadAction::OFFROADACTION_COMMAND_IMAGE_WIDTH = 40;		//��
const int COffRoadAction::OFFROADACTION_COMMAND_IMAGE_HEIGHT = 40;		//����
//�g�k�{��
const float COffRoadAction::OFFROADACTION_COMMAND_DEFAULT_SCALE = 1.0f;
//���ۂɉ������摜�̎��
const CORRECTINPUTTYPE COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_NORMAL	= 0;
const CORRECTINPUTTYPE COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_EROR	= 1;
const CORRECTINPUTTYPE COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_SUCCESS = 2;
//���ۂɉ�������ސ�
const int COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_MAX = 3;


//�A�N�V�����R�}���h�\���Ԋu
const int COffRoadAction::OFFROADACTION_COMMAND_INTERVAL_TIME_EASY_MODE		= 55;		//Easy��
const int COffRoadAction::OFFROADACTION_COMMAND_INTERVAL_TIME_NORMAL_MODE	= 40;		//Normal��
const int COffRoadAction::OFFROADACTION_COMMAND_INTERVAL_TIME_HARD_MODE		= 31;		//Hard��

//�A�N�V�����R�}���h�\����
const int COffRoadAction::OFFROADACTION_COMMAND_CREATION_MAX_EASY_MODE		= 4;		//Easy��
const int COffRoadAction::OFFROADACTION_COMMAND_CREATION_MAX_NORMAL_MODE	= 6;		//Normal��
const int COffRoadAction::OFFROADACTION_COMMAND_CREATION_MAX_HARD_MODE		= 8;		//Hard��

//�R�}���h�������t���O
bool	COffRoadAction::m_CommandCreateFlag = false;

/*
************************************************************************************************
�R���X�g���N�^
************************************************************************************************
*/
COffRoadAction::COffRoadAction(){

	this->m_actionInterval = 0;
	this->m_commandMax = 0;
	this->m_commandCnt = 0;
	this->m_CommandTypeNow = OFFROADACTION_COMMAND_TYPE_VACANT;
	this->m_hWnd = NULL;
	this->m_lpCOffRoadActionImg = NULL;
	this->m_lpCommandType = NULL;
	this->m_lpCorrectInput = NULL;
	this->m_CommandMaterialNow = -1;

}

/*
************************************************************************************************
�f�B�X�g���N�^
************************************************************************************************
*/
COffRoadAction::~COffRoadAction() {
	//���͂̐���
	delete[] this->m_lpCorrectInput;
	this->m_lpCorrectInput = NULL;
}

//************************************************************************************************
//�A�N�V�����R�}���h�ł̏�����
//************************************************************************************************
void COffRoadAction::InitOffRoadAction(){
	
	//�Q�[����Փx���擾
	tagGAMELEVEL	gameLevel = GetGameLevel();

	//�R�}���h�\���Ԋu�f�[�^�e�[�u��
	int	commandIntervalTbl[LEVEL_MAX] = {
		
		COffRoadAction::OFFROADACTION_COMMAND_INTERVAL_TIME_EASY_MODE ,			//EASY
		COffRoadAction::OFFROADACTION_COMMAND_INTERVAL_TIME_NORMAL_MODE ,		//NORMAL
		COffRoadAction::OFFROADACTION_COMMAND_INTERVAL_TIME_HARD_MODE ,			//HARD
	};

	//�R�}���h�\���Ԋu�f�[�^�e�[�u��
	int	commandCreationTbl[LEVEL_MAX] = {
		
		COffRoadAction::OFFROADACTION_COMMAND_CREATION_MAX_EASY_MODE ,			//EASY
		COffRoadAction::OFFROADACTION_COMMAND_CREATION_MAX_NORMAL_MODE ,		//NORMAL
		COffRoadAction::OFFROADACTION_COMMAND_CREATION_MAX_HARD_MODE ,			//HARD
	};

	//�� �ǂݍ��މ摜�̃f�[�^�e�[�u��
	char	*imageTbl[ COffRoadAction::OFFROADACTION_IMAGE_MAX ] = {
	
		{"Image\\OffRoad\\ActionCommandFram.bmp"} ,			//�A�N�V�����R�}���h�̃t���[��
		{"Image\\OffRoad\\ActionCommand.bmp"} ,				//�A�N�V�����R�}���h
	};

	//�ǂݍ��ތ��ʉ��̃f�[�^�e�[�u��
	char	*seTbl[ COffRoadAction::OFFROADACTION_EFFECT_SOUND_MAX ] = {

		{ "SE\\cursor.wav"} ,		//�R�}���h������
		{ "SE\\cancel.wav"} ,		//�R�}���h���s��
	};

	//�� �R�}���h�A�N�V�����̓�Փx�ɉ������������z����m�ۂ���
	//�R�}���h�^�C�v
	this->m_lpCommandType = new tagCOMMANDACTIONTYPE[ commandCreationTbl[gameLevel] ];

	//���͂̐���
	this->m_lpCorrectInput = new CORRECTINPUTTYPE[ commandCreationTbl[gameLevel] ];
	for (int i = 0; i < commandCreationTbl[gameLevel]; i++) { this->m_lpCorrectInput[i] = COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_NORMAL; }

	//�� �������̊m��
	//����g�p����摜�̖������������������m�ۂ���
	this->m_lpCOffRoadActionImg = new int[ COffRoadAction::OFFROADACTION_IMAGE_MAX ];

	//CHARACTER�\���̗̂v�f���m�ۂ���
	//�@�A�N�V�����R�}���h�̃t���[��
	this->m_lpOffRoadActionFram = new CHARACTER;
	//0�ŏ�����
	ZeroMemory( this->m_lpOffRoadActionFram , sizeof( CHARACTER ) );

	//�A�A�N�V�����R�}���h
	this->m_lpOffRoadActionCommand = new CHARACTER[ commandCreationTbl[gameLevel] ];
	//0�ŏ�����
	ZeroMemory( this->m_lpOffRoadActionCommand , sizeof( CHARACTER ) * commandCreationTbl[gameLevel] );
	
	//���ʉ��̃��������m��
	this->m_lpCOffRoadActionSe = new int [ COffRoadAction::OFFROADACTION_EFFECT_SOUND_MAX ];

	//�� �\���ݒ�
	//�@�A�N�V�����R�}���h�̃t���[��
	this->m_lpOffRoadActionFram[0].x = COffRoadAction::OFFROADACTION_FRAM_DEFAULT_POSITION_X;			//�\�����WX
	this->m_lpOffRoadActionFram[0].y = COffRoadAction::OFFROADACTION_FRAM_DEFAULT_POSITION_Y;			//�\�����WY
	this->m_lpOffRoadActionFram[0].w = COffRoadAction::OFFROADACTION_FRAM_IMAGE_WIDTH;					//��
	this->m_lpOffRoadActionFram[0].h = COffRoadAction::OFFROADACTION_FRAM_IMAGE_HEIGHT;					//����
	this->m_lpOffRoadActionFram[0].flag = false;														//�\���t���O
	//�g�k�{�����Q�[�����x���ɉ����ĕύX������
	//�R�}���h�̐��ɉ��������������߂�
	float commandWidth = float(COffRoadAction::OFFROADACTION_COMMAND_IMAGE_WIDTH * commandCreationTbl[gameLevel]);
	this->m_lpOffRoadActionFram[0].scale = float(commandWidth / COffRoadAction::OFFROADACTION_FRAM_IMAGE_WIDTH + 0.1f);

	//�g�̐ݒ�擾�p
	//���W
	this->m_indicateFramPos.x = COffRoadAction::OFFROADACTION_FRAM_DEFAULT_POSITION_X;
	this->m_indicateFramPos.y = COffRoadAction::OFFROADACTION_FRAM_DEFAULT_POSITION_Y;
	//�T�C�Y
	this->m_FramSize.x = COffRoadAction::OFFROADACTION_FRAM_IMAGE_WIDTH;
	this->m_FramSize.y = COffRoadAction::OFFROADACTION_FRAM_IMAGE_HEIGHT;

	//�A�A�N�V�����R�}���h
	//�����R�}���h�̍ő吔
	this->m_commandMax = commandCreationTbl[gameLevel];

	for( int i = 0 ; i < this->m_commandMax ; i++ ){
		this->m_lpOffRoadActionCommand[i].w = COffRoadAction::OFFROADACTION_COMMAND_IMAGE_WIDTH;			//��
		this->m_lpOffRoadActionCommand[i].h = COffRoadAction::OFFROADACTION_COMMAND_IMAGE_HEIGHT;			//����
		this->m_lpOffRoadActionCommand[i].scale = COffRoadAction::OFFROADACTION_COMMAND_DEFAULT_SCALE;		//�g�k�{��
		this->m_lpOffRoadActionCommand[i].flag = false;														//�\���t���O
	}

	//�R�}���h�\���Ԋu
	this->m_actionInterval = commandIntervalTbl[gameLevel];

	//�� �摜�̓ǂݍ���
	for( int i = 0 ; i < COffRoadAction::OFFROADACTION_IMAGE_MAX ; i++ ){

		this->m_lpCOffRoadActionImg[i] = CreateDDImage( imageTbl[i] , COffRoadAction::m_hWnd );

	}

	//�� ���ʉ��̓ǂݍ���
	for( int i = 0 ; i < COffRoadAction::OFFROADACTION_EFFECT_SOUND_MAX ; i++ ){

		this->m_lpCOffRoadActionSe[i] = CreateSound( seTbl[i] );
	}


}

//************************************************************************************************
//�A�N�V�����R�}���h�̍X�V
//�߂�l �� �A�N�V���������̐���
//************************************************************************************************
bool COffRoadAction::UpdateOffRoadAction() {

	//�A�N�V�������s�t���O
	bool actionEror = false;

	//�R�}���h���ۂɉ�������ރf�[�^�e�[�u��
	CORRECTINPUTTYPE	correctTypeTbl[COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_MAX] = {

		COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_NORMAL	,
		COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_EROR		,
		COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_SUCCESS	,
	};

	//�� �R�}���h�̐��ۂɉ������X�V
	//�R�}���h�\�����Ȃ珈��
	if (this->m_lpOffRoadActionFram[0].flag) {
		for (int j = 0; j < COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_MAX; j++) {
			for (int i = this->m_CommandMaterialNow; i < this->m_commandMax; i++) {

				//�R�}���h���ۂɉ����ĉ摜�̍����ւ�
				if (this->m_lpCorrectInput[i] == correctTypeTbl[j]) {

					//1��ł����͂Ƀ~�X���Ă�����A�N�V���������t���O��false��
					if (this->m_lpCorrectInput[i] == COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_EROR) actionEror = true;
					//�\�������W��ύX
					this->m_lpOffRoadActionCommand[i].picy = correctTypeTbl[j] * this->m_lpOffRoadActionCommand[i].h;
					continue;
				}
			}
		}
	}
	//�A�N�V�������s�t���O
	return actionEror;
}


//************************************************************************************************
//�A�N�V�����R�}���h�̕\��
//************************************************************************************************
void COffRoadAction::DrawOffRoadAction(){


	//�A�N�V�����R�}���h�t���[���̕\��
	if( this->m_lpOffRoadActionFram[0].flag ){

		DDScaleOnlyWidthBlt( 
			this->m_lpOffRoadActionFram[0].x ,
			this->m_lpOffRoadActionFram[0].y ,
			this->m_lpOffRoadActionFram[0].w ,
			this->m_lpOffRoadActionFram[0].h ,
			this->m_lpCOffRoadActionImg[0] ,
			this->m_lpOffRoadActionFram[0].picx ,
			this->m_lpOffRoadActionFram[0].picy ,
			this->m_lpOffRoadActionFram[0].scale
			);
	}

	
	//�A�N�V�����R�}���h�̕\��
	for( int i = 0 ; i < this->m_commandMax ; i++ ){
		if( this->m_lpOffRoadActionCommand[i].flag ){

			DDScaleBlt( 
				this->m_lpOffRoadActionCommand[i].x ,
				this->m_lpOffRoadActionCommand[i].y ,
				this->m_lpOffRoadActionCommand[i].w ,
				this->m_lpOffRoadActionCommand[i].h ,
				this->m_lpCOffRoadActionImg[1] ,
				this->m_lpOffRoadActionCommand[i].picx ,
				this->m_lpOffRoadActionCommand[i].picy ,
				this->m_lpOffRoadActionCommand[i].scale
				);
		}
	}
	
}

//***************************************************************************************************
//���
//***************************************************************************************************
void COffRoadAction::ReleaseOffRoadAction(){

	//�摜
	//������Ȃ�������
	if( this->m_lpCOffRoadActionImg != NULL ){
		for( int i = 0 ; i < COffRoadAction::OFFROADACTION_IMAGE_MAX ; i++ ){
			ReleaseDDImage( this->m_lpCOffRoadActionImg[i] );
		}
	}
	delete []this->m_lpCOffRoadActionImg;
	this->m_lpCOffRoadActionImg = NULL;
	
	//���ʉ�
	//������Ȃ�������
	if( this->m_lpCOffRoadActionSe != NULL ){
		for( int i = 0 ; i < COffRoadAction::OFFROADACTION_EFFECT_SOUND_MAX ; i++ ){
			ReleaseSound( this->m_lpCOffRoadActionSe[i] );
		}
	}
	delete []this->m_lpCOffRoadActionSe;
	this->m_lpCOffRoadActionSe = NULL;

	//CHARACTER�\���̂̉��
	//�@�A�N�V�����R�}���h�̃t���[��
	delete this->m_lpOffRoadActionFram;
	this->m_lpOffRoadActionFram = NULL;

	//�A�A�N�V�����R�}���h
	delete[] this->m_lpOffRoadActionCommand;
	this->m_lpOffRoadActionCommand = NULL;

	//�A�N�V�����R�}���h�̎�ޔԍ��������z��
	delete[] this->m_lpCommandType;
	this->m_lpCommandType = NULL;

}

/*
***************************************************************************************************
������]�A�N�V�����R�}���h�̏�����
***************************************************************************************************
*/
void COffRoadAction::InitLevelRotateActionCommand(){

	//�A�N�V�����R�}���h�̎�ޔԍ��f�[�^�e�[�u��
	tagCOMMANDACTIONTYPE	commandTbl[ OFFROADACTION_COMMAND_KIND_OF_ACTION ]
	[ COffRoadAction::OFFROADACTION_COMMAND_CREATION_MAX_HARD_MODE] = {

		//�E�P��]
		{ OFFROADACTION_COMMAND_TYPE_UP		,		//��
		OFFROADACTION_COMMAND_TYPE_RIGHT	,		//�E
		OFFROADACTION_COMMAND_TYPE_DOWN		,		//��
		OFFROADACTION_COMMAND_TYPE_LEFT		,		//��
		OFFROADACTION_COMMAND_TYPE_UP		,		//��
		OFFROADACTION_COMMAND_TYPE_LEFT		,		//��
		OFFROADACTION_COMMAND_TYPE_DOWN		,		//��
		OFFROADACTION_COMMAND_TYPE_RIGHT    }		//�E
		,
		//���P��]
		{ OFFROADACTION_COMMAND_TYPE_UP		,		//��
		OFFROADACTION_COMMAND_TYPE_LEFT		,		//��
		OFFROADACTION_COMMAND_TYPE_DOWN		,		//��
		OFFROADACTION_COMMAND_TYPE_RIGHT	,		//�E
		OFFROADACTION_COMMAND_TYPE_UP		,		//��
		OFFROADACTION_COMMAND_TYPE_RIGHT	,		//�E
		OFFROADACTION_COMMAND_TYPE_DOWN		,		//��
		OFFROADACTION_COMMAND_TYPE_LEFT		}		//��
		,
		//�E����]
		{ OFFROADACTION_COMMAND_TYPE_UP		,		//��
		OFFROADACTION_COMMAND_TYPE_RIGHT	,		//�E
		OFFROADACTION_COMMAND_TYPE_UP		,		//��
		OFFROADACTION_COMMAND_TYPE_LEFT		,		//��
		OFFROADACTION_COMMAND_TYPE_UP		,		//��
		OFFROADACTION_COMMAND_TYPE_RIGHT	,		//�E
		OFFROADACTION_COMMAND_TYPE_VACANT	,
		OFFROADACTION_COMMAND_TYPE_VACANT	}
		,
		//������]
		{ OFFROADACTION_COMMAND_TYPE_UP		,		//��
		OFFROADACTION_COMMAND_TYPE_LEFT		,		//��
		OFFROADACTION_COMMAND_TYPE_UP		,		//��
		OFFROADACTION_COMMAND_TYPE_RIGHT	,		//�E
		OFFROADACTION_COMMAND_TYPE_UP		,		//��
		OFFROADACTION_COMMAND_TYPE_LEFT		,		//��
		OFFROADACTION_COMMAND_TYPE_VACANT	,
		OFFROADACTION_COMMAND_TYPE_VACANT	}

	};

	//�����_���̒l���擾
	int ran = RandomNum( 3 , 0 );

	//�����ő吔�܂Ń��[�v
	for( int i = 0 ; i < this->m_commandMax ; i++ ){

		//�R�}���h�Ȃ��Ȃ�break
		if( commandTbl[ran][i] == OFFROADACTION_COMMAND_TYPE_VACANT ) break;

		//�����R�}���h��
		this->m_commandCnt = ( i + 1 );

		//�\�����WX
		this->m_lpOffRoadActionCommand[i].x = COffRoadAction::OFFROADACTION_COMMAND_DEFAULT_POSITION_X + 
			( this->m_lpOffRoadActionCommand[i].w * this->m_lpOffRoadActionCommand[i].scale ) * i;
		//�\�����WY
		this->m_lpOffRoadActionCommand[i].y = COffRoadAction::OFFROADACTION_COMMAND_DEFAULT_POSITION_Y;
		//�\�������WX
		this->m_lpOffRoadActionCommand[i].picx = ( commandTbl[ran][i] - OFFROADACTION_COMMAND_TYPE_LEFT ) * 
			COffRoadAction::OFFROADACTION_COMMAND_IMAGE_WIDTH;				
		//�\������R�}���h�^�C�v
		this->m_lpCommandType[i] = commandTbl[ran][i];														//��ޔԍ�
	}

}


/*
***************************************************************************************************
�A�N�V�����R�}���h�̐���
***************************************************************************************************
*/
bool COffRoadAction::CreateActionCommand( bool actionFlag ){

	//������true�Ȃ�g��\��
	if( actionFlag && this->m_lpOffRoadActionFram[0].flag == false ){
		//�g�̕\��
		this->m_lpOffRoadActionFram[0].flag = true;
		//�R�}���h�̏�����
		//�����A�N�V����
		this->InitLevelRotateActionCommand();
	}


	//�g�̕\�����玞�Ԍo�߂ŃR�}���h��\�����Ă���
	if( this->m_lpOffRoadActionFram[0].flag ){

		//���񈵂��R�}���h���������[�v
		for( int i = 0 ; i < this->m_commandCnt ; i++ ){

			//�\���ς݂Ȃ玟�̗v�f��
			if( this->m_lpOffRoadActionCommand[i].flag ) continue;

			//���Ԍo��
			this->m_lpOffRoadActionCommand[i].time++;

			//�ݒ肵���t���[�����ɕ\��
			if (this->m_lpOffRoadActionCommand[i].time % (this->m_actionInterval * (i + 1)) == 0) {
				//subsutitute draw command flag for static variable
				this->m_CommandCreateFlag = true;
				//�R�}���h�̕\��
				this->m_lpOffRoadActionCommand[i].flag = true;
				//���ݕ\�������v�f����
				this->m_CommandMaterialNow = i;
				//�o�ߎ��Ԃ̃��Z�b�g
				this->m_lpOffRoadActionCommand[i].time = 0;
				//�r�b�g�ڂ��擾
				this->m_CommandTypeNow = this->m_lpCommandType[i];
				//���Ԍo�ߌ�̃R�}���h�̓G���[�\���ɂ���
				if (i > 0 && i < this->m_commandCnt) {					//�z��O�������
					//don't process when player's input was success and eror
					if (this->m_lpCorrectInput[i - 1] != COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_SUCCESS &&
						this->m_lpCorrectInput[i - 1] != COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_EROR) {
						this->m_lpCorrectInput[i - 1] = COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_EROR;
						PlaySound(this->m_lpCOffRoadActionSe[1], 0, false);
						//�\�������W��ύX
						this->m_lpOffRoadActionCommand[i - 1].picy = COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_EROR * this->m_lpOffRoadActionCommand[i - 1].h;
					}
				}
			}
		}
		//�Ō�̃A�N�V�����R�}���h�\����A�ݒ肵���\���Ԋu�o�ߌ�A��\���ɂ���
		if( this->m_lpOffRoadActionCommand[ ( this->m_commandCnt - 1 ) ].flag ){
			//���Ԍo��
			this->m_lpOffRoadActionCommand[ ( this->m_commandCnt - 1 ) ].time++;

			//���Ԍo�߂Ŕ�\���ɂ��A�o�ߎ��Ԃ����Z�b�g
			if( this->m_lpOffRoadActionCommand[ ( this->m_commandCnt - 1 ) ].time % 
				this->m_actionInterval == 0 ){
				//�t���[�����\����
				this->m_lpOffRoadActionFram[0].flag = false;
				//�\�������R�}���h�̗v�f�����Z�b�g
				this->m_CommandMaterialNow = -1;
				//�R�}���h�������t���O���擾�p�ɑ��
				this->m_CommandCreateFlag = false;
				//�o�ߎ��Ԃ����Z�b�g
				this->m_lpOffRoadActionCommand[ ( this->m_commandCnt - 1 ) ].time = 0;
				//�\�����̃A�N�V�����R�}���h�̎�ޔԍ������Z�b�g
				this->m_CommandTypeNow = OFFROADACTION_COMMAND_TYPE_VACANT;
				//�\�����ꂽ�R�}���h�̐�������\���ɂ��A�\�������W��ʏ��
				for( int i = 0 ; i < this->m_commandCnt ; i++ ){ 
					this->m_lpOffRoadActionCommand[i].flag = false;
					//�R�}���h�̐��ۂ����Z�b�g
					this->m_lpCorrectInput[i] = COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_NORMAL;
				}
			}
		}

	}

	//�g�\�����̓R�}���h�������t���O��Ԃ�
	return this->m_lpOffRoadActionFram[0].flag;

}

/*
************************************************************************************************
�A�N�V�����R�}���h�̓��͏󋵂�����
�߂�l
���� �F ���͂����L�[�R�[�h�ɑΉ������r�b�g��
���s �F -1
************************************************************************************************
*/
BYTE COffRoadAction::InputActionCommand(){
	
	//�R�}���h�̎�ޔԍ��f�[�^�e�[�u��
	tagCOMMANDACTIONTYPE	commandKindTbl1[ OFFROADACTION_COMMAND_KIND_OF_TYPE ] = {

		OFFROADACTION_COMMAND_TYPE_UP		,		//��
		OFFROADACTION_COMMAND_TYPE_RIGHT	,		//�E
		OFFROADACTION_COMMAND_TYPE_DOWN		,		//��
		OFFROADACTION_COMMAND_TYPE_LEFT		,		//��
	};

	//�Q�[���p�b�h�p
	BYTE	commandKindTbl2[OFFROADACTION_COMMAND_KIND_OF_TYPE] = {
		LEVER_UP,
		LEVER_RIGHT,
		LEVER_DOWN,
		LEVER_LEFT,
	};

	//�R�}���h�̃r�b�g�ڂ̃f�[�^�e�[�u��
	BYTE	commandBitTbl[ OFFROADACTION_COMMAND_KIND_OF_TYPE ] = {

		OFFROADACTION_COMMAND_BIT_TYPE_UP		,		//��
		OFFROADACTION_COMMAND_BIT_TYPE_RIGHT	,		//�E
		OFFROADACTION_COMMAND_BIT_TYPE_DOWN		,		//��
		OFFROADACTION_COMMAND_BIT_TYPE_LEFT		,		//��
	};

	//�߂�l�p
	BYTE	ret = OFFROADACTION_COMMAND_BIT_TYPE_VACANT;

	//���͂�1��̂�
	if( this->m_lpCorrectInput[ this->m_CommandMaterialNow ] == COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_EROR ||
		this->m_lpCorrectInput[ this->m_CommandMaterialNow ] == COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_SUCCESS ){
		return OFFROADACTION_COMMAND_BIT_TYPE_VACANT;
	}
	//���͏󋵂𒲂ׂ�
	for( int i = 0 ; i < OFFROADACTION_COMMAND_KIND_OF_TYPE ; i++ ){

		//���͂��擾
		if (JustKey(commandKindTbl1[i]) || JustLever(commandKindTbl2[i])) {

			//�Ή������R�}���h�̎�ޔԍ��i�R�[�h�ԍ��j�𒲂ׂ�
			if (this->m_CommandTypeNow == commandKindTbl1[i] || this->m_CommandTypeNow == commandKindTbl2[i]) {

				//���ʉ�
				PlaySound(this->m_lpCOffRoadActionSe[0], 0, false);

				//�R�}���h�̐��ۂ���
				this->m_lpCorrectInput[this->m_CommandMaterialNow] = COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_SUCCESS;

				//�Ή������R�}���h�̃r�b�g�ڂ�߂�l�ɑ��
				ret = commandBitTbl[i];
				break;
			}
			else {
				//�قȂ�R�}���h
				//���ʉ�
				PlaySound(this->m_lpCOffRoadActionSe[1], 0, false);
				//�R�}���h�̐��ۂ���
				this->m_lpCorrectInput[this->m_CommandMaterialNow] = COffRoadAction::OFFROADACTION_COMMAND_IMAGE_VARIATION_EROR;
				return ret;
			}
		}
	}

	return ret;

}