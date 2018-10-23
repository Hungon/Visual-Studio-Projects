//
#pragma warning ( disable : 4996 )

#include "talk.h"
#include "record.h"
#include "input.h"
#include <stdio.h>
#include <string.h>

//���O���[�o���ϐ�
//�e�L�X�g�t�@�C���ǂݎ��p
TALK g_talk[TALK_LOAD_MAX] = {0};
//�ǂݍ��ރt�@�C���̗v�f�ԍ�
int g_file_cnt = 0;
//�摜�������ϐ�

//************************************************************************************************
//������
//************************************************************************************************
void InitTalk(){
	
	//���͊Ǘ��\���̂̏�����
	ZeroMemory(&g_talk[0] , sizeof(TALK) * TALK_LOAD_MAX);
	//�t�@�C���̗v�f��
	g_file_cnt = 0;
}

//************************************************************************************************
//�X�V
//************************************************************************************************
bool UpdateTalk() {

	//���͖������Ȃ�I��
	if (g_talk[g_file_cnt].fShow == false) {
		return g_talk[g_file_cnt].fShow;		//false��Ԃ�
	}

	//�\�������t�@�C�����܂Ń��[�v���ď���
	for (int i = 0; i < (g_file_cnt + 1); i++) {

		//�I���t���O�������Ă�����S�ĕ\���I��
		if (g_talk[i].fDelete == true) {

			//�\���t���O��false
			g_talk[i].fShow = false;
			//�����������Z�b�g
			g_talk[i].word = 0;
			//����܂ŕ\���������͂����Z�b�g	
			ZeroMemory(&g_talk[i].mes[0], sizeof(char) * 256);
		}

	}

	//�������������I����Ă�����ǂݍ���
	if (g_talk[g_file_cnt].fRead == false) {

		//�����������Z�b�g
		g_talk[g_file_cnt].word = 0;

		//����܂ŕ\���������͂����Z�b�g	
		ZeroMemory(&g_talk[g_file_cnt].mes[0], sizeof(char) * 256);


		//���̕��͂�ǂݍ���
		g_talk[g_file_cnt].readoffset = LoadTalk(&g_talk[g_file_cnt].pass[0],	  //�ǂݍ��ރt�@�C�� 
			&g_talk[g_file_cnt].mes[0],	  //������
			g_talk[g_file_cnt].readoffset);  //�ǂݍ���ł���o�C�g��

	}

	//�ǂݍ��ރt�@�C����������
	for (int i = 0; i < (g_file_cnt + 1); i++) {

		//�ǂݍ��݃t���O��ture�Ȃ當�����𑝉�
		if (g_talk[i].fRead) {

			//�o�ߎ��Ԃ̌v��	
			g_talk[i].cnt++;

			//�o�ߎ��Ԃ��\���Ԋu�̔{�����m�F
			if (g_talk[i].cnt % g_talk[i].next == 0) {

				g_talk[i].word++;	//�\�����������Z
				g_talk[i].cnt = 0;	//�o�ߎ��ԃ��Z�b�g

				//�����������𐧌�
				if (g_talk[i].word >= g_talk[i].restrictive_word) {
					//�S�p�T�C�Y��+�k�������̕����������������𐧌�
					g_talk[i].word = g_talk[i].restrictive_word;
				}
			}
		}
	}
	//����̃t���[���ł̏������I��
	return g_talk[g_file_cnt].fShow;
}

//************************************************************************************************
//�\��
//************************************************************************************************
void DrawTalk() {

	POINT pos = { 0 };		//�����̍��W
	char mes[3] = { 0 };	//1�����쐬�p

	//�ǂݍ��񂾃t�@�C�����������[�v���񂵂ĕ\������
	for (int j = 0; j < (g_file_cnt + 1); j++) {

		//���͑��肪�J�n������g��\��
		if (g_talk[j].fShow) {

			//���݂̕������Ԃ񃋁[�v������
			for (int i = 0; i < g_talk[j].word; i++) {

				//���ꕶ���u@r�v������Ε����F��ԐF�ɂ���
				if (g_talk[j].mes[i * 2] == '@' &&
					g_talk[j].mes[i * 2 + 1] == 'r') {

					//�����F��ԐF�ɂ���
					g_talk[j].change_color = RGB(255, 0, 0);
					//���̕�����
					continue;
				}

				//���ꕶ���u@g�v������Ε����F��ΐF�ɂ���
				if (g_talk[j].mes[i * 2] == '@' &&
					g_talk[j].mes[i * 2 + 1] == 'g') {

					//�����F��ΐF�ɂ���
					g_talk[j].change_color = RGB(0, 255, 0);
					//���̕�����
					continue;
				}

				//���ꕶ���u@b�v������Ε����F��F�ɂ���
				if (g_talk[j].mes[i * 2] == '@' &&
					g_talk[j].mes[i * 2 + 1] == 'b') {

					//�����F��F�ɂ���
					g_talk[j].change_color = RGB(0, 0, 255);
					//���̕�����
					continue;
				}

				//���ꕶ���u@y�v������Ε����F�����F�ɂ���
				if (g_talk[j].mes[i * 2] == '@' &&
					g_talk[j].mes[i * 2 + 1] == 'y') {

					//�����F�����F�ɂ���
					g_talk[j].change_color = RGB(255, 255, 0);
					//���̕�����
					continue;
				}

				//���ꕶ���u#t�v������΃^�u1���󂯂ĕ\��
				if (g_talk[j].mes[i * 2] == '#' &&
					g_talk[j].mes[i * 2 + 1] == 't') {

					//���ɕ\�����镶���̂w���W���E��1���������炷
					pos.x += g_talk[j].size;
					//���̕�����
					continue;
				}

				//���ꕶ���u#e�v������Ε����������I��
				if (g_talk[j].mes[i * 2] == '#' &&
					g_talk[j].mes[i * 2 + 1] == 'e') {

					//�ǂݍ��݃t���O��false
					g_talk[j].fRead = false;
					//���̕�����
					continue;
				}

				//���ꕶ���u��n�v������Ή��s
				if (g_talk[j].mes[i * 2] == '#' &&
					g_talk[j].mes[i * 2 + 1] == 'n') {

					//X���W�����Z�b�g
					pos.x = 0;

					//���s����Y���W�����Z
					pos.y += g_talk[j].size + g_talk[g_file_cnt].line_space;

					//���s���ꂽ�當���F�����Z�b�g
					g_talk[j].change_color = g_talk[j].default_color;
					//���̕�����
					continue;
				}

				//�ǂݍ��񂾃f�[�^�̕��тɁu#d�v���Ȃ����`�F�b�N
				if (g_talk[j].mes[i * 2] == '#' &&
					g_talk[j].mes[i * 2 + 1] == 'd') {

					//�\���I���t���O��true��
					g_talk[j].fDelete = true;
					//���̕�����
					continue;
				}

				//�\������1�������쐬���A�\��
				//�\������1�������쐬
				mes[0] = g_talk[j].mes[i * 2];
				mes[1] = g_talk[j].mes[(i * 2) + 1];
				//�k��������
				mes[2] = '\0';

				//�����́u�����̃t�H���g�v�֕ϊ�����
				ChangeFont(FONT_MIN);

				//�����̕\��
				DDTextOutEx(mes, (int)g_talk[j].baseX + pos.x,	//�\��X���W 
					(int)g_talk[j].baseY + pos.y,			//�\��Y���W
					g_talk[j].size,						//�����̃T�C�Y
					g_talk[j].change_color,				//�����̐F
					true);									//�C�^���b�N�̃t���O

				//�������m�̊Ԋu���J����
				pos.x += g_talk[j].size;

			}
		}

		//���̃t�@�C���\���ɍ��W�����Z�b�g����
		pos.x = 0;							//X���W
		pos.y = 0;							//Y���W
	}
}

//************************************************************************************************
//���
//************************************************************************************************
void ReleaseTalk(){
	//�ő�ǂݍ��݃t�@�C�����������͂����Z�b�g
	//�O�ŏ�����
	ZeroMemory(&g_talk[0], sizeof(TALK) * TALK_LOAD_MAX);
	//�t�@�C���̗v�f�������Z�b�g
	g_file_cnt = 0;
}


//************************************************************************************************
//�ǂݍ���
//************************************************************************************************
int LoadTalk(char *pass, char *mes, int offset) {

	//�t�@�C���ǂݍ��ݗp
	FILE *fp = NULL;

	//�t�@�C�����o�C�i���`���ŊJ��
	fp = fopen(pass, "rb");

	//�G���[�`�F�b�N
	if (fp == NULL) {
		MessageBox(NULL, "�t�@�C���̓ǂݍ��݂Ɏ��s", "TALK", MB_OK);
		return -1;
	}

	//�t�@�C���|�C���^���ړ�
	fseek(fp, offset, SEEK_SET);

	//256�̗v�f��1�o�C�g���ǂݍ���
	for (int i = 0; i < 256; i++) {

		//���̖͂��[�Ȃ甲����
		if (feof(fp) != 0) {

			//�ǂݍ��񂾃t�@�C���̖��[���炻�̃e�L�X�g�̕��������擾
			g_talk[g_file_cnt].restrictive_word = i;

			//���̃t�@�C����ǂݍ��߂�t���O��true
			g_talk[g_file_cnt].fNext = true;
			break;
		}

		//mess�̗v�fi�Ԃɓǂݍ���
		fread(mes + i, sizeof(char), 1, fp);

		//�ǂݍ��񂾃T�C�Y�����Z
		g_talk[g_file_cnt].readoffset++;

		//�ǂݍ��񂾃f�[�^�̕��тɁu#e�v���Ȃ����`�F�b�N
		if (mes[i] == 'e' &&
			mes[i - 1] == '#') {

			//�t�@�C������ēǂݍ��ݏI��
			fclose(fp);
			//�t�@�C���̉��o�C�g�ڂ܂œǂݍ��񂾂���Ԃ�
			return offset + g_talk[g_file_cnt].readoffset;
		}

		//�ǂݍ��񂾃f�[�^�̕��тɉ��s���Ȃ����`�F�b�N
		if (mes[i] == 10 &&
			mes[i - 1] == 13) {

			//���s�̎��̕�����ǂݍ���
			mes[i] = 0;
			mes[i - 1] = 0;
			//���s���߂�
			i -= 2;
		}
	}

	//�t�@�C�������
	fclose(fp);
	//�ǂݍ��񂾃o�C�g����Ԃ�
	return offset;
}


//************************************************************************************************
//�t�@�C����ǂݍ���ŕ��͂𐶐�
//************************************************************************************************
void CreateTalk(TALK* ptalk) {

	//�V�����t�@�C���̓ǂݍ��݃t���O�������Ă����玟�ɓǂݍ��ރt�@�C���̗v�f�������Z
	if (g_talk[g_file_cnt].fNext) {

		//�t�@�C�������J�E���g�A�b�v
		g_file_cnt++;
		//�ő�ǂݍ��݃t�@�C�����܂ŉ��Z����
		g_file_cnt %= TALK_LOAD_MAX + 1;
	}

	//���͐����ς݂Ȃ�I��
	if (g_talk[g_file_cnt].fShow) {
		return;
	}

	//���@���͓ǂݍ��݂̐ݒ�

	//���ݕ\�����Ă��镶����
	g_talk[g_file_cnt].word = 0;

	//1��������\�����Ă���̌o�ߎ���
	g_talk[g_file_cnt].cnt = 0;

	//����1��������\������Ԋu
	g_talk[g_file_cnt].next = ptalk->next;

	//1�����̃T�C�Y
	g_talk[g_file_cnt].size = ptalk->size;

	//�s�̊Ԋu
	g_talk[g_file_cnt].line_space = ptalk->line_space;

	//�ύX�����������F
	g_talk[g_file_cnt].change_color = RGB(255, 255, 255);
	//�����̕����F
	g_talk[g_file_cnt].default_color = RGB(255, 255, 255);

	//�\�����W
	g_talk[g_file_cnt].baseX = ptalk->baseX;		//X���W
	g_talk[g_file_cnt].baseY = ptalk->baseY;		//Y���W

	//�������𑝂₷
	g_talk[g_file_cnt].fRead = true;

	//�ǂݍ��݈ʒu���Z�b�g
	g_talk[g_file_cnt].readoffset = 0;

	//�ǂݍ��݃t�@�C���p�X�����Z�b�g
	strcpy(&g_talk[g_file_cnt].pass[0], ptalk->pass);

	//�t�@�C�����玟�̕��͂�ǂݍ���
	g_talk[g_file_cnt].readoffset = LoadTalk(&g_talk[g_file_cnt].pass[0],	  //�t�@�C���p�X
		&g_talk[g_file_cnt].mes[0],											  //������
		g_talk[g_file_cnt].readoffset);										  //�ǂݍ��񂾃o�C�g��

	//���͐����I��
	g_talk[g_file_cnt].fShow = true;

}

//***********************************************************************************************
//���͏����擾����
//***********************************************************************************************
TALK* GetTalk(){ return g_talk; }

