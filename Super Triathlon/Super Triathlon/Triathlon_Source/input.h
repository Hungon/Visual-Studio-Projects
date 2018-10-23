#ifndef __INPUT_H__


//▼マクロの定義
#define __INPUT_H__
//ボタンのマクロ
#define BUTTON_1 0x01
#define BUTTON_2 0x02
#define BUTTON_3 0x04
#define BUTTON_4 0x08
#define BUTTON_5 0x10
#define BUTTON_6 0x20
#define BUTTON_7 0x40
#define BUTTON_8 0x80

//十字キーのマクロ
#define LEVER_LEFT	0x01
#define LEVER_UP	0x02
#define LEVER_RIGHT 0x04
#define LEVER_DOWN	0x08

//▼ヘッダーファイルの定義
#include <windows.h>


//▼自作関数を定義

//プロタイプ宣言
void UpdateInput();					//入力状況の更新
bool JustUpKey(BYTE code);			//キーを離した瞬間を調べる関数
bool KeepKey(BYTE code);			//キーが押しっぱなしかを調べる関数
bool JustKey(BYTE code);			//キーを押した瞬間かを調べる関数
bool JustButton(int button);		//ボタン入力の設定
bool KeepButton(int button);		//ボタンの押しっぱなし
bool JustUpButton(int buton);		//ボタンを離した瞬間
bool JustLever(int lever);			//十字キーが押された瞬間を調べる
bool KeepLever(int lever);			//十字キーを押しっぱなしかを調べる
bool JustUpLever(int lever);		//十字キーを離した瞬間を調べる
/*
今回の入力コードを返す
*/
BYTE	GetInputKeyCode();

#endif
