//�����C�u�����̒ǉ�
#pragma comment(lib,"ddraw.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"msimg32.lib")


//���w�b�_�[�t�@�C���̃C���N���[�h
#include "graphics.h"
#include "window.h"
#include <math.h>

//���O���[�o���ϐ�

//�v���C�}���t�F�C�X�̍쐬�p
LPDIRECTDRAW7	g_lpDD = {0};	
//�o�b�N�T�[�t�F�C�X�̍쐬�p				
LPDIRECTDRAWSURFACE7	g_lpDDSPrimary = {0};
//�N���C�A���g�̈�̍쐬�p
LPDIRECTDRAWSURFACE7	g_lpDDSBack = {0};
//�N���b�p�[�I�u�W�F�N�g
LPDIRECTDRAWCLIPPER g_lpDDClipper = NULL;
//�t���X�N���[�����[�h���E�B���h�E���[�h�̐؂�ւ�������
int g_displayModeNow = MODE_FULLSCREEN;			//���̃��[�h
int g_displayModeOld = MODE_FULLSCREEN;				//���O�̃��[�h
//�摜�������T�[�t�F�C�X�z��
IMAGESURFACE g_img[IMAGE_MAX] = {0};
//�����̂�����
char* g_Font = FONT_MIN;


//*****************************************************************************************
//�E�B���h�E���[�h�ł̏������@InitDDWindow
//*****************************************************************************************
bool InitDDWindow(HWND hWnd){
		
	//�߂�l�󂯎��p
	BOOL ret = {0};
	//�v���C�}���t�F�C�X�̎g�p
	DDSURFACEDESC2 ddsd = {0};
	//�o�b�N�T�[�t�F�C�X�̎g�p
	DDSCAPS2	ddsCaps = {0};
	//�N���C�A���g�̈�̒����p
	RECT rect = {0};


	//���̒��E�B���h�E���[�h�̏������̏���������
	//���菇�P�@�E�B���h�E�X�^�C���̐ݒ�
	SetWindowLong(hWnd,GWL_STYLE,WS_OVERLAPPEDWINDOW);
	SetWindowLong(hWnd,GWL_EXSTYLE,0);
	
	//���菇�Q�@DirectDraw�I�u�W�F�N�g�̐���
	ret = DirectDrawCreateEx(NULL , (void**)&g_lpDD , IID_IDirectDraw7 , NULL);
	//�G���[�`�F�b�N
	if(ret != DD_OK){
		MessageBox(NULL ,"DD�I�u�W�F�N�g�̐����Ɏ��s\n" , "DirectDraw" ,MB_OK);
		return false;
	}

	//���菇�R�@�������x���̐ݒ�
	ret = g_lpDD->SetCooperativeLevel(hWnd,DDSCL_NORMAL);

	//�G���[�`�F�b�N
	if(ret != DD_OK){
		MessageBox(NULL ,"�������x���̐ݒ�Ɏ��s\n" , "DirectDraw" ,MB_OK);
		return false;
	}
	
	//���菇�S�@�v���C�}���[�T�[�t�F�C�X�̐ݒ�
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	
	//dds�̒��g�����ƂɃT�[�t�F�C�X�𐶐�
	ret = g_lpDD->CreateSurface(&ddsd,&g_lpDDSPrimary,NULL);
	
	//���菇�T�@�o�b�N�T�[�t�t�F�C�X�̐���
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = SCREEN_WIDTH;
	ddsd.dwHeight = SCREEN_HEIGHT;
	//ddsd�����ƂɃT�[�t�F�C�X�𐶐�
	ret = g_lpDD->CreateSurface(&ddsd,&g_lpDDSBack,NULL);

	//�N���C�A���g�̈�̐ݒ�
	SetRect( &rect , 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT );
	AdjustWindowRectEx( &rect , WS_OVERLAPPEDWINDOW , FALSE , 0 );

	//��x�����E�B���h�E������ɕ\�����A����Ɏc�鍕����`������
	SetWindowPos(hWnd,HWND_TOP,0,0,rect.right - rect.left,rect.bottom - rect.top,SWP_SHOWWINDOW);

	//�\���ʒu�̐ݒ�i�N���C�A���g�̈�̔��f�j
	SetWindowPos(hWnd,HWND_TOP,SCREEN_X,SCREEN_Y,
				rect.right - rect.left,
				rect.bottom - rect.top,
				SWP_SHOWWINDOW);


	//���菇�U�@�N���b�p�[�I�u�W�F�N�g�̐���
	ret = g_lpDD->CreateClipper(0,&g_lpDDClipper,NULL);
	//�G���[�`�F�b�N
	if(ret != DD_OK){
		MessageBox(NULL,"�N���b�p�[�I�u�W�F�N�g�̓ǂݍ��݂Ɏ��s","DIRECT_DRAW",MB_OK);
	}
	//�E�B���h�E�ɐݒ�
	ret = g_lpDDClipper->SetHWnd(0,hWnd);
	//�G���[�`�F�b�N
	if(ret != DD_OK){
		MessageBox(NULL,"�E�B���h�E�ݒ�Ɏ��s","DIRECT_DRAW",MB_OK);
	}
	//�v���C�}���[�T�[�t�F�C�X�ɐݒ�
	ret = g_lpDDSPrimary->SetClipper(g_lpDDClipper);
	//�G���[�`�F�b�N
	if(ret != DD_OK){
		MessageBox(NULL,"�v���C�}���[�T�[�t�F�C�X�̐ݒ�Ɏ��s","DIRECT_DRAW",MB_OK);
	}
	
	//�������ɐ��������猻�݂̃��[�h���E�B���h�E��
	g_displayModeNow = MODE_WINDOW;

	//�������ɐ���
	return true;
}

//****************************************************************************
//DirectDraw�i�t���X�N���[���j������	InitDDFullScreen
//****************************************************************************

bool InitDDFullScreen(HWND hWnd){

	//�v���C�}���t�F�C�X�̎g�p
	DDSURFACEDESC2 ddsd = {0};
	//�o�b�N�T�[�t�F�C�X�̎g�p
	DDSCAPS2	ddsCaps = {0};
	//�N���C�A���g�̈�̒����p
	RECT rect = {0};
	//�߂�l�󂯎��p
	BOOL ret = {0};


	//�菇�P���@�E�B���h�E�X�^�C���̕ύX
	SetWindowLong(hWnd , GWL_STYLE , WS_POPUP);
	SetWindowLong(hWnd , GWL_EXSTYLE , 0);


	//�菇�Q���@DirectDraw�I�u�W�F�N�g�̐���
	ret = DirectDrawCreateEx(NULL , (void**)&g_lpDD , IID_IDirectDraw7 , NULL);
	//�G���[�`�F�b�N
	if(ret != DD_OK){
		MessageBox(NULL ,"DD�I�u�W�F�N�g�̐����Ɏ��s\n" , "DirectDraw" ,MB_OK);
		return false;
	}

	//�菇�R���@�������x���̐ݒ�
	ret = g_lpDD->SetCooperativeLevel(hWnd , DDSCL_EXCLUSIVE |
			DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT);

	//�G���[�`�F�b�N
	if(ret != DD_OK){
		MessageBox(NULL ,"�������x���̐ݒ�Ɏ��s\n" , "DirectDraw" ,MB_OK);
		return false;
	}

	//�菇�S���@��ʃ��[�h�̐ݒ�
	ret = g_lpDD->SetDisplayMode( SCREEN_WIDTH , SCREEN_HEIGHT , BPP , 0 , 0 );
	//�G���[�`�F�b�N
	if(ret != DD_OK){
		MessageBox(NULL ,"��ʃ��[�h�̐ݒ�Ɏ��s\n" , "DirectDraw" ,MB_OK);
		return false;
	}

	//�菇�T���@�v���C�}���T�[�t�F�C�X�i����ʁj�̐���
	ZeroMemory(&ddsd , sizeof(ddsd));		//�N���A
	//�T�C�Y�ݒ�
	ddsd.dwSize = sizeof(ddsd);
	//�t���O�ݒ�
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	//�o�b�N�T�[�t�F�C�X���̐ݒ�
	ddsd.dwBackBufferCount = 1; 
	//�o�b�N�T�[�t�F�C�X�̍쐬
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
							DDSCAPS_COMPLEX | DDSCAPS_FLIP;

	//��L�̐ݒ���v���C�}���T�[�t�F�C�X�ɍ쐬
	ret = g_lpDD->CreateSurface(&ddsd , &g_lpDDSPrimary , NULL);
	//�G���[�`���b�N
	if(ret != DD_OK){
		MessageBox(NULL ,"�v���C�}���T�[�t�F�C�X�̍쐬�Ɏ��s\n" , "DirectDraw" ,MB_OK);
		return false;
	}

	//�菇�U���@�o�b�N�T�[�t�F�C�X�i���z��ʁj�̐���
	ZeroMemory(&ddsCaps , sizeof(ddsCaps));

	//�T�[�t�F�C�X�̎�ނ��o�b�N�ɐݒ�
	ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;

	//�v���C�}���T�[�t�F�C�X�̎擾
	ret = g_lpDDSPrimary->GetAttachedSurface(&ddsCaps , &g_lpDDSBack);

	//�G���[�`�F�b�N
	if(ret != DD_OK){
		MessageBox(NULL ,"�o�b�N�T�[�t�F�C�X�̎擾�Ɏ��s\n" , "DirectDraw" ,MB_OK);
		return false;
	}
	
	//�������ɐ��������猻�݂̃��[�h���t���X�N���[�����[�h��
	g_displayModeNow = MODE_FULLSCREEN;

	return true;
}


//***********************************************************************************
//DD�S�ʂ̉���iDD�I�u�W�F�N�g�̊܂ޑS�Ẳ���j
//***********************************************************************************

void ReleaseDirectDraw(){

	int i = 0;
	
	//���S�ẴI�t�X�N���[���T�[�t�F�C�X�����
	for(i = 0;i < IMAGE_MAX;i++){
		if(g_img[i].surface){
			//�T�[�t�F�C�X�������Ă���Ȃ���
			g_img[i].surface->Release();
			g_img[i].surface = NULL;
		}
	}	
	//���N���b�p�[�I�u�W�F�N�g�̉��
	if(g_lpDDClipper){
		g_lpDDClipper->Release();
		g_lpDDClipper = NULL;
	}
	//���o�b�N�[�T�[�t�F�C�X�̉��
	if(g_lpDDSBack){
		g_lpDDSBack->Release();
		g_lpDDSBack = NULL;
	}
	//���v���C�}���T�[�t�F�C�X�̉��
	if(g_lpDDSPrimary){
		g_lpDDSPrimary->Release();
		g_lpDDSPrimary = NULL;
	}
	//��DD�I�u�W�F�N�g�̉��
	if(g_lpDD){
		g_lpDD->Release();
		g_lpDD = NULL;
	}
}



//***************************************************************************************
//�v���C�}���T�[�t�F�C�X�̐؂�ւ��i�ŏI�o�́j
//***************************************************************************************

void UpdatePrimary(HWND hWnd){

	RECT dst = {0};		//�]����̋�`
	RECT src = {0};		//�]�����Ƃ̋�`
	POINT pos = {0};		//�E�B���h�E�̍��W

	//���E�B���h�E���[�h�ł̍ŏI�o��
	ClientToScreen(hWnd,&pos);
	SetRect( &src , 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT );
	SetRect( &dst , pos.x , pos.y , pos.x + SCREEN_WIDTH , pos.y + SCREEN_HEIGHT );

	//�G���[�`�F�b�N
	if(!g_lpDDSPrimary){
		MessageBox(NULL , "�t���b�v�Ɏ��s" , "DirectDraw" , MB_OK);
	}
	//�����݂̃��[�h���t���X�N���[�����[�h�Ȃ�
	if( g_displayModeNow == MODE_FULLSCREEN ){
		//�v���C�}���T�[�t�F�C�X�̃t���b�v
		g_lpDDSPrimary->Flip( NULL , DDFLIP_WAIT );
	}else{
		//�]����A���̋�`���g���ăo�b�N�T�[�t�F�C�X��\��
		g_lpDDSPrimary->Blt( &dst , g_lpDDSBack , &src , DDBLT_WAIT , NULL );
	}
}


//****************************************************************************************
//�o�b�N�T�[�t�F�C�X�̓h��Ԃ�
//****************************************************************************************

void FillBackSurface(){

	//�h��Ԃ��p
	DDBLTFX		ddbltfx = {0};

	//�G���[�`�F�b�N
	if(!g_lpDDSBack){
		MessageBox(NULL , "�o�b�N�T�[�t�F�C�X�̓h��Ԃ��Ɏ��s\n" , "DirectDraw" ,MB_OK);
		return;
	}

	//���h��Ԃ�
	//���T�C�Y�w��
	ddbltfx.dwSize = sizeof(ddbltfx);
	
	//����ʂ�h��Ԃ�
	ddbltfx.dwFillColor = DDRGB( 0 , 0 , 0 );

	//���F��]��
	g_lpDDSBack->Blt( NULL , NULL , NULL , DDBLT_WAIT | DDBLT_COLORFILL , &ddbltfx );

}


//*****************************************************************************************
//�o�b�N�T�[�t�F�C�X�̎擾
//*****************************************************************************************

LPDIRECTDRAWSURFACE7 GetBackSurface(){
	//�G���[�`�F�b�N
	if(!g_lpDDSBack){
		MessageBox(NULL , "�o�b�N�T�[�t�F�C�X�擾���s\n" , "DirectDraw" , MB_OK);
		return NULL;
	}
	return g_lpDDSBack;
}


//*****************************************************************************************
//�X�N���[�����[�h��ύX����
//*****************************************************************************************
void ChangeDisplayMode(HWND hWnd){

	int i = 0;
	char mes[32] = {0};
	bool (*pDisplayMode[2])(HWND) = {
		InitDDFullScreen,
		InitDDWindow,
	};

	//���݂̃��[�h�����
	ReleaseDirectDraw();

	//���O�̃��[�h���擾
	g_displayModeOld = g_displayModeNow;
	//���[�h�؂�ւ�
	g_displayModeNow++;
	g_displayModeNow %= 2;

	//��ʂ̃��[�h�����݂ɕύX����
	pDisplayMode[g_displayModeNow]( hWnd );

	//�摜�̍ēǂݍ���
	for(i = 0;i < IMAGE_MAX;i++){
		if(g_img[i].flag == true){
			//i�Ԗڂ̃p�X��true�Ȃ�ēǂݍ���
			CreateDDImage(&g_img[i].pass[0],hWnd);
		}
	}
	//���݂̃��[�h��\��
	if(g_displayModeNow == MODE_WINDOW){
		DDTextOut("�E�B���h�E���[�h",0,50);
	}
	if(g_displayModeNow == MODE_FULLSCREEN){
		DDTextOut("�t���X�N���[�����[�h",0,50);
	}
}

//*****************************************************************************************
//�摜�̓ǂݍ���
//*****************************************************************************************
int CreateDDImage(char* pass,HWND hWnd){

	HBITMAP hBit = NULL;		//�摜�p
	BITMAP bmp = {0};			//�摜���p
	DDSURFACEDESC2 ddsd = {0};	//�摜�������ݗp
	int i = 0;
	HDC hDC = NULL;
	HDC memDC = NULL;
	//�J���[�L�[
	DDCOLORKEY ddck = {0};

	//�J���[�L�[�̐ݒ�i���𓧉ߐF�ɂ���j��
	ZeroMemory(&ddck,sizeof(ddck));
	ddck.dwColorSpaceHighValue = DDRGB( 0 , 50 , 0 );
	ddck.dwColorSpaceLowValue = DDRGB( 0 , 50 , 0 );


	//���菇�P�@�r�b�g�}�b�v���̎擾
	hBit = (HBITMAP)LoadImage(NULL,pass ,
					IMAGE_BITMAP ,
					0 , 0 ,
					LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	//�G���[�`�F�b�N
	if(hBit == NULL){
		MessageBox(NULL , pass , "�ǂݍ��݂Ɏ��s" , MB_OK);
		return -1;
	}
	//�r�b�g�}�b�v���̎擾
	GetObject(hBit , sizeof(BITMAP) , &bmp);
	
	//���菇�Q�@�I�t�X�N���[���T�[�t�F�C�X�̐���
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);	
	ddsd.dwFlags = DDSD_CAPS |
			    DDSD_WIDTH |
			    DDSD_HEIGHT;
	//�T�[�t�F�C�X�̕����摜�Ɠ���
	ddsd.dwWidth = bmp.bmWidth;
	//�T�[�t�F�C�X�̍������摜�Ɠ����Ɂi���̒l�Ȃ甽�]���j
	ddsd.dwHeight = bmp.bmHeight > 0 ?
				 bmp.bmHeight :
				 bmp.bmHeight * -1;
	//��ނ̓I�t�X�N���[���T�[�t�F�C�X
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	//��̗v�f�ɃT�[�t�F�C�X�𐶐�
	for(i = 0;i < IMAGE_MAX;i++){
		if(g_img[i].surface == NULL){
			//���i�ԖڂɃT�[�t�F�C�X�𐶐�
			g_lpDD->CreateSurface(
			&ddsd , &g_img[i].surface , NULL);
				
			//���ߐF���w��
			g_img[i].surface->SetColorKey(
			DDCKEY_SRCBLT , &ddck);

			//���菇�R�@�r�b�g�}�b�v�̏�������
			hDC = GetDC(hWnd);
			memDC = CreateCompatibleDC(hDC);
			ReleaseDC(hWnd , hDC);
			//hBit��memDC���֘A�t��
			SelectObject(memDC , hBit);
			//�I�t�X�N���[���T�[�t�F�C�X��HDC���擾
			g_img[i].surface->GetDC(&hDC);
			//�T�[�t�F�C�X�ɉ摜��\��
			BitBlt(hDC , 0 , 0 , ddsd.dwWidth , ddsd.dwHeight , memDC , 0 , 0 , SRCCOPY);
			//�T�[�t�F�C�XHDC�̉��
			g_img[i].surface->ReleaseDC(hDC);
			//�摜��memDC�����
			DeleteDC(memDC);
			//�摜��hBit�����
			DeleteObject(hBit);
			//i�ԖڂɃt�@�C���p�X���R�s�[
			strcpy(&g_img[i].pass[0] , pass);
			//�g�p�t���O��true�ɂ���
			g_img[i].flag = true;
		
			//�ǂݍ��񂾗v�f�ԍ���Ԃ��i�菇�P�Ǝ菇�Q�ŋL�q�������́j
			return i;
		}
	} 
	return -1;	//�ǂݍ��ݐ���
}

//*****************************************************************************************
//�摜�̕\��
//*****************************************************************************************
void DDBlt(int dst_x,int dst_y,int w,int h,int id,int src_x,int src_y){

	RECT src = {0};
	RECT dst = {0};
	//�]������`�̐���
	SetRect(&src , src_x , src_y , src_x + w , src_y + h);
	//�]������`�̍쐬
	SetRect(&dst , dst_x , dst_y , dst_x + w , dst_y + h);
	
	//�쐬������`���N���b�s���O
	Clipping(&dst,&src);

	//�o�b�N�T�[�t�F�C�X��id�Ԃ̃T�[�t�F�C�X��\��
	g_lpDDSBack->BltFast(dst.left,dst.top,
						g_img[id].surface,&src,DDBLTFAST_WAIT | 
						DDBLTFAST_SRCCOLORKEY);
} 

//*****************************************************************************************
//�����\��
//*****************************************************************************************
void DDTextOut(char* mes,int x,int y){

	HDC hDC = NULL;
	//�o�b�N�T�[�t�F�C�X��HDC���擾
	g_lpDDSBack->GetDC(&hDC);
	//�������g����TextOut
	TextOut(hDC,x,y,mes,lstrlen(mes));
	//�o�b�N�T�[�t�F�C�X��hDC�����
	g_lpDDSBack->ReleaseDC(hDC);
}

//*****************************************************************************************
//�t�H���g�ł̕����\��
//*****************************************************************************************
void DDTextOutEx(char* mes,int x,int y ,int size,COLORREF color,bool Italic){

	HDC hDC = NULL;
	COLORREF defColor = 0;		//�ύX�O�̐F
	HFONT hFont = NULL;			//����̃t�H���g
	HFONT defFont = NULL;		//�ύX�O�̃t�H���g
	
	//�o�b�N�T�[�t�F�C�X��HDC���擾
	g_lpDDSBack->GetDC(&hDC);

	//�t�H���g�̐���
	hFont = CreateFont(size , 0 , 0 , 0 , FW_HEAVY , Italic , 
		false , false , SHIFTJIS_CHARSET ,
		OUT_DEFAULT_PRECIS , CLIP_DEFAULT_PRECIS , DEFAULT_QUALITY ,
		FIXED_PITCH | FF_DONTCARE , g_Font);

	//�t�H���g���o�b�N�T�[�t�F�C�X�Ɋ֘A�t��
	defFont = (HFONT)SelectObject(hDC , hFont);

	//�w�i�̓���
	SetBkMode(hDC , TRANSPARENT);

	//�����F�̕ύX�i���j
	defColor = SetTextColor(hDC , color);

	//�������g����TextOut
	TextOut(hDC , x , y , mes , lstrlen(mes));

	//�t�H���g�̉��
	DeleteObject(hFont);

	//�t�H���g�̏�����
	SelectObject(hDC , defFont);

	//�w�i�̓��߂����Z�b�g
	SetBkMode(hDC , OPAQUE);
	
	//�o�b�N�T�[�t�F�C�X��hDC�����
	g_lpDDSBack->ReleaseDC(hDC);
}

//*****************************************************************************************
//�t�H���g�̕����̂�ύX����
//*****************************************************************************************
void ChangeFont(char* mes){
	g_Font = mes;
}

//*****************************************************************************************
//�N���b�s���O���s���@Clipping
//*****************************************************************************************
void Clipping( RECT *dst , RECT *src ){
	
	//���[���͂ݏo����
	if( dst->left < 0 ){
		//�]�����̍��[���͂ݏo���Ԃ񂾂�����
		src->left -= dst->left;
		//�]����̍��[��0�ɂ���
		dst->left = 0;
	}
	//��[�̃N���b�s���O
	if( dst->top < 0 ){
		src->top -= dst->top;
		dst->top = 0;
	}
	//�E�[�̃N���b�s���O
	if( dst->right > SCREEN_WIDTH ){
		src->right -= ( dst->right - SCREEN_WIDTH );
		dst->right = SCREEN_WIDTH;
	}
	//���[�̃N���b�s���O
	if( dst->bottom > SCREEN_HEIGHT){
		src->bottom -= ( dst->bottom - SCREEN_HEIGHT );
		dst->bottom = SCREEN_HEIGHT;
	}
}

//*****************************************************************************************
//�摜�̊g�k���s����u���b�N�A�A�����b�N�Ȃ��v
//*****************************************************************************************
void DDScaleBlt( float dst_x , float dst_y , int w , int h , int id , int src_x , int src_y , float pow ){

	//�]������`
	RECT src = { 0 };
	//�]�����`
	RECT dst = { 0 };

	//�g��E�k����̕��E���������߂�
	float scale_w = (float)(w * pow);
	float scale_h = (float)(h * pow);
	//�摜�̒��S���W�����߂�
	float dst_center_x = (float)(w / 2);	//X���W
	float dst_center_y = (float)(h / 2);	//Y���W

	//�]������`���쐬
	SetRect( &src , src_x , src_y , src_x + w , src_y + h );
	//�g��E�k����̓]�����`���쐬
	//���܂������S���W����ɁA�\����w�E�x���W���C������
	SetRect( &dst , (int)( ( dst_x + dst_center_x ) - ( scale_w / 2 ) ) ,
		(int)( ( dst_y + dst_center_y ) - ( scale_h / 2 ) ) ,
		(int)(dst_x + dst_center_x + scale_w / 2) , 
		(int)(dst_y + dst_center_y + scale_h / 2) ); 

	//�쐬������`���N���b�s���O
	ClippingEx(&dst,&src,pow);

	//�`��
	g_lpDDSBack -> Blt(&dst, 								//�]����
			           	g_img[id].surface, 					//�摜�̃T�[�t�F�C�X
			            &src,								//�]����
				        DDBLT_WAIT | DDBLT_KEYSRC , NULL );	//�]���}�N��			
}


//*****************************************************************************************
//�摜�̕������̊g�k���s����u���b�N�A�A�����b�N�Ȃ��v
//*****************************************************************************************
void DDScaleOnlyWidthBlt( float dst_x , float dst_y , int w , int h , 
						 int id , int src_x , int src_y , float pow ){

	//�]������`
	RECT src = { 0 };
	//�]�����`
	RECT dst = { 0 };

	//�g��E�k����̕��E���������߂�
	float scale_w = (float)( w * pow );
	float scaleH = (float)( h * 1.0f );
	
	//�]������`���쐬
	SetRect( &src , src_x , src_y , src_x + w , src_y + h );
	
	//�g��E�k����̓]�����`���쐬
	SetRect( &dst , (int)dst_x , (int)dst_y ,
		(int)( dst_x + scale_w ) , (int)( dst_y + scaleH ) ); 

	//�쐬������`���N���b�s���O
	ClippingEx( &dst , &src , pow );

	//�`��
	g_lpDDSBack -> Blt(&dst, 								//�]����
			           	g_img[id].surface, 					//�摜�̃T�[�t�F�C�X
			            &src,								//�]����
				        DDBLT_WAIT | DDBLT_KEYSRC , NULL );	//�]���}�N��			
}

//*****************************************************************************************
//�摜�̊g�k���s����u���b�N�A�A�����b�N�Ȃ��v
//*****************************************************************************************
void DDScaleBltSelectIndicate( float dst_x , float dst_y , int w , int h , int id , int src_x , int src_y , float pow , char dstPointFlag ){

	//�]������`
	RECT src = { 0 };
	//�]�����`
	RECT dst = { 0 };

	//�g��E�k����̕��E���������߂�
	float scale_w = (float)( w * pow );
	float scale_h = (float)( h * pow );
	//�摜�̒��S���W�����߂�
	float dst_center_x = (float)(w / 2);	//X���W
	float dst_center_y = (float)(h / 2);	//Y���W

	//�]������`���쐬
	SetRect( &src , src_x , src_y , src_x + w , src_y + h );

	//��ʍ������_�Ɋg��E�k���������鏈��
	switch( dstPointFlag ){
		//�������_
		case DDSCALEBLT_SELECT_STARTING_POINT_LEFT_TOP:
			//�g��E�k����̓]�����`���쐬
			SetRect( &dst , (int)dst_x , (int)dst_y ,
				(int)( dst_x + scale_w ) , (int)( dst_y + scale_h ) );
			break;
		//���S����_
		case DDSCALEBLT_SELECT_STARTING_POINT_CENTER:
			//�g��E�k����̓]�����`���쐬
			//���܂������S���W����ɁA�\����w�E�x���W���C������
			SetRect( &dst , (int)( ( dst_x + dst_center_x ) - ( scale_w / 2 ) ) ,
				(int)( ( dst_y + dst_center_y ) - ( scale_h / 2 ) ) ,
				(int)(dst_x + dst_center_x + scale_w / 2) , (int)(dst_y + dst_center_y + scale_h / 2) ); 
			break;
	}

	//�쐬������`���N���b�s���O
	ClippingEx(&dst,&src,pow);

	//�`��
	g_lpDDSBack -> Blt(&dst, 								//�]����
			           	g_img[id].surface, 					//�摜�̃T�[�t�F�C�X
			            &src,								//�]����
				        DDBLT_WAIT | DDBLT_KEYSRC , NULL );	//�]���}�N��			
}


//*****************************************************************************************
//�g�k��̃N���b�s���O
//*****************************************************************************************
void ClippingEx(RECT *pdst,RECT *psrc,float pow){
	
	//���[�̃N���b�s���O
	if(pdst->left < 0){
		psrc->left -= (int)(pdst->left/pow);
		pdst->left = 0;
	}
	//��[�̃N���b�s���O
	if(pdst->top < 0){
		psrc->top -= (int)(pdst->top/pow);
		pdst->top = 0;
	}
	//�E�[�̃N���b�s���O
	if(pdst->right > SCREEN_WIDTH){
		psrc->right -= (int)((pdst->right - SCREEN_WIDTH) / pow);
		pdst->right = SCREEN_WIDTH;
	}
	//���[�̃N���b�s���O
	if(pdst->bottom > SCREEN_HEIGHT){
		psrc->bottom -= (int)((pdst->bottom - SCREEN_HEIGHT) / pow);
		pdst->bottom = SCREEN_HEIGHT;
	}
}

//*****************************************************************************************
//�T�[�t�F�C�X���X�g�΍�
//*****************************************************************************************
void RestoreSurface(HWND hWnd){
	int i = 0;
	//�v���C�}���[�̏C��
	if(g_lpDDSPrimary){
		//�������Ă��邩���m�F
		if(g_lpDDSPrimary->IsLost() == DDERR_SURFACELOST){
			//�������Ă����v���C�}���[���C��
			g_lpDDSPrimary->Restore();
		}	
	}
	//�o�b�N�̏C��
	if(g_lpDDSBack){
		//�������Ă��邩���m�F
		if(g_lpDDSBack->IsLost() == DDERR_SURFACELOST){
			//�������Ă����o�b�N���C��
			g_lpDDSBack->Restore();
		}	
	}
	//�I�t�X�N���[���i�摜�j�̏C��
	for(i = 0;i < IMAGE_MAX;i++){
		if(g_img[i].surface){
			//�������Ă��Ȃ������m�F
			if(g_img[i].surface->IsLost() == DDERR_SURFACELOST){
				//�I�t�X�N���[����������čēǂݍ���
				g_img[i].surface->Release();
				g_img[i].surface = NULL;
				CreateDDImage(&g_img[i].pass[0],hWnd);
			}
		}
	}
}

//*****************************************************************************************
//���݂̃s�N�Z���t�H�[�}�b�g��\��
//*****************************************************************************************
void DrawBpp(int x,int y){
	
	char mes[32] = {0};
	DDPIXELFORMAT ddpf = {0};
	
	//�e�F��2�i���m�F�p
	DWORD rgb = DDRGB(255,15,3);

	//�s�N�Z���t�H�[�}�b�g���擾
	ZeroMemory(&ddpf,sizeof(ddpf));
	ddpf.dwSize = sizeof(ddpf);
	g_lpDDSBack->GetPixelFormat(&ddpf);
	
	//���݂̃s�N�Z���t�H�[�}�b�g��\��
	wsprintf(&mes[0],"BPP = %d",ddpf.dwRGBBitCount);
	DDTextOut(&mes[0],x,y);
	
	//�Ԑ����̃}�X�N�̒l
	wsprintf(&mes[0],"�� = %d",ddpf.dwRBitMask);
	DDTextOut(&mes[0],x+150,y);
	wsprintf(&mes[0],"�� = %d",ddpf.dwGBitMask);
	DDTextOut(&mes[0],x+300,y);
	wsprintf(&mes[0],"�� = %d",ddpf.dwBBitMask);
	DDTextOut(&mes[0],x+450,y);

	//rgb�����F���R�F�ɕ�������
	wsprintf(&mes[0],"R = %d",rgb & ddpf.dwRBitMask);
	DDTextOut(&mes[0] , x , y + 20);
	wsprintf(&mes[0],"G = %d",rgb & ddpf.dwGBitMask);
	DDTextOut(&mes[0] , x , y + 40);
	wsprintf(&mes[0],"B = %d",rgb & ddpf.dwBBitMask);
	DDTextOut(&mes[0] , x , y + 60);


}

//*****************************************************************************************
//�F�̍���
//*****************************************************************************************
DWORD DDRGB( DWORD r , DWORD g , DWORD b ){
	 
	DWORD dwColor = 0;				//�F�����p
	DDPIXELFORMAT ddpf = {0};		//�s�N�Z���t�H�[�}�b�g
	BYTE RShift = 0;				//�Ԃ̃V�t�g�l
	BYTE RMask = 0;					//�Ԃ̃}�X�N�l
	BYTE GShift = 0;				//�΂̃V�t�g�l
	BYTE GMask = 0;					//�΂̃}�X�N�l
	BYTE BShift = 0;				//�̃V�t�g�l
	BYTE BMask = 0;					//�̃}�X�N�l
	
	//���s�N�Z���t�H�[�}�b�g�̎擾
	ZeroMemory(&ddpf,sizeof(ddpf));
	ddpf.dwSize = sizeof(ddpf);
	g_lpDDSBack->GetPixelFormat(&ddpf);
	
	//���Ԃ�0�r�b�g�ڂ��O�Ȃ烋�[�v
	while(!(ddpf.dwRBitMask & 0x01)){
		//�O�̐��i�V�t�g�l�j�����Z
		RShift++;
		//���̃r�b�g�ڂ𒲂ׂ邽�߁A�}�X�N���E�ɃV�t�g
		ddpf.dwRBitMask = ddpf.dwRBitMask >> 1;
	}
	//�Ԃ�0�r�b�g�ڂ��P�Ȃ烋�[�v
	while((ddpf.dwRBitMask & 0x01)){
		//�P�̐������Z
		RMask++;
		//���̃r�b�g�ڂ𒲂ׂ邽�߁A�}�X�N���E�ɂP�V�t�g
		ddpf.dwRBitMask = ddpf.dwRBitMask >> 1;
	}
	//�}�X�N�l�A�V�t�g�l�����Ƃ�r��dwColor�ɒǉ�
	dwColor = dwColor | ((r >> (8-RMask)) << RShift);
	
	//���΂�0�r�b�g�ڂ��O�Ȃ烋�[�v
	while(!(ddpf.dwGBitMask & 0x01)){
		//�O�̐��i�V�t�g�l�j�����Z
		GShift++;
		//���̃r�b�g�ڂ𒲂ׂ邽�߁A�}�X�N���E�ɃV�t�g
		ddpf.dwGBitMask = ddpf.dwGBitMask >> 1;
	}
	//�΂�0�r�b�g�ڂ��P�Ȃ烋�[�v
	while((ddpf.dwGBitMask & 0x01)){
		//�P�̐������Z
		GMask++;
		//���̃r�b�g�ڂ𒲂ׂ邽�߁A�}�X�N���E�ɂP�V�t�g
		ddpf.dwGBitMask = ddpf.dwGBitMask >> 1;
	}
	//�}�X�N�l�A�V�t�g�l�����Ƃ�g��dwColor�ɒǉ�
	dwColor = dwColor | ((g >> (8-GMask)) << GShift);

	//����0�r�b�g�ڂ��O�Ȃ烋�[�v
	while(!(ddpf.dwBBitMask & 0x01)){
		//�O�̐��i�V�t�g�l�j�����Z
		BShift++;
		//���̃r�b�g�ڂ𒲂ׂ邽�߁A�}�X�N���E�ɃV�t�g
		ddpf.dwBBitMask = ddpf.dwBBitMask >> 1;
	}
	//��0�r�b�g�ڂ��P�Ȃ烋�[�v
	while((ddpf.dwBBitMask & 0x01)){
		//�P�̐������Z
		BMask++;
		//���̃r�b�g�ڂ𒲂ׂ邽�߁A�}�X�N���E�ɂP�V�t�g
		ddpf.dwBBitMask = ddpf.dwBBitMask >> 1;
	}
	//�}�X�N�l�A�V�t�g�l�����Ƃ�b��dwColor�ɒǉ�
	dwColor = dwColor | ((b >> (8-BMask)) << BShift);


	//��������
	return dwColor;
}

//*****************************************************************************************
//�w��̉摜�̉��
//*****************************************************************************************
void ReleaseDDImage(int id){

	//�G���[�`�F�b�N
	//�����̉摜�̗v�f���}�N���̒l�̍ő喇����葽���Ȃ�
	if(id >= IMAGE_MAX){
		MessageBox(NULL ,"�摜�̉���Ɏ��s\n" , "ReleaseDDImage" ,MB_OK);
	}
	//�w��̗v�f���T�[�t�F�C�X�����Ȃ���
	if( g_img[id].surface ){
		g_img[id].surface->Release();
		//id�Ԃ̃����o��0�ŏ�����
		ZeroMemory(&g_img[id] , sizeof(IMAGESURFACE));
	}
}

//*****************************************************************************************
//����`����K
//*****************************************************************************************
void DDRectBlt(int id,int dst_x,int dst_y,DWORD w,DWORD h,DWORD color){

	DDSURFACEDESC2 ddsd = {0};
	DWORD* pd = NULL;	//32�r�b�g���[�h�ł̃|�C���^
	WORD* pw = NULL;	//16�r�b�g���[�h�ł̃|�C���^
	unsigned long i = 0,j = 0;
	//���b�N����T�[�t�F�C�X������
	LPDIRECTDRAWSURFACE7 surface = NULL;
	//�J���[�L�[
	DDCOLORKEY ddck = {0};
	//�s�N�Z���t�H�[�}�b�g�p	
	DDPIXELFORMAT ddpf = {0};

	//�s�N�Z���t�H�[�}�b�g���擾
	ZeroMemory(&ddpf,sizeof(ddpf));
	ddpf.dwSize = sizeof(ddpf);
	g_lpDDSBack->GetPixelFormat(&ddpf);

	//����id�����ƂɃ��b�N�Ώۂ��w��
	if(id >= 0){
		//id��0�ȏ�Ȃ�Ή�����摜�����b�N
		surface = g_img[id].surface;
	}else{
		//id���O��菬�����Ȃ�A�o�b�N�T�[�t�F�C�X�����b�N
		surface = g_lpDDSBack;
	}

	//��dsd�Ɏ擾����T�[�t�F�C�X�̐ݒ�
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	//���菇�P�@�T�[�t�F�C�X�����b�N
	surface->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL);
	
	//���菇�Q�@�擪�|�C���^���R�s�[
	pd = (DWORD*)ddsd.lpSurface;			//32�r�b�g���[�h
	pw = (WORD*)ddsd.lpSurface;				//16�r�b�g���[�h

	//���菇�R�@�F�𑀍�
	//32�r�b�g���[�h�̏ꍇ �u�E�B���h�E���[�h�v
	if(ddpf.dwRGBBitCount == 32){
		for(j = 0;j < h;j++){
			for(i = 0;i < w;i++){
				//���W���͈͊O�Ȃ������΂�
				if(j + dst_y < 0 ||
					j + dst_y >= ddsd.dwHeight ||
					i + dst_x < 0 ||
					i + dst_x >= ddsd.dwWidth){
					continue;
				}
				//X:0�AY:0�̈ʒu�ɕ�16����32�̋�`
				*(pd+(j + dst_y) * (ddsd.lPitch/4) + (i + dst_x)) = color;
			}
		}
	}
	//16�r�b�g���[�h�̏ꍇ �u�t���X�N���[�����[�h�v
	if(ddpf.dwRGBBitCount == 16){
		for(j = 0;j < h;j++){
			for(i = 0;i < w;i++){
				//X:0�AY:0�̈ʒu�ɕ�16����32�̋�`
				*(pw+(j + dst_y) * (ddsd.lPitch/2) + (i + dst_x)) = (unsigned short)color;
			}
		}
	}
	//���菇�S�@�T�[�t�F�C�X�̃A�����b�N
	surface->Unlock(NULL);
}

//*****************************************************************************************
//�F�̕ύX
//*****************************************************************************************
void DDChangeColor( int id , DWORD src_color , DWORD dst_color ){

	DDSURFACEDESC2 ddsd = {0};
	DWORD* pd = NULL;			//32�r�b�g���[�h�ł̃|�C���^
	WORD* pw = NULL;			//16�r�b�g���[�h�ł̃|�C���^
	unsigned long i = 0,j = 0;
	//���b�N����T�[�t�F�C�X������
	LPDIRECTDRAWSURFACE7 surface = NULL;
	//�J���[�L�[
	DDCOLORKEY ddck = {0};
	//�s�N�Z���t�H�[�}�b�g�p	
	DDPIXELFORMAT ddpf = {0};

	//�s�N�Z���t�H�[�}�b�g���擾
	ZeroMemory(&ddpf,sizeof(ddpf));
	ddpf.dwSize = sizeof(ddpf);
	g_lpDDSBack->GetPixelFormat(&ddpf);

	//����id�����ƂɃ��b�N�Ώۂ��w��
	if(id >= 0){
		//id��0�ȏ�Ȃ�Ή�����摜�����b�N
		surface = g_img[id].surface;
	}else{
		//id���O��菬�����Ȃ�A�o�b�N�T�[�t�F�C�X�����b�N
		surface = g_lpDDSBack;
	}

	//��dsd�Ɏ擾����T�[�t�F�C�X�̐ݒ�
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	//���菇�P�@�T�[�t�F�C�X�����b�N
	surface->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL);
	
	//���菇�Q�@�擪�|�C���^���R�s�[
	pd = (DWORD*)ddsd.lpSurface;			//32�r�b�g���[�h
	pw = (WORD*)ddsd.lpSurface;				//16�r�b�g���[�h

	//���菇�R�@���b�N�������T�[�t�F�C�X�̐F��ς���
	//32�r�b�g���[�h
	if(ddpf.dwRGBBitCount == 32){
		for(j = 0;j < ddsd.dwHeight;j++){
			for(i = 0;i < ddsd.dwWidth;i++){
				//X:i�AY:j�̍��W��src_color�Ɠ����F�����ׂ�
				if(*(pd + (j * (ddsd.lPitch / 4)) + i) == src_color){
					//�F��dst_color�����F�ɕύX
					*(pd + (j * (ddsd.lPitch / 4)) + i) = dst_color;
				}
			}
		}
	}
	//16�r�b�g���[�h
	if(ddpf.dwRGBBitCount == 16){
		for(j = 0;j < ddsd.dwHeight;j++){
			for(i = 0;i < ddsd.dwWidth;i++){
				//X:i�AY:j�̍��W��src_color�Ɠ����F�����ׂ�
				if(*(pw + (j * (ddsd.lPitch / 2)) + i) == src_color){
					//�F��dst_color�����F�ɕύX
					*(pw + (j * (ddsd.lPitch / 2)) + i) = (WORD)dst_color;
				}
			}
		}
	}
	//���菇�S�@�T�[�t�F�C�X�̃A�����b�N
	surface->Unlock(NULL);
}

//*****************************************************************************************
//�摜�̕\���i���b�N�A�A�����b�N�L��j
//*****************************************************************************************
void DDBltEx(int dst_x,int dst_y,DWORD w,DWORD h,int id,int src_x,int src_y){

	DDSURFACEDESC2 dst = {0};		//�]����̃T�[�t�F�C�X
	DDSURFACEDESC2 src = {0};		//�]�����̃T�[�t�F�C�X
	DWORD* pdst32 = NULL;			//�i32�r�b�g���[�h�j�]����̐擪�|�C���^
	DWORD* psrc32 = NULL;			//�i32�r�b�g���[�h�j�]�����̐擪�|�C���^
	WORD* pdst16 = NULL;			//�i16�r�b�g���[�h�j�]����̐擪�|�C���^
	WORD* psrc16 = NULL;			//�i16�r�b�g���[�h�j�]����̐擪�|�C���^
	//���[�v�p
	DWORD i = 0,j = 0;
	//�J���[�L�[
	DDCOLORKEY ddck = {0};
	//�s�N�Z���t�H�[�}�b�g�p	
	DDPIXELFORMAT ddpf = {0};

	//�s�N�Z���t�H�[�}�b�g���擾
	ZeroMemory(&ddpf,sizeof(ddpf));
	ddpf.dwSize = sizeof(ddpf);
	g_lpDDSBack->GetPixelFormat(&ddpf);

	//���菇�P�@dst�Asrc�̐ݒ�
	//dst�Ɏ擾����T�[�t�F�C�X�̐ݒ�
	ZeroMemory(&dst,sizeof(dst));
	dst.dwSize = sizeof(dst);
	
	//src�Ɏ擾����T�[�t�F�C�X�̐ݒ�
	ZeroMemory(&src,sizeof(src));
	src.dwSize = sizeof(src);

	//���菇�Q�@dst�Asrc�����Ƃ�2�̃T�[�t�F�C�X�����b�N
	g_lpDDSBack->Lock(NULL,&dst,DDLOCK_WAIT,NULL);
	g_img[id].surface->Lock(NULL,&src,DDLOCK_WAIT,NULL);

	//���菇�R�@���b�N�����Q�̃T�[�t�F�C�X����擪�|�C���^���擾
	pdst32 = (DWORD*)dst.lpSurface;		//�i32�r�b�g�j�]����
	psrc32 = (DWORD*)src.lpSurface;		//�i32�r�b�g�j�]����
	pdst16 = (WORD*)dst.lpSurface;		//�i16�r�b�g�j�]����
	psrc16 = (WORD*)src.lpSurface;		//�i16�r�b�g�j�]����

	//���菇�S�@�\������T�C�Y�����[�v���񂵁A�]�����Ƃ̐F��]����ɑ��
	//32�r�b�g���[�h
	if(ddpf.dwRGBBitCount == 32){
		for(j = 0;j < h;j++){
			for(i = 0;i < w;i++){
				//�]���悪�͈͊O�Ȃ��΂�
				if((j + dst_y) < 0 ||
					(i + dst_x) < 0 ||
					(j + dst_y) >= dst.dwHeight ||
					(i + dst_x) >= dst.dwWidth){
						continue;
				}
				//�]�������͈͊O�Ȃ珈�����΂�
				if((j + src_y) < 0 ||
					(i + src_x) < 0 ||
					(j + src_y) >= src.dwHeight ||
					(i + src_x) >= src.dwWidth){
						continue;
				}
				//���ߐF�Ȃ������΂�
				if(*(psrc32 + ((src_y + j) * (src.lPitch / 4)) + (src_x + i)) == DDRGB(0,50,0)){
					continue;
				}
				//�]�����Ƃ̉摜��1�h�b�g���]��
				*(pdst32 + ((dst_y + j) * (dst.lPitch / 4)) + (dst_x + i)) = 
					*(psrc32 + ((src_y + j) * (src.lPitch / 4)) + (src_x + i));
			}
		}
	}
	//16�r�b�g���[�h
	if(ddpf.dwRGBBitCount == 16){
		for(j = 0;j < h;j++){
			for(i = 0;i < w;i++){
				
				//�]���悪�͈͊O�Ȃ��΂�
				if((j + dst_y) < 0 ||
					(i + dst_x) < 0 ||
					(j + dst_y) >= dst.dwHeight ||
					(i + dst_x) >= dst.dwWidth){
						continue;
				}
				//�]�������͈͊O�Ȃ珈�����΂�
				if((j + src_y) < 0 ||
					(i + src_x) < 0 ||
					(j + src_y) >= src.dwHeight ||
					(i + src_x) >= src.dwWidth){
						continue;
				}
				//���ߐF�Ȃ������΂�
				if(*(psrc16 + ((src_y + j) * (src.lPitch / 2)) + (src_x + i)) == DDRGB(0,50,0)){
					continue;
				}
				//�]�����Ƃ̉摜��1�h�b�g���]��
				*(pdst16 + ((dst_y + j) * (dst.lPitch / 2)) + (dst_x + i)) = 
					*(psrc16 + ((src_y + j) * (src.lPitch / 2)) + (src_x + i));
			}
		}
	}
	//���菇�T�@�Q�̃T�[�t�F�C�X���A�����b�N
	g_img[id].surface->Unlock(NULL);
	g_lpDDSBack->Unlock(NULL);
}

//*****************************************************************************************
//�g�k���s����`��֐��u���b�N�A�A�����b�N�L��v
//*****************************************************************************************
void DDScaleBltEx(int dst_x,int dst_y,DWORD w,DWORD h,int id,int src_x,int src_y,float pow){

	DDSURFACEDESC2 dst = {0};		//�]����̃T�[�t�F�C�X
	DDSURFACEDESC2 src = {0};		//�]�����̃T�[�t�F�C�X
	DWORD* pdst32 = NULL;			//�i32�r�b�g���[�h�j�]����̐擪�|�C���^
	DWORD* psrc32 = NULL;			//�i32�r�b�g���[�h�j�]�����̐擪�|�C���^
	WORD* pdst16 = NULL;			//�i16�r�b�g���[�h�j�]����̐擪�|�C���^
	WORD* psrc16 = NULL;			//�i16�r�b�g���[�h�j�]����̐擪�|�C���^
	//���[�v�p
	DWORD i = 0,j = 0;
	//�J���[�L�[
	DDCOLORKEY ddck = {0};
	//�s�N�Z���t�H�[�}�b�g�p	
	DDPIXELFORMAT ddpf = {0};
	
	//�g�k����]�������W�̒l������
	DWORD ssx = 0;
	DWORD ssy = 0;
	
	//�g�k����{�����擾����
	DWORD scale_w = (DWORD)(w * pow);
	DWORD scale_h = (DWORD)(h * pow);

	//�s�N�Z���t�H�[�}�b�g���擾
	ZeroMemory(&ddpf,sizeof(ddpf));
	ddpf.dwSize = sizeof(ddpf);
	g_lpDDSBack->GetPixelFormat(&ddpf);

	//���菇�P�@dst�Asrc�̐ݒ�
	//dst�Ɏ擾����T�[�t�F�C�X�̐ݒ�
	ZeroMemory(&dst,sizeof(dst));
	dst.dwSize = sizeof(dst);
	
	//src�Ɏ擾����T�[�t�F�C�X�̐ݒ�
	ZeroMemory(&src,sizeof(src));
	src.dwSize = sizeof(src);

	//���菇�Q�@dst�Asrc�����Ƃ�2�̃T�[�t�F�C�X�����b�N
	g_lpDDSBack->Lock(NULL,&dst,DDLOCK_WAIT,NULL);
	g_img[id].surface->Lock(NULL,&src,DDLOCK_WAIT,NULL);

	//���菇�R�@���b�N�����Q�̃T�[�t�F�C�X����擪�|�C���^���擾
	pdst32 = (DWORD*)dst.lpSurface;		//�i32�r�b�g�j�]����
	psrc32 = (DWORD*)src.lpSurface;		//�i32�r�b�g�j�]����
	pdst16 = (WORD*)dst.lpSurface;		//�i16�r�b�g�j�]����
	psrc16 = (WORD*)src.lpSurface;		//�i16�r�b�g�j�]����

	//���菇�S�@�\������T�C�Y�����[�v���񂵁A�]�����Ƃ̐F��]����ɑ��
	//32�r�b�g���[�h
	if(ddpf.dwRGBBitCount == 32){
		for(j = 0;j < scale_h;j++){
			for(i = 0;i < scale_w;i++){
				//���炩���ߓ]�����ɔ{�����|�����l�����߂Ă���
				ssx = (DWORD)(i / pow) + src_x;
				ssy = (DWORD)(j / pow) + src_y;

				//�]���悪�͈͊O�Ȃ��΂�
				if((j + dst_y) < 0 ||
					(i + dst_x) < 0 ||
					(j + dst_y) >= dst.dwHeight ||
					(i + dst_x) >= dst.dwWidth){
						continue;
				}
				//�]�������͈͊O�Ȃ珈�����΂�
				if(ssx < 0 || ssy < 0 ||
					ssy >= src.dwHeight ||
					ssx >= src.dwWidth){
						continue;
				}
				//���ߐF�Ȃ������΂�
				if(*(psrc32 + (ssy * (src.lPitch / 4)) + ssx) == DDRGB(0,50,0)){
					continue;
				}
				//�]�����Ƃ̉摜��1�h�b�g���]��
				*(pdst32 + ((dst_y + j) * (dst.lPitch / 4)) + (dst_x + i)) = 
					*(psrc32 + (ssy * (src.lPitch / 4)) + ssx);
			}
		}
	}
	//16�r�b�g���[�h
	if(ddpf.dwRGBBitCount == 16){
		for(j = 0;j < scale_h;j++){
			for(i = 0;i < scale_w;i++){

				//���炩���ߓ]�����ɔ{�����|�����l�����߂Ă���
				ssx = (DWORD)(i / pow) + src_x;
				ssy = (DWORD)(j / pow) + src_y;

				//�]���悪�͈͊O�Ȃ��΂�
				if((j + dst_y) < 0 ||
					(i + dst_x) < 0 ||
					(j + dst_y) >= dst.dwHeight ||
					(i + dst_x) >= dst.dwWidth){
						continue;
				}
				//�]�������͈͊O�Ȃ珈�����΂�
				if(ssx < 0 || ssy < 0 ||
					ssy >= src.dwHeight ||
					ssx >= src.dwWidth){
						continue;
				}
				//���ߐF�Ȃ������΂�
				if(*(psrc16 + (ssy * (src.lPitch / 2)) + ssx) == DDRGB(0,50,0)){
					continue;
				}
				//�]�����Ƃ̉摜��1�h�b�g���]��
				*(pdst16 + ((dst_y + j) * (dst.lPitch / 2)) + (dst_x + i)) = 
					*(psrc16 + (ssy * (src.lPitch / 2)) + ssx);
			}
		}
	}
	//���菇�T�@�Q�̃T�[�t�F�C�X���A�����b�N
	g_img[id].surface->Unlock(NULL);
	g_lpDDSBack->Unlock(NULL);
}

//*****************************************************************************************
//�������̕`��u���b�N�A�A�����b�N�L��v
//*****************************************************************************************
void DDAlphaBltEx(int dst_x,int dst_y,DWORD w,DWORD h,int id,int src_x,int src_y){

	DDSURFACEDESC2 dst = {0};		//�]����̃T�[�t�F�C�X
	DDSURFACEDESC2 src = {0};		//�]�����̃T�[�t�F�C�X

	DWORD* pdst32 = NULL;			//�i32�r�b�g���[�h�j�]����̐擪�|�C���^
	DWORD* psrc32 = NULL;			//�i32�r�b�g���[�h�j�]�����̐擪�|�C���^
	WORD* pdst16 = NULL;			//�i16�r�b�g���[�h�j�]����̐擪�|�C���^
	WORD* psrc16 = NULL;			//�i16�r�b�g���[�h�j�]����̐擪�|�C���^

	DWORD dst_color = 0;			//�]����̐F
	DWORD src_color = 0;			//�]�����Ƃ̐F
	DWORD dst_r = 0;				//�]����̐Ԃ̐���
	DWORD src_r = 0;				//�]�����Ƃ̐Ԃ̐���
	DWORD dst_g = 0;				//�]����̗΂̐���
	DWORD src_g = 0;				//�]�����Ƃ̗΂̐���
	DWORD dst_b = 0;				//�]����̐̐���
	DWORD src_b = 0;				//�]�����Ƃ̐̐���

	//���[�v�p
	DWORD i = 0,j = 0;
	//�J���[�L�[
	DDCOLORKEY ddck = {0};
	//�s�N�Z���t�H�[�}�b�g�p	
	DDPIXELFORMAT ddpf = {0};

	//�s�N�Z���t�H�[�}�b�g���擾
	ZeroMemory( &ddpf , sizeof( ddpf ) );
	ddpf.dwSize = sizeof( ddpf );
	g_lpDDSBack->GetPixelFormat( &ddpf );

	//���菇�P�@dst�Asrc�̐ݒ�
	//dst�Ɏ擾����T�[�t�F�C�X�̐ݒ�
	ZeroMemory( &dst , sizeof( dst ) );
	dst.dwSize = sizeof( dst );
	
	//src�Ɏ擾����T�[�t�F�C�X�̐ݒ�
	ZeroMemory( &src , sizeof( src ) );
	src.dwSize = sizeof( src );

	//���菇�Q�@dst�Asrc�����Ƃ�2�̃T�[�t�F�C�X�����b�N
	g_lpDDSBack->Lock( NULL , &dst , DDLOCK_WAIT , NULL );
	g_img[id].surface->Lock( NULL , &src , DDLOCK_WAIT , NULL );

	//���菇�R�@���b�N�����Q�̃T�[�t�F�C�X����擪�|�C���^���擾
	pdst32 = (DWORD*)dst.lpSurface;		//�i32�r�b�g�j�]����
	psrc32 = (DWORD*)src.lpSurface;		//�i32�r�b�g�j�]����
	pdst16 = (WORD*)dst.lpSurface;		//�i16�r�b�g�j�]����
	psrc16 = (WORD*)src.lpSurface;		//�i16�r�b�g�j�]����

	//���菇�S�@�\������T�C�Y�����[�v���񂵁A�]�����Ƃ̐F��]����ɑ��
	//32�r�b�g���[�h
	if(ddpf.dwRGBBitCount == 32){
		for(j = 0;j < h;j++){
			for(i = 0;i < w;i++){

				//�]���悪�͈͊O�Ȃ��΂�
				if((j + dst_y) < 0 ||
					(i + dst_x) < 0 ||
					(j + dst_y) >= dst.dwHeight ||
					(i + dst_x) >= dst.dwWidth){
						continue;
				}
				//�]�������͈͊O�Ȃ珈�����΂�
				if((j + src_y) < 0 ||
					(i + src_x) < 0 ||
					(j + src_y) >= src.dwHeight ||
					(i + src_x) >= src.dwWidth){
						continue;
				}
				//���ߐF�Ȃ������΂�
				if(*(psrc32 + ((src_y + j) * (src.lPitch / 4)) + (src_x + i)) == DDRGB(0,50,0)){
					continue;
				}

				//�]����̐F���擾
				dst_color = (DWORD)( *( pdst32 ) + ( j + dst_y )
				*( dst.lPitch / 4 ) ) + ( i + dst_x );
				dst_r = dst_color & ddpf.dwRBitMask;
				//�]�����Ƃ̐F���擾
				src_color = (DWORD)( *( psrc32 ) + ( j + src_y )
				*( src.lPitch / 4 ) ) + ( i + src_x );
				src_r = src_color & ddpf.dwRBitMask;

				//���Ԃ̐���
				//���ԐF�̍쐬
				dst_r = (DWORD)( ( dst_r * ( 1.0f - 0.5f ) )
				+ ( src_r * 0.5f ) ) & ddpf.dwRBitMask;

				//���΂̐���
				dst_g = dst_color & ddpf.dwGBitMask;
				src_g = src_color & ddpf.dwGBitMask;
				//���ԐF�̍쐬
				dst_g = (DWORD)( ( dst_g * ( 1.0f - 0.5f ) )
				+ ( src_g * 0.5f ) ) & ddpf.dwGBitMask;

				//���̐���
				dst_b = dst_color & ddpf.dwBBitMask;
				src_b = src_color & ddpf.dwBBitMask;
				//���ԐF�̍쐬
				dst_b = (DWORD)( ( dst_b * ( 1.0f - 0.5f ) )
				+ ( src_b * 0.5f ) ) & ddpf.dwBBitMask;

				//���ԐF��]����ɑ��
				*( pdst32 + ( ( j + dst_y ) * ( dst.lPitch / 4 ) ) + ( i + dst_x ) ) = 
					dst_r | dst_g | dst_b;
			}
		}
	}
	//16�r�b�g���[�h
	if(ddpf.dwRGBBitCount == 16){
		for(j = 0;j < h;j++){
			for(i = 0;i < w;i++){
				//�]���悪�͈͊O�Ȃ��΂�
				if((j + dst_y) < 0 ||
					(i + dst_x) < 0 ||
					(j + dst_y) >= dst.dwHeight ||
					(i + dst_x) >= dst.dwWidth){
						continue;
				}
				//�]�������͈͊O�Ȃ珈�����΂�
				if((j + src_y) < 0 ||
					(i + src_x) < 0 ||
					(j + src_y) >= src.dwHeight ||
					(i + src_x) >= src.dwWidth){
						continue;
				}
				//���ߐF�Ȃ������΂�
				if(*(psrc16 + ((src_y + j) * (src.lPitch / 2)) + (src_x + i)) == DDRGB(0,50,0)){
					continue;
				}
				//�]����̐F���擾
				dst_color = (WORD)(*(pdst16) + (j + dst_y)
				*(dst.lPitch / 2)) + (i + dst_x);
				dst_r = dst_color & ddpf.dwRBitMask;
				//�]�����Ƃ̐F���擾
				src_color = (WORD)(*(psrc16) + (j + src_y)
				*(src.lPitch / 2)) + (i + src_x);
				src_r = src_color & ddpf.dwRBitMask;

				//���Ԃ̐���
				//���ԐF�̍쐬
				dst_r = (WORD)((dst_r * (1.0f - 0.5f))
				+ (src_r * 0.5f)) & ddpf.dwRBitMask;

				//���΂̐���
				dst_g = dst_color & ddpf.dwGBitMask;
				src_g = src_color & ddpf.dwGBitMask;
				//���ԐF�̍쐬
				dst_g = (WORD)((dst_g * (1.0f - 0.5f))
				+ (src_g * 0.5f)) & ddpf.dwGBitMask;

				//���̐���
				dst_b = dst_color & ddpf.dwBBitMask;
				src_b = src_color & ddpf.dwBBitMask;
				//���ԐF�̍쐬
				dst_b = (WORD)((dst_b * (1.0f - 0.5f))
				+ (src_b * 0.5f)) & ddpf.dwBBitMask;

				//���ԐF��]����ɑ��
				*(pdst16 + ((j + dst_y) * (dst.lPitch / 2)) + (i + dst_x)) = 
					(WORD)dst_r | (WORD)dst_g | (WORD)dst_b;
			}
		}
	}
	//���菇�T�@�Q�̃T�[�t�F�C�X���A�����b�N
	g_img[id].surface->Unlock(NULL);
	g_lpDDSBack->Unlock(NULL);
}

//*****************************************************************************************
//�����\���i���b�N�E�A�����b�N�����j
//*****************************************************************************************
void DDAlphaBlt(int dst_x,int dst_y,DWORD w,DWORD h,int id,int src_x,int src_y,float alpha){

	BLENDFUNCTION bf = {0};		//�����̐ݒ�
	HDC dsthDC = NULL;			//�]�����HDC
	HDC srchDC = NULL;			//�]������HDC
	int dummy = 0;				//��̃T�[�t�F�C�X�p
	RECT dst = {0};
	RECT src = {0};
	
	//���菇�P�@��̃T�[�t�F�C�X����
	dummy = CreateDDOffScreen(w , h);

	//AlphaBlend�̐ݒ�
	bf.AlphaFormat = AC_SRC_OVER;
	bf.BlendFlags = NULL;
	bf.BlendOp = NULL;
	bf.SourceConstantAlpha = (int)( 255 * alpha );	//�A���t�@�l�̐ݒ�

	//���菇�Q�@��̃T�[�t�F�C�X�Ƀo�b�N�T�[�t�F�C�X�̕`��
	SetRect( &dst , 0 , 0 , w , h );
	SetRect( &src , dst_x , dst_y , dst_x + w , dst_y + h );
	g_img[dummy].surface->Blt( &dst , g_lpDDSBack , &src , DDBLT_WAIT , NULL );

	//���菇�R�@
	SetRect( &src , src_x , src_y , src_x + w , src_y + h );
	g_img[dummy].surface->Blt(
		&dst , g_img[id].surface ,
		&src , DDBLT_WAIT | DDBLT_KEYSRC , NULL);

	
	//�]����HDC�̎擾�i��̃T�[�t�F�C�X�ɕύX�j
	g_img[dummy].surface->GetDC( &srchDC );
	//�]����HDC�̎擾
	g_lpDDSBack->GetDC( &dsthDC );
	
	//AlphaBlend�̌Ăяo�������̕\��
	AlphaBlend( dsthDC , dst_x , dst_y , w , h , srchDC , 0 , 0 , w , h , bf );
	
	//�]����HDC�̉���i��̃T�[�t�F�C�X�ɕύX�j
	g_img[dummy].surface->ReleaseDC( srchDC );
	//�]�����HDC�����
	g_lpDDSBack->ReleaseDC( dsthDC );

	//���菇�T�@��̃T�[�t�F�C�X�����
	ReleaseDDImage( dummy );

}

//*****************************************************************************************
//��̃T�[�t�F�C�X�����
//*****************************************************************************************
int CreateDDOffScreen(int w,int h){

	DDSURFACEDESC2 ddsd = {0};
	DDCOLORKEY ddck = {0};
	int i = 0;

	//��������T�[�t�F�C�X�̐ݒ�
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = w;			//���E�����͈����̒l
	ddsd.dwHeight = h;

	//���ߐF�̎w��
	ZeroMemory( &ddck , sizeof( ddck ) );
	ddck.dwColorSpaceHighValue = DDRGB( 0 , 50 , 0 );
	ddck.dwColorSpaceLowValue = DDRGB( 0 , 50 , 0 );

	//g_img�̋�̗v�f�ɃT�[�t�F�C�X�𐶐�
	for(i = 0;i < IMAGE_MAX;i++){
		if(g_img[i].surface == NULL){
			//�ݒ�����ƂɃT�[�t�F�C�X�𐶐�
			g_lpDD->CreateSurface(&ddsd,&g_img[i].surface,0);
			//���ߐF�̐ݒ�
			g_img[i].surface->SetColorKey(DDCKEY_SRCBLT,&ddck);
			//��̃T�[�t�F�C�X��h��Ԃ�
			FillImage(i,0,0,0);
			//�����ɐ���
			return i;
		}
	}
	//�����Ɏ��s
	return -1;
}

//*****************************************************************************************
//�w��T�[�t�F�C�X�̓h��Ԃ�
//*****************************************************************************************
void FillImage(int id,DWORD r,DWORD g,DWORD b){

	DDBLTFX ddbltfx = {0};
	
	//�h��Ԃ��̐ݒ�
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = DDRGB(r,g,b);
	//�w��̃T�[�t�F�C�X��h��Ԃ�
	g_img[id].surface->Blt(NULL,NULL,NULL,DDBLT_WAIT | DDBLT_COLORFILL,&ddbltfx);
}

//*****************************************************************************************
//��]���s����`��i���b�N�A�A�����b�N�L��j
//*****************************************************************************************
void DDRotateBltEx(int dst_x ,int dst_y,int w,int h,int id,int src_x,int src_y,int angle){
	
	DDSURFACEDESC2 ddst = {0};		//�]����̃T�[�t�F�C�X
	DDSURFACEDESC2 dsrc = {0};		//�]�����̃T�[�t�F�C�X
	DWORD* pdst32 = NULL;			//�i32�r�b�g���[�h�j�]����̐擪�|�C���^
	DWORD* psrc32 = NULL;			//�i32�r�b�g���[�h�j�]�����̐擪�|�C���^
	WORD* pdst16 = NULL;			//�i16�r�b�g���[�h�j�]����̐擪�|�C���^
	WORD* psrc16 = NULL;			//�i16�r�b�g���[�h�j�]����̐擪�|�C���^
	//���[�v�p
	DWORD i = 0,j = 0;
	//�J���[�L�[
	DDCOLORKEY ddck = {0};
	//�s�N�Z���t�H�[�}�b�g�p	
	DDPIXELFORMAT ddpf = {0};
	
	//��̃T�[�t�F�C�X�p
	int dummy = 0;
	//�傫���ق���1.5�{��
	int size = w > h ? (int)(w * 1.5f) : (int)(h * 1.5f);
	RECT src = {0};
	//��]�̒��S���T�[�t�F�C�X�̒��S��
	int rotate_x = 0;			//��]��̓]����x���W
	int rotate_y = 0;			//��]��̓]��y���W
	int cx = size / 2;			//��]�̒��Sx���W
	int cy = size / 2;			//��]�̒��Sy���W
	
	//��̃T�[�t�F�C�X�𐶐�
	dummy = CreateDDOffScreen(size,size);
	//�]������`�̍쐬
	SetRect(&src , src_x , src_y , src_x + w , src_y + h);
	//��̃T�[�t�F�C�X�̒��S�ɉ�]����摜��\��
	g_img[dummy].surface->BltFast((size / 2 - w / 2) , (size / 2 - h / 2),
	g_img[id].surface , &src , DDBLTFAST_WAIT);

	//�s�N�Z���t�H�[�}�b�g���擾
	ZeroMemory(&ddpf,sizeof(ddpf));
	ddpf.dwSize = sizeof(ddpf);
	g_lpDDSBack->GetPixelFormat(&ddpf);

	//���菇�P�@dst�Asrc�̐ݒ�
	//dst�Ɏ擾����T�[�t�F�C�X�̐ݒ�
	ZeroMemory(&ddst,sizeof(ddst));
	ddst.dwSize = sizeof(ddst);
	
	//src�Ɏ擾����T�[�t�F�C�X�̐ݒ�
	ZeroMemory(&dsrc,sizeof(dsrc));
	dsrc.dwSize = sizeof(dsrc);

	//���菇�Q�@dst�Asrc�����Ƃ�2�̃T�[�t�F�C�X�����b�N
	g_lpDDSBack->Lock(NULL,&ddst,DDLOCK_WAIT,NULL);
	g_img[dummy].surface->Lock(NULL,&dsrc,DDLOCK_WAIT,NULL);

	//���菇�R�@���b�N�����Q�̃T�[�t�F�C�X����擪�|�C���^���擾
	pdst32 = (DWORD*)ddst.lpSurface;		//�i32�r�b�g�j�]����
	psrc32 = (DWORD*)dsrc.lpSurface;		//�i32�r�b�g�j�]����
	pdst16 = (WORD*)ddst.lpSurface;		//�i16�r�b�g�j�]����
	psrc16 = (WORD*)dsrc.lpSurface;		//�i16�r�b�g�j�]����

	//���菇�S�@�\������T�C�Y�����[�v���񂵁A�]�����Ƃ̐F��]����ɑ��
	//32�r�b�g���[�h
	if(ddpf.dwRGBBitCount == 32){
		for(int j = 0;j < size;j++){
			for(int i = 0;i < size;i++){
				//�]���悪�͈͊O�Ȃ��΂�
				if((j + dst_y) < 0 ||
					(i + dst_x) < 0 ||
					(j + dst_y) >= (int)ddst.dwHeight ||
					(i + dst_x) >= (int)ddst.dwWidth){
						continue;
				}
				//�]�������͈͊O�Ȃ珈�����΂�
				if((j + src_y) < 0 ||
					(i + src_x) < 0 ||
					(j + src_y) >= (int)dsrc.dwHeight ||
					(i + src_x) >= (int)dsrc.dwWidth){
						continue;
				}
				//�]������W�����]��̓]�������Z�o
				rotate_x = (int)( ( i - cx ) * cos( angle * 3.14 / 180.0 )
								- ( j - cy ) * sin( angle * 3.14 / 180.0 ) + cx );
				rotate_y = (int)( ( i - cx ) * sin( angle * 3.14 / 180.0 ) 
								+ ( j - cy ) * cos( angle * 3.14 / 180.0 ) + cy );
				//�]�������͈͊O�Ȃ��΂�
				if( rotate_y < 0 || rotate_y >= ( int )dsrc.dwHeight ||
					rotate_x < 0 || rotate_x >= ( int )dsrc.dwWidth ){
					continue;
				}

				//�]���������ߐF�Ȃ��΂�
				if( *( psrc32+(rotate_y)*(dsrc.lPitch/4)+(rotate_x) ) == DDRGB(0,50,0)){
					continue;
				}	
				//�]����ɐF����				
				*(pdst32+(j + (int)dst_y - (size / 2 - h / 2)) * (ddst.lPitch/4) + 
					(i + (int)dst_x - (size / 2 - w / 2))) = 
							*( psrc32 + (rotate_y) * (dsrc.lPitch / 4) + (rotate_x));
			}
		}
	}
	//16�r�b�g���[�h
	if(ddpf.dwRGBBitCount == 16){
		for(j = 0;j < (WORD)size;j++){
			for(i = 0;i < (WORD)size;i++){
				
				//�]���悪�͈͊O�Ȃ��΂�
				if((j + dst_y) < 0 ||
					(i + dst_x) < 0 ||
					(j + dst_y) >= ddst.dwHeight ||
					(i + dst_x) >= ddst.dwWidth){
						continue;
				}
				//�]�������͈͊O�Ȃ珈�����΂�
				if((j + src_y) < 0 ||
					(i + src_x) < 0 ||
					(j + src_y) >= dsrc.dwHeight ||
					(i + src_x) >= dsrc.dwWidth){
						continue;
				}
				//�]������W�����]��̓]�������Z�o
				rotate_x = (int)( ( i - cx ) * cos( angle * 3.14 / 180.0 )
								- ( j - cy ) * sin( angle * 3.14 / 180.0 ) + cx );
				rotate_y = (int)( ( i - cx ) * sin( angle * 3.14 / 180.0 ) 
								+ ( j - cy ) * cos( angle * 3.14 / 180.0 ) + cy );
				//�]�������͈͊O�Ȃ��΂�
				if( rotate_y < 0 || rotate_y >= ( int )dsrc.dwHeight ||
					rotate_x < 0 || rotate_x >= ( int )dsrc.dwWidth ){
					continue;
				}
				//���ߐF�Ȃ������΂�
				if(*(psrc16 + ((src_y + j) * (dsrc.lPitch / 2)) + (src_x + i)) == DDRGB(0,50,0)){
					continue;
				}
				//�]����ɐF����				
				*(pdst16+(j + (int)dst_y - (size / 2 - h / 2)) * (ddst.lPitch / 2) + 
					(i + (int)dst_x - (size / 2 - w / 2))) = 
							*( psrc16 + (rotate_y) * (dsrc.lPitch / 2) + (rotate_x));
			}
		}
	}
	//���菇�T�@�Q�̃T�[�t�F�C�X���A�����b�N
	g_img[dummy].surface->Unlock(NULL);
	g_lpDDSBack->Unlock(NULL);

	//��̃T�[�t�F�C�X�̊J��
	ReleaseDDImage(dummy);
}

//*****************************************************************************************
//�F�Ƃ̔�������֐��i���b�N�A�A�����b�N�L��j
//*****************************************************************************************
bool ColisionDDRGB(CHARACTER* ch,int id,DWORD dst_color,DWORD src_color){

	DDSURFACEDESC2 ddst = {0};		//�]����̃T�[�t�F�C�X
	DDSURFACEDESC2 dsrc = {0};		//�]�����̃T�[�t�F�C�X
	DWORD* pdst32 = NULL;			//�i32�r�b�g���[�h�j�]����̐擪�|�C���^
	DWORD* psrc32 = NULL;			//�i32�r�b�g���[�h�j�]�����̐擪�|�C���^
	WORD* pdst16 = NULL;			//�i16�r�b�g���[�h�j�]����̐擪�|�C���^
	WORD* psrc16 = NULL;			//�i16�r�b�g���[�h�j�]����̐擪�|�C���^
	//���[�v�p
	int i = 0,j = 0;
	//�J���[�L�[
	DDCOLORKEY ddck = {0};
	//�s�N�Z���t�H�[�}�b�g�p	
	DDPIXELFORMAT ddpf = {0};
	//�߂�l�p
	bool ret = false;
	//�]����̍��W�v�Z�p
	int dx = 0,dy = 0;
	//�]�����̍��W�v�Z�p
	int sx = 0,sy = 0;
		
	//�s�N�Z���t�H�[�}�b�g���擾
	ZeroMemory(&ddpf,sizeof(ddpf));
	ddpf.dwSize = sizeof(ddpf);
	g_lpDDSBack->GetPixelFormat(&ddpf);

	//���菇�P�@dst�Asrc�̐ݒ�
	//dst�Ɏ擾����T�[�t�F�C�X�̐ݒ�
	ZeroMemory(&ddst,sizeof(ddst));
	ddst.dwSize = sizeof(ddst);
	
	//src�Ɏ擾����T�[�t�F�C�X�̐ݒ�
	ZeroMemory(&dsrc,sizeof(dsrc));
	dsrc.dwSize = sizeof(dsrc);

	//���菇�Q�@dst�Asrc�����Ƃ�2�̃T�[�t�F�C�X�����b�N
	g_lpDDSBack->Lock(NULL,&ddst,DDLOCK_WAIT,NULL);
	g_img[id].surface->Lock(NULL,&dsrc,DDLOCK_WAIT,NULL);

	//���菇�R�@���b�N�����Q�̃T�[�t�F�C�X����擪�|�C���^���擾
	pdst32 = (DWORD*)ddst.lpSurface;		//�i32�r�b�g�j�]����
	psrc32 = (DWORD*)dsrc.lpSurface;		//�i32�r�b�g�j�]����
	pdst16 = (WORD*)ddst.lpSurface;			//�i16�r�b�g�j�]����
	psrc16 = (WORD*)dsrc.lpSurface;			//�i16�r�b�g�j�]����

	//���菇�S�@�\������T�C�Y�����[�v���񂵁A�]�����Ƃ̐F��]����ɑ��
	//32�r�b�g���[�h
	if(ddpf.dwRGBBitCount == 32){
		for(j = 0;j < ch->h;j++){
			for(i = 0;i < ch->w;i++){
				//���[�v���Ŏg�p������W���v�Z
				dx = (int)(i + ch->x); 
				dy = (int)(j + ch->y); 
				sx = (int)(i + ch->picx);
				sy = (int)(j + ch->picy); 
				//�]���悪�͈͊O�Ȃ��΂�
				if(dy < 0 || dx < 0 ||
					dy >= (int)ddst.dwHeight || dx >= (int)ddst.dwWidth){
						continue;
				}
				//�]�������͈͊O�Ȃ珈�����΂�
				if(sy < 0 || sx < 0 ||
					sy >= (int)dsrc.dwHeight || sx >= (int)dsrc.dwWidth){
						continue;
				}
				//���ߐF�Ȃ������΂�
				if(*(psrc32 + (sy * (dsrc.lPitch / 4)) + sx) == DDRGB(0,50,0)){
					continue;
				}
				//���茳�̃L�����N�^�[�ƈ����Ŏw�肵���F�Ƃ̔���
				if(*(psrc32 + (sy * (dsrc.lPitch / 4)) + sx) == src_color &&
					*(pdst32 + (dy * (ddst.lPitch / 4)) + dx) == dst_color){
					//����������true��Ԃ�
					ret = true;
					break;	//�����蔻�肪���������烋�[�v�I��
				}
			}
		}
	}	
	//���菇�T�@�Q�̃T�[�t�F�C�X���A�����b�N
	g_img[id].surface->Unlock(NULL);
	g_lpDDSBack->Unlock(NULL);
	//�������Ă��Ȃ�������false��Ԃ�
	return ret;
}

//***********************************************************************************************
//���̕`��
//***********************************************************************************************
void DDLine(){
		HDC hDC = NULL;
		//�o�b�N�T�[�t�F�C�X��HDC���擾
		g_lpDDSBack->GetDC( &hDC );
		//�n�_�ݒ�
		MoveToEx( hDC , 0 , 0 , NULL );
		//�n�_����I�_�܂ł���łȂ�
		LineTo( hDC , 320 , 240 );
		//�o�b�N�T�[�t�F�C�X��HDC���J��
		g_lpDDSBack->ReleaseDC( hDC );
}

//***********************************************************************************************
//���p�`�̕`��
//***********************************************************************************************
void DDPolygon(POINT *p,int size){

	HDC hDC = NULL;
	//�o�b�N�T�[�t�F�C�X�̂g�c�b���擾
	g_lpDDSBack->GetDC(&hDC);
	//�����̒��_�ő��p�`�̕`��
	Polygon(hDC , p , size);
	//�o�b�N�T�[�t�F�C�X�̂g�c�b�����
	g_lpDDSBack->ReleaseDC(hDC);
}

//***********************************************************************************************
//�~�̕`��
//***********************************************************************************************
void DDCircle(float cx,float cy,int len){

	HDC hDC = NULL;
	//�o�b�N�T�[�t�F�C�X�̂g�c�b���擾
	g_lpDDSBack->GetDC(&hDC);
	//�~��\��
	Ellipse(hDC ,(int)cx - (int)len ,(int)cy - (int)len ,(int)cx + (int)len ,(int)cy + (int)len);
	//�o�b�N�T�[�t�F�C�X�̂g�c�b�����
	g_lpDDSBack->ReleaseDC(hDC);
}
