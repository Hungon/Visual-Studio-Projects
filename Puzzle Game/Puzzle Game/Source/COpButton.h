#pragma once
#ifndef		COPBUTTON_H__
#define		COPBUTTON_H__

#include	"CCharacter.h"
#include	"CSceneManager.h"

//kind of button
enum tagOPENINGBUTTON {
	OPENINGBUTTON_START,			//start
	OPENINGBUTTON_KIND,				//kind
};

//define the class
class		COpButton : public CCharacter {

private:
	GAMESCENE_NUMBER	m_nextGameScene;		//treat transition next scene by each button
public:

	//Бе define static members
	//start button
	static const TPOINT<int>	OPBUTTON_IMAGE_BUTTON_SIZE;
	static const TPOINT<float>	OPBUTTON_IMAGE_BUTTON_STARTING_COORDINATE;

	COpButton() :
		m_nextGameScene(GAMESCENE_NOTHING)
	{}
	~COpButton();

	void	InitChara(tagOPENINGBUTTON buttonNumber);
	void	UpdateChara();
	void	ReleaseChara();
	void	DrawChara();
	/*
	check mouse's coordinate and transition scene to next scene
	*/
	GAMESCENE_NUMBER	TransitionScene();
};

#endif