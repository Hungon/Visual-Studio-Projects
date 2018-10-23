#include "effect.h"
#include "map.h"
#include <stdio.h>



//���O���[�o���ϐ�
EFFECT g_effect[EFFECT_MAX] = {0};
int g_effectImg[1] = {0};

//************************************************************************************************
//��������
//************************************************************************************************
void InitEffect(HWND hWnd){

	int i = 0;
	
	//�G�t�F�N�g�摜��ǂݍ���
	g_effectImg[0] = CreateDDImage("Image\\Sea\\bubble.bmp",hWnd);
	
	//�G�t�F�N�g�摜�̏����ݒ�
	for(i = 0;i < EFFECT_MAX;i++){

		g_effect[i].ef.flag = false;
		g_effect[i].ef.h = 32;
		g_effect[i].ef.w = 32;
		g_effect[i].ef.scale = 1.0f;	//�g�k�{��

		//�A�j���[�V�����ݒ�
		g_effect[i].ani.fram = 10;		//�t���[����
		g_effect[i].ani.max = 5;		//�ő�R�}��


		SetAnimation(&g_effect[i].ef,&g_effect[i].ani,
			g_effect[i].ani.sx,g_effect[i].ani.sy,
			g_effect[i].ani.max,g_effect[i].ani.fram,
			g_effect[i].type,false);
	}
}

//************************************************************************************************
//�X�V
//************************************************************************************************
void UpdateEffect(){

	int i = 0;

	for(i = 0;i < EFFECT_MAX;i++){
		//�G�t�F�N�g�̐����t���O��true�Ȃ�A�j���[�V����
		if(g_effect[i].ef.flag == true){	
			//�G�t�F�N�g�̐����t���O�ɃA�j���[�V�����̍X�V�t���O����
			g_effect[i].ef.flag = UpdateAnimation(&g_effect[i].ef,&g_effect[i].ani);
		}
	}
}

//************************************************************************************************
//�\��
//************************************************************************************************
void DrawEffect(){

	int i = 0;

	float px = GetCameraX();						
	float py = GetCameraY();						

	for(i = 0;i < EFFECT_MAX;i++){
		//�G�t�F�N�g�̐����t���O��true�Ȃ�\��
		if(g_effect[i].ef.flag == true){			
			//�G�t�F�N�g�摜
			DDBlt(
				(int)( g_effect[i].ef.x - px ) ,//X���W
				(int)( g_effect[i].ef.y - py ) ,//Y���W
				g_effect[i].ef.w,				//��
				g_effect[i].ef.h,				//����
				g_effectImg[0],					//�Ή������摜�̔ԍ�
				g_effect[i].ef.picx,			//�\�����w���W
				g_effect[i].ef.picy);			//�\�����x���W		
		}
	}
}

//************************************************************************************************
//���
//************************************************************************************************
void ReleaseEffect(){
	ReleaseDDImage(g_effectImg[0]);
}

//************************************************************************************************
//�G�t�F�N�g�𔭐�������֐�
//************************************************************************************************
void CreateEffect(float x,float y){

	int i = 0;

	for(i = 0;i < EFFECT_MAX;i++){
		//�G�l�~�[�̐����t���O��false�Ȃ珈�����s��
		if(g_effect[i].ef.flag == false){
				
			//�G�̍��W�������̒l�ɐݒ�
			g_effect[i].ef.x = x;
			g_effect[i].ef.y = y;
			//�\���T�C�Y
			g_effect[i].ef.w = 32;
			g_effect[i].ef.h = 32;
			//�����t���O��true�ɂ���
			g_effect[i].ef.flag = true;
			//�A�j���[�V�����ݒ�
			SetAnimation(&g_effect[i].ef,&g_effect[i].ani,
				g_effect[i].ani.sx,g_effect[i].ani.sy,
				g_effect[i].ani.max,g_effect[i].ani.fram,
				g_effect[i].type,false);

			return;
		}
	}
}

//************************************************************************************************
//�X�e�[�W�ύX���̃G�t�F�N�g�̃��Z�b�g
//************************************************************************************************
void ResetEffect(){
	int i = 0;
	//�G�t�F�N�g�̏�����
	for(i = 0;i < EFFECT_MAX;i++){
		ZeroMemory(&g_effect[i] , sizeof(EFFECT));
	}
}