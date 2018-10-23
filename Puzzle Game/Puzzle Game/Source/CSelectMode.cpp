#include	"CSelectMode.h"
#include	"CSelectButton.h"
#include	"CWipeManager.h"

//redefine static members
HWND CSelectMode::m_hWnd = NULL;
const int	CSelectMode::SELECTMODE_KIND_OF_IMAGE = 4;
//default setting
//each buttons' size
const TPOINT<int>	CSelectMode::SELECTMODE_IMAGE_BUTTON_SELL_SIZE = { 96, 48 };				//sell
const TPOINT<int>	CSelectMode::SELECTMODE_IMAGE_BUTTON_PUZZLE_SIZE = { 128, 48 };				//puzzle
const TPOINT<int>	CSelectMode::SELECTMODE_IMAGE_BUTTON_DESIGN_SIZE = { 128, 48 };				//design
//each coordinate setting
const TPOINT<float>	CSelectMode::SELECTMODE_IMAGE_BUTTON_SELL_COORDINATE = { 0, 290.0f };		//indication
const TPOINT<float>	CSelectMode::SELECTMODE_IMAGE_BUTTON_COORDINATE_SPACE = { 0.0f, 10.0f };	//space

//select image
const TPOINT<int>	CSelectMode::SELECTMODE_IMAGE_SELECT_IMAGE_SIZE = { 256, 192 };
const TPOINT<float>	CSelectMode::SELECTMODE_IMAGE_SELECT_IMAGE_COORDINATE = { 0.0f , 50.0f };
const float			CSelectMode::SELECTMODE_IMAGE_SELECT_IMAGE_DEFAULT_ALPHA = { 255.0f };
const float			CSelectMode::SELECTMODE_IMAGE_SELECT_IMAGE_ADDTION_ALPHA = { 5.0f };

//fram of select image
const TPOINT<int>	CSelectMode::SELECTMODE_IMAGE_SELECT_IMAGE_FRAM_SIZE = { 422, 281 };
const TPOINT<float>	CSelectMode::SELECTMODE_IMAGE_SELECT_IMAGE_FRAM_COORDINATE = { 0.0f, 10.0f };


CSelectMode::~CSelectMode() {
	delete this->m_lpImage;
	this->m_lpImage = NULL;
	delete[]this->m_lpButton;
	this->m_lpButton = NULL;
}


SCENE_NUM	CSelectMode::Init() {

	//kind of image
	char	*lpImage[CSelectMode::SELECTMODE_KIND_OF_IMAGE] = {
		"Image\\SelectMode\\SelectMode.bmp",					//each select mode of image
		"Image\\SelectMode\\SelectBack.bmp",					//back ground image
		"Image\\SelectMode\\ImageFram.bmp",						//fram of image
		"Image\\SelectMode\\SelectHouse.bmp",					//image to select button
	};
	//allot memory
	this->m_lpImage = new int[CSelectMode::SELECTMODE_KIND_OF_IMAGE];			//image
	this->m_lpButton = new CSelectButton[SELECTMODE_KIND_OF_SELECTION];			//button
	this->m_lpSelectImage = new CSelectButton[SELECTMODE_KIND_OF_IMAGE];		//for select image

	//create material of image
	for (int i = 0; i < CSelectMode::SELECTMODE_KIND_OF_IMAGE; i++) { this->m_lpImage[i] = CreateDDImage(lpImage[i], CSelectMode::m_hWnd); }

	//each buttons' initializer
	TPOINT<float> buttonPos[SELECTMODE_KIND_OF_SELECTION] = {								//each position
		{CHARACTER_CENTER_POSITION_X(CSelectMode::SELECTMODE_IMAGE_BUTTON_SELL_SIZE.x),
		CSelectMode::SELECTMODE_IMAGE_BUTTON_SELL_COORDINATE.y} ,															//button for sell
		{ CHARACTER_CENTER_POSITION_X(CSelectMode::SELECTMODE_IMAGE_BUTTON_PUZZLE_SIZE.x),
		CSelectMode::SELECTMODE_IMAGE_BUTTON_SELL_COORDINATE.y +
		(CSelectMode::SELECTMODE_IMAGE_BUTTON_SELL_SIZE.y + CSelectMode::SELECTMODE_IMAGE_BUTTON_COORDINATE_SPACE.y) } ,	//button for puzzle
		{ CHARACTER_CENTER_POSITION_X(CSelectMode::SELECTMODE_IMAGE_BUTTON_DESIGN_SIZE.x),
		CSelectMode::SELECTMODE_IMAGE_BUTTON_SELL_COORDINATE.y +
		(CSelectMode::SELECTMODE_IMAGE_BUTTON_SELL_SIZE.y + CSelectMode::SELECTMODE_IMAGE_BUTTON_COORDINATE_SPACE.y) +
		(CSelectMode::SELECTMODE_IMAGE_BUTTON_PUZZLE_SIZE.y + CSelectMode::SELECTMODE_IMAGE_BUTTON_COORDINATE_SPACE.y) } ,	//button for design
	};
	TPOINT<int> buttonSize[SELECTMODE_KIND_OF_SELECTION] = {
		CSelectMode::SELECTMODE_IMAGE_BUTTON_SELL_SIZE,						//sell
		CSelectMode::SELECTMODE_IMAGE_BUTTON_PUZZLE_SIZE,					//puzzle
		CSelectMode::SELECTMODE_IMAGE_BUTTON_DESIGN_SIZE,					//design
	};
	for (int i = 0; i < SELECTMODE_KIND_OF_SELECTION; i++) {
		this->m_lpButton[i].InitChara(buttonPos[i], buttonSize[i]);			//each setting for drawing
		this->m_lpButton[i].m_Ipic.y = (this->m_lpButton[i].m_Isize.y * i);	//origin position
		this->m_lpButton[i].SetCharaImg(this->m_lpImage[0]);				//set each material of image
		this->m_lpButton[i].m_existFlag = true;
	}

	//set each images to select button
	TPOINT<float> selectPos[SELECTMODE_KIND_OF_IMAGE] = {					//each coordinate
		{ CHARACTER_CENTER_POSITION_X(CSelectMode::SELECTMODE_IMAGE_SELECT_IMAGE_FRAM_SIZE.x),
		CSelectMode::SELECTMODE_IMAGE_SELECT_IMAGE_FRAM_COORDINATE.y } ,			//fram
		{ CHARACTER_CENTER_POSITION_X(CSelectMode::SELECTMODE_IMAGE_SELECT_IMAGE_SIZE.x),
		CSelectMode::SELECTMODE_IMAGE_SELECT_IMAGE_COORDINATE.y } ,					//image
	};
	TPOINT<int> selectSize[SELECTMODE_KIND_OF_IMAGE] = {					//each size
		CSelectMode::SELECTMODE_IMAGE_SELECT_IMAGE_FRAM_SIZE,				//fram
		CSelectMode::SELECTMODE_IMAGE_SELECT_IMAGE_SIZE,					//image
	};
	int	selectImage[SELECTMODE_KIND_OF_IMAGE] = { this->m_lpImage[2], this->m_lpImage[3] };			//each material of image
	for (int i = 0; i < SELECTMODE_KIND_OF_IMAGE; i++) {
		this->m_lpSelectImage[i].InitChara(selectPos[i], selectSize[i]);	//each initializer
		this->m_lpSelectImage[i].SetCharaImg(selectImage[i]);
		this->m_lpSelectImage[i].m_existFlag = true;
	}
	//set alpha to image of house
	this->m_lpSelectImage[1].m_alpha = CSelectMode::SELECTMODE_IMAGE_SELECT_IMAGE_DEFAULT_ALPHA;

	return SCENE_UPDATE;
}

SCENE_NUM	CSelectMode::Update() {

	//kind of function
	tagSELECTMODEKIND buttonKind[SELECTMODE_KIND_OF_SELECTION] = {
		SELECTMODE_SELL,
		SELECTMODE_PUZZLE,
		SELECTMODE_DESIGN,
	};
	//kind of transition scene
	GAMESCENE_NUMBER nextScene[SELECTMODE_KIND_OF_SELECTION] = {
		GAMESCENE_STORE,				//to store for sell my knits
		GAMESCENE_PLAY_PUZZLE,			//to play puzzle
		GAMESCENE_DESIGN,				//to design knit
	};

	//select mode to play, these are knit or puzzle or design
	for (int i = 0; i < SELECTMODE_KIND_OF_SELECTION; i++) {
		this->m_lpButton[i].UpdateChara();
		if (this->m_lpButton[i].CheckMouseAction()) {				//when cursor overlap with each buttons and press key on keybord, return is true
			CWipeManager::CreateWipeManager(nextScene[i], WIPEMANAGER_FEEDOUT);
			return SCENE_RELEASE;
		}
	}

	//change image of selection as mouse's cursor is overlaped with each buttons
	tagSELECTMODEHOUSEKIND houseKind = SELECTMODE_HOUSE_NOTHING;
	for (int i = 0; i < SELECTMODE_KIND_OF_SELECTION; i++) {
		if (this->m_lpButton[i].GetMouseisOverlaped()) {
			houseKind = (tagSELECTMODEHOUSEKIND)i;				//subsutitute kind of house to penetrate process
			break;
		}
	}
	if (houseKind != SELECTMODE_HOUSE_NOTHING) {			//penetartion drawing, add defafult value of alpha into global variable
		this->m_lpSelectImage[1].m_Ipic.y = (CSelectMode::SELECTMODE_IMAGE_SELECT_IMAGE_SIZE.y * houseKind);			//change kind
		this->m_lpSelectImage[1].m_alpha -= CSelectMode::SELECTMODE_IMAGE_SELECT_IMAGE_ADDTION_ALPHA;
		if (this->m_lpSelectImage[1].m_alpha <= 1.0f) this->m_lpSelectImage[1].m_alpha = 1.0f;
	}
	else {//subtract default value of alpha from global variable
		this->m_lpSelectImage[1].m_alpha += CSelectMode::SELECTMODE_IMAGE_SELECT_IMAGE_ADDTION_ALPHA;
		if (this->m_lpSelectImage[1].m_alpha >= 255.0f) this->m_lpSelectImage[1].m_alpha = 255.0f;
	}

	return SCENE_UPDATE;
}

void		CSelectMode::Draw() {

	//draw back ground image
	DDBlt(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, this->m_lpImage[1], 0, 0);

	for (int i = 0; i < SELECTMODE_KIND_OF_SELECTION; i++) {		//draw each buttons
		this->m_lpButton[i].DrawChara(DRAWING_TYPE_DDBLT, DDSCALE_STARTING_POINT_NOTHING);
	}

	//set each how to draw
	tagDRAWINGKIND	drawingKind[SELECTMODE_KIND_OF_IMAGE] = { DRAWING_TYPE_DDBLT, DRAWING_TYPE_DDALPHABLT };
	for (int i = 0; i < SELECTMODE_KIND_OF_IMAGE; i++) {				//draw each image
		this->m_lpSelectImage[i].DrawChara(drawingKind[i], DDSCALE_STARTING_POINT_NOTHING);
	}
}

SCENE_NUM	CSelectMode::Release() {
	for (int i = 0; i < CSelectMode::SELECTMODE_KIND_OF_IMAGE; i++) { ReleaseDDImage(this->m_lpImage[i]); }		//each image
	for (int i = 0; i < SELECTMODE_KIND_OF_SELECTION; i++) {				//each button
		this->m_lpButton[i].ReleaseChara();
	}
	for (int i = 0; i < SELECTMODE_KIND_OF_IMAGE; i++) {					//each images
		this->m_lpSelectImage[i].ReleaseChara();
	}

	return SCENE_END;
}
