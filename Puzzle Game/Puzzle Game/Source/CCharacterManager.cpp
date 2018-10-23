#include	"CCharacterManager.h"
#include	"MemoryLeakCheck.h"
#include	"Likelihood.h"

//for get setting
TPOINT<float>		CCharacterManager::m_GetPos = { 0 };
TPOINT<float>		CCharacterManager::m_GetMove = { 0 };
TPOINT<int>			CCharacterManager::m_GetSize = { 0 };


CCharacterManager::~CCharacterManager() {}

/*
**************************************************************************************************
�����̒l����CCharacter�N���X���m�ۂ���
**************************************************************************************************
*/
void	CCharacterManager::SetCharacter( int charaMax ){

	//�����̒l���擾
	this->m_CharaMax = charaMax;
	//�����̒l����CCharacter�N���X�̃��������m��
	this->m_lplpCharaTbl = new CCharacter*[ charaMax ];
	//0�ŏ�����
	ZeroMemory( this->m_lplpCharaTbl , sizeof( CCharacter* ) * charaMax );

}

/*
***************************************************************************************************
������CCharacter�N���X���A�b�v�L���X�g����
�m�ۂ�����true��Ԃ�
***************************************************************************************************
*/
bool	CCharacterManager::SetUpCastCharacter( CCharacter *lpcChara ){
	
	//�߂�l�p
	bool ret = false;
	//�L�����N�^�[�̐��������[�v����
	for( int i = 0; i < this->m_CharaMax; i ++ ){

		//����������Ă�����m�ۂ���
		if( this->m_lplpCharaTbl[i] == NULL ){	
			//�A�b�v�L���X�g
			this->m_lplpCharaTbl[i] = lpcChara; 
			ret = true; 
		}
	}
	return ret;
}


