#pragma once
#ifndef		CSTORE_H__
#define		CSTORE_H__

#include	"CScene.h"
#include	"CCharacter.h"
#include	<windows.h>

//front define
class CSelectButton;

//define the play class
class		CStore : public CScene
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
	static const int	STORE_KIND_OF_IMAGE;
	//default setting

	CStore() :
		m_lpImage(NULL)
	{}
	~CStore();

	//override functions in CScene
	SCENE_NUM	Init();
	SCENE_NUM	Update();
	SCENE_NUM	Release();
	void		Draw();

};

#endif