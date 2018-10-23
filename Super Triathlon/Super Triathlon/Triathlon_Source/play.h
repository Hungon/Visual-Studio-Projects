#ifndef __PLAY_H__
#define __PLAY_H__

//���}�N���̒�`


//��Փx�ݒ�
enum tagGAMELEVEL{

	 EASY_MODE   ,		//EASY���[�h
	 NORMAL_MODE ,		//NORMAL���[�h
	 HARD_MODE   ,		//HARD���[�h
	 LEVEL_MAX	 ,		//���x���ݒ�̑���
};
	

//���w�b�_�[���C���N���[�h
#include	"graphics.h"
#include	"SelectStage.h"


//������֐��̐錾
void InitPlay(HWND hWnd);		//������
void UpdatePlay();				//�X�V
void DrawPlay();			//�\��
void ReleasePlay();				//���
//�X�e�[�W�ԍ��ύX
void ChangeStage( tagSELECTSTAGE select );
//���݂̃X�e�[�W�ԍ����擾����
tagSELECTSTAGE	 GetStage();
//�v���C��������
void Playing(bool play);
//�v���C�������擾����
bool GetPlay();
//��Փx��ݒ�
void SetGameLevel(tagGAMELEVEL level);
//��Փx���擾
tagGAMELEVEL GetGameLevel();
//���[�X���X�^�[�g����n�߂�
void RestartRace();


#endif
