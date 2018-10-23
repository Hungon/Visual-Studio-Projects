#ifndef		CPlayer_H__
#define		CPlayer_H__

#include	"CCharacter.h"
#include	<windows.h>


//前方宣言
class	CPieceManager;
class	CPiece;

//▼ クラスの定義
// プレイヤークラスを作成する
class		CPlayer : virtual public CCharacter{

private:
	

protected:


public:

	//▼ 静的メンバ変数の定義
	

	//▼ 自作関数の定義
	//コンストラクタ
	CPlayer();
	//ディストラクタ
	~CPlayer();

	//▼ 純粋仮想関数のオーバーロード
	// CCharacterクラス
	void	InitChara();				//初期化
	void	UpdateChara();				//更新	
	void	DrawChara();				//表示
	void	ReleaseChara();				//解放

};


#endif