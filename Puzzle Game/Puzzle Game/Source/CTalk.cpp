#include	"CTalk.h"
#include	<stdio.h>
#include	"input.h"

CTalk::~CTalk() {}

/*
set each default value
*/
void	CTalk::InitializeTalk(
	int fontSize, int drawingSpeed,
	int lineSpace, COLORREF	color) 
{
	this->m_next = drawingSpeed;			//spatial time to draw
	this->m_size = fontSize;				//font's size
	this->m_lineSpace = lineSpace;			//spatial line
	this->m_defaultColor = color;			//default color to draw

}

//************************************************************************************************
//�X�V
//************************************************************************************************
bool	CTalk::UpdateTalk() {

	//���͖������Ȃ�I��
	if (this->m_fShow == false) return false;

	//�I���t���O�������Ă�����S�ĕ\���I��
	if (this->m_fDelete) {
		//�\���t���O��false
		this->m_fShow = false;
		//�����������Z�b�g
		this->m_word = 0;
		//����܂ŕ\���������͂����Z�b�g	
		ZeroMemory(&this->m_mes[0], sizeof(char) * 256);
	}
	//�������������I����Ă�����ǂݍ���
	if (this->m_fRead == false && JustKey(VK_RETURN)) {
		//�����������Z�b�g
		this->m_word = 0;
		//����܂ŕ\���������͂����Z�b�g	
		ZeroMemory(&this->m_mes[0], sizeof(char) * 256);
		this->m_fRead = true;
		//���̕��͂�ǂݍ���
		this->m_readoffset = LoadTalk(
			&this->m_pass[0],	  //�ǂݍ��ރt�@�C�� 
			&this->m_mes[0],	  //������
			this->m_readoffset);  //�ǂݍ���ł���o�C�g��
	}

	//�ǂݍ��݃t���O��ture�Ȃ當�����𑝉�
	if (this->m_fRead) {
		//�o�ߎ��Ԃ̌v��	
		this->m_cnt++;
		//�o�ߎ��Ԃ��\���Ԋu�̔{�����m�F
		if (this->m_cnt % this->m_next == 0) {
			this->m_word++;			//�\�����������Z
			this->m_cnt = 0;		//�o�ߎ��ԃ��Z�b�g
		}
	}
	//����̃t���[���ł̏������I��
	return this->m_fShow;
}

//************************************************************************************************
//�\��
//************************************************************************************************
void	CTalk::DrawTalk() {

	POINT pos = { 0 };		//�����̍��W
	char mes[3] = { 0 };	//1�����쐬�p

	//���͑��肪�J�n������g��\��
	if (this->m_fShow) {
		//���݂̕������Ԃ񃋁[�v������
		for (int i = 0; i < this->m_word; i++) {
			//���ꕶ���u@r�v������Ε����F��ԐF�ɂ���
			if (this->m_mes[i * 2] == '@' &&
				this->m_mes[i * 2 + 1] == 'r') {
				this->m_changeColor = RGB(255, 0, 0);
				continue;
			}
			//���ꕶ���u@g�v������Ε����F��ΐF�ɂ���
			if (this->m_mes[i * 2] == '@' &&
				this->m_mes[i * 2 + 1] == 'g') {
				this->m_changeColor = RGB(0, 255, 0);
				continue;
			}
			//���ꕶ���u@b�v������Ε����F��F�ɂ���
			if (this->m_mes[i * 2] == '@' &&
				this->m_mes[i * 2 + 1] == 'b') {
				this->m_changeColor = RGB(0, 0, 255);
				continue;
			}
			//���ꕶ���u@y�v������Ε����F�����F�ɂ���
			if (this->m_mes[i * 2] == '@' &&
				this->m_mes[i * 2 + 1] == 'y') {
				this->m_changeColor = RGB(255, 255, 0);
				continue;
			}
			//���ꕶ���u#t�v������΃^�u1���󂯂ĕ\��
			if (this->m_mes[i * 2] == '#' &&
				this->m_mes[i * 2 + 1] == 't') {
				pos.x += this->m_size;
				continue;
			}
			//���ꕶ���u#e�v������Ε����������I��
			if (this->m_mes[i * 2] == '#' &&
				this->m_mes[i * 2 + 1] == 'e') {
				this->m_fRead = false;
				break;
			}
			//���ꕶ���u��n�v������Ή��s
			if (this->m_mes[i * 2] == '#' &&
				this->m_mes[i * 2 + 1] == 'n') {
				pos.x = 0;
				pos.y += this->m_size + this->m_lineSpace;
				this->m_changeColor = this->m_defaultColor;
				continue;
			}
			//�ǂݍ��񂾃f�[�^�̕��тɁu#d�v���Ȃ����`�F�b�N
			if (this->m_mes[i * 2] == '#' &&
				this->m_mes[i * 2 + 1] == 'd') {
				this->m_fDelete = true;
				break;
			}
			//�\������1�������쐬���A�\��
			//�\������1�������쐬
			mes[0] = this->m_mes[i * 2];
			mes[1] = this->m_mes[(i * 2) + 1];
			//�k��������
			mes[2] = '\0';

			//�����̕\��
			DDTextOutEx(mes, (int)this->m_baseX + pos.x,	//�\��X���W 
				(int)this->m_baseY + pos.y,					//�\��Y���W
				this->m_size,								//�����̃T�C�Y
				this->m_changeColor,						//�����̐F
				true);										//�C�^���b�N�̃t���O

			//�������m�̊Ԋu���J����
			pos.x += this->m_size;
		}
	}

	this->m_changeColor = this->m_defaultColor;			//back to default color
	//���̃t�@�C���\���ɍ��W�����Z�b�g����
	pos.x = 0;							//X���W
	pos.y = 0;							//Y���W
}


//************************************************************************************************
//�ǂݍ���
//************************************************************************************************
int		CTalk::LoadTalk(char *pass, char *mes, int offset) {

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
			//���̃t�@�C����ǂݍ��߂�t���O��true
			this->m_fNext = true;
			return this->m_readoffset;
		}
		//mess�̗v�fi�Ԃɓǂݍ���
		fread(mes + i, sizeof(char), 1, fp);
		//�ǂݍ��񂾃T�C�Y�����Z
		this->m_readoffset++;
		//�ǂݍ��񂾃f�[�^�̕��тɁu#e�v���Ȃ����`�F�b�N
		if (mes[i] == 'e' &&
			mes[i - 1] == '#') {
			this->m_limitWords = i;
			fclose(fp);
			return this->m_readoffset;
		}
		//�ǂݍ��񂾃f�[�^�̕��тɉ��s���Ȃ����`�F�b�N
		if (mes[i] == 10 &&
			mes[i - 1] == 13) {
			mes[i] = 0;
			mes[i - 1] = 0;
			i -= 2;
		}
	}
	fclose(fp);
	return offset;
}


//************************************************************************************************
//�t�@�C����ǂݍ���ŕ��͂𐶐�
//************************************************************************************************
void	CTalk::CreateTalk(
	char *lpPass,
	float startX, float startY) 
{

	//���͐����ς݂Ȃ�I��
	if (this->m_fShow) return;

	//���ݕ\�����Ă��镶����
	this->m_word = 0;
	//1��������\�����Ă���̌o�ߎ���
	this->m_cnt = 0;
	//�\�����W
	this->m_baseX = startX;		//X���W
	this->m_baseY = startY;		//Y���W
	//�������𑝂₷
	this->m_fRead = true;
	//�ǂݍ��݈ʒu���Z�b�g
	this->m_readoffset = 0;
	//�ǂݍ��݃t�@�C���p�X�����Z�b�g
	strcpy(&this->m_pass[0], lpPass);
	//�t�@�C�����玟�̕��͂�ǂݍ���
	this->m_readoffset = LoadTalk(
		&this->m_pass[0],							 //�t�@�C���p�X
		&this->m_mes[0],							  //������
		this->m_readoffset);						  //�ǂݍ��񂾃o�C�g��

	//���͐����I��
	this->m_fShow = true;

}


/*
from below functions for baloon of talking
*/

/*
initializer
*/
void	CTalk::InitChara(
	TPOINT<float> baloonPos, TPOINT<int> baloonSize) 
{
	this->m_Fpos.x = this->m_baseX;
	this->m_Fpos.y = this->m_baseY;
	this->m_Isize = baloonSize;
}

/*
Update baloon
*/
void	CTalk::UpdateChara() {
	if (this->m_existFlag) {
		this->m_Fpos.x = this->m_baseX - 20.0f;
		this->m_Fpos.y = this->m_baseY - 20.0f;
	}
}

/*
drawing function
*/
void	CTalk::DrawChara(
	tagDRAWINGKIND drawKind, 
	tagDDSCALESTARTINGPOINT DDScaleStartPoint) 
{

	if (this->m_existFlag) {

		switch (drawKind)
		{
		case DRAWING_TYPE_DDBLT:
			DDBlt(
				this->m_Fpos.x,
				this->m_Fpos.y,
				this->m_Isize.x,
				this->m_Isize.y,
				this->m_ImgNum,
				this->m_Ipic.x,
				this->m_Ipic.y
				);
			break;

		case DRAWING_TYPE_DDSCALEBLT:
			DDScaleBltSelectIndicate(
				this->m_Fpos.x,
				this->m_Fpos.y,
				this->m_Isize.x,
				this->m_Isize.y,
				this->m_ImgNum,
				this->m_Ipic.x,
				this->m_Ipic.y,
				this->m_scale,
				DDScaleStartPoint);
			break;

		case DRAWING_TYPE_DDALPHABLT:
			DDAlphaBlt(
				(int)this->m_Fpos.x,
				(int)this->m_Fpos.y,
				this->m_Isize.x,
				this->m_Isize.y,
				this->m_ImgNum,
				this->m_Ipic.x,
				this->m_Ipic.y,
				this->m_alpha);
			break;
		}
	}
}
