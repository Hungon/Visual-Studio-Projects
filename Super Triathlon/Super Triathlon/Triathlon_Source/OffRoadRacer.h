#ifndef OFFROADRACER_H__
#define OFFROADRACER_H__


//�� �w�b�_�[�̃C���N���[�h
#include	"graphics.h"
#include	"animation.h"
#include	<windows.h>
#include	"OffRoadAction.h"
#include	"OffRoadScoreManager.h"

//�񋓌^
//���[�T�[�̃X�R�A������
enum tagRACERSCORE {
	OFFROADRACER_SCORE_ACTION_POINT,					//�A�N�V�����|�C���g			
	OFFROADRACER_SCORE_ACTION_CONSECUTIVE_MAX,			//�A���Ő��������A�N�V�����ő吔
	OFFROADRACER_KIND_OF_SCORE,							//�X�R�A�̎�ސ�
};

//kind of racer's action
enum tagRACERTECHNIQUE {
	RACERTECHNIQUE_TYPE_NORMAL,					//run on the road
	RACERTECHNIQUE_TYPE_RIGHT_ROTATE,			//once turn right in the sky
	RACERTECHNIQUE_TYPE_LEFT_ROTATE,			//once turn left in the sky
	RACERTECHNIQUE_TYPE_RIGHT_HALF_ROTATE,		//half turn right in the sky
	RACERTECHNIQUE_TYPE_LEFT_HALF_ROTATE,		//half trun left in the sky
	RACERTECHNIQUE_KIND_OF_TYPE,				//kind of technique
};
//racer's sizes
const int OFFROADRACER_IMAGE_RACER_WIDTH = 32;
const int OFFROADRACER_IMAGE_RACER_HEIGHT = 64;	
//set animation's value
//for normal style
const int OFFROADRACER_ANIMATION_TYPE_NORMAL_CNT_MAX = 2;			//max count
const int OFFROADRACER_ANIMATION_TYPE_NORMAL_FRAM = 5;				//fram speed
//for once rotate
const int OFFROADRACER_ANIMATION_ROTATE_IMAGE_WIDTH = 64;			//change width
const int OFFROADRACER_ANIMATION_TYPE_ROTATE_CNT_MAX = 19;			//count max
const int OFFROADRACER_ANIMATION_TYPE_ROTATE_FRAM = 2;				//fram speed
//for half rotate
const int OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_CNT_MAX = 5;		//count max
const int OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_FRAM = 2;			//fram speed


//�� �O���錾
class	COffRoadStage;			//�X�e�[�W�N���X
class   COffRoadScoreManager;	//�X�R�A�Ǘ��N���X

//���I�t���[�h�v���C���[�N���X�̒�`
class COffRoadRacer : public COffRoadAction {

private:

	//�摜�̗v�f������
	int		*m_lpCOffRoadRacerImg;
	
	//���ʉ��̗v�f������
	int		*m_lpOffRoadRacerSe;

	//�X�R�A�Ǘ��N���X
	COffRoadScoreManager	*m_lpScoreManager;

	//�摜�ݒ�p
	CHARACTER		m_OffRoadRacerCh;

	//�A�j���[�V�����ݒ�p
	ANIMATION		m_OffRoadRacerAni;

	//�X�e�[�W�̃N���X
	COffRoadStage	*m_lpCOffRoadStage;

	//���C�_�[�̑��x�̍��v������
	static float	m_AggregateSpeed;

	//���C�_�[�̌��݂̍��W���擾
	static TPOINT<float>	m_RiderPosition;

	//���������A�N�V�����R�}���h�̃r�b�g�ڂ�����
	static BYTE	m_CommandSuccess;

	//���s�����A�N�V�����R�}���h�̃r�b�g�ڂ�����
	BYTE	m_DidCommandAction;

	//�v���C���[�̃A�N�V�����t���O
	static bool	m_actionFlag;

	//�A�N�V�������s���̊g�k�{���̉��Z�i���Z�j�l������
	float	m_actionScaleAdd;
	float	m_actionScaleSub;

	//process for scale up score number image
	float	*m_lpScoreScaleAdd;			//add scale rate to score image for racer's action
	int		*m_lpNumberAfterScaleUp;	//treat after scale up number

	//�A�N�V�����|�C���g
	static int		m_ActionPoint;
	//�A�N�V�����|�C���g�̕\������W
	static TPOINT<float>	m_IndicateActionPoint;
	
	//���������A�N�V�����̐����������߂�
	static int	m_SuccessActionCount[OFFROADACTION_COMMAND_KIND_OF_ACTION];
	//�A���ŃA�N�V��������������������
	static int	m_SucceedActoinConsecutive;
	//�A���ŃA�N�V�������������ő吔������
	static int	m_SucceedActoinConsecutiveMax;
	//�A���Ő��������A�N�V�������̕\������W
	static TPOINT<float>	m_IndicateActionConsecutive;

	//count time for scale up score image
	int		*m_lpScoreScaleUpCountTime;

protected:


public:

	//�� �ÓI�����o�ϐ��̒�`
	static const int OFFROADRACER_IMAGE_MAX;			//�摜�g�p����
	static const int OFFROADRACER_EFFECT_SOUND_MAX;		//���ʉ��g�p��


	//�E�B���h�E�n���h��
	static HWND	m_hWnd;

	//�e�평���l
	//�\�����W�i���[�J�����W�j
	static const float OFFFROADRACER_DEFAULT_POSITION_X;		//�\�����WX
	static const float OFFFROADRACER_DEFAULT_POSITION_Y;		//�\�����WY
	//�ړ���
	static const float OFFFROADRACER_DEFAULT_MOVE_UP;			//�O��
	static const float OFFFROADRACER_DEFAULT_MOVE_RIGHT;		//�E
	static const float OFFFROADRACER_DEFAULT_MOVE_BACK;			//���
	static const float OFFFROADRACER_DEFAULT_MOVE_LEFT;			//��
	//���x�{��
	static const float	OFFROADRACER_DEFAULT_SPEED_RATE;
	//�摜�T�C�Y
	static const int OFFROADRACER_IMAGE_WIDTH;					//��
	static const int OFFROADRACER_IMAGE_HEIGHT;					//����
	//�g�k�{��
	static const float OFFFROADRACER_DEFAULT_SCALE;
	//�g�k�{���̕ϐ��l
	//���Z�l
	static const float	OFFROADRACER_VARIABLE_SCALE_RATE_ADD_EASY;
	static const float	OFFROADRACER_VARIABLE_SCALE_RATE_ADD_NORMAL;
	static const float	OFFROADRACER_VARIABLE_SCALE_RATE_ADD_HARD;
	//���Z�l
	static const float	OFFROADRACER_VARIABLE_SCALE_RATE_SUB_EASY;
	static const float	OFFROADRACER_VARIABLE_SCALE_RATE_SUB_NORMAL;
	static const float	OFFROADRACER_VARIABLE_SCALE_RATE_SUB_HARD;


	//�A�j���[�V�����ݒ�
	//�ʏ펞
	static const int OFFROADRACER_ANIMATION_TYPE_NORMAL_CNT_MAX;	//�ő�R�}��
	static const int OFFROADRACER_ANIMATION_TYPE_NORMAL_FRAM;		//�t���[��
	
	//��]�A�j���[�V������
	static const int OFFROADRACER_ANIMATION_ROTATE_IMAGE_WIDTH;		//��
	static const int OFFROADRACER_ANIMATION_TYPE_ROTATE_CNT_MAX;	//�ő�R�}��
	static const int OFFROADRACER_ANIMATION_TYPE_ROTATE_FRAM;		//�t���[��

	//����]�A�j���[�V������
	static const int OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_CNT_MAX;//�ő�R�}��
	static const int OFFROADRACER_ANIMATION_TYPE_HALF_ROTATE_FRAM;	//�t���[��

	//�A�j���[�V�������̃r�b�g��
	//��]�A�j���[�V����
	static const BYTE OFFROADRACER_ANIMATION_TYPE_NORMAL;			//�ʏ펞
	static const BYTE OFFROADRACER_ANIMATION_TYPE_RIGHT_HALF_ROTATE;//�E����]
	static const BYTE OFFROADRACER_ANIMATION_TYPE_LEFT_HALF_ROTATE;	//������]
	static const BYTE OFFROADRACER_ANIMATION_TYPE_RIGHT_ROTATE;		//�E1��]
	static const BYTE OFFROADRACER_ANIMATION_TYPE_LEFT_ROTATE;		//��1��]
	//�A�j���[�V�������̕\�������WY
	static const int OFFROADRACER_ANIMATION_TYPE_NORMAL_PICY;				//�ʏ펞
	static const int OFFROADRACER_ANIMATION_TYPE_RIGHT_HALF_ROTATE_PICY;	//�E����]
	static const int OFFROADRACER_ANIMATION_TYPE_LEFT_HALF_ROTATE_PICY;	//������]
	static const int OFFROADRACER_ANIMATION_TYPE_RIGHT_ROTATE_PICY;		//�E1��]
	static const int OFFROADRACER_ANIMATION_TYPE_LEFT_ROTATE_PICY;			//��1��]

	//�A�j���[�V�����̎��
	static const int OFFROADRACER_ANIMATION_KIND_OF_TYPE;

	//�A�N�V�������̃v���C���[�̍ő�g�k�{��
	static const float OFFROADRACER_ACTION_SCALE_MAX;

	//�A�N�V�����|�C���g�̕\������
	static const int	OFFROADRACER_ACTION_POINT_DIGIT;
	//�A�N�V�����R�}���h�ő���͐�
	static const int OFFROADRACER_COMMAND_INPUT_MAX;
	//�A�N�V�����R�}���h�������̑��x�ω��l
	static const float OFFROADRACER_SUCCEED_ACTION_VARIABLE_SPEED_RATE;

	//process for scale of total number score image
	//default add scale rate
	static const float OFFROADRACER_TOTAL_ACTION_POINT_ADD_SCALE_RATE;
	//add scale max
	static const float OFFROADRACER_TOTAL_ACTION_POINT_ADD_SCALE_RATE_MAX;
	//starting point of scale up
	static const int OFFROADRACER_TOTAL_ACTION_POINT_STARTING_POINT_SCALE_UP;

	//process for scale of action consecutive number score image
	//default add scale rate
	static const float OFFROADRACER_CONSECUTIVE_ACTION_ADD_SCALE_RATE;
	//add scale max
	static const float OFFROADRACER_CONSECUTIVE_ACTION_ADD_SCALE_RATE_MAX;
	//scale up consecutive success count
	static const int OFFROADRACER_CONSECUTIVE_ACTION_SCALE_UP_NUMBER_SUCCESS_COUNT;
	//limit time for scale up of score image
	static const int OFFROADRACER_LIMIT_TIME_FOR_SCALE_UP_SCORE_IMAGE;

	//blank time that before player can input
	static const int OFFROADRACER_BLANK_TIME_FOR_INPUT;


	//�R���X�g���N�^
	COffRoadRacer() :	
		//�摜�̗v�f������
		m_lpCOffRoadRacerImg(NULL) ,
		//�X�e�[�W�̃N���X
		m_lpCOffRoadStage(NULL)	   ,
		m_actionScaleAdd(NULL)	   ,
		m_actionScaleSub(NULL)	   
	{}

	//�f�B�X�g���N�^
	~COffRoadRacer();
	
	//�� ���상���o�֐��̒�`
	//�I�t���[�h�v���C���[�̏�����
	void InitOffRoadRacer();
	//���H�̍X�V
	void UpdateOffRoadRacer();
	//�I�t���[�h�v���C���[�̕\��
	void DrawOffRoadRacer();
	//���
	void ReleaseOffRoadRacer();
	//�ړ�
	void MoveOffRoadRacer();
	//�ړ��͈͂𐧌�����
	void ConstrainOffRoadRacerMove();
	//���C�_�[�̃A�j���[�V�������X�V
	void ChangeAnimationOffRoadRacer();

	/*
	���C�_�[�̉�]�A�N�V�����̐ݒ�
	*/
	void SetRotateActionOffRoadRacer();
	/*
	���C�_�[�̉�]�A�N�V�����̍X�V
	*/
	void UpdateRotateActionOffRoadRacer();
	/*
	Starting point of scale up for number image
	*/
	void	RacerStartingPointScaleUpScoreNumber();



	
	//�擾�p
	//���C�_�[�̍��W���擾
	TPOINT<float>	GetRiderPosition() const { return this->m_RiderPosition; }
	//���C�_�[�̑��x�擾�p
	float GetOffRoadRacerSpeed() const { return this->m_AggregateSpeed; }
	//���C�_�[�̓��͏��擾�p
	BYTE	GetOffRoadRacerCommandInfo() const { return this->m_CommandSuccess; }
	//�A�N�V�����|�C���g�\�����W
	TPOINT<float>	GetActionPointIndicate() const { return this->m_IndicateActionPoint; }
	//�A�N�V�����|�C���g�擾�p
	int		GetTotalActionPoint() const { return this->m_ActionPoint; }
	//���������A�N�V�����̐��擾�p
	int*	GetSuccessActionCount() const { return this->m_SuccessActionCount; }
	//�A���A�N�V�������������擾
	int		GetScceedActionConsecutive() const { return this->m_SucceedActoinConsecutiveMax; }
	//�A���Ő��������A�N�V�������\������W��Ԃ�
	TPOINT<float>	GetOffRoadRacerIndicateConsecutiveAction() const { return this->m_IndicateActionConsecutive; }
	//�A�N�V����������Ԃ�
	bool	GetRacerActionFlag() const { return this->m_actionFlag; }

	//�ݒ�p
	void SetOffRoadRacerSpeed( float AggregateSpeed ) { this->m_AggregateSpeed = AggregateSpeed; }
	//���C�_�[�̓��͏��擾�p
	void	SetOffRoadRacerCommandInfo( BYTE CommandSuccess );


};


#endif
