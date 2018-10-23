#ifndef  PLAYERRUNNER_H__
#define  PLAYERRUNNER_H__


//���w�b�_�[�t�@�C���̓ǂݍ���
#include "CRunner.h"
#include "score.h"

//�� �O���錾
class	CEffectManager;			//�G�t�F�N�g�Ǘ��N���X
class	CRoadEnemyManager;		//��Q���Ǘ��N���X

//�v���C���[�����i�[�N���X
class	CPlayerRunner : public CRunner{

private:

	//�摜�̗v�f������
	int		*m_lpPlayerRunnerImg;
	//���ʉ�
	int		*m_lpPlayerRunnerEffectSound;

	//�摜�ݒ�p
	//�̗�
	RUNNER	*m_lpPlayerStatus;

	//��Q���Ǘ��N���X
	CRoadEnemyManager	*m_lpCRoadEnemyForPlayer;

	//���擾�p
	static RUNNER	m_GetRunner;
	
	//for draw chain
	SCOREVALUE	m_PlayerScore;

protected:

public:

	//�� �ÓI�����o�ϐ��̒�`
	static const HWND	m_hWnd;
	//�摜
	static const int PLAYER_RUNNER_IMAGE_MAX;		//�g�p����

	//��
	static const int PLAYER_RUNNER_EFFECT_SOUND_MAX;//�g�p��

	//�X�e�[�^�X���
	//�̗�
	static const int PLAYER_RUNNER_STAMINA_MAX;	


	CPlayerRunner();
	~CPlayerRunner();
	//�� ����֐��̒�`
	//�I�[�o�[���C�h
	//�����i�[�̏�����
	void    InitRunner( int runnerNum , CRoadEnemyManager	*lpRoadEnemy , float roadDistance );
	//�����i�[�̍X�V
	void    UpdateRunner( int runner , CEffectManager *lpCEffect );
	//�\������֐�
	void    DrawRunner();
	//������s���֐�
	void    ReleaseRunner();

	//�v���C���[�����i�[�̑��x�̍X�V
	void    UpdatePlayerRunnerSpeed();
	//�v���C���[�����i�[�̈ړ��̍X�V
	void    UpdatePlayerRunnerMove();
	//�v���C���[�����i�[�̃W�����v�����̍X�V
	void    UpdateActionPlayerRunnerJump();
	//�v���C���[�����i�[�̍U�������̍X�V
	void    UpdateActionPlayerRunnerAttack();
	//�v���C���[�����i�[�̃A�j���[�V�������X�V
	void    ChangePlayerRunnerAnimation();
	//�ړ��𐧌�����
	void    ConstrainPlayerRunnerMove();
	//�����̃L�����N�^�[�̍��G�͈͂Ŕ�������
	bool	 BroadenCollisionSquarePlayer( CHARACTER* pch );
	/*************************************************************************************************
	�n�[�h�����щz�����ۂ̃v���C���[�̋������ʂR�u�񕜁v
	�������֐�
	**************************************************************************************************
	*/
	void    RunnerEffectHealing();

	//�� �ݒ�p�֐�
	//���g�̏���ݒ�
	void	SetRunner( RUNNER* lpRunner ) { this->m_RunnerDefault = *( lpRunner ); }

	//�� �擾�p�֐�
	//���g�̏���Ԃ�
	RUNNER	*GetRunner() const { return &this->m_GetRunner; }
	
};


#endif