#ifndef		CEFFECTMANAGER_H__
#define		CEFFECTMANAGER_H__


//�w�b�_�[�̃C���N���[�h
#include	"graphics.h"
#include	"CRunner.h"

//�� �O���錾
class CRunnerEffect;

//�� �����i�[�G�t�F�N�g�N���X
class CEffectManager{

private:

	//�摜�̗v�f��
	int		*m_lpEffectManagerImg;

	//�G�t�F�N�g�N���X
	CRunnerEffect	*m_lpCEffect;

	//�G�t�F�N�g�������҂̑���
	int		m_EffectUserMax;


protected:

public:

	//�� �ÓI�����o�̒�`
	//�E�B���h�E�n���h��
	static HWND m_hWnd;
	//�摜�̍ő喇��
	static const int EFFECTMANAGER_PICTURE_MAX;
	//�G�t�F�N�g�̍ő唭����
	static const int EFFECTMANAGER_CREATE_EFFECT_MAX;


	//�� ����֐��̒�`
	//�R���X�g���N�^
	CEffectManager();
	//�f�B�X�g���N�^
	~CEffectManager();
	//������
	void	InitEffectManager( int runnerMax );	
	//�X�V
	void	UpdateEffectManager( RUNNER *lpRunner , int runner );					
	//�\��
	void	DrawEffectManager( int runner );							
	//���
	void	ReleaseEffectManager();	
	//�����������L�����N�^�[�p�ɃG�t�F�N�g�𐶐�����
	void	CreateEffectManaager( RUNNER *lpRunner , int runner );
	//���ʂ𖳌��ɂ���G�t�F�N�g�̊Ǘ�
	void	RepealEffectManager( unsigned char type , int runner );


};


#endif