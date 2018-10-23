#include    "opening.h"
#include    "game.h"
#include    "input.h"
#include    "sound.h"
#include    "wipe.h"
#include    "score.h"
#include    "play.h"
#include    "result.h"
#include	"SelectStage.h"
#include	"map.h"
#include	 <stdio.h>


//���O���[�o���ϐ�
int g_openingImg[ OPENING_IMAGE_MAX ] = {0};
//�^�C�g�����S�p
CHARACTER	g_OpTitleLogo[ OPENING_TITLE_LOGO_KIND_OF_IMAGE ] = {0};
//�e��\���ݒ�p
BUTTON g_Opbutton[ OPENING_BUTTON_KIND_OF_IMAGE ] = {0};
//���̕\���ݒ�p
CHARACTER g_cursor1 = {0};

//�I�[�v�j���O���̃V�[��������
//����
BYTE	g_OpSceneFlagNow = OPENING_SCENE_TITLE_LOGO_DIRECTION;		//�n�߂̓^�C�g�����S���o
//��
BYTE	g_OpSceneFlagNext = OPENING_SCENE_TITLE_LOGO_DIRECTION;		//�n�߂̓^�C�g�����S���o


//���̓ǂݍ���
int g_op_sound[ OPENING_SOUND_EFFECT_MAX ] = {0};	//�J�[�\����

//************************************************************************************************
//��������
//************************************************************************************************
void InitOpening(HWND hWnd) {

	//�J�E���g�p
	int cnt = 0;

	//get current language
	tagGAMELANGUAGE language = GetGameLanguage();

	//�摜�f�[�^
	char	*lpImageTbl[OPENING_IMAGE_MAX] = {
		{ "Image\\title.bmp" }				,		//�w�i
		{ "Image\\GameTitle.bmp" }			,		//�Q�[���^�C�g��
		{ "Image\\OpeningButton.bmp" }		,		//�e��{�^��
		{ "Image\\cursor.bmp" }		,				//�J�[�\��
		{ "Image\\EventImage.bmp" }			,		//�e��X�e�[�W�C���[�W�摜
		{ "Image\\menu.bmp" }				,		//�{�^���摜�̔w�i

	};

	//�e��X�e�[�W�C���[�W�摜�̉摜�T�C�Y�f�[�^
	//��v�f�F�����A��
	int		StageImageTbl[STAGE_MAX][2] = {
		{ 64 , 192 }		,			//�C
		{ 128 , 110 }		,			//���H
		{ 128 , 192 }		,			//�I�t���[�h
	};

	//�X�e�[�W
	tagSELECTSTAGE stageTbl[STAGE_MAX] = { STAGE_SEA , STAGE_ROAD , STAGE_OFF_ROAD };

	//�� �N���A�σX�e�[�W�����Z�b�g
	for (int i = 0; i < STAGE_MAX; i++) {
		SetCompleteStage(SELECTSTAGE_STAGE_NOT_COMPLETE, stageTbl[i]);
	}

	//���摜�̓ǂݍ���
	for (int i = 0; i < OPENING_IMAGE_MAX; i++) { g_openingImg[i] = CreateDDImage(lpImageTbl[i], hWnd); }

	//�����f�[�^�̓ǂݍ���
	//�J�[�\����
	g_op_sound[0] = CreateSound("SE\\cursor.wav");
	//���艹
	g_op_sound[1] = CreateSound("SE\\ok.wav");
	//�L�����Z����
	g_op_sound[2] = CreateSound("SE\\cancel.wav");


	//�^�C�g����ʂł̋�
	CreateStreaming("BGM\\title.wav");

	//�� �^�C�g�����S�\���ݒ�
	g_OpTitleLogo[0].picy = 0;								//case current language is Japanese
	//change title logo image accoriding to current language
	if (language == GAME_LANGUAGE_ENGLISH) g_OpTitleLogo[0].picy = (OPENING_IMAGE_TITLE_LOGO_HEIGHT * 4);
	//�Q�[���^�C�g��
	g_OpTitleLogo[0].x = OPENING_TITLE_LOGO_BIGIN_INDICATE_X;			//�\����X
	g_OpTitleLogo[0].y = OPENING_TITLE_LOGO_BIGIN_INDICATE_Y;			//�\����Y
	g_OpTitleLogo[0].w = OPENING_IMAGE_TITLE_LOGO_WIDTH;				//��
	g_OpTitleLogo[0].h = OPENING_IMAGE_TITLE_LOGO_HEIGHT;				//����
	g_OpTitleLogo[0].flag = true;										//�\���t���O
	g_OpTitleLogo[0].my = OPENING_TITLE_LOGO_MOVE_Y;					///�ړ���Y
	g_OpTitleLogo[0].millisecond = 0;									//�o�ߎ���
	//�ŏI�I�Ȉʒu
	g_OpTitleLogo[0].ey = OPENING_TITLE_LOGO_TERMINATE_INDICATE_Y;		//�I�_Y
	g_OpTitleLogo[0].ex = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(OPENING_IMAGE_TITLE_LOGO_WIDTH);//X
	//�^�C�g�����S�̔w��̐}�`
	for (int i = 4; i < OPENING_TITLE_LOGO_KIND_OF_IMAGE; i++) {
		//�v�f�J�E���g
		cnt++;
		//�����牄�т�}�`
		g_OpTitleLogo[i].x = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH
			(OPENING_IMAGE_TITLE_LOGO_WIDTH);								//�\����X
		g_OpTitleLogo[i].y = OPENING_TITLE_LOGO_TERMINATE_INDICATE_Y;		//�\����Y
		g_OpTitleLogo[i].w = OPENING_IMAGE_TITLE_LOGO_WIDTH;				//��
		g_OpTitleLogo[i].h = OPENING_IMAGE_TITLE_LOGO_HEIGHT;				//����
		g_OpTitleLogo[i].picy = (OPENING_IMAGE_TITLE_LOGO_HEIGHT * cnt);	//�\����Y
		g_OpTitleLogo[i].flag = false;										//�\���t���O
	}
	//�E����L�т�}�`�͕���0�Ŏ��Ԍo�߂ŐL�΂��Ă���
	g_OpTitleLogo[6].w = 0;


	//�e��X�e�[�W�C���[�W�摜
	for (int i = 1; i < (1 + OPENING_TITLE_STAGE_IMAGING_KIND_OF_IMAGE); i++) {
		g_OpTitleLogo[i].x = OPENING_STAGE_IMAGE_CHARACTER_INDICATE_X;				//�\����X
		g_OpTitleLogo[i].y = OPENING_STAGE_IMAGE_CHARACTER_INDICATE_Y;				//�\����Y
		g_OpTitleLogo[i].ey = OPENING_STAGE_IMAGE_CHARACTER_INDICATE_Y;				//�ŏI�I�ȕ\����Y
		g_OpTitleLogo[i].w = StageImageTbl[i - 1][1];								//��
		g_OpTitleLogo[i].h = StageImageTbl[i - 1][0];								//����
		g_OpTitleLogo[i].flag = false;												//�\���t���O
		g_OpTitleLogo[i].mx = (OPENING_STAGE_IMAGE_CHARACTER_MOVE_X + (i - 1));	//�ړ���X
		g_OpTitleLogo[i].scale = OPENING_STAGE_IMAGE_CHARACTER_SCALE;				//�g�嗦
	}
	//�ŏI�I�Ȉʒu
	g_OpTitleLogo[1].ex = OPENING_STAGE_IMAGE_CHARACTER_TERMINATE_INDICATE_X;
	g_OpTitleLogo[2].ex = g_OpTitleLogo[1].ex + StageImageTbl[0][1] +
		OPENING_STAGE_IMAGE_CHARACTER_SPACE_X;
	g_OpTitleLogo[3].ex = g_OpTitleLogo[2].ex + StageImageTbl[1][1] +
		OPENING_STAGE_IMAGE_CHARACTER_SPACE_X;
	//�\����Y
	g_OpTitleLogo[1].picy = 0;
	g_OpTitleLogo[2].picy = StageImageTbl[0][0];
	g_OpTitleLogo[3].picy = StageImageTbl[0][0] + StageImageTbl[1][0];

	//�I�[�v�j���O���̃V�[��������
	//����
	g_OpSceneFlagNow = OPENING_SCENE_TITLE_LOGO_DIRECTION;		//�n�߂̓^�C�g�����S���o
	//��
	g_OpSceneFlagNext = OPENING_SCENE_TITLE_LOGO_DIRECTION;		//�n�߂̓^�C�g�����S���o

	//�� �e��{�^���摜�ݒ�
	for (int i = 0; i < OPENING_BUTTON_KIND_OF_IMAGE; i++) {
		g_Opbutton[i].ch.w = OPENING_BUTTON_IMAGE_WIDTH;				//��
		g_Opbutton[i].ch.h = OPENING_BUTTON_IMAGE_HEIGHT;				//����
		g_Opbutton[i].ch.picy = (i * OPENING_BUTTON_IMAGE_HEIGHT);		//�\����Y
		g_Opbutton[i].ch.flag = false;									//�\���t���O

	}
	// [Start]
	g_Opbutton[0].ch.x = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(OPENING_BUTTON_IMAGE_WIDTH);
	g_Opbutton[0].ch.y = OPENING_BUTTON_BEGIN_IMAGE_INDICATE_Y;
	g_Opbutton[0].ch.flag = false;
	//�uOPTION�v
	g_Opbutton[1].ch.x = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(OPENING_BUTTON_IMAGE_WIDTH);
	g_Opbutton[1].ch.y = g_Opbutton[0].ch.y + OPENING_BUTTON_IMAGE_INDICATE_SPACE_Y;
	g_Opbutton[1].ch.flag = false;

	//��Փx
	//EASY
	g_Opbutton[2].ch.x = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(OPENING_BUTTON_IMAGE_WIDTH);
	g_Opbutton[2].ch.y = OPENING_BUTTON_LEVEL_IMAGE_INDICATE_Y;
	//NORMAL
	g_Opbutton[3].ch.x = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(OPENING_BUTTON_IMAGE_WIDTH);
	g_Opbutton[3].ch.y = g_Opbutton[2].ch.y + OPENING_BUTTON_IMAGE_INDICATE_SPACE_Y;
	//HARD
	g_Opbutton[4].ch.x = CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(OPENING_BUTTON_IMAGE_WIDTH);
	g_Opbutton[4].ch.y = g_Opbutton[3].ch.y + OPENING_BUTTON_IMAGE_INDICATE_SPACE_Y;

	//�{�^���摜�̔w�i
	g_Opbutton[5].ch.x = g_Opbutton[0].ch.x - OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN;
	g_Opbutton[5].ch.y = g_Opbutton[0].ch.y - OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN;
	g_Opbutton[5].ch.w = g_Opbutton[0].ch.w + (int)(OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN * 2);
	g_Opbutton[5].ch.h = (int)(((g_Opbutton[1].ch.y + g_Opbutton[1].ch.h + OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN)
		- (g_Opbutton[0].ch.y - OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN)));
	g_Opbutton[5].ch.picy = 0;
	g_Opbutton[5].ch.alpha = 100.0f;
	g_Opbutton[5].ch.flag = false;

	//�J�[�\��
	g_cursor1.x = g_Opbutton[0].ch.x;
	g_cursor1.y = g_Opbutton[0].ch.y;
	g_cursor1.w = g_Opbutton[0].ch.w;
	g_cursor1.h = OPENING_BUTTON_IMAGE_HEIGHT;
	g_cursor1.my = OPENING_BUTTON_IMAGE_INDICATE_SPACE_Y;
	g_cursor1.flag = false;
}

//************************************************************************************************
//�X�V
//************************************************************************************************
void UpdateOpening() {


	//�� �e��I�[�v�j���O���̑J��
	if (g_OpSceneFlagNow == g_OpSceneFlagNext) {
		switch (g_OpSceneFlagNow) {
		case OPENING_SCENE_TITLE_LOGO_DIRECTION:
			//�� �^�C�g�����S���o�̍X�V
			UpdateTitleLogoDirect();
			break;
		case OPENING_SCENE_SELECT_START_FUNCTION:
			//�� �{�^���ݒ�1�iStart , Option�j
			SetFirstButton();
			break;
		case OPENING_SCENE_SELECT_LEVEL_FUNCTION:
			//�� �{�^���ݒ�2 ���x���ݒ�
			SetSecondButton();
			break;
		}
	}

	//���݂̑J�ڃt���O�Ǝ��̑J�ڃt���O��������玞�Ԍo��
	if (g_OpSceneFlagNow != g_OpSceneFlagNext) g_OpTitleLogo[0].time++;
	//��莞�Ԍo�߂Ŏ��̃V�[������
	if (g_OpTitleLogo[0].time >= OPENING_CURSOR_DONT_ACCEPT_CONTROL_TIME) {
		g_OpSceneFlagNow = g_OpSceneFlagNext;
		g_OpTitleLogo[0].time = 0;
	}

	//�Ȃ̃X�g���[�~���O�Đ�
	PlayStreaming(true);
}

//************************************************************************************************
//�\��
//************************************************************************************************
void DrawOpening() {

	//�I�[�v�j���O�摜�̕\��
	DDBlt(
		0, 0,						//�\�����WX�x
		640, 480,					//���A����
		g_openingImg[0],			//�Ή������摜�ԍ�
		0, 0);						//�\�������WX�E�x

	//�^�C�g�����S�̔w��̐}�`�\��
	for (int i = 4; i < OPENING_TITLE_LOGO_KIND_OF_IMAGE; i++) {
		if (g_OpTitleLogo[i].flag) {
			DDBlt(
				(int)g_OpTitleLogo[i].x,
				(int)g_OpTitleLogo[i].y,
				g_OpTitleLogo[i].w,
				g_OpTitleLogo[i].h,
				g_openingImg[1],
				g_OpTitleLogo[i].picx,
				g_OpTitleLogo[i].picy);
		}
	}

	//�^�C�g�����S�̕\��
	if (g_OpTitleLogo[0].flag) {
		DDBlt(
			(int)g_OpTitleLogo[0].x,
			(int)g_OpTitleLogo[0].y,
			g_OpTitleLogo[0].w,
			g_OpTitleLogo[0].h,
			g_openingImg[1],
			g_OpTitleLogo[0].picx,
			g_OpTitleLogo[0].picy);
	}

	//�e��X�e�[�W�C���[�W�摜�̕\��
	for (int i = 1; i < (1 + OPENING_TITLE_STAGE_IMAGING_KIND_OF_IMAGE); i++) {
		if (g_OpTitleLogo[i].flag) {
			DDScaleBltSelectIndicate(
				g_OpTitleLogo[i].x,
				g_OpTitleLogo[i].y,
				g_OpTitleLogo[i].w,
				g_OpTitleLogo[i].h,
				g_openingImg[4],
				g_OpTitleLogo[i].picx,
				g_OpTitleLogo[i].picy,
				g_OpTitleLogo[i].scale,
				0);
		}
	}

	//�e��{�^���摜�̔w�i
	if (g_Opbutton[5].ch.flag) {
		DDAlphaBlt(
			(int)g_Opbutton[5].ch.x,
			(int)g_Opbutton[5].ch.y,
			g_Opbutton[5].ch.w,
			g_Opbutton[5].ch.h,
			g_openingImg[5],
			g_Opbutton[5].ch.picx,
			g_Opbutton[5].ch.picy,
			g_Opbutton[5].ch.alpha);
	}


	//�e��{�^���摜�̕\��
	for (int i = 0; i < (OPENING_BUTTON_KIND_OF_IMAGE - 1); i++) {
		//�uStart�v[Option]�@�e�탌�x��
		if (g_Opbutton[i].ch.flag) {
			DDBlt(
				(int)g_Opbutton[i].ch.x,
				(int)g_Opbutton[i].ch.y,
				g_Opbutton[i].ch.w,
				g_Opbutton[i].ch.h,
				g_openingImg[2],
				g_Opbutton[i].ch.picx,
				g_Opbutton[i].ch.picy);
		}
	}

	//�J�[�\��
	if (g_cursor1.flag) {
		DDAlphaBlt((int)g_cursor1.x,
			(int)g_cursor1.y,
			g_cursor1.w,
			g_cursor1.h,
			g_openingImg[3],
			g_cursor1.picx,
			g_cursor1.picy,
			120.0f);
	}
}

//************************************************************************************************
//���
//************************************************************************************************
void ReleaseOpening() {


	//�摜�̉��
	for (int i = 0; i < OPENING_IMAGE_MAX; i++) {
		ReleaseDDImage(g_openingImg[i]);
	}
	//���ʉ��̉��
	for (int i = 0; i < OPENING_SOUND_EFFECT_MAX; i++) {
		ReleaseSound(g_op_sound[i]);
	}
	//�Ȃ̉��
	ReleaseStreaming();
}


/*
**************************************************************************************************
�^�C�g�����S���o�̍X�V
**************************************************************************************************
*/
void	UpdateTitleLogoDirect() {

	//�^�C�g�����S�̋Ȑ���`�����W
	POINT pos[OPENING_TITLE_LOGO_SET_BEZIER] = {
		{ OPENING_TITLE_LOGO_BIGIN_INDICATE_X ,
		  OPENING_TITLE_LOGO_BIGIN_INDICATE_Y } ,
		{ 100 , -100 } ,
		{ 150 , 50 } ,
		{ CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH(OPENING_IMAGE_TITLE_LOGO_WIDTH) ,
		OPENING_TITLE_LOGO_TERMINATE_INDICATE_Y }
	};

	//�^�C�g�����S�̈ړ��ʂ̍X�V
	if (g_OpTitleLogo[0].y < g_OpTitleLogo[0].ey) {
		//�~���b�̌o��
		g_OpTitleLogo[0].millisecond += 0.01;
		//�Ȑ���`��
		SetBezier(&g_OpTitleLogo[0], &pos[0],
			(OPENING_TITLE_LOGO_SET_BEZIER - 1), g_OpTitleLogo[0].millisecond);
	}
	//�\����ɓ��B������C���[�W�摜�̕\��
	if (g_OpTitleLogo[0].y >= g_OpTitleLogo[0].ey) {
		//�e��X�e�[�W�C���[�W�摜�ƃ^�C�g���w�i�̉摜��\��
		for (int i = 1; i < OPENING_TITLE_LOGO_KIND_OF_IMAGE; i++) { g_OpTitleLogo[i].flag = true; }
	}

	//�^�C�g���̔w��̐}�`
	//������\������}�`
	if (g_OpTitleLogo[6].flag) g_OpTitleLogo[6].w += OPENING_TITLE_LOGO_FIGURE_DRAWING_SPEED;
	if (g_OpTitleLogo[6].w > OPENING_IMAGE_TITLE_LOGO_WIDTH) g_OpTitleLogo[6].w = OPENING_IMAGE_TITLE_LOGO_WIDTH;
	//�E����\������}�`�ɏd�˂钷���`
	if (g_OpTitleLogo[5].flag) g_OpTitleLogo[5].w -= OPENING_TITLE_LOGO_FIGURE_DRAWING_SPEED;
	if (g_OpTitleLogo[5].w <= 0) g_OpTitleLogo[5].w = 0;


	//�� �e��C���[�W�摜�̈ړ��ʂ��X�V
	for (int i = 1; i < (1 + OPENING_TITLE_STAGE_IMAGING_KIND_OF_IMAGE); i++) {
		//�ړ��ʂ̉��Z
		if (g_OpTitleLogo[i].flag && g_OpTitleLogo[i].x > g_OpTitleLogo[i].ex) g_OpTitleLogo[i].x += g_OpTitleLogo[i].mx;
		//�ړ���
		if (g_OpTitleLogo[i].x < g_OpTitleLogo[i].ex) {
			g_OpTitleLogo[i].x = g_OpTitleLogo[i].ex;
			//�X�C�}�[�̉摜���I���n�_�ɓ��B������J�ڃt���O��ύX
			if (g_OpTitleLogo[1].x <= g_OpTitleLogo[1].ex) {
				g_OpSceneFlagNext = OPENING_SCENE_SELECT_START_FUNCTION;
				return;
			}
		}
	}

	// ���u����L�[�v�ŉ��o���΂�
	if (JustKey('Z') || JustButton(BUTTON_1)) {
		for (int i = 0; i < OPENING_TITLE_LOGO_KIND_OF_IMAGE; i++) { g_OpTitleLogo[i].flag = true; }
		for (int i = 0; i < (1 + OPENING_TITLE_STAGE_IMAGING_KIND_OF_IMAGE); i++) {
			//�\������W
			g_OpTitleLogo[i].x = g_OpTitleLogo[i].ex;	//X
			g_OpTitleLogo[i].y = g_OpTitleLogo[i].ey;	//Y
		}
		g_OpTitleLogo[6].w = OPENING_IMAGE_TITLE_LOGO_WIDTH;
		g_OpTitleLogo[5].w = 0;
		g_OpSceneFlagNext = OPENING_SCENE_SELECT_START_FUNCTION;
	}

}

/*
**************************************************************************************************
�{�^���ݒ�1�iStart , Option�j
**************************************************************************************************
*/
void	SetFirstButton() {

	//�w�i�̃T�C�Y�����킹��
	g_Opbutton[5].ch.h = (int)(((g_Opbutton[1].ch.y + g_Opbutton[1].ch.h + OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN)
		- (g_Opbutton[0].ch.y - OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN)));
	//���W
	g_Opbutton[5].ch.y = g_Opbutton[0].ch.y - OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN;
	g_Opbutton[5].ch.x = g_Opbutton[0].ch.x - OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN;
	//�{�^���摜�̕\���t���O
	//START OPTION�\��
	for (int i = 0; i < 2; i++) { g_Opbutton[i].ch.flag = true; }
	g_Opbutton[5].ch.flag = true;				//BackImage
	//�J�[�\��
	g_cursor1.flag = true;

	//���J�[�\���ړ�
	//START OPTION�I�����̏���
	//������
	if (JustKey(VK_DOWN) || JustLever(LEVER_DOWN)) {
		//�J�[�\����
		PlaySound(g_op_sound[0], 0, false);
		//�{�^���̊Ԋu����������
		g_cursor1.y += g_cursor1.my;
		//���̍��ڂ܂ŗ������̍��ڂ�
		if (g_cursor1.y > g_Opbutton[1].ch.y) g_cursor1.y = g_Opbutton[0].ch.y;
	}
	//�����
	if (JustKey(VK_UP) || JustLever(LEVER_UP)) {
		//�J�[�\����
		PlaySound(g_op_sound[0], 0, false);
		//�{�^���̊Ԋu����������
		g_cursor1.y -= g_cursor1.my;
		//��̍��ڂ܂ŗ����牺�̍��ڂ�
		if (g_cursor1.y < g_Opbutton[0].ch.y) g_cursor1.y = g_Opbutton[1].ch.y;
	}

	//���L�[����
	//����L�[�Ŋe��ʂ֑J��
	//���x���ݒ�
	if (g_cursor1.y == g_Opbutton[0].ch.y) {
		//�J�[�\���̕������ڂ̕��ɍ��킹��
		g_cursor1.w = g_Opbutton[0].ch.w;
		//����
		if (JustUpKey('Z') || JustUpButton(BUTTON_1)) {
			//���艹
			PlaySound(g_op_sound[1], 0, false);
			//START OPTION����
			for (int i = 0; i < 2; i++) { g_Opbutton[i].ch.flag = false; }
			//�J�[�\������̍��ڂ�
			g_cursor1.y = g_Opbutton[2].ch.y;
			g_cursor1.x = g_Opbutton[2].ch.x;
			//�J�ڃt���O
			g_OpSceneFlagNext = OPENING_SCENE_SELECT_LEVEL_FUNCTION;
			return;
		}
	}
	//�I�v�V������ʂ֑J��
	if (g_cursor1.y == g_Opbutton[1].ch.y) {
		//�J�[�\���̕������ڂ̕��ɍ��킹��
		g_cursor1.w = g_Opbutton[1].ch.w;
		//����
		if (JustKey('Z') || JustButton(BUTTON_1)) {
			//���艹
			PlaySound(g_op_sound[1], 0, false);
			//�I�v�V������ʂ�
			CreateWipe(SCENE_OPTION, CHANGE_SCENE, WIPE_CONTRACTION);
			return;
		}
	}

}

/*
**************************************************************************************************
�{�^���ݒ�2���x���ݒ�
**************************************************************************************************
*/
void	SetSecondButton() {

	//��Փx�f�[�^
	tagGAMELEVEL	gameLevelTbl[LEVEL_MAX] = {
		EASY_MODE		,
		NORMAL_MODE		,
		HARD_MODE		,
	};

	//�e���Փx�\��
	for (int i = 2; i < OPENING_BUTTON_KIND_OF_IMAGE; i++) { g_Opbutton[i].ch.flag = true; }

	//�w�i�̃T�C�Y�����킹��
	g_Opbutton[5].ch.h = (int)(((g_Opbutton[4].ch.y + g_Opbutton[4].ch.h + OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN)
		- (g_Opbutton[2].ch.y - OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN)));
	//���W
	g_Opbutton[5].ch.y = g_Opbutton[2].ch.y - OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN;
	g_Opbutton[5].ch.x = g_Opbutton[2].ch.x - OPENING_BUTTON_BACK_GROUND_IMAGE_MARGIN;

	//Level�I�����̏���
	//������
	if (JustKey(VK_DOWN) || JustLever(LEVER_DOWN)) {
		//�J�[�\����
		PlaySound(g_op_sound[0], 0, false);
		g_cursor1.y += g_cursor1.my;
		//���̍��ڂ̍��W�܂ŗ������ɖ߂�
		if (g_cursor1.y > g_Opbutton[4].ch.y) g_cursor1.y = g_Opbutton[2].ch.y;

	}
	//�����
	if (JustKey(VK_UP) || JustLever(LEVER_UP)) {
		//�J�[�\����
		PlaySound(g_op_sound[0], 0, false);
		g_cursor1.y -= g_cursor1.my;
		//��̍��ڂ̍��W�܂ŗ����牺�ɖ߂�
		if (g_cursor1.y < g_Opbutton[2].ch.y) g_cursor1.y = g_Opbutton[4].ch.y;

	}
	//�� ���x���ݒ�
	for (int i = 2; i < LEVEL_MAX + 2; i++) {
		//�J�[�\���ړ�
		if (g_cursor1.y == g_Opbutton[i].ch.y) {
			//�������킹��
			g_cursor1.w = g_Opbutton[i].ch.w;
			//����L�[��
			if (JustKey('Z') || JustButton(BUTTON_1)) {
				//���艹
				PlaySound(g_op_sound[1], 0, false);
				//��Փx����
				SetGameLevel(gameLevelTbl[i - 2]);
				//�X�e�[�W�I����ʂ֑J��
				CreateWipe(SCENE_SELECTSTAGE, CHANGE_SCENE, WIPE_CONTRACTION);
				return;
			}
		}
	}
	//�L�����Z����
	if (JustKey('X') || JustButton(BUTTON_2)) {
		//�L�����Z����
		PlaySound(g_op_sound[2], 0, false);
		//��Փx�ݒ����
		for (int i = 2; i < (OPENING_BUTTON_KIND_OF_IMAGE - 1); i++) { g_Opbutton[i].ch.flag = false; }
		//�J�[�\���̍��W��START�ɍ��킹��
		g_cursor1.y = g_Opbutton[0].ch.y;
		g_cursor1.x = g_Opbutton[0].ch.x;
		//�J�ڃt���O
		g_OpSceneFlagNext = OPENING_SCENE_SELECT_START_FUNCTION;
		return;
	}

}
