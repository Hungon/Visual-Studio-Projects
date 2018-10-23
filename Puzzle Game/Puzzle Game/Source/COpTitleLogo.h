#ifndef		COPTITLELOGO_H__
#define		COPTITLELOGO_H__

#include	"CCharacter.h"

//define the class
class		COpTitleLogo : public CCharacter {

private:
	//count time for draw bezier line
	double m_millisecond;

public:

	//Бе define static members
	//title logo
	static const TPOINT<int>	OPTITLELOGO_IMAGE_TITLE_LOGO_SIZE;
	static const TPOINT<float>	OPTITLELOGO_IMAGE_TITLE_LOGO_COORDINATE;

	COpTitleLogo() :
		m_millisecond(0) {}
	~COpTitleLogo();

	void	InitChara();
	void	UpdateChara();
	void	ReleaseChara();
	void	DrawChara();

};

#endif