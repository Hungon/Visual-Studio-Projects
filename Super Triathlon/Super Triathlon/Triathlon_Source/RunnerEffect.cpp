#include	"RunnerEffect.h"
#include	"map.h"
#include	"CPlayerRunner.h"

// ���ÓI�����o�̍Ē�`
//�G�t�F�N�g�^�C�v���̉摜�ݒ�
//�����O�^
const int CRunnerEffect::RUNNER_EFFECT_TYPE_RING_IMAGE_HEIGHT	= 96;					//����
const int CRunnerEffect::RUNNER_EFFECT_TYPE_RING_IMAGE_WIDTH	= 96;					//��
const float CRunnerEffect::RUNNER_EFFECT_TYPE_RING_IMAGE_SCALE	= 1.0F;					//�g�嗦
const float CRunnerEffect::RUNNER_EFFECT_TYPE_RING_IMAGE_CONTRACTION_SPEED = -0.05f;	//�g�嗦�̑��x

																			//�{�[���^
const int CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_IMAGE_HEIGHT	= 32;		//����
const int CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_IMAGE_WIDTH	= 32;		//��
const float CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_IMAGE_SCALE	= 1.0F;		//�g�嗦
//�A�j���[�V�����ݒ�
const int CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_ANIMATION_MAX = 4;			//�ő�R�}��
const int CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_ANIMATION_FRAM = 5;		//�t���[����

//************************************************************************************************
//�R���X�g���N�^
//************************************************************************************************
CRunnerEffect::CRunnerEffect(){


}

//************************************************************************************************
//�f�B�X�g���N�^
//************************************************************************************************
CRunnerEffect::~CRunnerEffect(){

	//�g�p�����������̉��
	delete []this->m_lpEffectAni;
	this->m_lpEffectAni = NULL;
	
	delete []this->m_lpPowerAni;
	this->m_lpPowerAni = NULL;
	
	delete []this->m_lpEffectCh;
	this->m_lpEffectCh = NULL;
	
	delete []this->m_lpPowerCh;
	this->m_lpPowerCh = NULL;

	delete []this->m_lpRunnerEffectImg;
	this->m_lpRunnerEffectImg = NULL;

	delete this->m_lpCPlayer;
	this->m_lpCPlayer = NULL;

}



/************************************************************************************************
�v���C���[�G�t�F�N�g�̏�����
*************************************************************************************************
*/
void CRunnerEffect::InitRunnerEffect( int effectMax ){

	//�����̍ő吔�����������̊m��
	//�@ ���ʔ������̉摜
	this->m_lpEffectAni = new ANIMATION [ effectMax ];		//�A�j���[�V����
	this->m_lpEffectCh = new CHARACTER [ effectMax ];		//�摜�ݒ�p

	//�A ���ʒ��̉摜�͎�ނ̐������m�ۂ���
	this->m_lpPowerAni = new ANIMATION[ RUNNER_EFFECT_SIGN_KIND_OF_TYPE ];
	this->m_lpPowerCh = new CHARACTER[ RUNNER_EFFECT_SIGN_KIND_OF_TYPE ];

	//allow plaayer class
	this->m_lpCPlayer = new CPlayerRunner;

	//����������G�t�F�N�g�̍ő吔����
	this->m_CreateEffectMax = effectMax;
			
	//�� �摜�ݒ�
	for( int i = 0 ; i < this->m_CreateEffectMax ; i++ ){
	
		//�� ���ʔ������̃G�t�F�N�g
		this->m_lpEffectCh[i].x = 0.0f;			//�\�������WX
		this->m_lpEffectCh[i].y = 0.0f;			//�\�������WY
		this->m_lpEffectCh[i].w = CRunnerEffect::RUNNER_EFFECT_TYPE_RING_IMAGE_WIDTH;			//��
		this->m_lpEffectCh[i].h = CRunnerEffect::RUNNER_EFFECT_TYPE_RING_IMAGE_HEIGHT;			//����
		this->m_lpEffectCh[i].picx = 0;			//�\����X
		this->m_lpEffectCh[i].picy = 0;			//�\����Y
		this->m_lpEffectCh[i].flag = false;		//�\���t���O
		this->m_lpEffectCh[i].scale = CRunnerEffect::RUNNER_EFFECT_TYPE_RING_IMAGE_SCALE;		//�g�k��

	}
		
	//�� ���ʒ��̃G�t�F�N�g
	for( int i = 0 ; i < RUNNER_EFFECT_SIGN_KIND_OF_TYPE ; i++ ){
	
		this->m_lpPowerCh[i].x = 0.0f;			//�\�������WX
		this->m_lpPowerCh[i].y = 0.0f;			//�\�������WY
		this->m_lpPowerCh[i].w = CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_IMAGE_WIDTH;			//��
		this->m_lpPowerCh[i].h = CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_IMAGE_HEIGHT;			//����
		this->m_lpPowerCh[i].picx = 0;			//�\����X
		this->m_lpPowerCh[i].picy = 0;			//�\����Y
		this->m_lpPowerCh[i].flag = false;		//�\���t���O
		this->m_lpPowerCh[i].scale = CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_IMAGE_SCALE;		//�g�k��

		//�A�j���[�V�����ݒ�
		this->m_lpPowerAni[i].cnt = 0;			//�R�}��
		this->m_lpPowerAni[i].time = 0;			//����
		this->m_lpPowerAni[i].sx = 0;			//�\����X
		this->m_lpPowerAni[i].sy = 0;			//�\����Y
		this->m_lpPowerAni[i].max = CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_ANIMATION_MAX;		//�R�}�̍ő喇��
		this->m_lpPowerAni[i].fram = CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_ANIMATION_FRAM;		//�R�}�̃t���[����
	}

}

/*
***********************************************************************************************
�G�t�F�N�g����
*************************************************************************************************
*/
void CRunnerEffect::CreateRunnerEffect( float x , float y , char type ){

	//��ނ��܂Ƃ߂��f�[�^�e�[�u��
	tagRUNNEREFFECTKIND effectTypeTbl[ RUNNER_EFFECT_SIGN_KIND_OF_TYPE ] = {
		RUNNER_EFFECT_TYPE_BLUE		 ,
		RUNNER_EFFECT_TYPE_RED		 ,
	};

	//�o��������G�t�F�N�g�̐�������
	for( int i = 0 ; i < this->m_CreateEffectMax ; i++ ){

		//�����t���O��false�Ȃ�
		if( this->m_lpEffectCh[i].flag == false ){

			//�����̃^�C�v�𐶐�
			this->m_lpEffectCh[i].x = x;				//X���W
			this->m_lpEffectCh[i].y = y;				//Y���W
			this->m_lpEffectCh[i].type = type;			//��ޔԍ�����
			this->m_lpEffectCh[i].flag = true;			//�����t���O
			this->m_lpEffectCh[i].scale = CRunnerEffect::RUNNER_EFFECT_TYPE_RING_IMAGE_SCALE;	//�g�k��
			this->m_lpEffectCh[i].picy = type * CRunnerEffect::RUNNER_EFFECT_TYPE_RING_IMAGE_HEIGHT;

			//�񕜂̗v�f�Ȃ炱����return����
			if( type == RUNNER_EFFECT_TYPE_GREEN ) return;
			
			//���ʒ��摜�̐ݒ�
			for( int j = 0 ; j < RUNNER_EFFECT_SIGN_KIND_OF_TYPE ; j++ ){

				//�����t���O��false����
				//�擾�����^�C�v�ԍ��Ȃ�
				if( this->m_lpPowerCh[j].flag == false &&
					this->m_lpEffectCh[i].type == effectTypeTbl[j] ){

					//�����̃^�C�v�𐶐�
					this->m_lpPowerCh[j].x = x + CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_IMAGE_WIDTH;	//X���W
					this->m_lpPowerCh[j].y = y + ( effectTypeTbl[j] * 
						CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_IMAGE_HEIGHT);							//Y���W
					this->m_lpPowerCh[j].type = type;													//��ޔԍ�����
					this->m_lpPowerCh[j].flag = true;													//�����t���O
					this->m_lpPowerCh[j].scale = CRunnerEffect::RUNNER_EFFECT_TYPE_BALL_IMAGE_SCALE;	//�g�k��
					this->m_lpPowerCh[j].time = 0;
					//�A�j���[�V�����̐ݒ�
					this->m_lpPowerAni[j].cnt = 0;			//�R�}��
					this->m_lpPowerAni[j].time = 0;			//�o�ߎ���
					//��ނɉ����ĕ\�������W��ύX
					this->m_lpPowerCh[j].direction= type;
					break;
				}
			}
			return;
		}
	}
}


//************************************************************************************************
//�X�V
//************************************************************************************************
void CRunnerEffect::UpdateRunnerEffect(float x, float y, int w, int h, int invincibleTime) {

	//�� �G�t�F�N�g�摜�̕\��
	for (int i = 0; i < this->m_CreateEffectMax; i++) {
		//�����t���O��true�Ȃ�
		if (this->m_lpEffectCh[i].flag)
		{
			//���W���X�V
			this->m_lpEffectCh[i].x = x - w;
			this->m_lpEffectCh[i].y = y;
			//�k�����ĕ\���t���O��false��
			this->m_lpEffectCh[i].scale += CRunnerEffect::RUNNER_EFFECT_TYPE_RING_IMAGE_CONTRACTION_SPEED;
			//0�ȉ��ɂȂ�����\���t���O��false��
			if (this->m_lpEffectCh[i].scale <= 0) this->m_lpEffectCh[i].flag = false;
		}
	}

	//�� �G�t�F�N�g���ʒ��̍X�V
	for (int i = 0; i < RUNNER_EFFECT_SIGN_KIND_OF_TYPE; i++) {
		//�����t���O��true�Ȃ�X�V
		if (this->m_lpPowerCh[i].flag) {
			//signal of repealing time for player's invincible
			if(this->m_lpPowerCh[i].type == RUNNER_EFFECT_TYPE_RED){
				if (invincibleTime < (RUNNER_ABSOLUTE_EFFECT_TIME >> 2)) {			//absolute time less than 125
					//count for flickers of invincible effect
					this->m_lpPowerCh[i].time++;
				}
			}
			//���W���X�V
			this->m_lpPowerCh[i].x = x + this->m_lpPowerCh[i].w;			//X���W
			this->m_lpPowerCh[i].y = y +
				(this->m_lpPowerCh[i].type * this->m_lpPowerCh[i].h);		//Y���W
			//effect ball moves around the character
			this->m_lpPowerCh[i].sx = x;			//subsutitute character's coordinates for decide center coordinateX and Y 
			this->m_lpPowerCh[i].sy = y;
			//rotating angle
			this->m_lpPowerCh[i].cnt++;
			//rotate process
			CenterCharacter(&this->m_lpPowerCh[i], 25, this->m_lpPowerCh[i].cnt++);
			this->m_lpPowerCh[i].cnt %= 360;
			//�A�j���[�V�����X�V
			UpdateAnimation(&this->m_lpPowerCh[i], &this->m_lpPowerAni[i]);
		}
	}

}


//************************************************************************************************
//�\��
//************************************************************************************************
void CRunnerEffect::DrawRunnerEffect(){

	//�J�����̍��W���擾
	float comeCameraY = GetCameraY();

	//�G�t�F�N�g�������摜�̕\��
	for( int i = 0 ; i < this->m_CreateEffectMax ; i++ ){

		//�����t���Otrue�Ȃ�
		if( this->m_lpEffectCh[i].flag ){

			//���ʔ������̃G�t�F�N�g
			DDScaleBlt(						
				this->m_lpEffectCh[i].x ,									//�\�����WX
				this->m_lpEffectCh[i].y - comeCameraY ,					//�\�����WY
				this->m_lpEffectCh[i].w ,									//��
				this->m_lpEffectCh[i].h ,									//����
				this->m_lpRunnerEffectImg[0] ,							//�摜�̔ԍ�
				this->m_lpEffectCh[i].picx , this->m_lpEffectCh[i].picy , 	//�\�������WX�E�x
				this->m_lpEffectCh[i].scale );							//�g�k��
		}
	}

	//�G�t�F�N�g���ʒ��摜�̕\��
	for (int i = 0; i < RUNNER_EFFECT_SIGN_KIND_OF_TYPE; i++) {

		//�����t���Otrue�Ȃ�
		if (this->m_lpPowerCh[i].flag) {
			if (this->m_lpPowerCh[i].time % 2 == 0) { //if present effect is invincible, count time get less than 125 to flickers

				//���ʔ������̃G�t�F�N�g
				DDScaleBlt(
					this->m_lpPowerCh[i].x,									//�\�����WX
					this->m_lpPowerCh[i].y - comeCameraY,					//�\�����WY
					this->m_lpPowerCh[i].w,									//��
					this->m_lpPowerCh[i].h,									//����
					this->m_lpRunnerEffectImg[1],							//�摜�̔ԍ�
					this->m_lpPowerCh[i].picx, this->m_lpPowerCh[i].picy,	//�\�������WX�E�x
					this->m_lpPowerCh[i].scale);							//�g�k��
			}
		}
	}

}


/*
**************************************************************************************************
�G�t�F�N�g�̉��
**************************************************************************************************
*/
void	CRunnerEffect::ReleaseRunnerEffect(){

	//�g�p�����������̉��
	if( this->m_lpEffectAni != NULL ){
		delete []this->m_lpEffectAni;
		this->m_lpEffectAni = NULL;
	}
	if( this->m_lpPowerAni != NULL ){
		delete []this->m_lpPowerAni;
		this->m_lpPowerAni = NULL;
	}
	if( this->m_lpEffectCh != NULL ){
		delete []this->m_lpEffectCh;
		this->m_lpEffectCh = NULL;
	}
	if( this->m_lpPowerCh != NULL ){
		delete []this->m_lpPowerCh;
		this->m_lpPowerCh = NULL;
	}
	if( this->m_lpRunnerEffectImg != NULL ){
		delete []this->m_lpRunnerEffectImg;
		this->m_lpRunnerEffectImg = NULL;
	}

}


//************************************************************************************************
//�摜�̗v�f��ݒ肷��
//************************************************************************************************
void	CRunnerEffect::SetRunnerEffectImage( int* lpimage , int max ){

	//�����̍ő吔�����摜�̗v�f���m�ۂ���
	this->m_lpRunnerEffectImg = new int [ max ];

	//�����̍ő吔�����摜�̗v�f��ݒ肷��
	for( int i = 0 ; i < max ; i++ ){ this->m_lpRunnerEffectImg[i] = lpimage[i]; }
	
}
