#include	"Likelihood.h"


//************************************************************************************************
//渡された２つの値から、範囲内でランダムの値を返す
//************************************************************************************************
int RandomNum( int max , int min ){

	int r;
	//min〜maxの範囲で乱数を取得
	r = rand() % ( max - min + 1 ) + min;
	//渡された値の範囲内のランダムの値を返す
	return r;
}
