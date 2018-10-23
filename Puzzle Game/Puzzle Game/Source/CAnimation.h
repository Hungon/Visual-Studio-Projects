#pragma once
#ifndef		CANIMATION_H__
#define		CANIMATION_H__

#include	"CCharacter.h"

class CAnimation{

private:

protected:

public:

	//each members
	TPOINT<int>		m_startPic;
	TPOINT<int>		m_size;
	int		m_countMax;
	int		m_fram;
	int		m_time;
	int		m_count;
	int		m_type;
	bool	m_reverseFlag;
	int		m_direction;


	CAnimation();
	~CAnimation();

	void	SetAnimation(
		TPOINT<int>		startPic,
		TPOINT<int>		size,
		int		countMax,
		int		fram,
		int		type
		);
	bool	UpdateReverseAnimation(TPOINT<int> *lpPic, int direction, bool reverseFlag);

};


#endif
