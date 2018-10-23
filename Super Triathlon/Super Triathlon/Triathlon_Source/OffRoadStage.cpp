#include	"OffRoadStage.h"
#include	"OffRoadRacer.h"
#include	"map.h"
#include	"play.h"
#include	"wipe.h"
#include	<stdio.h>
#include	"input.h"


//�� �ÓI�����o�ϐ��̒�`
const int COffRoadStage::OFFROADSTAGE_IMAGE_MAX = 5;							//�g�p����

//�w�i�̃T�C�Y
//�n��
const int COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GROUND_WIDTH = 640;			//��
const int COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GROUND_HEIGHT = 480;			//����
//�S�[���n�_
const int COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GOAL_POINT_WIDTH = 320;		//��
const int COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GOAL_POINT_HEIGHT = 40;		//����

//�w�i�̎�ސ�
const int COffRoadStage::OFFROADSTAGE_BACK_IMAGE_KIND_OF_TYPE = 2;

//�W�����v�|�C���g�̃T�C�Y
const int COffRoadStage::OFFROADSTAGE_JUMP_POINT_IMAGE_WIDTH = 128;			//��
const int COffRoadStage::OFFROADSTAGE_JUMP_POINT_IMAGE_HEIGHT = 64;			//����
//�g�k�{�������l
const float COffRoadStage::OFFROADSTAGE_JUMP_POINT_DEFAULT_SCALE = 1.0f;
//�\���w�̊Ԋu
const int COffRoadStage::OFFROADSTAGE_JUMP_POINT_INDICATE_X_SPACE = 100;
//�\���w�̔z�u�ő吔
const int COffRoadStage::OFFROADSTAGE_JUMP_POINT_INDICATE_X_POSITION_MAX = 3;
//�z�u�ő吔
const int COffRoadStage::OFFROADSTAGE_JUMP_POINT_POSITION_MAX = 1;

//�W�����v�n�_�̏��擾�p
CHARACTER* COffRoadStage::m_lpOffRoadGetJumpPoint = NULL;

//�E�B���h�E�n���h��
HWND	COffRoadStage::m_hWnd = NULL;

//�X�e�[�W�̋���
const int COffRoadStage::OFFROADSTAGE_DISTANCE_EAZY		= 20000;	//EAZY���[�h
const int COffRoadStage::OFFROADSTAGE_DISTANCE_NORMAL	= 30000;	//NORMAL���[�h
const int COffRoadStage::OFFROADSTAGE_DISTANCE_HARD		= 40000;	//HARD���[�h

//�X�e�[�W�̑S��������
float COffRoadStage::m_wholeDistance = 0;

//����̃X�e�[�W�̉�ʃT�C�Y
const int COffRoadStage::OFFROADSTAGE_SCREEN_WIDTH	= 640;		//��
const int COffRoadStage::OFFROADSTAGE_SCREEN_HEIGHT = 480;		//����

//setting brief map
 const int COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_WIDTH = 32;						//for back ground
 const int COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_HEIGHT = 420;
 const float COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_INDICATE_X = 598.0f;
 const float COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_INDICATE_Y = 30.0f;
 const float COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_RACER_DEFAULT_SCALE = 0.5f;		//for minimum racer
 const int COffRoadStage::OFFROADSTAGE_MINIMAP_KIND_OF_IMAGE = 2;						//kind of image

																		/*
************************************************************************************************
�f�B�X�g���N�^
************************************************************************************************
*/
COffRoadStage::~COffRoadStage() {

}

//************************************************************************************************
//����X�e�[�W�ł̏�����
//************************************************************************************************
void COffRoadStage::InitOffRoadStage() {

	//��Փx���擾
	tagGAMELEVEL gameLevel = GetGameLevel();

	//�X�e�[�W�̑S���f�[�^�e�[�u��
	int	distanceTbl[LEVEL_MAX] = {
		COffRoadStage::OFFROADSTAGE_DISTANCE_EAZY ,		//Easy
		COffRoadStage::OFFROADSTAGE_DISTANCE_NORMAL ,	//Normal
		COffRoadStage::OFFROADSTAGE_DISTANCE_HARD ,		//Hard
	};

	//�W�����v��̐����Ԋu
	int	CreationFramTbl[LEVEL_MAX] = { 200 , 150 , 70 };

	//��Փx�ɉ����ċ���������
	this->m_distance = (float)distanceTbl[gameLevel];
	//for get
	this->m_wholeDistance = (float)distanceTbl[gameLevel];

	//�W�����v��̐����Ԋu
	this->m_CreatrionFram = CreationFramTbl[gameLevel];

	//�� �ǂݍ��މ摜�̃f�[�^�e�[�u��
	char	*imageTbl[COffRoadStage::OFFROADSTAGE_IMAGE_MAX] = {
		{"Image\\OffRoad\\StageBackImage.bmp"} ,
		{"Image\\OffRoad\\JumpPoint.bmp"} ,
		{"Image\\OffRoad\\GoalLine.bmp"} ,
		{"Image\\Road\\Minimap.bmp"},
		{"Image\\OffRoad\\cyclist.bmp"},
	};

	//�� �摜�̐ݒ�
	//����g�p����摜�̖������������������m�ۂ���
	this->m_lpCOffRoadImg = new int[COffRoadStage::OFFROADSTAGE_IMAGE_MAX];

	//CHARACTER�\���̗̂v�f���m�ۂ���
	//�w�i
	this->m_lpOffRoad = new CHARACTER[COffRoadStage::OFFROADSTAGE_BACK_IMAGE_KIND_OF_TYPE];
	//0�ŏ�����
	ZeroMemory(this->m_lpOffRoad, sizeof(CHARACTER) * COffRoadStage::OFFROADSTAGE_BACK_IMAGE_KIND_OF_TYPE);
	//�W�����v��
	this->m_lpOffRoadJumpPoint = new CHARACTER[COffRoadStage::OFFROADSTAGE_JUMP_POINT_POSITION_MAX];
	//0�ŏ�����
	ZeroMemory(this->m_lpOffRoadJumpPoint, sizeof(CHARACTER) * COffRoadStage::OFFROADSTAGE_JUMP_POINT_POSITION_MAX);
	//�W�����v��擾�p
	this->m_lpOffRoadGetJumpPoint = new CHARACTER[COffRoadStage::OFFROADSTAGE_JUMP_POINT_POSITION_MAX];
	//0�ŏ�����
	ZeroMemory(this->m_lpOffRoadGetJumpPoint, sizeof(CHARACTER) * COffRoadStage::OFFROADSTAGE_JUMP_POINT_POSITION_MAX);
	//for brief map
	this->m_lpMinimap = new CHARACTER[COffRoadStage::OFFROADSTAGE_MINIMAP_KIND_OF_IMAGE];
	ZeroMemory(&this->m_lpMinimap[0], (sizeof(CHARACTER) * COffRoadStage::OFFROADSTAGE_MINIMAP_KIND_OF_IMAGE));
	//���C�_�[�̃N���X
	this->m_lpCOffRoadRacer = new COffRoadRacer;

	//�� �\���ݒ�
	//�w�i�u�n�ʁv
	this->m_lpOffRoad[0].w = COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GROUND_WIDTH;					//��
	this->m_lpOffRoad[0].h = COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GROUND_HEIGHT;					//����
	this->m_lpOffRoad[0].flag = true;																//�\���t���O

	//�S�[���n�_
	this->m_lpOffRoad[1].x = (COffRoadStage::OFFROADSTAGE_SCREEN_WIDTH -
		COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GOAL_POINT_WIDTH) / 2;								//�\����X
	this->m_lpOffRoad[1].y = 0.0f;																	//�\����Y
	this->m_lpOffRoad[1].w = COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GOAL_POINT_WIDTH;				//��
	this->m_lpOffRoad[1].h = COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GOAL_POINT_HEIGHT;				//����
	this->m_lpOffRoad[1].flag = true;																//�\���t���O

	//�W�����v�n�_
	for (int i = 0; i < COffRoadStage::OFFROADSTAGE_JUMP_POINT_POSITION_MAX; i++) {

		this->m_lpOffRoadJumpPoint[i].w = COffRoadStage::OFFROADSTAGE_JUMP_POINT_IMAGE_WIDTH;		//��
		this->m_lpOffRoadJumpPoint[i].h = COffRoadStage::OFFROADSTAGE_JUMP_POINT_IMAGE_HEIGHT;		//����
		this->m_lpOffRoadJumpPoint[i].scale = COffRoadStage::OFFROADSTAGE_JUMP_POINT_DEFAULT_SCALE;	//�g�k�{��
	}

	//initialize brief map
	this->m_lpMinimap[0].x = COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_INDICATE_X;
	this->m_lpMinimap[0].y = COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_INDICATE_Y;
	this->m_lpMinimap[0].w = COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_WIDTH;
	this->m_lpMinimap[0].h = COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_HEIGHT;
	this->m_lpMinimap[0].scale = 1.0f;
	this->m_lpMinimap[0].flag = true;
	//for a minimum cyclist
	//top width
	float cyclistW = this->m_lpCOffRoadRacer->OFFROADRACER_IMAGE_WIDTH * COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_RACER_DEFAULT_SCALE;
	this->m_lpMinimap[1].y = COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_INDICATE_Y + COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_HEIGHT;
	this->m_lpMinimap[1].w = this->m_lpCOffRoadRacer->OFFROADRACER_IMAGE_WIDTH;
	this->m_lpMinimap[1].h = this->m_lpCOffRoadRacer->OFFROADRACER_IMAGE_HEIGHT;
	this->m_lpMinimap[1].scale = COffRoadStage::OFFROADSTAGE_IMAGE_MINIMAP_RACER_DEFAULT_SCALE;
	this->m_lpMinimap[1].x = this->m_lpMinimap[0].x + ((this->m_lpMinimap[0].w - cyclistW) / 2);
	this->m_lpMinimap[1].flag = true;

	//�� �摜�̓ǂݍ���
	for (int i = 0; i < COffRoadStage::OFFROADSTAGE_IMAGE_MAX; i++) {

		this->m_lpCOffRoadImg[i] = CreateDDImage(imageTbl[i], COffRoadStage::m_hWnd);
	}

	//��ʂ͈̔͂�ύX
	ChangeCamera((float)(this->m_distance - COffRoadStage::OFFROADSTAGE_SCREEN_HEIGHT),
		(float)(this->m_distance),
		(float)(COffRoadStage::OFFROADSTAGE_SCREEN_WIDTH),
		(float)(this->m_distance));
}

//************************************************************************************************
//����X�e�[�W�̍X�V
//************************************************************************************************
void COffRoadStage::UpdateOffRoadStage() {

	//���C�_�[�̑��x���擾
	float racerSpeed = this->m_lpCOffRoadRacer[0].GetOffRoadRacerSpeed();
	//���C�_�[�̍��W���擾
	TPOINT<float>	riderPos = this->m_lpCOffRoadRacer[0].GetRiderPosition();

	//�S�������C�_�[�̈ړ��ʂŌ��Z
	this->m_distance += racerSpeed;

	//���C�_�[�̑��x����ɉ�ʂ͈̔͂�ύX
	ChangeCamera((float)(this->m_distance - COffRoadStage::OFFROADSTAGE_SCREEN_HEIGHT),
		(float)(this->m_distance),
		(float)(COffRoadStage::OFFROADSTAGE_SCREEN_WIDTH),
		(float)(this->m_distance));


	//�� ���C�_�[�̍��W���S�[���n�_�ɓ��B������A���U���g��
	if (this->m_lpCOffRoadRacer->GetRacerActionFlag() == false &&
		riderPos.y < (this->m_lpOffRoad[1].y + this->m_lpOffRoad[1].h)) {
		//�^�C�}�[��OFF�ɂ���
		ControlStageTime(false);
		//���U���g��
		CreateWipe(SCENE_RESULT, CHANGE_SCENE, WIPE_CONTRACTION);
	}

	//�W�����v�n�_����	�W�����v�䖢�������A�v���C���[���A�N�V�������łȂ���Ύ��Ԍo�߂Ő���
	if (this->m_lpOffRoadJumpPoint[0].flag == false &&
		this->m_lpCOffRoadRacer->GetRacerActionFlag() == false) {
		this->m_lpOffRoadJumpPoint[0].time++;
		// ���W�����v�n�_�̐���
		if (this->m_lpOffRoadJumpPoint[0].time >= this->m_CreatrionFram) {
			this->CreateJumpPoint();
			this->m_lpOffRoadJumpPoint[0].time = 0;
		}
	}

	//��ʊO�ɏo���琶���t���O��false
	for (int i = 0; i < COffRoadStage::OFFROADSTAGE_JUMP_POINT_POSITION_MAX; i++) {
		if (this->m_lpOffRoadJumpPoint[i].flag) {
			//�o������W�����v��̏���ݒ�
			this->SetJumpPointInfo(&this->m_lpOffRoadJumpPoint[i], i);
			this->m_lpOffRoadJumpPoint[i].flag = CollisionCamera(&this->m_lpOffRoadJumpPoint[i]);
		}
	}

	//indicate present cyclist's coordinateY in whole, that is brief map
	float indicateY = (riderPos.y / this->m_wholeDistance);
	float topH = this->m_lpMinimap[1].h * this->m_lpMinimap[1].scale;
	this->m_lpMinimap[1].y = ((this->m_lpMinimap[0].h * indicateY) - this->m_lpMinimap[0].y) + topH;
}


//************************************************************************************************
//����X�e�[�W�̕\��
//************************************************************************************************
void COffRoadStage::DrawOffRoadStage() {

	//�J�����̍��W
	float	pY = GetCameraY();

	//�w�i�̕\��
	if (this->m_lpOffRoad[0].flag) {

		DDBlt(
			(int)this->m_lpOffRoad[0].x,
			(int)this->m_lpOffRoad[0].y,
			this->m_lpOffRoad[0].w,
			this->m_lpOffRoad[0].h,
			this->m_lpCOffRoadImg[0],
			this->m_lpOffRoad[0].picx,
			this->m_lpOffRoad[0].picy
			);
	}

	//�W�����v�n�_�̕\��
	for (int i = 0; i < COffRoadStage::OFFROADSTAGE_JUMP_POINT_POSITION_MAX; i++) {

		if (this->m_lpOffRoadJumpPoint[i].flag) {

			DDBlt(
				(int)this->m_lpOffRoadJumpPoint[i].x,
				(int)this->m_lpOffRoadJumpPoint[i].y - (int)pY,
				this->m_lpOffRoadJumpPoint[i].w,
				this->m_lpOffRoadJumpPoint[i].h,
				this->m_lpCOffRoadImg[1],
				this->m_lpOffRoadJumpPoint[i].picx,
				this->m_lpOffRoadJumpPoint[i].picy
				);

		}
	}

	//�S�[���n�_�̕\��
	if (this->m_lpOffRoad[1].flag) {

		DDBlt(
			(int)this->m_lpOffRoad[1].x,
			(int)(this->m_lpOffRoad[1].y - pY),
			this->m_lpOffRoad[1].w,
			this->m_lpOffRoad[1].h,
			this->m_lpCOffRoadImg[2],
			this->m_lpOffRoad[1].picx,
			this->m_lpOffRoad[1].picy
			);
	}
	//draw brief map
	//material of image
	int image[COffRoadStage::OFFROADSTAGE_MINIMAP_KIND_OF_IMAGE] = { this->m_lpCOffRoadImg[3], this->m_lpCOffRoadImg[4] };
	for (int i = 0; i < COffRoadStage::OFFROADSTAGE_MINIMAP_KIND_OF_IMAGE; i++) {
		if (this->m_lpMinimap[i].flag) {
			DDScaleBltSelectIndicate(
				this->m_lpMinimap[i].x,
				this->m_lpMinimap[i].y,
				this->m_lpMinimap[i].w,
				this->m_lpMinimap[i].h,
				image[i],
				this->m_lpMinimap[i].picx,
				this->m_lpMinimap[i].picy,
				this->m_lpMinimap[i].scale,
				DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP);
		}
	}
}

//***************************************************************************************************
//���
//***************************************************************************************************
void COffRoadStage::ReleaseOffRoadStage() {

	//������Ȃ�������
	if (this->m_lpCOffRoadImg != NULL) {
		for (int i = 0; i < COffRoadStage::OFFROADSTAGE_IMAGE_MAX; i++) {
			ReleaseDDImage(this->m_lpCOffRoadImg[i]);
		}
	}
	delete[]this->m_lpCOffRoadImg;
	this->m_lpCOffRoadImg = NULL;

	//CHARACTER�\���̂̉��
	//�w�i
	delete[]this->m_lpOffRoad;
	this->m_lpOffRoad = NULL;
	//�W�����v��
	delete[]this->m_lpOffRoadJumpPoint;
	this->m_lpOffRoadJumpPoint = NULL;
	//�W�����v��擾�p
	delete this->m_lpOffRoadGetJumpPoint;
	this->m_lpOffRoadGetJumpPoint = NULL;

	//���C�_�[�̃N���X�̉��
	delete this->m_lpCOffRoadRacer;
	this->m_lpCOffRoadRacer = NULL;

	//for brief map
	delete[]this->m_lpMinimap;
	this->m_lpMinimap = NULL;

}

/*
***************************************************************************************************
�W�����v�n�_�̐���
***************************************************************************************************
*/
void COffRoadStage::CreateJumpPoint() {

	//�J�����̍��W
	float	pY = GetCameraY();

	//�\���w���W�f�[�^�e�[�u��
	float	posXTbl[COffRoadStage::OFFROADSTAGE_JUMP_POINT_INDICATE_X_POSITION_MAX] = {

		//����
		((COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GROUND_WIDTH - COffRoadStage::OFFROADSTAGE_JUMP_POINT_IMAGE_WIDTH) / 2) -
			COffRoadStage::OFFROADSTAGE_JUMP_POINT_INDICATE_X_SPACE ,
		//����
		(COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GROUND_WIDTH - COffRoadStage::OFFROADSTAGE_JUMP_POINT_IMAGE_WIDTH) / 2 ,
		//�E��
		((COffRoadStage::OFFROADSTAGE_BACK_IMAGE_GROUND_WIDTH - COffRoadStage::OFFROADSTAGE_JUMP_POINT_IMAGE_WIDTH) / 2) +
		COffRoadStage::OFFROADSTAGE_JUMP_POINT_INDICATE_X_SPACE

	};

	//�����_���̒l
	int ran = RandomNum(COffRoadStage::OFFROADSTAGE_JUMP_POINT_INDICATE_X_POSITION_MAX, 0);

	//�z�u�ő吔�܂Ń��[�v
	for (int i = 0; i < COffRoadStage::OFFROADSTAGE_JUMP_POINT_POSITION_MAX; i++) {
		if (this->m_lpOffRoadJumpPoint[i].flag == false) {


			//�\���ݒ�
			this->m_lpOffRoadJumpPoint[i].x = posXTbl[ran];
			this->m_lpOffRoadJumpPoint[i].y = pY - this->m_lpOffRoadJumpPoint[i].h;
			this->m_lpOffRoadJumpPoint[i].flag = true;

			break;

		}
	}

}

/*
***************************************************************************************************
�W�����v�n�_�ƃv���C���[�̏d�Ȃ�𒲂ׂ�
***************************************************************************************************
*/
bool	COffRoadStage::CollisionJumpPoint(CHARACTER *pch) {

	//�߂�l�p
	bool ret = false;

	//�\���ő吔�܂Ń��[�v
	for (int i = 0; i < COffRoadStage::OFFROADSTAGE_JUMP_POINT_POSITION_MAX; i++) {

		//�W�����v�䂪��������Ă����Ȃ������玟�̗v�f��
		if (this->m_lpOffRoadGetJumpPoint[i].flag == false) continue;

		//�����Ɣ�������
		if (CollisionCharacter(pch, &this->m_lpOffRoadGetJumpPoint[i])) {

			ret = true;
			break;
		}

	}

	return ret;

}
