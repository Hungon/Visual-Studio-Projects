#pragma once
#ifndef		CTALKMANAGER_H__
#define		CTALKMANAGER_H__

#include	"graphics.h"
#include	"CCharacter.h"

//front define
class CTalk;

//define the class, that manage talking
class CTalkManager
{

private:
	CTalk	*m_lpCTalk;			//for talk
	int		*m_lpImage;			//for image

protected:

public:

	//define static members
	//set default value
	static const int		TALKMANAGER_KIND_OF_IMAGE;				//kind of image

	static HWND	m_hWnd;

	//set bloon for talking
	static const TPOINT<int> TALKMANAGER_IMAGE_FRAM_DEAFULT_SIZE;

	//set talking
	static const int		TALKMANAGER_FONT_DEFAULT_SIZE;
	static const int		TALKMANAGER_FONT_DEFAULT_SPATIAL_TIME;
	static const int		TALKMANAGER_FONT_LINE_SPACE;
	static const COLORREF	TALKMANAGER_FONT_DEFAULT_COLOR;

	CTalkManager();
	~CTalkManager();
	void	InitializeTalkManager();
	void	UpdateTalkManager();
	void	DrawTalkManager();
	void	ReleaseTalkManager();

};


#endif#pragma once
