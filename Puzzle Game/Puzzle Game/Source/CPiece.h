#ifndef		CPIECE_H__
#define		CPIECE_H__

#include	<windows.h>
#include	"CCharacter.h"
#include	"graphics.h"

//flag of correct position
enum tagPIECEFLAG {
	PIECE_FLAG_NOTHING,					//nothing
	PIECE_FLAG_FITTING_POSITION,		//fitting with own correct position
	PIECE_KIND_OF_FLAG,					//kind of flag
};


class	CPiece : public CCharacter{


private:

protected:

public:

	//public members
	tagPIECEFLAG	m_PieceFlag;

	//static members
	static const float	PIECE_IMAGE_DEFAULT_ALPHA;
	static const float	PIECE_IMAGE_CORRECT_POSITION_ALPHA;

	CPiece() :
		m_PieceFlag(PIECE_FLAG_NOTHING)
	{}
	~CPiece();

	//overload pure functions
	void	InitChara(TPOINT<float> arrayPos, TPOINT<int> arrayMax, TPOINT<int> size, int kind);
	void	UpdateChara();				
	void	DrawChara(tagDRAWINGKIND drawKind, tagDDSCALESTARTINGPOINT DDScaleStartPoint);
	void	ReleaseChara();				

};


#endif