#include "seaenemy.h"
#include "map.h"
#include "input.h"
#include "swimer.h"
#include "item.h"
#include "effect.h"
#include "play.h"
#include "sound.h"
#include "seastage.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//���O���[�o���ϐ�
ENEMY g_enemy[ENEMY_MAX] = {0};			//���W�A�ړ��ʁA�T�C�Y�A�����t���O�������\���̌^�̕ϐ�
int g_enemyImg[ENEMY_IMG] = {0};		//�r�b�g�}�b�v�I�u�W�F�N�g�A�������f�o�C�X�R���e�L�X�g
//���ݏo�����G�l�~�[�̗v�f������
int g_enemy_type_now = 0;
//���O�ɏo�������G�l�~�[�̗v�f������
int g_enemy_type_old = 0;
//���ʉ�
int g_en_sound[ENEMY_SOUND_EFFECT_MAX] = {0};
//�o��������G�̐�
int g_enemyMax = ENEMY_MAX;


//************************************************************************************************
//������
//************************************************************************************************
void InitEnemy(HWND hWnd){

	//�Q�[���̓�Փx���擾
	tagGAMELEVEL getGameLevel = GetGameLevel();
	
	//�C�X�e�[�W�̏����擾����
	STAGE* pst = GetSeaStage();

	//�C�X�e�[�W�̕����擾
	float getStageWidth = GetStageWidth();

	//��Փx�ɉ����ēG�̏o���Ԋu��ύX����u�N���Q�A�T���A���Q�A�}���{�[�A�G�C�v�̏���
	int enemyframTbl[LEVEL_MAX][KIND_ENEMY_SEA] = {
		
		{ 100 , 100 , 100 , 0 , 0 } ,			//EASY
		{ 90 , 100 , 110 , 110 , 100 } ,		//NORMAL
		{ 80 , 90 , 100 , 100 , 90 } ,			//HARD
	};

	//��Փx�ɉ����ēG�̏o�����鐔��ύX����
	int enemyappearTbl[LEVEL_MAX][KIND_ENEMY_SEA] = {

		{ 2 , 4 , 3 , 0 , 0 } ,			//EASY
		{ 2 , 4 , 3 , 3 , 3 } ,			//NORMAL
		{ 3 , 5 , 4 , 4 , 4 } ,			//HARD
	};

	//��Փx�ɉ����ďo��������G�̍ő吔��ύX����
	int enemymaxTbl[LEVEL_MAX] = {
		5 , 7 , ENEMY_MAX ,
	};

	//�o��������ő吔����
	g_enemyMax = enemymaxTbl[getGameLevel];

	//�G�l�~�[�̏�����
	ZeroMemory(g_enemy, sizeof(ENEMY) * ENEMY_MAX);
	//���ݏo�����G�l�~�[��������
	g_enemy_type_now = 0;
	//���O�ɏo�������G�l�~�[��������
	g_enemy_type_old = 0;

	//�e�G�l�~�[�̐ݒ�
	for (int i = 0;i < g_enemyMax ; i++ ){
	
		g_enemy[i].ch.direction = ENEMY_DIRECTION_LEFT;			//�����u���v
		g_enemy[i].ch.scale = 1.0f;								//�g�k�{��
		//�o�����W
		g_enemy[i].popx = 640.0f;								//X
		g_enemy[i].popy = 480.0f;								//Y
	}

	//���e�G�l�~�[�̏o���E�����N�_�w���W
	//���X�N���[����
	//�N���Q�@�v�f�O
	g_enemy[JELLYFISH].appear_start_posx = 500.0f;									//�o���N�_���W�w
	g_enemy[JELLYFISH].appear_end_posx = (float)(getStageWidth / 5) * 2;			//�����N�_���W�w
	g_enemy[JELLYFISH].appear_type = JELLYFISH;										//�o���^�C�v
	g_enemy[JELLYFISH].appear = enemyappearTbl[getGameLevel][JELLYFISH];			//�o����
	g_enemy[JELLYFISH].appear_fram = enemyframTbl[getGameLevel][JELLYFISH];			//�o���t���[���Ԋu
	
	//�T���@�v�f�P
	g_enemy[SHARK].appear_start_posx = g_enemy[JELLYFISH].appear_end_posx + 500.0f;	//�o���N�_���W�w
	g_enemy[SHARK].appear_end_posx = (float)(getStageWidth / 5) * 4;				//�����N�_���W�w
	g_enemy[SHARK].appear_type = SHARK;												//�o���^�C�v
	g_enemy[SHARK].appear = enemyappearTbl[getGameLevel][SHARK];					//�o����
	g_enemy[SHARK].appear_fram = enemyframTbl[getGameLevel][SHARK];					//�o���t���[���Ԋu
	
	//���Q�@�v�f�Q
	g_enemy[SHOAL].appear_start_posx = g_enemy[SHARK].appear_end_posx + 500.0f;		//�o���N�_���W�w
	g_enemy[SHOAL].appear_end_posx = (float)getStageWidth - 1000.0f;				//�����N�_���W�w
	g_enemy[SHOAL].appear_type = SHOAL;												//�o���^�C�v
	g_enemy[SHOAL].appear = enemyappearTbl[getGameLevel][SHOAL];					//�o����
	g_enemy[SHOAL].appear_fram = enemyframTbl[getGameLevel][SHOAL];					//�o���t���[���Ԋu
	
	//�}���{�[
	g_enemy[SUNFISH].appear_type = SUNFISH;											//�o���^�C�v
	g_enemy[SUNFISH].appear = enemyappearTbl[getGameLevel][SUNFISH];				//�o����
	g_enemy[SUNFISH].appear_fram = enemyframTbl[getGameLevel][SUNFISH];				//�o���t���[���Ԋu
	
	//�G�C
	g_enemy[RAY].appear_type = RAY;													//�o���^�C�v
	g_enemy[RAY].appear = enemyappearTbl[getGameLevel][RAY];						//�o����
	g_enemy[RAY].appear_fram = enemyframTbl[getGameLevel][RAY];						//�o���t���[���Ԋu


	//���o���E��������w���W�N�_��ύX
	//�E�X�N���[����
	//EASY���[�h��
	if(getGameLevel == EASY_MODE){
		//���Q
		g_enemy[SHOAL].appear_start_posx_r = (float)(getStageWidth / 3) * 1 - 500.0f;//�n�_
		g_enemy[SHOAL].appear_end_posx_r = 640.0f;//�I�_
		//�T��
		g_enemy[SHARK].appear_start_posx_r = (float)(getStageWidth / 3) * 2 - 500.0f;//�n�_
		g_enemy[SHARK].appear_end_posx_r = g_enemy[SHOAL].appear_start_posx_r + 250.0f;//�I�_
		//�N���Q
		g_enemy[JELLYFISH].appear_start_posx_r = (float)getStageWidth - 500.0f;	//�n�_
		g_enemy[JELLYFISH].appear_end_posx_r = g_enemy[SHARK].appear_start_posx_r + 250.0f;//�I�_
	}
	//NORMAL���[�h��
	if(getGameLevel == NORMAL_MODE){
		//�G�C
		g_enemy[RAY].appear_start_posx_r = (float)(getStageWidth / 3) * 1 - 500.0f;//�n�_
		g_enemy[RAY].appear_end_posx_r = 640.0f;//�I�_
		//�}���{�[
		g_enemy[SUNFISH].appear_start_posx_r = (float)(getStageWidth / 3) * 2 - 500.0f;//�n�_
		g_enemy[SUNFISH].appear_end_posx_r = g_enemy[RAY].appear_start_posx_r + 250.0f;//�I�_
		//���Q
		g_enemy[SHOAL].appear_start_posx_r = (float)getStageWidth - 500.0f;	//�n�_
		g_enemy[SHOAL].appear_end_posx_r = g_enemy[SUNFISH].appear_start_posx_r + 250.0f;//�I�_
	}
	//HARD���[�h��
	if(getGameLevel == HARD_MODE){
		//�G�C
		g_enemy[RAY].appear_start_posx_r = (float)(getStageWidth / 5) * 1 - 500.0f;//�n�_
		g_enemy[RAY].appear_end_posx_r = 640.0f;//�I�_
		//�}���{�[
		g_enemy[SUNFISH].appear_start_posx_r = (float)(getStageWidth / 5) * 2 - 500.0f;//�n�_
		g_enemy[SUNFISH].appear_end_posx_r = g_enemy[RAY].appear_start_posx_r + 250.0f;//�I�_
		//���Q
		// AppearStartPos
		// appearStartPosX
		g_enemy[SHOAL].appear_start_posx_r = (float)(getStageWidth / 5) * 3 - 500.0f;	//�n�_
		g_enemy[SHOAL].appear_end_posx_r = g_enemy[SUNFISH].appear_start_posx_r + 250.0f;//�I�_
		//�T��
		g_enemy[SHARK].appear_start_posx_r = (float)(getStageWidth / 5) * 4 - 500.0f;	//�n�_
		g_enemy[SHARK].appear_end_posx_r = g_enemy[SHOAL].appear_start_posx_r + 250.0f;//�I�_
		//�N���Q
		g_enemy[JELLYFISH].appear_start_posx_r = (float)getStageWidth - 500.0f;	//�n�_
		g_enemy[JELLYFISH].appear_end_posx_r = g_enemy[SHARK].appear_start_posx_r + 250.0f;//�I�_
	}

	//���摜�̓ǂݍ���
	//���炰�̉摜��ǂݍ���
	g_enemyImg[0] = CreateDDImage("Image\\Sea\\jellyfish.bmp" , hWnd);
	//�T���̉摜��ǂݍ���
	g_enemyImg[1] = CreateDDImage("Image\\Sea\\shark.bmp" , hWnd);
	//���Q�̉摜��ǂݍ���
	g_enemyImg[2] = CreateDDImage("Image\\Sea\\shoal.bmp" , hWnd);
	//�}���{�[�̉摜��ǂݍ���
	g_enemyImg[3] = CreateDDImage("Image\\Sea\\sunfish.bmp" , hWnd);
	//�G�C�̉摜��ǂݍ���
	g_enemyImg[4] = CreateDDImage("Image\\Sea\\ray.bmp" , hWnd);


	//�����ʉ��̓ǂݍ���
	g_en_sound[0] = CreateSound("SE\\attack.wav");

}

//************************************************************************************************
//�X�V
//************************************************************************************************
void UpdateEnemy(){

	//�Q�[�����x�����擾
	tagGAMELEVEL gameLevel = GetGameLevel();

	//�֐��|�C���^�ɓG�̍s�����܂Ƃ߂��֐���������
	void(*ptype[ENEMY_TYPE_SEA])(ENEMY*) = {

		ActionType0 ,
		ActionType1 ,
		ActionType2 ,
		ActionType3 ,
		ActionType4 ,
	};

	//easy normal
	//���W����_�ɂ����G�̏o��
	if( gameLevel == EASY_MODE || gameLevel == NORMAL_MODE ) SetCreationEnemyByPosition();

	//hard
	//�G�������_���ŏo��
	if (gameLevel == HARD_MODE) SetCreationEnemyByRandom();

	//�o��������G�̐��������[�v����
	for (int i = 0; i < g_enemyMax; i++) {

		//�G�l�~�[�̐����t���O��true�Ȃ瓮����
		if (g_enemy[i].ch.flag == true) {

			//��ނɂ���ĈقȂ�s��
			ptype[g_enemy[i].action_type](&g_enemy[i]);

			//���W���ړ��ʂɉ��Z
			g_enemy[i].ch.x += g_enemy[i].ch.mx;
			g_enemy[i].ch.y += g_enemy[i].ch.my;
		}//�����t���O��true�̏����͂����܂�	

		//i�ԂƂ̓G�ƃJ�����̔���
		if (CollisionCamera(&g_enemy[i].ch) == false) {

			//�������Ă��Ȃ�������G������
			g_enemy[i].ch.flag = false;
			continue;
		}
	}//for���̏I���
}

//************************************************************************************************
//�\��
//************************************************************************************************
void DrawEnemy(){

	//�J�������W���擾����֐��|�C���^��錾
	float px = GetCameraX();						
	float py = GetCameraY();

	//�o��������G�̐��������[�v����
	for (int i = 0; i < g_enemyMax; i++) {

		//�G�l�~�[�̐����t���O��true�Ȃ�\��
		if (g_enemy[i].ch.flag == true) {

			//�G�l�~�[�̉摜
			DDScaleBlt(
				g_enemy[i].ch.x - px,					//X���W
				g_enemy[i].ch.y - py,					//Y���W
				g_enemy[i].ch.w,						//��
				g_enemy[i].ch.h,						//����
				g_enemyImg[g_enemy[i].ch.bitnum],		//�Ή������������f�o�C�X�R���e�L�X�g
				g_enemy[i].ch.picx,					//�\�����w���W
				g_enemy[i].ch.picy,					//�\�����x���W
				g_enemy[i].ch.scale);					//�g�k�{��
		}//g_enemy[i] = true�̎��̏����͂����܂�
	}//for���̏����͂����܂�
	
}

//************************************************************************************************
//���
//************************************************************************************************
void ReleaseEnemy(){

	//�g�p�����摜��������������s��
	for (int i = 0; i < ENEMY_IMG; i++) {
		ReleaseDDImage(g_enemyImg[i]);
	}
	//���ʉ�
	for (int i = 0; i < ENEMY_SOUND_EFFECT_MAX; i++) {
		ReleaseSound(g_en_sound[i]);
	}
}

//************************************************************************************************
//�G���w�肳�ꂽ���W�ɏo��������֐�
//************************************************************************************************
void CreateEnemy(int appear,int type,int action){

	//�v���C���[�̏����擾����
	SWIMER* pch = GetSwimer();
	//�X�e�[�W�̏����擾
	STAGE*	lpStage = GetSeaStage();
	//�J�������W���擾����֐��|�C���^��錾
	float px = GetCameraX();
	float py = GetCameraY();

	//�^�C�v���̏�����
	void (*pInitAction[KIND_ENEMY_SEA])(ENEMY*) = {
		InitEnemyAction0 ,
		InitEnemyAction1 ,
		InitEnemyAction2 ,
		InitEnemyAction3 ,
		InitEnemyAction4 ,
	};


	//�e�G�l�~�[�̃f�[�^�e�[�u���u���A�����ADDBlt�̗v�f���v
	int typeTbl[KIND_ENEMY_SEA][3] = {

		{32 , 32 , 0} ,			//��ޔԍ��O�̃G�l�~�[�u�N���Q�v
		{64 , 32 , 1} ,			//��ޔԍ��P�̃G�l�~�[�u�T���v
		{64 , 32 , 2} ,			//��ޔԍ��Q�̃G�l�~�[�u���Q�v
		{96 , 128 , 3} ,		//��ޔԍ��R�̃G�l�~�[�u�}���{�[�v
		{160 , 96 , 4} ,		//��ޔԍ��S�̃G�l�~�[�u�G�C�v	
	};

	//����Փx�ݒ�
	//��Փx�ɉ����đ��x��ύX����u��v�f���N���Q�A�T���A���Q�A�}���{�[�A�G�C�v�̏�
	float enemyspeedTbl[LEVEL_MAX][KIND_ENEMY_SEA] = {

		{ 1.0f , -2.0f , -2.0f , 2.0f , 2.0f } ,	//EASY
		{ 1.5f , -3.0f , -3.0f , 3.0f , 3.0f } ,	//NORMAL
		{ 2.0f , -4.0f , -4.0f , 4.0f , 4.0f } ,	//HARD
	
	};

	//�ő�o�����܂ŏ���
	for(int i = 0 ; i < g_enemyMax ; i++ ){

		//�G�l�~�[�̐����t���O��false�Ȃ珈�����s��
		if(g_enemy[i].ch.flag == false){
			
			//��ޔԍ��ɂ���ĈقȂ�l��ݒ�
			g_enemy[i].ch.w = typeTbl[type][0];							//��
			g_enemy[i].ch.h = typeTbl[type][1];							//����
			g_enemy[i].ch.bitnum = typeTbl[type][2];					//BitBlt���Ɉ����v�f��
			g_enemy[i].ch.scale = 1.0f;									//�g�k�{��
			g_enemy[i].ch.speed = enemyspeedTbl[GetGameLevel()][type];	//���x
			
			//�o�������ނ̐ݒ�
			g_enemy[i].type = type;
		
			//���݂̃A�N�V�����^�C�v��ݒ�
			g_enemy[i].action_type = action;

			//�o�����W�����i��ʉ��[�j
			g_enemy[i].popy = (py + STAGE_HEIGHT);				//Y

			//���X�N���[����
			if (lpStage->scroll_l) {
				g_enemy[i].ch.picx = 0;								//�\�������WY
				g_enemy[i].ch.direction = ENEMY_DIRECTION_LEFT;		//�ړ�����
				//�o�����W
				g_enemy[i].popx = (px + STAGE_WIDTH);				//X
			}
			//�E�X�N���[�����̐ݒ�
			if (lpStage->scroll_r) {
				g_enemy[i].ch.picx = g_enemy[i].ch.w;								//�\�������WY
				g_enemy[i].ch.direction = ENEMY_DIRECTION_RIGHT;					//�ړ�����
				//�o�����W
				g_enemy[i].popx = px + ( 0.0f - ( g_enemy[i].ch.w * g_enemy[i].ch.scale ));	//X
			}

			//��ޖ��ɂ�鏉����
			pInitAction[g_enemy[i].action_type](&g_enemy[i]);
					
			//�����t���O��ON
			g_enemy[i].ch.flag = true;

			//���݂̃A�N�V�����^�C�v�Ő������鐔���w�肵�����ɒB��������A��������
			if( g_enemy[action].appearCnt == appear ){
				//���������������Z�b�g
				g_enemy[action].appearCnt = 0;
				//���˃t���O��ON
				g_enemy[action].launch = true;
				return;
			}
		}
	}//for���̏I���
}

//************************************************************************************************
//�G�̍s���̏������֐��u���̏�ŉ~��`�������v
//************************************************************************************************
void InitEnemyAction0(ENEMY* pen){
	
	//��Փx�ɉ����Ċp�x��ύX����u�A�N�V�����^�C�v0�ԁv�̂�
	float enemyangleTbl[LEVEL_MAX] = {
		45.0f , 55.0f , 65.0f ,
	};
	//�p�x����
	pen->ch.angle = (int)enemyangleTbl[GetGameLevel()];
	//�w���W
	pen->ch.x = pen->popx;
	//�x���W
	pen->ch.y = (float)RandomNum((int)( pen->popy - ( pen->ch.h * pen->ch.scale )) , (int)(pen->popy - STAGE_HEIGHT));
	//���݂̃A�N�V�����^�C�v�Ő������������J�E���g
	g_enemy[ENEMY_ACTION_CIRCLE].appearCnt++;
	
}

//************************************************************************************************
//�G�̍s���̏������֐��u�T���v�̃A�N�V�����u�꒼���ɓ����v
//************************************************************************************************
void InitEnemyAction1(ENEMY* pen){
	
	//�o�������鐔���v�f���ɒB�����ꍇ��return
	if(g_enemy[ENEMY_ACTION_STRAIGHT].appearCnt > g_enemy[ENEMY_ACTION_STRAIGHT].appear) return;

	//���񂳂���x���W
	int setTbl[4][8] = {

		{10 , 52 , 94 , 136 , 178 , 220 , 262 , 304} ,
		{136 , 178 , 220 , 262 , 304 , 346 , 388 , 430} ,
		{10 , 52 , 94 , 262 , 304 , 346 , 388 , 430} ,
		{10 , 52 , 94 , 136 , 178 , 346 , 388 , 430} ,
	
	};

	//�z�񂳂���x���W�̍s�v�f�ԍ����擾
	int pos = RandomNum(3,0);

	//���񂳂���s�v�f�����w��
	pen->ch.y = ( ( pen->popy - STAGE_HEIGHT ) + (float)setTbl[pos][g_enemy[ENEMY_ACTION_STRAIGHT].appearCnt] );
	//�w���W���v���C���[�̈ړ��ɍ��킹�āA��ʒ[�őҋ@
	pen->ch.x = pen->popx;
	//�������������J�E���g
	g_enemy[ENEMY_ACTION_STRAIGHT].appearCnt++;
}

//************************************************************************************************
//�G�̍s���̏������֐��u���Q�v�̃A�N�V�����u�㉺�ɔg�ł����v
//************************************************************************************************
void InitEnemyAction2(ENEMY* pen){
	
	//�w���W
	pen->ch.x = pen->popx;	
	//�x���W
	pen->ch.y = (float)RandomNum( (int)( pen->popy - (pen->ch.h * pen->ch.scale) ), (int)(pen->popy - STAGE_HEIGHT) );
	//���݂̃A�N�V�����^�C�v�Ő������������J�E���g
	g_enemy[ENEMY_ACTION_WAVE].appearCnt++;

}

//************************************************************************************************
//�G�̍s���̏������֐��u�}���{�[�v�̃A�N�V�����u�G�l�~�[�̍��W�����_�Ƀv���C���[�̍��W�܂ň꒼���ɓ����v
//************************************************************************************************
void InitEnemyAction3(ENEMY* pen){
	
	//�v���C���[���̎擾
	SWIMER* pch = GetSwimer();

	//�w���W
	pen->ch.x = pen->popx;
	//�x���W
	pen->ch.y = (float)RandomNum( (int) ( pen->popy - (pen->ch.h * pen->ch.scale)), (int)( pen->popy - STAGE_HEIGHT ) );
	//���݂̃A�N�V�����^�C�v�Ő������������J�E���g
	g_enemy[ENEMY_ACTION_APPROACH].appearCnt++;
	//�v���C���[�̍��W���擾
	pen->targetx = pch->ch.x;
	pen->targety = pch->ch.y;
}

//************************************************************************************************
//�G�̍s���̏������֐��u�G�C�v�̃A�N�V�����u�G�l�~�[�̍��W�����_�Ƀv���C���[�̍��W��ǐՂ���v
//************************************************************************************************
void InitEnemyAction4(ENEMY* pen){

	//�w���W
	pen->ch.x = pen->popx;
	//�x���W
	pen->ch.y = (float)RandomNum((int)(pen->popy - (pen->ch.h * pen->ch.scale)), (int)(pen->popy - STAGE_HEIGHT));
	//���݂̃A�N�V�����^�C�v�Ő������������J�E���g
	g_enemy[ENEMY_ACTION_CHASE].appearCnt++;
}

//************************************************************************************************
//�G�̍s����ω�������֐��u�N���Q�v�̃A�N�V�����u���̏�ŉ~��`�������v
//************************************************************************************************
void ActionType0(ENEMY* pen){
	
	//��]
	pen->circleCnt++;
	//���̏�ŉ~��`���Ȃ��瓮������
	pen->ch.mx = (float)cos(pen->circleCnt * 3.14 / pen->ch.angle) *
									(pen->ch.speed * pen->ch.direction);		//�w�ړ�

	pen->ch.my = (float)sin(pen->circleCnt * 3.14 / pen->ch.angle) * pen->ch.speed;//�x�ړ�
	//�J�E���g����1000�܂�
	if(pen->circleCnt > 1000) pen->circleCnt = 0;
}
	
//************************************************************************************************
//�G�̍s����ω�������֐��u�T���v�̃A�N�V�����u�꒼���ɓ����v
//************************************************************************************************
void ActionType1(ENEMY* pen){

	//���񂵂�����5�Έȏ�ɂȂ�����s��
	if (g_enemy[ENEMY_ACTION_STRAIGHT].launch) {
		//���Ԍo��
		g_enemy[ENEMY_ACTION_STRAIGHT].stayTime++;
		//�i�s�����֒��i���铮��
		pen->ch.mx = ( pen->ch.speed * pen->ch.direction );		//X�ړ�
		pen->ch.my = 0.0f;										//Y�ړ�
	}

	//1500�t���[���ȏ�ɂȂ����玞�Ԃ����Z�b�g���A���˃t���O��false�ɂ���
	if (g_enemy[ENEMY_ACTION_STRAIGHT].stayTime >= 1500) {
		g_enemy[ENEMY_ACTION_STRAIGHT].stayTime = 0;
		//���˃t���O
		g_enemy[ENEMY_ACTION_STRAIGHT].launch = false;
	}
}

//************************************************************************************************
//�G�̍s����ω�������֐��u���Q�v�̃A�N�V�����u�㉺�ɔg�ł����v
//************************************************************************************************
void ActionType2(ENEMY* pen){

	pen->waveCnt += 2;
	//�㉺�ɔg�ł��Ȃ����ʍ������ֈړ����铮��
	pen->ch.mx = ( pen->ch.speed * pen->ch.direction);							//X�ړ�
	pen->ch.my = (float)sin(pen->waveCnt * 3.14 / 180.0 ) * pen->ch.speed;		//Y�ړ�
	//�J�E���g����1000�܂�
	pen->waveCnt %= 1000;

}

//************************************************************************************************
//�G�̍s����ω�������֐��u�G�l�~�[�̍��W�����_�Ƀv���C���[�̍��W�܂ň꒼���ɓ����v
//************************************************************************************************
void ActionType3(ENEMY* pen){
	
	//�v���C���[�̏����擾
	SWIMER* pch = GetSwimer();
	//�C�}�b�v�̏����擾����
	STAGE* pmap = GetSeaStage();

	//�������͉��X�N���[�����̂�
	if(pmap->scrollx == true){

		//�����X�N���[����
		if(pmap->scroll_l == true){
		
			//�v���C���[�̒��S�w���W�𒴂����炻�̂܂܉�ʍ���֒��i������
			if(pen->ch.x + (pen->ch.w / 2) <= pch->ch.x + (pch->ch.w * pch->ch.scale)){
				pen->ch.mx = -4.0f;
				pen->ch.my = -4.0f;
			}

			//�G�l�~�[�̍��W���v���C���[�̑O���Ɉʒu���Ă�����
			//CreateEnemy���Ŏ擾�����v���C���[�̒��S���W������
			//�G�l�~�[�����_�i�n���j�Ƃ��A���p�O�p�`�̒�ӂƍ����ƎΕӂ����߂�
			if(pen->ch.x + (pen->ch.w / 2) >= pch->ch.x + (pch->ch.w * pch->ch.scale)){
				
				//���
				float a = ( pen->targetx + ( pch->ch.w * pch->ch.scale ) / 2 ) - 
							( pen->ch.x + ( pen->ch.w / 2 ) );
				//����
				float b = ( pen->targety + ( pch->ch.h * pch->ch.scale ) / 2 ) - 
							( pen->ch.y + (pen->ch.h / 2 ) );
				//�Ε�
				float c = sqrtf( (a * a) + (b * b) );
				//cos�l�����߂�
				pen->ch.mx = (a / c) * pen->ch.speed;
				//tan�l�����߂�
				pen->ch.my = (b / c) * pen->ch.speed;
			}
		}
		//���E�X�N���[�����ł̏���
		if(pmap->scroll_r == true){
			//�v���C���[�̒��S�w���W�𒴂����炻�̂܂܉�ʍ���֒��i������
			if(pen->ch.x + (pen->ch.w / 2) >= pch->ch.x){
				pen->ch.mx = 4.0f;
				pen->ch.my = -4.0f;
			}
			//�G�l�~�[�̍��W���v���C���[�̑O���Ɉʒu���Ă�����
			//CreateEnemy���Ŏ擾�����v���C���[�̒��S���W������
			//�G�l�~�[�����_�i�n���j�Ƃ��A���p�O�p�`�̒�ӂƍ����ƎΕӂ����߂�
			if(pen->ch.x + (pen->ch.w / 2) <= pch->ch.x){
				//���
				float a = (pen->targetx + (pch->ch.w * pch->ch.scale) / 2) - 
							(pen->ch.x + (pen->ch.w / 2));
				//����
				float b = (pen->targety + (pch->ch.h * pch->ch.scale) / 2) - 
							(pen->ch.y + (pen->ch.h / 2));
				//�Ε�
				float c = sqrtf((a * a) + (b * b));
				//cos�l�����߂�
				pen->ch.mx = (a / c) * pen->ch.speed;
				//tan�l�����߂�
				pen->ch.my = (b / c) * pen->ch.speed;
			}
		}
	}
}

//************************************************************************************************
//�G�̍s����ω�������֐��u�G�l�~�[�̍��W�����_�Ƀv���C���[�̍��W��ǐՂ���v
//************************************************************************************************
void ActionType4(ENEMY* pen){
	
	//�v���C���[�̏����擾
	SWIMER* pch = GetSwimer();
	//�}�b�v�̏����擾����
	STAGE* pmap = GetSeaStage();
	
	//�����X�N���[���̎��̂ݏ���
	if(pmap->scrollx == true){

		//�����X�N���[�����̏���
		if(pmap->scroll_l == true){
		
			//�v���C���[�̉E�[�w���W�𒴂����炻�̂܂܉�ʍ���֒��i������
			if(pen->ch.x + (pen->ch.w / 2) <= pch->ch.x + (pch->ch.w * pch->ch.scale)){
				pen->ch.mx = -4.0f;
				pen->ch.my = -4.0f;
			}

			//�G�l�~�[�̒��S���W���v���C���[�̉E�[���z���Ă��Ȃ�������
			//�G�l�~�[�����_�i�n���j�Ƃ��A���p�O�p�`�̒�ӂƍ����ƎΕӂ����߂�
			if(pen->ch.x + (pen->ch.w / 2) >= pch->ch.x + (pch->ch.w * pch->ch.scale)){

				//���
				float a = (pch->ch.x + (pch->ch.w * pch->ch.scale) / 2) - 
							(pen->ch.x + (pen->ch.w / 2));
				//����
				float b = (pch->ch.y + (pch->ch.h * pch->ch.scale) / 2) - 
							(pen->ch.y + (pen->ch.h / 2));
				//�Ε�
				float c = sqrtf((a * a) + (b * b));
				//cos�l�����߂�
				pen->ch.mx = (a / c) * pen->ch.speed;
				//sin�l�����߂�
				pen->ch.my = (b / c) * pen->ch.speed;
			}
		}
		
		//���E�X�N���[�����̏���
		if(pmap->scroll_r == true){
		
			//�v���C���[�̍��[�w���W�𒴂����炻�̂܂܉�ʍ���֒��i������
			if(pen->ch.x + (pen->ch.w / 2) >= pch->ch.x){
				pen->ch.mx = 4.0f;
				pen->ch.my = 4.0f;
			}

			//�G�l�~�[�̒��S���W���v���C���[�̍��[���z���Ă��Ȃ�������
			//�G�l�~�[�����_�i�n���j�Ƃ��A���p�O�p�`�̒�ӂƍ����ƎΕӂ����߂�
			if(pen->ch.x + (pen->ch.w / 2) <= pch->ch.x){

				//���
				float a = (pch->ch.x + (pch->ch.w * pch->ch.scale) / 2) - 
							(pen->ch.x + (pen->ch.w / 2));
				//����
				float b = (pch->ch.y + (pch->ch.h * pch->ch.scale) / 2) - 
							(pen->ch.y + (pen->ch.h / 2));
				//�Ε�
				float c = sqrtf((a * a) + (b * b));
				//cos�l�����߂�
				pen->ch.mx = (a / c) * pen->ch.speed;
				//sin�l�����߂�
				pen->ch.my = (b / c) * pen->ch.speed;
			}
		}
	}
}

//************************************************************************************************
//�����蔻��
//************************************************************************************************
bool CollisionEnemy(CHARACTER* ch){

	//�߂�l�p
	bool bump = false;

	SWIMER* pch = GetSwimer();		//�v���C���[�̃|�C���^���擾
	ITEM* pi = GetItem();			//�A�C�e���̏����擾

	//�����ŃL�����N�^�[�Ɛ����Ă���G�Ŕ���
	for (int i = 0; i < g_enemyMax; i++) {

		//�����̓G�������Ă�����
		if (g_enemy[i].ch.flag) {

			//�����̃L�����N�^�[��i�Ԃ̓G�Ŕ���
			if (CollisionCharacter(ch, &g_enemy[i].ch)) {

				//�G�̃_���[�W�G�t�F�N�g��\��
				CreateEffect(g_enemy[i].ch.x, g_enemy[i].ch.y);
				//���ʉ�
				PlaySound(g_en_sound[0], 0, false);
				//�v���C���[���U����or���G��ԂȂ�G������
				if (pch->attack || pch->itemEffectType == SWIMER_ITEM_EFFECT_TYPE_ABSOLUTE) {
					//���������G������	
					g_enemy[i].ch.flag = false;
					//�|����������ޖ��ɃJ�E���g
					g_enemy[0].defeat[g_enemy[i].type]++;
					//CHAIN�����J�E���g
					pch->defeat_cnt++;
					//�ő哢�������X�V
					if (pch->defeat_cnt > pch->chain_max) pch->chain_max = pch->defeat_cnt;
					//�G�������ꏊ�̏����O���ɃA�C�e�����o��������
					CreateItem((float)g_enemy[i].ch.x + pi->pos_x,		//X���W
						(float)g_enemy[i].ch.y,							//Y���W
						RandomNum(ITEM_APPEAR_RANDOM_MAX,
							ITEM_APPEAR_RANDOM_MIN));				//�����_���Ŏ�ތ���			
					//�G�ɓ���������true��Ԃ�
					bump = true;
					//���̓G�Ɣ���
					continue;
				}

				//�v���C���[�̐����t���O��false��
				pch->ch.flag = false;
				//CHAIN������
				pch->defeat_cnt = 0;

				//�G�ɓ���������true��Ԃ�
				bump = true;
				break;
			}
		}
	}
	//�������Ă���G�����Ȃ�������false��Ԃ�
	return bump;
}

//************************************************************************************************
//�G�̏���Ԃ�
//************************************************************************************************
ENEMY* GetEnemy(){ return &g_enemy[0]; }


/*
�ݒ肵�����W���K��ɂ����G�̏o��
*/
void	SetCreationEnemyByPosition() {


	//�v���C���[�̏����擾
	SWIMER* pch = GetSwimer();
	//�}�b�v�̏����擾
	STAGE* pst = GetSeaStage();

	//�f�t�H���g�̓G�̃A�N�V�����p�^�[��
	int enemy_actionTbl[KIND_ENEMY_SEA] = {

		ENEMY_ACTION_CIRCLE ,			//�N���Q
		ENEMY_ACTION_STRAIGHT ,			//�T��
		ENEMY_ACTION_WAVE ,				//���Q
		ENEMY_ACTION_APPROACH ,			//�}���{�[
		ENEMY_ACTION_CHASE ,			//�G�C
	};

	//���@���݂̃v���C���[�̍��W����ɃG�l�~�[�̎�ނ����肷��
	//�G�̎�ނ������[�v
	for (int i = 0; i < KIND_ENEMY_SEA; i++) {

		//���@���X�N���[����
		if (pst->scroll_l) {
			//�v���C���[�̌��݂̂w���W����ɏo��������G������
			if (pch->ch.x >= g_enemy[i].appear_start_posx &&
				pch->ch.x <= g_enemy[i].appear_end_posx) {
				//�o���Ԋu���v��
				g_enemy[0].ch.time++;
				//�A�N�V�����^�C�v����
				g_enemy_type_now = enemy_actionTbl[i];
				//���@�G�̔�������		
				//�G���e�G�l�~�[�ɐݒ肳�ꂽ�t���[�����ɐ���
				if (g_enemy[0].ch.time % g_enemy[i].appear_fram == 0) {
					g_enemy[0].ch.time = 0;
					//�o�����鐔�A�o�������ށA�A�N�V�����^�C�v��n��
					CreateEnemy(g_enemy[i].appear, g_enemy[i].appear_type,
						g_enemy_type_now);
					break;
				}
			}
		}

		//���@�E�X�N���[����
		if (pst->scroll_r) {
			//�v���C���[�̌��݂̂w���W����ɏo��������G������
			if (pch->ch.x <= g_enemy[i].appear_start_posx_r &&
				pch->ch.x >= g_enemy[i].appear_end_posx_r) {
				//�o���Ԋu���v��
				g_enemy[0].ch.time++;
				//�A�N�V�����^�C�v����
				g_enemy_type_now = enemy_actionTbl[i];
				//���@�G�̔�������		
				//�G���e�G�l�~�[�ɐݒ肳�ꂽ�t���[�����ɐ���
				if (g_enemy[0].ch.time % g_enemy[i].appear_fram == 0) {
					g_enemy[0].ch.time = 0;
					//�o�����鐔�A�o�������ށA�A�N�V�����^�C�v��n��
					CreateEnemy(g_enemy[i].appear, g_enemy[i].appear_type,
						g_enemy_type_now);
					break;
				}
			}
		}
	}
}

/*
�G�̏o���������_��
*/
void	SetCreationEnemyByRandom() {

	//�f�t�H���g�̓G�̃A�N�V�����p�^�[��
	int enemy_actionTbl[KIND_ENEMY_SEA] = {

		ENEMY_ACTION_CIRCLE ,			//�N���Q
		ENEMY_ACTION_STRAIGHT ,			//�T��
		ENEMY_ACTION_WAVE ,				//���Q
		ENEMY_ACTION_APPROACH ,			//�}���{�[
		ENEMY_ACTION_CHASE ,			//�G�C
	};

	//�����_���̒l
	int	random = RandomNum(ENEMY_ACTION_CHASE, ENEMY_ACTION_CIRCLE);

	//���@���݂̃v���C���[�̍��W����ɃG�l�~�[�̎�ނ����肷��
	//�o���Ԋu���v��
	g_enemy[0].ch.time++;
	//�A�N�V�����^�C�v����
	g_enemy_type_now = enemy_actionTbl[random];
	//���@�G�̔�������		
	//�G���e�G�l�~�[�ɐݒ肳�ꂽ�t���[�����ɐ���
	if (g_enemy[0].ch.time % g_enemy[random].appear_fram == 0) {
		g_enemy[0].ch.time = 0;
		//�o�����鐔�A�o�������ށA�A�N�V�����^�C�v��n��
		CreateEnemy(g_enemy[random].appear, g_enemy[random].appear_type,
			g_enemy_type_now);
		
	}
}