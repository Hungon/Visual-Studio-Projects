#pragma once
#ifndef		CRISDONEFFECT_H__
#define		CRISDONEFFECT_H__

#include	"CAnimation.h"
#include	"graphics.h"

//define the class
class CRisdonEffect : public CAnimation,
	public CCharacter
{

private:

	CAnimation	*m_lpCAnimation;		//for animation
	int			*m_lpImage;				//for image	
protected:

public:

	CRisdonEffect();
	~CRisdonEffect();
	//over ride functions in CCharacter class
	void	InitChara(TPOINT<int> size);
	void	UpdateChara();
	void	DrawChara(tagDRAWINGKIND drawKind, tagDDSCALESTARTINGPOINT DDScaleStartPoint);
	void	ReleaseChara();

};


#endif