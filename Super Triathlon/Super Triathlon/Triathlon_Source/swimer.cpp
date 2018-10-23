//
#pragma warning ( disable : 4996 )

#include "swimer.h"
#include "gadget.h"
#include "input.h"
#include "seaenemy.h"
#include "item.h"
#include "map.h"
#include "seastage.h"
#include "play.h"
#include "sound.h"
#include "talk.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//���O���[�o���֐�
SWIMER		g_swimer = {0};
SWIMER		g_SwimerStatus[SWIMER_STATUS] = {0};
SCOREVALUE	g_swimerScore[SWIMER_SCORE] = {0};

//�R���g���[���̃t���O
bool g_ristrictControl = true;

//�摜�̗v�f������
int g_swimerImg[SWIMER_IMAGE_MAX] = {0};

//���ʉ��̗v�f������
int g_swimerSound[SWIMER_SOUND_EFFECT_MAX] = {0};

//�U�����ߎ���
int	g_AttackChargeTime = 0;

//���݂̍U���r�b�g�^�C�v������
BYTE	g_SwimerAttackBitTypeNow = SWIMER_ANIMATION_TYPE_DIRECTION_RIGHT;

//************************************************************************************************
//���������s���֐�
//************************************************************************************************
void InitSwimer(HWND hWnd){

	//����Փx�ɂ��ύX
	//��Փx�ɂ���čU���񕜑��x��ύX����
	int attackrecoverTbl[LEVEL_MAX] = {
		5 , 45 , 55 ,
	};

	//�v���C���[�̏�����
	ZeroMemory(&g_swimer, sizeof(SWIMER));
	//�X�e�[�^�X�̏�����
	ZeroMemory(&g_SwimerStatus[0], sizeof(SWIMER) * SWIMER_STATUS);
	//�X�R�A�̏�����
	ZeroMemory(&g_swimerScore[0], sizeof(SCOREVALUE) * SWIMER_SCORE);
	//���쐧��
	g_ristrictControl = true;
	
	//���摜�̑S�X�e�[�W���ʂ̏����ݒ�
	//�v���C���[�i�X�C�}�[�j�̉摜��ǂݍ���
	g_swimerImg[0] = CreateDDImage("Image\\Sea\\swimer.bmp" , hWnd);	
	//�U�����[�^�[
	g_swimerImg[1] = CreateDDImage("Image\\Sea\\attack.bmp" , hWnd);


	//�����ʉ�
	//�X�s�[�h�`���[�W�㏸��
	g_swimerSound[0] = CreateSound("SE\\speedcharge.wav");
	//�X�s�[�h�`���[�W������
	g_swimerSound[1] = CreateSound("SE\\chargedown.wav");


	//���X�e�[�^�X���
	//�G������		
	g_swimer.defeat_cnt = 0;							//������
	g_swimer.chain_max = 0;								//CHAIN�ő吔
	g_swimerScore[0].score_x = 450.0f;					//�\����X
	g_swimerScore[0].score_y = 50.0f;					//�\����Y
	g_swimerScore[0].score_scale = 0.5f;				//�g�k��
	g_swimerScore[0].dicimals_flag = false;				//�����\���t���O
	g_swimerScore[0].ch.type = SCORE_NUMBER_TYPE_NORMAL;	//�X�R�A�摜�̎��
	g_swimerScore[0].scoreColor = SCORE_YELOW;			//�X�R�A�̐F
	g_swimerScore[0].digit = 3;							//����
	//�G��|�������̕\��
	g_swimerScore[0].ch.flag = true;


	//�U�����[�^�[
	//�P��
	g_SwimerStatus[0].ch.x = 50;			//�\����X
	g_SwimerStatus[0].ch.y = 30;			//�\����Y
	g_SwimerStatus[0].ch.h = 64;			//�\������
	g_SwimerStatus[0].ch.w = 64;			//�\����
	g_SwimerStatus[0].ch.scale = 0.5f;		//�g�k�{��
	g_SwimerStatus[0].able_attack = true;	//�U���\�t���O
	g_SwimerStatus[0].attack_charge = true;//�U���`���[�W�t���O
	g_AttackChargeTime = attackrecoverTbl[GetGameLevel()];//�U�����ߎ���
	
	//2��
	g_SwimerStatus[1].ch.x = g_SwimerStatus[0].ch.x + 
		g_SwimerStatus[0].ch.w * g_SwimerStatus[0].ch.scale;	//�\����X
	g_SwimerStatus[1].ch.y = g_SwimerStatus[0].ch.y;			//�\����Y
	g_SwimerStatus[1].ch.h = 64;								//�\������
	g_SwimerStatus[1].ch.w = 64;								//�\����
	g_SwimerStatus[1].ch.scale = 0.1f;							//�g�k�{��
	g_SwimerStatus[1].able_attack = false;						//�U���\�t���O
	
	//3��
	g_SwimerStatus[2].ch.x = g_SwimerStatus[1].ch.x + 
		g_SwimerStatus[0].ch.w * g_SwimerStatus[0].ch.scale;	//�\����X
	g_SwimerStatus[2].ch.y = g_SwimerStatus[0].ch.y;			//�\����Y
	g_SwimerStatus[2].ch.h = 64;								//�\������
	g_SwimerStatus[2].ch.w = 64;								//�\����
	g_SwimerStatus[2].ch.scale = 0.1f;							//�g�k�{��
	g_SwimerStatus[2].able_attack = false;						//�U���\�t���O



	//���v���C���[�u�X�C�}�[�v�̏����ݒ�
	g_swimer.ch.x = 200.0f;								//�\������w���W
	g_swimer.ch.y = 240.0f;								//�\������x���W
	g_swimer.ch.w = SWIMER_DEFAULT_IMAGE_WIDTH;			//�\������摜�̕�
	g_swimer.ch.h = SWIMER_DEFAULT_IMAGE_HEIGHT;		//�\������摜�̍���
	g_swimer.ch.picx = 0;								//�\����X
	g_swimer.ch.picy = 0;								//�\����Y
	g_swimer.ch.scale = SWIMER_DEFAULT_SCALE;			//�g�k�̔{��
	g_swimer.ch.speed = SWIMER_DEFAULT_SPEED_RATE;		//���x�{��
	g_swimer.ch.defaultSpeed = SWIMER_DEFAULT_SPEED_RATE;//���x�{�������l
	g_swimer.ch.mx = SWIMER_DEFAULT_SPEEDX_L;			//�ړ��ʂw
	g_swimer.ch.default_mx = SWIMER_DEFAULT_SPEEDX_L;	//�ړ��ʂ̏����ݒ�
	g_swimer.ch.my = SWIMER_DEFAULT_SPEEDY;				//�ړ���Y
	g_swimer.ch.default_my = g_swimer.ch.my;			//�ړ���Y�����l
	g_swimer.speedmax = SWIMER_DEFAULT_SPEEDMAX_RATE;	//�ő�����{��
	g_swimer.speedmin = SWIMER_DEFAULT_SPEEDMIN_RATE;	//�ŏ������{��
	g_swimer.ch.flag = true;							//�����t���O
	g_swimer.attack = false;							//�U�����t���O
	g_swimer.avoid_time = SWIMER_DEFAULT_AVOID_TIME;	//�G�ɓ��������ۂ̉������
	g_swimer.speedlevel = SWIMER_SPEED_NORMAL;			//�v���C���[�̑��x���x��
	g_swimer.attack_type = SWIMER_ATTACK_NORMAL;		//�U���̃^�C�v
	g_swimer.ch.direction = D_RIGHT;					//����
	g_swimer.ch.direction_now = g_swimer.ch.direction;	//���݂̕���
	g_swimer.itemEffectType = SWIMER_ITEM_EFFECT_TYPE_VACANT;	//�A�C�e�����ʂ̃^�C�v

	//�ʒu�ݒ�
	g_swimer.pos_x = 200.0f;							//X���W
	g_swimer.pos_y = 240.0f;							//Y���W

	//���A�j���[�V�����ݒ�
	g_swimer.ani.fram = SWIMER_TYPE_NORMAL_ANIMATION_FRAM;		//�t���[����
	g_swimer.ani.max = SWIMER_TYPE_NORMAL_ANIMATION_MAX;		//�ő�R�}��
	g_swimer.ani.type = SWIMER_ANIMATION_TYPE_DIRECTION_RIGHT;	//�j���A�j���[�V����
	
	//�v���C���[�̃A�j���[�V��������ݒ�
	SetAnimation(&g_swimer.ch , &g_swimer.ani ,
				g_swimer.ani.sx , g_swimer.ani.sy ,
				g_swimer.ani.max , g_swimer.ani.fram , SWIMER_ANIMATION_TYPE_DIRECTION_RIGHT, true);
	
}

//************************************************************************************************
//�X�V���s���֐�
//************************************************************************************************
void UpdateSwimer(){

	//�A�C�e�������擾
	ITEM* pi = GetItem();
	
	//�X�e�[�W�̏����擾
	STAGE* pst = GetSeaStage();
	
	//�G��|���������X�V����
	g_swimerScore[0].num = (float)g_swimer.defeat_cnt;

	//�� �c�X�N���[�����̕ύX
	if( pst->scrolly ){
		
		//�ړ��ʂ��}�C�i�X�̒l��
		g_swimer.ch.mx = SWIMER_DEFAULT_SPEEDX_R;

		//���݂̈ړ��ʂ�ύX
		g_swimer.ch.default_mx = g_swimer.ch.mx;
		
		//�v���C���[�_��
		g_swimer.ch.flag = false;
		
		//����s�\
		g_ristrictControl = false;
		
		//�G��|�����X�R�A���\��
		g_swimerScore[0].ch.flag = false;
	
	}

	//�� �E�X�N���[�����̕ύX
	if( pst->scroll_r && g_swimer.ch.direction == D_RIGHT ){
	
		//����\
		g_ristrictControl = true;
		
		//�J�����ʒu�����ֈړ�������i�v���C���[�͉�ʉE�Ɂj
		g_swimer.pos_x = 500.0f;
		
		//����������
		g_swimer.ch.direction = D_LEFT;
		
		//���݂̕������擾
		g_swimer.ch.direction_now = g_swimer.ch.direction;		

		//�G��|�����X�R�A��\��
		g_swimerScore[0].ch.flag = true;
	
	}

	//�� �ړ��̍X�V
	UpdateSwimerMove();
	
	//�� �U���Q�[�W�̉�
	SwimerChargeAttack();

	//�� �Ή������L�[���͂̋���
	//���쐧��t���O
	if( g_ristrictControl ){
				
		//�� �U���̍X�V
		UpdateSwimerAttack();
		
		//���@���x�̍X�V
		UpdateSwimerSpeed();

		//�� �A�C�e���̎g�p
		//�A�C�e���擾�ς݂Ȃ�AC�{�^���ŃA�C�e���g�p
		if( JustButton(BUTTON_3) || JustKey('C') && g_swimer.attack == false ) UseItem();

	}

	//�� �����t���O�Ȃ�G�Ƃ̓����蔻��
	if( g_swimer.ch.flag )	CollisionEnemy( &g_swimer.ch );

	//�� �ʏ�̃A�j���[�V�����X�V
	ChangeSwimerNormalAnimation();

	//�� �G����莞�ԉ������i�_���[�W���̏����j
	AvoidEnemy(&g_swimer.ch , g_swimer.avoid_time);

	//�� �A�C�e���Ƃ̓����蔻��
	CollisionItem( &g_swimer );

	//�� �J�����̍��W���v���C���[�����S���A��⍶�̈ʒu�Ɏw�肷��
	SetCamera(g_swimer.ch.x - g_swimer.pos_x + g_swimer.ch.w ,
		g_swimer.ch.y - g_swimer.pos_y);

	//�� �ړ��͈͂𐧌�����
	StrictSwimerMove();

}

//************************************************************************************************
//�\�����s���֐�
//************************************************************************************************
void DrawSwimer() {

	//�J�����̍��W���擾
	float px = GetCameraX();
	float py = GetCameraY();

	//�G�ƂԂ����Ă�����_�ł�����
	if (g_swimer.ch.time % 2 == 0) {

		//�L�����N�^�[�̉摜
		DDScaleBlt(
			g_swimer.ch.x - px,					//X���W
			g_swimer.ch.y - py,					//Y���W
			g_swimer.ch.w,							//��
			g_swimer.ch.h,							//����
			g_swimerImg[0],						//�Ή������摜�ԍ�
			g_swimer.ch.picx,						//�\�����w���W
			g_swimer.ch.picy,						//�\�����x���W
			g_swimer.ch.scale);					//�g�k�{��
	}

	//�U�����[�^�[�̉摜
	for (int i = 0; i < SWIMER_ATTACK_MAX; i++) {
		DDScaleBlt(
			g_SwimerStatus[i].ch.x,				//X���W
			g_SwimerStatus[i].ch.y,				//Y���W
			g_SwimerStatus[i].ch.w,				//��
			g_SwimerStatus[i].ch.h,				//����
			g_swimerImg[1],						//�Ή������摜�ԍ�
			g_SwimerStatus[i].ch.picx,			//�\�����w���W
			g_SwimerStatus[i].ch.picy,			//�\�����x���W
			g_SwimerStatus[i].ch.scale);			//�g�k�{��
	}

	//�G��|��������\������
	if (g_swimerScore[0].ch.flag) DrawScore(&g_swimerScore[0]);

	//�]���摜�̕\��
	DrawValueation(g_swimerScore[0].score_x +
		(SCORE_NUMBER_AND_PUNCTUAL_IMAGE_WIDTH * g_swimerScore[0].score_scale),//X���W 
			g_swimerScore[0].score_y,											//Y���W
			g_swimer.defeat_cnt);												//�G������
	
}

//************************************************************************************************
//������s���֐�
//************************************************************************************************
void ReleaseSwimer(){

	//�g�p���������������[�v�ŉ񂵂ĉ�����s��
	for (int i = 0; i < SWIMER_IMAGE_MAX; i++) {
		ReleaseDDImage(g_swimerImg[i]);
	}
	//���ʉ�
	for (int i = 0; i < SWIMER_SOUND_EFFECT_MAX; i++) {
		ReleaseSound(g_swimerSound[i]);
	}
}

//************************************************************************************************
//�A�j���[�V�������X�V(�ʏ�T�C�Y���̍X�V)
//************************************************************************************************
void ChangeSwimerNormalAnimation(){

	//�摜�f�[�^�e�[�u���iY���W�ŕω��j
	BYTE	 animationTbl[ SWIMER_ANIMATION_KIND_OF_TYPE ] = {

		//�� �A�j���[�V��������̃r�b�g��
		SWIMER_ANIMATION_TYPE_DIRECTION_RIGHT			 ,					//�E����
		SWIMER_ANIMATION_TYPE_DIRECTION_LEFT			 ,					//������
		
		SWIMER_ANIMATION_BIT_TYPE_NORMAL_ATTACK		 ,						//�o�^�t���C��
		SWIMER_ANIMATION_TYPE_D_LEFT_NORMAL_ATTACK		 ,					//�o�^�t���C

		SWIMER_ANIMATION_TYPE_D_RIGHT_ABSOLUTE		 ,						//���G��
		SWIMER_ANIMATION_TYPE_D_LEFT_ABSOLUTE		 ,						//���G��
		SWIMER_ANIMATION_TYPE_D_RIGHT_NORMAL_ATTACK_ABSOLUTE	 ,			//���G��
		SWIMER_ANIMATION_TYPE_D_LEFT_NORMAL_ATTACK_ABSOLUTE		 ,			//���G��
		
		SWIMER_ANIMATION_TYPE_D_RIGHT_SPEED_UP		 ,						//���x�㏸��
		SWIMER_ANIMATION_TYPE_D_LEFT_SPEED_UP		 ,						//���x�㏸��
		SWIMER_ANIMATION_TYPE_D_RIGHT_NORMAL_ATTACK_SPEED_UP	 ,			//���x�㏸��
		SWIMER_ANIMATION_TYPE_D_LEFT_NORMAL_ATTACK_SPEED_UP		 ,			//���x�㏸��

		SWIMER_ANIMATION_TYPE_D_RIGHT_SPEED_DOWN	 ,						//���x������
		SWIMER_ANIMATION_TYPE_D_LEFT_SPEED_DOWN		 ,						//���x������
		SWIMER_ANIMATION_TYPE_D_RIGHT_NORMAL_ATTACK_SPEED_DOWN	 ,			//���x������
		SWIMER_ANIMATION_TYPE_D_LEFT_NORMAL_ATTACK_SPEED_DOWN	 ,			//���x������

	};

	//���݂̃A�j���[�V�����̃^�C�v�ԍ��ɉ����ĉ摜��ω�������
	for( int i = 0 ; i < SWIMER_ANIMATION_KIND_OF_TYPE ; i++ ){

		//�A�j���[�V�����ɉ����ĉ摜����
		if( animationTbl[i] == g_swimer.ani.type ){

			//�\����Y���W�𔽉f����
			g_swimer.ani.sy = ( i * g_swimer.ch.h );

			//�� �����҂̃A�j���[�V�����X�V
			if( UpdateAnimation( &g_swimer.ch , &g_swimer.ani ) == false ){

				//���݂̍U���A�j���[�V�����̃r�b�g�ڂ�0�ɂ���
				g_swimer.ani.type &= ~g_SwimerAttackBitTypeNow;		
				//�U���^�C�v��ʏ�ɖ߂�
				g_swimer.attack_type = SWIMER_ATTACK_NORMAL;	
				//���݂̍U���r�b�g�ڂ�ʏ��
				g_SwimerAttackBitTypeNow = SWIMER_ANIMATION_TYPE_DIRECTION_RIGHT;
				//�U���t���O��false��
				g_swimer.attack = false;

			}
			break;
		}

	}
}


//************************************************************************************************
//�A�j���[�V�������X�V(�U���T�C�Y���̍X�V)
//************************************************************************************************
void	ChangeSwimerAttackAnimation() {

	//�g�p����A�j���[�V��������
	bool(*lpAnimation[2])(CHARACTER*, ANIMATION*) = {
		UpdateAnimation,				//��]�U����
		UpdateReverseAnimation,			//�ːi�U����
	};

	//�摜�f�[�^�e�[�u���iY���W�ŕω��j
	BYTE	 animationTbl[SWIMER_ANIMATION_KIND_OF_TYPE] = {

		//�E����
		SWIMER_ANIMATION_TYPE_D_RIGHT_CIRCLE_ATTACK		 ,		//��]��
		SWIMER_ANIMATION_TYPE_D_LEFT_CIRCLE_ATTACK		 ,		//��]

		SWIMER_ANIMATION_TYPE_D_RIGHT_CIRCLE_ATTACK_ABSOLUTE	 ,			//���G��
		SWIMER_ANIMATION_TYPE_D_LEFT_CIRCLE_ATTACK_ABSOLUTE		 ,			//���G��
		SWIMER_ANIMATION_TYPE_D_RIGHT_CIRCLE_ATTACK_SPEED_UP	 ,			//���x�㏸��
		SWIMER_ANIMATION_TYPE_D_LEFT_CIRCLE_ATTACK_SPEED_UP		 ,			//���x�㏸��
		SWIMER_ANIMATION_TYPE_D_RIGHT_CIRCLE_ATTACK_SPEED_DOWN	 ,			//���x������
		SWIMER_ANIMATION_TYPE_D_LEFT_CIRCLE_ATTACK_SPEED_DOWN	 ,			//���x������
		//�ːi
		SWIMER_ANIMATION_TYPE_D_RIGHT_BUMP_ATTACK		 ,		//�ːi��
		SWIMER_ANIMATION_TYPE_D_LEFT_BUMP_ATTACK		 ,		//�ːi

		SWIMER_ANIMATION_TYPE_D_RIGHT_BUMP_ATTACK_ABSOLUTE		 ,			//���G��
		SWIMER_ANIMATION_TYPE_D_LEFT_BUMP_ATTACK_ABSOLUTE		 ,			//���G��
		SWIMER_ANIMATION_TYPE_D_RIGHT_BUMP_ATTACK_SPEED_UP		 ,			//���x�㏸��
		SWIMER_ANIMATION_TYPE_D_LEFT_BUMP_ATTACK_SPEED_UP		 ,			//���x�㏸��
		SWIMER_ANIMATION_TYPE_D_RIGHT_BUMP_ATTACK_SPEED_DOWN	 ,			//���x������
		SWIMER_ANIMATION_TYPE_D_LEFT_BUMP_ATTACK_SPEED_DOWN		 ,			//���x������

	};

	//�摜�\����Y
	int	picY = 0;
	//���[�v�̎n�߂̗v�f
	int start = 0;
	//�A�j���[�V�����X�V�֐��̕���p
	int diverge;
	//���݂̍U���^�C�v�ɉ����ĕ\�����x���W��ύX����
	//��]�U��
	if (g_SwimerAttackBitTypeNow == SWIMER_ANIMATION_TYPE_D_RIGHT_CIRCLE_ATTACK) {
		start = 0;
		picY = SWIMER_ANIMATION_TYPE_CIRCLE_ATTACK_PIC_Y;
	}
	//�ːi�U��
	if (g_SwimerAttackBitTypeNow == SWIMER_ANIMATION_TYPE_D_RIGHT_BUMP_ATTACK) {
		start = 8;
		picY = SWIMER_ANIMATION_TYPE_BUMP_ATTACK_PIC_Y;
	}

	//���݂̃A�j���[�V�����̃^�C�v�ԍ��ɉ����ĉ摜��ω�������
	for (int i = start; i < SWIMER_ANIMATION_KIND_OF_TYPE; i++) {
		//�A�j���[�V�����ɉ����ĉ摜����
		if (animationTbl[i] == g_swimer.ani.type) {
			//�A�j���[�V�����X�V����p�̗v�f��
			if (start < 8) { diverge = 0; }	//��]
			else {
				diverge = 1;				//�ːi
			}
			//�\����Y���W�𔽉f����
			g_swimer.ani.sy = picY + ((i - start) * g_swimer.ch.h);
			//�� �����҂̃A�j���[�V�����X�V
			if (lpAnimation[diverge](&g_swimer.ch, &g_swimer.ani) == false) {
				//���݂̍U���A�j���[�V�����̃r�b�g�ڂ�0�ɂ���
				g_swimer.ani.type &= ~g_SwimerAttackBitTypeNow;
				//���݂̍U���r�b�g�ڂ�ʏ��
				g_SwimerAttackBitTypeNow = SWIMER_ANIMATION_TYPE_DIRECTION_RIGHT;
				//�t���[�����������l��
				g_swimer.ani.fram = SWIMER_TYPE_NORMAL_ANIMATION_FRAM;
				//�ő�R�}���������l��
				g_swimer.ani.max = SWIMER_TYPE_NORMAL_ANIMATION_MAX;
				//�U���^�C�v��ʏ�ɖ߂�
				g_swimer.attack_type = SWIMER_ATTACK_NORMAL;
				//�\�����鍂����ύX
				g_swimer.ch.h = SWIMER_DEFAULT_IMAGE_HEIGHT;
				//�U���t���O��false��
				g_swimer.attack = false;
				//���x�{�����ō��{���ɂ���
				g_swimer.ch.speed = SWIMER_DEFAULT_SPEEDMAX_RATE;
			}
			break;
		}
	}
}


//*************************************************************************************************
//�X�C�}�[�̈ړ����X�V����
//*************************************************************************************************
void UpdateSwimerMove(){

	//�u���v�̓��́u�������x��5.0�v
	if(KeepLever(LEVER_UP) || KeepKey(VK_UP)) g_swimer.ch.y -= g_swimer.ch.my;
	//�u���v�̓���
	if(KeepLever(LEVER_DOWN) || KeepKey(VK_DOWN)) g_swimer.ch.y += g_swimer.ch.my;
}

//*************************************************************************************************
//�X�C�}�[�̑��x���X�V����
//*************************************************************************************************
void UpdateSwimerSpeed(){

	//���x�ɏ������^�C�v����
	tagSWIMERANIMATIONSPEED speedType[4] = {

		SWIMER_SPEED_SLOW ,			//�x��
		SWIMER_SPEED_NORMAL ,		//����
		SWIMER_SPEED_DECENT ,		//��������
		SWIMER_SPEED_RAPID ,		//����	
	
	};
	//�v���C���[�̕����ɉ��������͂���L�[�ύX�e�[�u��
	BYTE	inputSpeedUpTbl[2] = { VK_RIGHT , VK_LEFT };		//���x�㏸
	BYTE	inputSpeedDownTbl[2] = { VK_LEFT , VK_RIGHT };		//���x����

	//�� ���x
	//�a�{�^�����A�Eor���L�[�ŉ����i�����ɉ����ĕω��j
	if(KeepButton(BUTTON_2) || KeepKey(inputSpeedUpTbl[g_swimer.ch.direction_now])){
		//���x�{���㏸
		g_swimer.charge++;
		//���x���㏸���Ȃ特��炷
		if(g_swimer.ch.speed < g_swimer.speedmax && g_swimer.charge % 50 == 0){
			//���ʉ�
			PlaySound(g_swimerSound[0] , 0 , false);
		}
		//���x�ɔ{�������Z
		g_swimer.ch.speed += 0.0001f * g_swimer.charge;
	}

	//B�{�^�����A�Eor���L�[�������ꂽ��`���[�W�̒l�����Z�b�g
	if(JustUpButton(BUTTON_2) || JustUpKey(inputSpeedUpTbl[g_swimer.ch.direction_now])){
		//���x�{�������Z�b�g
		g_swimer.charge = 0;
	}
	//Y�{�^�����A�Eor���L�[��������Ă����珙�X�Ɍ���
	if(KeepButton(BUTTON_4) || KeepKey(inputSpeedDownTbl[g_swimer.ch.direction_now])){
		//���x�{������
		g_swimer.charge++;
		//�������Ȃ特��炷
		if(g_swimer.ch.speed > g_swimer.speedmin && g_swimer.charge % 50 == 0){
			//���ʉ�
			PlaySound(g_swimerSound[1] , 0 , false);
		}
		//���x�ɔ{���̒l�����Z
		g_swimer.ch.speed -= 0.0002f * g_swimer.charge;
	}
	//Y�{�^�����A�Eor���L�[�������ꂽ��`���[�W�̒l�����Z�b�g
	if(JustUpButton(BUTTON_4) || JustUpKey(inputSpeedDownTbl[g_swimer.ch.direction_now])){
		//���x�{�������Z�b�g
		g_swimer.charge = 0;
	}
	//�{���̏���l��999�܂�
	g_swimer.charge %= 1000;

	//���x�̏��
	if(g_swimer.ch.speed >= g_swimer.speedmax) g_swimer.ch.speed = g_swimer.speedmax;
	//���x�̉���
	if(g_swimer.ch.speed <= g_swimer.speedmin) g_swimer.ch.speed = g_swimer.speedmin;

	//�����x���x�����X�V�i�A�j���[�V�����ω��j
	//���������璲�ׂČ��ݑ��x���������烋�[�v�𔲂��āA�A�j���[�V�������X�V
	for( int i = 3 ; i >= 0 ; i-- ){
		
		//���݂̑��x�����̒l�������Ă�����
		if(fabs( g_swimer.ch.mx ) * g_swimer.ch.speed > i * 2){
			
			//���x���x���̃}�N������
			g_swimer.speedlevel = speedType[i];
			break;
		}
	}

	//�U��������Ȃ������甽�f������
	if( g_swimer.attack == false ){

		//�v���C���[�̃A�j���[�V�������x��ω�
		g_swimer.ani.fram = g_swimer.speedlevel;
	}

	//�ړ��ʂ����W�ɉ��Z���Ĕ��f������
	//X�ړ��͐����t���O�������Ă��鎞�����s��
	if( g_swimer.ch.flag ){
		
		//�ړ���*�����x�����W�ɉ��Z
		g_swimer.ch.x += g_swimer.ch.mx * g_swimer.ch.speed;
	}	

	//�L�����N�^�[�̈ړ��ʂ𑬓x���[�^�[�ɔ��f������
	SetStatus( g_swimer.ch.mx * g_swimer.ch.speed );					//X�ړ�

}

//*************************************************************************************************
//�X�C�}�[�̍U�����X�V����
//*************************************************************************************************
void UpdateSwimerAttack(){

	//�U���̕���_
	//�ʏ�@���@�o�^�t���C�@���@��]�@���@�ːi�ƕω������Ă���
	tagSWIMERATTACKTYPE actiontype[SWIMER_ATTACK_TYPE_MAX] = {
		
		SWIMER_ATTACK_NORMAL ,			//�ʏ�̏��
		SWIMER_ATTACK_WAVE_ATTACK ,		//�o�^�t���C�̏��
		SWIMER_ATTACK_CIRCLE_ATTACK ,	//��]�̏��
		SWIMER_ATTACK_BUMP_ATTACK ,		//�ːi�̏��
	
	};

	//�U���^�C�v�̏������̕���
	void (*pAttackInit[3])() = {

		InitSwimerActionType0 ,		//�o�^�t���C
		InitSwimerActionType1 ,		//��]
		InitSwimerActionType2 ,		//�ːi
	
	};
	
	//�U���^�C�v�̕���
	void (*pAttackType[3])() = {
		
		SwimerActionType0 ,			//�o�^�t���C
		SwimerActionType1 ,			//��]
		SwimerActionType2 ,			//�ːi
	
	};

	//A�{�^�����AA�L�[�ōU��
	if( JustButton(BUTTON_1) || JustKey('A') && g_swimer.itemEffectType != SWIMER_ITEM_EFFECT_TYPE_ABSOLUTE ){
		//�U���\�Ȃ���s
		if( SwimerGoAttack() ){
			
			//�U���^�C�v�̃��[�v
			for( int j = 0 ; j <= SWIMER_ATTACK_TYPE_MAX ; j++ ){
				
				//�U���^�C�v���x�W�F�^�C�v�Ȃ���������break
				if( g_swimer.attack_type == SWIMER_ATTACK_BUMP_ATTACK ) break;

				//���݂̃^�C�v�Ɣ�r���ĈقȂ鎟�̗v�f��
				if( g_swimer.attack_type != actiontype[j] )	continue;
		
				//���݂̍U���^�C�v�ɉ����čU���p�^�[����ω�������
				if( g_swimer.attack_type == actiontype[j] ){
					
					//�h��������v�f���擾
					g_swimer.attack_type = actiontype[j] + 1;
					
					//�U������̏�����
					pAttackInit[j]();
					break;
				}
			}
		}
	}

	//�� �U���̕���
	//�U���t���O��true�Ȃ番��
	if( g_swimer.attack ) pAttackType[g_swimer.attack_type]();

}


//**************************************************************************************************
//�X�C�}�[�̈ړ��͈͂𐧌�����
//**************************************************************************************************
void StrictSwimerMove(){

	
	//�E��ʂ���͂ݏo�Ȃ��悤�ɐ���
	if(g_swimer.ch.x > GetCameraX() + 640 - ((float)g_swimer.ch.w * g_swimer.ch.scale)){
		
		//�E��ʒ[�ɂ҂�����������W	
		g_swimer.ch.x = GetCameraX() + 640 - ((float)g_swimer.ch.w * g_swimer.ch.scale);
	}

	//����ʂ���A�݂͂łȂ��悤�ɐ���
	if(g_swimer.ch.y > (float)GetCameraY() + 480 - ((float)g_swimer.ch.h * g_swimer.ch.scale)){
		
		//����ʒ[�ɂ҂�����������W
		g_swimer.ch.y = (float)GetCameraY() + 480 - ((float)g_swimer.ch.h * g_swimer.ch.scale);
	}

	//����ʂ���͂ݏo�Ȃ��悤�ɐ���
	if(g_swimer.ch.x < (float)GetCameraX()){

		//����ʒ[�ɂ҂����肭���������W
		g_swimer.ch.x = (float)GetCameraX();
	}

	//���ʂ���A�݂͂łȂ��悤�ɐ���
	if(g_swimer.ch.y < (float)GetCameraY()){
	
		//���ʒ[�ɂ҂����肭���������W
		g_swimer.ch.y = (float)GetCameraY();
	}


}



//************************************************************************************************
//�v���C���[�̃|�C���^��Ԃ��֐�
//************************************************************************************************
SWIMER* GetSwimer(){ return &g_swimer; }

//************************************************************************************************
//�v���C���[�̕t�����ʂ�����
//************************************************************************************************
void	OrdinarySwimer(){

	//�v���C���[�u�X�C�}�[�v�̏����ݒ�
	g_swimer.ch.w = SWIMER_DEFAULT_IMAGE_WIDTH;			//�\������摜�̕�
	g_swimer.ch.h = SWIMER_DEFAULT_IMAGE_HEIGHT;		//�\������摜�̍���
	g_swimer.ch.speed = SWIMER_DEFAULT_SPEED_RATE;		//���x�{��
	g_swimer.ch.mx = g_swimer.ch.default_mx;			//�������x
	g_swimer.ch.my = g_swimer.ch.default_my;			//�������x
	g_swimer.ch.time = 0;								//����
	g_swimer.avoid_time = SWIMER_DEFAULT_AVOID_TIME;	//�G�ɓ��������ۂ̉������
	g_swimer.speedmax = SWIMER_DEFAULT_SPEEDMAX_RATE;	//�ō����x�{��
	g_swimer.speedmin = SWIMER_DEFAULT_SPEEDMIN_RATE;	//�Œᑬ�x�{��
	g_swimer.ch.scale = SWIMER_DEFAULT_SCALE;			//�g�k�{��
	g_swimer.attack_type = SWIMER_ATTACK_NORMAL;		//�U���^�C�v
	g_swimer.attack = false;							//�U�����t���O
	g_swimer.itemEffectType = SWIMER_ITEM_EFFECT_TYPE_VACANT;//���݂̃A�C�e���̌��ʔԍ�

	//�v���C���[�u�X�C�}�[�v�̃A�j���[�V�����ݒ�
	g_swimer.speedlevel = SWIMER_SPEED_NORMAL;					//�t���[����
	g_swimer.ch.direction = g_swimer.ch.direction_now;			//�摜��ʏ�֕ύX
	g_swimer.ani.type = SWIMER_ANIMATION_TYPE_DIRECTION_RIGHT;	//�j���A�j���[�V����

	//�U���ύX�p�A�j���[�V�����^�C�v���擾
	g_swimer.ani.fram = SWIMER_TYPE_NORMAL_ANIMATION_FRAM;		//�t���[����
	g_swimer.ani.max = SWIMER_TYPE_NORMAL_ANIMATION_MAX;		//�ő�R�}��

}


//**********************************************************************************************************
//�U���Q�[�W������
//**********************************************************************************************************
void	SwimerChargeAttack(){


	//�e�U���Q�[�W���U���s��ԂȂ�A�`���[�W����
	for( int i = 0 ; i < SWIMER_ATTACK_MAX ; i++ ){
		if( g_SwimerStatus[i].able_attack ) continue;
		g_SwimerStatus[i].attack_charge = true; 
		break;
	}

	//�U���`���[�W���Ȃ�A�Ⴂ�v�f���珇�Ƀ`���[�W���Ă���
	for( int i = 0 ; i < SWIMER_ATTACK_MAX ; i++ ){
		//���ɍU���\�Ȃ玟�̗v�f��
		if( g_SwimerStatus[i].able_attack ) continue;
		//���߃t���O���Ȃ�
		if( g_SwimerStatus[i].attack_charge ){
			
			//���Ԃ��v��
			g_SwimerStatus[i].ch.time ++;
						
			//��莞�Ԍo�߂ŉ摜���g�傷��
			if( g_SwimerStatus[i].ch.time > g_AttackChargeTime ){
				
				//�o�ߎ��Ԃ����Z�b�g
				g_SwimerStatus[i].ch.time = 0;
				
				//�摜���g��
				g_SwimerStatus[i].ch.scale += 0.1f;
				
				//�g�k�������̒l�܂ŒB��������U���\�ɂ��A���߃t���O��false��
				if( g_SwimerStatus[i].ch.scale >= 0.5f ){
					g_SwimerStatus[i].ch.scale = 0.5f;
					g_SwimerStatus[i].able_attack = true;
					g_SwimerStatus[i].attack_charge = false;
					//���̗v�f��
					continue;
				}
			//1�x�ɉ񕜂���v�f�͂P��
			break;
				
			}
		}
	}

}


/*
***************************************************************************************************
�U���Q�[�W������čU�����s��
***************************************************************************************************
*/
bool	SwimerGoAttack(){

	bool Go = false;

	//�ő�U���񐔂̗v�f���������[�v����
	//�Ⴂ�v�f����U���Q�[�W������Ă���
	for( int i = ( SWIMER_ATTACK_MAX - 1 ) ; i >= 0 ; i-- ){
		
		//�U���\�t���O�������Ă��Ȃ������玟�̗v�f��
		if( g_SwimerStatus[i].able_attack == false ) continue;

		//�U���\�t���O��false��
		g_SwimerStatus[i].able_attack = false;

		//�U���t���O�̉摜���k��
		g_SwimerStatus[i].ch.scale = 0.1f;

		Go = true;
		break;
	}
	return Go;

}
//**********************************************************************************************************
//�v���C���[�A�N�V�����^�C�v�O�ԁu�o�^�t���C�̓����v�̏�����
//**********************************************************************************************************
void InitSwimerActionType0(){

	//�R�}�������Z�b�g
	g_swimer.ani.cnt = 0;
	//�o�ߎ���
	g_swimer.ani.time = 0;
	g_swimer.attack = true;												//�U���t���O
	
	//�A�j���[�V�����ݒ�
	g_swimer.ani.type |= SWIMER_ANIMATION_BIT_TYPE_NORMAL_ATTACK;		//�^�C�v���U����
	g_swimer.ani.fram = SWIMER_TYPE_NORMAL_ANIMATION_FRAM;				//�t���[����
	g_swimer.ani.max = SWIMER_TYPE_NORMAL_ANIMATION_MAX;				//�ő�R�}��
	//���݂̍U���r�b�g�ڂ��擾
	g_SwimerAttackBitTypeNow |= SWIMER_ANIMATION_BIT_TYPE_NORMAL_ATTACK;

}

//**********************************************************************************************************
//�v���C���[�A�N�V�����^�C�v�P�ԁu��]�̓����v�̏�����
//**********************************************************************************************************
void InitSwimerActionType1(){

	//�R�}�������Z�b�g
	g_swimer.ani.cnt = 0;
	//�p�x���Z�b�g
	g_swimer.ch.cnt = 0;
	//�o�ߎ���
	g_swimer.ani.time = 0;
	g_swimer.attack = true;							//�U���t���O
	g_swimer.ch.h = SWIMER_ATTACK_IMAGE_HEIGHT;		//�\�����鍂����ύX
	//�A�j���[�V�����ݒ�
	g_swimer.ani.type |= SWIMER_ANIMATION_BIT_TYPE_CIRCLE_ATTACK;		//�^�C�v����]�U����
	g_swimer.ani.fram = SWIMER_TYPE_CIRCLE_ATTACK_ANIMATION_FRAM;		//�t���[����
	g_swimer.ani.max = SWIMER_TYPE_CIRCLE_ATTACK_ANIMATION_MAX;			//�ő�R�}��
	//���݂̍U���r�b�g�ڂ��擾
	g_SwimerAttackBitTypeNow |= SWIMER_ANIMATION_BIT_TYPE_CIRCLE_ATTACK;
	//�o�����W���擾
	g_swimer.ch.sx = g_swimer.ch.x - ((g_swimer.ch.w * g_swimer.ch.scale) / 2);				//X���W
	g_swimer.ch.sy = g_swimer.ch.y;				//Y���W

}

//**********************************************************************************************************
//�v���C���[�A�N�V�����^�C�v�Q�ԁu�x�W�F�̓����v�̏�����
//**********************************************************************************************************
void InitSwimerActionType2(){

	//�R�}�������Z�b�g
	g_swimer.ani.cnt = 0;
	//�o�ߎ���
	g_swimer.ani.time = 0;
	
	//���Ԍo�߂����Z�b�g
	g_swimer.ch.millisecond = 0;
	g_swimer.attack = true;						//�U���t���O
	
	//�o�����W���擾
	g_swimer.ch.sx = g_swimer.ch.x;				//X���W
	g_swimer.ch.sy = g_swimer.ch.y;				//Y���W
	
	g_swimer.ch.h = SWIMER_ATTACK_IMAGE_HEIGHT;		//�\�����鍂����ύX

	//�A�j���[�V�����ݒ�
	g_swimer.ani.type |= SWIMER_ANIMATION_BIT_TYPE_BUMP_ATTACK;		//�^�C�v����]�U����
	g_swimer.ani.fram = SWIMER_TYPE_BUMP_ATTACK_ANIMATION_FRAM;		//�t���[����
	g_swimer.ani.max = SWIMER_TYPE_BUMP_ATTACK_ANIMATION_MAX;		//�ő�R�}��
	//���݂̍U���r�b�g�ڂ��擾
	g_SwimerAttackBitTypeNow |= SWIMER_ANIMATION_BIT_TYPE_BUMP_ATTACK;

}

//**********************************************************************************************************
//�v���C���[�A�N�V�����^�C�v�O�ԁu�o�^�t���C�̓����v
//**********************************************************************************************************
void SwimerActionType0(){
	
	//�U���^�C�v����
	g_swimer.attack_type = SWIMER_ATTACK_WAVE_ATTACK;		
}

//**********************************************************************************************************
//�v���C���[�A�N�V�����^�C�v�P�ԁu��]�̓����v
//**********************************************************************************************************
void SwimerActionType1(){

	//�U���^�C�v����
	g_swimer.attack_type = SWIMER_ATTACK_CIRCLE_ATTACK;
	//�p�x�̉��Z
	g_swimer.ch.cnt += 3;
	CenterCharacter(&g_swimer.ch, (g_swimer.ch.w * g_swimer.ch.scale), g_swimer.ch.cnt);
	//360���܂�
	g_swimer.ch.cnt %= 360;
	//�� �A�j���[�V�����X�V
	ChangeSwimerAttackAnimation();		
}

//**********************************************************************************************************
//�v���C���[�A�N�V�����^�C�v�Q�ԁu�x�W�F�Ȑ��̓����v
//**********************************************************************************************************
void SwimerActionType2(){

	//�C�X�e�[�W�����擾
	STAGE* pst = GetSeaStage();
	//�Z�b�g����x�W�F�Ȑ��̗v�f��
	int setbezier = 4;
	//�U���^�C�v����
	g_swimer.attack_type = SWIMER_ATTACK_BUMP_ATTACK;
	
	//���_
	POINT pos[4] = {
		{0 , 0} ,			//�n�_
		{800 , 300} ,		//���ԓ_�P
		{800 , -200} ,		//���ԓ_�R
		{0 , 0} ,			//�I�_
	};

	//�E�X�N���[����
	if( pst->scroll_r ){
		//���_�w��ύX
		pos[1].x = -800;
		pos[2].x = -800;
	}

	//�x�W�F�Ȑ��̒��_�Ɏ��g�̎n�_���W�����Z
	for( int i = 0 ; i < setbezier ; i++ ){
		pos[i].x += (int)g_swimer.ch.sx;		//X���W
		pos[i].y += (int)g_swimer.ch.sy;		//Y���W
	}
	//���Ԃ̌o��
	g_swimer.ch.millisecond += 0.01f;
	//�x�W�F�Ȑ��֐��̌Ăяo��
	SetBezier( &g_swimer.ch , &pos[0] , setbezier - 1 , g_swimer.ch.millisecond );
	//�� �A�j���[�V�����X�V
	ChangeSwimerAttackAnimation();

}

//************************************************************************************************
//���ʂP�u���x�Q�{�v
//************************************************************************************************
void SwimerEffect0(){


	g_swimer.attack_type = SWIMER_ATTACK_NORMAL;				//�U���^�C�v
	g_swimer.attack = false;									//�U�����t���O
	//�v���C���[�u�X�C�}�[�v�̃A�j���[�V�����ݒ�
	g_swimer.speedlevel = SWIMER_SPEED_NORMAL;					//�t���[����
	g_swimer.ch.direction = g_swimer.ch.direction_now;			//�摜��ʏ�֕ύX
	g_swimer.ani.type = SWIMER_ANIMATION_TYPE_DIRECTION_RIGHT;	//�j���A�j���[�V����
	//�U���ύX�p�A�j���[�V�����^�C�v���擾
	g_swimer.ani.fram = SWIMER_TYPE_NORMAL_ANIMATION_FRAM;		//�t���[����
	g_swimer.ani.max = SWIMER_TYPE_NORMAL_ANIMATION_MAX;		//�ő�R�}��

	//���蒼��ɕω�
	g_swimer.ani.time = g_swimer.ani.fram;

	//�r�b�g�ڂ���
	g_swimer.ani.type |= SWIMER_ANIMATION_TYPE_D_RIGHT_SPEED_UP;

	//���݂̃A�C�e���̌��ʔԍ����擾
	g_swimer.itemEffectType = SWIMER_ITEM_EFFECT_TYPE_SPEEDSTAR;

	//���x�{���������l��2�{
	//X�ړ���
	g_swimer.ch.mx = (float)( (int)g_swimer.ch.default_mx << 1 );
	//Y�ړ���
	g_swimer.ch.my = (float)( (int)g_swimer.ch.default_my << 1 ); 
	
	//�����2�{�܂�
	//X�ړ���
	if( g_swimer.ch.mx > (float)( (int)g_swimer.ch.default_mx << 1 ) ){
		g_swimer.ch.mx = (float)( (int)g_swimer.ch.default_mx << 1 );
	}
	//Y�ړ���
	if( g_swimer.ch.my > (float)( (int)g_swimer.ch.default_my << 1 ) ){
		g_swimer.ch.my = (float)( (int)g_swimer.ch.default_my << 1 );
	}
	
	//�����ʏ���Q�{��
	g_swimer.speedlevel = SWIMER_SPEED_RAPID;
}

//************************************************************************************************
//���ʂQ�u���G���v
//************************************************************************************************
void SwimerEffect1(){

	g_swimer.attack_type = SWIMER_ATTACK_NORMAL;				//�U���^�C�v
	g_swimer.attack = false;									//�U�����t���O
	//�v���C���[�u�X�C�}�[�v�̃A�j���[�V�����ݒ�
	g_swimer.speedlevel = SWIMER_SPEED_NORMAL;					//�t���[����
	g_swimer.ch.direction = g_swimer.ch.direction_now;			//�摜��ʏ�֕ύX
	g_swimer.ani.type = SWIMER_ANIMATION_TYPE_DIRECTION_RIGHT;	//�j���A�j���[�V����
	//�U���ύX�p�A�j���[�V�����^�C�v���擾
	g_swimer.ani.fram = SWIMER_TYPE_NORMAL_ANIMATION_FRAM;		//�t���[����
	g_swimer.ani.max = SWIMER_TYPE_NORMAL_ANIMATION_MAX;		//�ő�R�}��

	//���蒼��ɕω�
	g_swimer.ani.time = g_swimer.ani.fram;
	//�^�C�v�𖳓G��Ԃ�
	g_swimer.ani.type |= SWIMER_ANIMATION_TYPE_D_RIGHT_ABSOLUTE;
	//���݂̃A�C�e���̌��ʔԍ����擾
	g_swimer.itemEffectType = SWIMER_ITEM_EFFECT_TYPE_ABSOLUTE;
	//�v���C���[�̓G������Ԃ����Z�b�g
	g_swimer.ch.time = 0;
	//�����t���O
	g_swimer.ch.flag = true;
}

//************************************************************************************************
//���ʂR�u���x�����v
//************************************************************************************************
void SwimerEffect2(){

	//�v���C���[�̏�Ԃ�ʏ��
	OrdinarySwimer();

	//���蒼��ɕω�
	g_swimer.ani.time = g_swimer.ani.fram;

	//�r�b�g�ڂ���
	g_swimer.ani.type |= SWIMER_ANIMATION_TYPE_D_RIGHT_SPEED_DOWN;

	//���݂̃A�C�e���̌��ʔԍ����擾
	g_swimer.itemEffectType = SWIMER_ITEM_EFFECT_TYPE_SNAIL;

	//���x�{���������l�̔���
	//X�ړ���
	g_swimer.ch.mx = (float)((int)g_swimer.ch.default_mx >> 1);
	
	//Y�ړ���
	g_swimer.ch.my = (float)((int)g_swimer.ch.default_my >> 1); 
	
	//�����2�{�܂�
	//X�ړ���
	if(g_swimer.ch.mx > (float)((int)g_swimer.ch.default_mx >> 1)){
		g_swimer.ch.mx = (float)((int)g_swimer.ch.default_mx >> 1);
	}
	
	//Y�ړ���
	if(g_swimer.ch.my > (float)((int)g_swimer.ch.default_my >> 1)){
		g_swimer.ch.my = (float)((int)g_swimer.ch.default_my >> 1);
	}
	
	//�����ʏ���0.5�{��
	g_swimer.speedlevel = SWIMER_SPEED_SLOW;
}

//************************************************************************************************
//���ʂS�u�g��v
//************************************************************************************************
void SwimerEffect3(){
	//�摜�T�C�Y��1.25�{�ɂ���
	g_swimer.ch.scale = 1.25f;

}

//************************************************************************************************
//���ʂT�u�k���v
//************************************************************************************************
void SwimerEffect4(){
	//�摜���T�C�Y��3/4�ɂ���
	g_swimer.ch.scale = 0.75f;
}