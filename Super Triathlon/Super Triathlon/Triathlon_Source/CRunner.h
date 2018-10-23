#ifndef		CRUNNER_H__
#define		CRUNNER_H__


//���w�b�_�[���C���N���[�h
#include	"graphics.h"
#include	"animation.h"


//�� �}�N���̒�`
//�����i�[�̉摜�ݒ�
const int RUNNER_DEFAULT_IMAGE_WIDTH	= 32;			//��
const int RUNNER_DEFAULT_IMAGE_HEIGHT	= 96;			//����
const float RUNNER_DEFAULT_IMAGE_SCALE = 1.0f;		//�g�k�{��

//�A�j���[�V�����ݒ�
//�ʏ펞
const int RUNNER_ANIMATION_TYPE_NORMAL_CNT_MAX	= 4;			//�ő�R�}��
const int RUNNER_ANIMATION_TYPE_NORMAL_FRAM		= 10;			//�t���[����
//�W�����v
const int RUNNER_ANIMATION_TYPE_JUMP_CNT_MAX	= 18;			//�ő�R�}��
const int RUNNER_ANIMATION_TYPE_JUMP_FRAM		= 2;			//�t���[����
//�U��
const int RUNNER_ANIMATION_TYPE_ATTACK_CNT_MAX	= 14;			//�ő�R�}��
const int RUNNER_ANIMATION_TYPE_ATTACK_FRAM		= 2;			//�t���[����


//���X�e�[�^�X
//�̗�
const int RUNNER_DEFAULT_STAMINA_MAX	 = 3;			

//�ړ�
const float RUNNER_DEFAULT_LINE_MOVE		= 5.0f;			//�����̈ړ���
const float RUNNER_DEFAULT_MOVE_RIGHT		= 5.0f;			//���ړ�
const float RUNNER_DEFAULT_MOVE_LEFT		= -5.0f;		//�E�ړ�
const float RUNNER_DEFAULT_MOVE_FRONT		= -5.0f;		//Y�ړ��ʁu��ړ��v
const float RUNNER_DEFAULT_MOVE_BACK		= 5.0f;			//Y�ړ��ʁu���ړ��v

//���x
const float RUNNER_DEFAULT_MOVE_Y			= -3.0f;		//Y�ړ���
const float RUNNER_DEFAULT_SPEEDMAX_RATE	= 3.5f;			//���x�{�����
const float RUNNER_DEFAULT_SPEEDMIN_RATE	= 0.5f;			//���x�{������
const float RUNNER_DEFAULT_SPEED_RATE		= 1.0f;			//���x�{��
const int RUNNER_DEFAULT_CHARGEMAX_RATE	= 500;			//���x�`���[�W�l�̍ő�l
const int RUNNER_DEFAULT_AVOID_TIME		= 100;			//�������
//���x����
const float RUNNER_CONSTRAIN_MINIMUM_SPEED = -3.0f;

//�A�j���[�V����������
const BYTE	RUNNER_ANIMATION_ROUND		= 0x00;				//�ڒn���
const BYTE	RUNNER_ANIMATION_JUMP		= 0x01;				//�W�����v��
const BYTE	RUNNER_ANIMATION_ATTACK		= 0x02;				//�U����
const BYTE	RUNNER_ANIMATION_MAX		= 3;		   		//�ω�����A�j���[�V�����̎��

//�ړ�����������
const BYTE RUNNER_NOT_MOVE				= 0x00;			//��~���
const BYTE RUNNER_MOVE_DIRECTION_RIGHT  = 0x01;			//�E
const BYTE RUNNER_MOVE_DIRECTION_LEFT	= 0x02;			//��
const BYTE RUNNER_MOVE_DIRECTION_FRONT	= 0x03;			//�O
const BYTE RUNNER_MOVE_DIRECTION_BACK	= 0x04;			//��

//����t���O
const BYTE RUNNER_SAFTY				= 0x00;				//���S�t���O
const BYTE RUNNER_BUMP_ATTENTION	= 0x01;				//�Փ˃t���O
const BYTE RUNNER_WHILE_AVOID		= 0x02;				//��𒆃t���O

//�s���t���O
const BYTE RUNNER_ACTION_ROUND				= 0x00;				//�ڒn�t���O
const BYTE RUNNER_ACTION_JUMP				= 0x01;				//�W�����v�t���O
const BYTE RUNNER_ACTION_ATTACK				= 0x02;				//�U���t���O
const BYTE RUNNER_ACTION_JUMP_AND_ATTACK	= 0x03;				//�W�����v���U���t���O
const BYTE RUNNER_ACTION_LINE_MOVE			= 0x04;				//���ړ��t���O
const BYTE RUNNER_KIND_OF_ACTION			= 5;				//�A�N�V�����̎�ސ�

//��Q���Ƃ̔���ɂ��t���O
const char RUNNER_NOT_BUMP				= -1;						//�Փ˂Ȃ�
const char RUNNER_BUMP_TO_OBSTACLE		= 0;						//�Փ˃t���O
const char RUNNER_EFFECT_FLAG_SPEED_UP	= 1;						//���x�㏸�t���O
const char RUNNER_EFFECT_FLAG_ABSOLUTE	= 2;						//���G�t���O
const char RUNNER_EFFECT_FLAG_HEALING	= 3;						//�񕜃t���O	
const char RUNNNER_BUMP_TO_HURDLE		= 4;						//bump to hurdle
const char RUNNNER_BUMP_TO_ROCK			= 5;						//bump to rock
//�������ʂ������^�C�v�ԍ�
const BYTE RUNNER_TYPE_NORMAL					= 0x00;
const BYTE RUNNER_EFFECT_TYPE_SPEED_UP			= 0x01;				//���x�㏸
const BYTE RUNNER_EFFECT_TYPE_ABSOLUTE			= 0x02;				//���G���
const BYTE RUNNER_EFFECT_TYPE_SPEED_AND_ABSOLUTE = 0x03;			//���x�����G
const BYTE RUNNER_EFFECT_TYPE_HEALING			= 0x04;				//��
const BYTE RUNNER_EFFECT_KIND_OF_TYPE			= 3;				//�����^�C�v�̎��

//���G���ʎ���
const short RUNNER_ABSOLUTE_EFFECT_TIME = 1000;

//���G�͈�
const int RUNNER_DEFAULT_SEARCH_FRONT	= -70;				//�O���͈�
const int RUNNER_DEFAULT_SEARCH_WIDE	= 96;				//���͈�



//���x�ɉ������A�j���[�V����������
enum tagRUNNERANIMATIONSPEED{

	RUNNER_SPEED_SLOW	= 40 ,			//�x��	
	RUNNER_SPEED_NORMAL = 20 ,			//����
	RUNNER_SPEED_DECENT = 15 ,			//���\����
	RUNNER_SPEED_RAPID	= 10 ,			//����
	RUNNER_SPEED_ANIMATION_RATE = 2 ,	//�A�j���[�V�����̑��x��ω�������{��
};

//���\���̂̒�`
//�����i�[�p
typedef struct tagRUNNER{

	CHARACTER ch;						//CHARACTER�\����
	ANIMATION ani;						//ANIMATION�\����
	float pos_x;						//�ʒu�ύX�pX
	float pos_y;						//�ʒu�ύX�pY
	int speed_charge;					//�X�s�[�h�`���[�W�l
	int speedChargeMax;					//�X�s�[�h�`���[�W�l�̏��
	float speed_max;					//�X�s�[�h�ő�l
	float speed_min;					//�X�s�[�h�ŏ��l
	tagRUNNERANIMATIONSPEED speedLevel;	//���x�ɉ������A�j���[�V������ω�������
	int avoidtime;						//�������
	int speedTime;						//���x�Ɉ�������
	int absoluteTime;					//���G���Ԃ�����
	BYTE actionF;						//�s���t���O
	BYTE avoidF;						//����t���O
	char actionType;					//�s�����
	int avoidDistance;					//����ړ��������v��
	int designateDistance;				//����܂ł̈ړ�����
	BYTE getType;						//��ނ��󂯎��
	float aggregateSpeed;				//���x�̍��v������
	BYTE effectType;					//�������ʂ̃^�C�v
	char bumpType;						//��Q���Ƃ̏Փˎ��̃^�C�v�����p
	int obstacleBumpNow;				//���݂̏Փ˒���Q���̗v�f���p

	//�L�^�p
	int jumpedHurdleCnt[3];				//�n�[�h�����щz������
	int jumpedHurdleChain;				//�n�[�h����A���Ŕ�щz������
	int jumpedHurdleChainMax;			//�n�[�h����A���Ŕ�щz�����ő吔
	int defeatCnt[2];					//�j�󂵂���

}RUNNER , *LPRUNNER;


//�� �O���錾
class	CEffectManager;
class	CRoadEnemyManager;

//�����i�[�̊��N���X
class	CRunner{

private:

protected:

	//���҂̃f�t�H���g�\����
	RUNNER m_RunnerDefault;

	//���҂̉e�̃f�t�H���g�\����
	RUNNER m_ShadowDefault;

public:


	//�� �ÓI�����o�ϐ��̒�`

	//���֐��̃v���g�^�C�v�錾
	CRunner();
	~CRunner();

	//������
	virtual void InitRunner( int runnerNum , CRoadEnemyManager	*lpRoadEnemy ){}
	//�X�V
	virtual void UpdateRunner( int runner , CEffectManager *lpCEffect ){}
	//�\��
	virtual void DrawRunner() = 0;
	//���
	virtual void ReleaseRunner() = 0;
	
	/*************************************************************************************************
	�n�[�h�����щz�����ۂ̃v���C���[�̋������ʂP�u���x�㏸�v
	�������֐�
	**************************************************************************************************
	*/
	void RunnerEffectSpeedUp();
	/*************************************************************************************************
	�n�[�h�����щz�����ۂ̃v���C���[�̋������ʂQ�u���G��ԁv
	�������֐�
	**************************************************************************************************
	*/
	void RunnerEffectAbsolute();
	/*************************************************************************************************
	�n�[�h�����щz�����ۂ̃v���C���[�̋������ʂP�u���x�㏸�v
	�X�V�֐�
	**************************************************************************************************
	*/
	void RepealRunnerEffectSpeedUp();
	/*************************************************************************************************
	���Ԍo�߂Ŗ��G��Ԃ𖳌��ɂ���
	**************************************************************************************************
	*/
	bool RepealRunnerEffectAbsolute();
	//************************************************************************************************
	//�����蔻��@CollisionRunner
	//************************************************************************************************
	bool CollisionRunner( CHARACTER* pch1 , CHARACTER* pch2 );

};


#endif
