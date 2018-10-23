//���C�u�����̒ǉ�
#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"winmm.lib")

#include "sound.h"

//�O���[�o���ϐ�
STREAMING g_st = {0};		//�X�g���[�~���O�Đ��p�f�[�^
//Direct�I�u�W�F�N�g
LPDIRECTSOUND g_lpDS = NULL;
//�v���C�}���o�b�t�@
LPDIRECTSOUNDBUFFER g_lpPrimaryBuffer = NULL;
//�Z�J���_���o�b�t�@
LPDIRECTSOUNDBUFFER g_lpSoundBuffer[SOUND_MAX] = {0};


//***********************************************************************************************
//������
//***********************************************************************************************

bool InitDirectSound(HWND hWnd){

	//�ϐ�
	//�G���[�`�F�b�N
	HRESULT hRet = NULL;
	//�v���C�}���o�b�t�@�����p
	DSBUFFERDESC dsbd = {0};
	//WAVE�t�H�[�}�b�g���
	WAVEFORMATEX wfx = {0};

	//���@�@DirectSound�I�u�W�F�N�g�̐���
	hRet = DirectSoundCreate(NULL,&g_lpDS,NULL);

	//�G���[�`�F�b�N
	if(FAILED(hRet)){
		return false;
	}

	//���A�@�������x���̐ݒ�
	hRet = g_lpDS->SetCooperativeLevel(hWnd,DSSCL_PRIORITY);
	
	//�G���[�`�F�b�N
	if(FAILED(hRet)){
		return false;
	}

	//���B�@�v���C�}���o�b�t�@����
	//�v���C�}���o�b�t�@�̏��ݒ�
	dsbd.dwSize = (DWORD)sizeof(DSBUFFERDESC);
	//�v���C�}���o�b�t�@�̐����ݒ�
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME |
					DSBCAPS_PRIMARYBUFFER;

	//�v���C�}���o�b�t�@�̐���
	hRet = g_lpDS->CreateSoundBuffer(&dsbd,&g_lpPrimaryBuffer,NULL);
	
	//�G���[�`�F�b�N
	if(FAILED(hRet)){
		return false;
	}


	//���C�@�v���C�}���o�b�t�@��WAVE�t�H�[�}�b�g�̐ݒ�

	//WAVE�t�H�[�}�b�g�ݒ�
	//�T�C�Y�ݒ�
	wfx.cbSize = sizeof(WAVEFORMATEX);
	//�I�[�f�B�I�t�H�[�}�b�g�̃^�C�v
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	//�P���w��B���m�����Q�F�X�e���I
	wfx.nChannels = 2;
	//44100�����w��iHz�j
	wfx.nSamplesPerSec = 44100;
	//�W���P�U���w��
	wfx.wBitsPerSample = 16;
	//�f�[�^�̍ŏ��l
	wfx.nBlockAlign = wfx.wBitsPerSample * wfx.nChannels / 8;
	//1�b������̃o�C�g��
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	
	//�G���[�`�F�b�N
	if(FAILED(hRet)){
		return false;
	}
	return true;
}


//***********************************************************************************************
//�Ȃ̓ǂݍ���
//***********************************************************************************************
int CreateSound(char* pass){

	//���D�@WAVE�t�@�C���̓ǂݍ���
	DSBUFFERDESC dsbd = {0};
	WAVEFORMATEX wfx = {0};
	LRESULT lRet = {0};
	HRESULT hRet = {0};
	HMMIO hmmio = NULL;
	MMCKINFO Parent = {0};
	MMCKINFO Child = {0};
	LPVOID p = NULL;
	DWORD size = 0;
	int ret = -1;
	int i = 0;

	//WAVE�t�@�C���̓ǂݍ��݁iMMIO�n���h���擾�j
	hmmio = mmioOpen(pass,NULL,MMIO_READ);
	
	//�G���[�`�F�b�N
	if(!hmmio){
		return false;
	}

	//WAVE�t�@�C�����ǂ������m�F
	//RIFF�`�����N�ɐi��
	memset(&Parent,0,sizeof(MMCKINFO));
	Parent.ckid = mmioFOURCC('R','I','F','F');
	Parent.fccType = mmioFOURCC('W','A','V','E');
	if(mmioDescend(hmmio,&Parent,NULL,MMIO_FINDRIFF) == MMIOERR_CHUNKNOTFOUND){
		mmioClose(hmmio,0);
		return false;
	}

	//�t�H�[�}�b�g�ifmt�j�`�����N�ɐi��
	memset(&Child,0,sizeof(MMCKINFO));
	Child.ckid = mmioFOURCC('f','m','t',' ');
	if(mmioDescend(hmmio,&Child,&Parent,MMIO_FINDCHUNK) == MMIOERR_CHUNKNOTFOUND){
		mmioClose(hmmio,0);
		return false;
	}


	//WAVE�t�H�[�}�b�g�擾
	if(mmioRead(hmmio,(HPSTR)&wfx,sizeof(WAVEFORMATEX)) == -1){
		mmioClose(hmmio,0);
		return false;
	}

	//FMT�`�����N�A�Z���h
	if(mmioAscend(hmmio,&Child,NULL)){
		mmioClose(hmmio,0);
		return false;
	}

	//�q�`�����N�A�Z���hdata�Ƀf�B�Z���h
	Child.ckid = mmioFOURCC('d','a','t','a');
	if(mmioDescend(hmmio,&Child,&Parent,MMIO_FINDCHUNK) == MMIOERR_CHUNKNOTFOUND){
		mmioClose(hmmio,0);
		return false;
	}

	//�Z�J���_���o�b�t�@���\���̐ݒ�
	//�T�C�Y�ݒ�
	memset(&dsbd,0,sizeof(DSBUFFERDESC));
	//�Đ��J�[�\���̋L���ʒu
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	//�{�����[���ݒ��
	dsbd.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = Child.cksize;
	//WAVE�t�H�[�}�b�g�ݒ�
	dsbd.lpwfxFormat = &wfx;

	//��̗v�f�ɃZ�J���_���o�b�t�@�𐶐�
	for(i = 0;i < SOUND_MAX;i++){
		if(g_lpSoundBuffer[i] == NULL){
			//�o�b�t�@����
			hRet= g_lpDS->CreateSoundBuffer(&dsbd,&g_lpSoundBuffer[i],NULL);
			//�G���[�`�F�b�N
			if(FAILED(hRet)){
				mmioClose(hmmio,0);
				return false;
			}

			//�o�b�t�@�����b�N
			hRet = g_lpSoundBuffer[i]->Lock(0,Child.cksize,&p,&size,NULL,NULL,0);
			//�G���[�`�F�b�N
			if(FAILED(hRet)){
				mmioClose(hmmio,0);
				return false;
			}

			//���b�N�����o�b�t�@��WAVE�f�[�^��ǂݍ���
			lRet = mmioRead(hmmio,(char*)p,Child.cksize);
			//�G���[�`�F�b�N
			if(lRet < 0){
				mmioClose(hmmio,0);
				return false;
			}

			//�o�b�t�@���A�����b�N
			hRet = g_lpSoundBuffer[i]->Unlock(p,size,NULL,0);
			//�G���[�`�F�b�N
			if(FAILED(hRet)){
					mmioClose(hmmio,0);
				return false;
			}
			//�߂�l��ǂݍ��񂾗v�f�ԍ��ɐݒ�
			ret = i;
			//�Ȃ̓ǂݍ��݊���
			break;
		}
	}
	//MMIO�n���h�����
	mmioClose(hmmio,0);

	//�����F�ǂݍ��񂾗v�f�ԍ��A���s�F-1��Ԃ�
	return ret;

}

//***********************************************************************************************
//���
//***********************************************************************************************
void ReleaseDirectSound(){
	 
	int i = 0;

	//�X�g���[�~���O�̉��
	ReleaseStreaming();

	//���F�@�����������ԂƂ͋t���Ɋe�I�u�W�F�N�g�����
	//�Z�J���_���o�b�t�@���
	for(i = 0;i < SOUND_MAX;i++){
		if(g_lpSoundBuffer[i]){
			g_lpSoundBuffer[i]->Release();
			g_lpSoundBuffer[i] = NULL;
		}
	}

	//�v���C�}���o�b�t�@�����
	if(g_lpPrimaryBuffer){
		g_lpPrimaryBuffer->Release();
		g_lpPrimaryBuffer = NULL;
	}

	//DirectSound�I�u�W�F�N�g�̉��
	if(g_lpDS){
		g_lpDS->Release();
		g_lpDS = NULL;
	}
}

//***********************************************************************************************
//���y�̍Đ�
//***********************************************************************************************
void PlaySound(int id,int bigintime,bool loop){

	//�G���[�`�F�b�N
	if(g_lpSoundBuffer[id] == NULL) return;

	//�Đ��J�[�\����擪�ʒu��
	g_lpSoundBuffer[id]->SetCurrentPosition(SECOND * bigintime);
	
	//���@�Đ�
	//���[�v�w�聫
	if(loop == true){
		g_lpSoundBuffer[id]->Play(0,0,DSBPLAY_LOOPING);
	}
	//1��̂݁�
	if(loop == false){
		g_lpSoundBuffer[id]->Play(0,0,0);
	}
}

//***********************************************************************************************
//���y�̒�~
//***********************************************************************************************
void StopSound(int id){
	
	//�G���[�`�F�b�N
	if(g_lpSoundBuffer[id] == NULL) return;
	
	//��~
	g_lpSoundBuffer[id]->Stop();
}

//***********************************************************************************************
//�w�肵���Ȃ݂̂̉��
//***********************************************************************************************
void ReleaseSound(int id){

	//�Z�J���_���o�b�t�@�����
	if(g_lpSoundBuffer[id]){
		g_lpSoundBuffer[id]->Release();
		g_lpSoundBuffer[id] = NULL;

	}
}

//***********************************************************************************************
//�i�X�g���[�~���O�j2�b���̃Z�J���_���o�b�t�@�̐���
//***********************************************************************************************
bool CreateStreaming(char* pass){

	DSBUFFERDESC dsbd = {0};
	WAVEFORMATEX wfx = {0};
	LRESULT lRet = {0};
	HRESULT hRet = {0};
	HMMIO hmmio = NULL;
	MMCKINFO Parent = {0};
	MMCKINFO Child = {0};
	LPVOID p = NULL;
	DWORD size = 0;
	int ret = -1;
	int i = 0;
	MMIOINFO mmioinfo = {0};

	//�G���[�`�F�b�N
	if(g_st.buf){
		return false;		//����2�b���̃o�b�t�@������Ȃ�I��
	}

	//�w���wav�t�@�C�����J��
	//WAVE�t�@�C���̓ǂݍ��݁iMMIO�n���h���擾�j
	hmmio = mmioOpen(pass,NULL,MMIO_READ);
	
	//�G���[�`�F�b�N
	if(!hmmio){
		return false;
	}

	//WAVE�t�@�C�����ǂ������m�F
	//RIFF�`�����N�ɐi��
	memset(&Parent,0,sizeof(MMCKINFO));
	Parent.ckid = mmioFOURCC('R','I','F','F');
	Parent.fccType = mmioFOURCC('W','A','V','E');
	if(mmioDescend(hmmio,&Parent,NULL,MMIO_FINDRIFF) == MMIOERR_CHUNKNOTFOUND){
		mmioClose(hmmio,0);
		return false;
	}
	//�t�H�[�}�b�g�ifmt�j�`�����N�ɐi��
	memset(&Child,0,sizeof(MMCKINFO));
	Child.ckid = mmioFOURCC('f','m','t',' ');
	if(mmioDescend(hmmio,&Child,&Parent,MMIO_FINDCHUNK) == MMIOERR_CHUNKNOTFOUND){
		mmioClose(hmmio,0);
		return false;
	}
	//WAVE�t�H�[�}�b�g�擾
	if(mmioRead(hmmio,(HPSTR)&wfx,sizeof(WAVEFORMATEX)) == -1){
		mmioClose(hmmio,0);
		return false;
	}
	//FMT�`�����N�A�Z���h
	if(mmioAscend(hmmio,&Child,NULL)){
		mmioClose(hmmio,0);
		return false;
	}
	//�q�`�����N�A�Z���hdata�Ƀf�B�Z���h
	Child.ckid = mmioFOURCC('d','a','t','a');
	if(mmioDescend(hmmio,&Child,&Parent,MMIO_FINDCHUNK) == MMIOERR_CHUNKNOTFOUND){
		mmioClose(hmmio,0);
		return false;
	}

	//�Z�J���_���o�b�t�@���\���̐ݒ�
	//�T�C�Y�ݒ�
	memset(&dsbd,0,sizeof(DSBUFFERDESC));
	//�Đ��J�[�\���̋L���ʒu
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	//�{�����[���ݒ��
	dsbd.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = Child.cksize;
	//WAVE�t�H�[�}�b�g�ݒ�
	dsbd.lpwfxFormat = &wfx;

	//�Z�J���_���o�b�t�@�̐���
	//2�b���̃o�b�t�@�����ׂ̐ݒ�
	memset(&dsbd , 0 , sizeof(DSBUFFERDESC));
	dsbd.dwSize = (DWORD)sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = wfx.nAvgBytesPerSec * 2;
	dsbd.lpwfxFormat = &wfx;

	//�ݒ����Ƀo�b�t�@�𐶐�
	hRet = g_lpDS->CreateSoundBuffer(&dsbd , &g_st.buf , NULL);

	//1�ȕ��̃T�C�Y���R�s�[
	g_st.musicSize = Child.cksize;
	//1�b���̃T�C�Y��WAVE�t�H�[�}�b�g����R�s�[
	g_st.oneSecondBytes = wfx.nAvgBytesPerSec;
	//�t�@�C���p�X�̃R�s�[
	strcpy(&g_st.pass[0],pass);
	//�ŏ��̓o�b�t�@�̑O��1�b�ɓǂݍ���
	g_st.fFront = true;
	//�ǂݍ��݃t���O�n�m
	g_st.fRead = true;

	//wav�t�@�C�������擾
	mmioGetInfo(hmmio , &mmioinfo , 0);
	//wav�t�@�C�����̉��f�[�^�J�n�ʒu���R�s�[
	g_st.startOffset = mmioinfo.lDiskOffset;
	//���̓ǂݍ��݈ʒu���R�s�[
	g_st.readOffset = mmioinfo.lDiskOffset;

	//wav�t�@�C�������
	mmioClose(hmmio,0);
	return true;
}

//***********************************************************************************************
//�i�X�g���[�~���O�j���̍Đ�
//***********************************************************************************************
void PlayStreaming( bool play ){

	//�J����wav�t�@�C��
	HMMIO hmmio = 0;	
	//���b�N�����|�C���^
	LPVOID p = NULL;
	//���b�N�����T�C�Y
	DWORD size = 0;
	//�Đ��J�[�\���̈ʒu
	DWORD pos = 0;
	//�͂ݏo�����o�C�g��
	DWORD over = 0;
	//���݂̍Đ��ʒu�̃o�C�g�����擾����
	MMIOINFO mmioinfo = {0};

	//2�b���̃o�b�t�@���Ȃ��Ȃ�I��
	if(g_st.buf == NULL){
		return;
	}

	//�Đ��t���O
	if(play){
		//�K�v�������1�b���̉�����������
		if(g_st.fRead){
			
			//�t�@�C�����J��
			hmmio = mmioOpen(&g_st.pass[0] , NULL , MMIO_READ);
			
			//wav�t�@�C������̓ǂݍ��݈ʒu�����炷
			mmioSeek(hmmio , g_st.readOffset , SEEK_SET);

			if(g_st.fFront == true){
				//�O���o�b�t�@��1�b�����b�N
				g_st.buf->Lock(0 , g_st.oneSecondBytes , &p , &size , NULL , NULL , 0);
			}else{
				//�㔼�o�b�t�@��1�b�����b�N
				g_st.buf->Lock(g_st.oneSecondBytes , g_st.oneSecondBytes , &p , &size , NULL , NULL , 0);
			}

			//1�b�ǂ�ŉ��̏I���𒴂��Ȃ������m�F
			if(g_st.readOffset + g_st.oneSecondBytes > g_st.musicSize){
				//���o�C�g�Ԃ�A�͂ݏo�����������߂�
				over = g_st.oneSecondBytes - (g_st.musicSize - g_st.readOffset);
			}

			//1��ڂ�mmioRead�i����܂ł̓ǂݍ��ݕ����j
			g_st.readSize = mmioRead(hmmio , (char*)p , g_st.oneSecondBytes - over);
			//1�b���]�����Ă��Ȃ��Ȃ�2��ڂ̓]��
			if(g_st.readSize < g_st.oneSecondBytes){
				//���f�[�^�̐擪����s�������������]��
				mmioSeek(hmmio , g_st.startOffset , SEEK_SET);
				mmioRead(hmmio , (char*)p + g_st.readSize , over);
			}
			
			//wav�t�@�C�������擾
			mmioGetInfo(hmmio , &mmioinfo , 0);
			//���̓ǂݍ��݈ʒu���R�s�[
			g_st.readOffset = mmioinfo.lDiskOffset;

			//�A�����b�N
			g_st.buf->Unlock(p , size , NULL , 0);
			//�t�@�C�������
			mmioClose(hmmio , 0);
			//�ǂݍ��݊���
			g_st.fRead = false;
			
		}
		//�Đ��J�[�\���̎擾
		g_st.buf->GetCurrentPosition(&pos , NULL);
		
		//�O���Đ����ŁA�O���ǂݍ��ݍς݂����m�F
		if(pos < g_st.oneSecondBytes && g_st.fFront == true){
			//�㔼�ɓǂݍ��ނ��߂̐ݒ�
			g_st.fRead = true;			//�ǂݍ��ݕK�v
			g_st.fFront = false;		//�㔼�ɓǂݍ���
		}
		//�㔼�Đ����Ō㔼�ǂݍ��ݍς݂��m�F
		if(pos >= g_st.oneSecondBytes && g_st.fFront == false){
			//�O���ɓǂݍ��ނ��߂̐ݒ�
			g_st.fRead = true;			//�ǂݍ��ݕK�v
			g_st.fFront = true;			//�O���ɓǂݍ���
		}
		//2�b���̃o�b�t�@�����[�v�Đ�
		g_st.buf->Play(0 , 0 , DSBPLAY_LOOPING);
	}
}


//***********************************************************************************************
//�i�X�g���[�~���O�j�Z�J���_���o�b�t�@�̉��
//***********************************************************************************************
void ReleaseStreaming(){
	//2�b���̃o�b�t�@�����
	if(g_st.buf){
		g_st.buf->Release();
		g_st.buf = NULL;
		//�X�g���[�~���O�\���̂̏�����
		ZeroMemory(&g_st , sizeof(STREAMING));
	}
}

