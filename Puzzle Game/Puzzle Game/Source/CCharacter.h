#ifndef		CCHARACTER_H__
#define		CCHARACTER_H__

#include	"window.h"

//decide character's center position to screen's size
#define	CHARACTER_CENTER_POSITION_X(w)	(SCREEN_WIDTH - w) / 2
#define	CHARACTER_CENTER_POSITION_Y(h)	(SCREEN_HEIGHT - h) / 2

//define template
template < typename T >		//In main, treat position
struct TPOINT{
	T	x;
	T	y;
};

//define the class
class CCharacter{

private:


protected:

	//画像の要素を扱う
	int		m_ImgNum;
	
public:

	TPOINT<float>	m_Fpos;		//座標を扱う
	TPOINT<float>	m_StartFpos;//starting coordinates
	TPOINT<int>		m_Ipic;		//座標元を扱う
	TPOINT<float>	m_Fmove;	//移動量を扱う
	TPOINT<int>		m_Isize;	//サイズを扱う
	bool	m_existFlag;		//生存フラグ
	float	m_scale;			//拡縮の倍率
	float	m_alpha;			//透過率
	int		m_kindNum;			//種類番号

	//▼プロトタイプ宣言
	//コンストラクタ
	CCharacter();
	//ディストラクタ
	~CCharacter();
	/*
	***********************************************************************************************
	Culculate character's center position and rotate process
	************************************************************************************************
	*/
	void	CenterCharacter(int len, int angle);
	/*
	************************************************************************************************
	Update bezier drawing
	************************************************************************************************
	*/
	void	SetBezier(TPOINT<int>* lpPos, int bezier, double time);

	//各種設定用関数
	void	SetPosition( TPOINT<float> Tpos ){ this->m_Fpos = Tpos; }	//座標
	void	SetMove( TPOINT<float> Tmove ){ this->m_Fmove = Tmove; }	//移動量
	void	SetSize( TPOINT<int> Tsize ){ this->m_Isize = Tsize; }		//サイズ
	//画像の要素番号
	void	SetCharaImg( int ImgNum ){ this->m_ImgNum = ImgNum; }

	//各種取得用関数
	TPOINT<float>	GetPosition() const { return this->m_Fpos; }		//座標
	TPOINT<float>	GetMove() const { return this->m_Fmove; }			//移動量
	TPOINT<int>		GetSize() const { return this->m_Isize; }			//サイズ
	//画像の要素番号
	int		GetImageNum() const { return this->m_ImgNum; }

	//▼ 純粋仮想関数の定義
	virtual void	InitChara() {}					//初期化
	virtual void	UpdateChara() {}				//更新	
	virtual void	DrawChara() {}					//表示
	virtual void	ReleaseChara() = 0;				//解放

};


#endif