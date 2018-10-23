#include "CFps.h"
#include <stdio.h>
#include "window.h"

//�� �ÓI�����o�ϐ��̍Ē�`
//�������x������
const double CFps::FPS_CONSTRAIN_TIME = 1000.0f / 60.0f;


CFps::CFps(){

	this->m_Wait = timeGetTime();			//set fps in this time
	this->m_Old = 0;			//�O�񏈗����s��������
	this->m_Show = 0;			//�\������FPS
	this->m_Cnt = 0;			//������	
}

CFps::~CFps() {}

void CFps::DrawFps( int x , int y ){

	double now = 0.0;			//���݂̌o�ߎ��ԗp
	char mes[256] = {0};

	//�����񐔂����Z	
	this->m_Cnt++;

	//���݂̎��Ԃ��擾
	now = ( double )timeGetTime();

	//�P�b�Ԍo�߂������m�F
	if( now - this->m_Old >= 1000 ){

		//�O�񏈗����s�������Ԃ��X�V
		this->m_Old = now;
		//�E�B���h�E�ɕ\������l���X�V
		this->m_Show = this->m_Cnt;
		//�P�b�Ԃ̏����񐔂����Z�b�g
		this->m_Cnt = 0;
	
	}
	//���݂̂e�o�r����ʂɏo��
	wsprintf( &mes[0] , "FPS = %d" , this->m_Show );
	DDTextOut( &mes[0] , x , y );
}

bool CFps::WaitFps() {

	double now = 0.0;			//���݂̌o�ߎ��ԗp

	//���Ԃ��擾
	now = (double)timeGetTime();

	//1000/60�~���b�Ԃ��Ƃ�true��Ԃ�
	if (now - this->m_Wait >= CFps::FPS_CONSTRAIN_TIME) {

		//�O�񏈗����s�������Ԃ��X�V
		do {
			this->m_Wait += CFps::FPS_CONSTRAIN_TIME;	//this->m_Wait��CFps::FPS_CONSTRAIN_TIME�̒l�����Z��������

		} while (now - this->m_Wait >= CFps::FPS_CONSTRAIN_TIME);

		//CFps::FPS_CONSTRAIN_TIME�~���b�o�߂����̂Łutrue�v��Ԃ�
		return true;

	}
	return false;
}
