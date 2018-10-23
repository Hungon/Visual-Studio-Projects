#include	<iostream>
#include	"CSceneManager.h"
#include	"COpening.h"
#include	"CPlayPuzzle.h"
#include	"CStore.h"
#include	"CDesign.h"
#include	"CSelectMode.h"
#include	"CGameOver.h"
#include	"CEnding.h"
#include	"window.h"
#include	"event.h"
#include	"input.h"
#include	"graphics.h"
#include	"sound.h"
#include	"CWipeManager.h"

using namespace std;

// �ÓI�����o�֐��̏�����
HWND			CSceneManager::m_hWnd = NULL;
GAMESCENE_NUMBER	CSceneManager::m_NextGameScene = GAMESCENE_NOTHING;

//�R���X�g���N�^
CSceneManager::CSceneManager
( GAMESCENE_NUMBER StartScene )
{
	// �V�[���̍쐬
	// �A�b�v�L���X�g�Ŏ󂯎��
	this->m_lpcGameScene = this->CreateGameScene( StartScene );

	// �E�B���h�E�n���h���̏�����
	this->m_hWnd = NULL;
}


//�f�B�X�g���N�^
CSceneManager::~CSceneManager
()
{
	this->Release();
}


// CSceneManager::CreateGameScene
//
// ����
//  GAMESCENE_NUMBER GameScene		�쐬����Q�[���V�[����\���l
//
// �߂�l
//  CScene*						GameScene�ɑΉ�����Q�[���V�[�����m�ۏo�����炻�̃I�u�W�F�N�g�̃A�h���X
//								�m�ۂɎ��s�����ꍇ��NULL
//
// ����
//  GameScene�ɍ��킹�ăQ�[���V�[���N���X�̍쐬���s���܂��B
CScene*		CSceneManager::CreateGameScene
( GAMESCENE_NUMBER GameScene )
{

	// �����̃Q�[���V�[���̒l�ɍ��킹�Ĕh���N���X���m�ۂ��A���̂܂ܖ߂�l�ɂ���
	switch ( GameScene ){

		case	GAMESCENE_OPENING:
			return	new	COpening();

		case	GAMESCENE_PLAY_PUZZLE:
			return	new CPlayPuzzle();

		case	GAMESCENE_STORE:
			return new CStore();

		case	GAMESCENE_DESIGN:
			return new CDesign();

		case GAMESCENE_SELECT_MODE:				//during select mode
			return new CSelectMode();
		
		case	GAMESCENE_GAMEOVER:
			return	new CGameOver();

		case	GAMESCENE_ENDING:
			return	new CEnding();


	}

	// switch���Ɉ���������Ȃ�������GAMESCENE_NOTHING��A�ǉ����Y��ɂȂ�̂�NULL��Ԃ�
	return	NULL;
}


/*
*******************************************************************************************
�Q�[���S�̂̏�����
*******************************************************************************************
*/
SCENE_NUM		CSceneManager::Init()
{
	//create window
	CSceneManager::m_hWnd = InitWindow( "MY GAME" , 0 , 0 );

	//�E�B���h�E���[�h�̏�����
	if( InitDDWindow( CSceneManager::m_hWnd ) == false ){
		MessageBox( NULL , "DD���������s\n" , "CManagerInit" , MB_OK );
		return 	SCENE_ERROR;
	}
	//DirectSound�̏�����
	if( InitDirectSound( CSceneManager::m_hWnd ) == false ){	
		MessageBox( NULL , "DS�ǂݍ��ݎ��s" , "CManagerInit" , MB_OK );
		return 	SCENE_ERROR;
	}

	//allot memory
	this->m_lpCWipe = new CWipeManager;				//for create wipe

	this->m_lpCWipe->InitWipeManager();				//initialize wipe

	return	SCENE_UPDATE;

}


/*
*******************************************************************************************
�Q�[���S�̂̍X�V
*******************************************************************************************
*/
SCENE_NUM		CSceneManager::Update()
{

	// �Q�[���V�[�����m�ۂ���Ă��邩�𒲂ׂ�
	if (this->m_lpcGameScene == NULL)				return	SCENE_ERROR;

	//�ŏ��ɃL�[�̍X�V
	UpdateInput();

	//�t���[���̍ŏ��ɏ����𒲂ׂ�
	RestoreSurface(CSceneManager::m_hWnd);


	//�� �Q�[���V�[���̏������s��
	// �Q�[���V�[�����I������ꍇ��if���̒��̏������s��
	if (this->m_lpCWipe->UpdateWipeManager() == false) {			//when don't create wipe, transition to next scene
		if (!this->m_lpcGameScene->Scene()) {
			// �m�ۂ��Ă����Q�[���V�[���̉���E�s���Q�Ɩh�~
			delete this->m_lpcGameScene;
			this->m_lpcGameScene = NULL;
			// ���̃V�[�����ݒ肳��ĂȂ���������������
			if (CSceneManager::m_NextGameScene == GAMESCENE_NOTHING)	return	SCENE_RELEASE;
			// ���̃Q�[���V�[�����m��
			this->m_lpcGameScene = this->CreateGameScene(CSceneManager::m_NextGameScene);
			// ���̃V�[���𖳂��ɐݒ�
			CSceneManager::m_NextGameScene = GAMESCENE_NOTHING;
		}
	}
	//���b�Z�[�W�Ǘ�����ɍs��
	DoEvents();

	return	SCENE_UPDATE;

}

/*
*******************************************************************************************
�Q�[���S�̂̉��
*******************************************************************************************
*/
SCENE_NUM		CSceneManager::Release()
{
	// �Q�[����1�x�����s��Ȃ��������
	// DirectDraw��DirectSound�Ȃǂ̉�����s��

	//���Ԑ��x�̃��Z�b�g
	timeEndPeriod(1);

	// �Q�[���V�[�����m�ۂ���Ă�����
	if ( this->m_lpcGameScene != NULL ){
		// �Q�[���V�[���̉��
		this->m_lpcGameScene->Release();
		// ����E�s���Q�Ɩh�~
		delete this->m_lpcGameScene;
		this->m_lpcGameScene = NULL;
	}
	//DirectDraw�̉��
	ReleaseDirectDraw();
	//DirectSound�̉��
	ReleaseDirectSound();


	return	SCENE_END;
}


/*
*******************************************************************************************
�Q�[���S�̂̕\��
*******************************************************************************************
*/
void			CSceneManager::Draw()
{
	// �u�e�Q�[���V�[����Draw�v���uCSceneManager�N���X��Draw�v
	// �̏��ԂŌĂ΂��̂ŁA�g�[�N�⃏�C�v�̕`��A�ŏI�`���������
	
	//�n���h���R���e�L�X�g
	HDC hDC = NULL;
	//�o�b�N�T�[�t�F�C�X�̎擾
	LPDIRECTDRAWSURFACE7 lpDDSBack = GetBackSurface();

	//draw wipe
	this->m_lpCWipe->DrawWipeManager();

	//���z��ʂ�hDC���擾
	lpDDSBack->GetDC( &hDC );

	//���z��ʂ�hDC�����
	lpDDSBack->ReleaseDC( hDC );

	//�ŏI�o��
	UpdatePrimary( CSceneManager::m_hWnd );

}



// CSceneManager::SetNextScene
//
// ����
//  GAMESCENE_NUMBER GameScene		�쐬����Q�[���V�[����\���l
//
// �߂�l
//  ����
//
// ����
//  ���̃V�[����ݒ肵�܂��B
void			CSceneManager::SetNextScene
( GAMESCENE_NUMBER NextScene )
{

	// ���̃V�[����\���l��ݒ�
	CSceneManager::m_NextGameScene = NextScene;
}


// CSceneManager::GetWindowHandle
//
// ����
//  ����
//
// �߂�l
//  HWND						�E�B���h�E�n���h��
//
// ����
//  �E�B���h�E�n���h�����擾���܂��B
HWND			CSceneManager::GetWindowHandle
()
{
	return	CSceneManager::m_hWnd;
}
