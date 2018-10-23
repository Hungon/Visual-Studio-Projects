#ifndef CCHARACTER_H__
#define CCHARACTER_H__



//���w�b�_�[���C���N���[�h

//���\���̂̒�`
//�L�����N�^�[�̍��W������
//�e���v���[�g
template < typename T >
struct TPOINT{
	T	x;
	T	y;
};


//�� �L�����N�^�[�N���X�̒�`
class CCharacter{

private:


protected:

	//�摜�̗v�f������
	int		m_ImgNum;
	
public:

	TPOINT<float>	m_Fpos;		//���W������
	TPOINT<int>		m_Ipic;		//���W��������
	TPOINT<float>	m_Fmove;	//�ړ��ʂ�����
	TPOINT<int>		m_Isize;	//�T�C�Y������
	bool	m_existFlag;		//�����t���O
	float	m_scale;			//�g�k�̔{��
	float	m_alpha;			//���ߗ�
	int		m_direction;		//����
	int		m_kindNum;			//��ޔԍ�

	//���v���g�^�C�v�錾

	//�R���X�g���N�^
	CCharacter();
	//�f�B�X�g���N�^
	~CCharacter();

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

	/*
	***************************************************************************************************
	������CCharacter�N���X���A�b�v�L���X�g����
	�m�ۂ�����true��Ԃ�
	***************************************************************************************************
	*/
	bool	SetCharacter( CCharacter *lpcChara );
	
	
	//�� �������z�֐��̒�`
	virtual void	InitChara() = 0;				//������
	virtual void	UpdateChara() = 0;				//�X�V	
	virtual void	DrawChara() = 0;				//�\��
	virtual void	ReleaseChara() = 0;				//���

};


#endif