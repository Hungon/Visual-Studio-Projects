#ifndef __TALK_H__
#define __TALK_H__

//▼マクロの定義
#define TALK_LOAD_MAX		10		//読み込める最大ファイル数

//▼ヘッダーをインクルード
#include "window.h"
#include "graphics.h"

//▼構造体の定義
typedef struct tagTALK{

	char mes[256];			//表示数する文字列
	char pass[256];			//読み込みファイル
	int word;				//現在表示した文字数
	int restrictive_word;	//表示文字制限
	int size;				//文字サイズ
	int line_space;			//行間の間隔
	int cnt;				//経過時間
	int next;				//表示間隔
	int readoffset;			//読み込み位置
	float baseX;			//文字表示座標Ｘ
	float baseY;			//文字表示座標Ｙ
	bool fRead;				//読み込みフラグ
	bool fShow;				//表示フラグ
	bool fDelete;			//表示終了フラグ
	bool fNext;				//新しいファイルを読み込むフラグ
	COLORREF change_color;	//色変更用
	COLORREF default_color;	//色の初期設定

}TALK,*LPTALK;

//▼プロトタイプ宣言

//初期化
void InitTalk();

//更新
bool UpdateTalk();

//表示
void DrawTalk();

//解放
void ReleaseTalk();

//テキスト読み込み
int LoadTalk( char *pass , char *mes , int offset );

//ファイルから読み込んで文章を生成
void CreateTalk(TALK* ptalk );

//文章の情報を取得
TALK* GetTalk();

#endif