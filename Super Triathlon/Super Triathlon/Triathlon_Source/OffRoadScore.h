#ifndef COFFROADSCORE_H__
#define COFFROADSCORE_H__


//���w�b�_�[���C���N���[�h
#include	"CCharacter.h"

//���\���̂̒�`

//�� �I�t���[�h�X�R�A�N���X�̒�`
class COffRoadScore : public CCharacter{

private:


protected:

public:


	//���v���g�^�C�v�錾

	//�R���X�g���N�^
	COffRoadScore();
	//�f�B�X�g���N�^
	~COffRoadScore();
	
	
	//�� �������z�֐��̒�`
	void	InitChara();				//������
	void	UpdateChara();				//�X�V	
	void	DrawChara();				//�\��
	void	ReleaseChara();				//���

};


#endif