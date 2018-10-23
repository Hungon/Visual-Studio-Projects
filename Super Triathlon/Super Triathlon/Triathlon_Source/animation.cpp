#include "animation.h"

//���O���[�o���ϐ�


//***************************************************************************************************
//�L�����N�^�[�ɃA�j���[�V��������ݒ�
//***************************************************************************************************
void SetAnimation(CHARACTER* ch , ANIMATION* ani , int sx , int sy , int max , int fram , int type , bool flag){
	
	//�D��x�ɉ���������or�����I�ȕύX
	if((ani -> type < type) || (flag == true)){

		//���������ƂɃA�j���[�V��������ݒ�
		ani->sx = sx;
		ani->sy = sy;
		ani->max = max;
		ani->fram = fram;
		ani->time = 0;
		ani->cnt = 0;
		//�L�����N�^�[�̕\�������X�V
		ch->picx = sx;
		ch->picy = sy +(ch->direction * ch->h);
		//��ނ�������
		ani->type = type;
	}	
}

//***************************************************************************************************	
//�L�����N�^�[�̃A�j���[�V�������X�V
//***************************************************************************************************
bool UpdateAnimation( CHARACTER* ch , ANIMATION* ani ){

	bool ret = true;
	
	//�o�ߎ��Ԃ����Z
	ani->time++;

	//�o�ߎ��Ԃ��P�R�}�ڂ̎��Ԃ��z���������m�F
	if( ani->time > ani->fram ){
	
		ani->cnt++;		//���̃R�}��
		ani->time = 0;	//���Ԃ����Z�b�g
	}

	//�ő�̃R�}�𒴂��������m�F
	if( ani->cnt >= ani->max ){

		ani->cnt = 0;	//1�R�}�ڂ֖߂�
		ret = false;	//�A�j���[�V�����̍Ō�̃R�}�܂œ��B������false��Ԃ�
	}	

	//�\�������W�̍X�V
	ch->picx = ani->sx + ( ani->cnt * ch->w );
	ch->picy = ani->sy + ( ch->direction * ch->h );

	return ret;	//�ŏI�I�ȃt���O��Ԃ�

}

/*
**************************************************************************************************	
�L�����N�^�[�̃A�j���[�V�������X�V
**************************************************************************************************
*/
bool UpdateReverseAnimation( CHARACTER* ch , ANIMATION* ani ){

	//�߂�l�p
	bool ret = true;
	
	//�o�ߎ��Ԃ����Z
	ani->time++;

	//���Z�A�j���[�V����
	if( ani->reverseF == false ){
		//���Z�A�j���[�V��������
		//�o�ߎ��Ԃ��P�R�}�ڂ̎��Ԃ��z���������m�F
		if( ani->time > ani->fram ){
			
			ani->cnt++;		//���̃R�}��
			ani->time = 0;	//���Ԃ����Z�b�g
		}

		//�ő�̃R�}�𒴂����甽�΃A�j���[�V�����t���O�ɂ���
		if( ani->cnt >= ani->max )	ani->reverseF = true;

		//�\�������W�̍X�V
		ch->picx = ani->sx + ( ani->cnt * ch->w );
		ch->picy = ani->sy + ( ch->direction * ch->h );

		return ret;

	}
	
	//���Z�A�j���[�V��������
	if( ani->reverseF ){
		if( ani->time > ani->fram ){
			
			ani->cnt--;		//���̃R�}��
			ani->time = 0;	//���Ԃ����Z�b�g
		}

		//���΃A�j���[�V�������ŁA�A�j���[�V�����̎n�߂̃R�}�܂œ��B������false��Ԃ�
		if( ani->cnt <= 0 )  ret = false;

		//�\�������W�̍X�V
		ch->picx = ani->sx + ( ani->cnt * ch->w );
		ch->picy = ani->sy + ( ch->direction * ch->h );
	}

	//���΃t���O���X�V
	ani->reverseF = ret;
	return ret;	//�ŏI�I�ȃt���O��Ԃ�

}

	