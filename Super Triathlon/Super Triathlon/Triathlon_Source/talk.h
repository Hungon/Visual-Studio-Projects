#ifndef __TALK_H__
#define __TALK_H__

//���}�N���̒�`
#define TALK_LOAD_MAX		10		//�ǂݍ��߂�ő�t�@�C����

//���w�b�_�[���C���N���[�h
#include "window.h"
#include "graphics.h"

//���\���̂̒�`
typedef struct tagTALK{

	char mes[256];			//�\�������镶����
	char pass[256];			//�ǂݍ��݃t�@�C��
	int word;				//���ݕ\������������
	int restrictive_word;	//�\����������
	int size;				//�����T�C�Y
	int line_space;			//�s�Ԃ̊Ԋu
	int cnt;				//�o�ߎ���
	int next;				//�\���Ԋu
	int readoffset;			//�ǂݍ��݈ʒu
	float baseX;			//�����\�����W�w
	float baseY;			//�����\�����W�x
	bool fRead;				//�ǂݍ��݃t���O
	bool fShow;				//�\���t���O
	bool fDelete;			//�\���I���t���O
	bool fNext;				//�V�����t�@�C����ǂݍ��ރt���O
	COLORREF change_color;	//�F�ύX�p
	COLORREF default_color;	//�F�̏����ݒ�

}TALK,*LPTALK;

//���v���g�^�C�v�錾

//������
void InitTalk();

//�X�V
bool UpdateTalk();

//�\��
void DrawTalk();

//���
void ReleaseTalk();

//�e�L�X�g�ǂݍ���
int LoadTalk( char *pass , char *mes , int offset );

//�t�@�C������ǂݍ���ŕ��͂𐶐�
void CreateTalk(TALK* ptalk );

//���͂̏����擾
TALK* GetTalk();

#endif