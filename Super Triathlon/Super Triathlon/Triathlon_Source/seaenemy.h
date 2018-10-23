#ifndef  __ENEMY_H__
#define  __ENEMY_H__


//���}�N���̒�`
#define ENEMY_MAX		10			//�\������G�l�~�[�̍ő吔
#define ENEMY_IMG		5			//�����G�l�~�[�摜�̖���
#define KIND_ENEMY_SEA	5			//�G�l�~�[�̎�ށu�C�̃X�e�[�W�v
#define ENEMY_TYPE_SEA	5			//�G�̃A�N�V�����^�C�v��

#define JELLYFISH		0				//�N���Q�v�f��
#define SHARK			1				//�T���̗v�f��
#define SHOAL			2				//���Q�̗v�f��
#define SUNFISH			3				//�}���{�[�̗v�f��
#define RAY				4				//�G�C�̗v�f��

#define ENEMY_ACTION_CIRCLE		0		//�A�N�V����0�ԁu�~��`���v
#define ENEMY_ACTION_STRAIGHT	1		//�A�N�V����1�ԁu�^�������v
#define ENEMY_ACTION_WAVE		2		//�A�N�V����2�ԁu�g�łv
#define ENEMY_ACTION_APPROACH	3		//�A�N�V����3�ԁu�꒼���ɐڋ߁v
#define ENEMY_ACTION_CHASE		4		//�A�N�V����4�ԁu�ǐՁv

//����
const int	ENEMY_DIRECTION_RIGHT = -1;
const int	ENEMY_DIRECTION_LEFT = 1;

#define ENEMY_SOUND_EFFECT_MAX 1		//�g�p���ʉ�

//���w�b�_�[�t�@�C���̓ǂݍ���
#include "graphics.h"


//���\���̂̐錾

//ENEMY�\����
typedef struct tagENEMY{

	CHARACTER ch;		               //�L�����N�^�[�̊�{���������\���̌^�ϐ�
	float popx;			               //�o��������X���W
	float popy;			               //�o��������Y���W
	int type;			               //�G�l�~�[�̎�ޔԍ�
	int action_type;	               //�A�N�V�����^�C�v
	int cnt;			               //�����𐧌䂷��J�E���g
	int defeat[5];		               //��ޖ��̓|�������i���U���g�p�j
	float targetx;		               //�v���C���[�̍��W���擾����w���W
	float targety;		               //�v���C���[�̍��W���擾����x���W
	bool launch;		               //���˃t���O
	int  stayTime;		               //�؍ݎ���
	int appear;			               //�o����
	int	appearCnt;		               //�o�����J�E���g�p
	int appear_fram;	               //�o���t���[���Ԋu
	int appear_type;	               //�o����������
	float appear_start_posx;	       //�o���N�_X���W
	float appear_end_posx;		       //�����N�_X���W
	float appear_start_posx_r;	       //�E�X�N���[�����̏o���N�_���W�w
	float appear_end_posx_r;           //�E�X�N���[�����̏����N�_���W�w
	//�N���Q�̋����p
	int		circleCnt;				   //��]���̃J�E���g
	//���Q�̋����p
	int		waveCnt;				   //�g�ł����̃J�E���g

} ENEMY,*LPENEMY;



//������֐����`

//���������s���֐�
void InitEnemy(HWND hWnd);
//�X�V���s���֐�
void UpdateEnemy();
//�\�����s���֐�
void DrawEnemy();
//������s���֐�
void ReleaseEnemy();
//�w�肳�ꂽ���A��ޔԍ��̓G���o��������֐�
void CreateEnemy(int appear,int type,int action);
//�w�肳�ꂽ��ނɉ������s���̏�����
void InitEnemyAction0(ENEMY* pen);
void InitEnemyAction1(ENEMY* pen);
void InitEnemyAction2(ENEMY* pen);
void InitEnemyAction3(ENEMY* pen);
void InitEnemyAction4(ENEMY* pen);
//�w�肳�ꂽ��ޔԍ��ɂ���ēG�̍s����ς���֐�
void ActionType0(ENEMY* pen);		//�^�C�v�O��
void ActionType1(ENEMY* pen);		//�^�C�v�P��
void ActionType2(ENEMY* pen);		//�^�C�v�Q��
void ActionType3(ENEMY* pen);		//�^�C�v�R��
void ActionType4(ENEMY* pen);		//�^�C�v�S��
//�w��̓G�Ɠ����蔻������֐�
bool CollisionEnemy(CHARACTER* ch);
//�G�̏���Ԃ�
ENEMY* GetEnemy();
/*
�ݒ肵�����W���K��ɂ����G�̏o��
*/
void	SetCreationEnemyByPosition();
/*
�G�̏o���������_��
*/
void	SetCreationEnemyByRandom();

#endif