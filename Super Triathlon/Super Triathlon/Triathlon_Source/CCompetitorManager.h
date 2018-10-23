#ifndef		CCOMPETITORMANAGER_H__
#define		CCOMPETITORMANAGER_H__

//���w�b�_�[���C���N���[�h
#include	"graphics.h"
#include	"CCharacter.h"

//�� �O���錾
class	CCompetitor;
class	CEffectManager;
class	CRoadEnemyManager;

//�� �����҃N���X
class	CCompetitorManager{

private:
	
	//�����҃N���X
	static CCompetitor		*m_lpCCompetitorRunner;

	//�G�t�F�N�g�Ǘ��N���X
	CEffectManager	*m_lpCEffect;

	//�摜�̗v�f
	int		*m_lpCompetitorImg;

	//���ʉ��̗v�f
	int		*m_lpCompetitorEffectSound;

	//�����҂̐�
	static char	m_CompetitorMax;

	//�e�����҂̍��W������
	static TPOINT<float>	*m_lpCompetitorPos;

	//�e��Q���̌��ʂ𔭊�������Q���̗v�f
	static int		*m_lpCompetitorBumpObstacleNow;


protected:

public:

	//�� �ÓI�����o�ϐ��̒�`
	static const int	COMPETITORMANAGER_IMAGE_MAX;		//�g�p�摜����
	static const int	COMPETITORMANAGER_EFFECT_SOUND_MAX;	//�g�p���ʉ���
	static const HWND	m_hWnd;


	CCompetitorManager();
	~CCompetitorManager();

	//���v���g�^�C�v�錾
	//�����҂̏�����
	void	InitCompetitorManager( int runnerMax , CRoadEnemyManager	*lpRoadEnemy , float roadDistance );
	//�����҂̍X�V
	void	UpdateCompetitorManager();
	//�����҂̕\��
	void	DrawCompetitorManager();
	//�����҂̉��
	void	ReleaseCompetitorManager();
	
	//�e��擾�p
	//�e�����҂̍��W��Ԃ�
	TPOINT<float>*	GetCompetitorManagerPosition() const { return this->m_lpCompetitorPos; }
	//���ʂ𔭊�������Q���̗v�f���擾����
	int*	GetCompetitorManagerObstacleMaterial() const { return this->m_lpCompetitorBumpObstacleNow; }
	//�����҂̍ő吔
	int		GetCompetitorMax() const { return this->m_CompetitorMax; }

	//�e��ݒ�p
	//��Q���̗v�f
	void	SetCompetitorManagerObstacleMaterial( int material , int runner );

};


#endif
