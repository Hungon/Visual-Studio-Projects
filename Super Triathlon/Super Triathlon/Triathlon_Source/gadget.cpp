#include "gadget.h"
#include "seastage.h"
#include "swimer.h"
#include "game.h"
#include "input.h"
#include "score.h"
#include "sound.h"
#include "SelectStage.h"
#include "play.h"

//���O���[�o���ϐ�
//�摜������
int g_gadgetImg[GADGET_IMAGE_MAX] = {0};
//�ݒ�p
GADGET g_gadget[GADGET_SET_MAX] = {0};
//�l�\���p
SCOREVALUE g_gadgetScore[GADGET_SCORE_MAX] = {0};


//************************************************************************************************
//������
//************************************************************************************************
void InitGadget(HWND hWnd){

	//���݂̃X�e�[�W���擾
	tagSELECTSTAGE stageNum = GetStage();

	//���摜�̓ǂݍ���
	//�����{�����Q�[�W�ŕ\��
	g_gadgetImg[0] = CreateDDImage("Image\\charge.bmp" , hWnd);
	//���x���[�^�[
	g_gadgetImg[1] = CreateDDImage("Image\\speedindicator.bmp" , hWnd);
	//�j
	g_gadgetImg[2] = CreateDDImage("Image\\index.bmp" , hWnd);

	//�K�W�F�b�g�̏�����
	ZeroMemory(&g_gadget[0], sizeof(GADGET) * GADGET_SET_MAX);
	//�X�R�A�̏�����
	ZeroMemory(&g_gadgetScore[0], sizeof(SCOREVALUE) * GADGET_SCORE_MAX);

	//���e��ݒ�
	//���x�`���[�W �i�C�X�e�[�W�̂݁j
	if (stageNum == STAGE_SEA) {
		g_gadget[0].ch.x = 20.0f;			//�\����X
		g_gadget[0].ch.y = 460.0f;			//�\����Y
		g_gadget[0].ch.h = 16;				//�\������
		g_gadget[0].ch.w = 96;				//�\����
		g_gadget[0].ch.flag = true;			//�\���t���O
	}
	//�K�W�F�b�g���u���x���[�^�[�v
	g_gadget[1].ch.h = 96;
	g_gadget[1].ch.w = 96;
	g_gadget[1].ch.x = 20.0f;
	g_gadget[1].ch.y = 364.0f;
	//�\���t���O
	g_gadget[1].ch.flag = true;

	//���j
	g_gadget[2].ch.x = g_gadget[1].ch.x + 13.0f;
	g_gadget[2].ch.y = g_gadget[1].ch.y + 15.0f;
	g_gadget[2].ch.h = 68;
	g_gadget[2].ch.w = 68;
	g_gadget[2].ani.fram = 10;
	g_gadget[2].ani.max = 55;
	//�A�j���[�V�����ݒ�
	SetAnimation(&g_gadget[2].ch , &g_gadget[2].ani ,
			g_gadget[2].ani.sx , g_gadget[2].ani.sy ,
			g_gadget[2].ani.max , g_gadget[2].ani.fram , 0 , true);

	//���x�l
	g_gadgetScore[0].score_scale = 0.25f;					//�X�R�A�g�k��
	g_gadgetScore[0].score_x = 20.0f;						//�X�R�A�\����X
	g_gadgetScore[0].score_y = 400.0f;						//�X�R�A�\����Y
	g_gadgetScore[0].ch.flag = true;						//�\���t���O
	g_gadgetScore[0].dicimals_flag = true;					//�����\���t���O
	g_gadgetScore[0].kiloFlag = true;						//km�̕\���t���O
	g_gadgetScore[0].ch.type = SCORE_NUMBER_TYPE_SPEED;	//�X�R�A�摜�̎��
	g_gadgetScore[0].digit = 2;							//����

}


//************************************************************************************************
//�X�V
//************************************************************************************************
void UpdateGadget(){
	
	//�C�X�e�[�W�̏����擾
	STAGE* pst = GetSeaStage();
	//�v���C���[�̏����擾
	SWIMER* pch = GetSwimer();

	
	//�\�����鑬�x���X�V
	g_gadgetScore[0].num = g_gadget[1].coordinate * 10;

	//�C�X�e�[�W�ŏc�X�N���[���ł̕ύX
	if(pst->scrolly){

		//���x�`���[�W���\��
		g_gadget[0].ch.flag = false;

		//���x�`���[�W�̈ʒu�𔽑΂�
		g_gadget[0].ch.x = 640.0f - (float)(g_gadget[1].ch.w + 20.0f);

		//���x���[�^�[���\��
		g_gadget[1].ch.flag = false;

		//���x���[�^�[�̈ʒu�𔽑΂�
		g_gadget[1].ch.x = 640.0f - (float)(g_gadget[1].ch.w + 20.0f);

		//���x���[�^�[�̐j�̈ʒu�𔽑΂�
		g_gadget[2].ch.x = g_gadget[1].ch.x + 13.0f;
		g_gadget[2].ch.y = g_gadget[1].ch.y + 15.0f;

		//���x���\��
		g_gadgetScore[0].ch.flag = false;

		//���x�\���̈ʒu�𔽑΂�
		g_gadgetScore[0].score_x = 640.0f - (float)(g_gadget[1].ch.w + 20.0f);

	}

	//�E�X�N���[���ł̕ύX
	if(pst->scroll_r){

		//���x�`���[�W��\��
		g_gadget[0].ch.flag = true;

		//���[�^�[��\��
		g_gadget[1].ch.flag = true;

		//���x��\��
		g_gadgetScore[0].ch.flag = true;
	}

	//�擾�����L�����N�^�[�̈ړ��ʂ𔽉f�����l�����Ƃ�
	//���݂̑��x���Z�o����
	g_gadget[1].indicator = g_gadget[1].coordinate * 10;


	//���@���x�\��
	//�������i�|X�ړ��j�Ȃ琳�̒l�ɏC������
	if(g_gadget[1].indicator < 0){
		g_gadget[1].indicator *= -1;
	}

	//�v���C���[�̌��݂̑��x�ɉ����ăX�R�A�̎Α̃t���O��؂�ւ���
	//�v���C���[�̑��x��100km�ȏ�ɂȂ����當�����Α̂ɂ���
	if( g_gadget[1].indicator >= GADGET_SPEED_SCORE_ITALIC ){

		//�Α̂������}�N������
		g_gadgetScore[0].italic = SCORE_DESIGN_ITALIC;

	}

	//�ݒ肵�����l�ȉ��ɂȂ�����ʏ�̕����̂ɖ߂�
	if( g_gadget[1].indicator <= GADGET_SPEED_SCORE_ITALIC ){
	
		//���̂������}�N������
		g_gadgetScore[0].italic = SCORE_DESIGN_VIRTUALLY;

	}

	//�R�}�̕ω��͖�T�����v���ɓ����Ă���
	//���݂̑��x�@*�@270���p�̍ō���120�����@/�@�������̊p�x270�����@1���ɑ΂��鑬�x�̕ω���
	//���x�̕ω��ʁ@*�@�P�R�}�ŕω�����p�x�@+�@�덷�i�������j
	if(g_gadget[1].indicator >= g_gadget[2].ani.cnt * 120 / 270 * 5 + 0.62){

		//���̃R�}��
		g_gadget[2].ani.cnt++;

	}else{

		//���݂̑��x���R�}*�A�j���[�V�����ω����鑬�x�ʂ����������O�̃R�}��
		g_gadget[2].ani.cnt--;
	}

	//���x���[�^�[�̍ō��\�����x120km�𒴂�����j���J��Ԃ���]������
	if(g_gadget[1].indicator >= 120){

		//�\������R�}�������Z
		g_gadget[2].ani.cnt++;
		
		//�ݒ肵���ő�R�}���t�߂ɒB������
		if(g_gadget[2].ani.cnt >= g_gadget[2].ani.max - 1){
			
			//�R�}�̃J�E���g�����Z�b�g
			g_gadget[2].ani.cnt = 0;
		}
	}

	//�\�������W���X�V
	g_gadget[2].ch.picx = g_gadget[2].ani.sx + ( g_gadget[2].ani.cnt * g_gadget[2].ch.w );
	g_gadget[2].ch.picy = g_gadget[2].ani.sy;

}

//************************************************************************************************
//�\��
//************************************************************************************************
void DrawGadget(){

	//�v���C���[�̏����擾
	SWIMER* pch = GetSwimer();
	
	//�����t���O��true�Ȃ�\��
	if(g_gadget[0].ch.flag){
		//���x�`���[�W�l
		DDBlt(
			(int)g_gadget[0].ch.x ,									//X���W
			(int)g_gadget[0].ch.y ,									//Y���W
			(int)(g_gadget[0].ch.w / pch->speedmax * pch->ch.speed) ,//��
			g_gadget[0].ch.h ,										//����
			g_gadgetImg[0] ,										//�Ή������摜�ԍ�
			g_gadget[0].ch.picx ,									//�\�����w���W
			g_gadget[0].ch.picy);									//�\�����x���W
	}
	
	//�����t���O��true�Ȃ�\��
	if(g_gadget[1].ch.flag){
		//�K�W�F�b�g�摜�u���x���[�^�[�v
		DDBlt(
			(int)g_gadget[1].ch.x ,			//X���W
			(int)g_gadget[1].ch.y ,			//Y���W
			g_gadget[1].ch.w ,				//��
			g_gadget[1].ch.h ,				//����
			g_gadgetImg[1] ,				//�Ή������摜�ԍ�
			g_gadget[1].ch.picx ,			//�\�����w���W
			g_gadget[1].ch.picy);			//�\�����x���W

		//���x���[�^�[�̐j
		DDBlt(
			(int)g_gadget[2].ch.x ,			//X���W
			(int)g_gadget[2].ch.y ,			//Y���W
			g_gadget[2].ch.w ,				//��
			g_gadget[2].ch.h ,				//����
			g_gadgetImg[2] ,				//�Ή������摜�ԍ�
			g_gadget[2].ch.picx ,			//�\�����w���W
			g_gadget[2].ch.picy);			//�\�����x���W
	}

	//���x�̕\��
	if(g_gadgetScore[0].ch.flag){

		DrawScore( &g_gadgetScore[0] );
	}
}

//************************************************************************************************
//���
//************************************************************************************************
void ReleaseGadget(){
	//�摜�̉��
	for(int i = 0;i < GADGET_IMAGE_MAX;i++){
		ReleaseDDImage(g_gadgetImg[i]);
	}
}

//************************************************************************************************
//�K�W�F�b�g�̃|�C���^��Ԃ��֐�
//************************************************************************************************
GADGET* GetGadget(){ return &g_gadget[1]; }

//************************************************************************************************
//�v���C���[�̃X�e�[�^�X���K�W�F�b�g�ɔ��f������
//************************************************************************************************
void SetStatus(float status){ g_gadget[1].coordinate = status; }
