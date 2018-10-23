//�����C�u�����̒ǉ�
#pragma comment(lib,"winmm.lib")

//���w�b�_�C���N���[�h
#include <windows.h>
#include <stdio.h.>
#include "window.h"
#include "resource.h"

//���O���[�o���ϐ�
int g_mouseX = 0;			//�}�E�X�w���W
int g_mouseY = 0;			//�}�E�X�x���W
bool g_downL = false;		//�}�E�X���N���b�N�t���O

//***********************************************************************
//InitWindow�֐�
//[�@�\]		�E�B���h�E���쐬���\������
//[����]		�Ȃ�
//[�߂�l]		�E�B���h�E�n���h��
//***********************************************************************

//�E�B���h�E�N���X�̏����ݒ�
HWND InitWindow(char* title,int x,int y){

	WNDCLASSEX wcex ={0};									//�E�B���h�E�N���X�ݒ�p�̍\����
	HINSTANCE hInstance = GetModuleHandle(NULL);			//�C���X�^���X�n���h��
	HWND hWnd = NULL;										//�E�B���h�E�󂯎��
	RECT WindowRect = {								

	0,0,SCREEN_WIDTH,SCREEN_HEIGHT						//�E�B���h�E�̑傫��

	};

	char classname[32] = "Window_Sample";					//�E�B���h�E�N���X��

	//���菇�P�@�E�B���h�E�N���X�̐ݒ�
	wcex.cbSize = sizeof(wcex);																								//���g�̃T�C�Y
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;	//�E�B���h�E�N���X�̃X�^�C��
	wcex.lpfnWndProc = (WNDPROC)WndProc; 																		//�E�B���h�E�v���V�[�W��
	wcex.cbClsExtra = 0;																															//�g���p�̃p�����[�^�[
	wcex.cbWndExtra = 0;																															//�g���p�̃p�����[�^�[
	wcex.hInstance = hInstance;																								//�C���X�^���X�n���h��

	//�A�C�R���̃n���h��
	wcex.hIcon = LoadIcon(
				hInstance ,
				MAKEINTRESOURCE(IDI_ICON1));	
	//�J�[�\���̃n���h��
	wcex.hCursor = (HCURSOR)LoadImage(
	NULL,
	IDC_ARROW,
	IMAGE_CURSOR,
	0,0,
	LR_DEFAULTSIZE|LR_SHARED);

	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		//�o�b�N�J���[�ݒ�
	wcex.lpszMenuName = NULL;																																		//�N���X�̃��j���[
	wcex.lpszClassName = &classname[0];																								//�E�B���h�E�N���X��
	wcex.hIconSm = wcex.hIcon;																																	//�X���[���A�C�R���̃n���h��


		//���菇�Q�@�E�B���h�E�N���X�̓o�^
		if(!RegisterClassEx(&wcex)){

			//�߂�l���O�Ȃ�o�^���s�Areturn�ŏI��������
			printf("�E�B���h�E�N���X�̓o�^���s\n");

			return NULL;

		}

	//�N���C�A���g�̈�Ƙg�̃T�C�Y���܂߂��E�B���h�E�̕��ƍ��������߂�
	AdjustWindowRectEx(&WindowRect,WS_OVERLAPPEDWINDOW,FALSE,NULL);


	//���菇�R�@�E�B���h�E�N���X����ɃE�B���h�E���쐬
	hWnd=CreateWindowEx(
		0,																//�g���E�B���h�E�̃X�^�C��
		&classname[0],													//�E�B���h�E�N���X��
		title,															//�E�B���h�E�^�C�g��
		WS_OVERLAPPEDWINDOW,											//�E�B���h�E�X�^�C��
		x,																//�E�B���h�E�̍���X���W
		y,																//�E�B���h�E�̍���Y���W
		WindowRect.right-WindowRect.left,								//�E�B���h�E�̕�
		WindowRect.bottom-WindowRect.top,								//�E�B���h�E�̍���
		NULL,															//�e�E�B���h�E�̃n���h��
		NULL,															//���j���[�̃n���h��
		hInstance,														//�C���X�^���X�n���h��
		NULL															//�g���p�̃p�����[�^�[

	);


		//�E�B���h�E�쐬�̃G���[�`�F�b�N
		if(hWnd == NULL){

			printf("�E�B���h�E�쐬�ł���\n");

			return NULL;

		}


	//���菇�S�@�쐬�����E�B���h�E�̕\��
	ShowWindow(hWnd,SW_SHOW);


	//�E�B���h�E�̈�̍X�V
	UpdateWindow(hWnd);

	return hWnd;

}

//************************************************************************************************
//�}�E�X�̊��m
//************************************************************************************************
LRESULT CALLBACK WndProc( HWND hWnd , UINT message , WPARAM wParam , LPARAM lParam ){

	//���W�\���p
	char mes[256] = {0};

	HDC hDC = NULL;

	//���b�Z�[�W���Ƃ̏���
	switch( message ){

		//�}�E�X�̈ړ������m
		case WM_MOUSEMOVE:

			//���W�̎擾
			g_mouseX = lParam & 0x0000FFFF;
			g_mouseY = lParam >> 16;

		break;

		//���N���b�N�����������̏���
		case WM_LBUTTONDOWN:		

			//���N���b�N���������u�Ԃ́utrue�v
			g_downL = true;
			break;

		//�E�N���b�N�������ꂽ���̏���
		case WM_RBUTTONDOWN:		

			break;

		//���N���b�N�𗣂������̏���
		case WM_LBUTTONUP:

			//���N���b�N��������Ă���Ԃ͍��N���b�N�t���O���ufalse�v
			g_downL = false;

			break;

		//�E�N���b�N�𗣂������̏���
		case WM_RBUTTONUP:

			break;

		
		default:
			//DefWindowPro�v���V�[�W���ɏ�����C����
			return DefWindowProc( hWnd , message , wParam , lParam );
	}
	return 0;
}

//************************************************************************************************
//�}�E�X��X���W���擾
//************************************************************************************************
int GetMouseX() { return g_mouseX; }

//************************************************************************************************
//�}�E�X��Y���W���擾
//************************************************************************************************
int GetMouseY() { return g_mouseY; }

//************************************************************************************************
//���N���b�N�t���O��Ԃ��֐�
//************************************************************************************************
bool GetDownL() { return g_downL; }