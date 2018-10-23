#include	"wipe.h"
#include	"play.h"
#include	"map.h"
#include	"opening.h"
#include	"menu.h"
#include	"SelectStage.h"
#include	<stdio.h>
#include	<math.h>

//���O���[�o���ϐ��錾
//���C�v�摜������
int g_wipeImg[WIPE_IMAGE_MAX] = {0};

//���C�v�摜�ݒ�p
WIPE g_wipe[WIPE_MAX] = {0};

//���[�h���C���[�W�ݒ�p
int g_loadImg[LOAD_IMAGE_MAX] = {0};

//���[�h���C���[�W�ݒ�p
WIPE g_load[LOAD_IMAGE_MAX] = {0};

//���C�v���o������
tagWIPE_DIRECT g_directwipe = WIPE_FEEDOUT;

//�V�[���؂�ւ��t���O
bool g_wipescene = false;

//************************************************************************************************
//������
//************************************************************************************************
void InitWipe(HWND hWnd){
	
	//���摜�̓ǂݍ���
	//���C�v�摜�i�w�i���j���߉��o�p
	g_wipeImg[0] = CreateDDImage("Image\\wipe.bmp" , hWnd);

	//���C�v�摜�i�w�i���j�^�񒆂Ɏ��k���鉉�o�p
	g_wipeImg[1] = CreateDDImage("Image\\wipe_circle.bmp" , hWnd);
	g_wipeImg[2] = CreateDDImage("Image\\wipe_half1.bmp" , hWnd);
	g_wipeImg[3] = CreateDDImage("Image\\wipe_half2.bmp" , hWnd);

	//�ǂݍ��݃C���[�W�摜�uNowLoading�v
	g_loadImg[0] = CreateDDImage("Image\\loading.bmp" , hWnd);
	
	//��ʂ𕢂�����Ԃ̑؍ݎ���
	g_wipe[0].stay = WIPE_DEFAULT_STAY_TIME;

	//�����[�h�C���[�W�̏�����
	g_load[0].ch.h = 24;			//����
	g_load[0].ch.w = 150;			//��
	g_load[0].ch.x = 10.0f;			//X���W
	g_load[0].ch.y = 440.0f;		//Y���W
	g_load[0].ch.picx = 0;			//�\����X
	g_load[0].ch.picy = 0;			//�\����Y

	//���[�h�C���[�W�̃A�j���[�V�����ݒ�̏�����
	g_load[0].ani.cnt = 0;		//���݂̃R�}��
	g_load[0].ani.fram = 10;	//�t���[����
	g_load[0].ani.max = 5;		//�ő�R�}��
	g_load[0].ani.time = 0;		//�o�ߎ���
	g_load[0].ani.type = 0;		//��ޔԍ�

	//�A�j���[�V������ݒ�
	SetAnimation(&g_load[0].ch , &g_load[0].ani ,
				g_load[0].ani.sx , g_load[0].ani.sy ,
				g_load[0].ani.max , g_load[0].ani.fram ,
				g_load[0].ani.type , true);

}

//************************************************************************************************
//�X�V
//************************************************************************************************
bool UpdateWipe(){

	//���C�v�����̕���
	void (*pdirectwipe[WIPE_DIRECT_MAX])() = {
		
		DirectWipe0 ,		//���߉��o
		DirectWipe1 ,		//���k���o
	};

	//�X�e�[�W�N���A�𒲂ׂ�p
	int complete = 0;

	//���C�v�̏���
	if(g_wipe[0].ch.flag){
		
		//���C�v���o�̕���
		pdirectwipe[g_directwipe]();
		
		//�V�[���؂�ւ��t���O�����������ʑJ��
		if( g_wipescene ){

			//�X�e�[�W�N���A�𒲂ׂ�
			for( int i = 0 ; i < STAGE_MAX ; i++ ){
				if( *( GetCompleteStage() + i ) == SELECTSTAGE_STAGE_COMPLETE ) complete++;
			}

			//�� ���̃V�[�����X�e�[�W�I���V�[���ŁA�S�X�e�[�W���N���A�ςȂ玟�̃V�[����
			//�G���f�B���O�֕ύX����
			if( g_wipe[0].next == SCENE_SELECTSTAGE &&  complete == 3 ) g_wipe[0].next = SCENE_ENDING;

			//���C�v�̎�ނ����Ƃɏ�ʕύX
			switch( g_wipe[0].type ){
			
				//�V�[���؂�ւ�
				case CHANGE_SCENE:
					ChangeScene( g_wipe[0].next );
					break;
				
				//�X�e�[�W�؂�ւ�
				case CHANGE_STAGE:
					break;
				
				//���[�X���n�߂���X�^�[�g
				case RESTART_RACE:
					RestartRace();
					break;

				//���[�X�𔲂��ă^�C�g����
				case TO_TITLE:
					ToTitle();
					break;
				//�X�e�[�W�Z���N�g��
				case TO_SELECT:
					ToSelectStage();
					break;
			}
			//�ύX��A��ނ�-1��
			g_wipe[0].type = NOT_TRANSITION;
		}
		//���[�h�C���[�W�摜�̃A�j���[�V����
		UpdateAnimation( &g_load[0].ch , &g_load[0].ani );

	}
	//�Ō�Ƀ��C�v�̐����t���O��Ԃ�
	return g_wipe[0].ch.flag;

}


//************************************************************************************************
//���C�v�����i���߉��o�j
//************************************************************************************************
void DirectWipe0(){
	
	//���[�h�C���[�W���\���ŁA���C�v�̃A���t�@�l��1���傫���Ȃ�Ó]����
	if( g_load[0].ch.flag == false && 
		g_wipe[0].alpha > 1.0f && g_wipescene == false ) g_wipe[0].alpha += g_wipe[0].sub_alpha;

	//���C�v�Ŋ��S�ɈÓ]������A���̂܂܃��C�v���Ƃǂ߂āA��ʐ؂�ւ�
	if( g_wipe[0].alpha <= 1.0f && g_wipe[0].ch.time < g_wipe[0].stay ){
	
		//���[�h�C���[�W�摜��\��
		g_load[0].ch.flag = true;
		//���Ԃ��v��
		g_wipe[0].ch.time++;
		//�A���t�@�l�́u1�v
		g_wipe[0].alpha= 1.0f;
		//��ʐ؂�ւ��t���O��true��
		g_wipescene = true;
		return;
	}

	//��ʑJ�ڂ�����
	if( g_wipescene ){
		//�w�肵���؍݃t���[�����𒴂�����A��ʐ؂�ւ��A�Ó]���班�������߂ŃQ�[����ʂ�\��
		if( g_wipe[0].ch.time >= g_wipe[0].stay && g_wipe[0].alpha < 255.0f ){
			//���X�ɓ��߂��Ă���
			g_wipe[0].alpha += g_wipe[0].add_alpha;
			//���[�h�C���[�W�摜������
			g_load[0].ch.flag = false;
		}

		//���ߒl���u255�ȏ�v���A���[�h�C���[�W�摜���������烏�C�v����
		if( g_wipe[0].alpha >= 255.0f && g_load[0].ch.flag == false ){

			//�o�ߎ��ԃ��Z�b�g
			g_wipe[0].ch.time = 0;

			//���C�v�����t���O��false��
			g_wipe[0].ch.flag = false;
			
		}
	}

}


//************************************************************************************************
//���C�v�����i�g�k���o�j
//************************************************************************************************
void DirectWipe1(){


	//��ʂ̑Ίp�������߂�
	float line_d = sqrtf( (STAGE_WIDTH * STAGE_WIDTH) + (STAGE_HEIGHT * STAGE_HEIGHT) );

	//���Ɨ]���̍������߂�
	float margin = (float)g_wipe[1].ch.w - 700.0f;
	
	//���̊g�k�𔽉f�������a�����߂�
	float circle_d = (float)(g_wipe[1].ch.w * g_wipe[1].ch.scale) - (margin * g_wipe[1].ch.scale);

	//�g��E�k����̕��E���������߂�
	//��
	float scale_cw = (float)(g_wipe[1].ch.w * g_wipe[1].ch.scale);
	float scale_ch = (float)(g_wipe[1].ch.h * g_wipe[1].ch.scale);
	//��
	float scale_th = (float)(g_wipe[2].ch.h * g_wipe[2].ch.scale);	//����
	//��
	float scale_lw = (float)(g_wipe[3].ch.w * g_wipe[3].ch.scale);	//��
	//��
	float scale_bh = (float)(g_wipe[4].ch.h * g_wipe[4].ch.scale);	//����
	//�E
	float scale_rw = (float)(g_wipe[5].ch.w * g_wipe[5].ch.scale);	//��

	//���S�����߂�
	//��
	float center_cx = (float)(g_wipe[1].ch.w / 2);
	float center_cy = (float)(g_wipe[1].ch.h / 2);
	//��
	float center_ty = (float)(g_wipe[2].ch.h / 2);
	//��
	float center_lx = (float)(g_wipe[3].ch.w / 2);
	//��
	float center_by = (float)(g_wipe[4].ch.h / 2);
	//�E
	float center_rx = (float)(g_wipe[5].ch.w / 2);

	//�e���C�v�̍��W���Z�o����
	//���̍��W
	float wipe_cx = g_wipe[1].ch.x + center_cx - (scale_cw / 2);	//X
	float wipe_cy = g_wipe[1].ch.y + center_cy - (scale_ch / 2);	//Y
	//��̍��W
	float wipe_ty = g_wipe[2].ch.y + center_ty - (float)(scale_th / 2);	//Y
	//���̍��W
	float wipe_lx = g_wipe[3].ch.x + center_lx - (scale_lw / 2);	//X
	//���̍��W
	float wipe_by = g_wipe[4].ch.y + center_by - (scale_bh / 2);	//Y
	//�E�̍��W
	float wipe_rx = g_wipe[5].ch.x + center_rx - (scale_rw / 2);	//X

	//���܂�������������ɏ㉺���E�̃��C�v�̍��W���C������
	g_wipe[2].ch.y = (wipe_cy - scale_th) + ((scale_ch - circle_d) / 2);		//��
	g_wipe[3].ch.x = (wipe_cx - scale_lw) + ((scale_cw - circle_d) / 2);		//��
	g_wipe[4].ch.y = (wipe_cy + scale_ch) - ((scale_ch - circle_d) / 2);		//��
	g_wipe[5].ch.x = (wipe_cx + scale_cw) - ((scale_cw - circle_d) / 2);		//�E

	//���S���ۂ�����ʂ����摜�������Ȃ��Ȃ�܂ŏ���
	if(g_wipe[1].ch.scale > 0 && g_wipescene == false){
		g_wipe[1].ch.scale += g_wipe[1].contraction_speed;
	}
	//���������Ȃ�����Ó]�̏�Ԃ��ێ����ăV�[���؂�ւ�
	if(g_wipe[1].ch.scale <= 0){
		//���摜�̊g�k�{����0��
		g_wipe[1].ch.scale = 0.0f;
		//�V�[���؂�ւ��t���O��true��
		g_wipescene = true;
		//���[�h���C���[�W�̕\��
		g_load[0].ch.flag = true;
		//���Ԍo��
		g_wipe[0].ch.time++;
	}
	//�w�肵���؍݃t���[���𒴂�����V�[���\��
	if(g_wipe[0].ch.time > g_wipe[0].stay){
		//���[�h���C���[�W���\��
		g_load[0].ch.flag = false;
		//�g��
		g_wipe[1].ch.scale += g_wipe[1].scale_speed;
	}
	//������ʂ��O�܂ōL���肫������\���t���O�����Ԃ����Z�b�g
	if(circle_d > line_d){
		g_wipe[0].ch.flag = false;	//�\���t���O
		g_wipe[0].ch.time = 0;		//���Ԃ����Z�b�g
		
	}

}

//************************************************************************************************
//�\��
//************************************************************************************************
void DrawWipe(){

	//�g�又���p�摜�v�f
	int imageTbl[5] = {
		g_wipeImg[1],
		g_wipeImg[2],
		g_wipeImg[3],
		g_wipeImg[2],
		g_wipeImg[3]
	};

	//���@���ߏ���
	//���C�v�������t���O��true�Ȃ�\��
	if( g_wipe[0].ch.flag && g_directwipe == WIPE_FEEDOUT ){

		//���C�v�摜�̕\��
		DDAlphaBlt(						
			(int)g_wipe[0].ch.x ,
			(int)g_wipe[0].ch.y ,						
			g_wipe[0].ch.w ,
			g_wipe[0].ch.h ,				
			g_wipeImg[0] ,		
			g_wipe[0].ch.picx ,
			g_wipe[0].ch.picy ,
			g_wipe[0].alpha);		
	}

	//���@�g�k����
	//���C�v�������t���O��true�Ȃ�\��
	if(g_wipe[0].ch.flag == true && g_directwipe == WIPE_CONTRACTION){
		for (int i = 1; i < WIPE_MAX; i++) {
			DDScaleBltSelectIndicate(
				(float)g_wipe[i].ch.x,
				(float)g_wipe[i].ch.y,
				g_wipe[i].ch.w,
				g_wipe[i].ch.h,
				imageTbl[i - 1],
				g_wipe[i].ch.picx,
				g_wipe[i].ch.picy,
				g_wipe[i].ch.scale,
				1);
		}
	}
	
	//�����[�h���C���[�W�摜�uNowLoading�v
	//���[�h�C���[�W�摜�̐����t���O��true�Ȃ�\��
	if(g_load[0].ch.flag == true){

		//���[�h�C���[�W�摜�̕\��
		DDBlt(						
			(int)g_load[0].ch.x,
			(int)g_load[0].ch.y,						
			g_load[0].ch.w,
			g_load[0].ch.h,				
			g_loadImg[0],		
			g_load[0].ch.picx,
			g_load[0].ch.picy);		
	}
}

//************************************************************************************************
//���
//************************************************************************************************
void ReleaseWipe(){
	
	int i = 0;

	//���C�v�摜�̉��
	for(i = 0;i < WIPE_IMAGE_MAX;i++){
		ReleaseDDImage(g_wipeImg[i]);
	}
	//���[�h���C���[�W�摜�̉��
	for(i = 0;i < LOAD_IMAGE_MAX;i++){
		ReleaseDDImage(g_loadImg[i]);
	}
}

//************************************************************************************************
//����
//************************************************************************************************
void CreateWipe( tagGAMETRASITION next , tagWIPE_TRANSITION type , tagWIPE_DIRECT direct ){


	//�V�[���؂�ւ��t���O��false
	g_wipescene = false;
	//���C�v���o���擾
	g_directwipe = direct;
	//���Ԃ����Z�b�g
	g_wipe[0].ch.time = 0;
	//���̃V�[�����擾
	g_wipe[0].next = next;

	//���@���߉��o
	//�������Ȃ烏�C�v����
	if(g_wipe[0].ch.flag == false && direct == WIPE_FEEDOUT){
		
		//���C�v1�i���w�i�u���ߏ����p�v�j�̐ݒ�
		g_wipe[0].ch.x = 0.0f;		//X���W
		g_wipe[0].ch.y = 0.0f;		//Y���W
		g_wipe[0].ch.w = 640;		//��
		g_wipe[0].ch.h = 480;		//����
		g_wipe[0].ch.picx = 0;		//�\����X
		g_wipe[0].ch.picy = 0;		//�\����Y
		g_wipe[0].ch.flag = true;	//���C�v�̐����t���O
		g_wipe[0].type = type;		//���
		g_wipe[0].alpha = 255.0f;	//�A���t�@�l
		g_wipe[0].add_alpha = WIPE_DEFAULT_ALPHA_ADD;//�A���t�@���Z�l
		g_wipe[0].sub_alpha = WIPE_DEFAULT_ALPHA_SUB;//�A���t�@���Z�l
		
	}

	//���@���k���o
	//�������Ȃ烏�C�v����
	if(g_wipe[0].ch.flag == false && direct == WIPE_CONTRACTION){

		//���C�v2�i���w�i�u��ʒ��S�֎��k���鉉�o�p�v�j

		////��ʒ������ۂ�����ʂ����摜
		g_wipe[1].ch.w = 1500;
		g_wipe[1].ch.h = 1500;
		g_wipe[1].ch.x = (float)((STAGE_WIDTH - g_wipe[1].ch.w) / 2);
		g_wipe[1].ch.y = (float)((STAGE_HEIGHT - g_wipe[1].ch.h) / 2);
		g_wipe[1].ch.picx = 0;
		g_wipe[1].ch.picy = 0;
		//���C�v�̐����t���O��true��
		g_wipe[0].ch.flag = true;
		//�J�ڂ̎��
		g_wipe[0].type = type;
		//�g�k�{��
		g_wipe[1].ch.scale = 1.0f;
		//�k�����x
		g_wipe[1].contraction_speed = WIPE_DEFAULT_CONTRACTION_SPEED;
		//�g�呬�x
		g_wipe[1].scale_speed = WIPE_DEFAULT_SCALE_SPEED;

		//�ォ��g�k�u�����̒����`�v
		g_wipe[2].ch.x = 0.0f;
		g_wipe[2].ch.y = -240.0f;
		g_wipe[2].ch.w = 640;
		g_wipe[2].ch.h = 260;
		g_wipe[2].ch.picx = 0;
		g_wipe[2].ch.picy = 0;
		//�g�k�{��
		g_wipe[2].ch.scale = 1.0f;

		//������g�k�u�c���̒����`�v
		g_wipe[3].ch.x = -320.0f;
		g_wipe[3].ch.y = 0.0f;
		g_wipe[3].ch.w = 340;
		g_wipe[3].ch.h = 480;
		g_wipe[3].ch.picx = 0;
		g_wipe[3].ch.picy = 0;
		//�g�k�{��
		g_wipe[3].ch.scale = 1.0f;


		//������g�k�u�����̒����`�v
		g_wipe[4].ch.x = 0.0f;
		g_wipe[4].ch.y = 480.0f;
		g_wipe[4].ch.w = 640;
		g_wipe[4].ch.h = 260;
		g_wipe[4].ch.picx = 0;
		g_wipe[4].ch.picy = 0;
		//�g�k�{��
		g_wipe[4].ch.scale = 1.0f;


		//�E����g�k�u�����̒����`�v
		g_wipe[5].ch.x = 640.0f;
		g_wipe[5].ch.y = 0.0f;
		g_wipe[5].ch.w = 340;
		g_wipe[5].ch.h = 480;
		g_wipe[5].ch.picx = 0;
		g_wipe[5].ch.picy = 0;
		//�g�k�{��
		g_wipe[5].ch.scale = 1.0f;


	}
}
