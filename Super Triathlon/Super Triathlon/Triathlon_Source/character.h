#ifndef __CHARACTER_H__
#define __CHARACTER_H__


//���}�N���̒�`
#define D_RIGHT 0x00		//�E����
#define D_LEFT	0x01		//������

//�Փ˂���ʂ�����
#define CHARACTER_NOT_BUMP		-1			//�Փ˂Ȃ�
#define CHARACTER_BUMP_TOP		0x00		//��
#define CHARACTER_BUMP_BOTTOM	0x01		//��
#define CHARACTER_BUMP_LEFT		0x02		//��
#define CHARACTER_BUMP_RIGHT	0x03		//�E


//���w�b�_�[���C���N���[�h
#include <windows.h>

//���\���̂̒�`
//�Փˎ��̈��S��`�̎w��p
template < typename T >
struct TSAFETYRECT{
	T	up;
	T	right;
	T	down;
	T	left;
};


//�L�����N�^�[�̍��W�������ň���
typedef struct tagFPOSITION{

	float fX;			//X���W
	float fY;			//Y���W

} FPOSITION;

//�P�̂̃L�����N�^�[��\������f�[�^���܂Ƃ߂�
//CHARACTER�\����
typedef struct tagCHARACTER{

	float x;				//�w���W
	float y;				//�x���W
	float sx;				//�n�_X���W
	float sy;				//�n�_Y���W
	float ex;				//�I�_X���W
	float ey;				//�I�_Y���W
	float mx;				//�w�ړ���
	float my;				//�x�ړ���
	float smx;				//�n�_�w�ړ���
	float smy;				//�n�_�x�ړ���
	float default_mx;		//�����w�ړ���
	float default_my;		//�����x�ړ���
	int w;					//��
	int h;					//����
	float scale;			//�g�k�̔{��
	float defaultScale;		//�g�k���̏����l
	float alpha;			//���ߒl
	int picx;				//�\�����w���W
	int picy;				//�\�����x���W
	bool flag;				//�����t���O
	int direction;			//����
	int direction_now;		//���݂̕���
	int time;				//���Ԃ�����
	float millisecond;			//���ׂȎ��Ԃ�����
	int hp;					//�̗�
	float speed;			//���x
	float defaultSpeed;		//���x�����l
	int bitnum;				//�`�掞�Ɉ����v�f��
	int angle;				//�p�x
	int cnt;				//�J�E���g
	int type;				//��ޔԍ�
	int SearchSquareFront;	//�O���ɍL������G�͈�
	int SearchSquareWidth;	//�L������G�͈͂̕�


} CHARACTER,*LPCHARACTER;


//���v���g�^�C�v�錾

//�����蔻����s���֐�
bool CollisionCharacter(CHARACTER* pch1,CHARACTER* pch2);

//��莞�ԉ�����
bool AvoidEnemy(CHARACTER* pch , int avoidtime);

//�������x�W�F�Ȑ�
void SetBezier(CHARACTER* ch , POINT* pos , int bezier , float time);

//�L�����N�^�[�̒��S���W�����߂�
void CenterCharacter(CHARACTER* ch , int len , int angle);

//�n���ꂽ�ő�l�ƍŏ��l�����ɁA�����_���̒l��Ԃ��֐�
int RandomNum(int max,int min);

//�L�����N�^�[���m�̋�`�Ŕ�������A�ʒu���C������
BYTE CheckOverlapCharacter( CHARACTER *pch1 , CHARACTER *pch2, TSAFETYRECT<int>	safetyRect );

//�����̃L�����N�^�[�̍��G�͈͂ƈ����̃L�����N�^�[�̋�`�Ŕ�������@BroadenCollisionSquare
//�����P�@���@���g�̋�`�Ŕ�������L�����N�^�[
//�����Q�@���@���g�̎���ɔ�����L����L�����N�^�[
bool BroadenCollisionSquare( CHARACTER* pch1 , CHARACTER* pch2 );

//����J�������_�ɍ��킹�Ĉ����̃L�����N�^�[�̊g�k����K�����l�ɕύX����
void FittingPositionToBackCameraAngle( CHARACTER* pch , float scaleMax );

/*
�����̒l�̊m����true���擾����
�����P�@�F�@�擾�������m���̐����i1�`100�܂Łj
*/
bool GetLikelihood( char num );
/*
Inventor replaces material with Host's material, that Host make space for inventor
return is A replaed value with B, that matrix of materials
*/
int*	ReplaceValue(int inventor, int replaceMaterial, int* lpHost, int hostMax);

#endif