#ifndef		FILEVALUE_H__
#define		FILEVALUE_H__



//���@�w�b�_�[�̃C���N���[�h
#include	<windows.h>


//�t�@�C���̍��ڂ������uFILEVALUE�\���́v
typedef struct tagFILEVALUE {

	int fileBox[30][256];		//�L�^�𐔒l�Ƃ��Ĉ���
	
}FILEVALUE , *LPFILEVALUE;

/*
load value from selected file
*/
FILEVALUE* LoadFile(char* lpPass);
/*
save value into selected file
*/
void	SaveFile(char* lpPass, int kind, int* lpItem , FILEVALUE* lpRecord);

#endif