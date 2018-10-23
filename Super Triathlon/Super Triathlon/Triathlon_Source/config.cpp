#include "config.h"
#include "game.h"
#include "input.h"
#include "wipe.h"
#include "sound.h"

//�摜�ǂݍ��ݗp
int g_configImg[CONFIG_IMG_MAX] = {0};
//���ʉ��̓ǂݍ���
int g_co_sound[CONFIG_SOUND_EFFECT_MAX] = {0};

//���̊�{���
CHARACTER g_cursor = {0};

int g_config[ACTION_MAX] = {
	BUTTON_4 ,		//�����ݒ�̃{�^��
	BUTTON_3 ,		
	BUTTON_6 ,
};


//*************************************************************************************************
//�������@InitConfig
//*************************************************************************************************
void InitConfig( HWND hWnd ){

	//���摜�̓ǂݍ���
	g_configImg[0] = CreateDDImage("Image\\cursor.bmp" , hWnd );	//�G


	//�����ʉ��̓ǂݍ���
	//�J�[�\����
	g_co_sound[0] = CreateSound("SE\\cursor.wav");
	//���艹
	g_co_sound[1] = CreateSound("SE\\enter.wav");

	//���Ȃ̓ǂݍ���
	CreateStreaming("BGM\\option.wav");

	//�J�[�\���̏�����
	g_cursor.x = 50.0f;	//x���W	
	g_cursor.y = 100.0f;	//y���W
	g_cursor.w = 32;	//��
	g_cursor.h = 32;	//����
	g_cursor.scale = 1.0f;	//�g�k�{��

}

//*************************************************************************************************
//�X�V�@UpdateConfig
//*************************************************************************************************
void UpdateConfig(){

	int i = 0;

	//�Ȃ̍Đ�
	PlayStreaming(true);
	
	if((int)g_cursor.y == 400 && 
	  JustKey('Z') || JustButton(BUTTON_1)){
		//���艹
		PlaySound(g_co_sound[1],0,false);
		//�V�[���I�[�v�j���O��
		CreateWipe(SCENE_OPENING , CHANGE_SCENE , WIPE_FEEDOUT);
	}

	//�I�[�v�j���O�փV�[���ύX
	//�S�Ẵ{�^���̉������u�Ԃ𒲂ׂ�
	for(i = 0;i < BUTTON_MAX;i++){
		if(JustButton(0x01<<i)){
			//���艹
			PlaySound(g_co_sound[1],0,false);
			//�e�{�^���ݒ�̍��W
			if((int)g_cursor.y == 100){
				g_config[ATTACK] = 0x01<<i;
			}
			if((int)g_cursor.y == 200){
				g_config[JUMP] = 0x01<<i;
			}
			if((int)g_cursor.y == 300){
				g_config[DUSH] = 0x01<<i;
			}

		}	//�J�[�\���̒l���Q�O�O�Ȃ�W�����v�A�R�O�O�Ȃ�_�b�V��
	}


	//-------------------------------------------------------------
	//���̈ړ�(X���W��100�AY���W��100�`400�̊Ԃ�100���ω�����)
	//-------------------------------------------------------------

	//�L�[�u���v���A�\���L�[�u���v�Ŗ��ړ�
	if( JustKey( VK_UP ) || JustLever( LEVER_UP ) ){
		g_cursor.y -= 100;
		//�J�[�\����
		PlaySound(g_co_sound[0],0,false);
		//���������������A��ԉ��̍��ڂɍ��킹��
		if( g_cursor.y < 100 ){
			g_cursor.y = 400.0f;
		}
	}

	//�L�[�u���v���A�\���L�[�u���v�Ŗ��ړ�
	if( JustKey( VK_DOWN ) || JustLever( LEVER_DOWN ) ){
		g_cursor.y += 100;
		//�J�[�\����
		PlaySound(g_co_sound[0],0,false);
		//�������������A��ԏ�̍��ڂɍ��킹��
		if( g_cursor.y > 400 ){
			g_cursor.y = 100.0f;
		}
	}
	
}

//*************************************************************************************************
//�\���@DrawConfig
//*************************************************************************************************
void DrawConfig(){

	char mes[32] = {0};

	//�w�i�𔒂œh��Ԃ�
	DDBlt(0 , 0 , 640 , 480 , NULL , 0 , 0 );
	
	//X100 Y100�@�̍��W�Ɂh�U���{�^�� / ����{�^���h�ƕ\��
	DDTextOut("�U���{�^�� / ����{�^��",100 , 100 );
	
	//X100 Y200�@�̍��W�Ɂh�W�����v�{�^���h�ƕ\��
	DDTextOut("�W�����v�{�^��",100 , 200 );

	//X100 Y300�@�̍��W�Ɂh�W�����v�{�^���h�ƕ\��
	DDTextOut("�_�b�V���{�^��" ,100 , 300 );

	//X100 Y400�@�̍��W�Ɂh�W�����v�{�^���h�ƕ\��
	DDTextOut("�^�C�g���ɖ߂�",100 , 400 );

	//���̕\��
	DDBlt((int)g_cursor.x , (int)g_cursor.y , g_cursor.w , g_cursor.h , g_configImg[0] , 0 , 0);

	//���̑��̕����J�[�\���\��
	//�U���̎g�p�L�[���P�U�i���ŕ\��
	wsprintf(&mes[0] , "0x%d" , g_config[ATTACK]);
	DDTextOut(&mes[0] , 300 , 100);
	wsprintf(&mes[0] , "0x%d" , g_config[JUMP]);
	DDTextOut(&mes[0] , 300 , 200);
	wsprintf(&mes[0] , "0x%d" , g_config[DUSH]);
	DDTextOut(&mes[0] , 300 , 300);
}



//*************************************************************************************************
//����@ReleaseConfig
//*************************************************************************************************
void ReleaseConfig(){
	
	int i = 0;
	
	//�摜
	for(i = 0;i < CONFIG_IMG_MAX;i++){
		//���̊G�����
		ReleaseDDImage(g_configImg[i]);
	}
	//��
	for(i = 0;i < CONFIG_SOUND_EFFECT_MAX;i++){
		ReleaseSound(g_co_sound[i]);
	}
	//�Ȃ̉��
	ReleaseStreaming();
}

//************************************************************************************************
//�g�p�L�[�̎擾
//************************************************************************************************
int GetConfig(int action){
	return g_config[action];		//�����̍s���Ŏg���{�^����ݒ�
}
