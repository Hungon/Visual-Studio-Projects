#include	"CCharacter.h"
#include	"MemoryLeakCheck.h"
#include	<math.h>



/*
**************************************************************************************************
�R���X�g���N�^
**************************************************************************************************
*/
CCharacter::CCharacter(){

	this->m_Fpos.x = 0.0f;			//�\������WX
	this->m_Fpos.y = 0.0f;			//�\������WY
	this->m_StartFpos.x = 0.0f;		//starting coordinateX
	this->m_StartFpos.y = 0.0f;		//starting coordinateY
	this->m_Ipic.x = 0;				//�\�������WX
	this->m_Ipic.y = 0;				//�\�������WY
	this->m_Fmove.x = 0.0f;			//�ړ���X
	this->m_Fmove.y = 0.0f;			//�ړ���Y
	this->m_existFlag = false;		//�����t���O
	this->m_Isize.x = 0;			//��
	this->m_Isize.y = 0;			//����
	this->m_alpha = 0.0f;			//���ߔ{��
	this->m_scale = 0.0f;			//�g�k��
	this->m_kindNum = 0;			//��ޔԍ�
}

/*
**************************************************************************************************
�f�B�X�g���N�^
**************************************************************************************************
*/
CCharacter::~CCharacter(){

}
/*
***********************************************************************************************
Culculate character's center position and rotate process
************************************************************************************************
*/
void CCharacter::CenterCharacter(int len, int angle) {

	//��]���S���Z�o
	int cx = (int)this->m_StartFpos.x + (this->m_Isize.x >> 1);
	int cy = (int)this->m_StartFpos.y + (this->m_Isize.y >> 1);

	//��]��̍��W�����߂�
	this->m_Fpos.x = (float)cos(angle * 3.14 / 180.0) * len + cx - (this->m_Isize.x >> 1);
	this->m_Fpos.y = (float)sin(angle * 3.14 / 180.0) * len + cy - (this->m_Isize.y >> 1);
}


/*
************************************************************************************************
Update bezier drawing
************************************************************************************************
*/
void CCharacter::SetBezier(TPOINT<int> *lpPos, int bezier, double time) {

	int set = bezier - 1;
	//��X���W
	//�n�_���W�����߂�
	float sx = lpPos[0].x * (float)(pow((1 - time), set));
	//���ԍ��W
	float cx = 0.0f;
	//�I�_���W�����߂�
	float ex = lpPos[set].x * (float)(pow(time, set));

	//��Y���W
	//�n�_���W�����߂�
	float sy = lpPos[0].y * (float)(pow((1 - time), set));
	//���ԍ��W
	float cy = 0.0f;
	//�I�_���W�����߂�
	float ey = lpPos[set].y * (float)(pow(time, set));

	//�n�_����
	this->m_Fpos.x = sx;			//X
	this->m_Fpos.y = sy;			//Y

	//���S���W�����߂�
	for (int i = 1; i < set; i++) {
		//X���W
		this->m_Fpos.x += lpPos[i].x * set * (float)(pow((1 - time), (set - i)))
			* (float)(pow(time, i));
		//Y���W
		this->m_Fpos.y += lpPos[i].y * set * (float)(pow((1 - time), (set - i)))
			* (float)(pow(time, i));
	}
	//�I�_���Ō�ɉ��Z
	this->m_Fpos.x += ex;		//X
	this->m_Fpos.y += ey;		//Y
}