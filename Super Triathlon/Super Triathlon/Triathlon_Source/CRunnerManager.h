#ifndef		CRUNNERMANAGER_H__
#define		CRUNNERMANAGER_H__

//���w�b�_�[���C���N���[�h
#include	"graphics.h"
#include	"CCharacter.h"

//�� �O���錾
class	CCompetitorManager;
class	CRoadEnemyManager;
class	CPlayerRunner;
class	CEffectManager;
class	CRoadStage;

//�� �����҃N���X
class	CRunnerManager{

private:
	
		
	//�摜�̗v�f��
	int		*m_lpRunnerManagerImg;

	//�����҂̐�
	int		m_CompetitorMax;

	//�v���C���[�̌��݂̏���
	static int		m_PlayerRanking;

	//�X�e�[�W�̑S��
	float	m_StageWholeDistance;

	//���ʐݒ�p
	CHARACTER	m_RunnerRanking;
	//�~�j�}�b�v�p
	CHARACTER	*m_lpMiniMap;

	//�����҂̊Ǘ��N���X
	static CCompetitorManager	*m_lpCCompetitorManager;
	//�v���C���[�N���X
	static CPlayerRunner		*m_lpCPlayerRunner;
	//�G�t�F�N�g�Ǘ��N���X
	CEffectManager				*m_lpCEffectManager;
	//�X�e�[�W�N���X
	CRoadStage					*m_lpCRoadStage;


protected:

public:

	//�� �ÓI�����o�ϐ��̒�`
	static const HWND	m_hWnd;
	//�摜�̗v�f��
	static const int RUNNERMANAGER_IMAGE_MAX;

	//�����҂̍ő吔
	static const int RUNNERMANAGER_COMPETITOR_MAX;

	//�v���C���[�̏���
	static const float RUNNERMANAGER_RANKING_INDICATE_X;		//�\����X
	static const float RUNNERMANAGER_RANKING_INDICATE_Y;		//���\����Y
	static const int RUNNERMANAGER_RANKING_IMAGE_WIDTH;			//��
	static const int RUNNERMANAGER_RANKING_IMAGE_HEIGHT;		//����
	static const float RUNNERMANAGER_RANKING_DEFAULT_SCALE;		//�g�k�{��

	//�~�j�}�b�v
	static const int RUNNERMANAGER_MINIMAP_IMAGE_MAX;			//�摜�̐�
	static const float RUNNERMANAGER_MINIMAP_INDICATE_X;		//�\����X
	static const float RUNNERMANAGER_MINIMAP_INDICATE_Y;		//�\����Y
	static const int RUNNERMANAGER_MINIMAP_IMAGE_WDITH;			//��
	static const int RUNNERMANAGER_MINIMAP_IMAGE_HEIGHT;		//����

	//�~�j�}�b�v��̃v���C���[
	static const int RUNNERMANAGER_MINIMAP_PLAYER_IMAGE_WDITH;		//��
	static const int RUNNERMANAGER_MINIMAP_PLAYER_IMAGE_HEIGHT;		//����
	static const float RUNNERMANAGER_MINIMAP_PLAYER_DEFAULT_SCALE;	//�g�k�{��




	
	CRunnerManager();
	~CRunnerManager();

	//���v���g�^�C�v�錾
	//�����҂̏�����
	void	InitRunnerManager( CRoadEnemyManager	*lpRoadEnemy );
	//�����҂̍X�V
	void	UpdateRunnerManager();
	//�����҂̕\��
	void	DrawRunnerManager();
	//�����҂̉��
	void	ReleaseRunnerManager();

	//�����҂̃A�h���X�擾
	CCompetitorManager	*GetCCompetitorManager() const { return this->m_lpCCompetitorManager; }
	//�v���C���[�̃A�h���X�擾
	CPlayerRunner	*GetCPlayerRunner() const { return this->m_lpCPlayerRunner; }
	//Get player's ranking in this race
	int		GetPlayerRunnerRanking() const { return this->m_PlayerRanking; }
	

};


#endif
