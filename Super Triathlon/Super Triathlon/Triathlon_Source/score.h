#ifndef __SCORE_H__
#define __SCORE_H__


//���}�N���̒�`
//�摜
#define SCORE_IMAGE_MAX		4				//�����摜�̖���
//�i���o�[�̗v�f��
#define SCORE_NUMBER_MAX	6
//�]���̗v�f��
#define SCORE_VALUE_MAX		4

//kind of number image
#define SCORE_NUMBER_TYPE_NORMAL	0					//normal
#define SCORE_NUMBER_TYPE_SPEED		1					//speed
#define SCORE_NUMBER_TYPE_RANKING	2					//ranking
#define SCORE_NUMBER_KIND_OF_TYPE	3					//kind

//default value for normal number image
//kind
const char SCORE_NUMBER_TYPE_NORMAL_KIND_OF_TYPE = 2;		//normal
const char SCORE_NUMBER_TYPE_SPEED_KIND_OF_TYPE = 4;		//speed
//initialize indication
const int SCORE_NUMBER_AND_PUNCTUAL_IMAGE_WIDTH = 64;	//width
const int SCORE_NUMBER_AND_PUNCTUAL_IMAGE_HEIGHT = 96;	//height

//�X�R�A�\�����o	
#define	SCORE_GRADUALLY_DIRECT_NORMAL	0	//�ʏ�̌��Z�E���Z������A�\��
#define	SCORE_GRADUALLY_DIRECT_ROLLING	1	//�w�肵���t���[����������]��A�\��


#define NO_VALUE		-1			//�]���Ȃ�
#define SCORE_CHAIN		0			//CHAIN
#define SCORE_GOOD		1			//GOOD
#define SCORE_COOL		2			//COOL
#define SCORE_EXCELLENT 3			//EXCELLENT
//�X�R�A�摜�̕\���ݒ�
const float SCORE_VALUE_COMMON_STAY_INDICATION_X = 700.0f;
const int	SCORE_VALUE_IMAGE_WIDTH_COMMON = 160;
const int	SCORE_VALUE_IMAGE_HEIGHT_COMMON = 96;
const int	SCORE_VALUE_IMAGE_WIDTH_EXCELLENT = 288;			//EXCELLENT
const float	SCORE_VALUE_IMAGE_SCALE_COMMON = 0.6f;
const float	SCORE_VALUE_DEFAULT_SPEED_X = 5.0f;
//�X�R�A�摜�̑؍ݎ���
const int SCORE_VALUE_STAY_TIME = 150;

//ranking image
const int SCORE_NUMBER_RANKING_IMAGE_WIDTH = 64;	//width
const int SCORE_NUMBER_RANKING_IMAGE_HEIGHT = 64;	//height


//���񋓌^
//�X�R�A�̐F
enum tagSCORECOLOR{
	
	SCORE_BLACK				,		//��
	SCORE_RED				,		//��
	SCORE_BLUE				,		//��
	SCORE_YELOW				,		//��
	SCORE_GREEN				,		//��
	SCORE_COLOR_VALUATION	,		//�F�̎��
};

//score's color for ranking
enum tagSCORERANKINGCOLOR {
	SCORE_RANING_COLOR_BLACK,
	SCORE_RANING_COLOR_WHITE,
	SCORE_RANING_COLOR_RED,
	SCORE_RANING_COLOR_GREEN,
	SCORE_RANING_COLOR_BLUE,
	SCORE_RANING_COLOR_YELOW,
	SCORE_KIND_OF_RANKING_COLOR,
};

//�X�R�A�̕�����
enum tagSCOREDESIGN{

	SCORE_DESIGN_VIRTUALLY	,	//����
	SCORE_DESIGN_ITALIC		,	//�Α�
};

//���w�b�_�[���C���N���[�h
#include "graphics.h"
#include "record.h"


//�X�R�A�\���̂̒�`
typedef struct tagSCOREVALUE{

	CHARACTER ch;							//�L�����N�^�[�������\����
	RECORD re;								//�L�^�������\����
	TIME ti;								//���Ԃ������\����
	float num;								//�\���������l
	float numD;								//�\������l
	float sub;								//�\���������l�ƕ\������l�̍���
	int interval;							//�t���[�����Ƃɕ\�����鐔�l�Ԋu
	int digit;								//�\�����錅��
	float score_x;							//�X�R�A�p�\����X
	float score_y;							//�X�R�A�p�\����Y
	float score_posx;						//�ҋ@���W�w
	float score_scale;						//�X�R�A�p�g�k��
	float spaceX;							//���\���Ԋu
	float spaceY;							//�c�\���Ԋu
	bool dicimals_flag;						//�����\���t���O
	bool kiloFlag;							//km�\���t���O
	bool gradually;							//���X�ɒl�𑝉�����t���O
	bool directEnd;							//���o�I���t���O
	char graduallyDirect;					//���X�ɉ��Z�E���Z���鑬�x������
	char scoreColor;						//�\������X�R�A�̐F
	char renewalColor;						//�X�V���̃X�R�A�̐F
	tagSCOREDESIGN italic;					//�Α̃t���O
	int rollTime;							//��]����
	char renealF;							//�\���p�X�V�t���O

}SCOREVALUE,*LPSCOREVALUE;


//���v���g�^�C�v�錾
//������
void InitScore(HWND hWnd);

//�X�V
void UpdateValueation();

//�摜�ŃX�R�A��\������
void DrawScore( SCOREVALUE* pscore );

//���
void ReleaseScore();

//���Ԃ�\������
void DrawTime( SCOREVALUE* ps );

//�]����\������
int DrawValueation(float x , float y , int count);

//�ڕW�̒l�܂ŏ��X�ɉ��Z�i���Z�j����
int GraduallyNumber( SCOREVALUE* pscore );

//�X�R�A�\�����o�P�u�t���[�����ɒl�����Z�E���Z�v
void GraduallyDirectNormal( SCOREVALUE* pscore );

//�X�R�A�\�����o�Q�u�w��t���[������]��A�\���v
void GraduallyDirectRolling( SCOREVALUE* pscore );
/*
draw player's ranking in road stage
*/
void	DrawRanking(float x, float y, float scale, int ranking, tagSCORERANKINGCOLOR color);

#endif