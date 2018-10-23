#ifndef  __SWIMER_H__
#define  __SWIMER_H__



//���w�b�_�[�t�@�C���̓ǂݍ���
#include	"graphics.h"
#include	"animation.h"
#include	<windows.h>

//���}�N���̒�`
//�摜�ݒ�p
#define SWIMER_IMAGE_MAX			5		//�g�p����
#define SWIMER_ANIMATION_TYPE		7		//�摜�^�C�v�̎��
#define SWIMER_ATTACK_TYPE_MAX		4		//�U���^�C�v����_�̐�

//�ʏ펞�̃T�C�Y
const int SWIMER_DEFAULT_IMAGE_WIDTH = 104;
const int SWIMER_DEFAULT_IMAGE_HEIGHT = 40;

//��]�U���Ɠːi�U���p�摜�T�C�Y
const int SWIMER_ATTACK_IMAGE_HEIGHT = 96;


//�� �A�j���[�V�����ݒ�
const int SWIMER_TYPE_NORMAL_ANIMATION_MAX			= 3;			//�ʏ펞
//�e��U���^�C�v�ɉ������ő�R�}��
const int SWIMER_TYPE_CIRCLE_ATTACK_ANIMATION_MAX	= 69;			//��]�U��
const int SWIMER_TYPE_BUMP_ATTACK_ANIMATION_MAX		= 6;			//�ːi�U��

//�t���[����
const int SWIMER_TYPE_NORMAL_ANIMATION_FRAM			= 20;			//�ʏ펞
//�e��U���^�C�v�ɉ������t���[����
const int SWIMER_TYPE_CIRCLE_ATTACK_ANIMATION_FRAM	= 2;			//��]�U��
const int SWIMER_TYPE_BUMP_ATTACK_ANIMATION_FRAM	= 2;			//�ːi�U��


//�g�k��
#define SWIMER_DEFAULT_SCALE		1.0f

//�X�e�[�^�X�p
#define SWIMER_STATUS				3		//�g�p�v�f��
#define SWIMER_SCORE				1		//�g�p�v�f��

//�U���Q�[�W
#define	SWIMER_ATTACK_MAX			3		//�ő�U����

//�X�e�[�^�X�ϒl
//���x�̏����l
#define SWIMER_DEFAULT_SPEEDX_R			-2.0f	//�E�X�N���[������X�ړ���
#define SWIMER_DEFAULT_SPEEDX_L			2.0f	//���X�N���[������X�ړ���
#define SWIMER_DEFAULT_SPEEDY			5.0f	//Y�ړ���
#define SWIMER_DEFAULT_SPEEDMAX_RATE	3.5f	//���x�{�����
#define SWIMER_DEFAULT_SPEEDMIN_RATE	0.5f	//���x�{������
#define SWIMER_DEFAULT_SPEED_RATE		1.0f	//���x�{��

//�G�������
#define SWIMER_DEFAULT_AVOID_TIME	100

//���p
#define SWIMER_SOUND_EFFECT_MAX		2	//���ʉ��g�p��


//�� �A�j���[�V��������̃r�b�g��
//�U���r�b�g��
const BYTE	SWIMER_ANIMATION_BIT_TYPE_NORMAL_ATTACK		= 0x02;		//�o�^�t���C��
const BYTE	SWIMER_ANIMATION_BIT_TYPE_CIRCLE_ATTACK		= 0x04;		//��]��
const BYTE	SWIMER_ANIMATION_BIT_TYPE_BUMP_ATTACK		= 0x08;		//�ːi��

//�E����
const BYTE	SWIMER_ANIMATION_TYPE_DIRECTION_RIGHT			= 0x00;		//�E����
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_NORMAL_ATTACK		= 0x02;		//�o�^�t���C��
//�A�C�e�����ʂ̕���
//�ʏ펞
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_SPEED_UP		= 0x10;						//���x�㏸��
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_ABSOLUTE		= 0x20;						//���G��
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_SPEED_DOWN	= 0x40;						//���x������
//�o�^�t���C��
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_NORMAL_ATTACK_SPEED_UP	= 0x12;			//���x�㏸��
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_NORMAL_ATTACK_ABSOLUTE	= 0x22;			//���G��
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_NORMAL_ATTACK_SPEED_DOWN	= 0x42;			//���x������
//������
//�A�C�e������
//�ʏ펞
const BYTE	SWIMER_ANIMATION_TYPE_DIRECTION_LEFT			= 0x01;		//������
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_NORMAL_ATTACK		= 0x03;		//�o�^�t���C
//�A�C�e������
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_SPEED_UP		= 0x11;						//���x�㏸��
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_ABSOLUTE		= 0x21;						//���G��
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_SPEED_DOWN		= 0x41;						//���x������
//�o�^�t���C��
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_NORMAL_ATTACK_SPEED_UP		= 0x13;			//���x�㏸��
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_NORMAL_ATTACK_ABSOLUTE		= 0x23;			//���G��
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_NORMAL_ATTACK_SPEED_DOWN	= 0x43;			//���x������


//�A�j���[�V�����̎��
const int	SWIMER_ANIMATION_KIND_OF_TYPE	= 16;


//�� �T�C�Y�ύX�U��
//�E����
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_CIRCLE_ATTACK		= 0x06;		//��]��
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_BUMP_ATTACK		= 0x0E;		//�ːi��
//��]��
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_CIRCLE_ATTACK_SPEED_UP	= 0x16;			//���x�㏸��
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_CIRCLE_ATTACK_ABSOLUTE	= 0x26;			//���G��
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_CIRCLE_ATTACK_SPEED_DOWN	= 0x46;			//���x������
//�ːi
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_BUMP_ATTACK_SPEED_UP		= 0x1E;			//���x�㏸��
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_BUMP_ATTACK_ABSOLUTE		= 0x2E;			//���G��
const BYTE	SWIMER_ANIMATION_TYPE_D_RIGHT_BUMP_ATTACK_SPEED_DOWN	= 0x4E;			//���x������

//������
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_CIRCLE_ATTACK		= 0x08;		//��]
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_BUMP_ATTACK		= 0x0F;		//�ːi
//��]��
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_CIRCLE_ATTACK_SPEED_UP		= 0x18;			//���x�㏸��
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_CIRCLE_ATTACK_ABSOLUTE		= 0x28;			//���G��
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_CIRCLE_ATTACK_SPEED_DOWN	= 0x48;			//���x������
//�ːi
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_BUMP_ATTACK_SPEED_UP		= 0x1F;			//���x�㏸��
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_BUMP_ATTACK_ABSOLUTE		= 0x2F;			//���G��
const BYTE	SWIMER_ANIMATION_TYPE_D_LEFT_BUMP_ATTACK_SPEED_DOWN		= 0x4F;			//���x������

//�U�����̕\�������WY
const int	SWIMER_ANIMATION_TYPE_CIRCLE_ATTACK_PIC_Y	=	640;		//��]
const int	SWIMER_ANIMATION_TYPE_BUMP_ATTACK_PIC_Y		=	1408;		//�ːi


//�U���^�C�v���̃A�j���[�V����
#define SWIMER_ATTACK_ABSOLUTE	2		//���G��
#define SWIMER_ATTACK_SPEEDSTAR 4		//���x�㏸
#define SWIMER_ATTACK_SNAIL		6		//���x����


//�񋓌^�̒�`
//�v���C���[�ɍ�p����A�C�e���̌���
enum tagSWIMERITEMEFFECTTYPE {

	SWIMER_ITEM_EFFECT_TYPE_VACANT = -1,	//�Ȃ�
	SWIMER_ITEM_EFFECT_TYPE_SPEEDSTAR,		//���x�㏸
	SWIMER_ITEM_EFFECT_TYPE_ABSOLUTE,		//���G���
	SWIMER_ITEM_EFFECT_TYPE_BIG,			//�g��
	SWIMER_ITEM_EFFECT_TYPE_SNAIL,			//�ݑ�
	SWIMER_ITEM_EFFECT_TYPE_SMALL,			//�k��
	SWIMER_KIND_OF_ITEM_EFFECT,				//���ʂ̎��
};

//�U������p
enum tagSWIMERATTACKTYPE{

	SWIMER_ATTACK_NORMAL = -1	,		//�ʏ펞
	SWIMER_ATTACK_WAVE_ATTACK	,		//�o�^�t���C��
	SWIMER_ATTACK_CIRCLE_ATTACK ,		//��]��
	SWIMER_ATTACK_BUMP_ATTACK	,		//�ːi��

};

//���x�ɉ������A�j���[�V����������
enum tagSWIMERANIMATIONSPEED{

	SWIMER_SPEED_SLOW	= 40 ,		//�x��	
	SWIMER_SPEED_NORMAL = 20 ,		//����
	SWIMER_SPEED_DECENT = 15 ,		//���\����
	SWIMER_SPEED_RAPID	= 10 ,		//����
};


//���\���̂̒�`

//SWIMER�\����
typedef struct tagSWIMER{

	CHARACTER ch;							//�L�����N�^�[�̊�{���
	ANIMATION ani;							//�A�j���[�V�������
	bool	attack;							//�U�����t���O
	bool	able_attack;					//�U���\�t���O
	bool	attack_charge;					//�U���`���[�W�t���O
	int		attack_type;					//�U���̃^�C�v
	tagSWIMERITEMEFFECTTYPE	itemEffectType;	//���ʃA�C�e���̎��
	float	pos_x;							//�ʒu�ύXX���W
	float	pos_y;							//�ʒu�ύXY���W
	float	speedmax;						//�ő�����{��
	float	speedmin;						//�ŏ������{��
	int		speedlevel;						//���x���x��
	int		avoid_time;						//�G�������
	int		defeat_cnt;						//�G��|���������J�E���g
	int		chain_max;						//�ő�A��������
	int		charge;							//���ߎ���

} SWIMER , *LPSWIMER;


//������֐��̒�`

//���������s���֐�
void    InitSwimer( HWND hWnd );
//�X�V���s���֐�
void    UpdateSwimer();
//�\������֐�
void    DrawSwimer();
//������s���֐�
void    ReleaseSwimer();

//�A�j���[�V�������X�V(�ʏ�T�C�Y��)
void	ChangeSwimerNormalAnimation();

//�A�j���[�V�������X�V(�T�C�Y�ύX��)
void	ChangeSwimerAttackAnimation();

//�ړ��̍X�V
void    UpdateSwimerMove();

//���x�̍X�V
void    UpdateSwimerSpeed();

//�U���̍X�V
void    UpdateSwimerAttack();

//�ړ��͈͂𐧌䂷��
void    StrictSwimerMove();

//�v���C���[�̃|�C���^��Ԃ��֐�
SWIMER* GetSwimer();

//�v���C���[�̕t�����ʂ�����
void    OrdinarySwimer();

//�U���Q�[�W�̉�
void    SwimerChargeAttack();

//�U���Q�[�W������čU�����s��
bool	SwimerGoAttack();

//�A�N�V�����^�C�v�O�ԁu�o�^�t���C�v�̏�����
void    InitSwimerActionType0();

//�A�N�V�����^�C�v1�ԁu��]�v�̏�����
void    InitSwimerActionType1();

//�A�N�V�����^�C�v2�ԁu�x�W�F�v�̏�����
void    InitSwimerActionType2();

//�A�N�V�����^�C�v�O�ԁu�o�^�t���C�v
void    SwimerActionType0();

//�A�N�V�����^�C�v1�ԁu��]�v
void    SwimerActionType1();

//�x�W�F�Ȑ��̓���
void    SwimerActionType2();

//���ʂP�u���x�Q�{�v
void    SwimerEffect0();

//���ʂQ�u���G���v
void    SwimerEffect1();

//���ʂR�u���x�����v
void    SwimerEffect2();

//���ʂS�u���剻�v
void    SwimerEffect3();

//���ʂT�u�k���v
void    SwimerEffect4();

//�A�C�e�����ʂ̃r�b�g�ڂ�ݒ肷��
void	SetItemEffect( BYTE item );


#endif