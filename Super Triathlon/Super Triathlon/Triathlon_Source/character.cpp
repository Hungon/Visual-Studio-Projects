#include "character.h"
#include "map.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;


//���@�O���[�o���ϐ��̐錾

//************************************************************************************************
//�����蔻��@CollisionCharacter
//************************************************************************************************
bool CollisionCharacter(CHARACTER* pch1 , CHARACTER* pch2){


	//2�̃L�����N�^�[�̏d�Ȃ�𒲂ׂ�
	if((pch1->x - fabs(pch1->mx)) < pch2->x + (float)(pch2->w * pch2->scale) &&
	   (pch1->x + (float)(pch1->w * pch1->scale)) + fabs(pch1->mx) > pch2->x &&
 	   (pch1->y + fabs(pch1->my)) < pch2->y + (float)(pch2->h  * pch2->scale) &&
	   (pch1->y + (float)(pch1->h  * pch1->scale)) - fabs(pch1->my) > pch2->y ){		
		//�d�Ȃ��Ă�����true��Ԃ�
		return true;
	}
	//�d�Ȃ��Ă��Ȃ�������false��Ԃ�
	return false;
}


//************************************************************************************************
//��莞�Ԃ̊Ԃ����A�G�Ƃ̔�������Ȃ�
//************************************************************************************************
bool AvoidEnemy(CHARACTER* pch , int avoidtime){

	//�G����𒆂��ǂ�����Ԃ��ϐ�
	bool ret = false;

	//�����t���O��false�Ȃ�A���Ԃ��v��
	//���x���f�t�H���g�Ƀ��Z�b�g
	if(pch->flag == false){
		//�o�ߎ���
		pch->time++;
		//��~��Ԃɂ���
		pch->speed = 0;
		//��𒆂ł��邱�Ƃ�Ԃ�
		ret = true;
	}

	//�o�ߎ��Ԃ������̃t���[�����𒴂�����true�ɁA�o�ߎ��Ԃ����Z�b�g����
	if(pch->time > avoidtime){
		//�����t���O
		pch->flag = true;
		//���Ԃ����Z�b�g
		pch->time = 0;
		//���x�{�������ɖ߂�
		pch->speed = pch->defaultSpeed;
		//����t���O������
		ret = false;
	}

	//�ŏI�I�ɉ�𒆂��ǂ�����Ԃ�
	return ret;

}


//***********************************************************************************************
//�����̒��S���W�����߂�
//***********************************************************************************************
void CenterCharacter(CHARACTER* ch , int len , int angle){

	//��]���S���Z�o
	int cx = (int)ch->sx + (ch->w >> 1);
	int cy = (int)ch->sy + (ch->h >> 1);

	//��]��̍��W�����߂�
	ch->x = (float)cos(angle * 3.14 / 180.0) * len + cx - (ch->w >> 1);
	ch->y = (float)sin(angle * 3.14 / 180.0) * len + cy - (ch->h >> 1);
}


//************************************************************************************************
//�������x�W�F�Ȑ��i�����̃x�W�F�̒l�����Ȑ���`���j
//************************************************************************************************
void SetBezier(CHARACTER* ch , POINT* pos , int bezier , float time){
	
	int i = 0;

	//��X���W
	//�n�_���W�����߂�
	float sx = pos[0].x * (float)(pow((1 - time) , bezier));
	//���ԍ��W
	float cx = 0.0f;
	//�I�_���W�����߂�
	float ex = pos[bezier].x * (float)(pow(time , bezier));			
	
	//��Y���W
	//�n�_���W�����߂�
	float sy = pos[0].y * (float)(pow((1 - time) , bezier));
	//���ԍ��W
	float cy = 0.0f;
	//�I�_���W�����߂�
	float ey = pos[bezier].y * (float)(pow(time , bezier));					

	//�n�_����
	ch->x = sx;			//X
	ch->y = sy;			//Y

	//���S���W�����߂�
	for(i = 1;i < bezier;i++){
		//X���W
		ch->x += pos[i].x * bezier * (float)(pow((1 - time) , (bezier - i)))
			* (float)(pow(time , i));
		//Y���W
		ch->y += pos[i].y * bezier * (float)(pow((1 - time) , (bezier - i)))
			* (float)(pow(time , i));
	}
	//�I�_���Ō�ɉ��Z
	ch->x += ex;		//X
	ch->y += ey;		//Y
}


//************************************************************************************************
//�n���ꂽ�Q�̒l����A�͈͓��Ń����_���̒l��Ԃ�
//************************************************************************************************
int RandomNum(int max,int min){

	int r = 0;
	
	//min�`max�͈̔͂ŗ������擾
	r = rand() % ( max - min + 1 ) + min;
	//�n���ꂽ�l�͈͓̔��̃����_���̒l��Ԃ�
	return r;
}


//************************************************************************************************
//�L�����N�^�[���m�̋�`���d�Ȃ��Ă�����A�����ɉ����č��W���C������i�d�Ȃ�Ȃ��悤�ɂ���j
//************************************************************************************************
BYTE CheckOverlapCharacter( CHARACTER* pch1 , CHARACTER* pch2 , TSAFETYRECT<int> safetyRect ){

	//�߂�l�p
	BYTE	ret = CHARACTER_NOT_BUMP;

	//���������g�k�{���ʂ�Ɍv�Z
	//�L�����N�^�[1
	float chara1RectW = pch1->w * pch1->scale;			//��
	float chara1RectH = pch1->h * pch1->scale;			//����

	//�L�����N�^�[�Q
	float chara2RectW = pch2->w * pch2->scale;			//��
	float chara2RectH = pch2->h * pch2->scale;			//����

	//pch1�̍��[��pch2�̉E�[�̋�`�Ŕ���
	if( ( pch1->x + safetyRect.left ) < pch2->x + chara2RectW &&
		( pch1->x + safetyRect.left ) > pch2->x &&
		( pch1->y + safetyRect.up ) < pch2->y + chara2RectH &&
		( pch1->y + chara1RectH ) - safetyRect.down > pch2->y ){

		//pch2��pch1�̍��[�ɂ��������ʒu�ɏC������
		pch2->x = ( pch1->x - chara2RectW ) + safetyRect.left;

		//�Փ˂����ʂ�Ԃ�
		ret = CHARACTER_BUMP_LEFT;
	}				
/*
	//pch1�̏�[��pch2�̉��[�̋�`�Ŕ���
	if( ( pch1->x + chara1RectW ) - safetyRect.right > pch2->x &&
		( pch1->x + safetyRect.left ) < pch2->x + chara2RectW &&
		( pch1->y + safetyRect.up ) > pch2->y &&
		( pch1->y + safetyRect.up ) < pch2->y + chara2RectH ){
		

		//pch2��pch1�̏�[�ɂ��������ʒu�ɏC������
		pch2->y = ( pch1->y - chara2RectH ) + safetyRect.up;

		//�Փ˂����ʂ�Ԃ�
		ret = CHARACTER_BUMP_TOP;
	}

*/
	//pch1�̉E�[��pch2�̍��[�̋�`�Ŕ���
	if( ( pch1->x + chara1RectW ) - safetyRect.right < pch2->x + chara2RectW &&
		( pch1->x + chara1RectW ) - safetyRect.right > pch2->x &&
		( pch1->y + safetyRect.up ) < pch2->y + chara2RectH &&
		( pch1->y + chara1RectH ) - safetyRect.down > pch2->y ){

		//pch2��pch1�̉E�[�ɂ��������ʒu�ɏC������
		pch2->x = ( pch1->x + chara1RectW ) - safetyRect.right;

		//�Փ˂����ʂ�Ԃ�
		ret = CHARACTER_BUMP_RIGHT;
	}

	//pch1�̉��[��pch2�̏�[�̋�`�Ŕ���
	if( ( pch1->x + chara1RectW ) - safetyRect.right > pch2->x &&
		( pch1->x + safetyRect.left ) < pch2->x + chara2RectW &&
		( pch1->y + chara1RectH ) - safetyRect.down > pch2->y &&
		( pch1->y + chara1RectH ) - safetyRect.down < pch2->y + chara2RectH ){

		//pch2��pch1�̉��[�ɂ��������ʒu�ɏC������
		pch2->y = ( pch1->y + chara1RectH ) - safetyRect.down;

		//�Փ˂����ʂ�Ԃ�
		ret = CHARACTER_BUMP_BOTTOM;
	}

	//�Փ˂����ʂ�Ԃ�
	return ret;

}


//************************************************************************************************
//�����̃L�����N�^�[�̍��G�͈͂ƈ����̃L�����N�^�[�̋�`�Ŕ�������@BroadenCollisionSquare
//�����P�@���@���g�̋�`�Ŕ�������L�����N�^�[
//�����Q�@���@���g�̎���ɔ�����L����L�����N�^�[
//************************************************************************************************
bool BroadenCollisionSquare( CHARACTER* pch1 , CHARACTER* pch2 ){


	//�����Q�̒��S���W�����߂�
	float pch2CenterX = pch2->x + ( ( pch2->w * pch2->scale ) / 2 );		//X���W
	float pch2CenterY = pch2->y + ( ( pch2->h * pch2->scale ) / 2 );		//Y���W

	//���G�͈͂��v�Z�p�ɕϊ�����
	int pch2RectW = pch2->SearchSquareWidth / 2;							//��

	//����1�̃L�����N�^�[�̋�`�ƈ���2�̃L�����N�^�[���L������G�͈͂ƂŔ�������
	//���G�͈͂��L����L�����N�^�[�͒��S���W����_�Ƃ���
	if( pch1->x < pch2CenterX + (float)pch2RectW &&
	   pch1->x + (float)( pch1->w * pch1->scale ) > pch2CenterX - (float)pch2RectW &&
	   pch1->y < pch2CenterY + (float)pch2->SearchSquareFront &&
	   pch1->y + (float)( pch1->h  * pch1->scale ) > pch2->y ){		
	 
		  //�d�Ȃ��Ă�����true��Ԃ�
		  return true;
	}

	//�d�Ȃ��Ă��Ȃ�������false��Ԃ�
	return false;

}

//************************************************************************************************
//����J�������_�ɍ��킹�Ĉ����̃L�����N�^�[�̊g�k����K�����l�ɕύX����
//************************************************************************************************
void FittingPositionToBackCameraAngle( CHARACTER* pch , float scaleMax ){

	//�J�����̍��W���擾
	float comeCameraY = GetCameraY();

	//pch2�̃��[�J�����W�����߂�
	float localCameraY = pch->y - comeCameraY;

	//���@��O�ɗ���قǂɑ傫�������鏈��

	//pch2�̃��[�J�����W���c��ʃT�C�Y�́A
	//�ǂ̊����Ɉʒu���Ă��邩���v�Z
	float localRateY = localCameraY / 480.0f;

	//��ʏ�[����o�Ă���g�k���ɉ��Z���Ă���
	if( pch->y + ( pch->h * pch->scale ) >= comeCameraY ){
		//���݂̈ʒu�ɉ����đΉ�����g�k������
		pch->scale = pch->defaultScale + ( scaleMax * localRateY );
	}
	//����l
	if( pch->scale >= pch->defaultScale + scaleMax ){
		//��ʏ�[�̓��H�̓����Ɖ�ʉ��[�̓����̍����{���𓙔{�ɉ��Z
		pch->scale = pch->defaultScale + scaleMax;
	}

}


/**************************************************************************************************
�����̒l�̊m����true���擾����
�����P�@�F�@�擾�������m���̐����i1�`100�܂Łj
******************************************************************************************************/
bool GetLikelihood( char num ){

	//�����_���̒l���擾����
	char compairNum = RandomNum( 100 , 1 );

	//�߂�l�p
	bool ret = false;

	//�擾���������_���̒l�������ȉ��Ȃ�true��Ԃ�
	if( compairNum <= num ){

		ret = true;
	}

	//�ŏI�I�Ȍ��ʂ�Ԃ�
	return ret;
}
/*
Inventor replaces material with Host's material, that Host make space for inventor
return is A replaed value with B, that matrix of materials
*/
int*	ReplaceValue(int inventor, int replaceMaterial, int* lpHost, int hostMax) {
	
	int replace = 0;
	//count
	int next = 0;
	//next item
	next = (replaceMaterial + 1);
	//subsutitute value for matrix that starting point of replace process
	lpHost[replaceMaterial] = inventor;
	for (int j = replaceMaterial; j < hostMax; j++) {		//loop to hostMax
		replace = lpHost[j];
		if (next < hostMax) {
			//replace material with lpHost[]			
			lpHost[j] = lpHost[next];
			lpHost[next] = replace;
		}
		next = (j + 2);						//this is subsutitution material
	}
	return lpHost;
}