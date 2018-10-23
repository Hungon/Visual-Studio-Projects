#pragma once
#ifndef		CDESIGN_H__
#define		CDESIGN_H__

#include	"CScene.h"
#include	"CCharacter.h"
#include	<windows.h>

//front define
class CSelectButton;

//define the play class
class		CDesign : public CScene
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
	static const int	DESIGN_KIND_OF_IMAGE;
	//default setting

	CDesign() :
		m_lpImage(NULL)
	{}
	~CDesign();

	//override functions in CScene
	SCENE_NUM	Init();
	SCENE_NUM	Update();
	SCENE_NUM	Release();
	void		Draw();

};

#endif