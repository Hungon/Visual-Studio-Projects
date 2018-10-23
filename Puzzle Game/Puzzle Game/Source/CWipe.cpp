#include	"CWipe.h"


//each default of direction
bool	CWipe::m_ChangeScene = false;				//flag of transition scene
const int CWipe::WIPE_DEFAULT_ALPHA_ADD = 30.0f;
const int CWipe::WIPE_DEFAULT_ALPHA_SUB = 30.0f;
const int CWipe::WIPE_DEFAULT_SCALE_SPEED = 0.02f;
const int CWipe::WIPE_DEFAULT_CONTRACTION_SPEED = 0.02f;
const int CWipe::WIPE_DEFAULT_STAY_TIME = 100;


void	CWipe::InitChara(TPOINT<float> coordinate, TPOINT<int> size, tagWIPEMANAGER_DIRECT direction) {
	this->m_Fpos = coordinate;
	this->m_Isize = size;
	this->m_WipeDirect = direction;
	this->m_existFlag = true;
	this->m_alpha = 255.0f;
	this->m_stayTime = 0;
}

void	CWipe::UpdateChara() {}

bool	CWipe::UpdateWipe() {

	//diverge flag of wipe's direction in this time
	switch (this->m_WipeDirect) {
	case WIPEMANAGER_FEEDOUT:
		return this->DirectWipePenetration();
	case WIPEMANAGER_CONTRACTION:
		return this->DirectWipeContraction();
	}
	return false;
}

void	CWipe::DrawChara() {

	//diverge flag of direction to draw
	if (this->m_existFlag) {
		switch (this->m_WipeDirect) {
		case WIPEMANAGER_FEEDOUT:				//penetration drawing
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

		case WIPEMANAGER_CONTRACTION:				//contraction drawing
			DDScaleBltSelectIndicate(
				this->m_Fpos.x,
				this->m_Fpos.y,
				this->m_Isize.x,
				this->m_Isize.y,
				this->m_ImgNum,
				this->m_Ipic.x,
				this->m_Ipic.y,
				this->m_scale,
				DDSCALE_STARTING_POINT_CENTER);
			break;
		}
	}
}

void	CWipe::ReleaseChara() {}

//each wipes' direction
bool	CWipe::DirectWipePenetration() {			//direction of penetration

	if (this->m_alpha > 1.0f && this->m_stayTime <= 0) {
		this->m_alpha -= CWipe::WIPE_DEFAULT_ALPHA_SUB;
	}
	if (this->m_alpha <= 1.0f && this->m_stayTime < CWipe::WIPE_DEFAULT_STAY_TIME) {
		this->m_stayTime++;
		this->m_alpha = 1.0f;
		CWipe::m_ChangeScene = true;
	}
	if (this->m_stayTime >= CWipe::WIPE_DEFAULT_STAY_TIME) {
		if (this->m_alpha < 255.0f) {
			this->m_alpha += CWipe::WIPE_DEFAULT_ALPHA_ADD;
		}
		if (this->m_alpha > 255.0f) {
			this->m_alpha = 255.0f;
			this->m_existFlag = false;
			return false;					//direction is the end
		}
	}
	return true;							//during wipe is direciting
}

bool	CWipe::DirectWipeContraction() {			//direction of contraction
	return false;
}