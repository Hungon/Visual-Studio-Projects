#ifndef		CSCORE_H__
#define		CSCORE_H__


//▼ヘッダーをインクルード
#include	"CCharacter.h"

//kind of score's color
enum tagSCORECOLORTYPE {
	SCORE_COLOR_WHITE,
	SCORE_COLOR_BLACK,
	SCORE_COLOR_RED,
	SCORE_COLOR_BLUE,
	SCORE_COLOR_GREEN,
	SCORE_COLOR_YELLOW,
	SCORE_COLOR_LIGHT_BLUE,
	SCORE_KIND_OF_COLOR,
};

//▼ オフロードスコアクラスの定義
class CScore : public CCharacter{

private:

	//coun time for rolling direction to display number
	int		m_rollingCount;

protected:

public:
	//define the static number
	//for score's number
	static const TPOINT<int> SCORE_SCORE_NUMBER_IMAGE_SIZE;
	static const float SCORE_DEFAULT_SCORE_NUMBER_IMAGE_SCALE;

	static int	m_indicateNum;		//present drwing number
	int	m_terminateNum;				//tereminate to drawing number
	int m_intervalFram;				//interval time
	int m_digit;					//digit of numbaer
	tagSCORECOLORTYPE m_scoreColor;	//score's color

	//コンストラクタ
	CScore() :
	m_rollingCount(0),				//direction rolling
	m_terminateNum(0),				//tereminate to drawing number
	m_intervalFram(0),				//interval time
	m_digit(0),						//digit of numbaer
	m_scoreColor(SCORE_COLOR_BLACK)	//score's color
	{}
	//ディストラクタ
	~CScore();
	
	
	//▼ define pure functions
	void	InitChara();				
	void	UpdateChara();					
	void	DrawChara();
	void	ReleaseChara();				
	/*
	Draw number's image
	*/
	void	DrawNumber(
				TPOINT<float> pos, int number,				//indicate coordinate, draw number
				int digit, tagSCORECOLORTYPE color,			//digit, kind of color
				float variableScale);						//score's scale rate
	void	GraduallyNumber(
				int	indicateNum,				//present drwing number
				int	terminateNum,				//tereminate to drawing number
				int intervalFram				//interval time
				);								//direction of number drawing that increase or decrease
	/*
	direction rolling for draw number
	*/
	void	GraduallyRolling(
				int terminateNumber,		//end the indication number
				int rollingTimeMax,			//roll time
				int digit);					//indication digit

	//set present indication number
	void		SetNumber(int indicateNumber) const { this->m_indicateNum = indicateNumber; }
	//get present indication number
	int			GetNumber()const { return this->m_indicateNum; }
};


#endif