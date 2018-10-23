#ifndef		CROADENEMY_H__
#define		CROADENEMY_H__


//���w�b�_�[���C���N���[�h
#include	"graphics.h"
#include	"animation.h"
#include	"CRunner.h"


//��Q���̃^�C�v�ԍ��i�摜�p�j
enum tagOBSTACLETYPE{

	ROADENEMY_OBSTACLE_TYPE_HURDLE  ,			//�n�[�h��
	ROADENEMY_OBSTACLE_TYPE_ROCK    ,			//���
	ROADENEMY_KIND_OF_OBSTACLE		,			//��Q���̎��
};

//��Q���̎�ޔԍ��i�L�����N�^�[�s������p�j
const BYTE	ROADENEMY_OBSTACLE_BIT_TYPE_VACANT	= 0x00;	//��Q���Ȃ�
const BYTE	ROADENEMY_OBSTACLE_BIT_TYPE_HURDLE	= 0x01;	//�n�[�h��
const BYTE	ROADENEMY_OBSTACLE_BIT_TYPE_ROCK	= 0x02;	//���

//�n�[�h���̎��
enum tagHURDLECOLORTYPE{
	 ROADENEMY_OBSTACLE_HURDLE_COLOR_BLACK	,		//��
	 ROADENEMY_OBSTACLE_HURDLE_COLOR_RED	,		//��
	 ROADENEMY_OBSTACLE_HURDLE_COLOR_BLUE	,		//��
	 ROADENEMY_OBSTACLE_HURDLE_COLOR_GREEN	,		//��
	 ROADENEMY_OBSTACLE_HURDLE_KIND_OF_COLOR,	//�F�̎��
};

//���\���̂̒�`
//��Q���̍��W�������\����
typedef struct tagOBSTACLE{

	CHARACTER ch;		//CHARACTER�\����
	ANIMATION ani;		//ANIMATION�\����
	FPOSITION fitPos;	//��Q���̍��W�ύX�p
	BYTE bitType;		//��Q���̃r�b�g�ڂ�����
	char colorType;		//�F�̎��

}OBSTACLE , *LPOBSTACLE;


//���O���錾
class	CCompetitorManager;
class	CPlayerRunner;

//��Q���N���X
class	CObstacle{

private:

	//�����ҊǗ��N���X
	CCompetitorManager	*m_lpCCompetitorManager;

	//�v���C���[�N���X
	CPlayerRunner		*m_lpCPlayerRunner;

	//�摜�̗v�f������
	int		*m_lpRoadEnemyImg;

	//��Q���摜�̏�������
	OBSTACLE	*m_lpRoadObstacle;

	//���ʉ��̗v�f��
	int		*m_lpRoadEnemyEffectSound;

	//�e���Q���J�E���g�p
	int		*m_lpObstacleCnt;

	//�e���Q���̐����J�E���g�p
	int		*m_lpCreationOBstacleCnt;

	//��������n�[�h���̎�ނ̊m��
	int		*m_lpCreateRateToHurdle;

	//�e��n�[�h���̐��J�E���g�p
	int		*m_lpCreateHurdleCnt;

	//�e��n�[�h���̍ő吶����
	int		*m_lpCreateHurdleCntMax;

	//�z��̃����_���̒l������
	int		m_ObstacleRandomArrangeMax;

	//�z��̎�ނ̃����_���l������
	int		m_ObstacleInitArrangeRandomType;

	//��Q���̍ő吶����
	int		m_ObstacleCreateMax;

	//�����������W
	float	m_CreatedObstacleY;

	//��Q���𐶐�������W�Ԋu
	int		m_CreateObstacleSpaceY;


protected:

public:

	//�� �ÓI�����o�ϐ��̒�`
	//�摜
	static const int ROADENEMY_IMAGE_MAX;  		//�摜�̎g�p����

	static const int ROADENEMY_OBSTACLE_MAX;	//��Q���̏o������

	//���ʉ�
	static const int ROADENEMY_SOUND_EFFECT_MAX; 	//�g�p���ʉ���

	//��ʃX�N���[���ɍ��킹�Ċg�傷���Q���̉�ʉ��[�ł̍ŏI�I��X���W�̂���
	static const float OBSTACLE_FITTING_SCROL_POSITION_X;

	//�����ɕ��ׂ�ő吔
	static const int	OBSTACLE_HURDLE_LINE_MAX;
	//�����ɕ��ׂ�ۂ̔z��̎��
	static const int	OBSTACLE_HURDLE_LINE_ARRANGE_MAX;

	//�z��̎��
	static const char	OBSTACLE_HURDLE_ARRANGE_TYPE_STRAIGHT_LINE;  		//���꒼���ɓ����ǂ�
	static const char	OBSTACLE_HURDLE_ARRANGE_TYPE_STRAIGHT_RANDOM;  		//���꒼���Ƀ����_���ɔz�u
	static const char	OBSTACLE_HURDLE_KIND_OF_ARRANGE;  					//��ސ�

	//�n�[�h���̔z�u�n�_���W
	static const float OBSTACLE_HURDLE_POSITION_START_X;		//X
	//�n�[�h���̊Ԋu
	static const float OBSTACLE_HURDLE_SPACE_X;					//X

	//�e���Q���̕\���ݒ�
	//��
	static const int ROADENEMY_OBSTACLE_HURDLE_SIZE_WIDTH;
	static const int ROADENEMY_OBSTACLE_ROCK_SIZE_WIDTH;
	//����
	static const int ROADENEMY_OBSTACLE_HURDLE_SIZE_HEIGHT;
	static const int ROADENEMY_OBSTACLE_ROCK_SIZE_HEIGHT;
	//�g�k�{��
	static const float ROADENEMY_OBSTACLE_HURDLE_SIZE_SCALE;
	static const float ROADENEMY_OBSTACLE_ROCK_SIZE_SCALE;
	//�ړ���Y��Βl
	static const float ROADENEMY_OBSTACLE_HURDLE_ABSOLUTE_MOVE_Y;
	static const float ROADENEMY_OBSTACLE_ROCK_ABSOLUTE_MOVE_Y;
	//�ړ���X��Βl
	static const float ROADENEMY_OBSTACLE_HURDLE_ABSOLUTE_MOVE_X;
	static const float ROADENEMY_OBSTACLE_ROCK_ABSOLUTE_MOVE_X;

	//�e���Q���̃A�j���[�V�����ݒ�
	//�t���[����
	static const int ROADENEMY_OBSTACLE_HURDLE_ANIMATION_FRAM;
	static const int ROADENEMY_OBSTACLE_ROCK_ANIMATION_FRAM;
	//�R�}��
	static const int ROADENEMY_OBSTACLE_HURDLE_ANIMATION_CNT_MAX;
	static const int ROADENEMY_OBSTACLE_ROCK_ANIMATION_CNT_MAX;
	//��Q���̃A�j���[�V�����t���O
	static const BYTE ROADENEMY_ANIMATION_FLAG_NORMAL;	//�ʏ���
	static const BYTE ROADENEMY_ANIMATION_FLAG_BREAK;	//�j��t���O


	//�Փ˂���ʂ�����
	static const char OBSTACLE_NOT_BUMP;		//�Փ˂Ȃ�
	static const char OBSTACLE_BUMP_TOP;		//��
	static const char OBSTACLE_BUMP_BOTTOM;		//��
	static const char OBSTACLE_BUMP_LEFT;		//��
	static const char OBSTACLE_BUMP_RIGHT;		//�E


	//�� �����o�֐��̒�`

	CObstacle();
	CObstacle( int max );
	~CObstacle();

	void InitRoadEnemy(	CCompetitorManager	*lpCCompetitorManager , CPlayerRunner *lpCPlayerRunner );			//������
	void UpdateRoadEnemy();			//�X�V
	void DrawRoadEnemy();			//�\��
	void ReleaseRoadEnemy();		//���

	//��Q���̐���
	void	CreateObstacle( int type );

	//�����蔻��
	char	CollisionRoadEnemy( RUNNER* pRunner );

	//��Q��1�u�n�[�h���v�̍s���̏�����
	bool	InitObstacleHurdle( int material );

	//********************************************************************************************
	//��Q��1�u�n�[�h���v�z�u�̏�����
	//���꒼���ɔz�u����
	//********************************************************************************************
	bool	InitArrangeHurdleStraightLine( int material );

	/**************************************************************************************************
	�n�[�h���̔z�u�̏�����
	���꒼���Ƀ����_���ŕ��ׂ�
	***************************************************************************************************
	*/
	bool	InitArrangeHurdleRandomStraightLine( int material );

	//��Q��1�u�n�[�h���v�̍s���̍X�V
	void	UpdateActionObstacleHurdle( int material );

	//��Q��2�u��΁v�̍s���̏�����
	bool	InitObstacleRock(int material);

	//��Q2�u��΁v���̍s���̍X�V
	void	UpdateActionObstacleRock( int material );

	//�����̃L�����N�^�[�̍��G�͈͂Ɣ�������
	BYTE	BroadenCollisionSquareObstacle( CHARACTER* pch );

	//���̓����蔻������
	BYTE	AttemptCollisionSquare( CHARACTER* pch1 );
	/*
	initialize process for hurdle when player bump to hurdle
	*/
	char	CollisionInitializeHurdle(RUNNER *pRunner, int material);
	/*
	initialize process for rock when player bump to hurdle
	*/
	char	CollisionInitializeRock(RUNNER *pRunner);


	//�� �e��ݒ�p
	//��Q���p
	void	SetRoadEnemyImage( int *lpImage , int max );
	//��Q���̌��ʉ�
	void	SetRoadEnemyEffectSound( int *lpSound , int max );


};

#endif