#ifndef		CSTAGE_H__
#define		CSTAGE_H__

#include	<windows.h>
#include	"CCharacter.h"
#include	"graphics.h"

class		CStage : public CCharacter{

private:
	

public:

	CStage();
	~CStage();

	void	InitChara(TPOINT<float> coordinate, TPOINT<int> size);
	void	UpdateChara();
	void	ReleaseChara();
	void	DrawChara(tagDRAWINGKIND drawKind, tagDDSCALESTARTINGPOINT DDScaleStartPoint);

};

#endif