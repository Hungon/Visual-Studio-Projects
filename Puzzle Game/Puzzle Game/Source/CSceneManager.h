#ifndef		CSCENEMANAGER_H__
#define		CSCENEMANAGER_H__

// �w�b�_�C���N���[�h
#include	<windows.h>
#include	"CScene.h"								// CScene�N���X���p�����邽��

// GAMESCENE_NUMBER�񋓌^
enum	GAMESCENE_NUMBER{

	GAMESCENE_OPENING = 0	,						// �I�[�v�j���O
	GAMESCENE_PLAY_PUZZLE	,						//who is playing puzzle
	GAMESCENE_STORE,								//designed my knit to sell
	GAMESCENE_DESIGN,								//to design knit
	GAMESCENE_SELECT_MODE,							//who select mode to play
	GAMESCENE_GAMEOVER		,						// �Q�[���I�[�o�[
	GAMESCENE_ENDING		,						// �G���f�B���O
	GAMESCENE_NOTHING			,					// �����Ȃ�( NULL�̂悤�Ȃ��� )

};

//front define
class CWipeManager;

// CSceneManager�N���X
//  �Q�[���V�[�����Ǘ����邽�߂̃N���X
class		CSceneManager: public CScene{
private:
	static HWND					m_hWnd;				// �E�B���h�E�n���h��
	static GAMESCENE_NUMBER		m_NextGameScene;	// ���̃V�[����\��
	CScene						*m_lpcGameScene;	// ���݂̃Q�[���V�[���̃N���X���A�b�v�L���X�g�ň���
	CWipeManager	*m_lpCWipe;						//for create wipe
public:
	CSceneManager
	( GAMESCENE_NUMBER StartScene );
	~CSceneManager
	();

	// �����ł�CScene�N���X�̃����o�֐����I�[�o�[���C�h
	SCENE_NUM		Init();
	SCENE_NUM		Update();
	SCENE_NUM		Release();
	void			Draw();

	static void		SetNextScene		// ���̃V�[����ݒ肷��
	( GAMESCENE_NUMBER NextScene );
	
	static HWND		GetWindowHandle		// �E�B���h�E�n���h���̎擾
	();

private:
	CScene*		CreateGameScene			// �Q�[���V�[���̍쐬
	( GAMESCENE_NUMBER GameScene );		// ( �N���X�O����Ă΂��ƃ��������[�N�̌����ɂȂ�̂�private )
};

#endif