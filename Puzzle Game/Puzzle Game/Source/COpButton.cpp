#include	"COpButton.h"
#include	"graphics.h"
#include	"window.h"

//Бе redefine static members
//title's button
const TPOINT<int>	COpButton::OPBUTTON_IMAGE_BUTTON_SIZE = { 128,48 };
const TPOINT<float>	COpButton::OPBUTTON_IMAGE_BUTTON_STARTING_COORDINATE = { 0.0f,250.0f };

/*
destractor
*/
COpButton::~COpButton() {}

/*
Initialize
*/
void	COpButton::InitChara(tagOPENINGBUTTON buttonNumber) {

	//kind of game's scene
	GAMESCENE_NUMBER	sceneTbl[OPENINGBUTTON_KIND] = {
		GAMESCENE_SELECT_MODE,					//from start of button
	};
	//Initialize indication
	this->m_Fpos.y = COpButton::OPBUTTON_IMAGE_BUTTON_STARTING_COORDINATE.y;				//indication positionY
	this->m_Fpos.x = (float)CHARACTER_CENTER_POSITION_X(COpButton::OPBUTTON_IMAGE_BUTTON_SIZE.x);	//indication positionX
	this->m_Isize = COpButton::OPBUTTON_IMAGE_BUTTON_SIZE;
	this->m_existFlag = true;
	this->m_nextGameScene = sceneTbl[buttonNumber];					//next game scene
}

/*
Update
*/
void	COpButton::UpdateChara() {

}

/*
Draw
*/
void	COpButton::DrawChara() {

	//draw button
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
void	COpButton::ReleaseChara() {}

/*
check mouse's coordinate and transition scene to next scene
*/
GAMESCENE_NUMBER COpButton::TransitionScene() {

	//get mouse's coordinate
	int	mouseX = GetMouseX();
	int mouseY = GetMouseY();

	//diverge process from mouse's position
	if (mouseX > this->m_Fpos.x &&
		mouseY > this->m_Fpos.y &&
		mouseX < (this->m_Fpos.x + this->m_Isize.x) &&
		mouseY < (this->m_Fpos.y + this->m_Isize.y)) {
		this->m_Ipic.x = this->m_Isize.x;						//change drawing image
		if (GetDownL()) return this->m_nextGameScene;			//return next scene
	}
	else {
		this->m_Ipic.x = 0;										//drawing image when don't overlap mouse's coordinate and button's rectangular
	}
	return GAMESCENE_OPENING;
}