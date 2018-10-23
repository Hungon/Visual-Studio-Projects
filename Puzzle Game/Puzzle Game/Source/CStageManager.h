#ifndef		CStageManager_H__
#define		CStageManager_H__

#include	<windows.h>
#include	"CStage.h"

//�� �񋓌^
//�X�e�[�W�ԍ�������
enum	tagSTAGENUMBER{

	STAGE_1 ,
	STAGE_2 ,

};

//�O���錾

//�� �N���X�̒�`
// �v���C���[�N���X���쐬����
class		CStageManager{

private:
	
	// �w�i�摜�̗v�f�ԍ��ۑ��p
	int		*m_lpStageImg;

	//�X�e�[�W������
	CStage	*m_lpCStage;

	//�X�e�[�W�ԍ�������
	tagSTAGENUMBER		m_StageNum;


protected:


public:

	//�� �ÓI�����o�ϐ��̒�`
	//�E�B���h�E�n���h��
	static	HWND m_hWnd;
	//�摜�̎g�p����
	static const int	CSTAGEMANAGER_IMAGE_MAX;

	//�摜�T�C�Y
	//�@�w�i
	static const int	CSTAGEMANAGER_BACK_IMAGE_WIDTH;		//��
	static const int	CSTAGEMANAGER_BACK_IMAGE_HEIGHT;	//����

	//�� ����֐��̒�`
	//�R���X�g���N�^
	CStageManager();
	//�f�B�X�g���N�^
	~CStageManager();
	void	InitStageManager();				//������
	void	UpdateStageManager();			//�X�V	
	void	DrawStageManager();				//�\��
	void	ReleaseStageManager();			//���

	//�X�e�[�W�ԍ���ݒ肷��
	void	SetStageNumber( tagSTAGENUMBER stageNum ){ this->m_StageNum = stageNum; }

	//�X�e�[�W�ԍ����擾����
	tagSTAGENUMBER	GetStageNumber() const { return this->m_StageNum; }

	//�� �������z�֐��̃I�[�o�[���[�h

};


#endif