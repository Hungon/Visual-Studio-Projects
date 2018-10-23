#ifndef		CPLAYERMANAGER_H__
#define		CPLAYERMANAGER_H__


//�� �w�b�_�[�̃C���N���[�h
#include	<windows.h>
#include	"CCharacter.h"

//�� �O���錾
class	CPlayer;
class	CPieceManager;

//�� �N���X�̒�`
// �v���C���[�Ǘ��N���X
class		CPlayerManager : public CCharacter{

private:
	
	//�����摜�v�f�ԍ�
	int		*m_lpPlayerImg;

	//�s�[�X�Ǘ��p
	CPieceManager	*m_lpCPieceManager;

	//�v���C���[�N���X
	CPlayer		*m_lpCPlayer;

	//�v���C���[�̍��W������
	static TPOINT<float>	m_ArrowPosition;


public:

	//�� �ÓI�����o�ϐ��̒�`
	//�E�B���h�E�n���h��
	static HWND m_hWnd;
	//�摜�̎g�p����
	static const int CPLAYERMANAGER_IMAGE_MAX;
	//�v���C���[�̃T�C�Y
	static const TPOINT<int> CPLAYERMANAGER_ARROW_SIZE;
	//�v���C���[�̊g�k�{��
	static const float	CPLAYERMANAGER_DEFAULT_SCALE;


	//�� �v���g�^�C�v�錾
	//�f�t�H���g�R���X�g���N�^
	CPlayerManager() : m_lpPlayerImg(NULL) , m_lpCPieceManager(NULL) , m_lpCPlayer(NULL){}
	//�f�B�X�g���N�^
	~CPlayerManager();
	
	//�摜�̉��
	void	ReleaseImagePlayerManager();

	//�N���X�̉��
	void	ReleaseClassPlayerManager();	

	//���̈ړ�
	void	MoveArrow();
	//���̈ړ�����
	void	ConstrainArrowMove();

	//���̍��W��Ԃ�
	TPOINT<float>	GetArrowPosition() const { return CPlayerManager::m_ArrowPosition; }

	//�� �������z�֐��̒�`
	void	InitChara();				//������
	void	UpdateChara();				//�X�V	
	void	DrawChara();				//�\��
	void	ReleaseChara();				//���

};

#endif