#include	"COpTitleLogo.h"
#include	"graphics.h"

//Бе redefine static members
//title logo
const TPOINT<int>	COpTitleLogo::OPTITLELOGO_IMAGE_TITLE_LOGO_SIZE = { 480,96 };
const TPOINT<float>	COpTitleLogo::OPTITLELOGO_IMAGE_TITLE_LOGO_COORDINATE = { 0.0f,50.0f };

/*
destractor
*/
COpTitleLogo::~COpTitleLogo() {}

/*
Initialize
*/
void	COpTitleLogo::InitChara() {

	//Initialize indication
	this->m_StartFpos.y = float(0 - COpTitleLogo::OPTITLELOGO_IMAGE_TITLE_LOGO_SIZE.y);							//starting positionY
	this->m_StartFpos.x = 0.0f;																				//starting positionX
	this->m_Isize = COpTitleLogo::OPTITLELOGO_IMAGE_TITLE_LOGO_SIZE;
	this->m_existFlag = true;
}

/*
Update
*/
void	COpTitleLogo::UpdateChara() {

	if (this->m_millisecond < 1.0f) {
		//indicate bezier for draw title logo
		TPOINT<int> titlePos[3] = {
			{ CHARACTER_CENTER_POSITION_X(COpTitleLogo::OPTITLELOGO_IMAGE_TITLE_LOGO_SIZE.x),0 },
			{ CHARACTER_CENTER_POSITION_X(COpTitleLogo::OPTITLELOGO_IMAGE_TITLE_LOGO_SIZE.x),120 },
			{ CHARACTER_CENTER_POSITION_X(COpTitleLogo::OPTITLELOGO_IMAGE_TITLE_LOGO_SIZE.x),(int)COpTitleLogo::OPTITLELOGO_IMAGE_TITLE_LOGO_COORDINATE.y },
		};
		this->m_millisecond += 0.02;
		this->SetBezier(titlePos, 3, this->m_millisecond);
	}
	else {
		this->m_Fpos.y = COpTitleLogo::OPTITLELOGO_IMAGE_TITLE_LOGO_COORDINATE.y;
	}
}

/*
Draw
*/
void	COpTitleLogo::DrawChara() {

	//draw title logo
	if (this->m_existFlag) {
		DDBlt(
			(int)this->m_Fpos.x,
			(int)this->m_Fpos.y,
			this->m_Isize.x,
			this->m_Isize.y,
			this->m_ImgNum,
			this->m_Ipic.x,
			this->m_Ipic.y);
	}
}

/*
Release
*/
void	COpTitleLogo::ReleaseChara() {}