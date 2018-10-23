#include	"CSelectButton.h"
#include	"window.h"

void	CSelectButton::InitChara(TPOINT<float> coordinate, TPOINT<int> size) {
	this->m_Fpos = coordinate;
	this->m_Isize = size;
}

void	CSelectButton::DrawChara(tagDRAWINGKIND drawKind, tagDDSCALESTARTINGPOINT DDScaleStartPoint) {

	if (this->m_existFlag) {
		switch (drawKind)
		{

		case DRAWING_TYPE_DDBLT:
			DDBlt(
				this->m_Fpos.x,
				this->m_Fpos.y,
				this->m_Isize.x,
				this->m_Isize.y,
				this->m_ImgNum,
				this->m_Ipic.x,
				this->m_Ipic.y
				);
			break;

		case DRAWING_TYPE_DDSCALEBLT:
			DDScaleBltSelectIndicate(
				this->m_Fpos.x,
				this->m_Fpos.y,
				this->m_Isize.x,
				this->m_Isize.y,
				this->m_ImgNum,
				this->m_Ipic.x,
				this->m_Ipic.y,
				this->m_scale,
				DDScaleStartPoint);
			break;

		case DRAWING_TYPE_DDALPHABLT:
			DDAlphaBlt(
				(int)this->m_Fpos.x,
				(int)this->m_Fpos.y,
				this->m_Isize.x,
				this->m_Isize.y,
				this->m_ImgNum,
				this->m_Ipic.x,
				this->m_Ipic.y,
				this->m_alpha);
			break;
		default:
			break;
		}
	}
}

/*
check mouse's coordinate and transition scene to next scene
*/
bool	CSelectButton::CheckMouseAction() {

	//get mouse's coordinate
	int	mouseX = GetMouseX();
	int mouseY = GetMouseY();

	//diverge process from mouse's position
	if (this->m_existFlag) {
		if (mouseX > this->m_Fpos.x &&
			mouseY > this->m_Fpos.y &&
			mouseX < (this->m_Fpos.x + this->m_Isize.x) &&
			mouseY < (this->m_Fpos.y + this->m_Isize.y)) {
			this->m_Ipic.x = this->m_Isize.x;						//change drawing image
			if (GetDownL()) return true;							//if press mouse's left click, return is true
		}
		else {
			this->m_Ipic.x = 0;										//drawing image when don't overlap mouse's coordinate and button's rectangular
		}
	}
	return false;
}

/*
check mouse's cursor
mouse's cursor overlaped, return is true
*/
bool	CSelectButton::GetMouseisOverlaped() {

	//get mouse's coordinate
	int	mouseX = GetMouseX();
	int mouseY = GetMouseY();

	//diverge process from mouse's position
	if (this->m_existFlag) {
		if (mouseX > this->m_Fpos.x &&
			mouseY > this->m_Fpos.y &&
			mouseX < (this->m_Fpos.x + this->m_Isize.x) &&
			mouseY < (this->m_Fpos.y + this->m_Isize.y)) {
			return true;	//mouse's cursor is overlaped
		}
	}
	return false;
}