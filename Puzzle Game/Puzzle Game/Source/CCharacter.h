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

	//�摜�̗v�f������
	int		m_ImgNum;
	
public:

	TPOINT<float>	m_Fpos;		//���W������
	TPOINT<float>	m_StartFpos;//starting coordinates
	TPOINT<int>		m_Ipic;		//���W��������
	TPOINT<float>	m_Fmove;	//�ړ��ʂ�����
	TPOINT<int>		m_Isize;	//�T�C�Y������
	bool	m_existFlag;		//�����t���O
	float	m_scale;			//�g�k�̔{��
	float	m_alpha;			//���ߗ�
	int		m_kindNum;			//��ޔԍ�

	//���v���g�^�C�v�錾
	//�R���X�g���N�^
	CCharacter();
	//�f�B�X�g���N�^
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

	//�e��ݒ�p�֐�
	void	SetPosition( TPOINT<float> Tpos ){ this->m_Fpos = Tpos; }	//���W
	void	SetMove( TPOINT<float> Tmove ){ this->m_Fmove = Tmove; }	//�ړ���
	void	SetSize( TPOINT<int> Tsize ){ this->m_Isize = Tsize; }		//�T�C�Y
	//�摜�̗v�f�ԍ�
	void	SetCharaImg( int ImgNum ){ this->m_ImgNum = ImgNum; }

	//�e��擾�p�֐�
	TPOINT<float>	GetPosition() const { return this->m_Fpos; }		//���W
	TPOINT<float>	GetMove() const { return this->m_Fmove; }			//�ړ���
	TPOINT<int>		GetSize() const { return this->m_Isize; }			//�T�C�Y
	//�摜�̗v�f�ԍ�
	int		GetImageNum() const { return this->m_ImgNum; }

	//�� �������z�֐��̒�`
	virtual void	InitChara() {}					//������
	virtual void	UpdateChara() {}				//�X�V	
	virtual void	DrawChara() {}					//�\��
	virtual void	ReleaseChara() = 0;				//���

};


#endif