#ifndef OFFROADSTAGE_H__
#define OFFROADSTAGE_H__


//�� �w�b�_�[�̃C���N���[�h
#include	"graphics.h"
#include	<windows.h>

//�� �O���錾
class COffRoadRacer;

//���I�t���[�h�X�e�[�W�N���X�̒�`
class COffRoadStage{

private:

	//�摜�̗v�f������
	int		*m_lpCOffRoadImg;

	//�w�i�摜�ݒ�p
	CHARACTER		*m_lpOffRoad;

	//�W�����v��ݒ�p
	CHARACTER		*m_lpOffRoadJumpPoint;

	//for brief map
	CHARACTER	*m_lpMinimap;

	//�W�����v��擾�p
	static CHARACTER		*m_lpOffRoadGetJumpPoint;

	//���C�_�[�N���X
	COffRoadRacer	*m_lpCOffRoadRacer;

	//�X�e�[�W�̋���������
	float		m_distance;
	static float m_wholeDistance;

	//�W�����v��̐����Ԋu������
	int		m_CreatrionFram;


protected:


public:

	//�� �ÓI�����o�ϐ��̒�`
	static const int OFFROADSTAGE_IMAGE_MAX;				//�摜�g�p����

	//�w�i�̃T�C�Y
	//�n��
	static const int OFFROADSTAGE_BACK_IMAGE_GROUND_WIDTH;			//��
	static const int OFFROADSTAGE_BACK_IMAGE_GROUND_HEIGHT;			//����
	//�S�[���n�_
	static const int OFFROADSTAGE_BACK_IMAGE_GOAL_POINT_WIDTH;		//��
	static const int OFFROADSTAGE_BACK_IMAGE_GOAL_POINT_HEIGHT;		//����

	//�w�i�̎�ސ�
	static const int OFFROADSTAGE_BACK_IMAGE_KIND_OF_TYPE;

	//�W�����v�|�C���g�̃T�C�Y
	static const int OFFROADSTAGE_JUMP_POINT_IMAGE_WIDTH;	//��
	static const int OFFROADSTAGE_JUMP_POINT_IMAGE_HEIGHT;	//����
	//�g�k�{�������l
	static const float OFFROADSTAGE_JUMP_POINT_DEFAULT_SCALE;
	//�\���w�̊Ԋu
	static const int OFFROADSTAGE_JUMP_POINT_INDICATE_X_SPACE;
	//�\���w�̔z�u�ő吔
	static const int OFFROADSTAGE_JUMP_POINT_INDICATE_X_POSITION_MAX;
	//�z�u�ő吔
	static const int OFFROADSTAGE_JUMP_POINT_POSITION_MAX;

	//setting brief map
	static const int OFFROADSTAGE_IMAGE_MINIMAP_WIDTH;						//for back ground
	static const int OFFROADSTAGE_IMAGE_MINIMAP_HEIGHT;
	static const float OFFROADSTAGE_IMAGE_MINIMAP_INDICATE_X;
	static const float OFFROADSTAGE_IMAGE_MINIMAP_INDICATE_Y;
	static const float OFFROADSTAGE_IMAGE_MINIMAP_RACER_DEFAULT_SCALE;		//for minimum racer
	static const int OFFROADSTAGE_MINIMAP_KIND_OF_IMAGE;					//kind of image

	//�E�B���h�E�n���h��
	static HWND	m_hWnd;

	//�X�e�[�W�̋���
	static const int OFFROADSTAGE_DISTANCE_EAZY;	//EAZY���[�h
	static const int OFFROADSTAGE_DISTANCE_NORMAL;	//NORMAL���[�h
	static const int OFFROADSTAGE_DISTANCE_HARD;	//HARD���[�h

	//����̃X�e�[�W�̉�ʃT�C�Y
	static const int OFFROADSTAGE_SCREEN_WIDTH;		//��
	static const int OFFROADSTAGE_SCREEN_HEIGHT;	//����


	//�R���X�g���N�^
	COffRoadStage() :
	//�摜�̗v�f������
	m_lpCOffRoadImg(NULL) ,
	//�w�i�摜�ݒ�p
	m_lpOffRoad(NULL) ,
	//�W�����v��ݒ�p
	m_lpOffRoadJumpPoint(NULL) ,
	//���C�_�[�N���X
	m_lpCOffRoadRacer(NULL) 
	{}

	//�f�B�X�g���N�^
	~COffRoadStage();
	
	//�� ���상���o�֐��̒�`
	//����X�e�[�W�̏�����
	void InitOffRoadStage();
	//���H�̍X�V
	void UpdateOffRoadStage();
	//����X�e�[�W�̕\��
	void DrawOffRoadStage();
	//���
	void ReleaseOffRoadStage();
	//�X�e�[�W�̑S����Ԃ�
	float GetOffRoadStageDistance() const { return this->m_wholeDistance; }
	/*
	�W�����v�n�_�̐���
	*/
	void CreateJumpPoint();
	/*
	�W�����v�n�_�ƃv���C���[�̏d�Ȃ�𒲂ׂ�
	*/
	bool	CollisionJumpPoint( CHARACTER *pch );

	//���ݏo�����̃W�����v�n�_����ݒ肷��
	void	SetJumpPointInfo( CHARACTER* pJump , int element ){ *( this->m_lpOffRoadGetJumpPoint + element ) = *( pJump ); }


};


#endif
