#ifndef		CROADENEMYMANAGER_H__
#define		CROADENEMYMANAGER_H__

//���w�b�_�[���C���N���[�h
#include	"graphics.h"
#include	"CRunner.h"

//�� �O���錾
class	CObstacle;
class	CRunnerManager;

//�� �����҃N���X
class	CRoadEnemyManager{

private:
	

	//�摜�̗v�f
	int		*m_lpRoadEnemyManagerImg;

	//���ʉ��̗v�f
	int		*m_lpRoadEnemyManagerEffectSound;

	//��Q���N���X
	CObstacle	*m_lpCObstacle;

	//��Q���ő吶����
	int		m_ObstacleCreateMax;


protected:

public:

	//�� �ÓI�����o�ϐ��̒�`
	static const int	ROADENEMYMANAGER_IMAGE_MAX;			//�g�p�摜����
	static const int	ROADENEMYMANAGER_EFFECT_SOUND_MAX;	//�g�p���ʉ���
	static const HWND	m_hWnd;


	CRoadEnemyManager();
	~CRoadEnemyManager();

	//���v���g�^�C�v�錾
	//�����҂̏�����
	void	InitRoadEnemyManager( CRunnerManager *lpRunnerManager );
	//�����҂̍X�V
	void	UpdateRoadEnemyManager();
	//�����҂̕\��
	void	DrawRoadEnemyManager();
	//�����҂̉��
	void	ReleaseRoadEnemyManager();
	/*
	*************************************************************************************************
	�����̃����i�[�Əd�Ȃ�̔�������
	*************************************************************************************************
	*/
	char	CollisionRoadEnemyManager( RUNNER *lpRunner );
	/*
	*************************************************************************************************
	�����̃L�����N�^�[�\���̂̍��G�͈͂Ŕ�������
	����   : �L�����N�^�[�\����
	�߂�l : ��Q���̎�ޔԍ���Ԃ�
	***********************************************************************************************
	*/
	BYTE	BroadenCollisionSquareToObstacle( CHARACTER *lpCh );


	

};


#endif
