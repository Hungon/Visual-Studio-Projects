#ifndef CFPS_H__
#define CFPS_H__


//���w�b�_�[�t�@�C���̓ǂݍ���
#include "graphics.h"


//FPS�N���X
class CFps{

private:

	int m_Cnt;				//�o�߉񐔌v�Z�p
	double m_Old;			//�O��̌o�߉񐔗p
	int m_Show;				//�\���p
	double m_Wait;			//�v���O�������J�����ۂ̏������Ԃ��擾

protected:

public:

	static const double FPS_CONSTRAIN_TIME;
	CFps();
	~CFps();
	void DrawFps( int x , int y );
	bool WaitFps();

};

#endif
