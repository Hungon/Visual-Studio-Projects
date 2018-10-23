#include "window.h"
#include "game.h"
#include "event.h"
#include "fps.h"
#include "input.h"
#include "graphics.h"
#include "record.h"
#include "MemoryLeakCheck.h"
#include <time.h>

//************************************************************************************************
//main�֐��@�Q�[�����S�ʂ̏���
//************************************************************************************************
//�G���g���[�|�C���g
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nShowCmd) {

//���������[�N
#if	_DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	HWND hWnd = NULL;

	srand((unsigned)time(NULL));

	//words of the title
	char	*lpTitle[GAME_KIND_OF_LANGUAGE] = {
		{"���l�g���C�A�X����"},		//Japanese
		{"SuperTriathlon"},			//English
	};
	int language;					//for divergence
	//get language, that player changed language in the past
	language = LoadRecordFile("Data\\Language.dat", RECORD_PREVIEW)->recordBox[0][0];
	//set present language
	SetGameLanguage((tagGAMELANGUAGE)language);
	//�E�B���h�E����
	hWnd = InitWindow(lpTitle[language], 0, 0);

	//�Q�[���̏�����
	InitGame(hWnd);

	//���Ԃ̐��x��ݒ�
	timeBeginPeriod(1);

	//�Q�[�����[�v�̍쐬
	while (IsWindow(hWnd)) {

		//FPS�̐���
		if (WaitFps()) {
			//�t���[���̍ŏ��ɏ����𒲂ׂ�
			RestoreSurface(hWnd);
			//�V�[���ύX�̊m�F
			CheckScene(hWnd);
			//�Q�[���̍X�V
			UpdateGame();
			//�Q�[���̕`��
			DrawGame(hWnd);
		}
		//���b�Z�[�W�Ǘ�����ɍs��
		DoEvents();
	}
	//���Ԑ��x�̃��Z�b�g
	timeEndPeriod(1);
	//�Q�[���̉��
	ReleaseGame();
	//����I��
	return 0;
}
