#ifndef COFFROADSCOREMANAGER_H__
#define COFFROADSCOREMANAGER_H__


//���w�b�_�[���C���N���[�h
#include	"CCharacter.h"
#include	<windows.h>


//�񋓌^
//�X�R�A�̐F������
enum tagSCOREMANAGERCOLORTYPE {
	SCOREMANAGER_COLOR_BLACK,
	SCOREMANAGER_COLOR_RED,
	SCOREMANAGER_KIND_OF_COLOR,
};

//���\���̂̒�`
//���l���o�p
typedef struct tagSCOREDIRECT{

	int	indicateNum;
	int	restNum;
	int	terminateNum;
	int intervalFram;

} SCOREDIRECT;


//�� �O����`
class	COffRoadScore;
class	COffRoadRacer;
class	COffRoadAction;

//�� �I�t���[�h�X�R�A�N���X�̒�`
class COffRoadScoreManager{

private:

	//�摜�̗v�f������
	int		*m_lpOffRoadScoreImg;

	//�X�R�A�N���X
	COffRoadScore	*m_lpOffRoadScoreImage;

	//�����摜�p
	COffRoadScore	*m_lpOffRoadScoreNumber;

	//���C�_�[�N���X
	COffRoadRacer	*m_lpCOffRoadRacer;

	//�A�N�V�����N���X
	COffRoadAction	*m_lpCOffRoadAction;

	//�X�R�A�̕\������W������
	TPOINT<float>	m_ScoreArriveCoordinate;

	//�X�R�A�̏o���\��������
	TPOINT<float>	m_ScoreLeaveCoordinate;

	//�X�R�A�̕\����ł̑ҋ@���Ԃ�����
	int		m_ScoreStayTime;

	//�����摜�̕\�����鐔�l������
	int		m_DisplayNumber;

	//�v���C���[�̑O��s�����A�N�V�����r�b�g�ڂ�����
	BYTE	m_CommandActionOld;

	//�O��\�������]���摜�̗v�f������
	int		m_ValueTypeNext;

	//�����摜���o�p�\����
	SCOREDIRECT		*m_lpScoreDirect;

protected:
	
public:

	//�� �ÓI�����o�ϐ��̒�`
	static const int OFFROADSCOREMANAGER_SCORE_IMAGE_MAX;		//�摜�g�p����

	//�e�평���l
	//�@�A�N�V�����������̉摜
	//�X�R�A�̃T�C�Y
	static const int OFFROADSCOREMANAGER_SCORE_IMAGE_WIDTH;	//��
	static const int OFFROADSCOREMANAGER_SCORE_IMAGE_HEIGHT;//����
	//Chain
	static const int OFFROADSCOREMANAGER_SCORE_IMAGE_CHAIN_WIDTH;
	//EXCELLENT�̃T�C�Y
	static const int	OFFROADSCOREMANAGER_SCORE_IMAGE_EXCELLENT_WIDTH;

	//�g�k�{��
	static const float OFFROADSCOREMANAGER_DEFAULT_SCORE_IMAGE_SCALE;	
	//�X�R�A�̈ړ���
	static const float	OFFROADSCOREMANAGER_SCORE_MOVE_X;	//X
	static const float	OFFROADSCOREMANAGER_SCORE_MOVE_Y;	//Y
	//�X�R�A�̑ҋ@����
	static const int	OFFROADSCOREMANAGER_SCORE_STAY_TIME_MAX;

	//�A�����摜�\���p
	//�X�R�A�̃T�C�Y
	static const int OFFROADSCOREMANAGER_SCORE_NUMBER_IMAGE_WIDTH;	//��
	static const int OFFROADSCOREMANAGER_SCORE_NUMBER_IMAGE_HEIGHT;	//����
	//�g�k�{��
	static const float OFFROADSCOREMANAGER_DEFAULT_SCORE_NUMBER_IMAGE_SCALE;
	//��ސ�
	static const char	OFFROADSCOREMANAGER_SCORE_NUMBER_KIND_OF_TYPE;
	//�� �����摜���o�p
	//���o�����������X�R�A�̑���
	static const char	OFFROADSCOREMANAGER_SCORE_NUMBER_DIRECTION_MAX;
	//���Z�E���Z���o���̃t���[����
	static const int	OFFROADSCOREMANAGER_SCORE_NUMBER_DEFAULT_DIRECTION_INTERVAL_FRAM;



	//�X�R�A�̎��
	//SUCCEED
	static const char	OFFROADSCOREMANAGER_SCORE_TYPE_SUCCEED;
	//SCORE
	static const char	OFFROADSCOREMANAGER_SCORE_TYPE_SCORE;
	//GOOD
	static const char	OFFROADSCOREMANAGER_SCORE_TYPE_GOOD;
	//COOL
	static const char	OFFROADSCOREMANAGER_SCORE_TYPE_COOL;
	//EXCELLENT
	static const char	OFFROADSCOREMANAGER_SCORE_TYPE_EXCELLENT;

	//��ސ�
	static const char	OFFROADSCOREMANAGER_SCORE_IMAGE_KIND_OF_TYPE;

	//�E�B���h�E�n���h��
	static HWND	m_hWnd;


	//���v���g�^�C�v�錾
	//�R���X�g���N�^
	COffRoadScoreManager();
	//�f�B�X�g���N�^
	~COffRoadScoreManager();
	
	void InitScoreManager();					//������
	void UpdateScoreManagerSucceedAction();		//���������A�N�V�������ɉ������]���摜�̍X�V
	void DrawScoreManager();					//�\��
	void ReleaseScoreManager();					//���
	/*
	�����摜�\��
	*/
	void DrawScoreNumber( TPOINT<float> pos , int number , int digit, tagSCOREMANAGERCOLORTYPE color, float addScale);
	/*
	�ڕW�̐��l�܂ŏ��X�ɉ��Z�i���Z�j����
	*/
	int GraduallyNumber( SCOREDIRECT* pScore );



	
};


#endif