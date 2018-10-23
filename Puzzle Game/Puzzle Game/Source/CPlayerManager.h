#ifndef		CPLAYERMANAGER_H__
#define		CPLAYERMANAGER_H__


//▼ ヘッダーのインクルード
#include	<windows.h>
#include	"CCharacter.h"

//▼ 前方宣言
class	CPlayer;
class	CPieceManager;

//▼ クラスの定義
// プレイヤー管理クラス
class		CPlayerManager : public CCharacter{

private:
	
	//扱う画像要素番号
	int		*m_lpPlayerImg;

	//ピース管理用
	CPieceManager	*m_lpCPieceManager;

	//プレイヤークラス
	CPlayer		*m_lpCPlayer;

	//プレイヤーの座標を扱う
	static TPOINT<float>	m_ArrowPosition;


public:

	//▼ 静的メンバ変数の定義
	//ウィンドウハンドル
	static HWND m_hWnd;
	//画像の使用枚数
	static const int CPLAYERMANAGER_IMAGE_MAX;
	//プレイヤーのサイズ
	static const TPOINT<int> CPLAYERMANAGER_ARROW_SIZE;
	//プレイヤーの拡縮倍率
	static const float	CPLAYERMANAGER_DEFAULT_SCALE;


	//▼ プロトタイプ宣言
	//デフォルトコンストラクタ
	CPlayerManager() : m_lpPlayerImg(NULL) , m_lpCPieceManager(NULL) , m_lpCPlayer(NULL){}
	//ディストラクタ
	~CPlayerManager();
	
	//画像の解放
	void	ReleaseImagePlayerManager();

	//クラスの解放
	void	ReleaseClassPlayerManager();	

	//矢印の移動
	void	MoveArrow();
	//矢印の移動制限
	void	ConstrainArrowMove();

	//矢印の座標を返す
	TPOINT<float>	GetArrowPosition() const { return CPlayerManager::m_ArrowPosition; }

	//▼ 純粋仮想関数の定義
	void	InitChara();				//初期化
	void	UpdateChara();				//更新	
	void	DrawChara();				//表示
	void	ReleaseChara();				//解放

};

#endif