#ifndef OFFROADACTION_H__
#define OFFROADACTION_H__


//�� �w�b�_�[�̃C���N���[�h
#include	"graphics.h"
#include	"CCharacter.h"
#include	<windows.h>

//�� �񋓌^
//�R�}���h�̎��
enum	tagCOMMANDACTIONTYPE{

	OFFROADACTION_COMMAND_TYPE_VACANT = -1 ,	//��ނȂ�
	OFFROADACTION_COMMAND_TYPE_LEFT = 37 ,		//������
	OFFROADACTION_COMMAND_TYPE_UP		,		//�����
	OFFROADACTION_COMMAND_TYPE_RIGHT	,		//�E����
	OFFROADACTION_COMMAND_TYPE_DOWN		,		//������
	//�R�}���h�̎�ސ�
	OFFROADACTION_COMMAND_KIND_OF_TYPE = 4,
};

//�R�}���h�̃r�b�g��
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_VACANT		 = 0x00;		//��ނȂ�
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_LEFT			 = 0x01;		//������
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_UP			 = 0x02;		//�����
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_RIGHT		 = 0x04;		//�E����
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_DOWN			 = 0x08;		//������
//�A�N�V�����h���p�t���O
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_UP_RIGHT		 = 0x06;		//����͂ƉE����
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_UP_LEFT		 = 0x03;		//����͂ƍ�����
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_DOWN	 = 0x0C;		//�E���͂Ɖ�����
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_UP_RIGHT_DOWN = 0x0E;		//����́A�E���͂Ɖ�����
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_UP_LEFT_DOWN  = 0x0B;		//����͂ƍ��A������
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_UP_RIGHT_LEFT = 0x07;		//����́A�E���͂ƍ�����
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_ALL_MOVE_KEY	 = 0x0F;		//�ړ��L�[�S��
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_HALF_RIGHT_ROTATE = 0x10;	//�ォ�玞�v���180��
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_HALF_LEFT_ROTATE	 = 0x20;	//�ォ�甽���v���180��
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_ROTATE	 = 0x40;		//�ォ�玞�v���̓���
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_LEFT_ROTATE	 = 0x80;		//�ォ�甽���v���̓���

//�A�N�V�����h���p�t���O�̎��
static const BYTE	OFFROADACTION_COMMAND_BIT_KIND_OF_TYPE		 = 8;
//�A�N�V�������s�p�t���O
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_HALF_ROTATE_ACTION	 = 0x16;		//�ォ�玞�v���180�x��]�̃A�N�V�����t���O
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_LEFT_HALF_ROTATE_ACTION	 = 0x23;		//�ォ�甽���v���180�x��]�̃A�N�V�����t���O
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_RIGHT_ROTATE_ACTION		 = 0x1F;		//�ォ�玞�v���̓��͎��̃A�N�V�����t���O
static const BYTE	OFFROADACTION_COMMAND_BIT_TYPE_LEFT_ROTATE_ACTION		 = 0x2F;		//�ォ�甽���v���̓��͎��̃A�N�V�����t���O
static const BYTE	OFFROADACTION_COMMAND_KIND_OF_ACTION					 = 4;			//�A�N�V�����̐�

//�e�R�}���h�������̓��_
static const int	OFFROADACTION_COMMAND_TYPE_RIGHT_HALF_ROTATE_ACTION_POINT   = 100;		//�ォ�玞�v���180�x��]�̃A�N�V�����t���O
static const int	OFFROADACTION_COMMAND_TYPE_LEFT_HALF_ROTATE_ACTION_POINT    = 100;		//�ォ�甽���v���180�x��]�̃A�N�V�����t���O
static const int	OFFROADACTION_COMMAND_TYPE_RIGHT_ROTATE_ACTION_POINT	    = 200;		//�ォ�玞�v���̓��͎��̃A�N�V�����t���O
static const int	OFFROADACTION_COMMAND_TYPE_LEFT_ROTATE_ACTION_POINT			= 200;		//�ォ�甽���v���̓��͎��̃A�N�V�����t���O
//once success point
static const int	OFFROADACTION_SUCCESS_ACTION_BONUS_POINT = 10;

//�R�}���h���͂̐���
typedef unsigned char CORRECTINPUTTYPE;

//���I�t���[�h�v���C���[�N���X�̒�`
class COffRoadAction{

private:

	//�摜�̗v�f������
	int		*m_lpCOffRoadActionImg;
	//���ʉ��̗v�f������
	int		*m_lpCOffRoadActionSe;

	//�摜�ݒ�p
	CHARACTER		*m_lpOffRoadActionFram;		//�A�N�V�����R�}���h�̃t���[���p
	CHARACTER		*m_lpOffRoadActionCommand;	//�A�N�V�����R�}���h�p

	//�A�N�V�����R�}���h�\���Ԋu
	int		m_actionInterval;

	//�����A�N�V�����R�}���h�̍ő吔
	int		m_commandMax;

	//�����A�N�V�����R�}���h�J�E���g
	int		m_commandCnt;

	//���ݕ\�����̃A�N�V�����R�}���h�̎�ޔԍ�������
	tagCOMMANDACTIONTYPE	m_CommandTypeNow;

	//�����A�N�V�����^�C�v�̔z�������
	tagCOMMANDACTIONTYPE	*m_lpCommandType;

	//�A�N�V�����R�}���h�t���[���̕\�����W�擾�p
	static TPOINT<float>	m_indicateFramPos;

	//�A�N�V�����R�}���h�̃T�C�Y�擾�p
	static TPOINT<int>		m_FramSize;

	//�A�N�V�����������t���O
	static bool m_CommandCreateFlag;

	//���ݕ\�����̃A�N�V�����R�}���h�̓��͂̐���
	CORRECTINPUTTYPE	*m_lpCorrectInput;

	//���ݕ\�������R�}���h�̗v�f
	int		m_CommandMaterialNow;

protected:



public:

	//�� �ÓI�����o�ϐ��̒�`
	static const int OFFROADACTION_IMAGE_MAX;				//�摜�g�p����
	static const int OFFROADACTION_EFFECT_SOUND_MAX;		//���ʉ��g�p��
	

	//�E�B���h�E�n���h��
	static HWND	m_hWnd;

	//�@�A�N�V�����R�}���h�̃t���[��
	//�e�평���l
	//�\�����W�i���[�J�����W�j
	static const float OFFROADACTION_FRAM_DEFAULT_POSITION_X;			//�\�����WX
	static const float OFFROADACTION_FRAM_DEFAULT_POSITION_Y;			//�\�����WY
	//�摜�T�C�Y
	static const int OFFROADACTION_FRAM_IMAGE_WIDTH;		//��
	static const int OFFROADACTION_FRAM_IMAGE_HEIGHT;		//����
	//�g�k�{��
	static const float OFFROADACTION_FRAM_DEFAULT_SCALE;

	//�A�A�N�V�����R�}���h�摜
	//�e�평���l
	//�\�����W�i���[�J�����W�j
	static const float OFFROADACTION_COMMAND_DEFAULT_POSITION_X;			//�\�����WX
	static const float OFFROADACTION_COMMAND_DEFAULT_POSITION_Y;			//�\�����WY
	//�摜�T�C�Y
	static const int OFFROADACTION_COMMAND_IMAGE_WIDTH;			//��
	static const int OFFROADACTION_COMMAND_IMAGE_HEIGHT;		//����
	//�g�k�{��
	static const float OFFROADACTION_COMMAND_DEFAULT_SCALE;

	//���ۂɉ������摜�̎��
	static const CORRECTINPUTTYPE OFFROADACTION_COMMAND_IMAGE_VARIATION_NORMAL;
	static const CORRECTINPUTTYPE OFFROADACTION_COMMAND_IMAGE_VARIATION_EROR;
	static const CORRECTINPUTTYPE OFFROADACTION_COMMAND_IMAGE_VARIATION_SUCCESS;

	//���ۂɉ�������ސ�
	static const int OFFROADACTION_COMMAND_IMAGE_VARIATION_MAX;

	//�A�N�V�����R�}���h�\���Ԋu
	static const int OFFROADACTION_COMMAND_INTERVAL_TIME_EASY_MODE;		//Easy��
	static const int OFFROADACTION_COMMAND_INTERVAL_TIME_NORMAL_MODE;	//Normal��
	static const int OFFROADACTION_COMMAND_INTERVAL_TIME_HARD_MODE;		//Hard��

	//�A�N�V�����R�}���h�\����
	static const int OFFROADACTION_COMMAND_CREATION_MAX_EASY_MODE;		//Easy��
	static const int OFFROADACTION_COMMAND_CREATION_MAX_NORMAL_MODE;	//Normal��
	static const int OFFROADACTION_COMMAND_CREATION_MAX_HARD_MODE;		//Hard��

	//�R���X�g���N�^
	COffRoadAction();
	//�f�B�X�g���N�^
	~COffRoadAction();
	
	//�� ���상���o�֐��̒�`
	//�A�N�V�����̏�����
	void InitOffRoadAction();
	//�A�N�V�����̍X�V
	bool UpdateOffRoadAction();
	//�A�N�V�����̕\��
	void DrawOffRoadAction();
	//���
	void ReleaseOffRoadAction();
	/*
	������]�A�N�V�����R�}���h�̏�����
	*/
	void InitLevelRotateActionCommand();

	/*
	�A�N�V�����R�}���h�̐���
	�߂�l �� �R�}���h�������Ȃ�true
	*/
	bool CreateActionCommand( bool actionFlag );
		
	/*
	************************************************************************************************
	�A�N�V�����R�}���h�̓��͏󋵂�����
	�߂�l
	���� �F ���͂����L�[�R�[�h�ɑΉ������r�b�g��
	���s �F -1
	************************************************************************************************
	*/
	BYTE InputActionCommand();


	//�e��擾�p�̐ݒ�
	//�\�����W
	void	SetFramIndicatePosition( TPOINT<float> pos ){ this->m_indicateFramPos = pos; }
	//�T�C�Y
	void	SetFramSize( TPOINT<int> size ){ this->m_FramSize = size; }

	//�擾�p
	//���W
	TPOINT<float>	GetFramIndicatePosition() const { return this->m_indicateFramPos ; } 
	//�T�C�Y
	TPOINT<int>		GetFramSize() const { return this->m_FramSize; } 
	//�R�}���h���������擾
	bool	GetCommandActionCretaeFlag() const { return this->m_CommandCreateFlag; }

};


#endif
