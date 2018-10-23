#ifndef		CSCENE_H__
#define		CSCENE_H__

// SCENE_NUM�񋓌^
enum SCENE_NUM{

	SCENE_INIT = 0,				// ������
	SCENE_UPDATE,				// �X�V�E�`��
	SCENE_RELEASE,				// ���
	SCENE_END,					// �V�[���̏I��
	SCENE_ERROR					// �G���[

};

#include	"CFps.h"

// CScene�N���X
// 1�̃V�[�����Ǘ�����
class		CScene : public CFps{
private:
	char		m_SceneNum;					// �V�[���Ǘ��p�̒l
public:
	CScene();								// �R���X�g���N�^
	virtual ~CScene();						// �f�X�g���N�^
	
	bool				Scene();			// �V�[���J��
	// �e�Q�[���V�[�����ɏ������Ⴄ�̂ŁA�������z�֐��Ƃ��Ē�`����
	virtual SCENE_NUM	Init() = 0;			// ������
	virtual SCENE_NUM	Update() = 0;		// �X�V
	virtual SCENE_NUM	Release() = 0;		// ���
	virtual void		Draw() = 0;			// �`��
};

#endif