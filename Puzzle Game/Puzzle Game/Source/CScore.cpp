#include	"CScore.h"
#include	"graphics.h"
#include	"input.h"

//redefine the static number
//for score's number
const TPOINT<int> CScore::SCORE_SCORE_NUMBER_IMAGE_SIZE = { 40,50 };
const float CScore::SCORE_DEFAULT_SCORE_NUMBER_IMAGE_SCALE = 1.0f;
//present indicate number
int CScore::m_indicateNum = 0;

CScore::~CScore(){}

void CScore::InitChara() {
	//intialize indication
	this->m_Isize = CScore::SCORE_SCORE_NUMBER_IMAGE_SIZE;				//size
	this->m_scale = CScore::SCORE_DEFAULT_SCORE_NUMBER_IMAGE_SCALE;		//scale rate
}

void CScore::UpdateChara(){}

void CScore::DrawChara() {}

void CScore::ReleaseChara(){}


/*
Draw number's image
*/
void CScore::DrawNumber(
		TPOINT<float> pos, int number,				//indicate coordinate, draw number
		int digit, tagSCORECOLORTYPE color,			//digit, kind of color
		float variableScale) {						//score's scale rate

	//�\�����鐔�l���󂯎��
	int num = number;
	//�\�����鐔�l
	int rest = 0;
	//�\�����鐔�l�̕\����
	int numD = 0;
	//���J�E���g�p
	int digitCnt = 0;

	//�n���ꂽ�l�����̒l�Ȃ琳�̒l�ɂ���
	if (num < 0) num *= -1;

	//�n���ꂽ�����̐��l�̊e���̐��l�𒲂ׂ�
	do {
		//�]��̐��l�����߂錅�̐��l
		rest = num % 10;
		//�n���ꂽ�l�𖈉�10�Ŋ����Ă���
		num /= 10;
		//�\�����鐔�̕\����������
		numD = this->m_Isize.x * rest;

		//���l�摜�̕\��
		DDScaleBltSelectIndicate(
			pos.x - (this->m_Isize.x * this->m_scale) * digitCnt,
			pos.y,
			this->m_Isize.x,
			this->m_Isize.y,
			this->m_ImgNum,
			this->m_Ipic.x + numD,
			this->m_Ipic.y + (this->m_Isize.y * color),
			variableScale,
			DDSCALE_STARTING_POINT_LEFT_TOP
			);
		//�v�Z���閈�ɃJ�E���g
		digitCnt++;
		//�n���ꂽ�l��0�����ɂȂ����甲����
	} while (num > 0 || digitCnt < digit);
}


/*
direction of drawing number that increase or decrease
*/
void		CScore::GraduallyNumber(
			int	indicateNum,				//present drwing number
			int	terminateNum,				//tereminate to drawing number
			int intervalFram				//interval time
			){								//direction of number drawing that increase or decrease

	//�����l�̏���l�E�����l�̐ݒ�
	//������0
	if (terminateNum < 0) terminateNum = 0;

	//�����1������
	if (terminateNum > 100000000) terminateNum = 99999999;

	//���\������l�ƕ\���������l��������珈�����s��
	//�������͉�]�̏������܂������Ă�������s����
	if (terminateNum != indicateNum) {
		//rest of number
		int rest;
		//�\������l���\���������l��菬���������瑝������
		if (indicateNum <= terminateNum) {
			//�\������l�ƕ\���������l�̍��������߂�
			rest = terminateNum - indicateNum;
			//�t���[�����ɉ��Z����
			indicateNum += rest / intervalFram;
			//�\���������l���\������l���獷�����������l�t�߂ɂȂ�����
			//�\���������l��������
			//�������́A�uZ�L�[���@�{�^���v����������l����
			if (rest <= intervalFram || JustKey('Z') || JustButton(BUTTON_1)) {
				//�ŏI�I�ɕ\������l�̑��
				indicateNum = terminateNum;
			}
		}
		//�\������l���\���������l���傫�������猸������
		if (indicateNum >= terminateNum) {
			//�\������l�ƕ\���������l�̍��������߂�
			rest = indicateNum - terminateNum;
			//�t���[�����Ɍ��Z����
			indicateNum -= rest / intervalFram;
			//�\���������l���\������l���獷�����������l�t�߂ɂȂ�����
			//�\����������l��������
			if (rest <= intervalFram || JustKey('Z') || JustButton(BUTTON_1)) {
				//�ŏI�I�ɕ\������l�̑��
				indicateNum = terminateNum;
			}
		}
	}
	//subsutitute calculation value for present indication number of variable
	this->SetNumber(indicateNum);
}

/*
direction rolling for draw number
*/
void	CScore::GraduallyRolling(
				int terminateNumber,		//end the indication number
				int rollingTimeMax,			//roll time
				int digit) {				//indication digit

	//�t���[���o��
	this->m_rollingCount++;

	//�\���������̐��l������
	int unsigned terminateNum = 1;
	//����̌������v�Z
	for (int i = 0; i < digit; i++) { terminateNum *= 10; }
	//���t���[�����Z����l�����߂�
	int add = ((terminateNum - 1) / 9);

	//�w�肵���t���[���Ɍo�߂���܂ŃX�R�A�̕\������]��Ԃɂ���
	if (this->m_rollingCount < rollingTimeMax) {
		//�\������l�����Z����
		this->SetNumber(this->m_indicateNum + add);
		//���̈ʂ܂ŕ\������
		if (abs(this->m_indicateNum) >= terminateNum) this->m_indicateNum = 0;
	}
	//�o�߃t���[�����w��t���[���ɒB������A�l�������ĕ\������
	//�������́A����L�[�������ꂽ����
	if (this->m_rollingCount >= 20) {				//rolling process at least 20 fram
		if (this->m_rollingCount >= rollingTimeMax ||
			JustKey('Z') || JustButton(BUTTON_1)) {
			//�\������l�ɑ������
			this->m_indicateNum = terminateNumber;
		}
	}
}