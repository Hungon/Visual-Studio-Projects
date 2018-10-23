#pragma once
#ifndef		CRISDONMANAGER_H__
#define		CRISDONMANAGER_H__

#include	"CCharacterManager.h"

//kind of effect
enum tagRISDONEFFECTKIND {
	RISDONMANAGER_EFFECT_TYPE_PERSPIRATION,			//effect of perspiration
	RISDONMANAGER_KIND_OF_EFFECT,						//kind
};


//front define
class CRisdon;				//risdon own
class CRisdonEffect;		//risdon's effect

//define the class
class CRisdonManager : public CCharacterManager{

private:
	int		*m_lpImage;					//for material of image
	CRisdon		*m_lpCRisdon;			//for risdon own
	CRisdonEffect	*m_lpRisdonEffect;	//for risdon's effect
protected:

public:
	//define the static member
	static  HWND	m_hWnd;
	static const int	RISDONMANAGER_KIND_OF_IMAGE;
	//risdon's each sets
	static const TPOINT<float>	RISDONMANAGER_IMAGE_RISDON_DEFAULT_COORDINATE_IN_PLAYING_PUZZLE;
	static const TPOINT<int>	RISDONMANAGER_IMAGE_RISDON_DEFAULT_SIZE;
	static const float			RISDONMANAGER_IMAGE_DEFAULT_SCALE;
	
	//set risdon's aniamtion
	static const TPOINT<int>	RISDONEFFECT_IMAGE_PERSPIRATION_SIZE;
	static int		RISDONEFFECT_IMAGE_PERSPIRATION_COUNT_MAX;
	static int		RISDONEFFECT_IMAGE_PERSPIRATION_FRAM;


	CRisdonManager();
	~CRisdonManager();

	//over ride functions in CCharacterManager class
	 void	InitManagerCharacter();
	 void	UpdateManagerCharacter();
	 void	DrawManagerCharacter();
	 void	ReleaseManagerCharacter();

};


#endif
