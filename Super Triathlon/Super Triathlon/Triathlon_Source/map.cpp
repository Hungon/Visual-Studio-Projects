#include    "map.h"
#include    "play.h"
#include    "sound.h"
#include    "menu.h"
#include	"SelectStage.h"
#include	<string.h>
#include	<math.h>
#include	<stdio.h>


//���O���[�o���ϐ�

//���摜�̗v�f
int g_mapImg[MAP_IMAGE_MAX] = {0};

//�����ʉ�
int g_map_sound[MAP_SOUND_EFFECT_MAX] = {0};

//�X�e�[�W�Z���N�g�\��������
CHARACTER g_select = {0};

//�J�����ݒ�
float g_cameraX = 0;				//�J�����w���W
float g_cameraY = 0;				//�J�����x���W

//�T�C���\��������
STAGE g_sign[MAP_SIGN_MAX] = {0};

//���Z�X�^�[�g��������
bool g_start = false;

//���ԕ\��
SCOREVALUE g_timeScore = {0};

//�J�����͈̔͂�����
float g_stageW = 0;				//��
float g_stageH = 0;				//����
float g_stageTop = 0;			//�X�e�[�W�̏��
float g_stageBottom = 480.0f;	//�X�e�[�W�̉���


//************************************************************************************************
//��������
//************************************************************************************************
void InitMap(HWND hWnd){

	//���݂̃X�e�[�W�ԍ����擾����
	tagSELECTSTAGE stageNum = GetStage();

	//�摜�f�[�^
	char	*lpImageTbl[ MAP_IMAGE_MAX ] = {

		{"Image\\readygo.bmp"}		,
		{"Image\\countdown.bmp"}	,
		{"Image\\SelectStage.bmp"}	,

	};

	//�e�X�e�[�W�͈̔͂�����
	g_stageW = 0.0f;			//��
	g_stageH = 0.0f;			//����
	g_stageTop = 0.0f;			//�X�e�[�W�̏��
	g_stageBottom = 480.0f;		//�X�e�[�W�̉���
	//���Ԃ����Z�b�g
	ZeroMemory(&g_timeScore, sizeof(SCOREVALUE));
	//�X�e�[�W�Z���N�g
	ZeroMemory(&g_select, sizeof(CHARACTER));
	//�J�����̍��W�����Z�b�g
	g_cameraX = 0;				//�J�����w���W
	g_cameraY = 0;				//�J�����x���W
	//�T�C���̏�����
	ZeroMemory(g_sign, sizeof(STAGE) * MAP_SIGN_MAX);

	//���摜�̓ǂݍ���
	for( int i = 0 ; i < MAP_IMAGE_MAX ; i++ ){
		g_mapImg[i] = CreateDDImage( lpImageTbl[i] , hWnd );
	}

	//�����̓ǂݍ���
	//���ʉ��P
	g_map_sound[0] = CreateSound("SE\\countdown.wav");
	//���ʉ��Q
	g_map_sound[1] = CreateSound("SE\\startsign.wav");

	//���e�X�e�[�W�̉��y�t�@�C��
	char* backMusic[STAGE_MAX] = {
		"BGM\\sea.wav" ,
		"BGM\\road.wav" ,
		"BGM\\OffRoad.wav" ,
	};

	//�ǂݍ���
	CreateStreaming( backMusic[ stageNum ] );

	//���}�b�v�̏����ݒ�

	//���Z�X�^�[�g����false
	g_start = false;

	//�Z���N�g�摜
	g_select.x = 200.0f;
	g_select.y = 100.0f;
	g_select.w = SELECTSTAGE_STAGE_TITLE_IMAGE_WIDTH;
	g_select.h = SELECTSTAGE_STAGE_TITLE_IMAGE_HEIGHT;
	g_select.picy = SELECTSTAGE_STAGE_TITLE_IMAGE_WIDTH * stageNum;
	g_select.flag = true;

	//�X�^�[�g���}�摜�ureadygo!!�v
	g_sign[1].ch.x = 80;		//X���W
	g_sign[1].ch.y = 150;	//Y���W
	g_sign[1].ch.h = 176;	//����
	g_sign[1].ch.w = 496;	//��
	g_sign[1].ch.flag = false;

	//�u�R�C�Q�C�P�v�̉摜
	g_sign[0].sign = false;
	g_sign[0].ch.x = 250;	//X���W
	g_sign[0].ch.y = 150;	//Y���W
	g_sign[0].ch.h = 144;	//����
	g_sign[0].ch.w = 144;	//��
	g_sign[0].ch.flag = false;

	//�u�R�C�Q�C�P�v�̃A�j���[�V�����ݒ�
	g_sign[0].ani.fram = 40;		//�t���[����
	g_sign[0].ani.max = 3;		//�ő�\����
	
	SetAnimation(&g_sign[0].ch,&g_sign[0].ani ,
				g_sign[0].ani.sx,g_sign[0].ani.sy ,
				g_sign[0].ani.max ,
				g_sign[0].ani.fram ,
				0 , true);	

	//�����ԃX�R�A�̐ݒ�
	g_timeScore.digit = 2;								//2���\��
	g_timeScore.score_x = 100.0f;						//�\��X���W
	g_timeScore.score_y = 50.0f;						//�\��Y���W
	g_timeScore.spaceX = 10.0f;							//�\���Ԋu
	g_timeScore.ti.hour = 0;							//��
	g_timeScore.ti.minutes = 0;							//��
	g_timeScore.ti.second = 0;							//�b
	g_timeScore.ti.millisecond = 0;						//�~���b
	g_timeScore.score_scale = 0.25f;					//�g�k��
	g_timeScore.ch.type = SCORE_NUMBER_TYPE_NORMAL;		//�X�R�A�摜�̎��

}

//************************************************************************************************
//���X�V
//************************************************************************************************
void UpdateMap(){

	int i = 0;

	//���j���[�t���O���󂯎��
	bool menu = GetMenu();

	//�v���C�J�n���J�n�̏����ug_select == true�v�ŃX�e�[�W����\��
	//���j���[���J���Ă����珈�����X�V���Ȃ�
	if(g_start == false || menu == false){
			//���Ԍo��
			g_select.time++;
		
		//�t���[���o�߂ŃQ�[���J�n
		if(g_select.time > 100 && g_sign[0].sign == false && g_start == false){
			g_select.time = 0;
			g_select.flag = false;
			g_sign[0].sign = true;
		}

		//�X�^�[�g�T�C�����o���֐�
		g_sign[0].sign = StartSign(g_sign[0].sign);
	}

	//���j���[���J���Ă�����J�E���g���~����
	if(menu){

		//�J�E���g��~
		g_sign[0].sign = false;
		//���Ԓ�~
		g_timeScore.ti.cntFlag = false;
	}

	//���j���[���J���Ă��Ȃ����
	if( menu == false && g_start == true ){

		//���Ԍo��
		g_timeScore.ti.cntFlag = g_start;

		//���[�X�J�n
		Playing(g_start);
	}

	//�Ȃ��Đ�
	PlayStreaming(g_start);

}

//************************************************************************************************
//���\��
//************************************************************************************************
void DrawMap(){

	//�����t���O��true�Ȃ�\��
	if(g_sign[1].ch.flag == true){
		//�X�^�[�g�̍��}�ureadygo!!�v
		DDBlt(
			(int)g_sign[1].ch.x ,
			(int)g_sign[1].ch.y ,
			g_sign[1].ch.w ,
			g_sign[1].ch.h ,
			g_mapImg[0] ,
			g_sign[1].ch.picx ,
			g_sign[1].ch.picy);
	}
	//�����t���O��true�Ȃ�\��
	if(g_sign[0].ch.flag == true){
		//�X�^�[�g�̍��}�u�R�C�Q�C�P�v
		DDBlt(
			(int)g_sign[0].ch.x ,
			(int)g_sign[0].ch.y ,
			g_sign[0].ch.w ,
			g_sign[0].ch.h ,
			g_mapImg[1] ,
			g_sign[0].ch.picx ,
			g_sign[0].ch.picy);
	}

	//�X�e�[�W�Z���N�g�摜
	if(g_select.flag){
		DDBlt(
			(int)g_select.x ,
			(int)g_select.y ,
			g_select.w ,
			g_select.h ,
			g_mapImg[2] ,
			g_select.picx ,
			g_select.picy);
	}

	//���ԃX�R�A�̕\��
	DrawTime( &g_timeScore );

}

//************************************************************************************************
//�����
//************************************************************************************************
void ReleaseMap(){

	int i = 0;
	//�g�p���������������
	for(i = 0;i < MAP_IMAGE_MAX;i++){
		ReleaseDDImage(g_mapImg[i]);
	}

	//���ʉ��̉��
	for(i = 0;i < MAP_SOUND_EFFECT_MAX;i++){
		ReleaseSound(g_map_sound[i]);
	}
	//�Ȃ̉��
	ReleaseStreaming();
}

//************************************************************************************************
//�X�^�[�g���̃T�C����flag��n���ĕ\������
//************************************************************************************************
bool StartSign(bool flag){

	bool ret = flag;

	//�n���ꂽ�t���O��true�A���A�uReadyGo!!�v�̕\���J�E���g�����o�߂Ȃ�u3�C2�C1�v��\��
	if(flag == true){
		if(g_sign[1].ch.time == 0){
			//�J�E���g�_�E���̐������J��オ�邽�тɌ��ʉ���炷
			if(g_sign[0].ani.time == 0){
				//���ʉ�
				PlaySound(g_map_sound[0],0,false);
			}
			//�u3,2,1�v�摜���A�j���[�V�����\��
			g_sign[0].ch.flag = true;
			//�u3,2,1�v�\����ɏ����āA�uReadyGo!!�v��\��
			if( g_sign[0].ch.flag == true && UpdateAnimation( &g_sign[0].ch,&g_sign[0].ani ) == false ){
				g_sign[0].ch.flag = false;
				//���ʉ�
				PlaySound(g_map_sound[1],0,false);
				//�uReadyGo�I�I�v�\��
				g_sign[1].ch.flag = true;
			}
		}
		//�J�E���g�A�b�v
		if(g_sign[1].ch.flag == true){
			g_sign[1].ch.time++;		//�o�ߎ���
		}
		//�t���[�������P�O�O�𒴂�����
		if(g_sign[1].ch.time > 100){
			//���Ԃ����Z�b�g
			g_sign[1].ch.time = 0;
			//�uReadyGo!!�v������
			g_sign[1].ch.flag = false;
			//�߂�l
			ret = false;
			//�v���C�J�n
			g_start = true;
		}
	}
	return ret;
}

//***************************************************************************************************
//���݂̃X�e�[�W�̕���n��
//*************************************************************************************************
float GetStageWidth(){
	return g_stageW;
}

//***************************************************************************************************
//���݂̃X�e�[�W�̍�����n��
//*************************************************************************************************
float GetStageHeight(){
	return g_stageH;
}


//************************************************************************************************
//���Z�����擾
//************************************************************************************************
bool GetRacing(){
	return g_start;
}

//************************************************************************************************
//���J�����̍��W���w��̍��W�ɕύX����֐�
//************************************************************************************************
void SetCamera( float cameraX , float cameraY ){
	

	//�󂯎�������W����ɃJ�����̍��W�̊�_����ʂ̒��S�ɕύX����
	g_cameraX = cameraX;				//X���W
	g_cameraY = cameraY;				//Y���W

	//��ʊO�i��ʂ̍��j��\�����Ȃ��悤�ɃJ�����̊�_���W���C��
	if(g_cameraX <= 0){
		g_cameraX = 0;
	}
	//��ʂ̉E
	if( g_cameraX >= g_stageW - STAGE_WIDTH ){
		g_cameraX = (float)( g_stageW - STAGE_WIDTH );
	}
	//��ʂ̏�
	if( g_cameraY <= g_stageTop ){		
		g_cameraY = g_stageTop;
	}
	//��ʂ̉�
	if( g_cameraY >= g_stageBottom - STAGE_HEIGHT ){
		g_cameraY = g_stageBottom - STAGE_HEIGHT;
	}
}

//************************************************************************************************
//�J�����͈̔͂�ύX����
//************************************************************************************************
void ChangeCamera(float top , float bottom , float width , float height){
	
	//�n���ꂽ�l����
	g_stageTop = top;		//���
	g_stageBottom = bottom;	//����
	g_stageW = width;		//��
	g_stageH = height;		//����

}

//************************************************************************************************
//���J�����͈͓���`�Ƃ̓����蔻������֐�
//************************************************************************************************
bool CollisionCamera(CHARACTER* ch){

	//�J�����̔���������ϐ�
	CHARACTER rect = {0};
	
	//rect�ɉ�ʃT�C�Y���傫����`��ݒ�
	rect.x = g_cameraX - 320.0f;	//��ʍ��W�ɍ��킹��
	rect.y = g_cameraY - 240.0f;	//��ʍ��W�ɍ��킹��
	rect.w = 1280;					//��
	rect.h = 960;					//����
	rect.scale = 1.0f;				//�g�k�{��

	//rect�ƈ����œn���ꂽ�L�����N�^�[�Ŕ���
	return CollisionCharacter( &rect , ch );
}

//************************************************************************************************
//���J�����̂w���W��Ԃ��֐�
//************************************************************************************************
float GetCameraX(){	return g_cameraX; }

//************************************************************************************************
//���J�����̂x���W��Ԃ��֐�
//************************************************************************************************
float GetCameraY(){	return g_cameraY; }

//************************************************************************************************
//�X�e�[�W�̃N���A���Ԃ�Ԃ�
//************************************************************************************************
TIME* GetStageCompleteTime(){
	
	//�~���b���L�^�p�ɓK�����l�ɒ���
	g_timeScore.ti.millisecond *= 1.67;
	return &g_timeScore.ti; 
}


//************************************************************************************************
//���[�X�^�C�}�[��ON�AOFF������
//************************************************************************************************
void ControlStageTime(bool control){ g_timeScore.ti.cntFlag = control; }

