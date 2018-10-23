#include	"CRisdonEffect.h"
#include	"graphics.h"

CRisdonEffect::CRisdonEffect() {}

CRisdonEffect::~CRisdonEffect() {}

//over ride functions in CCharacter class
void	CRisdonEffect::InitChara(TPOINT<int> size) {
	this->m_Isize = size;
}

void	CRisdonEffect::UpdateChara() {}

void	CRisdonEffect::DrawChara(tagDRAWINGKIND drawKind, tagDDSCALESTARTINGPOINT DDScaleStartPoint) {

	if (this->m_existFlag) {

		switch (drawKind)						//diverge kind of drawing for how to draw
		{
		case DRAWING_TYPE_DDBLT:				//kind of drawing is DDBLT
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

		case DRAWING_TYPE_DDSCALEBLT:				//kind of drawing is DDSCALEBLT
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

		case DRAWING_TYPE_DDALPHABLT:				//kind of drawing is DDALPHABLT
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

void	CRisdonEffect::ReleaseChara() {}

