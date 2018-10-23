#pragma once
#ifndef		CWIPE_H__
#define		CWIPE_H__

#include	"CCharacter.h"
#include	"CWipeManager.h"

//define the play class
class		CWipe : public CCharacter {

private:

	int		m_stayTime;
	tagWIPEMANAGER_DIRECT m_WipeDirect;			//wipe's direction in this time
public:
	static bool	m_ChangeScene;					//flag of transition scene

	static const int WIPE_DEFAULT_ALPHA_ADD;
	static const int WIPE_DEFAULT_ALPHA_SUB;
	static const int WIPE_DEFAULT_SCALE_SPEED;
	static const int WIPE_DEFAULT_CONTRACTION_SPEED;
	static const int WIPE_DEFAULT_STAY_TIME;

	CWipe() :
		m_stayTime(0),
		m_WipeDirect(WIPEMANAGER_DIRECTION_NOTHING)
	{}
	~CWipe() {}

	void	InitChara(TPOINT<float> coordinate, TPOINT<int> size, tagWIPEMANAGER_DIRECT direction);
	void	UpdateChara();
	void	ReleaseChara();
	void	DrawChara();

	bool	UpdateWipe();

	//each wipes' direction
	bool	DirectWipePenetration();			//direction of penetration
	bool	DirectWipeContraction();			//direction of contraction

};

#endif