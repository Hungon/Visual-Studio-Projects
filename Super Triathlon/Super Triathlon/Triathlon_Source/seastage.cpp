#include "seastage.h"
#include "swimer.h"
#include "play.h"
#include "sound.h"
#include "item.h"
#include "seaenemy.h"
#include "wipe.h"
#include <stdio.h>

//���O���[�o���ϐ�
//���摜�̗v�f
int g_seamapImg[SEA_MAP_IMAGE_MAX] = {0};

//�����ʉ�
int g_seaSound[SEA_EFFECT_SOUND_MAX] = {0};

//�C�摜�̏�������
STAGE g_seaBack[SEASTAGE_BACKGROUND_MAX] = {0};
//�I�u�W�F�N�g�̏�������
STAGE g_seaObject[SEA_OBJECT_MAX] = {0};
//�T�C���\��������
STAGE g_seaSign[SEA_SIGN_MAX] = {0};
//�~�j�}�b�v
STAGE g_SeaMiniMap[SEA_MINIMAP_KIMD_OF_IMAGE] = { 0 };

//���X�e�[�W�͈̔͂�����
float g_seaStageW = 0.0f;			//��
float g_seaStageH = 0.0f;			//����
float g_seaStageTop = 0.0f;			//�X�e�[�W�̏��
float g_seaStageBottom = 480.0f;	//�X�e�[�W�̉���


//************************************************************************************************
//�C�̃X�e�[�W�̏�����
//************************************************************************************************
void InitSea(HWND hWnd){

	//��Փx�ݒ�
	//�ݒ肵����Փx�ɉ����ĕ��򂷂�
	int sealevelTbl[LEVEL_MAX] = {
		15 , 25 , 35 ,
	};
	//�g�p����摜
	char	*lpImage[SEA_MAP_IMAGE_MAX] = {
		{ "Image\\Sea\\sea.bmp" },					//ocean
		{ "Image\\Sea\\upheaval.bmp" },				//upheaval
		{ "Image\\Sea\\bubble.bmp" },				//bubble
		{ "Image\\Sea\\caution.bmp" },				//caution image
		{ "Image\\Sea\\Tide.bmp" },					//tide
		{ "Image\\Sea\\ground.bmp" },				//sea's ground
		{ "Image\\Sea\\Minimap.bmp" },				//brief map
		{ "Image\\Sea\\swimer.bmp"},				//swimer image for brief map
		{ "Image\\Sea\\Flag.bmp"},					//the half way's point and goal's point
	};
	//���摜�̓ǂݍ���
	for (int i = 0; i < SEA_MAP_IMAGE_MAX; i++) { g_seamapImg[i] = CreateDDImage(lpImage[i], hWnd); }
	
	//�����ʉ��̓ǂݍ���
	//�S�[����
	g_seaSound[0] = CreateSound("SE\\goal.wav");
	//CAUTION�x����
	g_seaSound[1] = CreateSound("SE\\caution.wav");
	//�C���̉�
	g_seaSound[2] = CreateSound("SE\\stream.wav");

	//�C�摜�̏������Z�b�g
	ZeroMemory(&g_seaBack[0], sizeof(STAGE) * SEASTAGE_BACKGROUND_MAX);
	//�I�u�W�F�N�g�̏������Z�b�g
	ZeroMemory(&g_seaObject[0], sizeof(STAGE) * SEA_OBJECT_MAX);
	//�T�C�����̃��Z�b�g
	ZeroMemory(&g_seaSign[0], sizeof(STAGE) * SEA_SIGN_MAX);
	//�~�j�}�b�v
	ZeroMemory(&g_SeaMiniMap[0], sizeof(STAGE) * SEA_MINIMAP_KIMD_OF_IMAGE);

	//�C�̔w�i�̐ݒ�
	//�X�N���[���ݒ�
	g_seaBack[0].scrollx = true;	//�n�߂͉��X�N���[��
	g_seaBack[0].scroll_l = true;	//���X�N���[���t���O
	g_seaBack[0].scroll_r = false;	//�E�X�N���[���t���O
	//�w�i�̐ݒ�
	for (int i = 0; i < SEASTAGE_BACKGROUND_MAX; i++) {
		g_seaBack[i].ch.w = 640;
		g_seaBack[i].ch.h = 480;
	}
	g_seaBack[0].ch.flag = true;				//�C
	g_seaBack[1].ch.flag = true;				//�C��

	//initialize brief map that is indication setting
	//back ground image
	g_SeaMiniMap[0].ch.w = SEA_MINIMAP_IMAGE_BACK_GROUND_WIDTH;
	g_SeaMiniMap[0].ch.h = SEA_MINIMAP_IMAGE_BACK_GROUND_HEIGHT;
	g_SeaMiniMap[0].ch.x = SEA_MINIMAP_IMAGE_BACK_GROUND_INDICATE_X;
	g_SeaMiniMap[0].ch.y = SEA_MINIMAP_IMAGE_BACK_GROUND_INDICATE_Y;
	g_SeaMiniMap[0].ch.flag = true;
	g_SeaMiniMap[0].ch.alpha = 120.0f;
	//minimum player
	float swimerTopHeight = SWIMER_DEFAULT_IMAGE_HEIGHT * SEA_MINIMAP_IMAGE_SWIMER_SCALE_RATE;
	float swimerTopWidth = SWIMER_DEFAULT_IMAGE_WIDTH * SEA_MINIMAP_IMAGE_SWIMER_SCALE_RATE;
	g_SeaMiniMap[1].ch.w = SWIMER_DEFAULT_IMAGE_WIDTH;
	g_SeaMiniMap[1].ch.h = SWIMER_DEFAULT_IMAGE_HEIGHT;
	g_SeaMiniMap[1].ch.scale = SEA_MINIMAP_IMAGE_SWIMER_SCALE_RATE;
	g_SeaMiniMap[1].ch.x = g_SeaMiniMap[0].ch.x - swimerTopWidth;
	g_SeaMiniMap[1].ch.y = g_SeaMiniMap[0].ch.y + (g_SeaMiniMap[0].ch.h - swimerTopHeight);
	g_SeaMiniMap[1].ch.flag = true;
	float flagPosX[2] = { 
		(SEA_MINIMAP_IMAGE_BACK_GROUND_INDICATE_X + SEA_MINIMAP_IMAGE_BACK_GROUND_WIDTH) - SEA_MINIMAP_IMAGE_FLAG_WIDTH,
		SEA_MINIMAP_IMAGE_BACK_GROUND_INDICATE_X,
	};
	for (int i = 2; i < SEA_MINIMAP_KIMD_OF_IMAGE; i++) {		//setting of flag. these are goal and the half point
		g_SeaMiniMap[i].ch.h = SEA_MINIMAP_IMAGE_FLAG_HEIGHT;
		g_SeaMiniMap[i].ch.w = SEA_MINIMAP_IMAGE_FLAG_WIDTH;
		g_SeaMiniMap[i].ch.picy = SEA_MINIMAP_IMAGE_FLAG_HEIGHT * (i - 2);
		g_SeaMiniMap[i].ch.x = flagPosX[i - 2];
		g_SeaMiniMap[i].ch.y = SEA_MINIMAP_IMAGE_BACK_GROUND_INDICATE_Y;
	}
	//starting drawing is flag of the half point
	g_SeaMiniMap[2].ch.flag = true;

	//���X�e�[�W�͈̔͂̐ݒ�
	//����
	g_seaStageH = 7 * STAGE_HEIGHT;
	//��
	g_seaStageW = (float)( sealevelTbl[GetGameLevel()] * STAGE_WIDTH );
	//���
	g_seaStageTop = 0;
	//����
	g_seaStageBottom = 480;
	
	//�C�ꂩ��˂��o������̐ݒ�
	g_seaObject[0].ch.x = 640.0f - 256.0f;
	g_seaObject[0].ch.y = 480.0f - 320;		//�\����x���W
	g_seaObject[0].ch.h = 320;			//����
	g_seaObject[0].ch.w = 256;			//��
	g_seaObject[0].ch.scale = 1.0f;	//�g�k��
	g_seaObject[0].ch.flag = true;	//�����t���O
	g_seaObject[0].obj_popx = 320.0f;	//�����N�_�w���W�i���̒l�̔{���̍��W�ɔ����j
	g_seaObject[0].obj_endx = g_seaStageW - 640.0f;//�����N�_�w���W
	g_seaObject[0].movex = 640.0f;				//�o����w���W

	//�uCAUTIOM�v�̐ݒ�
	g_seaSign[0].ch.x = 200;		//X���W
	g_seaSign[0].ch.y = 100;		//Y���W
	g_seaSign[0].ch.h = 48;			//����
	g_seaSign[0].ch.w = 250;		//��
	g_seaSign[0].ch.flag = false;
	//�uCAUTION�v�̃A�j���[�V�����ݒ�
	g_seaSign[0].ani.fram = 15;		//�t���[����
	g_seaSign[0].ani.max = 4;			//�ő�\����
	//�A�j���[�V�����̏�����
	SetAnimation(&g_seaSign[0].ch , &g_seaSign[0].ani ,
				g_seaSign[0].ani.sx , g_seaSign[0].ani.sy ,
				g_seaSign[0].ani.max ,
				g_seaSign[0].ani.fram ,
				0 , true);	

	//�J�����̍��W��ݒ肷��
	ChangeCamera( (float)g_seaStageTop , (float)g_seaStageBottom ,
				 (float)g_seaStageW , (float)g_seaStageH);
}

//************************************************************************************************
//�C�X�e�[�W�̍X�V
//************************************************************************************************
void UpdateSea(){

	//�v���C���[�̏����擾
	SWIMER* pch = GetSwimer();
	//�A�C�e���̏����擾
	ITEM* pit = GetItem();
	//�G�l�~�[�̏����擾
	ENEMY* pen = GetEnemy();

	//�J�����̍��W���擾
	float comeCameraX = GetCameraX();		//X���W
	float comeCameraY = GetCameraY();		//Y���W

	//���S�[���̐ݒ�
	//���X�N���[���ɐ؂�ւ���Ă���A�ݒ肵���w���W�܂œ��B�����烊�U���g��ʂ�
	if(g_seaBack[0].scroll_r && pch->ch.x < (STAGE_WIDTH >> 1)){
		//�^�C�}�[��OFF�ɂ���
		ControlStageTime(false);
		//���ʉ�
		PlaySound( g_seaSound[0] , 0 , false );
		//�V�[���؂�ւ�
		CreateWipe( SCENE_RESULT , CHANGE_SCENE , WIPE_CONTRACTION );
	}

	
	//���I�u�W�F�N�g�̏o��
	//�C�ꂩ��ˋN������
	//�x���W���X�V����
	g_seaObject[0].ch.y = comeCameraY + (STAGE_HEIGHT - g_seaObject[0].ch.h);
	//�v���C���[�̌����ɂ���ďo����w���W��ύX
	//�E����
	if(pch->ch.direction == D_RIGHT){
		g_seaObject[0].movex = 640.0f;
	}
	//������
	if(pch->ch.direction == D_LEFT){
		g_seaObject[0].movex = -320.0f;
	}
	//�v���C���[�̂w���W����ɁA�I�u�W�F�N�g�Ɋ��蓖�Ă��o�����W�w�̒l�ŏ�]�Z���A
	//�]�肪�O�Ȃ�o��������
	if(g_seaObject[0].ch.flag == false && (int)pch->ch.x % (int)g_seaObject[0].obj_popx == 0 &&
		pch->ch.x < g_seaObject[0].obj_endx){	
		//�I�u�W�F�N�g��X���W���X�V
		g_seaObject[0].ch.x = comeCameraX + g_seaObject[0].movex;
		//�\���t���O��true�ɂ���
		g_seaObject[0].ch.flag = true;
	}
	//�J�����̘g����o����false��
	if(g_seaObject[0].ch.flag) g_seaObject[0].ch.flag = CollisionCamera(&g_seaObject[0].ch);

	//�������X�N���[���O�ɒ��ӊ��N�̉摜���A�j���[�V�����ŕ\��
	if(pch->ch.x > g_seaStageW - STAGE_WIDTH * 2 &&
		pch->ch.y < STAGE_HEIGHT){
			
		//�x����
		if(g_seaSign[0].ch.flag == false){
			PlaySound(g_seaSound[1] , 0 , false);
		}

		//�����t���O��true�ɂ���
		g_seaSign[0].ch.flag = true;
		
		//�uCAUTION�v�摜�̕\���t���O��true�Ȃ�
		if(g_seaSign[0].ch.flag == true){	
			//�A�j���[�V�����X�V
			UpdateAnimation(&g_seaSign[0].ch , &g_seaSign[0].ani);
		}
	}
	
	//�v���C���[�̍��W������̍��W�ɒB������w�i���㉺�ɐ��񂵁A�X�N���[���t���O��true�ɂ���
	if(pch->ch.x > g_seaStageW - STAGE_WIDTH &&
		pch->ch.y < STAGE_HEIGHT){
		
		//���X�N���[���t���O��false�ɂ���
		g_seaBack[0].scrollx = false;
		//�c�X�N���[���t���O��true�ɂ���
		g_seaBack[0].scrolly = true;
		//�uCAUTION�v���\����
		g_seaSign[0].ch.flag = false;
		//draw tide
		g_seaBack[2].ch.flag = true;
	}

	//�c�X�N���[���t���O�i���ړ��j��true�Ȃ�w��̍��W�܂ŉ��֋����X�N���[��
	if(g_seaBack[0].scrolly == true && pch->ch.y < (g_seaStageH - STAGE_HEIGHT)){
		
		//�v���C���[��Y���W���ݒ肵���c�̖ʂ̍Œ�ʂ̏�[��Y���W�܂œ��B����܂�
		//�����t���O��false�ɂ��ړ��𐧌����A���֋����ړ�
		//�C���̉�
		PlaySound(g_seaSound[2] , 0 , true);
		//���v���C���[�̏���
		pch->ch.speed = 0.0f;	//���x�{���𖳌���		
		pch->ch.my = 5;			//���ֈړ�
		//�������E�ɂ��炵�Ă���
		if(pch->ch.x < comeCameraX + 500){
			pch->ch.x += 2;		//�E������
		}
		//�v���C���[��Y���W�����Z���āA��ʉ��[�̒l��ݒ�
		g_seaStageBottom += (int)pch->ch.my;
		//���ӂ���ɏ�ӂ��X�V
		g_seaStageTop = g_seaStageBottom - STAGE_HEIGHT;
	}
	
	//�v���C���[�̍��W���w��̍��W���z������A�c�X�N���[���t���O��false�ɂ��A
	//�v���C���[�̈ړ��������E���獶�֕ύX����i���ړ��j
	if(g_seaBack[0].scrolly == true && pch->ch.y >= (g_seaStageH - STAGE_HEIGHT)){
		
		//�C���̉���~
		StopSound(g_seaSound[2]);
		//don't draw tide
		g_seaBack[2].ch.flag = false;
		//�c�X�N���[���t���O��false�ɂ���
		g_seaBack[0].scrolly = false;
		//���X�N���[���t���O��true�ɂ���
		g_seaBack[0].scrollx = true;
		//���X�N���[������E�X�N���[���փt���O��؂�ւ���
		g_seaBack[0].scroll_l = false;	//���X�N���[���t���O
		g_seaBack[0].scroll_r = true;	//�E�X�N���[���t���O
		//change coordinateX of brief map and minimum swimer
		g_SeaMiniMap[0].ch.x = STAGE_WIDTH - (g_SeaMiniMap[0].ch.w + SEA_MINIMAP_IMAGE_BACK_GROUND_INDICATE_X);
		//change origin coordinateY of swimer
		g_SeaMiniMap[1].ch.picy = SWIMER_DEFAULT_IMAGE_HEIGHT;		//change facing direction
		//don't draw the half point
		g_SeaMiniMap[2].ch.flag = false;
		//draw goal's flag
		g_SeaMiniMap[3].ch.flag = true;
		//subsutitute coordinateX of the brief map for coordinateY of goal's variable
		g_SeaMiniMap[3].ch.x = g_SeaMiniMap[0].ch.x;
	}

	//�J�����̍��W���X�V����
	ChangeCamera( g_seaStageTop , g_seaStageBottom ,
				g_seaStageW , g_seaStageH );

	//�v���C���[��X���W���X�e�[�W�̑S���̂ǂ̂��炢�̊����Ɉʒu���Ă��邩�Ń~�j�}�b�v��̃v���C���[�̍��W���X�V����
	float topW = g_SeaMiniMap[1].ch.w * g_SeaMiniMap[1].ch.scale;
	g_SeaMiniMap[1].ch.x = (g_SeaMiniMap[0].ch.w * (pch->ch.x / g_seaStageW) + g_SeaMiniMap[0].ch.x) - topW;
}

//************************************************************************************************
//�C�X�e�[�W�̕\��
//************************************************************************************************
void DrawSea(){
		

	//�J�����̍��W���擾
	float comeCameraX = GetCameraX();		//X���W
	float comeCameraY = GetCameraY();		//Y���W

	//�C�̔w�i��\��
	//�C�A�n�ʁA����
	int backImg[SEASTAGE_BACKGROUND_MAX] = { g_seamapImg[0], g_seamapImg[5], g_seamapImg[4] };
	for (int i = 0; i < SEASTAGE_BACKGROUND_MAX; i++) {
		if (g_seaBack[i].ch.flag) {
			DDBlt(
				(int)g_seaBack[i].ch.x,
				(int)g_seaBack[i].ch.y,
				g_seaBack[i].ch.w,
				g_seaBack[i].ch.h,
				backImg[i],
				g_seaBack[i].ch.picx,
				g_seaBack[i].ch.picy);
		}
	}
	//���N������̉摜
	//�����t���O��true�Ȃ�\��
	if(g_seaObject[0].ch.flag == true){
		DDScaleBlt(
			g_seaObject[0].ch.x - comeCameraX ,
			g_seaObject[0].ch.y - comeCameraY ,
			g_seaObject[0].ch.w ,
			g_seaObject[0].ch.h ,
			g_seamapImg[1] ,
			g_seaObject[0].ch.picx ,
			g_seaObject[0].ch.picy ,
			g_seaObject[0].ch.scale);
	}
	//�uCAUTION�v�̉摜
	//�����t���O��true�Ȃ�\��
	if(g_seaSign[0].ch.flag == true){
		DDBlt(
			(int)g_seaSign[0].ch.x ,
			(int)g_seaSign[0].ch.y ,
			g_seaSign[0].ch.w ,
			g_seaSign[0].ch.h ,
			g_seamapImg[3] ,
			g_seaSign[0].ch.picx,
			g_seaSign[0].ch.picy);
	}
	//draw brief map and minimum swimer
	if (g_SeaMiniMap[0].ch.flag) {				//back ground
		DDAlphaBlt(
			g_SeaMiniMap[0].ch.x,
			g_SeaMiniMap[0].ch.y,
			g_SeaMiniMap[0].ch.w,
			g_SeaMiniMap[0].ch.h,
			g_seamapImg[6],
			g_SeaMiniMap[0].ch.picx,
			g_SeaMiniMap[0].ch.picy,
			g_SeaMiniMap[0].ch.alpha);
	}
	if (g_SeaMiniMap[1].ch.flag) {				//swimer
		DDScaleBltSelectIndicate(
			g_SeaMiniMap[1].ch.x,
			g_SeaMiniMap[1].ch.y,
			g_SeaMiniMap[1].ch.w,
			g_SeaMiniMap[1].ch.h,
			g_seamapImg[7],
			g_SeaMiniMap[1].ch.picx,
			g_SeaMiniMap[1].ch.picy,
			g_SeaMiniMap[1].ch.scale,
			DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP);
	}
	//draw flag
	for (int i = 2; i < SEA_MINIMAP_KIMD_OF_IMAGE; i++) {
		if (g_SeaMiniMap[i].ch.flag) {				//the half way's point and goal's point
			DDBlt(
				g_SeaMiniMap[i].ch.x,
				g_SeaMiniMap[i].ch.y,
				g_SeaMiniMap[i].ch.w,
				g_SeaMiniMap[i].ch.h,
				g_seamapImg[8],
				g_SeaMiniMap[i].ch.picx,
				g_SeaMiniMap[i].ch.picy);
		}
	}
}

//*************************************************************************************************
//���
//*************************************************************************************************
void ReleaseSea(){
	
	//�摜�̉��
	for (int i = 0; i < SEA_MAP_IMAGE_MAX; i++) {
		ReleaseDDImage(g_seamapImg[i]);
	}
	//���̉��
	for (int i = 0; i < SEA_EFFECT_SOUND_MAX; i++) {
		ReleaseSound(g_seaSound[i]);
	}
}

//************************************************************************************************
//�C�X�e�[�W�̏���Ԃ�
//************************************************************************************************
STAGE* GetSeaStage() { return &g_seaBack[0]; }
