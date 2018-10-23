#pragma		once
#ifndef		CWIPEMANAGAER_H__
#define 	CWIPEMANAGAER_H__

//kind of direction
enum tagWIPEMANAGER_DIRECT{
	WIPEMANAGER_FEEDOUT	 ,
	WIPEMANAGER_CONTRACTION ,	
	WIPEMANAGER_KIND_OF_DIRECTION  ,
	WIPEMANAGER_DIRECTION_NOTHING,
};

#include	"CSceneManager.h"

//front define
class CWipe;

class CWipeManager {

private:

	static GAMESCENE_NUMBER	m_NextGameScene;
	static int		*m_lpImage;				//for material of image
	static CWipe	*m_lpCWipe;				//for create wipe
	static bool	m_DirectionFlag;			//during wipe is directing that is flag
protected:

public:

	//define static members
	static HWND	m_hWnd;
	static const int WIPEMANAGER_KIND_OF_IMAGE;
	static const int WIPEMANAGER_LOAD_KIND_OF_IMAGE;


	CWipeManager();
	~CWipeManager();

	void	InitWipeManager();
	bool	UpdateWipeManager();
	void	DrawWipeManager();
	void	ReleaseWipeManager();

	static void	CreateWipeManager(GAMESCENE_NUMBER next, tagWIPEMANAGER_DIRECT direct);
										
};

#endif