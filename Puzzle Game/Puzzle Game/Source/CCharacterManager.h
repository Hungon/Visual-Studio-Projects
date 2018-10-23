#ifndef		CCHARACTERMANAGER_H__
#define		CCHARACTERMANAGER_H__

#include	<windows.h>
#include	"CCharacter.h"


//CCharacter�N���X���Ǘ�����
class	CCharacterManager{

private:

	//CCharacter�N���X�������f�[�^�e�[�u��
	CCharacter	**m_lplpCharaTbl;

	//CCharacter�N���X���p�������L�����N�^�[�̍ő吔
	int	m_CharaMax;

protected:

public:

	//for get setting
	static TPOINT<float>		m_GetPos;
	static TPOINT<float>		m_GetMove;
	static TPOINT<int>			m_GetSize;


	CCharacterManager() :
		m_lplpCharaTbl(NULL) ,
		m_CharaMax(0)
	{}
	~CCharacterManager();

	virtual void	InitManagerCharacter() = 0;				
	virtual void	UpdateManagerCharacter() = 0;
	virtual void	DrawManagerCharacter() = 0;				
	virtual void	ReleaseManagerCharacter() = 0;			

	/*
	**************************************************************************************************
	�����̒l����CCharacter�N���X���m�ۂ���
	**************************************************************************************************
	*/
	void	SetCharacter( int charaMax );		
	/*
	***************************************************************************************************
	������CCharacter�N���X���A�b�v�L���X�g����
	�m�ۂ�����true��Ԃ�
	***************************************************************************************************
	*/
	bool	SetUpCastCharacter( CCharacter *lpcChara );

	//�e��擾�p�֐�
	TPOINT<float>	GetCharacterManagerPosition() const { return this->m_GetPos; }		//���W
	TPOINT<float>	GetCharacterManagerMove() const { return this->m_GetMove; }			//�ړ���
	TPOINT<int>		GetCharacterManagerSize() const { return this->m_GetSize; }			//�T�C�Y

};


#endif