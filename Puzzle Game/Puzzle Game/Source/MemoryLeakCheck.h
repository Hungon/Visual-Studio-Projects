// MemoryLeakCheck.h
//  ���������[�N�𒲂ׂ邽�߂̋L�q���܂Ƃ߂��w�b�_�[�t�@�C���ł��B
//  ���������[�N�����Ă��邩�𒲂ׂ����\�[�X�t�@�C���ŃC���N���[�h���Ă��������B
#define		_CRTDBG_MAP_ALLOC
#include	<stdlib.h>
#include	<crtdbg.h>

// new���Z�q�Ē�`
/*
#ifdef		_DEBUG
   #ifndef		DBG_NEW
      #define		DBG_NEW		new ( _NORMAL_BLOCK, __FILE__, __LINE__ )
      #define		new			DBG_NEW
   #endif
#endif
*/
/* �ȉ��̓��e���X�^�[�g�A�b�v���[�`��( WinMain / main�֐� )�̐擪�ɒǉ�����I
#if	_DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
*/
