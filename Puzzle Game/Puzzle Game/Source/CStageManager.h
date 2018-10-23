#ifndef		CStageManager_H__
#define		CStageManager_H__

#include	<windows.h>
#include	"CStage.h"

//▼ 列挙型
//ステージ番号を扱う
enum	tagSTAGENUMBER{

	STAGE_1 ,
	STAGE_2 ,

};

//前方宣言

//▼ クラスの定義
// プレイヤークラスを作成する
class		CStageManager{

private:
	
	// 背景画像の要素番号保存用
	int		*m_lpStageImg;

	//ステージを扱う
	CStage	*m_lpCStage;

	//ステージ番号を扱う
	tagSTAGENUMBER		m_StageNum;


protected:


public:

	//▼ 静的メンバ変数の定義
	//ウィンドウハンドル
	static	HWND m_hWnd;
	//画像の使用枚数
	static const int	CSTAGEMANAGER_IMAGE_MAX;

	//画像サイズ
	//①背景
	static const int	CSTAGEMANAGER_BACK_IMAGE_WIDTH;		//幅
	static const int	CSTAGEMANAGER_BACK_IMAGE_HEIGHT;	//高さ

	//▼ 自作関数の定義
	//コンストラクタ
	CStageManager();
	//ディストラクタ
	~CStageManager();
	void	InitStageManager();				//初期化
	void	UpdateStageManager();			//更新	
	void	DrawStageManager();				//表示
	void	ReleaseStageManager();			//解放

	//ステージ番号を設定する
	void	SetStageNumber( tagSTAGENUMBER stageNum ){ this->m_StageNum = stageNum; }

	//ステージ番号を取得する
	tagSTAGENUMBER	GetStageNumber() const { return this->m_StageNum; }

	//▼ 純粋仮想関数のオーバーロード

};


#endif