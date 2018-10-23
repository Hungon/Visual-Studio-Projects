#ifndef		COPENING_H__
#define		COPENING_H__

#include	"CScene.h"
#include	<windows.h>

//front define
class COpTitleLogo;
class COpButton;
class CTalkManager;

//define the class, inherited from class of each scene manager
class		COpening: public CScene{
private:
	
	int		*m_lpImage;

	//for title logo
	COpTitleLogo	*m_lpCTitleLogo;
	COpButton		*m_lpCButton;
	CTalkManager	*m_lpCTalkManager;

public:


	//define static members
	static HWND m_hWnd;							//window hundle
	static const char COPENING_IMAGE_MAX;		//using kind of image

	COpening();
	~COpening();

	// CSceneクラスに定義されている
	// 純粋仮想関数のオーバーライド
	SCENE_NUM	Init();
	SCENE_NUM	Update();
	SCENE_NUM	Release();
	void		Draw();

};

#endif