#include "fps.h"
#include <stdio.h>
#include "window.h"


//���O���[�o���ϐ�
double g_fps = 1000.0f / 60.0f;			//�v�Z���ʂ������ň���
double g_wait = (double)timeGetTime();	//�v���O�������J�����ۂ̏������Ԃ��擾
int g_cnt = 0;							//�o�߉񐔌v�Z�p
double g_old = 0.0f;						//�O��̌o�߉񐔗p
int g_show = 0;							//�\���p

//************************************************************************************************
//Fps�̕\��
//************************************************************************************************
void DrawFps(int x, int y) {

	double now = 0.0;			//���݂̌o�ߎ��ԗp
	char mes[256] = { 0 };

	//�����񐔂����Z	
	g_cnt++;

	//���݂̎��Ԃ��擾
	now = (double)timeGetTime();

	//�P�b�Ԍo�߂����m�F
	if (now - g_old >= 1000) {

		//�O�񏈗����s�������Ԃ��X�V
		g_old = now;
		//�E�B���h�E�ɕ\������l���X�V
		g_show = g_cnt;
		//�P�b�Ԃ̏����񐔂����Z�b�g
		g_cnt = 0;

	}
	//���݂̂e�o�r����ʂɏo��
	wsprintf(&mes[0], "FPS = %d", g_show);
	DDTextOut(&mes[0], x, y);
}

//************************************************************************************************
//WaitFps�֐�
//************************************************************************************************
bool WaitFps() {

	double now = 0.0;			//���݂̌o�ߎ��ԗp

	//���Ԃ��擾
	now = (double)timeGetTime();

	//1000/60�~���b�Ԃ��Ƃ�true��Ԃ�
	if (now - g_wait >= g_fps) {
		//�O�񏈗����s�������Ԃ��X�V
		do {
			g_wait += g_fps;	//g_wait��g_fps�̒l�����Z��������

		} while (now - g_wait >= g_fps);
		//g_fps�~���b�o�߂����̂Łutrue�v��Ԃ�
		return true;
	}
	return false;
}
