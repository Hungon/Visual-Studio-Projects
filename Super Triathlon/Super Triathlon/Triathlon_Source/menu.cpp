#include "menu.h"
#include "sound.h"
#include "input.h"
#include "wipe.h"
#include "game.h"
#include "swimer.h"
#include "seaenemy.h"
#include "item.h"
#include "score.h"
#include "map.h"
#include "play.h"
#include <stdio.h>

//���O���[�o���ϐ�
int g_menuImg[MENU_IMAGE_MAX] = {0};
//CHARACTER�\����
CHARACTER g_menu[MENU_BACK_IMAGE_MAX] = {0};		//���j���[�w�i
CHARACTER g_menuItem[MENU_KIND_OF_ITEM] = { 0 };	//���j���[����
CHARACTER g_menuCursor = {0};						//�J�[�\��
CHARACTER g_menuAnswer[2] = { 0 };					//yes no
//���j���[�J��������̋󂫎���
int g_menuDontCloseTime = 0;

//���ʉ�
int g_menu_sound[MENU_EFFECT_SOUND_MAX] = {0};

//��ʑJ�ڂ���v�f���擾�p
tagWIPE_TRANSITION g_transition = NOT_TRANSITION;

//���j���[���̑J��
tagMENUSELECTION	g_menuSelection = MENU_SELECT_VACANT;
tagMENUSELECTION	g_menuSelectionNext = MENU_SELECT_VACANT;


//**************************************************************************************************************************
//������
//**************************************************************************************************************************
void InitMenu(HWND hWnd){

	//���摜�̓ǂݍ���
	//���j���[�w�i
	g_menuImg[0] = CreateDDImage("Image\\menu.bmp" , hWnd);
	//���j���[����
	g_menuImg[1] = CreateDDImage("Image\\menu_word.bmp" , hWnd);
	//�J�[�\��
	g_menuImg[2] = CreateDDImage("Image\\cursor.bmp",hWnd);
	//�I���uYES NO�v
	g_menuImg[3] = CreateDDImage("Image\\select.bmp",hWnd);


	//�����ʉ��̓ǂݍ���
	//�J�[�\����
	g_menu_sound[0] = CreateSound("SE\\cursor.wav");
	//���艹
	g_menu_sound[1] = CreateSound("SE\\ok.wav");
	//�L�����Z����
	g_menu_sound[2] = CreateSound("SE\\cancel.wav");

	//���j���[�w�i
	ZeroMemory(&g_menu[0], sizeof(CHARACTER) * MENU_BACK_IMAGE_MAX);
	//�J�[�\���̏�����
	ZeroMemory(&g_menuCursor, sizeof(CHARACTER) * 1);
	//yes no
	ZeroMemory(&g_menuAnswer[0], sizeof(CHARACTER) * 2);

	//��ʑJ�ڂ���v�f���擾�p
	g_transition = NOT_TRANSITION;
	//���j���[���̑J��
	g_menuSelection = MENU_SELECT_VACANT;
	g_menuSelectionNext = MENU_SELECT_VACANT;
	//���j���[����邱�Ƃ��ł��Ȃ�����
	g_menuDontCloseTime = 0;

	//���e�摜�̐ݒ�
	//���j���[�w�i
	g_menu[0].x = MENU_INDICATE_X;
	g_menu[0].y = MENU_INDICATE_Y;
	g_menu[0].w = 160;
	g_menu[0].h = 224;
	g_menu[0].flag = false;
	
	//���j���[����
	for (int i = 0; i < MENU_KIND_OF_ITEM; i++) {
		g_menuItem[i].x = MENU_INDICATE_X;
		g_menuItem[i].y = MENU_INDICATE_Y +
			( ( MENU_IMAGE_ITEM_HEIGHT + MENU_ITEM_INDICATE_SPACE_Y) * i);
		g_menuItem[i].w = MENU_IMAGE_ITEM_WIDTH;
		g_menuItem[i].h = MENU_IMAGE_ITEM_HEIGHT;
		g_menuItem[i].picx = 0;
		g_menuItem[i].picy = (MENU_IMAGE_ITEM_HEIGHT * i);
		g_menuItem[i].flag = false;
	}


	//yes no
	for (int i = 0; i < 2; i++) {
		g_menuAnswer[i].x = MENU_ANSWER_INDICATE_X;
		g_menuAnswer[i].y = (MENU_ANSWER_INDICATE_Y + 
			( ( MENU_ANSWER_INDICATE_SPACE_Y + MENU_IMAGE_ANSWER_HEIGHT ) * i ));
		g_menuAnswer[i].w = MENU_IMAGE_ANSWER_WIDTH;
		g_menuAnswer[i].h = MENU_IMAGE_ANSWER_HEIGHT;
		g_menuAnswer[i].picx = 0;
		g_menuAnswer[i].picy = (MENU_IMAGE_ANSWER_HEIGHT * i);
		g_menuAnswer[i].flag = false;
	}

	//�J�[�\��
	g_menuCursor.x = g_menuItem[1].x;
	g_menuCursor.y = g_menuItem[1].y;
	g_menuCursor.w = g_menuItem[1].w;
	g_menuCursor.h = 48;
	g_menuCursor.my = MENU_IMAGE_ITEM_HEIGHT + MENU_ITEM_INDICATE_SPACE_Y;
	
}

//**************************************************************************************************************************
//�X�V
//**************************************************************************************************************************
void UpdateMenu(){

	//���j���[��ʂ��J��
	if(g_menu[0].flag == false) {
		if (JustKey('M') || JustButton(BUTTON_8)) {
			//���ʉ�
			PlaySound(g_menu_sound[1], 0, false);
			//�w�i�@�e�퍀�ڂ�\��
			OpenMenu();
		}
	}

	//���j���[���J������
	if (g_menu[0].flag) {
		//�J��������̓��j���[����邱�Ƃ��ł��Ȃ�
		g_menuDontCloseTime++;
		if (g_menuDontCloseTime > MENU_DONT_CONTROL_TIME) { g_menuDontCloseTime = MENU_DONT_CONTROL_TIME; }

		//���j���[���J��
		switch (g_menuSelection) {
		case MENU_SELECT_ITEM:
			//�e�퍀�ڂ̑I��
			MenuSelectItem();
			break;
		case MENU_SELECT_ANSWER:
			//�I��
			MenuAnswer();
			break;
		case MENU_SELECT_CLOSE:
			//���j���[�̏�����
			CloseMenu();
			break;
		}
		//����s�\���Ԃ����
		if (g_menuSelection != g_menuSelectionNext) {
			g_menu[0].time++;
			if (g_menu[0].time >= MENU_DONT_CONTROL_TIME) {
				g_menuSelection = g_menuSelectionNext;
				g_menu[0].time = 0;
			}
		}
		//���j���[�����
		if (JustKey('M') || JustButton(BUTTON_8)){
			if (g_menuDontCloseTime >= MENU_DONT_CONTROL_TIME) {
				g_menuSelectionNext = MENU_SELECT_CLOSE;
				//���ʉ�
				PlaySound(g_menu_sound[2], 0, false);
			}
		}
	}
}

//**************************************************************************************************************************
//�\��
//**************************************************************************************************************************
void DrawMenu(){
	
	//���j���[�̕\��
	if(g_menu[0].flag){
		//���j���[�w�i
		DDBlt((int)g_menu[0].x ,
			(int)g_menu[0].y ,
			g_menu[0].w ,
			g_menu[0].h ,
			g_menuImg[0] ,
			g_menu[0].picx ,
			g_menu[0].picy);
		
		//���j���[���ڂ̕\��
		for (int i = 0; i < MENU_KIND_OF_ITEM; i++) {
			if (g_menuItem[i].flag) {
				DDBlt((int)g_menuItem[i].x,
					(int)g_menuItem[i].y,
					g_menuItem[i].w,
					g_menuItem[i].h,
					g_menuImg[1],
					g_menuItem[i].picx,
					g_menuItem[i].picy);
			}
		}
	
		//yes no
		for (int i = 0; i < 2; i++) {
			if (g_menuAnswer[i].flag) {
				//�uYES�v
				DDBlt((int)g_menuAnswer[i].x,
					(int)g_menuAnswer[i].y,
					g_menuAnswer[i].w,
					g_menuAnswer[i].h,
					g_menuImg[3],
					g_menuAnswer[i].picx,
					g_menuAnswer[i].picy);
			}
		}

		//�J�[�\��
		if (g_menuCursor.flag) {
			DDAlphaBlt(
				(int)g_menuCursor.x,
				(int)g_menuCursor.y,
				g_menuCursor.w,
				g_menuCursor.h,
				g_menuImg[2],
				g_menuCursor.picx,
				g_menuCursor.picy,
				120.0f);
		}
	}
}

//**************************************************************************************************************************
//���
//**************************************************************************************************************************
void ReleaseMenu(){

	//���摜�̉��
	for (int i = 0; i < MENU_IMAGE_MAX; i++) {
		ReleaseDDImage(g_menuImg[i]);
	}
	//�����ʉ��̉��
	for (int i = 0; i < MENU_EFFECT_SOUND_MAX; i++) {
		ReleaseSound(g_menu_sound[i]);
	}
}

//**************************************************************************************************************************
//���j���[���J��
//**************************************************************************************************************************
void OpenMenu(){
	//�w�i�\��
	g_menu[0].flag = true;
	//�e�퍀�ڂ�\��
	for (int i = 0; i < MENU_KIND_OF_ITEM; i++) { g_menuItem[i].flag = true; }
	//�J�[�\��
	g_menuCursor.x = g_menuItem[1].x;
	g_menuCursor.y = g_menuItem[1].y;
	g_menuCursor.w = g_menuItem[1].w;
	g_menuCursor.h = 48;
	//�J�ڃt���O
	g_menuSelection = MENU_SELECT_VACANT;
	g_menuSelectionNext = MENU_SELECT_ITEM;
	//���j���[����邱�Ƃ��ł��Ȃ�����
	g_menuDontCloseTime = 0;

}

//**************************************************************************************************************************
//���j���[�����擾
//**************************************************************************************************************************
bool GetMenu(){return g_menu[0].flag; }

/*
���j���[�����
*/
void	CloseMenu() {
	//�e��w�i
	for (int i = 0; i < MENU_BACK_IMAGE_MAX; i++) { g_menu[i].flag = false; }
	//����
	for (int i = 0; i < MENU_KIND_OF_ITEM; i++) { g_menuItem[i].flag = false; }
	// yes no
	for (int i = 0; i < 2; i++) { g_menuAnswer[i].flag = false; }
	g_menu[0].time = 0;
	g_menuCursor.flag = false;
	//�J�ڃt���O
	g_menuSelection = MENU_SELECT_VACANT;
}

/*
�e�퍀�ڑI��
*/
void	MenuSelectItem() {

	tagWIPE_TRANSITION selectTbl[MENU_KIND_OF_ITEM - 1] = {

		RESTART_RACE,		//�X�^�[�g����n�߂�
		TO_TITLE,			//�^�C�g����
		TO_SELECT,			//�X�e�[�W�Z���N�g��
	};


	//���L�[����
	//���j���[�I��
	if (g_menuItem[0].flag) {
		//�J�[�\���̈ړ���
		g_menuCursor.my = MENU_IMAGE_ITEM_HEIGHT + MENU_ITEM_INDICATE_SPACE_Y;
		//�J�[�\���\���t���O
		g_menuCursor.flag = true;

		//������
		if (JustKey(VK_DOWN) || JustLever(LEVER_DOWN)) {
			//�J�[�\����
			PlaySound(g_menu_sound[0], 0, false);
			//�{�^���̊Ԋu����������
			g_menuCursor.y += g_menuCursor.my;
			//���̍��ڂ܂ŗ������̍��ڂ�
			if (g_menuCursor.y > g_menuItem[3].y) g_menuCursor.y = g_menuItem[1].y;
		}
		//�����
		if (JustKey(VK_UP) || JustLever(LEVER_UP)) {
			//�J�[�\����
			PlaySound(g_menu_sound[0], 0, false);
			//�{�^���̊Ԋu����������
			g_menuCursor.y -= g_menuCursor.my;
			//���̍��ڂ܂ŗ������̍��ڂ�
			if (g_menuCursor.y < g_menuItem[1].y) g_menuCursor.y = g_menuItem[3].y;
		}

		//������L�[
		for (int i = 0; i < (MENU_KIND_OF_ITEM - 1); i++) {
			//�uRESTART�v
			if (g_menuCursor.y == g_menuItem[i + 1].y) {
				//�������킹��
				g_menuCursor.w = g_menuItem[i + 1].w;
				//���������킹��
				g_menuCursor.h = g_menuItem[i + 1].h;
				//����
				if (JustUpKey('Z') || JustButton(BUTTON_1)) {
					//���ʉ�
					PlaySound(g_menu_sound[1], 0, false);
					//���ڂ��\��
					for (int i = 0; i < MENU_KIND_OF_ITEM; i++) { g_menuItem[i].flag = false; }
					//�J�[�\���\���t���O
					g_menuCursor.flag = false;
					//yes no��\��
					for (int i = 0; i < 2; i++) { g_menuAnswer[i].flag = true; }
					//��ʑJ�ڂ���v�f���擾
					g_transition = selectTbl[i];
					//�J�ڃt���O
					g_menuSelectionNext = MENU_SELECT_ANSWER;
					//�J�[�\����yes�ɍ��킹��
					g_menuCursor.y = g_menuAnswer[0].y;
					g_menuCursor.x = g_menuAnswer[0].x;
				}
			}
		}
	}
}

/*
�e��I���ւ̕ԓ�
*/
void	MenuAnswer() {

	//����ʑJ�ڂ̑I��
	if (g_menuAnswer[0].flag) {
		//�J�[�\���̈ړ���
		g_menuCursor.my = MENU_IMAGE_ANSWER_HEIGHT + MENU_ANSWER_INDICATE_SPACE_Y;
		//�J�[�\���\���t���O
		g_menuCursor.flag = true;
		//������
		if (JustKey(VK_DOWN) || JustLever(LEVER_DOWN)) {
			//�J�[�\����
			PlaySound(g_menu_sound[0], 0, false);
			//�{�^���̊Ԋu����������
			g_menuCursor.y += g_menuCursor.my;
			//���̍��ڂ܂ŗ������̍��ڂ�
			if (g_menuCursor.y > g_menuAnswer[1].y) {
				g_menuCursor.y = g_menuAnswer[0].y;
			}
		}
		//�����
		if (JustKey(VK_UP) || JustLever(LEVER_UP)) {
			//�J�[�\����
			PlaySound(g_menu_sound[0], 0, false);
			//�{�^���̊Ԋu����������
			g_menuCursor.y -= g_menuCursor.my;
			//���̍��ڂ܂ŗ������̍��ڂ�
			if (g_menuCursor.y < g_menuAnswer[0].y) {
				g_menuCursor.y = g_menuAnswer[1].y;
			}
		}
		//����uYES�v��I��
		if (g_menuCursor.y == g_menuAnswer[0].y) {
			//�������킹��
			g_menuCursor.w = g_menuAnswer[0].w;
			//���������킹��
			g_menuCursor.h = g_menuAnswer[0].h;
			//����
			if (JustKey('Z') || JustButton(BUTTON_1)) {
				//���ʉ�
				PlaySound(g_menu_sound[1], 0, false);
				//��ʑJ��
				CreateWipe(MENU_TRANSITION, g_transition, WIPE_CONTRACTION);
			}
		}
		//����uNO�v��I��
		if (g_menuCursor.y == g_menuAnswer[1].y) {
			//�������킹��
			g_menuCursor.w = g_menuAnswer[1].w;
			//���������킹��
			g_menuCursor.h = g_menuAnswer[1].h;
			//����
			if (JustUpKey('Z') || JustUpButton(BUTTON_1)) {
				//���ʉ�
				PlaySound(g_menu_sound[2], 0, false);
				//���ڂ�\��
				for (int i = 0; i < MENU_KIND_OF_ITEM; i++) { g_menuItem[i].flag = true; }
				//yes no���\��
				for (int i = 0; i < 2; i++) { g_menuAnswer[i].flag = false; }
				//�J�[�\���\���t���O
				g_menuCursor.flag = false;
				//�J�[�\�������ڂɍ��킹��
				g_menuCursor.y = g_menuItem[1].y;
				g_menuCursor.x = g_menuItem[1].x;
				//�J�ڂ���v�f�����Z�b�g
				g_transition = NOT_TRANSITION;
				//�J�ڃt���O
				g_menuSelectionNext = MENU_SELECT_ITEM;
			}
		}
	}

}