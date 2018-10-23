#ifndef __RECORD_H__
#define __RECORD_H__

//▼　マクロの定義
//分をポイントへ変換する
#define MINUTES_INTO_MILLISECOND( a )	( ( ( a ) * ( 60 ) ) * ( 100 ) )
//秒をポイントへ変換する
#define SECONDS_INTO_MILLISECOND( a )	( ( a ) * ( 100 ) )
//３つの項目の合計を求める
#define SUM( a , b , c )				( ( a ) + ( b ) + ( c ) )


//▼ 静的変数の定義
//記録更新フラグ
const char RECORD_NOT_RENEWAL = 0;			//更新なし
const char RECORD_RENEWAL_FLAG = 1;			//更新あり

//▼　列挙型
//記録の種類を扱う
enum tagRENEWAL{
	RECORD_PREVIEW			,	//今回の記録
	RECORD_BEST				,	//最高記録
};
//各ステージの記録を扱う要素
enum tagRECORDMATRIX{
	RECORD_ONE			,				//要素1
	RECORD_TWO			,				//要素2
	RECORD_TIME			,				//時間の要素
	RECORD_FOUR			,				//要素4
	RECORD_TOTAL		,				//集計ポイントの要素
	RECORD_MAX			,				//項目の数
};
//道路ステージの記録を扱う要素数
enum tagROADRECORDMATRIX{
	RECORD_DFEAT_OBSTACLES	,			//破壊した障害物の数
	RECORD_JUMPED_HURDLE	,			//飛び越えたハードル数
};
//kind of record of the half way
enum tagRECORDPARTICIPANTS {
	PARTICIPANT_CONPETITOR1,
	PARTICIPANT_CONPETITOR2,
	PARTICIPANT_CONPETITOR3,
	PARTICIPANT_CONPETITOR4,
	PARTICIPANT_CONPETITOR5,
	PARTICIPANT_CONPETITOR6,
	PARTICIPANT_CONPETITOR7,
	PARTICIPANT_CONPETITOR8,
	PARTICIPANT_CONPETITOR9,
	PARTICIPANT_PLAYER,
	RECORD_KIND_OF_PARTICIPANT,
};
//kind of item
enum tagRECORDTHEHALFWAYITEM {
	RECORD_HALF_WAY_RECORD_TOTAL_POINT,				//aggregate point
	RECORD_HALF_WAY_RECORD_RACE_TIME,				//time of race
	RECORD_HALF_WAY_RECORD_CHAIN,					//consecutive something
	RECORD_KIND_OF_ITEM_FOR_THE_HALF_WAY_RESULT,	//kind
};
//kind of save record for option
enum tagRECORDRANKINGRESULTITEM {
	RECORD_RANKING_ITEM_TOTAL_POINT,		//total
	RECORD_RANKING_ITEM_MINUTE,				//minute
	RECORD_RANKING_ITEM_SECOND,				//second
	RECORD_RANKING_ITEM_MILLISECOND,		//millisecond
	RECORD_RANKING_ITEM_CHAIN,				//chain
	RECORD_RANKING_ITEM_COLOR_FOR_TOTAL,	//each characters' identification
	RECORD_RANKING_ITEM_COLOR_FOR_TIME,		//each characters' identification
	RECORD_RANKING_ITEM_COLOR_FOR_CHAIN,	//each characters' identification
	RECORD_KIND_OF_RANKING_ITEM,			//kind
};
//▼　ヘッダーのインクルード
#include	<windows.h>
#include	"ending.h"
#include	"SelectStage.h"

//▼ 構造体の定義
//時間の構造体
typedef struct tagTIME {

	int hour;			//時
	int minutes;		//分
	int second;			//秒
	int millisecond;	//小数
	bool cntFlag;		//時間経過フラグ

} TIME, *LPTIME;

//ファイルの項目を扱う「RECORD構造体」
typedef struct tagRECORD {

	int recordBox[RECORD_KIND_OF_PARTICIPANT][RECORD_KIND_OF_RANKING_ITEM];		//記録を数値として扱う
	char renewalFlag[RECORD_MAX][RECORD_MAX];									//スコア色変更フラグ
	
}RECORD , *LPRECORD;

//▼　関数のプロトタイプ宣言

void InitRecord();

//②　記録保存・更新
void RenewalRecord( char *compairFile );

//⑥　渡された数値を全角に変換して文字列として返す
char* ConvertNum			//戻り値　＝　全角の数字
( int num );				//引数　＝　変換したい数値

//ファイルから値を読み込む
RECORD* LoadRecordFile( char* pass , tagRENEWAL thisTime );

//記録を返す関数
RECORD* GetRecord( tagRENEWAL record );

//記録を種目毎に分けて各項目を保存する
void SaveRecordForStage( char* pass , RECORD* pre , tagRENEWAL thisTime );

/*
************************************************************************************************************
記録用
************************************************************************************************************
*/
void	SaveRecord(char* lpPass, int kind, int* lpItem , RECORD* lpRecord);
/*
*************************************************************************************************
今回の記録と最高記録とを比較し最高記録を上書きする
*************************************************************************************************
*/
void	RenewalRecordEndTheResult(char *previewpass, char *bestpass, int kind, int *lpItem);
/*
************************************************************************************************************
記録をリセットする
************************************************************************************************************
*/
void	ResetRecord(char* lpPass, int kind, int* lpItem, tagRENEWAL thisTime);

/*
Compair tim record
return is lpCompair1's ranking
*/
int	CompairRaceTimeRecord(TIME* lpCompair1, TIME* lpCompair2);
/*
Compair record more than time record
*/
int		CompairRecord(int compair1, int compair2);
/*
Save competitors' records in this race only
competitors' temporary records
*/
void	SaveCompetitorRecordTemporary(RECORD *lpRecordPoint, tagRECORDMATRIX kind, tagSELECTSTAGE stage);
/*
get competitors' records
*/
RECORD*	GetRecordCompetitors(tagSELECTSTAGE stage);

#endif