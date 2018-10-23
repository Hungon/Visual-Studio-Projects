#include	"CSceneManager.h"
#include	"MemoryLeakCheck.h"
#include	<time.h>

//�G���g���[�|�C���g
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nShowCmd) {

	//���������[�N
#if	_DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif


	timeBeginPeriod(1);
	srand((unsigned)time(NULL));

	//�I�[�v�j���O����J�n
	CSceneManager cSceneManager = CSceneManager(GAMESCENE_OPENING);
	
	while (cSceneManager.Scene());
	
	//end the game
	cSceneManager.Release();

	return 0;
}