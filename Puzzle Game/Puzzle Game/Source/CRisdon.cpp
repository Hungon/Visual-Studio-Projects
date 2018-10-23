#include	"CRisdon.h"
#include	"graphics.h"


CRisdon::CRisdon() {}

CRisdon::~CRisdon() {

}


void	CRisdon::InitChara(TPOINT<float> coordinate, TPOINT<int> size, float scale){

	//subsututite each values
	this->m_Fpos = coordinate;
	this->m_Isize = size;
	this->m_scale = scale;
}

void	CRisdon::UpdateChara(){}

void	CRisdon::DrawChara(){

	if (this->m_existFlag) {
		DDScaleBltSelectIndicate(
			this->m_Fpos.x,
			this->m_Fpos.y,
			this->m_Isize.x,
			this->m_Isize.y,
			this->m_ImgNum,
			this->m_Ipic.x,
			this->m_Ipic.y,
			this->m_scale,
			DDSCALE_STARTING_POINT_LEFT_TOP);
	}
}

void	CRisdon::ReleaseChara(){}