#include "CAnimation.h"


CAnimation::CAnimation() {
	this->m_startPic = { 0 };
	this->m_size = { 0 };
	this->m_count = 0;
	this->m_countMax = 0;
	this->m_fram = 0;
	this->m_time = 0;
	this->m_type = 0;
	this->m_reverseFlag = false;
	this->m_direction = 0;
}

CAnimation::~CAnimation() {}


/*
set character's each animation setting
*/
void	CAnimation::SetAnimation(
	TPOINT<int>		startPic,
	TPOINT<int>		size,
	int		countMax,
	int		fram,
	int		type
	) {

	this->m_startPic = startPic;
	this->m_size = size;
	this->m_countMax = countMax;
	this->m_fram = fram;
	this->m_type = type;
}

bool	CAnimation::UpdateReverseAnimation(TPOINT<int> *lpPic, int direction, bool reverseFlag) {

	this->m_time++;			//count time
	bool ret = true;

	if (this->m_reverseFlag == false) {	//addition count for normal animation
		if (this->m_time > this->m_fram) {
			this->m_count++;			//next animation
			this->m_time = 0;			//reset count time
		}
		if (this->m_count >= this->m_countMax) {//when present count more than count max, subtract from count for reverse
			if (reverseFlag == false) {			//normal animation
				this->m_count = 0;
				ret = false;					//end animation
			}
			else {
				this->m_reverseFlag = true;		//flag of reverse animation
			}
		}
	}

	if (this->m_reverseFlag) {			//subtract count for reverse animation
		if (this->m_time > this->m_fram) {
			this->m_count--;			//next animation
			this->m_time = 0;			//reset count time
		}
		if (this->m_count <= 0) {
			this->m_reverseFlag = false;
			ret = false;	//when present count more than count 0, reverse animation finishs
		}
	}

	lpPic->x = this->m_startPic.x + (this->m_count * this->m_size.x);			//change character's animation
	lpPic->y = this->m_startPic.y + (this->m_direction * this->m_size.y);

	return ret;					//if during animation, return value is true
}

