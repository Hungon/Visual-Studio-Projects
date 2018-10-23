#ifndef __WINDOW_H__
#define __WINDOW_H__		//���d��`�h�~�p


//���}�N���̒�`
#define SCREEN_WIDTH	640		//�E�B���h�̂̕�
#define SCREEN_HEIGHT	480		//�E�B���h�E�̍���
#define SCREEN_X		350		//�E�B���h�E�\���ʒuX
#define SCREEN_Y		120		//�E�B���h�E�\���ʒuY

//���w�b�_�[�t�@�C���̓ǂݍ���
#include <windows.h>


//���v���g�^�C�v�錾
//�}�E�X�̑��������
LRESULT CALLBACK WndProc( HWND hWnd , UINT message , WPARAM wParam , LPARAM lParam );
int GetMouseX();							//���݂̃}�E�X���W�w���擾����֐�
int GetMouseY();							//���݂̃}�E�X���W�x���擾����֐�
bool GetDownL();							//���N���b�N�������ꂽ��utrue�v��Ԃ��֐�


//������֐��̒�`
HWND InitWindow(char* title,int x,int y);		//�E�B���h�E�̍쐬�֐�


#endif