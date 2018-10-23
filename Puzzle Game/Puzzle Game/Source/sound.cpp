//ライブラリの追加
#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"winmm.lib")

#include "sound.h"

//グローバル変数
STREAMING g_st = {0};		//ストリーミング再生用データ
//Directオブジェクト
LPDIRECTSOUND g_lpDS = NULL;
//プライマリバッファ
LPDIRECTSOUNDBUFFER g_lpPrimaryBuffer = NULL;
//セカンダリバッファ
LPDIRECTSOUNDBUFFER g_lpSoundBuffer[SOUND_MAX] = {0};


//***********************************************************************************************
//初期化
//***********************************************************************************************

bool InitDirectSound(HWND hWnd){

	//変数
	//エラーチェック
	HRESULT hRet = NULL;
	//プレイマリバッファ生成用
	DSBUFFERDESC dsbd = {0};
	//WAVEフォーマット情報
	WAVEFORMATEX wfx = {0};

	//▼①　DirectSoundオブジェクトの生成
	hRet = DirectSoundCreate(NULL,&g_lpDS,NULL);

	//エラーチェック
	if(FAILED(hRet)){
		return false;
	}

	//▼②　協調レベルの設定
	hRet = g_lpDS->SetCooperativeLevel(hWnd,DSSCL_PRIORITY);
	
	//エラーチェック
	if(FAILED(hRet)){
		return false;
	}

	//▼③　プライマリバッファ生成
	//プライマリバッファの情報設定
	dsbd.dwSize = (DWORD)sizeof(DSBUFFERDESC);
	//プライマリバッファの生成設定
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME |
					DSBCAPS_PRIMARYBUFFER;

	//プライマリバッファの生成
	hRet = g_lpDS->CreateSoundBuffer(&dsbd,&g_lpPrimaryBuffer,NULL);
	
	//エラーチェック
	if(FAILED(hRet)){
		return false;
	}


	//▼④　プライマリバッファにWAVEフォーマットの設定

	//WAVEフォーマット設定
	//サイズ設定
	wfx.cbSize = sizeof(WAVEFORMATEX);
	//オーディオフォーマットのタイプ
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	//１か指定。モノラル２：ステレオ
	wfx.nChannels = 2;
	//44100かを指定（Hz）
	wfx.nSamplesPerSec = 44100;
	//８か１６を指定
	wfx.wBitsPerSample = 16;
	//データの最小値
	wfx.nBlockAlign = wfx.wBitsPerSample * wfx.nChannels / 8;
	//1秒当たりのバイト数
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	
	//エラーチェック
	if(FAILED(hRet)){
		return false;
	}
	return true;
}


//***********************************************************************************************
//曲の読み込み
//***********************************************************************************************
int CreateSound(char* pass){

	//▼⑤　WAVEファイルの読み込み
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

	//WAVEファイルの読み込み（MMIOハンドル取得）
	hmmio = mmioOpen(pass,NULL,MMIO_READ);
	
	//エラーチェック
	if(!hmmio){
		return false;
	}

	//WAVEファイルかどうかを確認
	//RIFFチャンクに進入
	memset(&Parent,0,sizeof(MMCKINFO));
	Parent.ckid = mmioFOURCC('R','I','F','F');
	Parent.fccType = mmioFOURCC('W','A','V','E');
	if(mmioDescend(hmmio,&Parent,NULL,MMIO_FINDRIFF) == MMIOERR_CHUNKNOTFOUND){
		mmioClose(hmmio,0);
		return false;
	}

	//フォーマット（fmt）チャンクに進入
	memset(&Child,0,sizeof(MMCKINFO));
	Child.ckid = mmioFOURCC('f','m','t',' ');
	if(mmioDescend(hmmio,&Child,&Parent,MMIO_FINDCHUNK) == MMIOERR_CHUNKNOTFOUND){
		mmioClose(hmmio,0);
		return false;
	}


	//WAVEフォーマット取得
	if(mmioRead(hmmio,(HPSTR)&wfx,sizeof(WAVEFORMATEX)) == -1){
		mmioClose(hmmio,0);
		return false;
	}

	//FMTチャンクアセンド
	if(mmioAscend(hmmio,&Child,NULL)){
		mmioClose(hmmio,0);
		return false;
	}

	//子チャンクアセンドdataにディセンド
	Child.ckid = mmioFOURCC('d','a','t','a');
	if(mmioDescend(hmmio,&Child,&Parent,MMIO_FINDCHUNK) == MMIOERR_CHUNKNOTFOUND){
		mmioClose(hmmio,0);
		return false;
	}

	//セカンダリバッファ情報構造体設定
	//サイズ設定
	memset(&dsbd,0,sizeof(DSBUFFERDESC));
	//再生カーソルの記憶位置
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	//ボリューム設定可
	dsbd.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = Child.cksize;
	//WAVEフォーマット設定
	dsbd.lpwfxFormat = &wfx;

	//空の要素にセカンダリバッファを生成
	for(i = 0;i < SOUND_MAX;i++){
		if(g_lpSoundBuffer[i] == NULL){
			//バッファ生成
			hRet= g_lpDS->CreateSoundBuffer(&dsbd,&g_lpSoundBuffer[i],NULL);
			//エラーチェック
			if(FAILED(hRet)){
				mmioClose(hmmio,0);
				return false;
			}

			//バッファをロック
			hRet = g_lpSoundBuffer[i]->Lock(0,Child.cksize,&p,&size,NULL,NULL,0);
			//エラーチェック
			if(FAILED(hRet)){
				mmioClose(hmmio,0);
				return false;
			}

			//ロックしたバッファにWAVEデータを読み込む
			lRet = mmioRead(hmmio,(char*)p,Child.cksize);
			//エラーチェック
			if(lRet < 0){
				mmioClose(hmmio,0);
				return false;
			}

			//バッファをアンロック
			hRet = g_lpSoundBuffer[i]->Unlock(p,size,NULL,0);
			//エラーチェック
			if(FAILED(hRet)){
					mmioClose(hmmio,0);
				return false;
			}
			//戻り値を読み込んだ要素番号に設定
			ret = i;
			//曲の読み込み完了
			break;
		}
	}
	//MMIOハンドル解放
	mmioClose(hmmio,0);

	//成功：読み込んだ要素番号、失敗：-1を返す
	return ret;

}

//***********************************************************************************************
//解放
//***********************************************************************************************
void ReleaseDirectSound(){
	 
	int i = 0;

	//ストリーミングの解放
	ReleaseStreaming();

	//▼⑦　生成した順番とは逆順に各オブジェクトを解放
	//セカンダリバッファ解放
	for(i = 0;i < SOUND_MAX;i++){
		if(g_lpSoundBuffer[i]){
			g_lpSoundBuffer[i]->Release();
			g_lpSoundBuffer[i] = NULL;
		}
	}

	//プライマリバッファを解放
	if(g_lpPrimaryBuffer){
		g_lpPrimaryBuffer->Release();
		g_lpPrimaryBuffer = NULL;
	}

	//DirectSoundオブジェクトの解放
	if(g_lpDS){
		g_lpDS->Release();
		g_lpDS = NULL;
	}
}

//***********************************************************************************************
//音楽の再生
//***********************************************************************************************
void PlaySound(int id,int bigintime,bool loop){

	//エラーチェック
	if(g_lpSoundBuffer[id] == NULL) return;

	//再生カーソルを先頭位置へ
	g_lpSoundBuffer[id]->SetCurrentPosition(SECOND * bigintime);
	
	//▼　再生
	//ループ指定↓
	if(loop == true){
		g_lpSoundBuffer[id]->Play(0,0,DSBPLAY_LOOPING);
	}
	//1回のみ↓
	if(loop == false){
		g_lpSoundBuffer[id]->Play(0,0,0);
	}
}

//***********************************************************************************************
//音楽の停止
//***********************************************************************************************
void StopSound(int id){
	
	//エラーチェック
	if(g_lpSoundBuffer[id] == NULL) return;
	
	//停止
	g_lpSoundBuffer[id]->Stop();
}

//***********************************************************************************************
//指定した曲のみの解放
//***********************************************************************************************
void ReleaseSound(int id){

	//セカンダリバッファを解放
	if(g_lpSoundBuffer[id]){
		g_lpSoundBuffer[id]->Release();
		g_lpSoundBuffer[id] = NULL;

	}
}

//***********************************************************************************************
//（ストリーミング）2秒分のセカンダリバッファの生成
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

	//エラーチェック
	if(g_st.buf){
		return false;		//既に2秒分のバッファがあるなら終了
	}

	//指定のwavファイルを開く
	//WAVEファイルの読み込み（MMIOハンドル取得）
	hmmio = mmioOpen(pass,NULL,MMIO_READ);
	
	//エラーチェック
	if(!hmmio){
		return false;
	}

	//WAVEファイルかどうかを確認
	//RIFFチャンクに進入
	memset(&Parent,0,sizeof(MMCKINFO));
	Parent.ckid = mmioFOURCC('R','I','F','F');
	Parent.fccType = mmioFOURCC('W','A','V','E');
	if(mmioDescend(hmmio,&Parent,NULL,MMIO_FINDRIFF) == MMIOERR_CHUNKNOTFOUND){
		mmioClose(hmmio,0);
		return false;
	}
	//フォーマット（fmt）チャンクに進入
	memset(&Child,0,sizeof(MMCKINFO));
	Child.ckid = mmioFOURCC('f','m','t',' ');
	if(mmioDescend(hmmio,&Child,&Parent,MMIO_FINDCHUNK) == MMIOERR_CHUNKNOTFOUND){
		mmioClose(hmmio,0);
		return false;
	}
	//WAVEフォーマット取得
	if(mmioRead(hmmio,(HPSTR)&wfx,sizeof(WAVEFORMATEX)) == -1){
		mmioClose(hmmio,0);
		return false;
	}
	//FMTチャンクアセンド
	if(mmioAscend(hmmio,&Child,NULL)){
		mmioClose(hmmio,0);
		return false;
	}
	//子チャンクアセンドdataにディセンド
	Child.ckid = mmioFOURCC('d','a','t','a');
	if(mmioDescend(hmmio,&Child,&Parent,MMIO_FINDCHUNK) == MMIOERR_CHUNKNOTFOUND){
		mmioClose(hmmio,0);
		return false;
	}

	//セカンダリバッファ情報構造体設定
	//サイズ設定
	memset(&dsbd,0,sizeof(DSBUFFERDESC));
	//再生カーソルの記憶位置
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	//ボリューム設定可
	dsbd.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = Child.cksize;
	//WAVEフォーマット設定
	dsbd.lpwfxFormat = &wfx;

	//セカンダリバッファの生成
	//2秒分のバッファを作る為の設定
	memset(&dsbd , 0 , sizeof(DSBUFFERDESC));
	dsbd.dwSize = (DWORD)sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = wfx.nAvgBytesPerSec * 2;
	dsbd.lpwfxFormat = &wfx;

	//設定を基にバッファを生成
	hRet = g_lpDS->CreateSoundBuffer(&dsbd , &g_st.buf , NULL);

	//1曲分のサイズをコピー
	g_st.musicSize = Child.cksize;
	//1秒分のサイズをWAVEフォーマットからコピー
	g_st.oneSecondBytes = wfx.nAvgBytesPerSec;
	//ファイルパスのコピー
	strcpy(&g_st.pass[0],pass);
	//最初はバッファの前半1秒に読み込み
	g_st.fFront = true;
	//読み込みフラグＯＮ
	g_st.fRead = true;

	//wavファイル情報を取得
	mmioGetInfo(hmmio , &mmioinfo , 0);
	//wavファイル内の音データ開始位置をコピー
	g_st.startOffset = mmioinfo.lDiskOffset;
	//次の読み込み位置をコピー
	g_st.readOffset = mmioinfo.lDiskOffset;

	//wavファイルを閉じる
	mmioClose(hmmio,0);
	return true;
}

//***********************************************************************************************
//（ストリーミング）音の再生
//***********************************************************************************************
void PlayStreaming( bool play ){

	//開いたwavファイル
	HMMIO hmmio = 0;	
	//ロックしたポインタ
	LPVOID p = NULL;
	//ロックしたサイズ
	DWORD size = 0;
	//再生カーソルの位置
	DWORD pos = 0;
	//はみ出したバイト数
	DWORD over = 0;
	//現在の再生位置のバイト数を取得する
	MMIOINFO mmioinfo = {0};

	//2秒分のバッファがないなら終了
	if(g_st.buf == NULL){
		return;
	}

	//再生フラグ
	if(play){
		//必要があれば1秒分の音を書き込み
		if(g_st.fRead){
			
			//ファイルを開く
			hmmio = mmioOpen(&g_st.pass[0] , NULL , MMIO_READ);
			
			//wavファイルからの読み込み位置をずらす
			mmioSeek(hmmio , g_st.readOffset , SEEK_SET);

			if(g_st.fFront == true){
				//前半バッファを1秒分ロック
				g_st.buf->Lock(0 , g_st.oneSecondBytes , &p , &size , NULL , NULL , 0);
			}else{
				//後半バッファを1秒分ロック
				g_st.buf->Lock(g_st.oneSecondBytes , g_st.oneSecondBytes , &p , &size , NULL , NULL , 0);
			}

			//1秒読んで音の終わりを超えないかを確認
			if(g_st.readOffset + g_st.oneSecondBytes > g_st.musicSize){
				//何バイトぶん、はみ出したかを求める
				over = g_st.oneSecondBytes - (g_st.musicSize - g_st.readOffset);
			}

			//1回目のmmioRead（これまでの読み込み部分）
			g_st.readSize = mmioRead(hmmio , (char*)p , g_st.oneSecondBytes - over);
			//1秒分転送していないなら2回目の転送
			if(g_st.readSize < g_st.oneSecondBytes){
				//音データの先頭から不足した分だけ転送
				mmioSeek(hmmio , g_st.startOffset , SEEK_SET);
				mmioRead(hmmio , (char*)p + g_st.readSize , over);
			}
			
			//wavファイル情報を取得
			mmioGetInfo(hmmio , &mmioinfo , 0);
			//次の読み込み位置をコピー
			g_st.readOffset = mmioinfo.lDiskOffset;

			//アンロック
			g_st.buf->Unlock(p , size , NULL , 0);
			//ファイルを閉じる
			mmioClose(hmmio , 0);
			//読み込み完了
			g_st.fRead = false;
			
		}
		//再生カーソルの取得
		g_st.buf->GetCurrentPosition(&pos , NULL);
		
		//前半再生中で、前半読み込み済みかを確認
		if(pos < g_st.oneSecondBytes && g_st.fFront == true){
			//後半に読み込むための設定
			g_st.fRead = true;			//読み込み必要
			g_st.fFront = false;		//後半に読み込み
		}
		//後半再生中で後半読み込み済みか確認
		if(pos >= g_st.oneSecondBytes && g_st.fFront == false){
			//前半に読み込むための設定
			g_st.fRead = true;			//読み込み必要
			g_st.fFront = true;			//前半に読み込み
		}
		//2秒分のバッファをループ再生
		g_st.buf->Play(0 , 0 , DSBPLAY_LOOPING);
	}
}


//***********************************************************************************************
//（ストリーミング）セカンダリバッファの解放
//***********************************************************************************************
void ReleaseStreaming(){
	//2秒分のバッファを解放
	if(g_st.buf){
		g_st.buf->Release();
		g_st.buf = NULL;
		//ストリーミング構造体の初期化
		ZeroMemory(&g_st , sizeof(STREAMING));
	}
}

