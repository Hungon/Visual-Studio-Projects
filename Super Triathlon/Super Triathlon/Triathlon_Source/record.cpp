//
#pragma warning ( disable : 4996 )

//▼　ヘッダーのインクルード
#include    "record.h"
#include    "seaenemy.h"
#include    "swimer.h"
#include    "item.h"
#include    "seaenemy.h"
#include    "swimer.h"
#include    "result.h"
#include    "play.h"
#include	<stdio.h>
#include	<string.h>


//▼グローバル変数の定義
//ファイルの項目を扱う
RECORD g_record[2] = {0};

//現在の扱うファイルの列挙型
tagRENEWAL g_thisTime = RECORD_BEST;

//記録で扱う各項目の要素数のデータテーブル
//①海ステージ：敵討伐、獲得アイテム、時間、敵討伐CHAIN数、集計ポイントの順番
//②道路ステージ：破壊した障害物、飛んだハードル、時間、ハードル跳躍CHAIN数、集計ポイントの順番
//③オフロードステージ：成功したアクション、合計アクションPt、時間、連続アクション数、集計ポイントの順番
char g_recordScoreItemTbl[STAGE_MAX][RECORD_MAX] = {

	{ 5 , 5 , 3 , 1 , 1 } ,			//海

	{ 2 , 3 , 3 , 1 , 2 } ,			//道路

	{ 4 , 1 , 3 , 1 , 1 } ,			//オフロード

};

//competitors' record
RECORD		g_RecordForCompetitors[STAGE_MAX][PARTICIPANT_PLAYER] = { 0 };

//**************************************************************************************************
//記録の初期化
//**************************************************************************************************
void InitRecord(){

	//各項目の初期化
	//今回の記録
	ZeroMemory( &g_record[0] , sizeof(RECORD) * 2 );

}


//**************************************************************************************************
//ファイルから値を読み込む
//**************************************************************************************************
RECORD*	 LoadRecordFile( char* pass , tagRENEWAL thisTime ){

	//ファイルポインタ
	FILE* fp = NULL;

	//ファイルからの文字列を受け取る
	char word[256] = {0};

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


	//引数のファイルを開く
	fp = fopen( pass , "rb" );

	//ファイルが開けたか確認
	if( fp == NULL ){
		//エラー内容を表示
		MessageBox(NULL ,"ファイルの読み込みに失敗しました" , "LoadFile" , MB_OK);
		//呼び出し元に返る
		return 0;
	}

	//ファイルの末端まで読み込む
	while( feof(fp) == 0 ){
	
		//文字列を読み込む
		fread( word + wordCnt , sizeof(char) , 1 , fp );

		//受け取った文字列をint型で受け取る
		seek = (int)word[wordCnt];

		//改行があれば次の項目へ
		if( word[wordCnt] == 10 && 
			word[wordCnt - 1] == 13 || 
			word[wordCnt] == '/'){

			//文字数のリセット
			wordCnt = 0;
			//文字列の初期化
			ZeroMemory( word , sizeof(char) * 256 );
			//項目数をリセット
			itemCnt = 0;
			//桁数をリセット
			digit = 0;
			//次の項目へ
			itemNext++;
			continue;
		}

		//スペースがあれば値を取得する
		if( isspace( seek ) != 0 || ispunct( seek ) ){
			//コピーした文字列を10進数に変換する
			g_record[thisTime].recordBox[itemNext][itemCnt] = atoi( word + wordCnt - digit );
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
	fclose( fp );
	//取得した値を返す
	return	&g_record[thisTime];
}


//*******************************************************************************************
//記録を保存する「種目毎に分けて各項目を書き込む」
//*******************************************************************************************
void SaveRecordForStage( char* pass ,  RECORD* pre , tagRENEWAL thisTime ){
	
	//ファイルポインタ
	FILE* fp = NULL;

	//現在のステージ番号を取得
	tagSELECTSTAGE getStage = GetStage();

	//▼　今回の記録を最高の記録へ書き込む
	//ファイルを開く
	fp = fopen( pass , "wb" );

	//ファイルが開けたか確認
	if( fp == NULL ){
		//エラー内容を表示
		MessageBox(NULL , "ファイルの読み込みに失敗しました" , "SaveRecordForStage" , MB_OK);
		//呼び出し元に返る
		return;
	}

	//▼　今回記録した各項目を保存する
	for( int j = 0 ; j < RECORD_MAX ; j++ ){
		//敵討伐数　⇒　アイテム獲得数　⇒　クリア時間　⇒　CHAIN数　⇒　集計ポイント
		//の順番で書き込んでいく
		//各項目数だけループを回す
		for( int i = 0 ; i < g_recordScoreItemTbl[getStage][j] ; i++ ){
			
			//今回記録を書き込むファイルが今回記録したデータなら値を比較用に取得する
			//各項目の値を代入する
			if( thisTime == RECORD_PREVIEW ){
				
				g_record[RECORD_PREVIEW].recordBox[j][i] = pre->recordBox[j][i];
			}

			//各項目の数に達したら抜ける
			if( i == g_recordScoreItemTbl[getStage][j] - 1 ){

				//各項目数を書き込んだら改行して次の要素へ
				fprintf( fp , "%d /" , pre->recordBox[j][i] );

				//次の要素へ
				break;
			}

			//記録を書き込む
			fprintf( fp , "%d " , pre->recordBox[j][i] );
		}
	}
	//ファイルを閉じる
	fclose(fp);

}

//*************************************************************************************************
//今回の記録と過去の記録を比較して記録を更新する
//*************************************************************************************************
void RenewalRecord( char *compairFile ){


	//現在のステージ番号を取得
	tagSELECTSTAGE getStage = GetStage();

	//更新フラグ
	bool renewal = false;

	//▼クリア時間比較用変数
	//今回の記録
	int previewTime = MINUTES_INTO_MILLISECOND( g_record[RECORD_PREVIEW].recordBox[RECORD_TIME][0] ) +//分
					  SECONDS_INTO_MILLISECOND( g_record[RECORD_PREVIEW].recordBox[RECORD_TIME][1] ) +//秒
					  g_record[RECORD_PREVIEW].recordBox[RECORD_TIME][2];							 //ミリ秒

	//▼ ファイルの読み込み
	//最高の記録を読み込む
	LoadRecordFile( compairFile , RECORD_BEST );

	//最高の記録
	int bestTime = MINUTES_INTO_MILLISECOND( g_record[RECORD_BEST].recordBox[RECORD_TIME][0] ) +		//分
				   SECONDS_INTO_MILLISECOND( g_record[RECORD_BEST].recordBox[RECORD_TIME][1] ) +		//秒
				   g_record[RECORD_BEST].recordBox[RECORD_TIME][2];								//ミリ秒


	//▼ 今回の記録と最高の記録を比較する
	for( int j = 0 ; j < RECORD_MAX ; j++ ){
		for( int i = 0 ; i < g_recordScoreItemTbl[getStage][j] ; i++ ){
			//if this record is road-stage,
			if (getStage == STAGE_ROAD && j == RECORD_TOTAL && i == 1) {
				if (g_record[RECORD_PREVIEW].recordBox[RECORD_TOTAL][1] < g_record[RECORD_BEST].recordBox[RECORD_TOTAL][1] ||
					g_record[RECORD_BEST].recordBox[RECORD_TOTAL][1] == 0) {
					g_record[RECORD_BEST].recordBox[RECORD_TOTAL][1] = g_record[RECORD_PREVIEW].recordBox[RECORD_TOTAL][1];
					g_record[RECORD_PREVIEW].renewalFlag[RECORD_TOTAL][1] = RECORD_RENEWAL_FLAG;
					renewal = true;
				}
				break;
			}
			//クリア時間の比較
			if( j == RECORD_TIME ){			
				//比較用に変換した変数同士で比べる
				if( previewTime < bestTime || bestTime == 0 ){				
					//値を代入
					//分
					g_record[RECORD_BEST].recordBox[RECORD_TIME][0] = 
						g_record[RECORD_PREVIEW].recordBox[RECORD_TIME][0];
					//秒
					g_record[RECORD_BEST].recordBox[RECORD_TIME][1] = 
						g_record[RECORD_PREVIEW].recordBox[RECORD_TIME][1];
					//ミリ秒
					g_record[RECORD_BEST].recordBox[RECORD_TIME][2] = 
						g_record[RECORD_PREVIEW].recordBox[RECORD_TIME][2];		
					//各項目毎の更新フラグを立たせる
					g_record[RECORD_PREVIEW].renewalFlag[RECORD_TIME][0] = RECORD_RENEWAL_FLAG;
					//更新フラグを立たせる
					renewal = true;
				}
				//代入し終わったら次の項目へ
				break;
			}
			//値の比較
			//今回の記録が最高記録を上回っていたら、今回の記録を
			//最高の記録に上書きする
			if( g_record[RECORD_PREVIEW].recordBox[j][i] >
				g_record[RECORD_BEST].recordBox[j][i] ){
				//値を代入
				g_record[RECORD_BEST].recordBox[j][i] = g_record[RECORD_PREVIEW].recordBox[j][i];
				//各項目毎の更新フラグを立たせる
				g_record[RECORD_PREVIEW].renewalFlag[j][i] = RECORD_RENEWAL_FLAG;
				//更新フラグを立たせる
				renewal = true;
			}
		}
	}

	//▼ 最高の記録を更新する
	//更新フラグがtrueなら更新処理
	if( renewal ){
		//記録の保存
		SaveRecordForStage( compairFile , &g_record[RECORD_BEST] , RECORD_BEST );
	}

}


/*
************************************************************************************************************
ファイルへ記録用
************************************************************************************************************
*/
void	SaveRecord(char* lpPass, int kind, int* lpItem, RECORD* lpRecord) {

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
				fprintf(lpFile, "%d /", lpRecord->recordBox[i][j]);
				break;
			}
			//各項目を半角スペースを空けて書き込む
			fprintf(lpFile, "%d ", lpRecord->recordBox[i][j]);
		}
	}
	//ファイルを閉じる
	fclose(lpFile);

}

/*
*************************************************************************************************
今回の記録と最高記録とを比較し最高記録を上書きする
*************************************************************************************************
*/
void	RenewalRecordEndTheResult(char *previewpass, char *bestpass, int kind, int *lpItem) {

	//記録した値と更新フラグをリセット
	InitRecord();

	//引数のファイルパスのファイルから値を取得する
	RECORD* previewRecord = LoadRecordFile(previewpass, RECORD_PREVIEW);
	RECORD* bestRecord = LoadRecordFile(bestpass, RECORD_BEST);

	//更新フラグ
	bool renewal = false;

	//▼クリア時間比較用変数
	//今回の記録
	int previewTime = MINUTES_INTO_MILLISECOND(previewRecord->recordBox[0][0]) +//分
		SECONDS_INTO_MILLISECOND(previewRecord->recordBox[0][1]) +				//秒
		previewRecord->recordBox[0][2];											//ミリ秒

																				//最高の記録
	int bestTime = MINUTES_INTO_MILLISECOND(bestRecord->recordBox[0][0]) +		//分
		SECONDS_INTO_MILLISECOND(bestRecord->recordBox[0][1]) +					//秒
		bestRecord->recordBox[0][2];											//ミリ秒

								

	//引数の項目の種類だけ書き込む処理
	for (int i = 0; i < kind; i++) {
		for (int j = 0; j < lpItem[i]; j++) {

			//クリア時間の比較
			if (i == 0) {
				//比較用に変換した変数同士で比べる
				if (previewTime < bestTime || bestTime == 0) {

					//値を代入
					//分
					bestRecord->recordBox[i][0] =
						previewRecord->recordBox[i][0];
					//秒
					bestRecord->recordBox[i][1] =
						previewRecord->recordBox[i][1];
					//ミリ秒
					bestRecord->recordBox[i][2] =
						previewRecord->recordBox[i][2];

					//各項目毎の更新フラグを立たせる
					previewRecord->renewalFlag[0][0] = RECORD_RENEWAL_FLAG;
					//更新フラグを立たせる
					renewal = true;
				}
				//代入し終わったら次の項目へ
				break;
			}

			//値の比較
			//今回の記録が最高記録を上回っていたら、今回の記録を
			//最高の記録に上書きする
			if (previewRecord->recordBox[i][j] >
				bestRecord->recordBox[i][j]) {
				//値を代入
				bestRecord->recordBox[i][j] = previewRecord->recordBox[i][j];
				//各項目毎の更新フラグを立たせる
				previewRecord->renewalFlag[i][j] = RECORD_RENEWAL_FLAG;
				//更新フラグを立たせる
				renewal = true;

			}
		}
	}

	//▼ 最高の記録を更新する
	//更新フラグがtrueなら更新処理
	if (renewal) SaveRecord(bestpass, kind, lpItem, bestRecord);

}


//***********************************************************************************************
//記録を返す関数
//***********************************************************************************************
RECORD* GetRecord( tagRENEWAL record ){	return &g_record[record]; }


/*
************************************************************************************************************
記録をリセットする
************************************************************************************************************
*/
void	ResetRecord(char* lpPass, int kind, int* lpItem,  tagRENEWAL thisTime) {

	FILE	*lpFile = NULL;

	//項目カウント用
	int cnt = 0;

	//▼　今回の記録を最高の記録へ書き込む
	//ファイルを開く
	lpFile = fopen(lpPass, "wb");

	//ファイルが開けたか確認
	if (lpFile == NULL) {
		//エラー内容を表示
		MessageBox(NULL, "ファイルの読み込みに失敗しました", "ResetRecord", MB_OK);
		//呼び出し元に返る
		return;
	}

	//引数の項目の種類だけ書き込む処理
	for (int i = 0; i < kind; i++) {
		for (int j = 0; j < lpItem[i]; j++) {

			//各項目を満たしたら次の記録へ
			if (j == (lpItem[i] - 1)) {
				//各項目を書き込んだら改行して次の要素へ
				fprintf(lpFile, "%d /", 0);
				//項目をカウント
				cnt++;
				break;
			}
			//各項目を半角スペースを空けて書き込む
			fprintf(lpFile, "%d ", 0);
			//項目をカウント
			cnt++;
		}
	}
	//ファイルを閉じる
	fclose(lpFile);
}

/*
Compair time record
return is count of lpCompair1's ranking
*/
int		CompairRaceTimeRecord(TIME* lpCompair1, TIME* lpCompair2) {

	//for compairing variable
	int compairPoint1 = MINUTES_INTO_MILLISECOND(lpCompair1->minutes) +
		SECONDS_INTO_MILLISECOND(lpCompair1->second) +
		lpCompair1->millisecond;
	//competitor
	int compairPoint2 = MINUTES_INTO_MILLISECOND(lpCompair2->minutes) +
		SECONDS_INTO_MILLISECOND(lpCompair2->second) +
		lpCompair2->millisecond;
	//count ranking
	int count = 0;
	//convert minute and second into millisecond
	if (compairPoint1 > compairPoint2)  count = 1;
	return count;
}

/*
Compair record more than time record
*/
int		CompairRecord(int compair1, int compair2) {
	//count ranking
	int count = 0;
	//compair each value
	if (compair1 < compair2) count = 1;
	return count;
}

/*
Save competitors' records in this race only
competitors' temporary records
*/
void	SaveCompetitorRecordTemporary(RECORD *lpRecordPoint, tagRECORDMATRIX kind, tagSELECTSTAGE stage) {

	//loop kind of competitor, that is nine
	for (int j = 0; j < g_recordScoreItemTbl[stage][kind]; j++) {
		for (int i = 0; i < PARTICIPANT_PLAYER; i++) {
			//aggregate point into global variable
			g_RecordForCompetitors[stage][i].recordBox[kind][j] = lpRecordPoint[i].recordBox[kind][j];
		}
	}
}

/*
get competitors' records
*/
RECORD*	GetRecordCompetitors(tagSELECTSTAGE stage) { return g_RecordForCompetitors[stage]; }

//************************************************************************************************
//半角の数値を全角に変換する
//************************************************************************************************
char* ConvertNum( int num ){

	//引数を文字列に変換用
	char dummy[256] = {0};

	//引数を１０進数の文字列に変換
	itoa(num , dummy , 10);

	//受け取った数字の文字列の長さを取得
	int len = strlen(dummy);

	//全角文字戻り値用
	char *reply = new char[len];

	//メモリ移動用
	int p = 0;

	//確保したサイズだけループして全角に変換
	for( int i = 0 ; i < len ; i++ ){
		switch(dummy[i]){
			case '0':
				strcpy(&reply[p] , "０");
				p += 2;
				break;
			case '1':
				strcpy(&reply[p] , "１");
				p += 2;
				break;
			case '2':
				strcpy(&reply[p] , "２");
				p += 2;
				break;
			case '3':
				strcpy(&reply[p] , "３");
				p += 2;
				break;
			case '4':
				strcpy(&reply[p] , "４");
				p += 2;
				break;
			case '5':
				strcpy(&reply[p] , "５");
				p += 2;
				break;
			case '6':
				strcpy(&reply[p] , "６");
				p += 2;
				break;
			case '7':
				strcpy(&reply[p] , "７");
				p += 2;
				break;
			case '8':
				strcpy(&reply[p] , "８");
				p += 2;
				break;
			case '9':
				strcpy(&reply[p] , "９");
				p += 2;
				break;
		}
	}

	//渡された数値を全角で返す
	return reply;
}


//**********************************************************************************************
//確保した文字列解放用の関数
//**********************************************************************************************
void DeleteChar( char** reply ){

	//解放の処理
	delete[] *(reply);
	*(reply) = NULL;
}