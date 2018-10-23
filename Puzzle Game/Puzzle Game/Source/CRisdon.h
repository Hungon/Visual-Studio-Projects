#pragma once
#ifndef		CRISDON_H__
#define		CRISDON_H__

#include	"CCharacter.h"

//define the class
class CRisdon : public CCharacter{

private:

protected:

public:

	CRisdon();
	~CRisdon();

	//over ride functions in CCharacter class
	void	InitChara(TPOINT<float> coordinate, TPOINT<int> size, float scale);
	void	UpdateChara();				
	void	DrawChara();				
	void	ReleaseChara();				

};


#endif