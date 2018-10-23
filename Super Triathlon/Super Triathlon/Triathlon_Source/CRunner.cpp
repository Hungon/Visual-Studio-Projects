#include	"CRunner.h"
#include	<iostream>

using namespace std;


/*
************************************************************************************
�R���X�g���N�^
************************************************************************************
*/
CRunner::CRunner(){

	//���e���҂̋��ʂ���������
	//0�̗v�f�̏�����
	ZeroMemory( &this->m_RunnerDefault , sizeof( RUNNER ) );

	this->m_RunnerDefault.ch.w = RUNNER_DEFAULT_IMAGE_WIDTH;					//��
	this->m_RunnerDefault.ch.h = RUNNER_DEFAULT_IMAGE_HEIGHT;					//����
	this->m_RunnerDefault.ch.my = RUNNER_DEFAULT_MOVE_Y;						//�ړ���Y
	this->m_RunnerDefault.ch.speed = RUNNER_DEFAULT_SPEED_RATE;					//���x�{��
	this->m_RunnerDefault.aggregateSpeed = this->m_RunnerDefault.ch.speed * 
		this->m_RunnerDefault.ch.my;											//���x���v
	this->m_RunnerDefault.ch.defaultSpeed = RUNNER_DEFAULT_SPEED_RATE;			//���x�{�������l
	this->m_RunnerDefault.avoidtime = RUNNER_DEFAULT_AVOID_TIME;				//�������
	this->m_RunnerDefault.speed_max = RUNNER_DEFAULT_SPEEDMAX_RATE;				//���x�{���̍ő�l
	this->m_RunnerDefault.speed_min = RUNNER_DEFAULT_SPEEDMIN_RATE;				//���x�{���̍ŏ��l
	this->m_RunnerDefault.speedChargeMax = RUNNER_DEFAULT_CHARGEMAX_RATE;		//���x�`���[�W�l�̍ő�l
	this->m_RunnerDefault.ch.scale = RUNNER_DEFAULT_IMAGE_SCALE;				//�g�k�{��
	this->m_RunnerDefault.ch.defaultScale = RUNNER_DEFAULT_IMAGE_SCALE;			//�g�k�{�������l
	this->m_RunnerDefault.ch.SearchSquareFront = RUNNER_DEFAULT_SEARCH_FRONT;	//�O���ɍL�����`�͈�
	this->m_RunnerDefault.ch.SearchSquareWidth = RUNNER_DEFAULT_SEARCH_WIDE;	//�L�����`�͈͂̕�
	this->m_RunnerDefault.bumpType = RUNNER_NOT_BUMP;							//�Փ˃t���O
	this->m_RunnerDefault.obstacleBumpNow = -1;									//��Q���̗v�f
	this->m_RunnerDefault.effectType = RUNNER_TYPE_NORMAL;						//�G�t�F�N�g�^�C�v
	//�A�j���[�V�����ݒ�
	this->m_RunnerDefault.ani.max = RUNNER_ANIMATION_TYPE_NORMAL_CNT_MAX;					//�ő�R�}��
	this->m_RunnerDefault.ani.fram = RUNNER_ANIMATION_TYPE_NORMAL_FRAM;						//�t���[����

	//���e�̐ݒ�
	//0�ŏ�����
	ZeroMemory( &this->m_ShadowDefault , sizeof( RUNNER ) );
	this->m_ShadowDefault.ch.w = 64;											//��
	this->m_ShadowDefault.ch.h = 64;											//����
	this->m_ShadowDefault.ch.alpha = 120.0f;									//���ߗ�
}

/*
************************************************************************************
�f�B�X�g���N�^
************************************************************************************
*/
CRunner::~CRunner(){

}

/*************************************************************************************************
�n�[�h�����щz�����ۂ̃����i�[�̋������ʂP�u���x�㏸�v
**************************************************************************************************
*/
void   CRunner::RunnerEffectSpeedUp(){

	//���݂̌��ʃ^�C�v
	this->m_RunnerDefault.effectType |= RUNNER_EFFECT_TYPE_SPEED_UP;

	//����
	this->m_RunnerDefault.ch.speed += 0.1f;

	//���x�̏��
	if( this->m_RunnerDefault.ch.speed >= this->m_RunnerDefault.speed_max ){
		this->m_RunnerDefault.ch.speed = this->m_RunnerDefault.speed_max;
	}

	cout << "SpeedUp" << endl;

}


/*************************************************************************************************
�n�[�h�����щz�����ۂ̃����i�[�̋������ʂQ�u���G��ԁv
**************************************************************************************************
*/
void   CRunner::RunnerEffectAbsolute(){

	//���݂̌��ʃ^�C�v
	this->m_RunnerDefault.effectType |= RUNNER_EFFECT_TYPE_ABSOLUTE;
	
	//���Ԃ���
	this->m_RunnerDefault.absoluteTime = RUNNER_ABSOLUTE_EFFECT_TIME;

	cout << "Absolute" << endl;

}




/*************************************************************************************************
��Q���ɂԂ������ۂɌ��ʒ��̋����𖳌��ɂ���
���x�㏸�𖳌�
**************************************************************************************************
*/
void   CRunner::RepealRunnerEffectSpeedUp(){

	//�^�C�v���Ȃ���
	this->m_RunnerDefault.effectType &= ~RUNNER_EFFECT_TYPE_SPEED_UP;
	
	//���x�����ɖ߂�
	this->m_RunnerDefault.ch.speed = RUNNER_DEFAULT_SPEED_RATE;

}


/*************************************************************************************************
���Ԍo�߂Ŗ��G��Ԃ𖳌��ɂ���
**************************************************************************************************
*/
bool	CRunner::RepealRunnerEffectAbsolute() {

	//�߂�l�p
	bool ret = false;

	//���G���A�������͑��x�㏸�����G���Ȃ玞�Ԃ𑪂�
	if (this->m_RunnerDefault.effectType == RUNNER_EFFECT_TYPE_ABSOLUTE ||
		this->m_RunnerDefault.effectType == RUNNER_EFFECT_TYPE_SPEED_AND_ABSOLUTE) {
		//���Ԍo��
		this->m_RunnerDefault.absoluteTime--;
		ret = true;
	}
	//�ݒ肵�����Ԍo�ߌ�A���G��Ԃ𖳌���
	if (this->m_RunnerDefault.absoluteTime < 0) {
		//�^�C�v���Ȃ���
		this->m_RunnerDefault.effectType &= ~RUNNER_EFFECT_TYPE_ABSOLUTE;
		ret = false;
	}
	return ret;
}


/*
************************************************************************************************
�����蔻��@CollisionRunner
�����i�[�O���̏�Q���Ƃ̔�������ۂɁAY���W�̐H�����݂𔭐������Ĕ�������
�H�����܂���Y���W�̓����i�[�̍����̔����̃T�C�Y
************************************************************************************************
*/ 
bool	CRunner::CollisionRunner( CHARACTER* pch1 , CHARACTER* pch2 ){

	//�����i�[�O���̔���������ۂ́AY���W�̐H�����݂�����
	float attempt = (float)( pch1->h * pch1->scale ) / 2;

	//2�̏d�Ȃ�𒲂ׂ鎞�� �A�摜�̋�`�����A-������Y���W�������򂢍��܂��Ĕ�������
	if( pch1->x < pch2->x + (float)pch2->w * pch2->scale &&
	    pch1->x + (float)pch1->w * pch1->scale > pch2->x &&
 	    ( pch1->y + attempt ) < pch2->y + (float)pch2->h  * pch2->scale &&
	    pch1->y + (float)pch1->h  * pch1->scale > pch2->y ){		
		//�d�Ȃ��Ă�����true��Ԃ�
		return true;
	}
	//�d�Ȃ��Ă��Ȃ�������false��Ԃ�
	return false;
}
