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
	this->m_Ipic.x = 0;				//�\�������WX
	this->m_Ipic.y = 0;				//�\�������WY
	this->m_Fmove.x = 0.0f;			//�ړ���X
	this->m_Fmove.y = 0.0f;			//�ړ���Y
	this->m_existFlag = false;		//�����t���O
	this->m_Isize.x = 0;			//��
	this->m_Isize.y = 0;			//����
	this->m_alpha = 0.0f;			//���ߔ{��
	this->m_scale = 0.0f;			//�g�k��
	this->m_direction = 0;			//����
	this->m_kindNum = 0;			//��ޔԍ�

}

/*
**************************************************************************************************
�f�B�X�g���N�^
**************************************************************************************************
*/
CCharacter::~CCharacter(){

}
