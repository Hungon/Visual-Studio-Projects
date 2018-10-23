#include	"CDesign.h"

//define static members
HWND	CDesign::m_hWnd = NULL;
//using kind of image
const int	CDesign::DESIGN_KIND_OF_IMAGE = 1;
//default setting


CDesign::~CDesign() {
	delete[]this->m_lpImage;		//for image
	this->m_lpImage = NULL;
}

//override functions in CScene
SCENE_NUM	CDesign::Init() {

	//using image
	char	*lpImage[CDesign::DESIGN_KIND_OF_IMAGE] = {
		"Image\\Design\\Background.bmp",
	};
	//allot memory
	this->m_lpImage = new int[CDesign::DESIGN_KIND_OF_IMAGE];

	//load using elemental image
	for (int i = 0; i < CDesign::DESIGN_KIND_OF_IMAGE; i++) { this->m_lpImage[i] = CreateDDImage(lpImage[i], CDesign::m_hWnd); }

	return SCENE_UPDATE;
}

SCENE_NUM	CDesign::Update() {
	return SCENE_UPDATE;
}

void		CDesign::Draw() {
	//draw background image
	DDBlt(0, 0, 640, 480, this->m_lpImage[0], 0, 0);
}

SCENE_NUM	CDesign::Release() {
	for (int i = 0; i < CDesign::DESIGN_KIND_OF_IMAGE; i++) { ReleaseDDImage(this->m_lpImage[i]); }
	return SCENE_END;
}
