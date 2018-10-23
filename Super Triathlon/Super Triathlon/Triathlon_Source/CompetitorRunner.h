#ifndef		COMPETITORRUNNER_H__
#define		COMPETITORRUNNER_H__


//���}�N���̒�`
//�s���p�^�[���̎�ސ�
enum tagCOMPETITORACTIONTYPE{

	COMPETITOR_RUNNER_ACTION_TYPE_VACANT = -1 ,			//�A�N�V�����Ȃ�
	COMPETITOR_RUNNER_ACTION_TYPE_JUMP		  ,			//�W�����v
	COMPETITOR_RUNNER_ACTION_TYPE_ATTACK	  ,			//�U��
	COMPETITOR_RUNNER_ACTION_TYPE_EVADE		  ,			//���
	COMPETITOR_RUNNER_KIND_OF_ACTION		  ,			//�A�N�V�����̎��

};

//���w�b�_�[���C���N���[�h
#include	"CRunner.h"
#include	"CCharacter.h"

//���@�O���錾
class	CEffectManager;
class	CRoadEnemyManager;
class	CPlayerRunner;

//�� �����҃N���X
class	CCompetitor : public CRunner{

private:

	//�摜�̗v�f�ԍ�
	int		m_CompetitorRunnerImg;

	//�e�p�̉摜�v�f
	int		m_CompetitorShadowImg;

	//���ʉ��̗v�f
	int		m_CompetitorEffectSound;

	//�e��Q���ɑ΂���A�N�V����������
	int		*m_lpActionRateToHurdle;	//�n�[�h��
	int		*m_lpActionRateToRock;		//���

	//���݂̎��g�̈ʒu�ƃv���C���[�̈ʒu�ɉ��������x�̕ω�
	float	m_VariableSpeedNow;
	//���f�����鑬�x�ω��l
	float	m_GetVariableSpeed;

	//���x����
	float	m_ConstarinSpeedMax;

	//�����ҊǗ��N���X
	CRoadEnemyManager	*m_lpCRoadEnemy;
	//�v���C���[�N���X
	CPlayerRunner		*m_lpCPlayerRunner;

	//��щz������Q���̗v�f�������i�擾�p�j
	static int m_getJumpedMaterial;


protected:

public:

	//���W�擾�p
	static TPOINT<float>	m_CompetitorPos;

	//�� �ÓI�����o�ϐ��̒�`
	//�G�t�F�N�g���ʂ̎��
	static const int	COMPETITOR_KIND_OF_EFFECT;
	//�\���Ԋu
	static const int	COMPETITOR_INDICATE_SPACE_X;

	//��ʊO�ɏo���ۂ̐������W
	static const float	COMPETITOR_CONSTRAIN_FRONT_POSITION_Y;			//�O��
	static const float	COMPETITOR_CONSTRAIN_BACK_POSITION_Y;			//���

	//�R���X�g���N�^
	CCompetitor();
	//�f�B�X�g���N�^
	~CCompetitor();

	//���v���g�^�C�v�錾
	//�I�[�o�[���C�h
	//�����҂̏�����
	void InitRunner( int runnerNum , CRoadEnemyManager	*lpRoadEnemy , float roadDistance );
	//�����҂̍X�V
	void UpdateRunner( int runner , CEffectManager *lpCEffect );
	//�����҂̕\��
	void DrawRunner();
	//�����҂̉��
	void ReleaseRunner();

	//�����҂̑��x���Ǘ�����
	void ManageCompetitorRunnerSpeed();
	//�����҂̈ړ��͈͂𐧌�����
	void ManageConstrainCompetitorRunnerMove();
	/*
	��Q���̎�ނɉ����ēK�؂ȍs�����m���Ŕ��f����
	*/
	void SetActionToObstacle();
	//�s���p�^�[���P�̏�����
	void SetCompetitorRunnerLineMove();
	//�s���p�^�[���Q�̏�����
	void SetCompetitorRunnerEvadeHardol();
	//�s���p�^�[���R�̏�����
	void SetCompetitorRunnerAttack();
	//�s���p�^�[���P�̉��ɉ��
	void UpdateCompetitorRunnerLineMove();
	//�s���p�^�[���Q�̉���s��
	void UpdateCompetitorRunnerEvadeHardol();
	//�s���p�^�[���R�̍U���̍X�V
	void UpdateCompetitorRunnerAttack();
	//�����̃L�����N�^�[�\���̂̍��G�͈͂ƂŔ�������
	bool BroadenCollisionSquareCompetitor( CHARACTER* pch );
	//�A�j���[�V�����̍X�V
	void ChangeCompetitorRunnerAnimation();
	/*****************************************************************************************************
	�����̒l�Ŋm���ōs�������߂�
	�����P�F�m���@1�`100
	�����Q�F�s�������A�N�V�����̎�ޔԍ�

	�߂�l�F�A�N�V�����̎�ޔԍ�
	�����s������0��Ԃ�
	******************************************************************************************************/
	char ConmpetitorAttemptDoAction( char likelihood , BYTE actionType );

	/*
	********************************************************************************
	�W�����v�Ŕ�щz������Q���̗v�f��ݒ肷��
	********************************************************************************
	*/
	void	SetCompetitorJumpedObstacleMaterial( int material ){ this->m_RunnerDefault.obstacleBumpNow = material; }

	//�摜�v�f�ݒ�p
	//�����җp
	void	SetCompetitorImage( int image ) { this->m_CompetitorRunnerImg = image; }
	//�����҂̉e�p
	void	SetCompetitorShadowImage( int image ) { this->m_CompetitorShadowImg = image; }
	//�����҂̌��ʉ�
	void	SetCompetitorEffectSound( int SE ) { this->m_CompetitorEffectSound = SE; }


	//�e��擾�p
	/*
	********************************************************************************
	�W�����v�Ŕ�щz������Q���̗v�f���擾����
	********************************************************************************
	*/
	int			GetCompetitorJumpedObstacleMaterial(){ return this->m_getJumpedMaterial; }
	
	//���W
	TPOINT<float>	GetOneCompetitorPosition() const { return this->m_CompetitorPos; }



};


#endif
