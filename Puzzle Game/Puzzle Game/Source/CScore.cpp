#include	"CScore.h"
#include	"graphics.h"
#include	"input.h"

//redefine the static number
//for score's number
const TPOINT<int> CScore::SCORE_SCORE_NUMBER_IMAGE_SIZE = { 40,50 };
const float CScore::SCORE_DEFAULT_SCORE_NUMBER_IMAGE_SCALE = 1.0f;
//present indicate number
int CScore::m_indicateNum = 0;

CScore::~CScore(){}

void CScore::InitChara() {
	//intialize indication
	this->m_Isize = CScore::SCORE_SCORE_NUMBER_IMAGE_SIZE;				//size
	this->m_scale = CScore::SCORE_DEFAULT_SCORE_NUMBER_IMAGE_SCALE;		//scale rate
}

void CScore::UpdateChara(){}

void CScore::DrawChara() {}

void CScore::ReleaseChara(){}


/*
Draw number's image
*/
void CScore::DrawNumber(
		TPOINT<float> pos, int number,				//indicate coordinate, draw number
		int digit, tagSCORECOLORTYPE color,			//digit, kind of color
		float variableScale) {						//score's scale rate

	//表示する数値を受け取る
	int num = number;
	//表示する数値
	int rest = 0;
	//表示する数値の表示元
	int numD = 0;
	//桁カウント用
	int digitCnt = 0;

	//渡された値が負の値なら正の値にする
	if (num < 0) num *= -1;

	//渡された引数の数値の各桁の数値を調べる
	do {
		//余りの数値が求める桁の数値
		rest = num % 10;
		//渡された値を毎回10で割っていく
		num /= 10;
		//表示する数の表示元を扱う
		numD = this->m_Isize.x * rest;

		//数値画像の表示
		DDScaleBltSelectIndicate(
			pos.x - (this->m_Isize.x * this->m_scale) * digitCnt,
			pos.y,
			this->m_Isize.x,
			this->m_Isize.y,
			this->m_ImgNum,
			this->m_Ipic.x + numD,
			this->m_Ipic.y + (this->m_Isize.y * color),
			variableScale,
			DDSCALE_STARTING_POINT_LEFT_TOP
			);
		//計算する毎にカウント
		digitCnt++;
		//渡された値が0未満になったら抜ける
	} while (num > 0 || digitCnt < digit);
}


/*
direction of drawing number that increase or decrease
*/
void		CScore::GraduallyNumber(
			int	indicateNum,				//present drwing number
			int	terminateNum,				//tereminate to drawing number
			int intervalFram				//interval time
			){								//direction of number drawing that increase or decrease

	//▼数値の上限値・下限値の設定
	//下限は0
	if (terminateNum < 0) terminateNum = 0;

	//上限は1億未満
	if (terminateNum > 100000000) terminateNum = 99999999;

	//▼表示する値と表示したい値が違ったら処理を行う
	//もしくは回転の処理がまだ続いていたら実行する
	if (terminateNum != indicateNum) {
		//rest of number
		int rest;
		//表示する値が表示したい値より小さかったら増加する
		if (indicateNum <= terminateNum) {
			//表示する値と表示したい値の差分を求める
			rest = terminateNum - indicateNum;
			//フレーム毎に加算する
			indicateNum += rest / intervalFram;
			//表示したい値が表示する値から差分を引いた値付近になったら
			//表示したい値を代入する
			//もしくは、「Zキーか①ボタン」を押したら値を代入
			if (rest <= intervalFram || JustKey('Z') || JustButton(BUTTON_1)) {
				//最終的に表示する値の代入
				indicateNum = terminateNum;
			}
		}
		//表示する値が表示したい値より大きかったら減少する
		if (indicateNum >= terminateNum) {
			//表示する値と表示したい値の差分を求める
			rest = indicateNum - terminateNum;
			//フレーム毎に減算する
			indicateNum -= rest / intervalFram;
			//表示したい値が表示する値から差分を引いた値付近になったら
			//表示したいを値を代入する
			if (rest <= intervalFram || JustKey('Z') || JustButton(BUTTON_1)) {
				//最終的に表示する値の代入
				indicateNum = terminateNum;
			}
		}
	}
	//subsutitute calculation value for present indication number of variable
	this->SetNumber(indicateNum);
}

/*
direction rolling for draw number
*/
void	CScore::GraduallyRolling(
				int terminateNumber,		//end the indication number
				int rollingTimeMax,			//roll time
				int digit) {				//indication digit

	//フレーム経過
	this->m_rollingCount++;

	//表示する上限の数値を扱う
	int unsigned terminateNum = 1;
	//上限の桁数を計算
	for (int i = 0; i < digit; i++) { terminateNum *= 10; }
	//毎フレーム加算する値を求める
	int add = ((terminateNum - 1) / 9);

	//指定したフレームに経過するまでスコアの表示を回転状態にする
	if (this->m_rollingCount < rollingTimeMax) {
		//表示する値を加算する
		this->SetNumber(this->m_indicateNum + add);
		//桁の位まで表示する
		if (abs(this->m_indicateNum) >= terminateNum) this->m_indicateNum = 0;
	}
	//経過フレームが指定フレームに達したら、値を代入して表示する
	//もしくは、決定キーが押されたら代入
	if (this->m_rollingCount >= 20) {				//rolling process at least 20 fram
		if (this->m_rollingCount >= rollingTimeMax ||
			JustKey('Z') || JustButton(BUTTON_1)) {
			//表示する値に代入する
			this->m_indicateNum = terminateNumber;
		}
	}
}