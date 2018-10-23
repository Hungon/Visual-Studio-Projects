#include	"CScene.h"
#include	"graphics.h"
#include	"input.h"

// �R���X�g���N�^
CScene::CScene(){
	// �V�[���͕K������������n�߂�悤�ɂ���
	this->m_SceneNum = SCENE_INIT;
}

// �f�X�g���N�^
CScene::~CScene(){
}

// CScene::Scene
//
// ����
//  ����
//
// �߂�l
//  bool						true:	�Q�[���V�[���̏�����
//								false:	�Q�[���V�[���̏I���
//
// ����
//  �V�[���J�ڂ��s���܂��B
bool	CScene::Scene(){


	//strict fps
	if (this->WaitFps()) {
		// �V�[���Ǘ��l�ɂ���ĕ���
		switch (this->m_SceneNum) {

		case	SCENE_INIT:

			// ����������
			this->m_SceneNum = this->Init();
			break;

		case	SCENE_UPDATE:

			// �X�V����
			this->m_SceneNum = this->Update();
			// �`�揈��
			this->Draw();
			//draw fps
			this->DrawFps(0, 0);
			//end the game
			if (JustKey(VK_ESCAPE)) this->m_SceneNum = SCENE_END;
			break;

		case	SCENE_RELEASE:

			// �������
			this->m_SceneNum = this->Release();

			break;
		}

		// �G���[�̒l���Ԃ��Ă�����
		if (this->m_SceneNum == SCENE_ERROR) {

			// ���͉���������s���悤�ɕύX
			this->m_SceneNum = SCENE_RELEASE;
		}

		// �I���̒l���Ԃ��Ă�����
		if (this->m_SceneNum == SCENE_END) {

			// �V�[���̏I���
			return	false;
		}
	}

	// �V�[���̏�����
	return	true;

}
