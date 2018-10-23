#ifndef		CPIECEMANAGER_H__
#define		CPIECEMANAGER_H__

//kind of puzzle's size first
enum tagPIECEMANAGERPUZZLESIZEKIND1 {
	PIECEMANAGER_PUZZLE_SIZE_1_ONE,
	PIECEMANAGER_PUZZLE_SIZE_1_TWO,
	PIECEMANAGER_PUZZLE_SIZE_1_THREE,
	PIECEMANAGER_PUZZLE_KIND_OF_SIZE_1,
};

//kind of puzzle's size second
enum tagPIECEMANAGERPUZZLESIZEKIND2 {
	PIECEMANAGER_PUZZLE_SIZE_2_ONE,
	PIECEMANAGER_PUZZLE_SIZE_2_TWO,
	PIECEMANAGER_PUZZLE_SIZE_2_THREE,
	PIECEMANAGER_PUZZLE_KIND_OF_SIZE_2,
};
//kind of puzzle
enum tagPIECEMANAGERPUZZLEKIND {
	PIECEMANAGER_PUZZLE_SIZE_1,				//8 X 4
	PIECEMANAGER_PUZZLE_SIZE_2,				//4 X 4
	PIECEMANAGER_KIND_OF_PUZZLE,			//kind
};
//kind of fram
enum tagPIECEMANAGERFRAMKIND {
	PIECEMANAGER_PUZZLE_FRAM_FOR_SIZE_1,
	PIECEMANAGER_PUZZLE_FRAM_FOR_SIZE_2,
	PIECEMANAGER_KIND_OF_PUZZLE_FRAM,
};

//kind of swapping process
enum tagPIECEMANAGERSWAPPROCESS {
	PIECEMANAGER_VACANT = -1,					//for make blank time
	PIECEMANAGER_ARRANGING_PIECE,				//during arranging pieces
	PIECEMANAGER_SELECT_PIECE,					//during select piece
	PIECEMANAGER_SELECTED_PIECE,				//select piece to replace
	PIECEMANAGER_SWAPPING_PIECE,				//during swapping process
	PIECEMANAGER_SWAP_ONE_ROW,					//swap the one row
	PIECEMANAGER_SWAP_ALL_LINE,					//swap all line
	PIECEMANAGER_KIND_OF_SWAPPING_PROCESS,		//kind fo process
};

//kind of select move
enum tagPIECEMANAGERSELECTMOVEKIND {
	PIECEMANAGER_DONT_SELECT_MOVE = -1,		//don't select
	PIECEMANAGER_SELECT_MOVE_RIGHT,			//right arrow
	PIECEMANAGER_SELECT_MOVE_DOWN,			//down arrow
	PIECEMANAGER_SELECT_MOVE_LEFT,			//left arrow
	PIECEMANAGER_SELECT_MOVE_UP,			//up arrow
	PIECEMANAGER_KIND_OF_SELECT_MOVE,		//kind
};

//�� �w�b�_�[�̃C���N���[�h
#include	<windows.h>
#include	"CCharacter.h"
#include	"CPiece.h"

//front define
class CPiece;
class CRisdonManager;

//�� �p�Y���s�[�X�N���X�̒�`
class		CPieceManager{

private:

	//�摜�p
	int		*m_lpPieceImg;
	int		*m_lpPieceSe;			//for effect sound
	//�s�[�X�������p
	CPiece	*m_lpCPiece;
	//�s�[�X�̔w�i������
	CPiece	*m_lpCBackImage;
	CPiece	*m_lpCompleteImage;		//for completed puzzle image
	CPiece	*m_lpSelectMoveCh;		//for select move
	//�s�[�X����ׂ鐔������
	TPOINT<int>		m_Tarray;
	//�s�[�X�����t���O
	tagPIECEMANAGERSWAPPROCESS	m_swapFlag;
	//�������s�[�X�̗v�f�ԍ�
	int		m_getPieceNum;
	//������s�[�X�̗v�f�ԍ�
	int		m_swapPieceNum;
	//���ׂ�s�[�X�̍��W������
	TPOINT<float>	m_pieceStartPosition;
	TPOINT<float>	m_selectPieceManagerFpos;
	//���ׂ�s�[�X�̃T�C�Y������
	TPOINT<int>		m_pieceSize;
	//�S�̂̃T�C�Y
	TPOINT<int>		m_WholeSize;
	//�����摜�\����
	char	m_ScaleCnt;
	//flag of complete
	static bool m_CompleteFlag;
	//for arragement piece
	int	m_ArrangementPieceCountTime;
	//for correct coutn
	int	m_CorrectCount;
	//using kind of puzzle
	tagPIECEMANAGERPUZZLEKIND	m_PuzzleKind;
	//for get risdon's setting
	CRisdonManager	*m_lpRisdonManager;
protected:


public:

	//�� �ÓI�����o�ϐ��̒�`
	static	HWND m_hWnd;							//�E�B���h�E�n���h��
	static const int	CPIECEMANAGER_IMAGE_MAX;	//�摜�̎g�p����
	static const int	CPIECEMANAGER_BACKIMAGE_MAX;//�w�i�摜�̎g�p����
	static const int	CPIECEMANAGER_KIND_OF_EFFECT_SOUND;		//for effect sound
	static const float	PIECEMANAGER_SELECT_ALPHA;	//�I�𒆂̃s�[�X�̓��ߔ{��
	static const int	STAGE_WIDTH;				//�X�e�[�W�̕�
	static const int	STAGE_HEIGHT;				//�X�e�[�W�̍���

	//kind of puzzle's size
	static const TPOINT<int>		PIECEMANAGER_IMAGE_PUZZLE_WHOLE_SIZE_1;
	static const TPOINT<int>		PIECEMANAGER_IMAGE_PUZZLE_WHOLE_SIZE_2;
	
	//each piece's setting
	static const TPOINT<int>	PIECEMANAGER_IMAGE_PIECE_SIZE_1;
	static const TPOINT<int>	PIECEMANAGER_IMAGE_PIECE_SIZE_2;
	static const float			PIECEMANAGER_IMAGE_SELECTING_PIECE_ALPHA;

	//fram image
	static const TPOINT<int>	PIECEMANAGER_IMAGE_FRAM_1_DEFAULT_SIZE;
	static const TPOINT<int>	PIECEMANAGER_IMAGE_FRAM_2_DEFAULT_SIZE;
	static const float			PIECEMANANGER_IMAGE_FRAM_DEFAULT_SCALE;

	//�B �������ɏo���uCongratulation!�v
	static const int	PIECE_CONGRATULATION_DEFAULT_WIDTH;	//��
	static const int	PIECE_CONGRATULATION_DEFAULT_HEIGHT;//����
	static const float	PIECE_CONGRATULATION_DEFAULT_SCALE;	//�g�k�{��
	static const float	PIECE_CONGRATULATION_DEFAULT_ADD_SCALE;	//�g�k�{�������l
	//complete image
	static const TPOINT<float>	PIECEMANAGER_IMAGE_COMPLETE_IMAGE_COORDINATE;
	static const float		PIECEMANAGER_IMAGE_COMPLETE_IMAGE_DEFAULT_SCALE;

	//arrangement time
	static const int	PIECEMANAGER_ARRANGEMENT_PIECES_TIME;

	//kind of file for arrangement pieces
	static const int	PIECEMANAGER_KIND_OF_FILE_FOR_8_X_4;
	static const int	PIECEMANAGER_KIND_OF_FILE_FOR_4_X_4;

	//don't execution time
	static const int	PIECEMANAGER_DONT_PROCESS_TIME;

	//set select move
	static const TPOINT<int>	PIECEMANAGER_IMAGE_SELECT_MOVE_SIZE;
	static const TPOINT<float>	PIECEMANAGER_IMAGE_SELECT_MOVE_COORDINATE;
	static const float			PIECEMANAGER_IMAGE_SELECT_MOVE_DEFAULT_SCALE;

	//�f�t�H���g�R���X�g���N�^
	CPieceManager();	
	//�f�B�X�g���N�^
	~CPieceManager();
	
	//������
	void	InitPieceManager();
	
	//�X�V
	bool	UpdatePieceManager();
	
	//�\��
	void	DrawPieceManager();

	//���
	void	ReleasePieceManager();

	//�s�[�X���������̏�����
	void    InitSwapPieceManager();
	
	//�s�[�X�I�������̍X�V
	void    UpdateSelectSwapPieceManager();
	
	//�s�[�X�������̏���
	void    UpdateDoSwapPieceManager();

	//���ׂ��s�[�X�̈�ԍ���i�n�߂ɕ��ׂ���s�[�X�j�̍��W�擾
	TPOINT<float>	GetPiecePosition(){ return this->m_pieceStartPosition; }
	//���ׂ�s�[�X�̃T�C�Y���擾
	TPOINT<int>		GetPieceSize(){ return this->m_pieceSize; }
	//�s�[�X�̑S�̂̃T�C�Y���擾
	TPOINT<int>	GetWholePieceSize(){ return this->m_WholeSize; }

	/*
	************************************************************************************
	�Q�[���N���A�����ׂ�֐�
	�s�[�X�̕��т���������E���܂łP�������ƂȂ��Ă�����Atrue��Ԃ�
	************************************************************************************
	*/
	bool	CheckExactlyArrayPiece();

	/*
	initialize arrangemnent piece and draw pieces
	*/
	void	ArrangePiece();
	/*
	swap one row and select row
	*/
	void	InitializeSwapOneRow();
	/*
	swap all line to right
	*/
	void	UpdateSwapAllRowToRight();
	/*
	swap all line to left
	*/
	void	UpdateSwapAllRowToLeft();
	/*
	swap all line to up
	*/
	void	UpdateSwapAllLineToUp();
	/*
	swap all line to down
	*/
	void	UpdateSwapAllLineToDown();
	/*
	initialize the swapping process
	*/
	void	InitializeSwapAllPieceProcess();
	//get flag of complete
	bool	GetCompletePuzzle() const { return this->m_CompleteFlag; }
	/*
	for Debug
	when pressed D key on keybord, complete puzzle
	*/
	void	CompletePuzzleWithoutPlay();
};


#endif