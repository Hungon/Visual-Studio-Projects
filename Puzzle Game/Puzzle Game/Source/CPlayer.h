#ifndef		CPlayer_H__
#define		CPlayer_H__

#include	"CCharacter.h"
#include	<windows.h>


//�O���錾
class	CPieceManager;
class	CPiece;

//�� �N���X�̒�`
// �v���C���[�N���X���쐬����
class		CPlayer : virtual public CCharacter{

private:
	

protected:


public:

	//�� �ÓI�����o�ϐ��̒�`
	

	//�� ����֐��̒�`
	//�R���X�g���N�^
	CPlayer();
	//�f�B�X�g���N�^
	~CPlayer();

	//�� �������z�֐��̃I�[�o�[���[�h
	// CCharacter�N���X
	void	InitChara();				//������
	void	UpdateChara();				//�X�V	
	void	DrawChara();				//�\��
	void	ReleaseChara();				//���

};


#endif