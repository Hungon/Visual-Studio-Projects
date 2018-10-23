#pragma once
#ifndef		CSELECTMODE_H__
#define		CSELECTMODE_H__

#include	"CScene.h"
#include	"CCharacter.h"
#include	<windows.h>

//front define
class CSelectButton;


//kind of selection
enum tagSELECTMODEKIND
{
	SELECTMODE_SELL,
	SELECTMODE_PUZZLE,
	SELECTMODE_DESIGN,
	SELECTMODE_KIND_OF_SELECTION,
};

//kind of select images
enum tagSELECTMODEIMAGEKIND {
	SELECTMODE_IMAGE_STAGE,				//image to select button
	SELECTMODE_IMAGE_FRAM,				//fram of the image
	SELECTMODE_KIND_OF_IMAGE,			//kind
};

//kind of house's image
enum tagSELECTMODEHOUSEKIND {
	SELECTMODE_HOUSE_KNIT,					//knitting house
	SELECTMODE_HOUSE_PUZZLE,				//house of play to puzzle
	SELECTMODE_HOUSE_DESIGNER,				//house of design puzzle
	SELECTMODE_KIND_OF_HOUSE,				//kind
	SELECTMODE_HOUSE_NOTHING,
};

//define the play class
class		CSelectMode : public CScene
{

private:

	//for image
	int		*m_lpImage;
	CSelectButton	*m_lpButton;
	CSelectButton	*m_lpSelectImage;
public:

	//define static members
	static HWND m_hWnd;
	//using kind of image
	static const int	SELECTMODE_KIND_OF_IMAGE;
	//default setting
	//each buttons' setting
	static const TPOINT<int>	SELECTMODE_IMAGE_BUTTON_SELL_SIZE;
	static const TPOINT<int>	SELECTMODE_IMAGE_BUTTON_PUZZLE_SIZE;
	static const TPOINT<int>	SELECTMODE_IMAGE_BUTTON_DESIGN_SIZE;
	static const TPOINT<float>	SELECTMODE_IMAGE_BUTTON_SELL_COORDINATE;
	static const TPOINT<float>	SELECTMODE_IMAGE_BUTTON_COORDINATE_SPACE;

	//select image
	static const TPOINT<int>	SELECTMODE_IMAGE_SELECT_IMAGE_SIZE;
	static const TPOINT<float>	SELECTMODE_IMAGE_SELECT_IMAGE_COORDINATE;
	static const float			SELECTMODE_IMAGE_SELECT_IMAGE_DEFAULT_ALPHA;
	static const float			SELECTMODE_IMAGE_SELECT_IMAGE_ADDTION_ALPHA;
	//fram of select image
	static const TPOINT<int>	SELECTMODE_IMAGE_SELECT_IMAGE_FRAM_SIZE;
	static const TPOINT<float>	SELECTMODE_IMAGE_SELECT_IMAGE_FRAM_COORDINATE;


	CSelectMode() :
		m_lpImage(NULL)
	{}
	~CSelectMode();

	//override functions in CScene
	SCENE_NUM	Init();
	SCENE_NUM	Update();
	SCENE_NUM	Release();
	void		Draw();

};

#endif