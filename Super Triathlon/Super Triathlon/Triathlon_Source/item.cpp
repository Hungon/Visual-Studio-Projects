#include "item.h"
#include "swimer.h"
#include "seaenemy.h"
#include "map.h"
#include "input.h"
#include "play.h"
#include "sound.h"
#include "seastage.h"
#include <stdio.h>


//���O���[�o���ϐ��̐錾
//�摜�̗v�f������
int g_itemImg[ITEM_IMAGE_MAX] = {0};
//�A�C�e���ݒ�p
ITEM g_item[ITEM_MAX] = {0};
//�A�C�e���l�����̓��ꕨ�̐ݒ�p
ITEM g_item_box[1] = {0};

//�g�p�O�̂̃^�C�v
int g_type_old = ITEM_VACANT;
//���݂̏����A�C�e���̗v�f�ԍ�������
int g_get = ITEM_VACANT;
//���ʒ��̗v�f�ԍ�������
int g_among = ITEM_VACANT;
//���ʉ�
int g_it_sound[ITEM_SOUND_EFFECT_MAX] = {0};
//��Փx�ɂ���ăA�C�e���o������ύX
int g_itemincidentTbl[LEVEL_MAX] = {1 , 2 , 3};

//************************************************************************************************
//������
//************************************************************************************************
void InitItem(HWND hWnd){

	
	//���摜�̓ǂݍ���
	//�A�C�e���̉摜
	g_itemImg[0] = CreateDDImage("Image\\Sea\\item.bmp" , hWnd);
	//�A�C�e���l�����̓��ꕨ
	g_itemImg[1] = CreateDDImage("Image\\Sea\\itembox.bmp" , hWnd);

	//�����̓ǂݍ���
	//�A�C�e���l����
	g_it_sound[0] = CreateSound("SE\\itemget.wav");
	//�����A�C�e���g�p��
	g_it_sound[1] = CreateSound("SE\\strongitem.wav");
	//��̉��A�C�e���擾��
	g_it_sound[2] = CreateSound("SE\\weakitem.wav");


	//�A�C�e���̏�����
	ZeroMemory(&g_item[0], sizeof(ITEM) * ITEM_MAX);
	//�g�p�ς݂̃A�C�e���̔ԍ��������l�����Z�b�g
	g_type_old = ITEM_VACANT;
	//���݂̃^�C�v�����Z�b�g
	g_among = ITEM_VACANT;
	//���݂̏����A�C�e���̗v�f�ԍ������Z�b�g
	g_get = ITEM_VACANT;

	//�A�C�e���̊l�����̓��ꕨ�p
	g_item_box[0].ch.x = 320.0f - 48.0f;
	g_item_box[0].ch.y = 0.0f;
	g_item_box[0].ch.w = 96;
	g_item_box[0].ch.h = 96;
	g_item_box[0].ch.alpha = 120.0f;
	g_item_box[0].ch.flag = true;

	//���e�A�C�e���̏����l��ݒ�
	for (int i = 0; i < ITEM_MAX; i++) {

		g_item[i].ch.flag = false;								//�����t���O
		g_item[i].ch.h = 32;									//����
		g_item[i].ch.w = 32;									//��
		g_item[i].displayX = (int)(g_item_box[0].ch.x + 32.0f);	//�\��X���W
		g_item[i].displayY = (int)(g_item_box[0].ch.y + 32.0f);	//�\��Y���W
		g_item[i].ch.scale = 1.5f;								//�g�k�{��
		g_item[i].type = ITEM_VACANT;							//�^�C�v
	}


	//�A�C�e���̏o�������Փx�ɉ����Đݒ�
	//EASY�Ȃ疈��
	//NORMAL�Ȃ�2���1��̕p�x
	//HARD�Ȃ�3���1��̕p�x
	g_itemincidentTbl[GetGameLevel()];
	//�A�C�e���̏o���ʒu
	g_item[0].pos_x = 300.0f;		//X���W

}

//************************************************************************************************
//�X�V
//************************************************************************************************
void UpdateItem(){

	int i = 0;

	//�C�X�e�[�W�̏����擾
	STAGE* pst = GetSeaStage();

	//���E�X�N���[�����̕ύX
	if(pst->scroll_r){
		g_item[0].pos_x = -300.0f;	//�o���w���W
	}
	
	//�����X�N���[�����̕ύX
	if(pst->scroll_l){
		g_item[0].pos_x = 300.0f;	//�o���w���W
	}

	//�o������A�C�e���̍ő吔������
	for(i = 0;i < ITEM_MAX;i++){
		
		//��ʊO�Ȃ琶���t���O��false��
		if(CollisionCamera(&g_item[i].ch) == false){

			//�A�C�e���̃t���O��false��
			g_item[i].ch.flag = false;
			continue;
		}
	}

	//�A�C�e���̌��ʂ����������邩�𔻒肷��֐�
	CheckResetEffect();
}

//************************************************************************************************
//�\��
//************************************************************************************************
void DrawItem(){

	int i = 0;

	//�v���C���[�̏����擾����
	SWIMER* pch = GetSwimer();

	char mes[25] = {0};

	//�J�����̍��W���擾����
	float px = GetCameraX();		//X���W					
	float py = GetCameraY();		//Y���W


	//�o������A�C�e���̍ő吔������
	for(i = 0;i < ITEM_MAX;i++){

		//�A�C�e���̐����t���O��true�Ȃ�\��
		if(g_item[i].ch.flag){

			//�A�C�e���̉摜
			DDBlt(
				(int)( g_item[i].ch.x - px ) ,		//X���W
				(int)( g_item[i].ch.y - py ) ,		//Y���W
				g_item[i].ch.w ,				//��
				g_item[i].ch.h ,				//����
				g_itemImg[0] ,					//�Ή������摜�ԍ�
				g_item[i].ch.picx ,				//�\�����w���W
				g_item[i].ch.picy);				//�\�����x���W
		}
	}

	//����ς݂Ȃ���肵���A�C�e���摜������ɕ\��
	if(g_item[g_get].have){

		//�A�C�e���̉摜
		DDScaleBlt(
			(float)g_item[g_get].displayX ,	//X���W
			(float)g_item[g_get].displayY ,	//Y���W
			g_item[g_get].ch.w ,			//��
			g_item[g_get].ch.h ,			//����
			g_itemImg[0] ,					//�Ή������摜�ԍ�
			g_item[g_get].ch.picx ,			//�\�����w���W
			g_item[g_get].ch.picy ,			//�\�����x���W
			g_item[g_get].ch.scale);		//�g�k��
	}

	//����ς݂̃A�C�e�������߂�g�̉摜
	if(g_item_box[0].ch.flag){
		
		//�������ŕ\��
		DDAlphaBlt(
			(int)g_item_box[0].ch.x ,
			(int)g_item_box[0].ch.y ,
			g_item_box[0].ch.w ,
			g_item_box[0].ch.h ,
			g_itemImg[1] ,
			g_item_box[0].ch.picx ,
			g_item_box[0].ch.picy ,
			g_item_box[0].ch.alpha);
	}

}


//************************************************************************************************
//���
//************************************************************************************************
void ReleaseItem(){

	int i = 0;
	
	//�摜
	for(i = 0;i < ITEM_IMAGE_MAX;i++){
		ReleaseDDImage(g_itemImg[i]);
	}
	//���ʉ�
	for(i = 0;i < ITEM_SOUND_EFFECT_MAX;i++){
		ReleaseSound(g_it_sound[i]);
	}
}

//************************************************************************************************
//�A�C�e�����o��������֐�
//************************************************************************************************
void CreateItem(float x, float y, int r) {

	int i = 0;
	//�A�C�e���̃f�[�^�e�[�u���u�\����X���W�A���ʎ��ԁA�o���m���l�̏�l�A�o���m���l�̉��l�v
	//������
	int strengTbl[ITEM_KIND_STRENGTH][4] = {

		{ ITEM_1 , 1000 , 9 , 0 }   ,
		{ ITEM_2 , 1000 , 19 , 10 } ,
		{ ITEM_4 , 1000 , 29 , 20 } ,
	};
	//���
	int weakTbl[ITEM_KIND_WEAK][4] = {

		{ ITEM_3 , 500 , 39 , 30 } ,
		{ ITEM_5 , 500 , 49 , 40 } ,
	};

	int weak_type = 0;

	//�A�C�e�����o�������邩���Փx�ɉ����Ĕ���
	//�����_���̒l1�`1000�͈̔͂̒l���A���x�����ɐݒ肳�ꂽ�l��
	//��]�Z�Ŋ���؂ꂽ��A�C�e���𔭐�������
	if (RandomNum(1000, 1) % g_itemincidentTbl[GetGameLevel()] == 0) {

		//�o������A�C�e���̍ő吔������
		for (i = 0; i < ITEM_MAX; i++) {

			//�A�C�e���̐����t���O��false�Ȃ�l����
			if (g_item[i].ch.flag == false && g_item[i].have == false) {

				//�o�����W������
				g_item[i].ch.x = x;		//�w���W
				g_item[i].ch.y = y;		//�x���W

				//�� �o��������A�C�e����n���ꂽ�l�����ɕ��򂳂���		
				//���x�㏸
				if (r >= strengTbl[0][3] && r <= strengTbl[0][2]) {
					//��ޔԍ����擾
					g_item[i].type = ITEM_TYPE_SPEED_UP;
				}
				//���G
				if (r >= strengTbl[1][3] && r <= strengTbl[1][2]) {
					//��ޔԍ����擾
					g_item[i].type = ITEM_TYPE_ABSOLUTE;
				}
				//�g��
				if (r >= strengTbl[2][3] && r <= strengTbl[2][2]) {
					//��ޔԍ����擾
					g_item[i].type = ITEM_TYPE_BIG;
				}
				//����
				if (r >= weakTbl[0][3] && r <= weakTbl[0][2]) {
					//��ޔԍ����擾
					g_item[i].type = ITEM_TYPE_SPEED_DOWN;
				}
				//�k��
				if (r >= weakTbl[1][3] && r <= weakTbl[1][2]) {
					//��ޔԍ����擾
					g_item[i].type = ITEM_TYPE_SMALL;
				}
				//�t���O���n�m
				g_item[i].ch.flag = true;
				//0�ȏォ�A10��菬�����Ȃ�u�����A�C�e���v
				if (g_item[i].type >= 0 && g_item[i].type < ITEM_KIND_BOUND) {
					//���ʎ��Ԃ���
					g_item[i].ch.time = strengTbl[g_item[i].type][1];
					//�\����X���W
					g_item[i].ch.picy = strengTbl[g_item[i].type][0];
				}
				//�n���ꂽ��ޔԍ�����̉��������P�O�ȏ�A�P�P�ȉ��̒l�Ȃ�
				//�Ή������v�f�ԍ��ɕς���u��̉��A�C�e���v
				if (g_item[i].type >= ITEM_KIND_BOUND &&
					g_item[i].type < (ITEM_KIND_BOUND + ITEM_KIND_WEAK)) {
					//�z��̗v�f�ɒl�����킹��
					weak_type = (g_item[i].type - ITEM_KIND_BOUND);
					//��ޔԍ��ɉ������l����
					//���ʎ��Ԃ���
					g_item[i].ch.time = weakTbl[weak_type][1];
					//�\����X���W
					g_item[i].ch.picy = weakTbl[weak_type][0];
				}
				return;
			}
		}
	}
}

//************************************************************************************************
//�A�C�e���Ƃ̔�������֐�
//************************************************************************************************
bool CollisionItem(SWIMER* pch){

	int i = 0;
	int weak_type = 0;

	//�߂�l�p
	bool ret = false;

	//�A�C�e���̌��ʂ������֐��|�C���^
	void (*pweak[ITEM_KIND_WEAK])() = {	
		
		SwimerEffect2 ,			//����
		SwimerEffect4 ,			//�k��
	};

	//�����ŃL�����N�^�[�Əo�������A�C�e���ƂŔ���
	for( i = 0 ; i < ITEM_MAX ; i++ ){

		//�����̃A�C�e�����o��������
		if(g_item[i].ch.flag == true){
			
			//�����̃L�����N�^�[��i�Ԃ̃A�C�e���Ŕ���
			if( CollisionCharacter( &pch->ch , &g_item[i].ch ) ){
		
				//���������A�C�e��������	
				g_item[i].ch.flag = false;
			
				//�擾�����A�C�e������ޖ��ɃJ�E���g
				g_item[0].get[g_item[i].type]++;
		
				//���������A�C�e���̎�ޔԍ���0�Ԉȏ�A9�Ԉȉ��Ȃ珈���i�����A�C�e���j
				if( g_item[i].type >= 0 && g_item[i].type < ITEM_KIND_BOUND ){

					//�A�C�e������ς݃t���OON
					g_item[i].have = true;

					//�\������v�f�ɒl�����i�����p�j����̃A�C�e���͔�\��
					g_get = i;

					//�l����
					PlaySound(g_it_sound[0] , 0 , false);

					//�A�C�e���擾��Ԃ�
					ret = true;

					break;
				}

				//���������A�C�e���̎�ޔԍ���10�Ԉȏ�i��̉��A�C�e���j�Ȃ�
				//�Ή������v�f�ԍ��ɕϊ�����
				//��̂������l�����𒲂ׂ�
				if( g_item[i].type >= ITEM_KIND_BOUND && 
					g_item[i].type < ( ITEM_KIND_BOUND + ITEM_KIND_WEAK ) ){

					//����ς݃t���OOFF
					g_item[i].have = false;	
				
					//��̂������v�f�Ɍ��݂̎�ޔԍ��|�����Ǝ�̂̋��E�̒l��������
					weak_type = ( g_item[i].type - ITEM_KIND_BOUND );
					
					//�K�p�������ʂ̗v�f�ԍ����擾
					g_among = i;
					
					//���ʒ��t���O��true�ɂ���
					g_item[i].among = true;
					
					//�l����
					PlaySound(g_it_sound[2],0,false);
					
					//���ʒ����A������ނȂ���ʎ��Ԃ�����
					if(g_item[i].among == true && g_type_old == g_item[i].type){
					
						//���ԉ��Z
						g_item[i].e_time += g_item[i].ch.time;
					}

					//�^�C�v���قȂ�Ȃ�A�Ή��������ʎ��Ԃ���
					if(g_type_old != g_item[i].type){

						//���Ԃ���
						g_item[i].e_time = g_item[i].ch.time;

					}
				
					//��ޔԍ����ƂɌ��ʂ𕪊�
					pweak[weak_type]();

					//�g�p�ς݂̃A�C�e���ԍ����擾
					g_type_old = g_item[i].type;

					//�A�C�e���ɓ���������true��Ԃ�
					ret = true;

					break;
				}
			}
		}
	}
	//�������Ă���A�C�e�����Ȃ�������false��Ԃ�
	return ret;
}

//************************************************************************************************
//�A�C�e���g�p���̏���
//************************************************************************************************
void UseItem(){

	SWIMER* pch = GetSwimer();		//�v���C���[�̃|�C���^���擾
	
	//�A�C�e���̌��ʂ������֐��|�C���^
	void (*pstreng[ITEM_KIND_STRENGTH])() = {	
		
		SwimerEffect0 ,				//���x�㏸
		SwimerEffect1 ,				//���G
		SwimerEffect3 ,				//�g��
	};
	
	//�͈͊O�Ȃ�return����
	if(g_item[g_get].type < 0 || g_item[g_get].type >= ITEM_KIND_STRENGTH ) return;

	//�����A�C�e���̗v�f�͈͓̔����𒲂ׂ�
	//�����𕪊򂳂���
	if(g_item[g_get].type >= 0 && g_item[g_get].type < ITEM_KIND_STRENGTH){
	
		//���ݏ������Ă���A�C�e���̌��ʂ𔭊�
		if(g_item[g_get].have == true){

			//���ʒ��̗v�f�ԍ����擾
			g_among = g_get;

			//���ʒ��t���O��ON
			g_item[g_get].among = true;

			//�p���[�A�b�v��
			PlaySound(g_it_sound[1],0,false);

			//���ʒ����A������ނȂ���ʎ��Ԃ�����
			if(g_item[g_get].among == true && g_type_old == g_item[g_get].type){
				
				//���Ԃ�����
				g_item[g_get].e_time += g_item[g_get].ch.time;
			}

			//�^�C�v���قȂ�Ȃ�A�Ή��������ʎ��Ԃ���
			if(g_type_old != g_item[g_get].type){

				//���Ԃ�������
				g_item[g_get].e_time = g_item[g_get].ch.time;
				
			}
			
			//���ʂ̕���
			pstreng[g_item[g_get].type]();
			
			//�g�p�ς݂̃A�C�e���ԍ����擾
			g_type_old = g_item[g_get].type;
			
			//����ς݃t���OOFF
			g_item[g_get].have = false;	
			
			//�������Ă���v�f�����Z�b�g
			g_get = ITEM_VACANT;
		}
	}
}

//************************************************************************************************
//�o�ߎ��Ԃɂ���ăA�C�e���̌��ʂ�����������֐�
//************************************************************************************************
void CheckResetEffect(){
	
	//�A�C�e���̌��ʒ��Ȃ玞�Ԃ��v������
	if(g_item[g_among].among == true){

		//���Ԍo��
		g_item[g_among].e_time--;
	}

	//�o�ߎ��ԂƃA�C�e�����̌��ʎ��Ԃ��ׂāA
	//�o�ߎ��Ԃ������Ă�����A�v���C���[�̏�Ԃ����Z�b�g���A���Ԃ����Z�b�g
	if(g_item[g_among].e_time < 0){

		//���݂̃^�C�v�������l��
		g_item[g_among].type = ITEM_VACANT;

		//���݂̃^�C�v�̗v�f�ԍ������Z�b�g
		g_among = ITEM_VACANT;

		//���Ԃ����Z�b�g
		g_item[g_among].e_time = 0;

		//���ʒ��t���O��OFF
		g_item[g_among].among = false;

		//�v���C���[�̕t�����ʂ�����
		OrdinarySwimer();
	}

}

//************************************************************************************************
//�A�C�e���̏����擾����
//************************************************************************************************
ITEM* GetItem(){ return g_item; }


