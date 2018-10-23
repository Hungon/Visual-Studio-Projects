#ifndef		CSCOREMANAGER_H__
#define		CSCOREMANAGER_H__


//���w�b�_�[���C���N���[�h
#include	"CCharacter.h"
#include	<windows.h>

//kind of score
enum tagSCOREMANAGERNUMBERKIND {
	SCOREMANAGER_SCORENUMBER_COUNT_DOWN_TIME,
	SCOREMANAGER_KIND_OF_SCORENUMBER,
};

//front declaration
class CScore;
class CPieceManager;

//define class scoremanager
class CScoreManager{

private:

	//�摜�̗v�f������
	int		*m_lpScoreManagerImg;
	//�����摜�p
	CScore	*m_lpCScoreNumber;
	//for count
	int		m_millisecond;
	//for get present drawing number score
	static int	*m_lpPresentNumber;
	CPieceManager *m_lpCPieceManager;		//piecemanager class for get flag of complete

protected:
	
public:

	//�� �ÓI�����o�ϐ��̒�`
	static const int SCOREMANAGER_SCORE_IMAGE_MAX;			//�摜�g�p����
	//�E�B���h�E�n���h��
	static HWND	m_hWnd;

	//���v���g�^�C�v�錾
	//�R���X�g���N�^
	CScoreManager() :
		m_millisecond(0)
	{}
	//�f�B�X�g���N�^
	~CScoreManager();
	
	void InitScoreManager();					//������
	void UpdateScoreManager();					//�X�V
	void DrawScoreManager();					//�\��
	void ReleaseScoreManager();					//���
		
	//get selected value of score number
	int		GetScoreNumber(tagSCOREMANAGERNUMBERKIND kind)const { return this->m_lpPresentNumber[kind]; }
};


#endif