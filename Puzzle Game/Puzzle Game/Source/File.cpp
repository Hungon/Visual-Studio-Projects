#include    "File.h"
#include	<stdio.h>
#include	<string.h>


//**************************************************************************************************
//ファイルから値を読み込む
//**************************************************************************************************
FILEVALUE*	 LoadFile(char* lpPass) {

	//ファイルポインタ
	FILE* fp = NULL;
	//ファイルからの文字列を受け取る
	char word[256] = { 0 };
	//区切り文字とスペースを調べる用
	int seek = 0;
	//文字数カウント用
	int wordCnt = 0;
	//項目カウント用
	int itemCnt = 0;
	//次の項目の要素数
	int itemNext = 0;
	//桁数用
	int digit = 0;
	FILEVALUE	fileValue;			//for response

	//引数のファイルを開く
	fp = fopen(lpPass, "rb");
	//ファイルが開けたか確認
	if (fp == NULL) {
		//エラー内容を表示
		MessageBox(NULL, "ファイルの読み込みに失敗しました", "LoadFile", MB_OK);
		//呼び出し元に返る
		return 0;
	}
	//ファイルの末端まで読み込む
	while (feof(fp) == 0) {
		//文字列を読み込む
		fread(word + wordCnt, sizeof(char), 1, fp);
		//受け取った文字列をint型で受け取る
		seek = (int)word[wordCnt];
		//改行があれば次の項目へ
		if (word[wordCnt] == 10 &&
			word[wordCnt - 1] == 13 ||
			word[wordCnt] == '/') {
			//文字数のリセット
			wordCnt = 0;
			//文字列の初期化
			ZeroMemory(word, sizeof(char) * 256);
			//項目数をリセット
			itemCnt = 0;
			//桁数をリセット
			digit = 0;
			//次の項目へ
			itemNext++;
			continue;
		}
		//スペースがあれば値を取得する
		if (isspace(seek) != 0 || ispunct(seek)) {
			//コピーした文字列を10進数に変換する
			fileValue.fileBox[itemNext][itemCnt] = atoi(word + wordCnt - digit);
			//桁数をリセット
			digit = 0;
			//次の項目へ
			itemCnt++;
		}
		//桁数をカウント
		digit++;
		//文字数をカウントアップ
		wordCnt++;
	}
	//ファイルを閉じる
	fclose(fp);
	//取得した値を返す
	return	&fileValue;
}


/*
************************************************************************************************************
ファイルへ記録用
************************************************************************************************************
*/
void	SaveRecord(char* lpPass, int kind, int* lpItem, FILEVALUE* lpRecord) {

	FILE	*lpFile = NULL;

	//▼　今回の記録を最高の記録へ書き込む
	//ファイルを開く
	lpFile = fopen(lpPass, "wb");
	//ファイルが開けたか確認
	if (lpFile == NULL) {
		//エラー内容を表示
		MessageBox(NULL, "ファイルの読み込みに失敗しました", "SaveRecordEndTheResult", MB_OK);
		//呼び出し元に返る
		return;
	}
	//引数の項目の種類だけ書き込む処理
	for (int i = 0; i < kind; i++) {
		for (int j = 0; j < lpItem[i]; j++) {
			//各項目を満たしたら次の記録へ
			if (j == (lpItem[i] - 1)) {
				//各項目を書き込んだら改行して次の要素へ
				fprintf(lpFile, "%d /", lpRecord->fileBox[i][j]);
				break;
			}
			//各項目を半角スペースを空けて書き込む
			fprintf(lpFile, "%d ", lpRecord->fileBox[i][j]);
		}
	}
	//ファイルを閉じる
	fclose(lpFile);
}