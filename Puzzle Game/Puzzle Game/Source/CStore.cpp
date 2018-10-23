#include	"CStore.h"

//define static members
HWND	CStore::m_hWnd = NULL;
//using kind of image
const int	CStore::STORE_KIND_OF_IMAGE = 1;
//default setting


CStore::~CStore() {
	delete[]this->m_lpImage;		//for image
	this->m_lpImage = NULL;
}

//override functions in CScene
SCENE_NUM	CStore::Init() {

	//using image
	char	*lpImage[CStore::STORE_KIND_OF_IMAGE] = {
		"Image\\Store\\Background.bmp",
	};
	//allot memory
	this->m_lpImage = new int[CStore::STORE_KIND_OF_IMAGE];

	//load using elemental image
	for (int i = 0; i < CStore::STORE_KIND_OF_IMAGE; i++) { this->m_lpImage[i] = CreateDDImage(lpImage[i], CStore::m_hWnd); }
	return SCENE_UPDATE;
}

SCENE_NUM	CStore::Update() {
	return SCENE_UPDATE;
}

void		CStore::Draw() {
	//draw background image
	DDBlt(0, 0, 640, 480, this->m_lpImage[0], 0, 0);
}

SCENE_NUM	CStore::Release() {
	for (int i = 0; i < CStore::STORE_KIND_OF_IMAGE; i++) { ReleaseDDImage(this->m_lpImage[i]); }
	return SCENE_END;
}
