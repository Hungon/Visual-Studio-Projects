#ifndef __SOUND_H__
#define __SOUND_H__

#include <dsound.h>

//マクロ
//最大曲数
#define SOUND_MAX	30
//1秒分のバイト数
#define SECOND	176400

//ストリーミング構造体
typedef struct tagSTREAMING{
	LPDIRECTSOUNDBUFFER buf;	//2秒分のバッファ
	char pass[256];				//ファイルパス
	long startOffset;			//音データまでのバイト数
	long readOffset;			//次の読み込み位置
	DWORD oneSecondBytes;		//1秒分のサイズ
	DWORD musicSize;			//1曲分のサイズ
	DWORD readSize;				//今回読み込んだサイズ
	bool fFront;				//前半、後半の読み込み場所
	bool fPlay;					//再生中かどうか
	bool fRead;					//読み込みが必要かどうか
}STREAMING;

//プロトタイプ宣言
//初期化
bool InitDirectSound(HWND hWnd);
//解放
void ReleaseDirectSound();
//指定曲の再生
void PlaySound(int id,int bigintime,bool loop);
//指定曲の停止
void StopSound(int id);
//指定曲の解放
void ReleaseSound(int id);
//曲の読み込み
int CreateSound(char* pass);
//セカンダリバッファの生成
bool CreateStreaming(char* pass);
//ストリーミング再生を行う
void PlayStreaming(bool play);
//ストリーミングの解放
void ReleaseStreaming();

#endif