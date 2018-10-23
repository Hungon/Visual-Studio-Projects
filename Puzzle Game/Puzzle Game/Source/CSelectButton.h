#pragma once
#ifndef		CSELECTBUTTON_H__
#define		CSELECTBUTTON_H__

#include	"CCharacter.h"
#include	"graphics.h"

//define the play class
class		CSelectButton : public CCharacter {

private:

public:
	CSelectButton(){}
	~CSelectButton(){}

	void	InitChara(TPOINT<float> coordinate, TPOINT<int> size);
	void	UpdateChara() {}
	void	ReleaseChara() {}
	void	DrawChara(tagDRAWINGKIND drawKind, tagDDSCALESTARTINGPOINT DDScaleStartPoint);
	/*
	check mouse's coordinate and transition scene to next scene
	*/
	bool	CheckMouseAction();

	/*
	check mouse's cursor
	mouse's cursor overlaped, return is true
	*/
	bool	GetMouseisOverlaped();
};

#endif