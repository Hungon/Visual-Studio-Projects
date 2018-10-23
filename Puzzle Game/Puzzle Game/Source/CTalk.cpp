#include	"CTalk.h"
#include	<stdio.h>
#include	"input.h"

CTalk::~CTalk() {}

/*
set each default value
*/
void	CTalk::InitializeTalk(
	int fontSize, int drawingSpeed,
	int lineSpace, COLORREF	color) 
{
	this->m_next = drawingSpeed;			//spatial time to draw
	this->m_size = fontSize;				//font's size
	this->m_lineSpace = lineSpace;			//spatial line
	this->m_defaultColor = color;			//default color to draw

}

//************************************************************************************************
//更新
//************************************************************************************************
bool	CTalk::UpdateTalk() {

	//文章未発生なら終了
	if (this->m_fShow == false) return false;

	//終了フラグが立っていたら全て表示終了
	if (this->m_fDelete) {
		//表示フラグをfalse
		this->m_fShow = false;
		//文字数をリセット
		this->m_word = 0;
		//これまで表示した文章をリセット	
		ZeroMemory(&this->m_mes[0], sizeof(char) * 256);
	}
	//文字数増加が終わっていたら読み込み
	if (this->m_fRead == false && JustKey(VK_RETURN)) {
		//文字数をリセット
		this->m_word = 0;
		//これまで表示した文章をリセット	
		ZeroMemory(&this->m_mes[0], sizeof(char) * 256);
		this->m_fRead = true;
		//次の文章を読み込み
		this->m_readoffset = LoadTalk(
			&this->m_pass[0],	  //読み込むファイル 
			&this->m_mes[0],	  //文字列
			this->m_readoffset);  //読み込んでいるバイト数
	}

	//読み込みフラグがtureなら文字数を増加
	if (this->m_fRead) {
		//経過時間の計測	
		this->m_cnt++;
		//経過時間が表示間隔の倍数か確認
		if (this->m_cnt % this->m_next == 0) {
			this->m_word++;			//表示文字数加算
			this->m_cnt = 0;		//経過時間リセット
		}
	}
	//今回のフレームでの処理が終了
	return this->m_fShow;
}

//************************************************************************************************
//表示
//************************************************************************************************
void	CTalk::DrawTalk() {

	POINT pos = { 0 };		//文字の座標
	char mes[3] = { 0 };	//1文字作成用

	//文章送りが開始したら枠を表示
	if (this->m_fShow) {
		//現在の文字数ぶんループさせる
		for (int i = 0; i < this->m_word; i++) {
			//特殊文字「@r」があれば文字色を赤色にする
			if (this->m_mes[i * 2] == '@' &&
				this->m_mes[i * 2 + 1] == 'r') {
				this->m_changeColor = RGB(255, 0, 0);
				continue;
			}
			//特殊文字「@g」があれば文字色を緑色にする
			if (this->m_mes[i * 2] == '@' &&
				this->m_mes[i * 2 + 1] == 'g') {
				this->m_changeColor = RGB(0, 255, 0);
				continue;
			}
			//特殊文字「@b」があれば文字色を青色にする
			if (this->m_mes[i * 2] == '@' &&
				this->m_mes[i * 2 + 1] == 'b') {
				this->m_changeColor = RGB(0, 0, 255);
				continue;
			}
			//特殊文字「@y」があれば文字色を黄色にする
			if (this->m_mes[i * 2] == '@' &&
				this->m_mes[i * 2 + 1] == 'y') {
				this->m_changeColor = RGB(255, 255, 0);
				continue;
			}
			//特殊文字「#t」があればタブ1個分空けて表示
			if (this->m_mes[i * 2] == '#' &&
				this->m_mes[i * 2 + 1] == 't') {
				pos.x += this->m_size;
				continue;
			}
			//特殊文字「#e」があれば文字数増加終了
			if (this->m_mes[i * 2] == '#' &&
				this->m_mes[i * 2 + 1] == 'e') {
				this->m_fRead = false;
				break;
			}
			//特殊文字「＃n」があれば改行
			if (this->m_mes[i * 2] == '#' &&
				this->m_mes[i * 2 + 1] == 'n') {
				pos.x = 0;
				pos.y += this->m_size + this->m_lineSpace;
				this->m_changeColor = this->m_defaultColor;
				continue;
			}
			//読み込んだデータの並びに「#d」がないかチェック
			if (this->m_mes[i * 2] == '#' &&
				this->m_mes[i * 2 + 1] == 'd') {
				this->m_fDelete = true;
				break;
			}
			//表示する1文字を作成し、表示
			//表示する1文字を作成
			mes[0] = this->m_mes[i * 2];
			mes[1] = this->m_mes[(i * 2) + 1];
			//ヌル文字分
			mes[2] = '\0';

			//文字の表示
			DDTextOutEx(mes, (int)this->m_baseX + pos.x,	//表示X座標 
				(int)this->m_baseY + pos.y,					//表示Y座標
				this->m_size,								//文字のサイズ
				this->m_changeColor,						//文字の色
				true);										//イタリック体フラグ

			//文字同士の間隔を開ける
			pos.x += this->m_size;
		}
	}

	this->m_changeColor = this->m_defaultColor;			//back to default color
	//次のファイル表示に座標をリセットする
	pos.x = 0;							//X座標
	pos.y = 0;							//Y座標
}


//************************************************************************************************
//読み込み
//************************************************************************************************
int		CTalk::LoadTalk(char *pass, char *mes, int offset) {

	//ファイル読み込み用
	FILE *fp = NULL;
	//ファイルをバイナリ形式で開く
	fp = fopen(pass, "rb");
	//エラーチェック
	if (fp == NULL) {
		MessageBox(NULL, "ファイルの読み込みに失敗", "TALK", MB_OK);
		return -1;
	}
	//ファイルポインタを移動
	fseek(fp, offset, SEEK_SET);
	//256個の要素に1バイトずつ読み込み
	for (int i = 0; i < 256; i++) {

		//文章の末端なら抜ける
		if (feof(fp) != 0) {
			//次のファイルを読み込めるフラグをtrue
			this->m_fNext = true;
			return this->m_readoffset;
		}
		//messの要素i番に読み込む
		fread(mes + i, sizeof(char), 1, fp);
		//読み込んだサイズを加算
		this->m_readoffset++;
		//読み込んだデータの並びに「#e」がないかチェック
		if (mes[i] == 'e' &&
			mes[i - 1] == '#') {
			this->m_limitWords = i;
			fclose(fp);
			return this->m_readoffset;
		}
		//読み込んだデータの並びに改行がないかチェック
		if (mes[i] == 10 &&
			mes[i - 1] == 13) {
			mes[i] = 0;
			mes[i - 1] = 0;
			i -= 2;
		}
	}
	fclose(fp);
	return offset;
}


//************************************************************************************************
//ファイルを読み込んで文章を生成
//************************************************************************************************
void	CTalk::CreateTalk(
	char *lpPass,
	float startX, float startY) 
{

	//文章生成済みなら終了
	if (this->m_fShow) return;

	//現在表示している文字数
	this->m_word = 0;
	//1文字数を表示してからの経過時間
	this->m_cnt = 0;
	//表示座標
	this->m_baseX = startX;		//X座標
	this->m_baseY = startY;		//Y座標
	//文字数を増やす
	this->m_fRead = true;
	//読み込み位置リセット
	this->m_readoffset = 0;
	//読み込みファイルパスをリセット
	strcpy(&this->m_pass[0], lpPass);
	//ファイルから次の文章を読み込み
	this->m_readoffset = LoadTalk(
		&this->m_pass[0],							 //ファイルパス
		&this->m_mes[0],							  //文字列
		this->m_readoffset);						  //読み込んだバイト数

	//文章生成終了
	this->m_fShow = true;

}


/*
from below functions for baloon of talking
*/

/*
initializer
*/
void	CTalk::InitChara(
	TPOINT<float> baloonPos, TPOINT<int> baloonSize) 
{
	this->m_Fpos.x = this->m_baseX;
	this->m_Fpos.y = this->m_baseY;
	this->m_Isize = baloonSize;
}

/*
Update baloon
*/
void	CTalk::UpdateChara() {
	if (this->m_existFlag) {
		this->m_Fpos.x = this->m_baseX - 20.0f;
		this->m_Fpos.y = this->m_baseY - 20.0f;
	}
}

/*
drawing function
*/
void	CTalk::DrawChara(
	tagDRAWINGKIND drawKind, 
	tagDDSCALESTARTINGPOINT DDScaleStartPoint) 
{

	if (this->m_existFlag) {

		switch (drawKind)
		{
		case DRAWING_TYPE_DDBLT:
			DDBlt(
				this->m_Fpos.x,
				this->m_Fpos.y,
				this->m_Isize.x,
				this->m_Isize.y,
				this->m_ImgNum,
				this->m_Ipic.x,
				this->m_Ipic.y
				);
			break;

		case DRAWING_TYPE_DDSCALEBLT:
			DDScaleBltSelectIndicate(
				this->m_Fpos.x,
				this->m_Fpos.y,
				this->m_Isize.x,
				this->m_Isize.y,
				this->m_ImgNum,
				this->m_Ipic.x,
				this->m_Ipic.y,
				this->m_scale,
				DDScaleStartPoint);
			break;

		case DRAWING_TYPE_DDALPHABLT:
			DDAlphaBlt(
				(int)this->m_Fpos.x,
				(int)this->m_Fpos.y,
				this->m_Isize.x,
				this->m_Isize.y,
				this->m_ImgNum,
				this->m_Ipic.x,
				this->m_Ipic.y,
				this->m_alpha);
			break;
		}
	}
}
