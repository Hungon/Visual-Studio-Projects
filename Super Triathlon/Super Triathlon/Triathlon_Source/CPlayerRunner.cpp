#include	"CPlayerRunner.h"
#include	"CompetitorRunner.h"
#include	"RunnerEffect.h"
#include	"input.h"
#include	"sound.h"
#include	"map.h"
#include	"gadget.h"
#include	"wipe.h"
#include	"CEffectManager.h"
#include	"CRoadEnemy.h"
#include	"CRoadEnemyManager.h"
#include	"CRoadStage.h"
#include	<stdio.h>
#include	<iostream>
#include	<math.h>

using namespace std;


//�� �ÓI�����o�ϐ��̍Ē�`
const HWND	CPlayerRunner::m_hWnd = NULL;
//�摜
const int CPlayerRunner::PLAYER_RUNNER_IMAGE_MAX = 3;			//�g�p����
//��
const int CPlayerRunner::PLAYER_RUNNER_EFFECT_SOUND_MAX = 8;	//�g�p��
//�X�e�[�^�X���
//�̗�
const int CPlayerRunner::PLAYER_RUNNER_STAMINA_MAX = 3;	

//���擾�p
RUNNER	CPlayerRunner::m_GetRunner = { 0 };


/*
************************************************************************************************
�R���X�g���N�^
************************************************************************************************
*/
CPlayerRunner::CPlayerRunner(){}


/*
************************************************************************************************
�f�B�X�g���N�^
************************************************************************************************
*/
CPlayerRunner::~CPlayerRunner(){

}


//************************************************************************************************
//�X�e�[�W�P�ԁu����X�e�[�W�v�ł̏�����
//************************************************************************************************
void   CPlayerRunner::InitRunner( int runnerNum , CRoadEnemyManager	*lpRoadEnemy , float roadDistance ){

	//�� �摜�f�[�^
	char	*lpImageTbl[ CPlayerRunner::PLAYER_RUNNER_IMAGE_MAX ] = {
		
		{ "Image\\Road\\PlayerRunner.bmp" }	,		//player's own
		{ "Image\\Road\\shadow.bmp" }	,			//player's shadow
		{ "Image\\Road\\stamina.bmp" }	,			//player's stamina
	};
	//�� ���ʉ��f�[�^
	char	*lpSoundTbl[ CPlayerRunner::PLAYER_RUNNER_EFFECT_SOUND_MAX ] = {

		{ "SE\\speedcharge.wav" }		,			//speed up
		{ "SE\\chargedown.wav" }		,			//speed down
		{ "SE\\attack.wav" }		,				//attack
		{ "SE\\jump.wav"},							//jump
		{ "SE\\strongitem.wav"},					//when player got an invincible body
		{ "SE\\weakitem.wav"},						//when time out that is invincible body
		{ "SE\\Healing.wav"},						//healing body
	};


	//�e�탁�����̊m��
	this->m_lpPlayerRunnerImg = new int [ CPlayerRunner::PLAYER_RUNNER_IMAGE_MAX ];					//�摜
	this->m_lpPlayerRunnerEffectSound = new int [ CPlayerRunner::PLAYER_RUNNER_EFFECT_SOUND_MAX ];	//���ʉ�
	this->m_lpPlayerStatus = new RUNNER [ CPlayerRunner::PLAYER_RUNNER_STAMINA_MAX ];				//�̗�

	//��Q���̃A�h���X���擾
	this->m_lpCRoadEnemyForPlayer = lpRoadEnemy;

	//�� �\���̐ݒ�

	//�e�l��������
	this->m_RunnerDefault.ch.x = (float)( CHARACTER_CENTER_POSITION_X_TO_STAGE_WIDTH( this->m_RunnerDefault.ch.w ) );	//�\����X
	this->m_RunnerDefault.ch.y = roadDistance - CAMERA_FOCUS_POSITION_Y;				//�\����Y
	this->m_RunnerDefault.pos_x = this->m_RunnerDefault.ch.x;											//�\���ʒuX
	this->m_RunnerDefault.pos_y = STAGE_HEIGHT - CAMERA_FOCUS_POSITION_Y;								//�\���ʒuY
	this->m_RunnerDefault.ch.flag = true;																//�����t���O
	this->m_RunnerDefault.ch.hp = RUNNER_DEFAULT_STAMINA_MAX;											//�̗�

	//�L�^�p
	this->m_RunnerDefault.jumpedHurdleChain = 0;				//�n�[�h����A���Ŕ�щz������							
	this->m_RunnerDefault.jumpedHurdleChainMax = 0;				//�A���Ŕ�щz�����ō��L�^
	
	//�L�^�p
	//��щz�����n�[�h���̐�
	for( int i = 0 ; i < ROADENEMY_OBSTACLE_HURDLE_KIND_OF_COLOR ; i++ ){ this->m_RunnerDefault.jumpedHurdleCnt[i] = 0; }

	//�A�j���[�V�������̐ݒ�
	SetAnimation( &this->m_RunnerDefault.ch , &this->m_RunnerDefault.ani ,
				this->m_RunnerDefault.ani.sx , this->m_RunnerDefault.ani.sy ,
				this->m_RunnerDefault.ani.max , this->m_RunnerDefault.ani.fram , 
				this->m_RunnerDefault.ani.type , true);


	//�A�X�e�[�^�X���̐ݒ�
	//�̗�
	//1��
	this->m_lpPlayerStatus[0].ch.x = 30.0f;							//�\����X���W
	this->m_lpPlayerStatus[0].ch.y = 120.0f;						//�\����Y���W
	this->m_lpPlayerStatus[0].ch.w = 32;							//��
	this->m_lpPlayerStatus[0].ch.h = 32;							//����
	this->m_lpPlayerStatus[0].ch.flag = true;						//�\���t���O
	this->m_lpPlayerStatus[0].ch.picx = 0;							//�\����X���W
	this->m_lpPlayerStatus[0].ch.picy = 0;							//�\����Y���W
	this->m_lpPlayerStatus[0].ch.scale = 1.0f;						//�g�k��
	//2��
	this->m_lpPlayerStatus[1] = this->m_lpPlayerStatus[0];
	this->m_lpPlayerStatus[1].ch.x += this->m_lpPlayerStatus[1].ch.w;		//�\����X���W
	//3��
	this->m_lpPlayerStatus[2] = this->m_lpPlayerStatus[1];
	this->m_lpPlayerStatus[2].ch.x += this->m_lpPlayerStatus[2].ch.w;		//�\����X���W

	//�摜�̗v�f�𐶐�
	for( int i = 0 ; i < CPlayerRunner::PLAYER_RUNNER_IMAGE_MAX ; i++ ){
		this->m_lpPlayerRunnerImg[i] = CreateDDImage( lpImageTbl[i] , CPlayerRunner::m_hWnd );
	}
	//���ʉ��̗v�f�𐶐�
	for( int i = 0 ; i < CPlayerRunner::PLAYER_RUNNER_EFFECT_SOUND_MAX ; i++ ){
		this->m_lpPlayerRunnerEffectSound[i] = CreateSound( lpSoundTbl[i] );
	}

	//�擾�p�ɏ���ݒ肷��
	this->m_GetRunner = this->m_RunnerDefault;

	//initialize score setting
	ZeroMemory(&this->m_PlayerScore, sizeof(SCOREVALUE));
	this->m_PlayerScore.score_x = 450.0f;					
	this->m_PlayerScore.score_y = 50.0f;					
	this->m_PlayerScore.score_scale = 0.5f;				
	this->m_PlayerScore.dicimals_flag = false;				
	this->m_PlayerScore.ch.type = SCORE_NUMBER_TYPE_NORMAL;	
	this->m_PlayerScore.scoreColor = SCORE_BLACK;			
	this->m_PlayerScore.digit = 3;							
	this->m_PlayerScore.ch.flag = true;
}


//************************************************************************************************
//�u�����i�[�v�̍X�V
//************************************************************************************************
void   CPlayerRunner::UpdateRunner( int runner , CEffectManager *lpCEffect ){


	//�Փˎ��Ɏ擾������ʃf�[�^�e�[�u��
	BYTE bumpTbl[RUNNER_EFFECT_KIND_OF_TYPE] = {

		RUNNER_EFFECT_FLAG_ABSOLUTE				,		//���G���
		RUNNER_EFFECT_FLAG_SPEED_UP 			,		//���x�㏸
		RUNNER_EFFECT_FLAG_HEALING				,		//��
	
	};

	//���ʃf�[�^�e�[�u��
	BYTE effectTbl[RUNNER_EFFECT_KIND_OF_TYPE] = {

		RUNNER_EFFECT_TYPE_ABSOLUTE			,		//���G���
		RUNNER_EFFECT_TYPE_SPEED_UP			,		//���x�㏸
		RUNNER_EFFECT_TYPE_HEALING			,		//��
	};

	//�J�����̍��W���擾
	float comeCameraY = GetCameraY();
		
	//�e��A�N�V����
	if (this->m_RunnerDefault.ani.type == RUNNER_ANIMATION_ROUND) {
		//�� �W�����v�̍X�V
		this->UpdateActionPlayerRunnerJump();
		//�� �U���̍X�V
		this->UpdateActionPlayerRunnerAttack();
	}

	//���@�ړ��̍X�V
	this->UpdatePlayerRunnerMove();
	
	//���@���x�̍X�V
	//�ʏ펞�̂�
	if( this->m_RunnerDefault.ani.type == RUNNER_ANIMATION_ROUND ){
		this->UpdatePlayerRunnerSpeed();
	}
	//���L�����N�^�[�̈ړ��ʂ𑬓x���[�^�[�ɔ��f������
	SetStatus( this->m_RunnerDefault.aggregateSpeed );	

	//�� ��Q�������G���^�C�v�ԍ����擾����
	//�擾�����ޔԍ��͏�Q���̃r�b�g��
	this->m_RunnerDefault.getType = this->m_lpCRoadEnemyForPlayer->BroadenCollisionSquareToObstacle( &this->m_RunnerDefault.ch );
	
	//�� ��Q���Ƃ̓����蔻��
	//�����t���O��true�Ȃ�
	if( this->m_RunnerDefault.ch.flag ){
		//��Q���Ɣ��������ăt���O���󂯎��
		this->m_RunnerDefault.bumpType = this->m_lpCRoadEnemyForPlayer->CollisionRoadEnemyManager( &this->m_RunnerDefault );
		//�� �������ʂ̕���
		//�ʏ�̏ՓˁA�Փ˂��Ă��Ȃ��Ȃ珈�������Ȃ�
		if (this->m_RunnerDefault.bumpType != RUNNER_NOT_BUMP ||
			this->m_RunnerDefault.bumpType != RUNNNER_BUMP_TO_HURDLE ||
			this->m_RunnerDefault.bumpType != RUNNNER_BUMP_TO_ROCK) {
			//�^�C�v�̐�������
			for (int i = 0; i < RUNNER_EFFECT_KIND_OF_TYPE; i++) {
				//�擾�����Փ˃t���O�̔ԍ������ɕ���
				if (this->m_RunnerDefault.bumpType == bumpTbl[i]) {
					//�� �G�t�F�N�g�̐���
					lpCEffect->CreateEffectManaager(&this->m_RunnerDefault, runner);
					//�� �L�^�p
					//��щz�����n�[�h���̐����J�E���g
					this->m_RunnerDefault.jumpedHurdleChain++;
					//for draw
					this->m_PlayerScore.num = this->m_RunnerDefault.jumpedHurdleChain;
					//�^�C�v���ɃJ�E���g����
					this->m_RunnerDefault.jumpedHurdleCnt[this->m_RunnerDefault.bumpType - 1]++;
					//��щz���������ō��L�^�𒴂�����X�V����
					if (this->m_RunnerDefault.jumpedHurdleChain > this->m_RunnerDefault.jumpedHurdleChainMax) {
						//�L�^��h��ւ���
						this->m_RunnerDefault.jumpedHurdleChainMax = this->m_RunnerDefault.jumpedHurdleChain;
					}
					//�� ���ʂ̕���
					switch (i) {
					case 0:
						CRunner::RunnerEffectAbsolute();
						PlaySound(this->m_lpPlayerRunnerEffectSound[4], 0, false);		//invincible sound
						break;
					case 1:
						CRunner::RunnerEffectSpeedUp();
						PlaySound(this->m_lpPlayerRunnerEffectSound[0], 0, false);		//speed up sound
						break;
					case 2:
						this->RunnerEffectHealing();
						PlaySound(this->m_lpPlayerRunnerEffectSound[6], 0, false);		//Healing soung
						break;
					}
					//�Փ˃t���O���Ȃ���
					this->m_RunnerDefault.bumpType = RUNNER_NOT_BUMP;
					break;
				}
			}
		}
		//�� ���ʂɏՓ˂����珈��
		if( this->m_RunnerDefault.bumpType == RUNNNER_BUMP_TO_HURDLE ||
			this->m_RunnerDefault.bumpType == RUNNNER_BUMP_TO_ROCK){
			PlaySound( this->m_lpPlayerRunnerEffectSound[2] , 0 , false );			//beating sound
			if (fabs(this->m_RunnerDefault.aggregateSpeed) > fabs((RUNNER_DEFAULT_SPEED_RATE * RUNNER_DEFAULT_MOVE_Y))) {
				PlaySound(this->m_lpPlayerRunnerEffectSound[1], 0, false);			//speed down sound
			}
			//reset consecutive to chain of jump
			this->m_RunnerDefault.jumpedHurdleChain = 0;
			//for draw
			this->m_PlayerScore.num = this->m_RunnerDefault.jumpedHurdleChain;
			//�� ���x�㏸���ʂ𖳌���
			CRunner::RepealRunnerEffectSpeedUp();
			//�G�t�F�N�g���\��
			lpCEffect->RepealEffectManager( 0 , runner );
			//�����t���O
			this->m_RunnerDefault.ch.flag = false;
			//�̗͂����Z
			this->m_RunnerDefault.ch.hp--;
			//���݂̗̑͂��X�e�[�^�X�̗v�f���O�Ȃ珈�����s��Ȃ�
			if( this->m_RunnerDefault.ch.hp < PLAYER_RUNNER_STAMINA_MAX &&
				this->m_RunnerDefault.ch.hp >= 0 ){
				//���݂̗̑͂ɉ����đ̗͂̉摜�\�������W��ύX����
				this->m_lpPlayerStatus[this->m_RunnerDefault.ch.hp].ch.picx = this->m_lpPlayerStatus[0].ch.w;
				//�Փ˃t���O���Ȃ���
				this->m_RunnerDefault.bumpType = RUNNER_NOT_BUMP;
			}
		}
	}

	//�� ���G���̏���
	//���G���Ȃ玞�Ԃ𑪂�A���̎��ԂɂȂ�����ʏ�̖��G��Ԃ𖳌���
	if (this->m_RunnerDefault.effectType == RUNNER_EFFECT_TYPE_ABSOLUTE ||
		this->m_RunnerDefault.effectType == RUNNER_EFFECT_TYPE_SPEED_AND_ABSOLUTE) {
		if (CRunner::RepealRunnerEffectAbsolute() == false) {
			PlaySound(this->m_lpPlayerRunnerEffectSound[5], 0, false);			//into ordinary
			//�G�t�F�N�g��\����
			lpCEffect->RepealEffectManager(1, runner);
		}
	}
	//���@�̗͂�0�ɂȂ�����Q�[���I�[�o�[�֑J��
	if( this->m_RunnerDefault.ch.hp <= 0 ) CreateWipe( SCENE_GAMEOVER , CHANGE_SCENE , WIPE_CONTRACTION );

	//�� �G�t�F�N�g�̍X�V
	lpCEffect->UpdateEffectManager( &this->m_RunnerDefault , runner );

	//�v���C���[�̃��[�J�����W���X�V
	this->m_RunnerDefault.pos_y = this->m_RunnerDefault.ch.y - comeCameraY;
	
	//�� �J�����̏œ_���v���C���[���W�ōX�V�����J�������W�ɐݒ肷��
	SetCamera( 0 , this->m_RunnerDefault.ch.y - this->m_RunnerDefault.pos_y  ); 

	//�� �G�������
	AvoidEnemy( &this->m_RunnerDefault.ch , this->m_RunnerDefault.avoidtime );

	//���@��O�ɗ���قǂɑ傫�������鏈��
	FittingPositionToBackCameraAngle( &this->m_RunnerDefault.ch , 380.0f / 640.0f );

	//�� �v���C���[�A�j���[�V�����̕���
	this->ChangePlayerRunnerAnimation();

	//�� �ړ��͈͂𐧌�
	this->ConstrainPlayerRunnerMove();

	//�擾�p�ɏ���ݒ肷��
	this->m_GetRunner = this->m_RunnerDefault;

}

//************************************************************************************************
//�\�����s���֐�
//************************************************************************************************
void   CPlayerRunner::DrawRunner(){

	//�J�������W���擾
	float comeCameraX = GetCameraX();		//X
	float comeCameraY = GetCameraY();		//Y

	//�v���C���[�����i�[�̕\��
	if( this->m_RunnerDefault.ch.time % 2 == 0 ){
		//�e
		DDAlphaBlt(
			(int)( ( this->m_RunnerDefault.ch.x - 5.0f ) - comeCameraX ) ,
			(int)( ( this->m_RunnerDefault.ch.y + this->m_RunnerDefault.ch.h - 
			( this->m_ShadowDefault.ch.h / 2 ) ) - comeCameraY ) ,
			this->m_ShadowDefault.ch.w ,
			this->m_ShadowDefault.ch.h ,
			this->m_lpPlayerRunnerImg[1] ,
			this->m_ShadowDefault.ch.picx ,
			this->m_ShadowDefault.ch.picy ,
			this->m_ShadowDefault.ch.alpha );

		//�v���C���[
		DDScaleBlt(
			this->m_RunnerDefault.ch.x  - comeCameraX ,
			this->m_RunnerDefault.ch.y  - comeCameraY ,
			this->m_RunnerDefault.ch.w ,
			this->m_RunnerDefault.ch.h ,
			this->m_lpPlayerRunnerImg[0] ,
			this->m_RunnerDefault.ch.picx ,
			this->m_RunnerDefault.ch.picy ,
			this->m_RunnerDefault.ch.scale );
	}

	//�v���C���[�����i�[�̗̑͂�\��
	//3�\��
	for( int i = 0 ; i < CPlayerRunner::PLAYER_RUNNER_STAMINA_MAX ; i++ ){
		if( this->m_lpPlayerStatus[i].ch.flag ){
			//�u�n�[�g�摜�v
			DDScaleBlt(
				this->m_lpPlayerStatus[i].ch.x ,
				this->m_lpPlayerStatus[i].ch.y ,
				this->m_lpPlayerStatus[i].ch.w ,
				this->m_lpPlayerStatus[i].ch.h ,
				this->m_lpPlayerRunnerImg[2] ,
				this->m_lpPlayerStatus[i].ch.picx ,
				this->m_lpPlayerStatus[i].ch.picy ,
				this->m_lpPlayerStatus[i].ch.scale );
		}
	}
	//draw chain's score
	if (this->m_PlayerScore.ch.flag) DrawScore(&this->m_PlayerScore);
	//draw score's valueation
	DrawValueation(this->m_PlayerScore.score_x +
		(this->m_PlayerScore.score_scale * SCORE_NUMBER_AND_PUNCTUAL_IMAGE_WIDTH),
		this->m_PlayerScore.score_y,
		this->m_RunnerDefault.jumpedHurdleChain);
}


//************************************************************************************************
//������s���֐�
//************************************************************************************************
void   CPlayerRunner::ReleaseRunner(){

	//�摜�̉��
	if( this->m_lpPlayerRunnerImg != NULL ){
		for( int i = 0 ; i < PLAYER_RUNNER_IMAGE_MAX ; i++ ){
			ReleaseDDImage( this->m_lpPlayerRunnerImg[i] );
		}
	}
	//���ʉ��̉��
	if( this->m_lpPlayerRunnerEffectSound != NULL ){
		for( int i = 0 ; i < PLAYER_RUNNER_EFFECT_SOUND_MAX ; i++ ){
			ReleaseSound( this->m_lpPlayerRunnerEffectSound[i] );
		}
	}
	
	//�m�ۂ����������̉��
	delete []this->m_lpPlayerRunnerImg;			//�摜
	this->m_lpPlayerRunnerImg = NULL;
	delete []this->m_lpPlayerRunnerEffectSound;	//���ʉ�
	this->m_lpPlayerRunnerEffectSound = NULL;

	//�������̉��
	if( this->m_lpPlayerStatus != NULL ){
		delete []this->m_lpPlayerStatus;			//�X�e�[�^�X
		this->m_lpPlayerStatus = NULL;
	}



}

//************************************************************************************************
//�u�����i�[�̈ړ��v�̍X�V
//************************************************************************************************
void   CPlayerRunner::UpdatePlayerRunnerMove(){

	//������
	//�E�ړ�
	if( KeepKey( VK_RIGHT ) || KeepLever(LEVER_RIGHT)) this->m_RunnerDefault.ch.mx = RUNNER_DEFAULT_MOVE_RIGHT;
	//���ړ�
	if( KeepKey( VK_LEFT ) || KeepLever(LEVER_LEFT)) this->m_RunnerDefault.ch.mx = RUNNER_DEFAULT_MOVE_LEFT;
	//�����t���O��true�Ȃ�ړ��\
	if( this->m_RunnerDefault.ch.flag ){
		//��ړ�
		if( KeepKey( VK_UP ) || KeepLever(LEVER_UP)) this->m_RunnerDefault.ch.y += RUNNER_DEFAULT_MOVE_FRONT;
	}
	//���ړ�
	if( KeepKey( VK_DOWN ) || KeepLever(LEVER_DOWN)) this->m_RunnerDefault.ch.y += RUNNER_DEFAULT_MOVE_BACK;

	//���x�̍��v�l���X�V
	this->m_RunnerDefault.aggregateSpeed = this->m_RunnerDefault.ch.my * this->m_RunnerDefault.ch.speed;

	//�ړ��ʂ����Z
	this->m_RunnerDefault.ch.y += this->m_RunnerDefault.aggregateSpeed;
	//X�ړ���
	this->m_RunnerDefault.ch.x += this->m_RunnerDefault.ch.mx;

	//�ړ��ʂ����Z�b�g
	this->m_RunnerDefault.ch.mx = 0.0f;							//X
	this->m_RunnerDefault.ch.my = RUNNER_DEFAULT_MOVE_Y;		//Y


}


//************************************************************************************************
//�u�����i�[�̑��x�v�̍X�V
//************************************************************************************************
void   CPlayerRunner::UpdatePlayerRunnerSpeed(){


	//���x�ɏ������^�C�v����
	tagRUNNERANIMATIONSPEED speedType[4] = {

		RUNNER_SPEED_SLOW	,		//�x��
		RUNNER_SPEED_NORMAL ,		//����
		RUNNER_SPEED_DECENT ,		//��������
		RUNNER_SPEED_RAPID	,		//����	
	
	};

	//�����x���x�����X�V
	//���������璲�ׂČ��ݑ��x���������烋�[�v�𔲂��āA�A�j���[�V�������X�V
	for( int i = 3 ; i >= 0 ; i-- ){
		
		//���݂̑��x�����̒l�������Ă�����
		if( fabs( this->m_RunnerDefault.ch.my ) * this->m_RunnerDefault.ch.speed > 
			i * RUNNER_SPEED_ANIMATION_RATE ){
			
			//���x���x���̃}�N������
			this->m_RunnerDefault.ani.fram = speedType[i];
			break;
		}
	}

}


//************************************************************************************************
//�v���C���[�����i�[�̃W�����v�����̍X�V
//************************************************************************************************
void   CPlayerRunner::UpdateActionPlayerRunnerJump() {


	//���n���uW�L�[ or B�{�^���v�ŃW�����v
	if (JustKey('W') || JustButton(BUTTON_2)) {
		PlaySound(this->m_lpPlayerRunnerEffectSound[3], 0, false);		//jump sound
		//���݂̃A�j���[�V�����̃R�}�������Z�b�g
		this->m_RunnerDefault.ani.cnt = 0;
		//���Ԃ����Z�b�g
		this->m_RunnerDefault.ani.time = 0;
		//�ő�R�}�����Z�b�g
		this->m_RunnerDefault.ani.max = RUNNER_ANIMATION_TYPE_JUMP_CNT_MAX;
		//�t���[�������Z�b�g
		this->m_RunnerDefault.ani.fram = RUNNER_ANIMATION_TYPE_JUMP_FRAM;
		//�W�����v�t���O�𗧂Ă�
		this->m_RunnerDefault.actionF |= RUNNER_ACTION_JUMP;
		//�A�j���[�V�����^�C�v���W�����v���ɕω�������
		this->m_RunnerDefault.ani.type |= RUNNER_ANIMATION_JUMP;		//�W�����v�̃A�j���[�V����
	}
	//�ڒn��Ԃɖ߂��Ă�����W�����v�t���O���Ȃ���
	if (this->m_RunnerDefault.ani.type == RUNNER_ACTION_ROUND) {
		//�t���O��OFF
		this->m_RunnerDefault.actionF &= ~RUNNER_ACTION_JUMP;
	}

}


//************************************************************************************************
//�v���C���[�����i�[�̍U�������̍X�V
//************************************************************************************************
void   CPlayerRunner::UpdateActionPlayerRunnerAttack() {

	//���n���A���AA�L�[ or A�{�^���ōU��
	if (JustKey('A') || JustButton(BUTTON_1)) {
		//���݂̃A�j���[�V�����̃R�}�������Z�b�g
		this->m_RunnerDefault.ani.cnt = 0;
		//���Ԃ����Z�b�g
		this->m_RunnerDefault.ani.time = 0;
		//�ő�R�}�����Z�b�g
		this->m_RunnerDefault.ani.max = RUNNER_ANIMATION_TYPE_ATTACK_CNT_MAX;
		//�t���[�������Z�b�g
		this->m_RunnerDefault.ani.fram = RUNNER_ANIMATION_TYPE_ATTACK_FRAM;
		//�U���t���O��
		this->m_RunnerDefault.actionF |= RUNNER_ACTION_ATTACK;
		//�A�j���[�V�����^�C�v���U���̓����
		this->m_RunnerDefault.ani.type |= RUNNER_ANIMATION_ATTACK;
	}
	//�ڒn��Ԃɖ߂��Ă�����U���t���O���Ȃ���
	if (this->m_RunnerDefault.ani.type == RUNNER_ACTION_ROUND) {
		//�t���O��OFF
		this->m_RunnerDefault.actionF &= ~RUNNER_ACTION_ATTACK;
	}

}


//************************************************************************************************
//�v���C���[�����i�[�̃A�j���[�V�������X�V
//************************************************************************************************
void   CPlayerRunner::ChangePlayerRunnerAnimation(){

	//�摜�f�[�^�e�[�u���iY���W�ŕω��j
	char animationTbl[RUNNER_ANIMATION_MAX] = {

		RUNNER_ANIMATION_ROUND	,		//�ڒn���
		RUNNER_ANIMATION_JUMP	,		//�W�����v��
		RUNNER_ANIMATION_ATTACK	,		//�U����
	
	};

	//���݂̃A�j���[�V�����̃^�C�v�ԍ��ɉ����ĉ摜��ω�������
	for( int i = 0 ; i < RUNNER_ANIMATION_MAX ; i++ ){

		//�A�j���[�V�����ɉ����ĉ摜����
		if( animationTbl[i] == this->m_RunnerDefault.ani.type ){

			//�\����Y���W�𔽉f����
			this->m_RunnerDefault.ch.direction = animationTbl[i];

			//�� �v���C���[�̃A�j���[�V�����X�V
			if( UpdateAnimation( &this->m_RunnerDefault.ch , &this->m_RunnerDefault.ani ) == false ){

				//���݂̃A�j���[�V�����̃r�b�g�ڂ�0�ɂ���
				this->m_RunnerDefault.ani.type &= ~animationTbl[i];
				//�ʏ�̃A�j���[�V�����ɕύX
				this->m_RunnerDefault.ani.max = RUNNER_ANIMATION_TYPE_NORMAL_CNT_MAX;	//�ő�R�}��
				this->m_RunnerDefault.ani.fram = RUNNER_ANIMATION_TYPE_NORMAL_FRAM;		//�t���[����
			
			}

			break;

		}

	}

}


//************************************************************************************************
//�ړ��͈͂𐧌�����
//************************************************************************************************
void   CPlayerRunner::ConstrainPlayerRunnerMove() {


	//�J�������W���擾
	float comeCameraY = GetCameraY();

	//���ړ��͈͂𐧌�
	//calculate competitor's coordinateY in local coordinateY
	float localY = this->m_RunnerDefault.ch.y - comeCameraY;
	//calculate railing's coordinateY into rate of coordinateY in stage's height
	float rateY = ((float)STAGE_HEIGHT - localY) / (float)(STAGE_HEIGHT - ROADSTAGE_SUBTLTY_POSITIONX_FOR_ROAD_WIDTH);
	//constrain move
	float margin = ROADSTAGE_MARGIN_WIDTH_FOR_TOP_AND_BOTTOM_IN_ROAD * rateY;
	//��ʍ��[
	if (this->m_RunnerDefault.ch.x < margin) this->m_RunnerDefault.ch.x = margin;
	//��ʉE�[
	if (this->m_RunnerDefault.ch.x + (this->m_RunnerDefault.ch.w * this->m_RunnerDefault.ch.scale) >
		(STAGE_WIDTH - margin)) {
		this->m_RunnerDefault.ch.x = (STAGE_WIDTH - margin) - (this->m_RunnerDefault.ch.w * this->m_RunnerDefault.ch.scale);
	}

	//��ʏ�[
	if (this->m_RunnerDefault.ch.y < comeCameraY) this->m_RunnerDefault.ch.y = comeCameraY;
	//��ʉ��[
	if (this->m_RunnerDefault.ch.y + (this->m_RunnerDefault.ch.h * this->m_RunnerDefault.ch.scale) >
		comeCameraY + STAGE_HEIGHT) {
		this->m_RunnerDefault.ch.y = comeCameraY + STAGE_HEIGHT -
			(this->m_RunnerDefault.ch.h * this->m_RunnerDefault.ch.scale);
	}

}



//***********************************************************************************************
//�����̃L�����N�^�[�\���̂̍��G�͈͂𔻒�����
//***********************************************************************************************
bool	CPlayerRunner::BroadenCollisionSquarePlayer( CHARACTER* pch ){


	//�߂�l�p
	bool ret = false;

	//�����̃L�����N�^�[�̍��G�͈͋�`�ƁA���g�̋�`��
	//��������
	if( BroadenCollisionSquare( &this->m_RunnerDefault.ch , pch ) ) ret = true;

	//�ŏI�I�Ȕ����Ԃ�
	return ret;

}

/*************************************************************************************************
�n�[�h�����щz�����ۂ̃����i�[�̋������ʂR�u�񕜁v
**************************************************************************************************
*/
void   CPlayerRunner::RunnerEffectHealing(){

	//�z��O�Ȃ�return
	if( this->m_RunnerDefault.ch.hp >= RUNNER_DEFAULT_STAMINA_MAX ) return;

	//���݂̌��ʃ^�C�v
	this->m_RunnerDefault.effectType |= RUNNER_EFFECT_TYPE_HEALING;

	//���݂̗̑͂ɉ����đ̗͂̉摜�\�������W��ύX����
	this->m_lpPlayerStatus[this->m_RunnerDefault.ch.hp].ch.picx = 0;

	//�̗͂��P��
	this->m_RunnerDefault.ch.hp++;

	//�̗͂̏���l
	if( this->m_RunnerDefault.ch.hp > RUNNER_DEFAULT_STAMINA_MAX ) this->m_RunnerDefault.ch.hp = RUNNER_DEFAULT_STAMINA_MAX;

	//���݂̌��ʃ^�C�v�𖳌�
	this->m_RunnerDefault.effectType &= ~RUNNER_EFFECT_TYPE_HEALING;

	cout << "Heal" << endl;


}
