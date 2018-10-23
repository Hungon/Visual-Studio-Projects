#ifndef __INPUT_H__


//���}�N���̒�`
#define __INPUT_H__
//�{�^���̃}�N��
#define BUTTON_1 0x01
#define BUTTON_2 0x02
#define BUTTON_3 0x04
#define BUTTON_4 0x08
#define BUTTON_5 0x10
#define BUTTON_6 0x20
#define BUTTON_7 0x40
#define BUTTON_8 0x80

//�\���L�[�̃}�N��
#define LEVER_LEFT	0x01
#define LEVER_UP	0x02
#define LEVER_RIGHT 0x04
#define LEVER_DOWN	0x08

//���w�b�_�[�t�@�C���̒�`
#include <windows.h>


//������֐����`

//�v���^�C�v�錾
void UpdateInput();					//���͏󋵂̍X�V
bool JustUpKey(BYTE code);			//�L�[�𗣂����u�Ԃ𒲂ׂ�֐�
bool KeepKey(BYTE code);			//�L�[���������ςȂ����𒲂ׂ�֐�
bool JustKey(BYTE code);			//�L�[���������u�Ԃ��𒲂ׂ�֐�
bool JustButton(int button);		//�{�^�����͂̐ݒ�
bool KeepButton(int button);		//�{�^���̉������ςȂ�
bool JustUpButton(int buton);		//�{�^���𗣂����u��
bool JustLever(int lever);			//�\���L�[�������ꂽ�u�Ԃ𒲂ׂ�
bool KeepLever(int lever);			//�\���L�[���������ςȂ����𒲂ׂ�
bool JustUpLever(int lever);		//�\���L�[�𗣂����u�Ԃ𒲂ׂ�
/*
����̓��̓R�[�h��Ԃ�
*/
BYTE	GetInputKeyCode();

#endif
