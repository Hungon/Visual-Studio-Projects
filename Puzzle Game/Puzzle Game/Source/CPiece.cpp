#include	"CPiece.h"
#include	"MemoryLeakCheck.h"
#include	<stdio.h>


//static members
const float	CPiece::PIECE_IMAGE_DEFAULT_ALPHA = 1.0f;
const float	CPiece::PIECE_IMAGE_CORRECT_POSITION_ALPHA = 150.0f;

CPiece::~CPiece() {}


void CPiece::InitChara(TPOINT<float> arrayPos, TPOINT<int> arrayMax, TPOINT<int> size, int kind){
	this->m_Isize = size;
	this->m_Fpos = arrayPos;
	this->m_Ipic.x = (kind % arrayMax.x) * size.x;
	this->m_Ipic.y = (kind / arrayMax.x) * size.y;
	this->m_kindNum = kind;
	this->m_alpha = CPiece::PIECE_IMAGE_DEFAULT_ALPHA;
}

void CPiece::UpdateChara() {
	//if piece's position is correct, change piece's penetration
	if (this->m_PieceFlag == PIECE_FLAG_FITTING_POSITION) {
		this->m_alpha = CPiece::PIECE_IMAGE_CORRECT_POSITION_ALPHA;
	}
	else {
		this->m_alpha = CPiece::PIECE_IMAGE_DEFAULT_ALPHA;
	}
}

void CPiece::DrawChara(tagDRAWINGKIND drawKind, tagDDSCALESTARTINGPOINT DDScaleStartPoint) {

	char mes[32] = { 0 };

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
/*		//Ží—Þ”Ô†‚ð•\Ž¦
		wsprintf(mes, "%d", this->m_kindNum);
		DDTextOutEx(mes, this->m_Fpos.x, this->m_Fpos.y, 20, RGB(0, 0, 0), false);
*/
	}
}


void CPiece::ReleaseChara() {}
