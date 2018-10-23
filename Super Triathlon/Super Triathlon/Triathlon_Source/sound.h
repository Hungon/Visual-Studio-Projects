#ifndef __SOUND_H__
#define __SOUND_H__

#include <dsound.h>

//�}�N��
//�ő�Ȑ�
#define SOUND_MAX	30
//1�b���̃o�C�g��
#define SECOND	176400

//�X�g���[�~���O�\����
typedef struct tagSTREAMING{
	LPDIRECTSOUNDBUFFER buf;	//2�b���̃o�b�t�@
	char pass[256];				//�t�@�C���p�X
	long startOffset;			//���f�[�^�܂ł̃o�C�g��
	long readOffset;			//���̓ǂݍ��݈ʒu
	DWORD oneSecondBytes;		//1�b���̃T�C�Y
	DWORD musicSize;			//1�ȕ��̃T�C�Y
	DWORD readSize;				//����ǂݍ��񂾃T�C�Y
	bool fFront;				//�O���A�㔼�̓ǂݍ��ݏꏊ
	bool fPlay;					//�Đ������ǂ���
	bool fRead;					//�ǂݍ��݂��K�v���ǂ���
}STREAMING;

//�v���g�^�C�v�錾
//������
bool InitDirectSound(HWND hWnd);
//���
void ReleaseDirectSound();
//�w��Ȃ̍Đ�
void PlaySound(int id,int bigintime,bool loop);
//�w��Ȃ̒�~
void StopSound(int id);
//�w��Ȃ̉��
void ReleaseSound(int id);
//�Ȃ̓ǂݍ���
int CreateSound(char* pass);
//�Z�J���_���o�b�t�@�̐���
bool CreateStreaming(char* pass);
//�X�g���[�~���O�Đ����s��
void PlayStreaming(bool play);
//�X�g���[�~���O�̉��
void ReleaseStreaming();

#endif