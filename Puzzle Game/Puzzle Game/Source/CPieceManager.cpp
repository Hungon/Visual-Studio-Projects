#include	<iostream>
#include	"CPieceManager.h"
#include	"CPiece.h"
#include	"Likelihood.h"
#include	"graphics.h"
#include	"input.h"
#include	"MemoryLeakCheck.h"
#include	"window.h"
#include	"sound.h"
#include	"File.h"
#include	"CRisdonManager.h"
#include	<stdio.h>

using namespace std;


//▼ 静的メンバ変数の再定義
//kind of puzzle's size
const TPOINT<int>	CPieceManager::PIECEMANAGER_IMAGE_PUZZLE_WHOLE_SIZE_1 = { 288, 192 };
const TPOINT<int>	CPieceManager::PIECEMANAGER_IMAGE_PUZZLE_WHOLE_SIZE_2 = { 288, 288 };

//each piece's setting
const TPOINT<int>	CPieceManager::PIECEMANAGER_IMAGE_PIECE_SIZE_1 = { 36, 48 };
const TPOINT<int>	CPieceManager::PIECEMANAGER_IMAGE_PIECE_SIZE_2 = { 72, 72 };
const float			CPieceManager::PIECEMANAGER_IMAGE_SELECTING_PIECE_ALPHA = 120.0f;

HWND CPieceManager::m_hWnd = NULL;									//ウィンドウハンドル
const int	CPieceManager::CPIECEMANAGER_IMAGE_MAX = 4;				//画像の使用枚数
const int	CPieceManager::CPIECEMANAGER_KIND_OF_EFFECT_SOUND = 2;	//for effect sound
const int	CPieceManager::CPIECEMANAGER_BACKIMAGE_MAX = 2;			//背景画像の使用枚数
const float	CPieceManager::PIECEMANAGER_SELECT_ALPHA = 0.75f;		//選択中のピースの透過倍率
const int	CPieceManager::STAGE_WIDTH = 640;						//ステージの幅
const int	CPieceManager::STAGE_HEIGHT = 480;						//ステージの高さ

//fram image
const TPOINT<int>	CPieceManager::PIECEMANAGER_IMAGE_FRAM_1_DEFAULT_SIZE = { 352, 240 };
const TPOINT<int>	CPieceManager::PIECEMANAGER_IMAGE_FRAM_2_DEFAULT_SIZE = { 352, 352 };
const float			CPieceManager::PIECEMANANGER_IMAGE_FRAM_DEFAULT_SCALE = 1.0f;

//③ 完成時に出す「Congratulation!」
const int	CPieceManager::PIECE_CONGRATULATION_DEFAULT_WIDTH = 320;		//幅
const int	CPieceManager::PIECE_CONGRATULATION_DEFAULT_HEIGHT = 40;		//高さ
const float	CPieceManager::PIECE_CONGRATULATION_DEFAULT_SCALE = 1.0f;		//拡縮倍率
const float	CPieceManager::PIECE_CONGRATULATION_DEFAULT_ADD_SCALE = 0.02f;	//拡縮倍率増加値
//flag of complete
bool	CPieceManager::m_CompleteFlag = false;

//complete image
const TPOINT<float>	CPieceManager::PIECEMANAGER_IMAGE_COMPLETE_IMAGE_COORDINATE = { 30.0f, 150.0f };
const float		CPieceManager::PIECEMANAGER_IMAGE_COMPLETE_IMAGE_DEFAULT_SCALE = 0.4f;

//arrangement time
const int	CPieceManager::PIECEMANAGER_ARRANGEMENT_PIECES_TIME = 200;

//kind of file for arrangement pieces
const int	CPieceManager::PIECEMANAGER_KIND_OF_FILE_FOR_8_X_4 = 5;
const int	CPieceManager::PIECEMANAGER_KIND_OF_FILE_FOR_4_X_4 = 4;

//don't execution time
const int	CPieceManager::PIECEMANAGER_DONT_PROCESS_TIME = 50;

//set select move
const TPOINT<int>	CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_SIZE = { 64, 64 };
const TPOINT<float>	CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_COORDINATE = { 20.0f, 340.0f };
const float			CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_DEFAULT_SCALE = 0.7f;

/*
************************************************************************************
ピースのコンストラクタ
************************************************************************************
*/	
CPieceManager::CPieceManager(){

	//▼ 扱うクラス
	this->m_lpCPiece = NULL;									//ピース
	this->m_lpPieceImg = NULL;									//画像の要素
	this->m_lpCBackImage = NULL;								//背景
	this->m_lpPieceSe = NULL;									//effect sound
	this->m_lpCompleteImage = NULL;								//completed image
	this->m_lpRisdonManager = NULL;								//for get risdon's setting

	//▼ 各種座標の設定
	this->m_getPieceNum = 0;											//ピースの要素取得用
	this->m_pieceStartPosition.x = 0.0f;								//座標X
	this->m_pieceStartPosition.y = 0.0f;								//座標Y
	this->m_selectPieceManagerFpos = { 0 };								//選択した座標X
	this->m_swapFlag = PIECEMANAGER_ARRANGING_PIECE;					//配列フラグ
	this->m_swapPieceNum = 0;											//交換するピースの要素
	//flag of complete
	this->m_CompleteFlag = false;
	//for arrangement piece
	this->m_ArrangementPieceCountTime = 0;
	//for correct count
	this->m_CorrectCount = 0;
	//kind of puzzle
	this->m_PuzzleKind = PIECEMANAGER_PUZZLE_SIZE_1;
}


/*
************************************************************************************
ピースのディストラクタ
************************************************************************************
*/
CPieceManager::~CPieceManager(){}

/*
************************************************************************************
ピースの初期化
************************************************************************************
*/
void CPieceManager::InitPieceManager() {

	//puzzle's size is the first of images
	char	*lpPuzzleImage1[PIECEMANAGER_PUZZLE_KIND_OF_SIZE_1] = {
		"Image\\Puzzle\\Audrey1.bmp",
		"Image\\Puzzle\\Audrey2.bmp",
		"Image\\Puzzle\\Audrey3.bmp",
	};
	//puzzle's size is the second of images
	char	*lpPuzzleImage2[PIECEMANAGER_PUZZLE_KIND_OF_SIZE_2] = {
		"Image\\Puzzle\\Taylor.bmp",
		"Image\\Puzzle\\Grande.bmp",
		"Image\\Puzzle\\Tarzan.bmp",
	};
	//kind of fram
	char	*lpFramImage[PIECEMANAGER_KIND_OF_PUZZLE_FRAM] = {
		"Image\\Bg\\Fram1.bmp",				//for arrangement 8x4
		"Image\\Bg\\Fram2.bmp",				//for arrangement 4x4
	};

	int PuzzleKind = RandomNum(PIECEMANAGER_PUZZLE_SIZE_2, PIECEMANAGER_PUZZLE_SIZE_1);
	this->m_PuzzleKind = (tagPIECEMANAGERPUZZLEKIND)PuzzleKind;					//get present kind of puzzle
	char PuzzleImage[256] = { 0 };
	//decide kind of puzzle in random
	switch (PuzzleKind) {
	case PIECEMANAGER_PUZZLE_SIZE_1:
		strcpy(PuzzleImage, lpPuzzleImage1[RandomNum(PIECEMANAGER_PUZZLE_SIZE_1_THREE, PIECEMANAGER_PUZZLE_SIZE_1_ONE)]);
		//initialize each global variable
		this->m_pieceSize = CPieceManager::PIECEMANAGER_IMAGE_PIECE_SIZE_1;		//piece's size
		this->m_WholeSize = CPieceManager::PIECEMANAGER_IMAGE_PUZZLE_WHOLE_SIZE_1;			//whole puzzle's size
		this->m_Tarray.x = this->m_WholeSize.x / this->m_pieceSize.x;			//array piece
		this->m_Tarray.y = this->m_WholeSize.y / this->m_pieceSize.y;			//array piece
		break;
	case PIECEMANAGER_PUZZLE_SIZE_2:
		strcpy(PuzzleImage, lpPuzzleImage2[RandomNum(PIECEMANAGER_PUZZLE_SIZE_2_THREE, PIECEMANAGER_PUZZLE_SIZE_2_ONE)]);
		//initialize each global variable
		this->m_pieceSize = CPieceManager::PIECEMANAGER_IMAGE_PIECE_SIZE_2;		//piece's size
		this->m_WholeSize = CPieceManager::PIECEMANAGER_IMAGE_PUZZLE_WHOLE_SIZE_2;			//whole puzzle's size
		this->m_Tarray.x = this->m_WholeSize.x / this->m_pieceSize.x;			//array piece
		this->m_Tarray.y = this->m_WholeSize.y / this->m_pieceSize.y;			//array piece
		break;
	}

	//kind of image
	char *PictureTbl[CPieceManager::CPIECEMANAGER_IMAGE_MAX] = {
		PuzzleImage ,							//puzzle's place
		lpFramImage[PuzzleKind] ,				//puzzle's fram
		"Image\\Bg\\Congratulation.bmp" ,		//Congratulation!
		"Image\\Bg\\SelectMove.bmp",			//for select move
	};

	//using effect sound
	char *lpSe[CPieceManager::CPIECEMANAGER_KIND_OF_EFFECT_SOUND] = {
		"SE\\swap.wav",				//slected effect sound
		"SE\\correct.wav",			//correct sound
	};

	//▼ 各種メモリ確保
	//CPieceクラスのメモリを確保する
	this->m_lpCPiece = new CPiece[this->m_Tarray.y * this->m_Tarray.x];
	//背景のメモリを確保
	this->m_lpCBackImage = new CPiece[CPieceManager::CPIECEMANAGER_BACKIMAGE_MAX];
	this->m_lpCompleteImage = new CPiece;				//for complete image
	//画像の要素数を確保する
	this->m_lpPieceImg = new int[CPieceManager::CPIECEMANAGER_IMAGE_MAX];
	this->m_lpPieceSe = new int[CPieceManager::CPIECEMANAGER_KIND_OF_EFFECT_SOUND];
	this->m_lpRisdonManager = new CRisdonManager;								//for get risdon's setting
	this->m_lpSelectMoveCh = new CPiece[PIECEMANAGER_KIND_OF_SELECT_MOVE];		//for select puzzle's move

	//▼ 画像の読み込み
	//最大枚数だけ読み込む
	for (int i = 0; i < CPieceManager::CPIECEMANAGER_IMAGE_MAX; i++) {
		//画像パス
		this->m_lpPieceImg[i] = CreateDDImage(PictureTbl[i], CPieceManager::m_hWnd);
	}
	//load effect sound
	for (int i = 0; i < CPieceManager::CPIECEMANAGER_KIND_OF_EFFECT_SOUND; i++) {
		this->m_lpPieceSe[i] = CreateSound(lpSe[i]);
	}

	//piece's starting coordinate
	this->m_pieceStartPosition.x = CHARACTER_CENTER_POSITION_X(this->m_WholeSize.x);
	this->m_pieceStartPosition.y = CHARACTER_CENTER_POSITION_Y(this->m_WholeSize.y);

	//initialize pieces' indication
	int cnt = 0;				//for material's count
	TPOINT<float> piecePos = this->m_pieceStartPosition;
	//表示先座標、表示元座標を代入
	for (int j = 0; j < this->m_Tarray.y; j++) {
		for (int i = 0; i < this->m_Tarray.x; i++) {
			//画像の要素を表示用に渡す
			this->m_lpCPiece[cnt].SetCharaImg(this->m_lpPieceImg[0]);
			//生存フラグ
			this->m_lpCPiece[cnt].m_existFlag = true;
			piecePos.x = this->m_pieceStartPosition.x + (this->m_pieceSize.x * i);
			piecePos.y = this->m_pieceStartPosition.y + (this->m_pieceSize.y * j);
			//initialize each pieces' indication
			this->m_lpCPiece[cnt].InitChara(piecePos, this->m_Tarray,			//coordinate, arrangements
				this->m_pieceSize, cnt);										//each size, kind
			cnt++;
		}
	}

	//fram image
	//difference fram's size
	TPOINT<int> FramSizeTbl[PIECEMANAGER_KIND_OF_PUZZLE_FRAM] = {
		CPieceManager::PIECEMANAGER_IMAGE_FRAM_1_DEFAULT_SIZE,
		CPieceManager::PIECEMANAGER_IMAGE_FRAM_2_DEFAULT_SIZE,
	};
	this->m_lpCBackImage[0].m_Isize = FramSizeTbl[PuzzleKind];
	this->m_lpCBackImage[0].m_existFlag = true;
	//set coordinate that fit coordinate with screen's size
	this->m_lpCBackImage[0].m_Fpos.x = CHARACTER_CENTER_POSITION_X(FramSizeTbl[PuzzleKind].x);
	this->m_lpCBackImage[0].m_Fpos.y = this->m_pieceStartPosition.y - (this->m_pieceSize.y / 2);
	this->m_lpCBackImage[0].m_scale = CPieceManager::PIECEMANANGER_IMAGE_FRAM_DEFAULT_SCALE;
	//set image's material
	this->m_lpCBackImage[0].SetCharaImg(this->m_lpPieceImg[1]);

	//③ パズル完成時に出す「Congratulation!」
	this->m_lpCBackImage[1].m_Isize.x = CPieceManager::PIECE_CONGRATULATION_DEFAULT_WIDTH;	//幅
	this->m_lpCBackImage[1].m_Isize.y = CPieceManager::PIECE_CONGRATULATION_DEFAULT_HEIGHT;	//高さ
	this->m_lpCBackImage[1].m_existFlag = true;												//生存フラグ
	//表示先Xは画面中央
	this->m_lpCBackImage[1].m_Fpos.x = (float)((CPieceManager::STAGE_WIDTH - this->m_lpCBackImage[1].m_Isize.x) / 2);
	//表示先Yは画面上
	this->m_lpCBackImage[1].m_Fpos.y = this->m_lpCBackImage[0].m_Fpos.y - this->m_lpCBackImage[1].m_Isize.y;
	//表示回数
	this->m_ScaleCnt = 0;
	//set image's material
	this->m_lpCBackImage[1].SetCharaImg(this->m_lpPieceImg[2]);

	//set complete image
	this->m_lpCompleteImage->m_Isize = this->m_WholeSize;
	this->m_lpCompleteImage->m_Fpos = CPieceManager::PIECEMANAGER_IMAGE_COMPLETE_IMAGE_COORDINATE;
	this->m_lpCompleteImage->m_scale = CPieceManager::PIECEMANAGER_IMAGE_COMPLETE_IMAGE_DEFAULT_SCALE;
	this->m_lpCompleteImage->m_existFlag = true;
	//set image's material
	this->m_lpCompleteImage->SetCharaImg(this->m_lpPieceImg[0]);

	//set image of select puzzle's move
	tagPIECEMANAGERSELECTMOVEKIND selectKind[PIECEMANAGER_KIND_OF_SELECT_MOVE] = {
		PIECEMANAGER_SELECT_MOVE_RIGHT,
		PIECEMANAGER_SELECT_MOVE_DOWN,
		PIECEMANAGER_SELECT_MOVE_LEFT,
		PIECEMANAGER_SELECT_MOVE_UP,
	};
	TPOINT<float> selectPos[PIECEMANAGER_KIND_OF_SELECT_MOVE] = {
		{ CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_COORDINATE.x + 
		((CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_SIZE.x * 2) * CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_DEFAULT_SCALE), 
		CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_COORDINATE.y + 
		(CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_SIZE.y * CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_DEFAULT_SCALE)} ,			//right
		{ CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_COORDINATE.x + 
		(CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_SIZE.x * CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_DEFAULT_SCALE),
		CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_COORDINATE.y + 
		((CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_SIZE.y * 2) * CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_DEFAULT_SCALE) } ,	//down
		{ CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_COORDINATE.x ,
		CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_COORDINATE.y + 
		(CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_SIZE.y * CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_DEFAULT_SCALE ) } ,		//left
		{ CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_COORDINATE.x + 
		(CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_SIZE.x * CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_DEFAULT_SCALE) ,
		CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_COORDINATE.y } ,																	//up
	};
	for (int i = 0; i < PIECEMANAGER_KIND_OF_SELECT_MOVE; i++) {
		this->m_lpSelectMoveCh[i].m_Isize = CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_SIZE;
		this->m_lpSelectMoveCh[i].m_scale = CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_DEFAULT_SCALE;
		this->m_lpSelectMoveCh[i].m_Ipic.y = selectKind[i] * CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_SIZE.y;
		this->m_lpSelectMoveCh[i].m_kindNum = selectKind[i];
		this->m_lpSelectMoveCh[i].m_Fpos = selectPos[i];
		this->m_lpSelectMoveCh[i].m_existFlag = true;
		//set image's material
		this->m_lpSelectMoveCh[i].SetCharaImg(this->m_lpPieceImg[3]);
	}
}

/*
************************************************************************************
ピースの更新
************************************************************************************
*/
bool CPieceManager::UpdatePieceManager() {

	//get mouse's coodinate
	int mouseX = GetMouseX();
	int mouseY = GetMouseY();
	//get risdon's setting
	TPOINT<float> risdonPos = this->m_lpRisdonManager->CCharacterManager::GetCharacterManagerPosition();
	TPOINT<int>	risdonSize = this->m_lpRisdonManager->CCharacterManager::GetCharacterManagerSize();
	if (this->m_swapFlag == PIECEMANAGER_SELECT_PIECE) {
		//when click the risdon, subsutitute flag of swapping
		if (mouseX >= risdonPos.x && mouseX <= (risdonPos.x + risdonSize.x) &&
			mouseY >= risdonPos.y && mouseY <= (risdonPos.y + risdonSize.y)) {
			if (GetDownL()) {
				this->m_swapFlag = PIECEMANAGER_SWAP_ALL_LINE;
			}
		}
	}

	//piece's process, first arranging pieces, second select piece to swap,
	//third replace selected piece with swap piece, finally swapping pieces during press mouse's left click
	if (this->m_CompleteFlag == false) {
		switch (this->m_swapFlag) {
		
		case PIECEMANAGER_VACANT:					//for make nothing process
			this->m_ArrangementPieceCountTime++;
			if (this->m_ArrangementPieceCountTime >= CPieceManager::PIECEMANAGER_DONT_PROCESS_TIME) {
				this->m_swapFlag = PIECEMANAGER_SELECT_PIECE;				//subsutitute flag of selection piece
				this->m_ArrangementPieceCountTime = 0;						//reset count time
			}
			break;

		case PIECEMANAGER_ARRANGING_PIECE:			//initialize arrangement of pieces
			this->ArrangePiece();
			if (this->m_ArrangementPieceCountTime < CPieceManager::PIECEMANAGER_ARRANGEMENT_PIECES_TIME) {
				this->m_ArrangementPieceCountTime++;					//count time for arranging pieces
			}
			else {
				this->m_swapFlag = PIECEMANAGER_SELECT_PIECE;			//subsutitute select flag 
				this->m_ArrangementPieceCountTime = 0;					//reset count time
			}
			break;

		case PIECEMANAGER_SELECT_PIECE:				//select piece to swap
			this->InitSwapPieceManager();
			break;

		case PIECEMANAGER_SELECTED_PIECE:			//select piece to replace
			this->UpdateSelectSwapPieceManager();
			break;

		case PIECEMANAGER_SWAPPING_PIECE:			//during swapping process
			this->UpdateDoSwapPieceManager();
			break;

		case PIECEMANAGER_SWAP_ONE_ROW:
			break;

		case PIECEMANAGER_SWAP_ALL_LINE:
			this->InitializeSwapAllPieceProcess();					//all pieces move to up or right or down or left in random
			break;
		}
		if(this->m_swapFlag != PIECEMANAGER_ARRANGING_PIECE) this->m_CompleteFlag = this->CheckExactlyArrayPiece();	//check complete
	}
	else
	{	//when complete
		//拡大処理３回まで処理
		if (this->m_ScaleCnt <= 3 && this->m_lpCBackImage[1].m_scale < CPieceManager::PIECE_CONGRATULATION_DEFAULT_SCALE) {
			//完成時に出す画像の拡縮倍率を毎フレーム0.01増やす
			this->m_lpCBackImage[1].m_scale += CPieceManager::PIECE_CONGRATULATION_DEFAULT_ADD_SCALE;
			//上限値
			if (this->m_lpCBackImage[1].m_scale >=
				CPieceManager::PIECE_CONGRATULATION_DEFAULT_SCALE) {
				this->m_lpCBackImage[1].m_scale = CPieceManager::PIECE_CONGRATULATION_DEFAULT_SCALE / 2;
				this->m_ScaleCnt++;
			}
			//３回表示したら等倍を代入
			if (this->m_ScaleCnt >= 3) {
				this->m_lpCBackImage[1].m_scale = CPieceManager::PIECE_CONGRATULATION_DEFAULT_SCALE;
			}
		}
	}

	CPieceManager::CompletePuzzleWithoutPlay();			//if pressed D-key on keybord, complete puzzle without playing

	return this->m_CompleteFlag;
}


/*
************************************************************************************
ピースの交換処理の初期化
************************************************************************************
*/
void CPieceManager::InitSwapPieceManager() {


	//get present mouse's coordinate
	int mouseX = GetMouseX();
	int mouseY = GetMouseY();

	//when select piece's position is selection square, execute process
	if (mouseX >= this->m_pieceStartPosition.x &&
		mouseX <= (this->m_pieceStartPosition.x + this->m_WholeSize.x) &&
		mouseY >= this->m_pieceStartPosition.y &&
		mouseY <= (this->m_pieceStartPosition.y + this->m_WholeSize.y)) {
		if (GetDownL()) {			//swap selected piece with choice piece as press mouse's left click
			for (int i = 0; i < (this->m_Tarray.y * this->m_Tarray.x); i++) {	//loop to piece's max
				if (this->m_lpCPiece[i].m_PieceFlag == PIECE_FLAG_FITTING_POSITION) continue;		//if piece's position is correct, jump to next material
				if ((int)mouseX >= (int)this->m_lpCPiece[i].m_Fpos.x &&
					(int)mouseX <= (int)(this->m_lpCPiece[i].m_Fpos.x + this->m_lpCPiece[i].m_Isize.x) &&
					(int)mouseY >= (int)this->m_lpCPiece[i].m_Fpos.y &&
					(int)mouseY <= (int)(this->m_lpCPiece[i].m_Fpos.y + this->m_lpCPiece[i].m_Isize.y)) {

					this->m_swapFlag = PIECEMANAGER_SELECTED_PIECE;						//during flag of swapping process

					//subsutitute slected piece's kind for global variable
					this->m_getPieceNum = i;
					//get selected piece's coordinate
					this->m_selectPieceManagerFpos = this->m_lpCPiece[i].m_Fpos;
					return;
				}
			}
		}
	}
}

/*
************************************************************************************
ピース交換選択処理の更新
************************************************************************************
*/
void CPieceManager::UpdateSelectSwapPieceManager() {

	//get mouse's coordinate
	int mouseX = GetMouseX();
	int mouseY = GetMouseY();

	//fit mouse's coordinate with piece's coordinate
	this->m_lpCPiece[this->m_getPieceNum].m_Fpos.x = mouseX - (this->m_lpCPiece[this->m_getPieceNum].m_Isize.x >> 1);
	this->m_lpCPiece[this->m_getPieceNum].m_Fpos.y = mouseY - (this->m_lpCPiece[this->m_getPieceNum].m_Isize.y >> 1);
	//penatrate piece's image
	this->m_lpCPiece[this->m_getPieceNum].m_alpha = CPieceManager::PIECEMANAGER_IMAGE_SELECTING_PIECE_ALPHA;

	//when get off mouse's left click and mouse's coordinate is out of arrange square, selected piece's coordinate get back to last coordinate
	if (GetDownL() == false) {
		//選択フラグをOFF
		this->m_swapFlag = PIECEMANAGER_SELECT_PIECE;
		//ピースの座標を元に戻す
		this->m_lpCPiece[this->m_getPieceNum].m_Fpos = this->m_selectPieceManagerFpos;
		//透過を元に戻す
		this->m_lpCPiece[this->m_getPieceNum].m_alpha = this->m_lpCPiece->PIECE_IMAGE_DEFAULT_ALPHA;
		return;
	}
	if(GetDownL()){
		//swapping process
		if (mouseX >= this->m_pieceStartPosition.x &&
			mouseX <= (this->m_pieceStartPosition.x + this->m_WholeSize.x) &&
			mouseY >= this->m_pieceStartPosition.y &&
			mouseY <= (this->m_pieceStartPosition.y + this->m_WholeSize.y)) {
			for (int i = 0; i < (this->m_Tarray.y * this->m_Tarray.x); i++) {	//loop to piece's max
				if (this->m_lpCPiece[i].m_PieceFlag == PIECE_FLAG_FITTING_POSITION) continue;		//if piece's position is correct, jump to next material
				if ((int)mouseX >= (int)this->m_lpCPiece[i].m_Fpos.x &&
					(int)mouseX <= (int)(this->m_lpCPiece[i].m_Fpos.x + this->m_lpCPiece[i].m_Isize.x) &&
					(int)mouseY >= (int)this->m_lpCPiece[i].m_Fpos.y &&
					(int)mouseY <= (int)(this->m_lpCPiece[i].m_Fpos.y + this->m_lpCPiece[i].m_Isize.y) &&
					this->m_getPieceNum != i) {

					this->m_swapPieceNum = i;								//get present selected piece's material
					this->m_swapFlag = PIECEMANAGER_SWAPPING_PIECE;			//subsutitute floga of swapping process
					return;
				}
			}
		}
	}
}

/*
************************************************************************************
ピース交換時の処理
************************************************************************************
*/
void CPieceManager::UpdateDoSwapPieceManager() {

	//for swap process
	TPOINT<int>		pic = this->m_lpCPiece[this->m_getPieceNum].m_Ipic;				//origin coordinate
	int		kind = this->m_lpCPiece[this->m_getPieceNum].m_kindNum;
	//get mouse's coordinate
	int mouseX = GetMouseX();
	int mouseY = GetMouseY();
	//reset flag of swapping
	this->m_swapFlag = PIECEMANAGER_SELECT_PIECE;
	//piece get back to default penatration
	this->m_lpCPiece[this->m_getPieceNum].m_alpha = 1.0f;
	//replace selected piece with swapping piece
	this->m_lpCPiece[this->m_getPieceNum].m_kindNum = this->m_lpCPiece[this->m_swapPieceNum].m_kindNum;
	this->m_lpCPiece[this->m_swapPieceNum].m_kindNum = kind;
	this->m_lpCPiece[this->m_getPieceNum].m_Ipic = this->m_lpCPiece[this->m_swapPieceNum].m_Ipic;
	this->m_lpCPiece[this->m_swapPieceNum].m_Ipic = pic;
	this->m_lpCPiece[this->m_getPieceNum].m_Fpos = this->m_selectPieceManagerFpos;
	PlaySound(m_lpPieceSe[0], 0, false);			//swapping effect sound
}

/*
************************************************************************************
ピースの表示
************************************************************************************
*/
void CPieceManager::DrawPieceManager() {

	//並べるピースの数だけループを回す
	for (int i = 0; i < (this->m_Tarray.x * this->m_Tarray.y); i++) {
		//透過表示
		this->m_lpCPiece[i].DrawChara(DRAWING_TYPE_DDALPHABLT, DDSCALE_STARTING_POINT_NOTHING);
	}
	//手前に表示したいピースの表示
	//現在選択中なら取得した要素番号のピースを表示する
	if (this->m_swapFlag == PIECEMANAGER_SELECTED_PIECE) {
		//透過表示
		this->m_lpCPiece[this->m_getPieceNum].DrawChara(DRAWING_TYPE_DDALPHABLT, DDSCALE_STARTING_POINT_NOTHING);
	}

	//draw fram and image of congratulation
	tagDDSCALESTARTINGPOINT scalePoint[2] = { DDSCALE_STARTING_POINT_LEFT_TOP, DDSCALE_STARTING_POINT_CENTER };
	for (int i = 0; i < 2; i++) {
		this->m_lpCBackImage[i].DrawChara(DRAWING_TYPE_DDSCALEBLT, scalePoint[i]);
	}

	//draw image of select move
	for (int i = 0; i < PIECEMANAGER_KIND_OF_SELECT_MOVE; i++) {
		this->m_lpSelectMoveCh[i].DrawChara(DRAWING_TYPE_DDSCALEBLT, DDSCALE_STARTING_POINT_LEFT_TOP);
	}

	//complete image of puzzle
	this->m_lpCompleteImage->DrawChara(DRAWING_TYPE_DDSCALEBLT, DDSCALE_STARTING_POINT_LEFT_TOP);
}


/*
************************************************************************************
解放
************************************************************************************
*/
void CPieceManager::ReleasePieceManager() {
	//初期化されていなかったら解放する
	if (m_lpPieceImg != NULL) {
		//画像の解放
		for (int i = 0; i < CPieceManager::CPIECEMANAGER_IMAGE_MAX; i++) {
			ReleaseDDImage(this->m_lpPieceImg[i]);
		}
	}
	for (int i = 0; i < CPieceManager::CPIECEMANAGER_KIND_OF_EFFECT_SOUND; i++) {			//effect sound
		ReleaseSound(this->m_lpPieceSe[i]);
	}
	delete[]this->m_lpCPiece;			//ピース
	this->m_lpCPiece = NULL;
	delete[]this->m_lpCBackImage;		//背景
	this->m_lpCBackImage = NULL;
	delete[]this->m_lpPieceImg;			//image
	this->m_lpPieceImg = NULL;
	delete[]this->m_lpPieceSe;			//effect sound
	this->m_lpPieceSe = NULL;
	delete this->m_lpCompleteImage;		//complete image
	this->m_lpCompleteImage = NULL;
	delete this->m_lpRisdonManager;		//for risdon
	this->m_lpRisdonManager = NULL;
	delete[]this->m_lpSelectMoveCh;		//select move
	this->m_lpSelectMoveCh = NULL;
}

/*
************************************************************************************
ゲームクリアか調べる関数

機能：ピースの並びが左隅から右隅まで１ずつ昇順となっていたら、trueを返す
************************************************************************************
*/
bool	CPieceManager::CheckExactlyArrayPiece() {

	//各ピースの要素番号が0から１ずつ昇順となっているか調べる
	for (int i = 0; i < (this->m_Tarray.x * this->m_Tarray.y); i++) {

		this->m_lpCPiece[i].UpdateChara();								//each pieces' update

		//各種種類番号の座標が昇順の並びの座標になっているかを調べる
		if ((this->m_pieceStartPosition.x + this->m_lpCPiece[i].m_Ipic.x) == this->m_lpCPiece[i].m_Fpos.x &&
			(this->m_pieceStartPosition.y + this->m_lpCPiece[i].m_Ipic.y) == this->m_lpCPiece[i].m_Fpos.y) {

			if (this->m_lpCPiece[i].m_PieceFlag == PIECE_FLAG_FITTING_POSITION) continue;		//if piece's position is correct, jump to next material

			this->m_lpCPiece[i].m_PieceFlag = PIECE_FLAG_FITTING_POSITION;		//flag of correct position
			PlaySound(this->m_lpPieceSe[1], 0, false);							//correct effect sound
			this->m_CorrectCount++;												//count up correct position
		}
		else {
			this->m_lpCPiece[i].m_PieceFlag = PIECE_FLAG_NOTHING;			//don't fit with correct position
		}
	}
	//if all pieces' position is correct, subsutitute flag of complete	
	if (this->m_CorrectCount == (this->m_Tarray.y * this->m_Tarray.x)) {		//each pieces' penetration get back to default
		for (int j = 0; j < (this->m_Tarray.x * this->m_Tarray.y); j++) { this->m_lpCPiece[j].m_alpha = 1.0f; }
		return true;
	}
	return false;
}

/*
Arrange pieces
*/
void	CPieceManager::ArrangePiece() {

	//using file for arrangement pieces
	char	*lpFile1[CPieceManager::PIECEMANAGER_KIND_OF_FILE_FOR_8_X_4] = {
		"Data\\8x4\\Arrangement1.txt",
		"Data\\8x4\\Arrangement2.txt",
		"Data\\8x4\\Arrangement3.txt",
		"Data\\8x4\\Arrangement4.txt",
		"Data\\8x4\\Arrangement5.txt",
	};
	char	*lpFile2[CPieceManager::PIECEMANAGER_KIND_OF_FILE_FOR_4_X_4] = {
		"Data\\4x4\\Arrangement1.txt",
		"Data\\4x4\\Arrangement2.txt",
		"Data\\4x4\\Arrangement3.txt",
		"Data\\4x4\\Arrangement4.txt",
	};
	int ran;		//random value
	FILEVALUE	fileValue = { 0 };			//for get value in file
	//diverge kind of puzzle to load file
	switch (this->m_PuzzleKind) {
	case PIECEMANAGER_PUZZLE_SIZE_1:			//whole puzzle's size is 8x4
		ran = RandomNum(CPieceManager::PIECEMANAGER_KIND_OF_FILE_FOR_8_X_4 - 1, 0);				//load file in random
		fileValue = *(LoadFile(lpFile1[ran]));	//load file and get each pieces' material
		break;
	case PIECEMANAGER_PUZZLE_SIZE_2:			//whole puzzle's size is 4x4
		ran = RandomNum(CPieceManager::PIECEMANAGER_KIND_OF_FILE_FOR_4_X_4 - 1, 0);				//load file in random
		fileValue = *(LoadFile(lpFile2[ran]));	//load file and get each pieces' material
		break;
	}
	int cnt = 0;
	//arrangement pieces' indication
	for (int j = 0; j < this->m_Tarray.y; j++) {
		for (int i = 0; i < this->m_Tarray.x; i++) {
			//subsutitute origin coordiinate and kind for indication of global variable
			this->m_lpCPiece[cnt].m_kindNum = fileValue.fileBox[j][i];
			this->m_lpCPiece[cnt].m_Ipic.x = (this->m_lpCPiece[cnt].m_kindNum % this->m_Tarray.x) * this->m_lpCPiece[cnt].m_Isize.x;
			this->m_lpCPiece[cnt].m_Ipic.y = (this->m_lpCPiece[cnt].m_kindNum / this->m_Tarray.x) * this->m_lpCPiece[cnt].m_Isize.y;
			//count material
			cnt++;
		}
	}
}

/*
swap one row and select row
*/
void	CPieceManager::InitializeSwapOneRow() {

}

/*
initialize the swapping process
*/
void	CPieceManager::InitializeSwapAllPieceProcess() {

	//get mouse's coordinate
	int mouseX = GetMouseX();
	int mouseY = GetMouseY();

	//for divergence
	tagPIECEMANAGERSELECTMOVEKIND selectMove = PIECEMANAGER_DONT_SELECT_MOVE;

	//when plaayer pressed image of move, 
	for (int i = 0; i < PIECEMANAGER_KIND_OF_SELECT_MOVE; i++) {
		if (mouseX > this->m_lpSelectMoveCh[i].m_Fpos.x &&
			mouseX < this->m_lpSelectMoveCh[i].m_Fpos.x + (this->m_lpSelectMoveCh[i].m_Isize.x * this->m_lpSelectMoveCh[i].m_scale) &&
			mouseY > this->m_lpSelectMoveCh[i].m_Fpos.y &&
			mouseY < this->m_lpSelectMoveCh[i].m_Fpos.y + (this->m_lpSelectMoveCh[i].m_Isize.y * this->m_lpSelectMoveCh[i].m_scale)) {
			//change image
			this->m_lpSelectMoveCh[i].m_Ipic.x = CPieceManager::PIECEMANAGER_IMAGE_SELECT_MOVE_SIZE.x;
			if (GetDownL()) {		//subsutitute kind for divergence local variable
				selectMove = (tagPIECEMANAGERSELECTMOVEKIND)this->m_lpSelectMoveCh[i].m_kindNum;
			}
			break;
		}
		else {
			//change image
			this->m_lpSelectMoveCh[i].m_Ipic.x = 0;
		}
	}

	//if selectMove is PIECEMANAGER_DONT_SELECT_MOVE, back to PieceManagerUpdate in here
	if (selectMove == PIECEMANAGER_DONT_SELECT_MOVE) return;

	//diverge material of random to swap all piece process
	switch (selectMove) {
	case PIECEMANAGER_SELECT_MOVE_UP:								//all pieces move to up
		this->UpdateSwapAllLineToUp();
		break;		
	case PIECEMANAGER_SELECT_MOVE_RIGHT:							//all pieces move to right
		this->UpdateSwapAllRowToRight();
		break;
	case PIECEMANAGER_SELECT_MOVE_DOWN:								//all pieces move to down
		this->UpdateSwapAllLineToDown();
		break;
	case PIECEMANAGER_SELECT_MOVE_LEFT:								//all pieces move to left
		this->UpdateSwapAllRowToLeft();
		break;
	default:
		break;
	}
	this->m_swapFlag = PIECEMANAGER_VACANT;
}

/*
swap all line to left
*/
void	CPieceManager::UpdateSwapAllRowToLeft() {

	//starting the head of line's material
	int head = 0;
	int next = 0;

	for (int i = 0; i < this->m_Tarray.x; i++) {				//swap got line's coordinate with next line's coordinate
		for (int j = 0; j < this->m_Tarray.y; j++) {			//subsutitute coordinate of line for local variable
			head = (j * this->m_Tarray.x) + i;					//calculate head of material for line
			if (head == (j * this->m_Tarray.x) + (this->m_Tarray.x - 1)) break;//when head is last present row, next row 
			next = head + 1;									//next material
			if (this->m_lpCPiece[head].m_PieceFlag == PIECE_FLAG_FITTING_POSITION) continue;		//if piece's position is correct, jump to next material
			while (this->m_lpCPiece[next].m_PieceFlag == PIECE_FLAG_FITTING_POSITION) {
				if (next >= (j * this->m_Tarray.x) + (this->m_Tarray.x - 1)) {
					next = head;
					break;			//don't swapping process
				}
				next++;
			}
			//for get last origin coordinate
			TPOINT<int> lastPic = this->m_lpCPiece[head].m_Ipic;				//get origin coordinate
			int lastKind = this->m_lpCPiece[head].m_kindNum;					//get last kind
			//swap process, that got material and next material
			this->m_lpCPiece[head].m_Ipic = this->m_lpCPiece[next].m_Ipic;
			this->m_lpCPiece[next].m_Ipic = lastPic;
			this->m_lpCPiece[head].m_kindNum = this->m_lpCPiece[next].m_kindNum;
			this->m_lpCPiece[next].m_kindNum = lastKind;

		}
	}
}

/*
swap all line to right
*/
void	CPieceManager::UpdateSwapAllRowToRight() {

	//for get last coordinate
	float lastX = 0.0f;
	//starting the head of line's material
	int head = 0;
	int next = 0;

	for (int i = (this->m_Tarray.x - 1); i >= 0; i--) {			//swap got line's coordinate with next line's coordinate
		for (int j = 0; j < this->m_Tarray.y; j++) {			//subsutitute coordinate of line for local variable
			head = (j * this->m_Tarray.x) + i;					//calculate head of material for line
			if (head == (j * this->m_Tarray.x)) break;			//when head is last present row, next row 
			next = head - 1;									//next material
			if (this->m_lpCPiece[head].m_PieceFlag == PIECE_FLAG_FITTING_POSITION) continue;		//if piece's position is correct, jump to next material
			while (this->m_lpCPiece[next].m_PieceFlag == PIECE_FLAG_FITTING_POSITION) {
				if (next <= (j * this->m_Tarray.x)) {
					next = head;
					break;			//don't swapping process
				}
				next--;
			}
			//for get last origin coordinate
			TPOINT<int> lastPic = this->m_lpCPiece[head].m_Ipic;				//get origin coordinate
			int lastKind = this->m_lpCPiece[head].m_kindNum;					//get last kind
			//swap process, that got material and next material
			this->m_lpCPiece[head].m_Ipic = this->m_lpCPiece[next].m_Ipic;
			this->m_lpCPiece[next].m_Ipic = lastPic;
			this->m_lpCPiece[head].m_kindNum = this->m_lpCPiece[next].m_kindNum;
			this->m_lpCPiece[next].m_kindNum = lastKind;

		}
	}
}

/*
swap all line to up
*/
void	CPieceManager::UpdateSwapAllLineToUp() {

	//for get last coordinate
	float lastY = 0.0f;
	//starting the head of line's material
	int head = 0;
	int next = 0;

	for (int i = 0; i < this->m_Tarray.y; i++) {								//swap got line's coordinate with next line's coordinate
		for (int j = 0; j < this->m_Tarray.x; j++) {							//subsutitute coordinate of line for local variable
			head = j + (i * this->m_Tarray.x);									//subsutitute material of row
			if (head == j + ((this->m_Tarray.y - 1) * this->m_Tarray.x)) break;	//when head is last present row, next line
			next = head + this->m_Tarray.x;										//next material
			if (this->m_lpCPiece[head].m_PieceFlag == PIECE_FLAG_FITTING_POSITION) continue;		//if piece's position is correct, jump to next material
			while (this->m_lpCPiece[next].m_PieceFlag == PIECE_FLAG_FITTING_POSITION) {
				if (next >= j + ((this->m_Tarray.y - 1) * this->m_Tarray.x)) {
					next = head;
					break;			//don't swapping process
				}
				next += this->m_Tarray.x;
			}
			//for get last origin coordinate
			TPOINT<int> lastPic = this->m_lpCPiece[head].m_Ipic;				//get origin coordinate
			int lastKind = this->m_lpCPiece[head].m_kindNum;					//get last kind
			//swap process, that got material and next material
			this->m_lpCPiece[head].m_Ipic = this->m_lpCPiece[next].m_Ipic;
			this->m_lpCPiece[next].m_Ipic = lastPic;
			this->m_lpCPiece[head].m_kindNum = this->m_lpCPiece[next].m_kindNum;
			this->m_lpCPiece[next].m_kindNum = lastKind;
		}
	}
}

/*
swap all line to down
*/
void	CPieceManager::UpdateSwapAllLineToDown() {

	//for get last coordinate
	float lastY = 0.0f;
	//starting the head of line's material
	int head = 0;
	int next = 0;

	for (int i = (this->m_Tarray.y - 1); i >= 0; i--) {							//swap got line's coordinate with next line's coordinate
		for (int j = 0; j < this->m_Tarray.x; j++) {							//subsutitute coordinate of line for local variable
			head = j + (i * this->m_Tarray.x);									//subsutitute material of row
			if (head == j) break;												//when head is last present row, next line
			next = head - this->m_Tarray.x;										//next material
			if (this->m_lpCPiece[head].m_PieceFlag == PIECE_FLAG_FITTING_POSITION) continue;		//if piece's position is correct, jump to next material
			while (this->m_lpCPiece[next].m_PieceFlag == PIECE_FLAG_FITTING_POSITION) {
				if (next <= j ) {
					next = head;
					break;			//don't swapping process
				}
				next -= this->m_Tarray.x;
			}
			//for get last origin coordinate
			TPOINT<int> lastPic = this->m_lpCPiece[head].m_Ipic;				//get origin coordinate
			int lastKind = this->m_lpCPiece[head].m_kindNum;					//get last kind
			//swap process, that got material and next material
			this->m_lpCPiece[head].m_Ipic = this->m_lpCPiece[next].m_Ipic;
			this->m_lpCPiece[next].m_Ipic = lastPic;
			this->m_lpCPiece[head].m_kindNum = this->m_lpCPiece[next].m_kindNum;
			this->m_lpCPiece[next].m_kindNum = lastKind;
		}
	}
}


/*
for Debug
when pressed D key on keybord, complete puzzle
*/
void	CPieceManager::CompletePuzzleWithoutPlay() {
	if (JustKey('D')) {
		for (int i = 0; i < (this->m_Tarray.x * this->m_Tarray.y); i++) {
			this->m_lpCPiece[i].m_Ipic.x = ((i % this->m_Tarray.x) * this->m_pieceSize.x);
			this->m_lpCPiece[i].m_Ipic.y = ((i / this->m_Tarray.x) * this->m_pieceSize.y);
		}
	}
}