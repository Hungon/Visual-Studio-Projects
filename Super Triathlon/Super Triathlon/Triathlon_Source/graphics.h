#ifndef __GARAPHICS_H__
#define __GARAPHICS_H__

//���}�N���̒�`
#define BPP 16				//�J���[���[�h�P�UbIT
#define MODE_FULLSCREEN 0	//�t���X�N���[�����[�h
#define MODE_WINDOW 1		//�E�B���h�E���[�h
#define IMAGE_MAX	50		//�摜�̖���
#define LOCK_BACK	-1		//�o�b�N�T�[�t�F�C�X�̃��b�N�w��

//�t�H���g�̎���
#define FONT_MIN	"�l�r ����"
#define FONT_GOS	"�l�r �S�V�b�N"

//DDScaleBltSelectIndicate
//Drawing starting point flag
#define DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP	0
#define DDSCALEBLT_SELECT_STARTING_POINT_CENTER		1

//���w�b�_�[�t�@�C���̃C���N���[�h
#include <windows.h>
#include <ddraw.h>
#include "character.h"

//���\���̂̒�`
typedef struct tagIMAGESURFACE{

	LPDIRECTDRAWSURFACE7 surface;	//�摜���������ރI�t�X�N���[���T�[�t�F�C�X
	char pass[32];					//�t�@�C���p�X
	bool flag;						//�g�p�t���O

}IMAGESURFACE,*LPIMAGESURFACE;


//���֐��̃v���g�^�C�v�錾
bool InitDDFullScreen(HWND hWnd);		//�t���X�N���[���̏�����
void ReleaseDirectDraw();				//���
void UpdatePrimary(HWND hWnd);			//�ŏI�o��
void FillBackSurface();					//�o�b�N�T�[�t�F�C�X�̓h��Ԃ�
LPDIRECTDRAWSURFACE7 GetBackSurface();	//�o�b�N�T�[�t�F�C�X�̎擾

//�E�B���h�E���[�h�ł̏�����
bool InitDDWindow(HWND hWnd);

//�X�N���[���̃��[�h��ύX����
void ChangeDisplayMode(HWND hWnd);

//�摜��ǂݍ���
int CreateDDImage(char* pass,HWND hWnd);

//�摜��\������
void DDBlt(int dst_x,int dst_y,int w,int h,int id,int src_x,int src_y);

//�����\��
void DDTextOut(char* mes,int x,int y);

//�t�H���g�ł̕����\��
void DDTextOutEx(char* mes ,int x, int y,int size,COLORREF color,bool Italic);

//�t�H���g�̕����̂�ύX����
void ChangeFont(char* mes);

//�N���b�s���O���s��
void Clipping(RECT* dst,RECT* src);	

//�摜�̊g�k���s��
void DDScaleBlt(float dst_x,float dst_y,int w,int h,int id,int src_x,int src_y,float pow);

//�摜�̊g�k���s����u�\������W�ύX�\�v
void DDScaleBltSelectIndicate( float dst_x , float dst_y , int w , int h , int id , int src_x , int src_y , float pow , char dstPointFlag );

//�摜�̕������̊g�k���s����u���b�N�A�A�����b�N�Ȃ��v
void DDScaleOnlyWidthBlt( float dst_x , float dst_y , int w , int h , int id , 
						 int src_x , int src_y , float pow );
//�g�k��N���b�s���O
void ClippingEx(RECT* pdst,RECT* psrc,float pow);

//�T�[�t�F�C�X���X�g�̑΍�
void RestoreSurface(HWND hWnd);

//���݂̃s�N�Z���t�H�[�}�b�g���擾���\������
void DrawBpp(int x,int y);

//�F����
DWORD DDRGB( DWORD r, DWORD g, DWORD b);

//�w��̉摜���������
void ReleaseDDImage(int id);

//����`����K
void DDRectBlt(int id,int dst_x,int dst_y,DWORD w,DWORD h,DWORD color);		

//�摜���w��̐F�ɕύX
void DDChangeColor(int id,DWORD src_color,DWORD dst_color);

//�F���w�肵�ĕ`��
void DDBltEx(int dst_x,int dst_y,DWORD w,DWORD h,int id,int src_x,int src_y);

//�g�k���s����`��֐�
void DDScaleBltEx(int dst_x,int dst_y,DWORD w,DWORD h,int id,int src_x,int src_y,float pow);

//�������̕`����s����֐��u���b�N�A�A�����b�N�L��v
void DDAlphaBltEx(int dst_x,int dst_y,DWORD w,DWORD h,int id,int src_x,int src_y);

//�������̕`��u���b�N�A�A�����b�N�Ȃ��v
void DDAlphaBlt(int dst_x,int dst_y,DWORD w,DWORD h,int id,int src_x,int src_y,float alpha);

//��̃T�[�t�F�C�X�𐶐�
int CreateDDOffScreen(int w,int h);

//�w��T�[�t�F�C�X�̓h��Ԃ�
void FillImage(int id,DWORD r,DWORD g,DWORD b);

//��]�̕`��i���b�N�A�A�����b�N�L��j
void DDRotateBltEx(int dst_x ,int dst_y,int w,int h,int id,int src_x,int src_y,int angle);

//�F�Ƃ̔�������֐��i���b�N�A�A�����b�N�L��j
bool ColisionDDRGB(CHARACTER* ch,int id,DWORD dst_color,DWORD src_color);

//���̕`��
void DDLine();

//���p�`�̕`��
void DDPolygon(POINT *p,int size);

//�~�̕`��
void DDCircle(float cx, float cy,int len);

#endif

