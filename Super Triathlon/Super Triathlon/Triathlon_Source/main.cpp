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
//main関数　ゲーム内全般の処理
//************************************************************************************************
//エントリーポイント
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nShowCmd) {

//メモリリーク
#if	_DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	HWND hWnd = NULL;

	srand((unsigned)time(NULL));

	//words of the title
	char	*lpTitle[GAME_KIND_OF_LANGUAGE] = {
		{"超人トライアスロン"},		//Japanese
		{"SuperTriathlon"},			//English
	};
	int language;					//for divergence
	//get language, that player changed language in the past
	language = LoadRecordFile("Data\\Language.dat", RECORD_PREVIEW)->recordBox[0][0];
	//set present language
	SetGameLanguage((tagGAMELANGUAGE)language);
	//ウィンドウ生成
	hWnd = InitWindow(lpTitle[language], 0, 0);

	//ゲームの初期化
	InitGame(hWnd);

	//時間の精度を設定
	timeBeginPeriod(1);

	//ゲームループの作成
	while (IsWindow(hWnd)) {

		//FPSの制御
		if (WaitFps()) {
			//フレームの最初に消失を調べる
			RestoreSurface(hWnd);
			//シーン変更の確認
			CheckScene(hWnd);
			//ゲームの更新
			UpdateGame();
			//ゲームの描画
			DrawGame(hWnd);
		}
		//メッセージ管理を常に行う
		DoEvents();
	}
	//時間精度のリセット
	timeEndPeriod(1);
	//ゲームの解放
	ReleaseGame();
	//正常終了
	return 0;
}
