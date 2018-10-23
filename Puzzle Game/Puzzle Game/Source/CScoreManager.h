#ifndef		CSCOREMANAGER_H__
#define		CSCOREMANAGER_H__


//▼ヘッダーをインクルード
#include	"CCharacter.h"
#include	<windows.h>

//kind of score
enum tagSCOREMANAGERNUMBERKIND {
	SCOREMANAGER_SCORENUMBER_COUNT_DOWN_TIME,
	SCOREMANAGER_KIND_OF_SCORENUMBER,
};

//front declaration
class CScore;
class CPieceManager;

//define class scoremanager
class CScoreManager{

private:

	//画像の要素を扱う
	int		*m_lpScoreManagerImg;
	//数字画像用
	CScore	*m_lpCScoreNumber;
	//for count
	int		m_millisecond;
	//for get present drawing number score
	static int	*m_lpPresentNumber;
	CPieceManager *m_lpCPieceManager;		//piecemanager class for get flag of complete

protected:
	
public:

	//▼ 静的メンバ変数の定義
	static const int SCOREMANAGER_SCORE_IMAGE_MAX;			//画像使用枚数
	//ウィンドウハンドル
	static HWND	m_hWnd;

	//▼プロトタイプ宣言
	//コンストラクタ
	CScoreManager() :
		m_millisecond(0)
	{}
	//ディストラクタ
	~CScoreManager();
	
	void InitScoreManager();					//初期化
	void UpdateScoreManager();					//更新
	void DrawScoreManager();					//表示
	void ReleaseScoreManager();					//解放
		
	//get selected value of score number
	int		GetScoreNumber(tagSCOREMANAGERNUMBERKIND kind)const { return this->m_lpPresentNumber[kind]; }
};


#endif