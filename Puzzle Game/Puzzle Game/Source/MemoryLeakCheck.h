// MemoryLeakCheck.h
//  メモリリークを調べるための記述をまとめたヘッダーファイルです。
//  メモリリークをしているかを調べたいソースファイルでインクルードしてください。
#define		_CRTDBG_MAP_ALLOC
#include	<stdlib.h>
#include	<crtdbg.h>

// new演算子再定義
/*
#ifdef		_DEBUG
   #ifndef		DBG_NEW
      #define		DBG_NEW		new ( _NORMAL_BLOCK, __FILE__, __LINE__ )
      #define		new			DBG_NEW
   #endif
#endif
*/
/* 以下の内容をスタートアップルーチン( WinMain / main関数 )の先頭に追加する！
#if	_DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
*/
