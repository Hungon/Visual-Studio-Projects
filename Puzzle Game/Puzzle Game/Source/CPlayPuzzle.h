#ifndef		CPLAY_PUZZLE_H__
#define		CPLAY_PUZZLE_H__

//kind of button
enum tagPLAYBUTTONKIND {
	PLAYBUTTON_NEXT,
	PLAYBUTTON_KIND_OF_BUTTON,
};

#include	"CScene.h"
#include	"CPieceManager.h"
#include	"CStageManager.h"
#include	"CScoreManager.h"
#include	"CRisdonManager.h"
#include	<windows.h>

//front define
class CSelectButton;

//define the play class
class		CPlayPuzzle: public CScene , public CPieceManager ,
	public CStageManager , public CScoreManager,
	public CRisdonManager
{

private:
	
	//for image
	int		*m_lpImage;
	CSelectButton	*m_lpCButton;	//for button
	bool	m_NextFlag;				//flag of next stage
	int		m_startintPlayCount;	//starting time for make blank time till the starting

public:

	//define static members
	static HWND m_hWnd;
	//using kind of image
	static const int	PLAY_KIND_OF_IMAGE;
	//each buttons' default setting
	static const TPOINT<int>	PLAY_IMAGE_BUTTON_NEXT_SIZE;
	static const TPOINT<float>	PLAY_IMAGE_BUTTON_NEXT_COORDINATE;


	CPlayPuzzle() :
		m_lpImage(NULL),
		m_NextFlag(false),
		m_startintPlayCount(0)
	{}
	~CPlayPuzzle();

	//override pure functions in CSecne
	SCENE_NUM	Init();
	SCENE_NUM	Update();
	SCENE_NUM	Release();
	void		Draw();

};

#endif