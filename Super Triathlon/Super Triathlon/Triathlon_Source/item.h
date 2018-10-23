#ifndef __ITEM_H__
#define __ITEM_H__




//���}�N���̒�`
//�e�A�C�e���̕\����X���W
#define ITEM_1		0			//���x�㏸
#define ITEM_2		32			//���G
#define ITEM_3		64			//�g��
#define ITEM_4		96			//���x����
#define ITEM_5		128			//�k��

//���摜
#define ITEM_IMAGE_MAX		2				//�g�p����


#define ITEM_MAX			10				//�ő�A�C�e���\����

#define ITEM_APPEAR_RANDOM_MAX		49		//�A�C�e���𔭐������̏��
#define ITEM_APPEAR_RANDOM_MIN		0		//�A�C�e���̔��������̉���

//��
#define ITEM_SOUND_EFFECT_MAX	3	//���ʉ��̎g�p��


//���@�񋓌^
//�e��A�C�e���̎�ޔԍ�������
enum tagITEMTYPE{

	ITEM_VACANT = -1			,			//�A�C�e���������Ă��Ȃ����
	ITEM_TYPE_SPEED_UP			,			//���x�㏸
	ITEM_TYPE_ABSOLUTE			,			//���G
	ITEM_TYPE_BIG				,			//�g��
	ITEM_KIND_STRENGTH			,			//�����A�C�e���̎�ސ�
	ITEM_KIND_BOUND = 10		,			//�����A�C�e���Ǝ�̉��A�C�e���̋��E�ƂȂ�v�f��
	ITEM_TYPE_SPEED_DOWN = 10	,			//���x����
	ITEM_TYPE_SMALL				,			//�k��
	ITEM_KIND_WEAK = 2			,			//��̉��A�C�e���̎�ސ�
	ITEM_KIND = 5				,			//�A�C�e���̎�ސ�
};

//���w�b�_�[�t�@�C���̓ǂݍ���
#include "graphics.h"
#include "animation.h"
#include "swimer.h"


//���\���̂��`
typedef struct tagITEM{
	
	CHARACTER ch;		//�A�C�e���̊�{���
	float pos_x;		//�A�C�e���̏o�����WX
	float pos_y;		//�A�C�e���̏o�����WY
	int type;			//�A�C�e���̎�ޔԍ�
	int time;			//�e�A�C�e���̌��ʎ���
	int e_time;			//���ʎ���
	int displayX;		//�擾�ς݂̃A�C�e����\��X���W
	int displayY;		//�擾�ς݂̃A�C�e����\��Y���W
	int get[ITEM_KIND];	//��ޖ��̎擾��
	bool among;			//���ʒ�������
	bool have;			//�擾������
	float speed_up;		//���x�㏸
	float speed_down;	//���x����

}ITEM,*LPITEM;


//�֐��̒�`
void InitItem(HWND hWnd);		//������
void UpdateItem();				//�X�V
void DrawItem();				//�\��
void ReleaseItem();				//���

//������֐��̒�`

//�A�C�e�����o��������֐�
void CreateItem(float x,float y,int r);
//�A�C�e���Ƃ̔�������֐�
bool CollisionItem(SWIMER* ch);
//�A�C�e���g�p���̏���
void UseItem();
//�A�C�e���̌��ʂ����������邩�̔�����s��
void CheckResetEffect();
//�A�C�e���̏����擾����
ITEM* GetItem();


#endif

