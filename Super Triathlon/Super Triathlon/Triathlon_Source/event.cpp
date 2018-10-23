#include <windows.h>

//************************************************************************************************
//Do Events関数
//************************************************************************************************
bool DoEvents() {

	MSG msg = { 0 };

	//メッセージループ
	while (1) {

		//メッセージの取得
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

			//メッセージが見つかった
			TranslateMessage(&msg);		//変換して
			DispatchMessage(&msg);		//転送

		}
		else {
			//メッセージがなかった
			return true;
		}
	}
}
