#ifndef		FILEVALUE_H__
#define		FILEVALUE_H__



//▼　ヘッダーのインクルード
#include	<windows.h>


//ファイルの項目を扱う「FILEVALUE構造体」
typedef struct tagFILEVALUE {

	int fileBox[30][256];		//記録を数値として扱う
	
}FILEVALUE , *LPFILEVALUE;

/*
load value from selected file
*/
FILEVALUE* LoadFile(char* lpPass);
/*
save value into selected file
*/
void	SaveFile(char* lpPass, int kind, int* lpItem , FILEVALUE* lpRecord);

#endif