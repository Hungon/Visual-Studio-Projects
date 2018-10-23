#ifndef		CROADSTAGE_H__
#define		CROADSTAGE_H__


//���w�b�_�[�C���N���[�h
#include	"graphics.h"
#include	"map.h"

//it is road's margin that subtract road's bottom from road's top
const float	ROADSTAGE_MARGIN_WIDTH_FOR_TOP_AND_BOTTOM_IN_ROAD = 105.0f;
//it's subtlty position acoording to road's width
const float	ROADSTAGE_SUBTLTY_POSITIONX_FOR_ROAD_WIDTH = 140.0f;

//�� �O���錾
class	CPlayerRunner;

//�X�e�[�W�N���X
class	CRoadStage{

private:

	//�摜�̗v�f
	int		*m_lpRoadStageImg;

	//�w�i�摜������
	STAGE	*m_lpRoadBackGround;

	//�I�u�W�F�N�g�̏�������
	STAGE	*m_lpRoadObject;

	//for railings
	CHARACTER	*m_lpRailings;

	//�X�e�[�W�͈̔͂�����
	float m_RoadStageBottom;		//����
	float m_RoadStageTop;			//���
	float m_RoadStageWidth;			//��
	static float m_RoadStageHeight;	//����

	//�v���C���[�N���X
	CPlayerRunner	*m_lpCPlayerRunner;


protected:

public:

	//�� �ÓI�����o�ϐ��̒�`
	static HWND	m_hWnd;
	static const int  ROADSTAGE_IMAGE_MAX;		//�g�p����
	static const int  ROADSTAGE_BACK_GROUND_MAX;//�w�i�̐�
	static const int  ROADSTAGE_OBJECT_MAX;		//�I�u�W�F�N�g��������

	//�X�e�[�W�̋���
	static const float  ROADSTAGE_DISTANCE_EAZY;		//EAZY���[�h
	static const float  ROADSTAGE_DISTANCE_NORMAL;	//NORMAL���[�h
	static const float  ROADSTAGE_DISTANCE_HARD;		//HARD���[�h

	//���H�̓����i�s�N�Z���j
	static const float  ROADSTAGE_THE_WIDTH_OF_A_ROAD;	//��ʏ�[�̓���

	//�����̏����l������
	static const int	ROADSTAGE_OBJECT_WHITE_LINE_WIDTH;				//��
	static const int	ROADSTAGE_OBJECT_WHITE_LINE_HEIGHT;				//����
	static const float	ROADSTAGE_OBJECT_WHITE_LINE_INDICATE_SPACE_Y;	//�\���ԊuY
	static const float  ROADSTAGE_OBJECT_WHITE_LINE_DEFAULT_SCALE;		//�g�k��
	static const int	ROADSTAGE_OBJECT_WHITE_LINE_MAX;				//�����̐�

	//setting railings
	static const int	ROADSTAGE_IMAGE_RAILING_WIDTH;
	static const int	ROADSTAGE_IMAGE_RAILING_HEIGHT;
	static const float	ROADSTAGE_IMAGE_RAILING_DEFAULT_SCALE;
	static const int	ROADSTAGE_IMAGE_RAILING_MAX;

	//�� ����֐��̒�`
	
	CRoadStage();
	~CRoadStage();

	//����X�e�[�W�̏�����
	void InitRoadStage();
	//���H�̍X�V
	void UpdateRoadStage();
	//����X�e�[�W�̕\��
	void DrawRoadStage();
	//���
	void ReleaseRoadStage();


	//�e��擾�p�֐�
	//****************************************************************************************************
	//�X�e�[�W�̑S�̋������擾
	//****************************************************************************************************
	float GetRoadStageDistance(){ return this->m_RoadStageHeight; }


};

#endif
