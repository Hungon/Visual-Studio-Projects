//
#pragma warning ( disable : 4996 )

#include "talk.h"
#include "record.h"
#include "input.h"
#include <stdio.h>
#include <string.h>

//▼グローバル変数
//テキストファイル読み取り用
TALK g_talk[TALK_LOAD_MAX] = {0};
//読み込むファイルの要素番号
int g_file_cnt = 0;
//画像を扱う変数

//************************************************************************************************
//初期化
//************************************************************************************************
void InitTalk(){
	
	//文章管理構造体の初期化
	ZeroMemory(&g_talk[0] , sizeof(TALK) * TALK_LOAD_MAX);
	//ファイルの要素数
	g_file_cnt = 0;
}

//************************************************************************************************
//更新
//************************************************************************************************
bool UpdateTalk() {

	//文章未発生なら終了
	if (g_talk[g_file_cnt].fShow == false) {
		return g_talk[g_file_cnt].fShow;		//falseを返す
	}

	//表示したファイル数までループして消去
	for (int i = 0; i < (g_file_cnt + 1); i++) {

		//終了フラグが立っていたら全て表示終了
		if (g_talk[i].fDelete == true) {

			//表示フラグをfalse
			g_talk[i].fShow = false;
			//文字数をリセット
			g_talk[i].word = 0;
			//これまで表示した文章をリセット	
			ZeroMemory(&g_talk[i].mes[0], sizeof(char) * 256);
		}

	}

	//文字数増加が終わっていたら読み込み
	if (g_talk[g_file_cnt].fRead == false) {

		//文字数をリセット
		g_talk[g_file_cnt].word = 0;

		//これまで表示した文章をリセット	
		ZeroMemory(&g_talk[g_file_cnt].mes[0], sizeof(char) * 256);


		//次の文章を読み込み
		g_talk[g_file_cnt].readoffset = LoadTalk(&g_talk[g_file_cnt].pass[0],	  //読み込むファイル 
			&g_talk[g_file_cnt].mes[0],	  //文字列
			g_talk[g_file_cnt].readoffset);  //読み込んでいるバイト数

	}

	//読み込むファイル数だけ回す
	for (int i = 0; i < (g_file_cnt + 1); i++) {

		//読み込みフラグがtureなら文字数を増加
		if (g_talk[i].fRead) {

			//経過時間の計測	
			g_talk[i].cnt++;

			//経過時間が表示間隔の倍数か確認
			if (g_talk[i].cnt % g_talk[i].next == 0) {

				g_talk[i].word++;	//表示文字数加算
				g_talk[i].cnt = 0;	//経過時間リセット

				//文字数増加を制限
				if (g_talk[i].word >= g_talk[i].restrictive_word) {
					//全角サイズ分+ヌル文字の文字数だけ文字数を制限
					g_talk[i].word = g_talk[i].restrictive_word;
				}
			}
		}
	}
	//今回のフレームでの処理が終了
	return g_talk[g_file_cnt].fShow;
}

//************************************************************************************************
//表示
//************************************************************************************************
void DrawTalk() {

	POINT pos = { 0 };		//文字の座標
	char mes[3] = { 0 };	//1文字作成用

	//読み込んだファイル分だけループを回して表示する
	for (int j = 0; j < (g_file_cnt + 1); j++) {

		//文章送りが開始したら枠を表示
		if (g_talk[j].fShow) {

			//現在の文字数ぶんループさせる
			for (int i = 0; i < g_talk[j].word; i++) {

				//特殊文字「@r」があれば文字色を赤色にする
				if (g_talk[j].mes[i * 2] == '@' &&
					g_talk[j].mes[i * 2 + 1] == 'r') {

					//文字色を赤色にする
					g_talk[j].change_color = RGB(255, 0, 0);
					//次の文字へ
					continue;
				}

				//特殊文字「@g」があれば文字色を緑色にする
				if (g_talk[j].mes[i * 2] == '@' &&
					g_talk[j].mes[i * 2 + 1] == 'g') {

					//文字色を緑色にする
					g_talk[j].change_color = RGB(0, 255, 0);
					//次の文字へ
					continue;
				}

				//特殊文字「@b」があれば文字色を青色にする
				if (g_talk[j].mes[i * 2] == '@' &&
					g_talk[j].mes[i * 2 + 1] == 'b') {

					//文字色を青色にする
					g_talk[j].change_color = RGB(0, 0, 255);
					//次の文字へ
					continue;
				}

				//特殊文字「@y」があれば文字色を黄色にする
				if (g_talk[j].mes[i * 2] == '@' &&
					g_talk[j].mes[i * 2 + 1] == 'y') {

					//文字色を黄色にする
					g_talk[j].change_color = RGB(255, 255, 0);
					//次の文字へ
					continue;
				}

				//特殊文字「#t」があればタブ1個分空けて表示
				if (g_talk[j].mes[i * 2] == '#' &&
					g_talk[j].mes[i * 2 + 1] == 't') {

					//次に表示する文字のＸ座標を右へ1文字分ずらす
					pos.x += g_talk[j].size;
					//次の文字へ
					continue;
				}

				//特殊文字「#e」があれば文字数増加終了
				if (g_talk[j].mes[i * 2] == '#' &&
					g_talk[j].mes[i * 2 + 1] == 'e') {

					//読み込みフラグをfalse
					g_talk[j].fRead = false;
					//次の文字へ
					continue;
				}

				//特殊文字「＃n」があれば改行
				if (g_talk[j].mes[i * 2] == '#' &&
					g_talk[j].mes[i * 2 + 1] == 'n') {

					//X座標をリセット
					pos.x = 0;

					//改行処理Y座標を加算
					pos.y += g_talk[j].size + g_talk[g_file_cnt].line_space;

					//改行されたら文字色をリセット
					g_talk[j].change_color = g_talk[j].default_color;
					//次の文字へ
					continue;
				}

				//読み込んだデータの並びに「#d」がないかチェック
				if (g_talk[j].mes[i * 2] == '#' &&
					g_talk[j].mes[i * 2 + 1] == 'd') {

					//表示終了フラグをtrueに
					g_talk[j].fDelete = true;
					//次の文字へ
					continue;
				}

				//表示する1文字を作成し、表示
				//表示する1文字を作成
				mes[0] = g_talk[j].mes[i * 2];
				mes[1] = g_talk[j].mes[(i * 2) + 1];
				//ヌル文字分
				mes[2] = '\0';

				//文字体「明朝体フォント」へ変換する
				ChangeFont(FONT_MIN);

				//文字の表示
				DDTextOutEx(mes, (int)g_talk[j].baseX + pos.x,	//表示X座標 
					(int)g_talk[j].baseY + pos.y,			//表示Y座標
					g_talk[j].size,						//文字のサイズ
					g_talk[j].change_color,				//文字の色
					true);									//イタリック体フラグ

				//文字同士の間隔を開ける
				pos.x += g_talk[j].size;

			}
		}

		//次のファイル表示に座標をリセットする
		pos.x = 0;							//X座標
		pos.y = 0;							//Y座標
	}
}

//************************************************************************************************
//解放
//************************************************************************************************
void ReleaseTalk(){
	//最大読み込みファイル数だけ文章をリセット
	//０で初期化
	ZeroMemory(&g_talk[0], sizeof(TALK) * TALK_LOAD_MAX);
	//ファイルの要素数をリセット
	g_file_cnt = 0;
}


//************************************************************************************************
//読み込み
//************************************************************************************************
int LoadTalk(char *pass, char *mes, int offset) {

	//ファイル読み込み用
	FILE *fp = NULL;

	//ファイルをバイナリ形式で開く
	fp = fopen(pass, "rb");

	//エラーチェック
	if (fp == NULL) {
		MessageBox(NULL, "ファイルの読み込みに失敗", "TALK", MB_OK);
		return -1;
	}

	//ファイルポインタを移動
	fseek(fp, offset, SEEK_SET);

	//256個の要素に1バイトずつ読み込み
	for (int i = 0; i < 256; i++) {

		//文章の末端なら抜ける
		if (feof(fp) != 0) {

			//読み込んだファイルの末端からそのテキストの文字数を取得
			g_talk[g_file_cnt].restrictive_word = i;

			//次のファイルを読み込めるフラグをtrue
			g_talk[g_file_cnt].fNext = true;
			break;
		}

		//messの要素i番に読み込む
		fread(mes + i, sizeof(char), 1, fp);

		//読み込んだサイズを加算
		g_talk[g_file_cnt].readoffset++;

		//読み込んだデータの並びに「#e」がないかチェック
		if (mes[i] == 'e' &&
			mes[i - 1] == '#') {

			//ファイルを閉じて読み込み終了
			fclose(fp);
			//ファイルの何バイト目まで読み込んだかを返す
			return offset + g_talk[g_file_cnt].readoffset;
		}

		//読み込んだデータの並びに改行がないかチェック
		if (mes[i] == 10 &&
			mes[i - 1] == 13) {

			//改行の次の文字を読み込む
			mes[i] = 0;
			mes[i - 1] = 0;
			//改行分戻す
			i -= 2;
		}
	}

	//ファイルを閉じる
	fclose(fp);
	//読み込んだバイト数を返す
	return offset;
}


//************************************************************************************************
//ファイルを読み込んで文章を生成
//************************************************************************************************
void CreateTalk(TALK* ptalk) {

	//新しいファイルの読み込みフラグが立っていたら次に読み込むファイルの要素数を加算
	if (g_talk[g_file_cnt].fNext) {

		//ファイル数をカウントアップ
		g_file_cnt++;
		//最大読み込みファイル数まで加算する
		g_file_cnt %= TALK_LOAD_MAX + 1;
	}

	//文章生成済みなら終了
	if (g_talk[g_file_cnt].fShow) {
		return;
	}

	//▼　文章読み込みの設定

	//現在表示している文字数
	g_talk[g_file_cnt].word = 0;

	//1文字数を表示してからの経過時間
	g_talk[g_file_cnt].cnt = 0;

	//次の1文字数を表示する間隔
	g_talk[g_file_cnt].next = ptalk->next;

	//1文字のサイズ
	g_talk[g_file_cnt].size = ptalk->size;

	//行の間隔
	g_talk[g_file_cnt].line_space = ptalk->line_space;

	//変更したい文字色
	g_talk[g_file_cnt].change_color = RGB(255, 255, 255);
	//初期の文字色
	g_talk[g_file_cnt].default_color = RGB(255, 255, 255);

	//表示座標
	g_talk[g_file_cnt].baseX = ptalk->baseX;		//X座標
	g_talk[g_file_cnt].baseY = ptalk->baseY;		//Y座標

	//文字数を増やす
	g_talk[g_file_cnt].fRead = true;

	//読み込み位置リセット
	g_talk[g_file_cnt].readoffset = 0;

	//読み込みファイルパスをリセット
	strcpy(&g_talk[g_file_cnt].pass[0], ptalk->pass);

	//ファイルから次の文章を読み込み
	g_talk[g_file_cnt].readoffset = LoadTalk(&g_talk[g_file_cnt].pass[0],	  //ファイルパス
		&g_talk[g_file_cnt].mes[0],											  //文字列
		g_talk[g_file_cnt].readoffset);										  //読み込んだバイト数

	//文章生成終了
	g_talk[g_file_cnt].fShow = true;

}

//***********************************************************************************************
//文章情報を取得する
//***********************************************************************************************
TALK* GetTalk(){ return g_talk; }

