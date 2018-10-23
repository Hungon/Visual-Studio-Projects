#ifndef RUNNER_EFFECT_H__
#define RUNNER_EFFECT_H__


enum tagRUNNEREFFECTKIND {
	RUNNER_EFFECT_TYPE_BLUE,			//���x�㏸
	RUNNER_EFFECT_TYPE_RED,				//���G
	RUNNER_EFFECT_TYPE_GREEN,			//��
	RUNNEREFFECT_KIND_OF_TYPE,			//���ʂ̎��
	RUNNER_EFFECT_SIGN_KIND_OF_TYPE = 2,	//���ʒ���m�点����
};

//�w�b�_�[�̃C���N���[�h
#include	"graphics.h"
#include	"animation.h"


//�� �O���錾
class CPlayerRunner;

//�� �����i�[�G�t�F�N�g�N���X
class CRunnerEffect{

private:

	//���ʔ������̃G�t�F�N�g�p
	CHARACTER	*m_lpEffectCh;
	ANIMATION	*m_lpEffectAni;
	//���ʒ��̃G�t�F�N�g�p
	CHARACTER	*m_lpPowerCh;
	ANIMATION	*m_lpPowerAni;
	//�摜�̗v�f��
	int		*m_lpRunnerEffectImg;
	//��������G�t�F�N�g�̍ő吔
	int		m_CreateEffectMax;
	//PlayerRunner's class
	CPlayerRunner	*m_lpCPlayer;

protected:

public:

	//�� �ÓI�����o�̒�`

	//�G�t�F�N�g�^�C�v���̉摜�ݒ�
	//�����O�^
	static const int RUNNER_EFFECT_TYPE_RING_IMAGE_HEIGHT;		//����
	static const int RUNNER_EFFECT_TYPE_RING_IMAGE_WIDTH;		//��
	static const float RUNNER_EFFECT_TYPE_RING_IMAGE_SCALE;		//�g�嗦
	//�g�嗦�̑��x
	static const float RUNNER_EFFECT_TYPE_RING_IMAGE_CONTRACTION_SPEED;

	//�{�[���^
	static const int RUNNER_EFFECT_TYPE_BALL_IMAGE_HEIGHT;		//����
	static const int RUNNER_EFFECT_TYPE_BALL_IMAGE_WIDTH;		//��
	static const float RUNNER_EFFECT_TYPE_BALL_IMAGE_SCALE;		//�g�嗦
	//�A�j���[�V�����ݒ�
	static const int RUNNER_EFFECT_TYPE_BALL_ANIMATION_MAX;		//�ő�R�}��
	static const int RUNNER_EFFECT_TYPE_BALL_ANIMATION_FRAM;	//�t���[����
	
																//�� ����֐��̒�`
	//�R���X�g���N�^
	CRunnerEffect();
	//�f�B�X�g���N�^
	~CRunnerEffect();
	//������
	void InitRunnerEffect( int effectMax );	
	//�X�V
	void UpdateRunnerEffect(float x, float y, int w, int h, int invincibleTime);
	//�\��
	void DrawRunnerEffect();							
	//���
	void ReleaseRunnerEffect();				
	/************************************************************************************************
	�G�t�F�N�g����
	*************************************************************************************************
	*/
	void CreateRunnerEffect( float x , float y , char type );
	/*
	***************************************************************************************************
	�����ɂ���G�t�F�N�g�^�C�v���󂯎��
	***************************************************************************************************
	*/
	void GetRunnerRepealEffectType( const unsigned char type ){	this->m_lpPowerCh[type].flag = false;	}

	//�摜�̗v�f��ݒ肷��
	void	SetRunnerEffectImage( int *lpimage , int max );

};


#endif