#ifndef		CENDING_H__
#define		CENDING_H__

#include	"CScene.h"
#include	<windows.h>

//�� �N���X�̒�`
// �V�[���Ǘ��p�N���X���p������
// �G���f�B���O�V�[���N���X���쐬����
class		CEnding: public CScene{

private:
	
	int		m_nBackImg[1];			// �w�i�摜�̗v�f�ԍ��ۑ��p
	//���W�Ȃǂ������L�����N�^�[�\����

public:

	//�� �ÓI�����o�ϐ��̒�`
	//�E�B���h�E�n���h��
	static HWND m_hWnd;
	//�摜�̎g�p����
	static const char CENDING_IMAGE_MAX;

	CEnding();
	~CEnding();

	// CScene�N���X�ɒ�`����Ă���
	// �������z�֐��̃I�[�o�[���C�h
	SCENE_NUM	Init();
	SCENE_NUM	Update();
	SCENE_NUM	Release();
	void		Draw();

};

#endif