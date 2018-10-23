#ifndef		CTALK_H__
#define		CTALK_H__

#include	"graphics.h"
#include	"CCharacter.h"

class CTalk : public CCharacter
{
private:

	char	m_mes[256];			
	char	m_pass[256];			
	int		m_word;				
	int		m_limitWords;	
	int		m_size;				
	int		m_lineSpace;			
	int		m_cnt;				
	int		m_next;				
	int		m_readoffset;			
	float	m_baseX;			
	float	m_baseY;			
	bool	m_fRead;				
	bool	m_fShow;				
	bool	m_fDelete;			
	bool	m_fNext;				
	COLORREF	m_changeColor;
	COLORREF	m_defaultColor;
	int		m_fileCount;

public:

	CTalk() :
		m_word(0),
		m_limitWords(0),
		m_size(0),
		m_lineSpace(0),
		m_cnt(0),
		m_next(0),
		m_readoffset(0),
		m_baseX(0),
		m_baseY(0),
		m_fRead(false),
		m_fShow(false),
		m_fDelete(false),
		m_fNext(false),
		m_changeColor(RGB(0,0,0)),
		m_defaultColor(RGB(0,0,0)),
		m_fileCount(0)
	{}
	~CTalk();
	void	InitializeTalk(
		int fontSize, int drawingSpeed,
		int lineSpace, COLORREF	color);
	bool	UpdateTalk();
	void	DrawTalk();
	int		LoadTalk(char *pass, char *mes, int offset);
	void	CreateTalk(
		char *lpPass,
		float startX, float startY);

	//each override functions
	void	InitChara(TPOINT<float> baloonPos, TPOINT<int> baloonSize);
	void	UpdateChara();
	void	DrawChara(tagDRAWINGKIND drawKind, tagDDSCALESTARTINGPOINT DDScaleStartPoint);
	void	ReleaseChara() {}
};

#endif