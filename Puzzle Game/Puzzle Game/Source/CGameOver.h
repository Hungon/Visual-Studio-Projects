#ifndef		CGAMEOVER_H__
#define		CGAMEOVER_H__

#include	"CScene.h"
#include	<windows.h>

//�� �N���X�̒�`
// �V�[���Ǘ��p�N���X���p������
// �Q�[���I�[�o�[�V�[���N���X���쐬����
class		CGameOver: public CScene {

private:
	
	int		m_nBackImg[1];			// �w�i�摜�̗v�f�ԍ��ۑ��p
	//���W�Ȃǂ������L�����N�^�[�\����

public:

	//�� �ÓI�����o�ϐ��̒�`
	//�E�B���h�E�n���h��
	static HWND m_hWnd;
	//�摜�̎g�p����
	static const char CGAMEOVER_IMAGE_MAX;

	CGameOver();
	~CGameOver();

	// CScene�N���X�ɒ�`����Ă���
	// �������z�֐��̃I�[�o�[���C�h
	SCENE_NUM	Init();
	SCENE_NUM	Update();
	SCENE_NUM	Release();
	void		Draw();

};

#endif