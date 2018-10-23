#include	<iostream>
#include	"CStage.h"
#include	"MemoryLeakCheck.h"
#include	"window.h"

using namespace std;


CStage::CStage(){}

CStage::~CStage(){}

void CStage::InitChara(TPOINT<float> coordinate, TPOINT<int> size) {
	this->m_Fpos = coordinate;
	this->m_Isize = size;
}


void CStage::UpdateChara() {}

void CStage::DrawChara(tagDRAWINGKIND drawKind, tagDDSCALESTARTINGPOINT DDScaleStartPoint){

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

void CStage::ReleaseChara() {}
