#include    "File.h"
#include	<stdio.h>
#include	<string.h>


//**************************************************************************************************
//�t�@�C������l��ǂݍ���
//**************************************************************************************************
FILEVALUE*	 LoadFile(char* lpPass) {

	//�t�@�C���|�C���^
	FILE* fp = NULL;
	//�t�@�C������̕�������󂯎��
	char word[256] = { 0 };
	//��؂蕶���ƃX�y�[�X�𒲂ׂ�p
	int seek = 0;
	//�������J�E���g�p
	int wordCnt = 0;
	//���ڃJ�E���g�p
	int itemCnt = 0;
	//���̍��ڂ̗v�f��
	int itemNext = 0;
	//�����p
	int digit = 0;
	FILEVALUE	fileValue;			//for response

	//�����̃t�@�C�����J��
	fp = fopen(lpPass, "rb");
	//�t�@�C�����J�������m�F
	if (fp == NULL) {
		//�G���[���e��\��
		MessageBox(NULL, "�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", "LoadFile", MB_OK);
		//�Ăяo�����ɕԂ�
		return 0;
	}
	//�t�@�C���̖��[�܂œǂݍ���
	while (feof(fp) == 0) {
		//�������ǂݍ���
		fread(word + wordCnt, sizeof(char), 1, fp);
		//�󂯎�����������int�^�Ŏ󂯎��
		seek = (int)word[wordCnt];
		//���s������Ύ��̍��ڂ�
		if (word[wordCnt] == 10 &&
			word[wordCnt - 1] == 13 ||
			word[wordCnt] == '/') {
			//�������̃��Z�b�g
			wordCnt = 0;
			//������̏�����
			ZeroMemory(word, sizeof(char) * 256);
			//���ڐ������Z�b�g
			itemCnt = 0;
			//���������Z�b�g
			digit = 0;
			//���̍��ڂ�
			itemNext++;
			continue;
		}
		//�X�y�[�X������Βl���擾����
		if (isspace(seek) != 0 || ispunct(seek)) {
			//�R�s�[�����������10�i���ɕϊ�����
			fileValue.fileBox[itemNext][itemCnt] = atoi(word + wordCnt - digit);
			//���������Z�b�g
			digit = 0;
			//���̍��ڂ�
			itemCnt++;
		}
		//�������J�E���g
		digit++;
		//���������J�E���g�A�b�v
		wordCnt++;
	}
	//�t�@�C�������
	fclose(fp);
	//�擾�����l��Ԃ�
	return	&fileValue;
}


/*
************************************************************************************************************
�t�@�C���֋L�^�p
************************************************************************************************************
*/
void	SaveRecord(char* lpPass, int kind, int* lpItem, FILEVALUE* lpRecord) {

	FILE	*lpFile = NULL;

	//���@����̋L�^���ō��̋L�^�֏�������
	//�t�@�C�����J��
	lpFile = fopen(lpPass, "wb");
	//�t�@�C�����J�������m�F
	if (lpFile == NULL) {
		//�G���[���e��\��
		MessageBox(NULL, "�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", "SaveRecordEndTheResult", MB_OK);
		//�Ăяo�����ɕԂ�
		return;
	}
	//�����̍��ڂ̎�ނ����������ޏ���
	for (int i = 0; i < kind; i++) {
		for (int j = 0; j < lpItem[i]; j++) {
			//�e���ڂ𖞂������玟�̋L�^��
			if (j == (lpItem[i] - 1)) {
				//�e���ڂ��������񂾂���s���Ď��̗v�f��
				fprintf(lpFile, "%d /", lpRecord->fileBox[i][j]);
				break;
			}
			//�e���ڂ𔼊p�X�y�[�X���󂯂ď�������
			fprintf(lpFile, "%d ", lpRecord->fileBox[i][j]);
		}
	}
	//�t�@�C�������
	fclose(lpFile);
}